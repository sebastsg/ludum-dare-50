#include "world.hpp"
#include "transform.hpp"
#include "graphics/window.hpp"
#include "graphics/tiles/layer.hpp"
#include "game.hpp"
#include "random.hpp"

/*



bugs to fix:

- customer/brewing ui should disappear if player walks too far away (or prevent walking altogether)
- clickong on customer/brewing stand should only work if player is close enough
- customers should move in a path and not just fucking weird like it is now



*/

game_world::game_world(game_program& game_) : game{ game_ } {
	sprite_shader = game.assets->find<nfwk::shader>("shaders/sprite");
	tilesheet = game.assets->find<nfwk::texture>("textures/tiles");
	map_texture = game.assets->find<nfwk::texture>("textures/map");

	spawn_object(96.0f, 288.0f, object_type::house_exterior);
	spawn_object(112.0f, 240.0f, object_type::house_interior);
	spawn_object(162.0f, 540.0f, object_type::player);
	spawn_object(224.0f + 96.0f, 288.0f, object_type::brewing_stand);

	player.items.push_back(object_type::barbarian_mix);
	player.items.push_back(object_type::colorful_potion);
	player.items.push_back(object_type::star_anise);
	player.items.push_back(object_type::thyme);

	new_customer_timer.start();
	ingredient_grow_timer.start();

	customer_spawn_position = { 256.0f, 720.0f };
	customer_queue_positions.push_back({ { 96.0f + 124.0f, 340.0f }, nullptr });
	customer_queue_positions.push_back({ { 96.0f + 101.0f, 380.0f }, nullptr });
	customer_queue_positions.push_back({ { 96.0f + 92.0f, 420.0f }, nullptr });
	customer_queue_positions.push_back({ { 96.0f + 85.0f, 460.0f }, nullptr });
	customer_queue_positions.push_back({ { 96.0f + 84.0f, 500.0f }, nullptr });
	customer_queue_positions.push_back({ { 96.0f + 74.0f, 540.0f }, nullptr });

	for (int type{ static_cast<int>(object_type::bald_cure) }; type <= static_cast<int>(object_type::eagle_potion); type++) {
		object_name_textures[static_cast<object_type>(type)] = std::make_unique<nfwk::texture>(game.font->render(game_object::get_name(static_cast<object_type>(type)), 0xffffffff), nfwk::scale_option::nearest_neighbor, false);
	}
	for (int type{ static_cast<int>(object_type::star_anise) }; type <= static_cast<int>(object_type::horehound); type++) {
		object_name_textures[static_cast<object_type>(type)] = std::make_unique<nfwk::texture>(game.font->render(game_object::get_name(static_cast<object_type>(type)), 0xffffffff), nfwk::scale_option::nearest_neighbor, false);
	}

	player.learn_cure(object_type::bald_cure);
	player.learn_cure(object_type::barbarian_mix);
	player.learn_cure(object_type::colorful_potion);
	player.learn_cure(object_type::eagle_potion);
	player.learn_cure(object_type::thick_mixture);
	player.learn_cure(object_type::youth_potion);

	add_collision(114.0f, 290.0f, 400.0f, 300.0f);
	add_collision(114.0f, 290.0f, 128.0f, 496.0f);
	add_collision(114.0f, 478.0f, 160.0f, 496.0f);
	add_collision(218.0f, 478.0f, 271.0f, 496.0f);
	add_collision(256.0f, 414.0f, 271.0f, 496.0f);
	add_collision(256.0f, 414.0f, 400.0f, 430.0f);
	add_collision(380.0f, 290.0f, 400.0f, 430.0f);
	add_collision(256.0f, 300.0f, 270.0f, 366.0f);
	add_collision(270.0f, 444.0f, 400.0f, 464.0f);
	add_collision(215.0f, 550.0f, 540.0f, 570.0f);

	add_collision(688.0f, 0.0f, 780.0f, 176.0f);
	add_collision(704.0f, 176.0f, 800.0f, 540.0f);
	add_collision(690.0f, 534.0f, 780.0f, 780.0f);
	add_collision(675.0f, 785.0f, 767.0f, 925.0f);

	add_ingredient_spawn(70.0f, 650.0f, 500.0f, 874.0f);
	add_ingredient_spawn(550.0f, 680.0f, 660.0f, 910.0f);
	add_ingredient_spawn(300.0f, 480.0f, 540.0f, 540.0f);
	add_ingredient_spawn(128.0f, 32.0f, 680.0f, 240.0f);
	add_ingredient_spawn(418.0f, 240.0f, 680.0f, 540.0f);

	mouse_press_listener = game.window->mouse.press.listen([this](nfwk::mouse::button button) {
		if (button == nfwk::mouse::button::left) {
			if (customer_ui) {
				customer_ui->on_mouse_press();
			}

			// start helping customer
			if (auto customer = customer_queue_positions[0].customer; customer && customer->ready_for_treatment) {
				if (customer->transform.collides_with(camera.mouse_position(game.window->mouse))) {
					if (!customer_ui) {
						customer_ui = std::make_unique<customer_chat_ui>(game, ui_type::treatment, customer);
					}
					return;
				}
			}

			// interact with brewing stand
			if (brewing_stand.transform.collides_with(camera.mouse_position(game.window->mouse))) {
				if (brewing_stand.result.type != object_type::no_cure) {
					player.items.push_back(brewing_stand.result.type);
					player.learn_cure(brewing_stand.result.type);
					brewing_stand.result = object_type::no_cure;
				} else if (!customer_ui && !brewing_stand.brew_timer.is_running() && brewing_stand.result.type == object_type::no_cure) {
					customer_ui = std::make_unique<customer_chat_ui>(game, ui_type::brewing, nullptr);
				}
				return;
			}
		}
	});
}

