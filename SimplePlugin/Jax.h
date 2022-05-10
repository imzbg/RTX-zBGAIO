#include "../plugin_sdk/plugin_sdk.hpp"

namespace Jax {

	void load();
	void unload();

	void on_draw();
	void on_update();
	void on_after_attack_orbwalker(game_object_script target);

	void q_logic(game_object_script target);
	void w_logic(game_object_script target);
	void e_logic(game_object_script target);
	void r_logic(game_object_script target);
}