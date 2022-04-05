#include "generator.hpp"
#include "world.hpp"
#include "random.hpp"
#if 0

world_generator::world_generator(game_world& world, nfwk::tiles::map& tiles_, const world_generator_settings& settings_)
	: world{ world }, settings{ settings_ }, tiles{ tiles_ },
	water_noise_map{ settings_.water.seed },
	river_noise_map{ settings_.water.seed },
	biome_noise_map{ settings_.biome.seed } {
	new_chunk_event = tiles.on_load_area.listen([this](int depth, int x, int y, int width, int height) {
		generate(depth, x, y, x + width, y + height);
	});
}

void world_generator::generate(const int depth, const int x1, const int y1, const int x2, const int y2) {
	tiles.change_layer(depth);

	constexpr int ground_dirt{ 1 }, dirt_path{ 2 }, stone_path{ 3 }, sand{ 4 }, water{ 5 }, dark_grass{ 6 }, light_grass{ 7 }, swamp_water{ 8 };

	nfwk::random_number_generator rng;

	auto x = x1;
	auto y = y1;
	for (; x < x2; x++) {
		int i{ y };
		for (; y < y2; y++) {

			float biome_noise = noise_at(biome_noise_map, settings.biome, x, y);
			float river_noise = noise_at(river_noise_map, settings.river, x, y);
			float water_noise = noise_at(water_noise_map, settings.water, x, y);

			bool is_swamp{ biome_noise > 0.3f };

			const auto w = is_swamp ? swamp_water : water;
			const auto g = is_swamp ? dark_grass : light_grass;

			if (river_noise > settings.water_threshold || water_noise > settings.water_threshold) {
				tiles.set(x, y, w);
				continue;
			}

			tiles.set(x, y, g);

			if (x % 2 == 0 && y % 2 == 0) {
				if (rng.chance(0.01f)) {
					world.spawn_object(static_cast<float>(x * 16), static_cast<float>(y * 16), object_type::bush);
				} else if (rng.chance(0.02f)) {
					world.spawn_object(static_cast<float>(x * 16), static_cast<float>(y * 16), object_type::tree);
				} else if (rng.chance(0.01f)) {
					world.spawn_object(static_cast<float>(x * 16), static_cast<float>(y * 16), object_type::big_rock);
				} else if (rng.chance(0.01f)) {
					world.spawn_object(static_cast<float>(x * 16), static_cast<float>(y * 16), object_type::tree_stump);
				} else if (rng.chance(0.01f)) {
					world.spawn_object(static_cast<float>(x * 16), static_cast<float>(y * 16), object_type::big_red_mushroom);
				} else if (rng.chance(0.01f)) {
					world.spawn_object(static_cast<float>(x * 16), static_cast<float>(y * 16), object_type::fat_blue_mushroom);
				}
			}

		}
		y = i;
	}
}

float world_generator::noise_at(const nfwk::simplex_noise_map& map, const world_generator_settings::noise_settings& settings, int x, int y) const {
	return map.get(settings.octaves, settings.persistence, settings.scale, static_cast<float>(x), static_cast<float>(y));
}
#endif