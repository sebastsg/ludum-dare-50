#pragma once

#include "graphics/texture.hpp"
#include "graphics/rectangle.hpp"
#include "transform.hpp"
#include "timer.hpp"
#include "object.hpp"

class game_world;
class game_program;

enum class ui_type { treatment, brewing };

class customer_chat_ui {
public:
	
	game_program& game;

	customer_object* customer{ nullptr };
	ui_type type{ ui_type::treatment };

	nfwk::timer treatment_timer;

	struct choice_data {
		std::unique_ptr<nfwk::texture> texture;
		object_type item{ object_type::no_cure };
	};

	std::unique_ptr<nfwk::texture> message_texture;
	std::unique_ptr<nfwk::texture> message_texture_shadow;
	std::vector<choice_data> choices;
	nfwk::rectangle rectangle;
	int active_choice{ -1 };

	bool should_be_closed{ false };

	customer_chat_ui(game_program& game, ui_type type, customer_object* customer);

	void refresh_message();

	void draw();

	nfwk::transform2 make_choice_transform(int index) const;

	void on_mouse_press();

private:

	void add_choice(object_type type);

	void draw_message();
	void draw_choices();

};
