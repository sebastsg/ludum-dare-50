#pragma once

#include "loop.hpp"
#include "subprogram.hpp"
#include "imgui_loop_component.hpp"
#include "assets.hpp"
#include "log.hpp"
#include "graphics/window.hpp"
#include "presets.hpp"
#include "graphics/ui.hpp"
#include "graphics/shader_variable.hpp"
#include "html_log_writer.hpp"
#include "editor.hpp"
#include "world.hpp"
#include "graphics/font.hpp"

class game_program : public nfwk::subprogram {
public:

	std::shared_ptr<nfwk::window> window;
	nfwk::event_listener draw_event;
	std::shared_ptr<nfwk::ui_manager> ui;
	std::shared_ptr<nfwk::asset_manager> assets;
	std::shared_ptr<nfwk::font> font;

	std::unique_ptr<game_world> world;

	game_program(nfwk::loop& loop);

	~game_program() override;

	void update() override;
	void draw();

};
