#include "object.hpp"
#include "game.hpp"

game_object::game_object(object_type type) {
	set_type(type);
	y_off = static_cast<float>(std::rand() % 8 - std::rand() % 16);
	y_off_dir = std::rand() % 2 != 0;
}

void game_object::set_type(object_type type_) {
	type = type_;
	switch (type) {
		using enum object_type;
	
	case player:
		transform.scale = { 32.0f, 48.0f };
		collision = { { 0.0f, 32.0f }, { 32.0f, 16.0f } };
		tex_coords = { 0.0f, 0.0f };
		break;

	case bush:
		transform.scale = { 32.0f, 32.0f };
		collision = { { 0.0f, 16.0f }, { 32.0f, 16.0f } };
		tex_coords = { 0.0f, 160.0f };
		break;

	case tree:
		transform.scale = { 32.0f, 32.0f };
		collision = { { 0.0f, 16.0f }, { 32.0f, 16.0f } };
		tex_coords = { 32.0f, 160.0f };
		break;

	case big_rock:
		transform.scale = { 32.0f, 32.0f };
		collision = { { 0.0f, 16.0f }, { 32.0f, 16.0f } };
		tex_coords = { 64.0f, 160.0f };
		break;

	case tree_stump:
		transform.scale = { 32.0f, 32.0f };
		collision = { { 0.0f, 16.0f }, { 32.0f, 16.0f } };
		tex_coords = { 96.0f, 160.0f };
		break;

	case big_red_mushroom:
		tex_coords = { 128.0f, 144.0f };
		break;

	case fat_blue_mushroom:
		tex_coords = { 240.0f, 144.0f };
		break;

	case star_anise:
		tex_coords = { 320.0f, 144.0f };
		break;

	case st_johns_wort:
		tex_coords = { 336.0f, 144.0f };
		break;

	case spearmint:
		tex_coords = { 352.0f, 144.0f };
		break;

	case solomons_seal:
		tex_coords = { 368.0f, 144.0f };
		break;

	case thyme:
		tex_coords = { 384.0f, 144.0f };
		break;

	case valerian:
		tex_coords = { 400.0f, 144.0f };
		break;

	case wormwood:
		tex_coords = { 416.0f, 144.0f };
		break;

	case sage:
		tex_coords = { 432.0f, 144.0f };
		break;

	case patchouli:
		tex_coords = { 448.0f, 144.0f };
		break;

	case hyssop:
		tex_coords = { 464.0f, 144.0f };
		break;

	case majoram:
		tex_coords = { 480.0f, 144.0f };
		break;

	case horehound:
		tex_coords = { 496.0f, 144.0f };
		break;

	case house_exterior:
		transform.scale = { 304.0f, 208.0f };
		collision = { { 32.0f, 32.0f }, { 288.0f, 288.0f } };
		tex_coords = { 320.0f, 304.0f };
		break;

	case house_interior:
		transform.scale = { 288.0f, 256.0f };
		collision = { { 0.0f, 0.0f }, { 288.0f, 256.0f } };
		tex_coords = { 0.0f, 256.0f };
		break;

	case brewing_stand:
		transform.scale = { 48.0f, 32.0f };
		collision = { { 0.0f, 16.0f }, { 48.0f, 16.0f } };
		tex_coords = { 48.0f, 192.0f };
		break;

	case bad_potion:
		transform.scale = { 16.0f, 16.0f };
		tex_coords = { 96.0f, 224.0f };
		break;

	case bald_cure:
		transform.scale = { 16.0f, 16.0f };
		tex_coords = { 0.0f, 224.0f };
		break;

	case barbarian_mix:
		transform.scale = { 16.0f, 16.0f };
		tex_coords = { 16.0f, 224.0f };
		break;

	case colorful_potion:
		transform.scale = { 16.0f, 16.0f };
		tex_coords = { 32.0f, 224.0f };
		break;

	case thick_mixture:
		transform.scale = { 16.0f, 16.0f };
		tex_coords = { 48.0f, 224.0f };
		break;

	case youth_potion:
		transform.scale = { 16.0f, 16.0f };
		tex_coords = { 64.0f, 224.0f };
		break;

	case eagle_potion:
		transform.scale = { 16.0f, 16.0f };
		tex_coords = { 80.0f, 224.0f };
		break;

	case balding_monk:
		tex_coords = { 32.0f, 0.0f };
		customer = true;
		break;

	case afro_monk:
		tex_coords = { 64.0f, 0.0f };
		break;

	case fat_virgin:
		tex_coords = { 96.0f, 0.0f };
		customer = true;
		break;

	case fit_chad:
		tex_coords = { 128.0f, 0.0f };
		break;

	case gray_hair_lumberjack:
		tex_coords = { 160.0f, 0.0f };
		customer = true;
		break;

	case blonde_hair_lumberjack:
		tex_coords = { 192.0f, 0.0f };
		break;

	case thin_hair_lady:
		tex_coords = { 224.0f, 0.0f };
		customer = true;
		break;

	case thick_hair_lady:
		tex_coords = { 256.0f, 0.0f };
		break;

	case saggy_tits_lady:
		tex_coords = { 288.0f, 0.0f };
		customer = true;
		break;

	case big_booba_lady:
		tex_coords = { 320.0f, 0.0f };
		break;

	case cant_see_shit_granny:
		tex_coords = { 352.0f, 0.0f };
		customer = true;
		break;

	case eagle_eyed_granny:
		tex_coords = { 384.0f, 0.0f };
		break;

	case fat_monk:
		tex_coords = { 416.0f, 0.0f };
		customer = true;
		break;

	case fit_monk:
		tex_coords = { 448.0f, 0.0f };
		break;

	case gray_hair_monk:
		tex_coords = { 480.0f, 0.0f };
		customer = true;
		break;

	case brown_hair_monk:
		tex_coords = { 512.0f, 0.0f };
		break;

	case weak_monk:
		tex_coords = { 544.0f, 0.0f };
		customer = true;
		break;

	case strong_monk:
		tex_coords = { 576.0f, 0.0f };
		break;

	case poor_sight_monk:
		tex_coords = { 416.0f, 48.0f };
		customer = true;
		break;

	case good_sight_monk:
		tex_coords = { 448.0f, 48.0f };
		break;

	case balding_man:
		tex_coords = { 480.0f, 48.0f };
		customer = true;
		break;

	case afro_man:
		tex_coords = { 512.0f, 48.0f };
		break;

	case gray_hair_man:
		tex_coords = { 544.0f, 48.0f };
		customer = true;
		break;

	case blonde_hair_man:
		tex_coords = { 576.0f, 48.0f };
		break;

	case weak_man:
		tex_coords = { 256.0f, 96.0f };
		customer = true;
		break;

	case strong_man:
		tex_coords = { 288.0f, 96.0f };
		break;

	case poor_sight_man:
		tex_coords = { 320.0f, 96.0f };
		customer = true;
		break;

	case good_sight_man:
		tex_coords = { 352.0f, 96.0f };
		break;

	case balding_lumberjack:
		tex_coords = { 384.0f, 96.0f };
		customer = true;
		break;

	case afro_lumberjack:
		tex_coords = { 416.0f, 96.0f };
		break;

	case fat_lumberjack:
		tex_coords = { 448.0f, 96.0f };
		customer = true;
		break;

	case fit_lumberjack:
		tex_coords = { 480.0f, 96.0f };
		break;

	case fat_lumberjack_2:
		tex_coords = { 512.0f, 96.0f };
		customer = true;
		break;

	case fit_lumberjack_2:
		tex_coords = { 544.0f, 96.0f };
		break;

	case poor_sight_lumberjack:
		tex_coords = { 576.0f, 96.0f };
		customer = true;
		break;

	case good_sight_lumberjack:
		tex_coords = { 608.0f, 96.0f };
		break;

	case fat_lady:
		tex_coords = { 128.0f, 160.0f };
		customer = true;
		break;

	case healthy_lady:
		tex_coords = { 160.0f, 160.0f };
		break;

	case gray_hair_lady:
		tex_coords = { 192.0f, 160.0f };
		customer = true;
		break;

	case blonde_hair_lady:
		tex_coords = { 224.0f, 160.0f };
		break;

	case old_lady:
		tex_coords = { 256.0f, 160.0f };
		customer = true;
		break;

	case young_lady:
		tex_coords = { 320.0f, 160.0f };
		break;

	case poor_sight_lady:
		tex_coords = { 288.0f, 160.0f };
		customer = true;
		break;

	case good_sight_lady:
		tex_coords = { 320.0f, 160.0f };
		break;

	case balding_lady:
		tex_coords = { 352.0f, 160.0f };
		customer = true;
		break;

	case hairful_lady:
		tex_coords = { 384.0f, 160.0f };
		break;

	case fat_lady_2:
		tex_coords = { 416.0f, 160.0f };
		customer = true;
		break;

	case healthy_lady_2:
		tex_coords = { 448.0f, 160.0f };
		break;

	case gray_hair_lady_2:
		tex_coords = { 480.0f, 160.0f };
		customer = true;
		break;

	case brown_hair_lady:
		tex_coords = { 512.0f, 160.0f };
		break;

	case poor_sight_lady_2:
		tex_coords = { 544.0f, 160.0f };
		customer = true;
		break;

	case good_sight_lady_2:
		tex_coords = { 512.0f, 160.0f };
		break;

	case thin_hair_lady_2:
		tex_coords = { 576.0f, 160.0f };
		customer = true;
		break;

	case thick_hair_lady_2:
		tex_coords = { 608.0f, 160.0f };
		break;

	case fat_lady_3:
		tex_coords = { 128.0f, 208.0f };
		customer = true;
		break;

	case healthy_lady_3:
		tex_coords = { 160.0f, 208.0f };
		break;

	case gray_hair_lady_3:
		tex_coords = { 192.0f, 208.0f };
		customer = true;
		break;

	case black_hair_lady:
		tex_coords = { 224.0f, 208.0f };
		break;

	case poor_sight_lady_3:
		tex_coords = { 256.0f, 208.0f };
		customer = true;
		break;

	case good_sight_lady_3:
		tex_coords = { 288.0f, 208.0f };
		break;
	}

	if (is_ingredient(type)) {
		transform.scale = { 16.0f, 16.0f };
		collision = { { 0.0f, 0.0f }, { 16.0f, 16.0f } };
		direction = std::rand() % 2 != 0 ? object_direction::left : object_direction::right;
		interactive = true;
	}

	if (static_cast<int>(type) >= static_cast<int>(object_type::balding_monk) && static_cast<int>(type) <= static_cast<int>(object_type::good_sight_lady_3)) {
		transform.scale = { 32.0f, 48.0f };
		collision = { { 0.0f, 32.0f }, { 32.0f, 16.0f } };
	}
}

