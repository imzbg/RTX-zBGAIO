#include "Jax.h"

namespace Jax
{
	TreeTab* main_tab;

	namespace spells
	{
		constexpr float Q_RANGE = 700.f;
		constexpr float W_RANGE = 0.f;
		constexpr float E_RANGE = 350.f;
		constexpr float R_RANGE = 0.f;
		constexpr float FLASH_RANGE = 400.f;

		script_spell* q = nullptr;
		script_spell* w = nullptr;
		script_spell* e = nullptr;
		script_spell* r = nullptr;
		script_spell* flash = nullptr;
	}

	namespace settings
	{
		namespace combo
		{
			TreeEntry* use_q = nullptr;
			TreeEntry* use_w = nullptr;
			TreeEntry* use_e = nullptr;
			TreeEntry* use_r = nullptr;

		}

		namespace draw
		{
			TreeEntry* only_ready = nullptr;
			TreeEntry* draw_range_q = nullptr;
			TreeEntry* draw_range_q_color = nullptr;
			TreeEntry* draw_range_q_tickness = nullptr;
			TreeEntry* draw_range_e = nullptr;
			TreeEntry* draw_range_e_color = nullptr;
			TreeEntry* draw_range_e_tickness = nullptr;
		}

		namespace flee
		{
			TreeEntry* use_q = nullptr;
		}

		namespace harass
		{
			TreeEntry* use_q = nullptr;
			TreeEntry* use_w = nullptr;
			TreeEntry* use_e = nullptr;
			TreeEntry* min_mana = nullptr;
		}
	}


	void load()
	{

		spells::q = plugin_sdk->register_spell(spellslot::q, spells::Q_RANGE);
		spells::w = plugin_sdk->register_spell(spellslot::w, spells::W_RANGE);
		spells::e = plugin_sdk->register_spell(spellslot::e, spells::E_RANGE);
		spells::r = plugin_sdk->register_spell(spellslot::r, spells::R_RANGE);


		if (myhero->get_spell(spellslot::summoner1)->get_spell_data()->get_name_hash() == spell_hash("SummonerFlash"))
			spells::flash = plugin_sdk->register_spell(spellslot::summoner1, spells::FLASH_RANGE);
		else if (myhero->get_spell(spellslot::summoner2)->get_spell_data()->get_name_hash() == spell_hash("SummonerFlash"))
			spells::flash = plugin_sdk->register_spell(spellslot::summoner2, spells::FLASH_RANGE);


		main_tab = menu->create_tab("zBGAIO.Jax", "zBGAIO");

		main_tab->add_separator("combo_separator", "--- Abilities ---");

		auto combo_tab = main_tab->add_tab("combo", "Combo");
		{
			settings::combo::use_q = combo_tab->add_checkbox("Q", "Use Q", true);
			settings::combo::use_q->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());

			settings::combo::use_w = combo_tab->add_checkbox("W", "Use W", true);
			settings::combo::use_w->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());