game_world::~game_world() {

}

void game_world::update() {
	camera.target_chase_speed = 0.5f;
	camera.target_chase_aspect = { 2.0f, 2.0f };
	camera.target = &player.transform;

	inside_house = false;
	auto floor_transform = house_interior.transform;
	floor_transform.position.x += 16.0f;
	floor_transform.position.y += 64.0f;
	floor_transform.scale.x = 256.0f;
	floor_transform.scale.y = 112.0f;
	if (floor_transform.collides_with(player.collision_transform)) {
		inside_house = true;
	} else {
		floor_transform.position.y += floor_transform.scale.y;
		floor_transform.scale.x = 128.0f;
		floor_transform.scale.y = 64.0f;
		if (floor_transform.collides_with(player.collision_transform)) {
			inside_house = true;
		}
	}

	camera.update();
	hud_camera.update();

	if (ingredient_grow_timer.milliseconds() > pickups.size() * 100) {
		nfwk::random_number_generator rng;
		for (const auto& area : ingredient_spawn_areas) {
			if (rng.chance(0.5f)) {
				continue;
			}
			const auto x = rng.next<float>(area.position.x, area.position.x + area.scale.x);
			const auto y = rng.next<float>(area.position.y, area.position.y + area.scale.y);
			const auto type = static_cast<object_type>(rng.next<int>(11, 22));
			spawn_object(x, y, type);
			ingredient_grow_timer.start();
			break;
		}
	}

	if (camera.transform.position.x < 0.0f) {
		camera.transform.position.x = 0.0f;
	}
	if (camera.transform.position.y < 0.0f) {
		camera.transform.position.y = 0.0f;
	}
	const auto map_size = map_texture->size().to<float>();
	if (camera.transform.position.x + camera.transform.scale.x > map_size.x * camera.zoom) {
		camera.transform.position.x = map_size.x * camera.zoom - camera.transform.scale.x;
	}
	if (camera.transform.position.y + camera.transform.scale.y > map_size.y * camera.zoom) {
		camera.transform.position.y = map_size.y * camera.zoom - camera.transform.scale.y;
	}

	player.update(game);
	house_exterior.update(game);
	house_interior.update(game);
	for (auto& decoration : decorations) {
		decoration.update(game);
	}
	for (auto& pickup : pickups) {
		pickup.update(game);
	}
	for (auto& customer : customers) {
		customer->update(game);
	}

	std::erase_if(customers, [](const auto& customer) {
		return customer->reached_target && customer->state == customer_state::leaving;
	});

	game.world->clear_decorations(house_interior.transform);

	// new customers
	if (new_customer_timer.seconds() > 15 && customers.size() < customer_queue_positions.size()) {
		nfwk::random_number_generator rng;
		constexpr object_type types[]{
			object_type::balding_monk,
			object_type::fat_virgin,
			object_type::gray_hair_lumberjack,
			object_type::thin_hair_lady,
			object_type::saggy_tits_lady,
			object_type::cant_see_shit_granny,
			object_type::fat_monk,
			object_type::gray_hair_monk,
			object_type::weak_monk,
			object_type::poor_sight_monk,
			object_type::balding_man,
			object_type::gray_hair_man,
			object_type::weak_man,
			object_type::poor_sight_man,
			object_type::balding_lumberjack,
			object_type::fat_lumberjack,
			object_type::fat_lumberjack_2,
			object_type::poor_sight_lumberjack,
			object_type::fat_lady,
			object_type::gray_hair_lady,
			object_type::old_lady,
			object_type::poor_sight_lady,
			object_type::balding_lady,
			object_type::fat_lady_2,
			object_type::gray_hair_lady_2,
			object_type::poor_sight_lady_2,
			object_type::thin_hair_lady_2,
			object_type::fat_lady_3,
			object_type::gray_hair_lady_3,
			object_type::poor_sight_lady_3
		};
		auto& customer = spawn_object(customer_spawn_position.x, customer_spawn_position.y, types[rng.next<int>(29)]);
		new_customer_timer.start();
	}

	if (customer_ui && customer_ui->should_be_closed) {
		customer_ui = nullptr;
	}
	if (customer_ui) {
		if (customer_ui->type == ui_type::treatment) {
			if (customer_ui->customer->state == customer_state::in_treatment) {
				if (customer_ui->treatment_timer.milliseconds() > 5000) {
					customer_ui->customer->state = customer_state::after_treatment;
					if (customer_ui->customer->treat()) {
						player.gold += 100;
					}
					customer_ui->refresh_message();
					customer_ui->treatment_timer.start();
				}
			} else if (customer_ui->customer->state == customer_state::after_treatment) {
				if (customer_ui->treatment_timer.milliseconds() > 2500) {
					customer_ui->customer->state = customer_state::leaving;
					customer_queue_positions[customer_ui->customer->queue_position.value()].customer = nullptr;
					customer_ui->customer->queue_position = std::nullopt;
				}
			} else if (customer_ui->customer->state == customer_state::leaving) {
				customer_ui = nullptr;
			}
		}
	}
	
	if (brewing_stand.brew_timer.milliseconds() > 5000) {
		brewing_stand.brew_timer.stop();
		brewing_stand.set_result_from_items();
	}
}