void game_object::update(game_program& game) {
	collision_transform.position = transform.position + collision.position;
	collision_transform.scale = collision.scale;
}

void player_object::update(game_program& game) {
	game_object::update(game);
	float speed{ 1.0f };
	auto& keyboard = game.window->keyboard;
	const auto w = keyboard.is_key_down(nfwk::key::w);
	const auto a = keyboard.is_key_down(nfwk::key::a);
	const auto s = keyboard.is_key_down(nfwk::key::s);
	const auto d = keyboard.is_key_down(nfwk::key::d);
	auto new_position = transform.position;
	if (a != d) {
		if (a) {
			new_position.x -= speed;
			direction = object_direction::left;
		} else if (d) {
			new_position.x += speed;
			direction = object_direction::right;
		}
		speed /= 2.0f;
	}
	if (w != s) {
		if (w) {
			new_position.y -= speed;
		} else if (s) {
			new_position.y += speed;
		}
	}
	if (w || a || s || d) {
		nfwk::transform2 new_collision_transform{ new_position + collision.position, collision.scale };
		bool collides{ false };
		for (const auto& world_collision : game.world->collisions) {
			if (world_collision.collides_with(new_collision_transform)) {
				collides = true;
				break;
			}
		}
		if (!collides) {
			transform.position = new_position;
		}
	}
	if (keyboard.is_key_down(nfwk::key::space)) {
		game.world->pickup(collision_transform);
	}
}