			settings::combo::use_e = combo_tab->add_checkbox("E", "Use E", true);
			settings::combo::use_e->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());

			settings::combo::use_r = combo_tab->add_checkbox("R", "Use R", true);
			settings::combo::use_r->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());

			combo_tab->add_separator("misc_separator", "--- Miscellaneous ---");

		}


		auto harass_tab = main_tab->add_tab("harass", "Harass");
		{
			settings::harass::use_q = harass_tab->add_checkbox("Q", "Use Q", true);
			settings::harass::use_q->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
			settings::harass::use_w = harass_tab->add_checkbox("W", "Use W", true);
			settings::harass::use_w->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());
			settings::harass::use_e = harass_tab->add_checkbox("E", "Use E", true);
			settings::harass::use_e->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
			settings::harass::min_mana = harass_tab->add_slider("min_mana", "Use only if mana > (x) | 0=Disabled", 45, 0, 100);
		}

		auto flee_tab = main_tab->add_tab("flee", "Flee");
		{
			settings::flee::use_q = flee_tab->add_checkbox("Q", "Use Q", true);
			settings::flee::use_q->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());
		}

		main_tab->add_separator("misc_separator", "--- Miscellaneous ---");

		auto draw_tab = main_tab->add_tab("draw", "Draw");
		{
			float vec_4f_color[] = { 0.f, 1.f, 0.f, 1.f };

			draw_tab->add_separator("draw_separator", "--- Spells Drawings ---");

			auto draw_q_tab = draw_tab->add_tab("Q", "Q Settings");
			{
				draw_q_tab->set_assigned_texture(myhero->get_spell(spellslot::q)->get_icon_texture());

				settings::draw::draw_range_q = draw_q_tab->add_checkbox("enable", "Draw Range Q", true);
				settings::draw::draw_range_q_color = draw_q_tab->add_colorpick("color", "Draw Color", vec_4f_color);
				settings::draw::draw_range_q_tickness = draw_q_tab->add_slider("tickness", "Draw Tickness", 1, 1, 5);
			}


			auto draw_e_tab = draw_tab->add_tab("E", "E Settings");
			{
				draw_e_tab->set_assigned_texture(myhero->get_spell(spellslot::e)->get_icon_texture());

				settings::draw::draw_range_e = draw_e_tab->add_checkbox("enable", "Draw Range E", true);
				settings::draw::draw_range_e_color = draw_e_tab->add_colorpick("color", "Range Color", vec_4f_color);
				settings::draw::draw_range_e_tickness = draw_e_tab->add_slider("tickness", "Draw Tickness", 1, 1, 5);
			}

			draw_tab->add_separator("trigger_separator", "--- Trigger Mode ---");
			{
				settings::draw::only_ready = draw_tab->add_checkbox("only_ready", "Only Draw If Spell Ready", false);
			}
		}


		event_handler<events::on_draw>::add_callback(on_draw);
		event_handler<events::on_update>::add_callback(on_update);
	}

	void unload()
	{
		menu->delete_tab(main_tab);


		plugin_sdk->remove_spell(spells::q);
		plugin_sdk->remove_spell(spells::e);
		plugin_sdk->remove_spell(spells::w);
		plugin_sdk->remove_spell(spells::r);
		if (spells::flash) plugin_sdk->remove_spell(spells::flash);


		event_handler<events::on_draw>::remove_handler(on_draw);
		event_handler<events::on_update>::remove_handler(on_update);
	}


	void on_draw()
	{
		if (myhero->is_dead())
			return;

		if (settings::draw::draw_range_q->get_bool())
		{
			if (!settings::draw::only_ready->get_bool() || (settings::draw::only_ready->get_bool() && spells::q->is_ready()))
			{
				draw_manager->add_circle(myhero->get_position(), spells::Q_RANGE, settings::draw::draw_range_q_color->get_color(), settings::draw::draw_range_q_tickness->get_int());
			}
		}

		if (settings::draw::draw_range_e->get_bool())
		{
			if (!settings::draw::only_ready->get_bool() || (settings::draw::only_ready->get_bool() && spells::e->is_ready()))
			{

				draw_manager->add_circle(myhero->get_position(), spells::E_RANGE, settings::draw::draw_range_e_color->get_color(), settings::draw::draw_range_e_tickness->get_int());
			}
		}
	}

	void on_update()
	{
		if (myhero->is_dead() || myhero->is_recalling() || !myhero->can_cast())
			return;

		if (!orbwalker->can_move(0.05f))
			return;


		if (orbwalker->combo_mode())
		{
			if (settings::combo::use_q->get_bool())
			{
				q_logic(nullptr);
			}

			if (settings::combo::use_e->get_bool())
			{
				e_logic(nullptr);
			}

			if (settings::combo::use_r->get_bool())
			{
				r_logic(nullptr);
			}
			return;
		}

		if (orbwalker->harass())
		{

			if (settings::harass::use_q->get_bool())
			{
				q_logic(nullptr);
			}

			if (settings::harass::use_e->get_bool())
			{
				e_logic(nullptr);
			}
		}

		if (orbwalker->flee_mode())
		{
			if (settings::flee::use_q->get_bool() && spells::q->is_ready())
			{
				std::vector<game_object_script> nearby_minions(entitylist->get_ally_minions());

				nearby_minions.erase(std::remove_if(nearby_minions.begin(), nearby_minions.end(), [](game_object_script object)
					{
						return (object->get_position().distance(myhero) > spells::Q_RANGE);
					}),
					nearby_minions.end()
						);

				if (!nearby_minions.empty())
				{
					std::sort(nearby_minions.begin(), nearby_minions.end(), [](game_object_script a, game_object_script b)
						{
							return (a->get_distance(myhero->get_position()) > b->get_distance(myhero->get_position()));
						}
					);
				}

				if (spells::q->can_cast(nearby_minions.front()))
					spells::q->cast(nearby_minions.front());
			}
		}
	}

	void on_after_attack_orbwalker(game_object_script target) {
		if (settings::combo::use_w->get_bool() && spells::w->is_ready()) {
			w_logic(target);
		}
	}

		void q_logic(game_object_script target)
		{
			if (!spells::q->is_ready())
				return;

			auto _target = (target ? target : target_selector->get_target(spells::q->range(), damage_type::magical));
			if (!_target->is_valid() || !_target->is_valid_target() || _target->is_dead())
				return;

			spells::q->cast(_target);
		}

		void w_logic(game_object_script target)
		{
			if (!spells::w->is_ready())
				return;

			spells::w->cast();

		}

		void e_logic(game_object_script target)
		{
			if (!spells::e->is_ready())
				return;

			spells::e->cast();

		}

		void r_logic(game_object_script target)
		{
			if (!spells::r->is_ready())
				return;
			
			spells::r->cast();
		}

}