#pragma once
#include "common.hpp"
#include "natives.hpp"

namespace big
{
	class gui
	{
	public:
		void dx_init();
		void dx_on_tick();

		void script_init();
		void script_on_tick();
		void delayed_loops();
		static void script_func();
		static void delayed_func();
	public:
		bool m_opened{};

		int rank = 1;
		int rp_mode = 1;

		int casino_chips;
		int casino_chips_2;

		int value_1;
		int value_2;
		int value_3;
		int value_4;
		int value_5;
		int value_6;
		int value_7;
		int value_8;
		int value_9;
		int value_10;
		int value_11;
		int value_12;
		int value_13;
		int value_14;
		int value_15;
		int value_16;
		int value_17;
		int value_18;
		int value_19;
		int value_20;
		int value_21;
		int value_22;
		int value_23;
		int value_24;
		int value_25;
		int value_26;
		int value_27;
		int value_28;
		int value_29;
		int value_30;
		int value_31;
		int value_32;
		int value_33;
		int value_34;
		int value_35;
		int value_36;
		int value_37;
		int value_38;
		int value_39;
		int value_40;
		int value_41;

		bool anti_afk = false;
		bool rp_loop = false;
		bool always_jackpot = false;
		bool bet_multiplier = false;
		bool slots_bet = false;
		bool cas_cooldown_bypass = false;
		bool stop_anim = false;
		bool force_trans = false;
		bool fast_spin = false;
		bool auto_play = false;
		bool auto_play_blackjack = false;
		bool rig_blackjack = false;
		bool bypass_cooldown_err = false;
		bool increase_chips_limit = false;
	};

	inline gui g_gui;
}