void player_object::remove_item(object_type item) {
	for (std::size_t i{ 0 }; i < items.size(); i++) {
		if (items[i] == item) {
			items.erase(items.begin() + i);
			break;
		}
	}
}

void player_object::learn_cure(object_type cure) {
	if (cure == object_type::bad_potion) {
		return;
	}
	if (std::ranges::find(unlocked_cures, cure) == unlocked_cures.end()) {
		unlocked_cures.push_back(cure);
	}
}

customer_object::customer_object(object_type type) : game_object{ type } {
	set_type(type);
}

void customer_object::update(game_program& game) {
	game_object::update(game);
	
	if (state == customer_state::waiting) {
		int index{ 0 };
		for (auto& queue : game.world->customer_queue_positions) {
			if (!queue.customer && index < queue_position.value_or(1000)) {
				if (queue_position.has_value()) {
					game.world->customer_queue_positions[queue_position.value()].customer = nullptr;
				}
				queue_position = index;
				target_position = queue.position;
				queue.customer = this;
				break;
			}
			index++;
		}
	} else if (state == customer_state::leaving) {
		target_position = game.world->customer_queue_positions.back().position;
	}

	if (target_position) {
		reached_target = false;
		constexpr float speed{ 0.5f };
		const nfwk::vector2f delta{
			target_position->x - collision_transform.position.x,
			target_position->y - collision_transform.position.y
		};
		const bool reached_x{ std::abs(delta.x) < speed };
		const bool reached_y{ std::abs(delta.y) < speed };
		if (!reached_x) {
			transform.position.x += delta.x > 0.0 ? speed : -speed;
		}
		if (!reached_y) {
			transform.position.y += delta.y > 0.0 ? speed : -speed;
		}
		reached_target = reached_x && reached_y;
		if (reached_target) {
			target_position = std::nullopt;
		}
	}
	ready_for_treatment = reached_target && state == customer_state::waiting;
}