void game_world::draw_object(const game_object& object) {
	if (inside_house) {
		if (object.type == object_type::house_exterior) {
			return;
		}
		sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f });
	} else {
		if (object.type == object_type::house_interior) {
			return;
		}
	}
	const auto [sheet_width, sheet_height] = tilesheet->size().to<float>();
	sprite_shader->set_model(object.transform);
	if (object.direction == object_direction::right) {
		rectangle.set_tex_coords(object.tex_coords.x / sheet_width, object.tex_coords.y / sheet_height, object.transform.scale.x / sheet_width, object.transform.scale.y / sheet_height);
	} else {
		rectangle.set_tex_coords((object.tex_coords.x + object.transform.scale.x) / sheet_width, object.tex_coords.y / sheet_height, -object.transform.scale.x / sheet_width, object.transform.scale.y / sheet_height);
	}
	rectangle.draw();

	if (object.customer) {
		auto& customer = static_cast<const customer_object&>(object);
		if (customer.state == customer_state::in_treatment && customer.treated_with == customer.wanted_treatment) {
			const auto ms_treated = static_cast<float>(customer_ui->treatment_timer.milliseconds());
			const float treatment_progress{ ms_treated / 5000.0f };
			sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f, 1.0f, 1.0f, treatment_progress });
			customer_object next_object{ static_cast<object_type>(static_cast<int>(customer.type) + 1) };
			if (object.direction == object_direction::right) {
				rectangle.set_tex_coords(next_object.tex_coords.x / sheet_width, next_object.tex_coords.y / sheet_height, next_object.transform.scale.x / sheet_width, next_object.transform.scale.y / sheet_height);
			} else {
				rectangle.set_tex_coords((next_object.tex_coords.x + next_object.transform.scale.x) / sheet_width, next_object.tex_coords.y / sheet_height, -next_object.transform.scale.x / sheet_width, next_object.transform.scale.y / sheet_height);
			}
			rectangle.draw();
			sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f });
		}
	}
}

