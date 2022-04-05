#include "game_ui.hpp"
#include "game.hpp"

customer_chat_ui::customer_chat_ui(game_program& game_, ui_type type_, customer_object* customer_) : game{ game_ }, type{ type_ }, customer{ customer_ } {
	refresh_message();

	for (const auto item : game.world->player.items) {
		if (type == ui_type::treatment && !game_object::is_treatment(item)) {
			continue;
		}
		if (type == ui_type::brewing && !game_object::is_ingredient(item)) {
			continue;
		}
		bool added{ false };
		for (const auto& choice : choices) {
			if (choice.item == item) {
				added = true;
				break;
			}
		}
		if (!added) {
			if (type != ui_type::brewing || game.world->brewing_stand.items.size() < 2) {
				add_choice(item);
			}
		}
	}

	if (type == ui_type::treatment) {
		add_choice(object_type::no_cure);
	} else if (type == ui_type::brewing) {
		if (!game.world->brewing_stand.items.empty()) {
			add_choice(object_type::start_brewing);
		}
		add_choice(object_type::no_ingredient);
	}
}

void customer_chat_ui::on_mouse_press() {
	if (type == ui_type::treatment) {
		if (customer->state == customer_state::waiting) {
			for (int i{ 0 }; i < static_cast<int>(choices.size()); i++) {
				auto transform = make_choice_transform(i);
				if (transform.collides_with(game.world->hud_camera.mouse_position(game.window->mouse))) {
					if (choices[i].item == object_type::no_cure) {
						should_be_closed = true;
					} else {
						customer->state = customer_state::in_treatment;
						customer->treated_with = choices[i].item;
						game.world->player.remove_item(choices[i].item);
						treatment_timer.start();
					}
					break;
				}
			}
		}
	} else if (type == ui_type::brewing) {
		for (int i{ 0 }; i < static_cast<int>(choices.size()); i++) {
			auto transform = make_choice_transform(i);
			if (transform.collides_with(game.world->hud_camera.mouse_position(game.window->mouse))) {
				if (choices[i].item == object_type::start_brewing) {
					game.world->brewing_stand.brew_timer.start();
					should_be_closed = true;
				} else if (choices[i].item == object_type::no_ingredient) {
					should_be_closed = true;
				} else {
					game.world->brewing_stand.items.emplace_back(choices[i].item);
					game.world->player.remove_item(choices[i].item);
					should_be_closed = true;
				}
				break;
			}
		}
	}
}

void customer_chat_ui::refresh_message() {
	if (!customer) {
		return;
	}
	auto text = customer->greeting;
	if (customer->state == customer_state::after_treatment && customer->wanted_treatment != customer->treated_with) {
		text = "I don't notice a difference. What a scam!";
	}
	message_texture = std::make_unique<nfwk::texture>(game.font->render(text, 0xffffffff), nfwk::scale_option::nearest_neighbor, false);
	message_texture_shadow = std::make_unique<nfwk::texture>(game.font->render(text, 0xff222222), nfwk::scale_option::nearest_neighbor, false);
}

void customer_chat_ui::draw() {
	draw_message();
	draw_choices();
}

nfwk::transform2 customer_chat_ui::make_choice_transform(int index) const {
	nfwk::transform2 transform;
	transform.position.x = game.world->hud_camera.transform.scale.x / 2.0f + 64.0f;
	transform.position.y = game.world->hud_camera.transform.scale.y / 2.0f - 128.0f + 32.0f * static_cast<float>(index);
	transform.scale = { 256.0f, 32.0f };
	return transform;
}

void customer_chat_ui::add_choice(object_type type) {
	const auto text = game_object::get_name(type);
	auto choice = std::make_unique<nfwk::texture>(game.font->render(text, 0xffffffff), nfwk::scale_option::nearest_neighbor, false);
	if (type == object_type::start_brewing) {
		choices.insert(choices.begin(), choice_data{ std::move(choice), type });
	} else {
		choices.push_back(choice_data{ std::move(choice), type });
	}
}

void customer_chat_ui::draw_message() {
	if (!customer) {
		return;
	}
	if (customer->state == customer_state::waiting || customer->state == customer_state::after_treatment) {
		const auto message_size = message_texture->size().to<float>();
		auto& camera = game.world->camera;

		nfwk::transform2 transform;
		transform.position.x = customer->transform.position.x - camera.transform.position.x + 350.0f;
		transform.position.y = customer->transform.position.y - camera.transform.position.y + 870.0f;
		transform.scale = message_size;

		game.world->sprite_shader->set_model(transform);
		message_texture_shadow->bind();
		rectangle.set_tex_coords(0.0f, 0.0f, 1.0f, 1.0f);
		rectangle.draw();

		transform.position -= 2.0f;
		game.world->sprite_shader->set_model(transform);
		message_texture->bind();
		rectangle.draw();
	}
}

void customer_chat_ui::draw_choices() {
	if (type == ui_type::treatment && customer->state != customer_state::waiting) {
		return;
	}
	const auto [sheet_width, sheet_height] = game.world->tilesheet->size().to<float>();
	nfwk::transform2 transform;
	active_choice = -1;
	int choice_index{ 0 };
	for (const auto& choice : choices) {
		auto transform = make_choice_transform(choice_index);
		auto text_transform = transform;
		text_transform.position.x += 20.0f;
		text_transform.position.y += 6.0f;
		text_transform.scale = choice.texture->size().to<float>();
		game_object object{ choice.item };
		object.transform.position = text_transform.position;
		object.transform.position.x -= 28.0f;
		object.transform.position.y -= 2.0f;
		if (transform.collides_with(game.world->hud_camera.mouse_position(game.window->mouse))) {
			active_choice = choice_index;
		}
		choice.texture->bind();
		game.world->sprite_shader->set_model(text_transform);
		game.world->sprite_shader->get_variable("color").set(nfwk::vector4f{ 0.2f, 0.2f, 0.2f, 1.0f });
		rectangle.draw();
		object.transform.position -= 8.0f;
		object.transform.scale += 16.0f;
		if (active_choice == choice_index) {
			game.world->sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f, 1.0f, 0.25f, 1.0f });
		} else {
			game.world->sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f });
		}
		text_transform.position -= 2.0f;
		game.world->sprite_shader->set_model(text_transform);
		rectangle.draw();
		if (choice.item != object_type::no_cure) {
			game.world->tilesheet->bind();
			game.world->sprite_shader->get_variable("color").set(nfwk::vector4f{ 1.0f });
			game.world->sprite_shader->set_model(object.transform);
			rectangle.set_tex_coords(object.tex_coords.x / sheet_width, object.tex_coords.y / sheet_height, 16.0f / sheet_width, 16.0f / sheet_height);
			rectangle.draw();
			rectangle.set_tex_coords(0.0f, 0.0f, 1.0f, 1.0f);
		}
		choice_index++;
	}
}
