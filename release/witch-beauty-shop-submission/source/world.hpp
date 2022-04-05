#pragma once

#include "assets.hpp"
#include "graphics/ortho_camera.hpp"
#include "graphics/shader.hpp"
#include "generator.hpp"
#include "graphics/rectangle.hpp"
#include "object.hpp"
#include "game_ui.hpp"

class game_program;

class game_world {
public:

	nfwk::ortho_camera camera;
	nfwk::ortho_camera hud_camera;
	nfwk::vector2f customer_spawn_position;
	game_program& game;
	bool inside_house{ false };

	std::shared_ptr<nfwk::shader> sprite_shader;
	std::shared_ptr<nfwk::texture> tilesheet;
	std::shared_ptr<nfwk::texture> map_texture;
	nfwk::rectangle rectangle;

	player_object player;
	game_object house_exterior{ object_type::house_exterior };
	game_object house_interior{ object_type::house_interior };
	brewing_stand_object brewing_stand;
	std::vector<game_object> decorations;
	std::vector<game_object> pickups;
	std::vector<std::unique_ptr<customer_object>> customers;

	std::vector<game_object*> objects_to_draw;

	nfwk::timer new_customer_timer;

	std::vector<nfwk::transform2> collisions;
	std::vector<nfwk::transform2> ingredient_spawn_areas;

	nfwk::timer ingredient_grow_timer;

	std::unordered_map<object_type, std::unique_ptr<nfwk::texture>> object_name_textures;

	struct queue_info {
		nfwk::vector2f position;
		customer_object* customer{ nullptr };
	};

	std::unique_ptr<customer_chat_ui> customer_ui;

	std::vector<queue_info> customer_queue_positions;

	nfwk::event_listener mouse_press_listener;

	game_world(game_program& game);
	game_world(const game_world&) = delete;
	game_world(game_world&&) = delete;

	~game_world();

	game_world& operator=(const game_world&) = delete;
	game_world& operator=(game_world&&) = delete;

	void update();
	void draw();

	game_object& spawn_object(float x, float y, object_type type);

	void pickup(const nfwk::transform2& collision);
	void clear_decorations(const nfwk::transform2& transform);
	
	void draw_inventory();

	void draw_object(const game_object& object);

	void add_collision(float x, float y, float w, float h);
	void add_ingredient_spawn(float x, float y, float w, float h);

};