void game_world::draw() {
	const auto [sheet_width, sheet_height] = tilesheet->size().to<float>();
	sprite_shader->set_view_projection(camera);
	if (inside_house) {
		sprite_shader->get_variable("color").set(nfwk::vector4f{ 0.2f, 0.2f, 0.2f, 1.0f });
	} else {
		sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f });
	}

	map_texture->bind();
	sprite_shader->set_model(nfwk::transform2{ {}, { 1920.0f, 1080.0f } });
	rectangle.set_tex_coords(0.0f, 0.0f, 1.0f, 1.0f);
	rectangle.draw();


	tilesheet->bind();

	objects_to_draw.clear();
	//objects_to_draw.push_back(&player);
	//objects_to_draw.push_back(&house_exterior);
	//objects_to_draw.push_back(&house_interior);
	//objects_to_draw.push_back(&brewing_stand);
	for (auto& decoration : decorations) {
		objects_to_draw.push_back(&decoration);
	}
	for (auto& pickup : pickups) {
		objects_to_draw.push_back(&pickup);
	}
	for (auto& customer : customers) {
		objects_to_draw.push_back(customer.get());
	}

	sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f });
	std::ranges::sort(objects_to_draw, [](const auto* a, const auto* b) {
		return a->collision_transform.position.y < b->collision_transform.position.y;
	});

	draw_object(house_interior);
	draw_object(brewing_stand);
	for (auto& object_to_draw : objects_to_draw) {
		draw_object(*object_to_draw);
	}
	draw_object(house_exterior);
	draw_object(player);

	float item_x_off{ 16.0f };
	for (auto& item : brewing_stand.items) {
		item.update_y_off();
		item.transform.position = brewing_stand.transform.position;
		item.transform.position.x += item_x_off;
		item.transform.position.y += item.y_off;
		draw_object(item);
		item_x_off += 12.0f;
	}
	if (brewing_stand.result.type != object_type::no_cure) {
		brewing_stand.result.update_y_off();
		brewing_stand.result.transform.position = brewing_stand.transform.position;
		brewing_stand.result.transform.position.x += 20.0f;
		brewing_stand.result.transform.position.y += brewing_stand.result.y_off;
		draw_object(brewing_stand.result);
	}

	if (brewing_stand.brew_timer.has_started()) {
		brewing_stand.bubbles.update(1.0f / 60.0f);
		brewing_stand.bubbles.draw(*sprite_shader, brewing_stand.transform.position - nfwk::vector2f{ 0.0f, 32.0f }, nfwk::vector2f{ 48.0f, 32.0f });
	}

	// draw queue positions to debug
