#include "game.hpp"

void test() {

}

void start() {
	nfwk::log::add_writer_type<nfwk::log::html_writer>();
	nfwk::fixed_time_step_loop loop;
	loop.add<game_program>();
	loop.run();
}
