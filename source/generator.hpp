#pragma once
#if 0
#include "graphics/tiles/tiles.hpp"
#include "noise.hpp"

class game_world;

class world_generator_settings {
public:

	struct noise_settings {
		int octaves{};
		float persistence{};
		float scale{};
		unsigned long long seed{};
	};

	noise_settings biome{ 7, 0.1f, 0.002f, 1600 };
	noise_settings river{ 5, 0.07f, 0.025f, 99000 };
	noise_settings water{ 5, 0.07f, 0.025f, 58000 };

	float water_threshold{ 0.4f };

};

class world_generator {
public:

	world_generator_settings settings;

	world_generator(game_world& world, nfwk::tiles::map& tiles, const world_generator_settings& settings);

	void generate(int depth, int x, int y, int x2, int y2);

private:

	float noise_at(const nfwk::simplex_noise_map& map, const world_generator_settings::noise_settings& settings, int x, int y) const;

	game_world& world;
	nfwk::tiles::map& tiles;
	nfwk::simplex_noise_map water_noise_map;
	nfwk::simplex_noise_map river_noise_map;
	nfwk::simplex_noise_map biome_noise_map;
	nfwk::event_listener new_chunk_event;

	bool spawned_player{ false };


};
#endif