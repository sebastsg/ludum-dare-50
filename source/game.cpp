#include "game.hpp"

constexpr bool development_version{ true };
static const std::filesystem::path asset_directory{ development_version ? u8"../.." : u8"." };

game_program::game_program(nfwk::loop& loop) : nfwk::subprogram{ loop } {
	window = nfwk::make_window(*this, "Ludum Dare 50");
	assets = nfwk::make_asset_manager(asset_directory);
	font = assets->find<nfwk::font>("fonts/leo");
	world = std::make_unique<game_world>(*this);
	world->camera.zoom = 4.0f;

	//if (auto windows_window = dynamic_cast<nfwk::platform::windows_window*>(window.get())) {
	//	windows_window->set_icon_from_resource(102);
	//}

	draw_event = window->on_draw.listen([this] {
		draw();
	});

	ui = nfwk::get_window_manager(*this).get_ui_manager();
	auto menu_bar = ui->get_main_menu_bar();

	menu_bar->add("game", "View", [this] {
		if (nfwk::ui::menu_item("Reset camera position")) {
			world->camera.transform.position = 0.0f;
		}
		for (const float zoom : { 4.0f, 2.0f, 1.0f, 0.5f, 0.25f }) {
			if (nfwk::ui::menu_item(std::format("Zoom ({}%)", static_cast<int>(zoom * 100.0f)))) {
				world->camera.zoom = zoom;
			}
		}
	});

	menu_bar->add("game", [this] {
		nfwk::ui::colored_text({ 0.3f, 0.9f, 0.9f }, "\tZoom: %i%%", static_cast<int>(world->camera.zoom * 100.0f));
		const auto mouse_position = world->camera.mouse_position(window->mouse);
		nfwk::ui::text("Mouse Position: %f, %f", mouse_position.x, mouse_position.y);
	});
}

game_program::~game_program() {
	auto menu_bar = nfwk::get_window_manager(*this).get_ui_manager()->get_main_menu_bar();
	menu_bar->remove("game");
}

void game_program::update() {
	//nfwk::ortho_camera::move_controller{}.update(world->camera, window->keyboard);
	world->camera.transform.scale = window->size().to<float>();
	world->hud_camera.transform.scale = window->size().to<float>();
	world->update();
}

void game_program::draw() {
	world->draw();
}