#if 0
	nfwk::texture blank{nfwk::surface{2, 2, nfwk::pixel_format::rgba, 0xffffffff}, nfwk::scale_option::nearest_neighbor, false};
	for (const auto& [position, customer] : customer_queue_positions) {
		sprite_shader->get_variable("color").set(nfwk::vector4f{ customer ? 1.0f : 0.4f, customer ? 0.3f : 1.0f, 0.4f, 0.5f });
		rectangle.set_tex_coords(0.0f, 0.0f, 1.0f, 1.0f);
		blank.bind();
		sprite_shader->set_model(nfwk::transform2{ position, 16.0f });
		rectangle.draw();
	}
#endif
	// draw collisions to debug
#if 0
	nfwk::texture blank{ nfwk::surface{2, 2, nfwk::pixel_format::rgba, 0xffffffff}, nfwk::scale_option::nearest_neighbor, false };
	for (const auto& collision : collisions) {
		sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f, 0.4f, 0.4f, 0.5f });
		rectangle.set_tex_coords(0.0f, 0.0f, 1.0f, 1.0f);
		blank.bind();
		sprite_shader->set_model(collision);
		rectangle.draw();
	}
	tilesheet->bind();
#endif
	// draw ingredient spawns to debug
#if 0
	for (const auto& ingredient_spawn_area : ingredient_spawn_areas) {
		sprite_shader->get_variable("color").set(nfwk::vector4f{ 0.0f, 1.0f, 0.4f, 0.5f });
		rectangle.set_tex_coords(0.0f, 0.0f, 1.0f, 1.0f);
		blank.bind();
		sprite_shader->set_model(ingredient_spawn_area);
		rectangle.draw();
	}
	tilesheet->bind();
#endif

	// ui
	sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f });
	draw_inventory();

	// recipes
	float recipe_y{ 0.0f };
	sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f });
	for (const auto cure : player.unlocked_cures) {
		game_object cure_object{ cure };
		
		// cure icon
		tilesheet->bind();
		{
			nfwk::transform2 transform;
			transform.position = { 32.0f, 32.0f + recipe_y };
			transform.scale = 64.0f;
			sprite_shader->set_model(transform);
			rectangle.set_tex_coords(cure_object.tex_coords.x / sheet_width, cure_object.tex_coords.y / sheet_height, cure_object.transform.scale.x / sheet_width, cure_object.transform.scale.y / sheet_height);
			rectangle.draw();
		}

		// recipe ingredient icons
		float ingredient_x{ 0.0f };
		for (const auto ingredient : game_object::ingredients_for_treatment(cure)) {
			game_object ingredient_object{ ingredient };
			nfwk::transform2 transform;
			transform.position = { 288.0f + ingredient_x, 32.0f + recipe_y };
			transform.scale = 64.0f;
			sprite_shader->set_model(transform);
			rectangle.set_tex_coords(ingredient_object.tex_coords.x / sheet_width, ingredient_object.tex_coords.y / sheet_height, ingredient_object.transform.scale.x / sheet_width, ingredient_object.transform.scale.y / sheet_height);
			rectangle.draw();
			ingredient_x += 80.0f;
		}

		// cure name
		const auto& name_texture = object_name_textures[cure];
		name_texture->bind();
		{
			nfwk::transform2 transform;
			transform.position = { 112.0f, 48.0f + recipe_y };
			transform.scale = name_texture->size().to<float>();
			sprite_shader->set_model(transform);
			rectangle.set_tex_coords(0.0f, 0.0f, 1.0f, 1.0f);
			rectangle.draw();
		}

		recipe_y += 80.0f;
	}

	if (customer_ui) {
		customer_ui->draw();
	}
}