void customer_object::set_type(object_type type) {
	game_object::set_type(type);
	switch (type) {
		using enum object_type;
	case balding_monk:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::bald_cure;
		break;
	case afro_monk:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case fat_virgin:
		greeting = "Ugh, I don't feel too well these days.\nWhat do you have for me?";
		wanted_treatment = object_type::barbarian_mix;
		break;
	case fit_chad:
		greeting = "I feel better than ever! Thank you so much.";
		break;

	case gray_hair_lumberjack:
		greeting = "Hmm. Hello there.";
		wanted_treatment = object_type::colorful_potion;
		break;
	case blonde_hair_lumberjack:
		greeting = "This does look better on me. Farewell.";
		break;

	case thin_hair_lady:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::thick_mixture;
		break;
	case thick_hair_lady:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case saggy_tits_lady:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::youth_potion;
		break;
	case big_booba_lady:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case cant_see_shit_granny:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::eagle_potion;
		break;
	case eagle_eyed_granny:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case fat_monk:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::barbarian_mix;
		break;
	case fit_monk:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case gray_hair_monk:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::colorful_potion;
		break;
	case brown_hair_monk:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case weak_monk:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::barbarian_mix;
		break;
	case strong_monk:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case poor_sight_monk:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::eagle_potion;
		break;
	case good_sight_monk:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case balding_man:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::bald_cure;
		break;
	case afro_man:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case gray_hair_man:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::colorful_potion;
		break;
	case blonde_hair_man:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case weak_man:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::barbarian_mix;
		break;
	case strong_man:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case poor_sight_man:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::eagle_potion;
		break;
	case good_sight_man:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case balding_lumberjack:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::bald_cure;
		break;
	case afro_lumberjack:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case fat_lumberjack:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::barbarian_mix;
		break;
	case fit_lumberjack:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case fat_lumberjack_2:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::barbarian_mix;
		break;
	case fit_lumberjack_2:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case poor_sight_lumberjack:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::eagle_potion;
		break;
	case good_sight_lumberjack:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case fat_lady:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::youth_potion;
		break;
	case healthy_lady:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case gray_hair_lady:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::colorful_potion;
		break;
	case blonde_hair_lady:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case old_lady:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::youth_potion;
		break;
	case young_lady:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case poor_sight_lady:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::eagle_potion;
		break;
	case good_sight_lady:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case balding_lady:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::bald_cure;
		break;
	case hairful_lady:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case fat_lady_2:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::youth_potion;
		break;
	case healthy_lady_2:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case gray_hair_lady_2:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::colorful_potion;
		break;
	case brown_hair_lady:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case poor_sight_lady_2:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::eagle_potion;
		break;
	case good_sight_lady_2:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case thin_hair_lady_2:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::thick_mixture;
		break;
	case thick_hair_lady_2:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case fat_lady_3:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::youth_potion;
		break;
	case healthy_lady_3:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case gray_hair_lady_3:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::colorful_potion;
		break;
	case black_hair_lady:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;

	case poor_sight_lady_3:
		greeting = "Good day. I am not how I was in my youth...\nDo you have the stuff for me?";
		wanted_treatment = object_type::eagle_potion;
		break;
	case good_sight_lady_3:
		greeting = "Wow. You are amazing!\nThis is exactly what I was looking for.";
		break;
	}
}

bool customer_object::treat() {
	if (wanted_treatment == treated_with) {
		set_type(static_cast<object_type>(static_cast<int>(type) + 1)); // kek
		return true;
	}
	return false;
}

void brewing_stand_object::set_result_from_items() {
	for (int treatment_type{ static_cast<int>(object_type::bald_cure) }; treatment_type <= static_cast<int>(object_type::eagle_potion); treatment_type++) {
		auto needed = game_object::ingredients_for_treatment(static_cast<object_type>(treatment_type));
		for (int i{ 0 }; i < static_cast<int>(needed.size()); i++) {
			for (auto& item : items) {
				if (item.type == needed[i]) {
					needed.erase(needed.begin() + i);
					i--;
					break;
				}
			}
		}
		if (needed.empty()) {
			items.clear();
			result = { static_cast<object_type>(treatment_type) };
		}
	}
	if (!items.empty()) {
		items.clear();
		result = { object_type::bad_potion };
	}
}