void game_world::draw_inventory() {
	const auto [sheet_width, sheet_height] = tilesheet->size().to<float>();
	sprite_shader->set_view_projection(hud_camera);
	nfwk::transform2 transform;
	transform.position = { hud_camera.width() - 384.0f, 32.0f};
	for (const auto item : player.items) {
		const game_object object{ item };
		transform.scale = object.transform.scale * 4.0f;
		
		sprite_shader->set_model(transform);
		rectangle.set_tex_coords(object.tex_coords.x / sheet_width, object.tex_coords.y / sheet_height, object.transform.scale.x / sheet_width, object.transform.scale.y / sheet_height);
		rectangle.draw();

		transform.position.x += transform.scale.x + 4.0f;
		if (transform.position.x + transform.scale.x + 32.0f > hud_camera.width()) {
			transform.position.x = hud_camera.width() - 384.0f;
			transform.position.y += transform.scale.y + 4.0f;
		}
	}
}
game_object& game_world::spawn_object(float x, float y, object_type type) {
	game_object object{ type };
	object.transform.position = { x, y };
	switch (type) {
	case object_type::player:
		player.transform.position = { x, y };
		return player;

	case object_type::house_exterior:
		house_exterior = object;
		return house_exterior;

	case object_type::house_interior:
		house_interior = object;
		return house_interior;

	case object_type::brewing_stand:
		brewing_stand = {};
		brewing_stand.transform.position = { x, y, };
		return brewing_stand;

	case object_type::balding_monk:
	case object_type::fat_virgin:
	case object_type::gray_hair_lumberjack:
	case object_type::thin_hair_lady:
	case object_type::saggy_tits_lady:
	case object_type::cant_see_shit_granny:
	case object_type::fat_monk:
	case object_type::gray_hair_monk:
	case object_type::weak_monk:
	case object_type::poor_sight_monk:
	case object_type::balding_man:
	case object_type::gray_hair_man:
	case object_type::weak_man:
	case object_type::poor_sight_man:
	case object_type::balding_lumberjack:
	case object_type::fat_lumberjack:
	case object_type::fat_lumberjack_2:
	case object_type::poor_sight_lumberjack:
	case object_type::fat_lady:
	case object_type::gray_hair_lady:
	case object_type::old_lady:
	case object_type::poor_sight_lady:
	case object_type::balding_lady:
	case object_type::fat_lady_2:
	case object_type::gray_hair_lady_2:
	case object_type::poor_sight_lady_2:
	case object_type::thin_hair_lady_2:
	case object_type::fat_lady_3:
	case object_type::gray_hair_lady_3:
	case object_type::poor_sight_lady_3:
	{
		auto& customer = customers.emplace_back(std::make_unique<customer_object>(type));
		customer->transform = object.transform;
		return *customer;
	}

	case object_type::bush:
	case object_type::tree:
	case object_type::big_rock:
	case object_type::tree_stump:
	case object_type::log:
	case object_type::small_rocks:
		return decorations.emplace_back(object);

	case object_type::star_anise:
	case object_type::st_johns_wort:
	case object_type::spearmint:
	case object_type::solomons_seal:
	case object_type::thyme:
	case object_type::valerian:
	case object_type::wormwood:
	case object_type::sage:
	case object_type::patchouli:
	case object_type::hyssop:
	case object_type::majoram:
	case object_type::horehound:
		return pickups.emplace_back(object);

	default:
		nfwk::error("game", "Type does not create object.");
		ASSERT(false);
	}
}

void game_world::pickup(const nfwk::transform2& collision) {
	if (customer_ui) {
		return;
	}
	for (int i{ 0 }; i < static_cast<int>(pickups.size()); i++) {
		const auto& object = pickups[i];
		if (object.interactive && object.collision_transform.collides_with(collision)) {
			player.items.push_back(object.type);
			pickups.erase(pickups.begin() + i);
			break;
		}
	}
}

void game_world::clear_decorations(const nfwk::transform2& transform) {
	std::erase_if(decorations, [&transform](const game_object& object) {
		return object.transform.collides_with(transform);
	});
}

void game_world::add_collision(float x, float y, float w, float h) {
	collisions.emplace_back(nfwk::vector2f{ x, y }, nfwk::vector2f{ w - x, h - y });
}

void game_world::add_ingredient_spawn(float x, float y, float w, float h) {
	ingredient_spawn_areas.emplace_back(nfwk::vector2f{ x, y }, nfwk::vector2f{ w - x, h - y });
}