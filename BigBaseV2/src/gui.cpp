#include "common.hpp"
#include "fiber_pool.hpp"
#include "gta/player.hpp"
#include "gta_util.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "memory/module.hpp"
#include "memory/pattern.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script.hpp"
#include "script_global.hpp"
#include "memory_edit.hpp"
#include "script_local.hpp"
#include "fonts.hpp"

#include <imgui.h>
#include <iostream>
#include <Windows.h>
#include <thread>

namespace big
{
	void gui::dx_init()
	{
		ImGuiStyle* style = &ImGui::GetStyle();

		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->ChildRounding = 5.0f;
		style->PopupRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 0.20f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 0.20f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 0.20f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 0.60f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
	}

	void notification(const std::string& text)
	{
		HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
		HUD::_THEFEED_SET_NEXT_POST_BACKGROUND_COLOR(184);
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(text.c_str()));
		HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, false);
	}

	void notification_error(const std::string& text)
	{
		HUD::BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
		HUD::_THEFEED_SET_NEXT_POST_BACKGROUND_COLOR(6);
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(text.c_str()));
		HUD::END_TEXT_COMMAND_THEFEED_POST_TICKER(false, false);
	}

	int rp_value(int value) {
		return *script_global(GetXPValue + 1).at(value).as<int*>();
	}

	int character() {
		return *script_global(CurrentMPChar).as<int*>();
	}

	int packed_bool(int iParam0, bool bParam1, int iParam2, int iParam3)
	{
		int iVar0;
		int iVar1;
		int iVar2;
		int iVar3;
		int iVar4;
		int iVar5;
		int iVar6;
		int iVar7;
		int iVar8;
		int iVar9;
		int iVar10;
		int iVar11;
		int iVar12;
		int iVar13;
		int iVar14;
		int iVar15;
		int iVar16;
		int iVar17;
		int iVar18;
		int iVar19;
		int iVar20;
		int iVar21;
		int iVar22;
		int iVar23;
		int iVar24;
		int iVar25;

		if (iParam2 == -1)
		{
			iParam2 = character();
		}
		iVar0 = 0;
		if (iParam0 >= 0 && iParam0 < 192)
		{
			iVar2 = STATS::GET_PACKED_BOOL_STAT_KEY((iParam0 - 0), 0, 1, iParam2);
			iVar1 = ((iParam0 - 0) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 0)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar2, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 192 && iParam0 < 384)
		{
			iVar3 = STATS::GET_PACKED_BOOL_STAT_KEY((iParam0 - 192), 1, 1, iParam2);
			iVar1 = ((iParam0 - 192) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 192)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar3, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 513 && iParam0 < 705)
		{
			iVar4 = STATS::GET_PACKED_BOOL_STAT_KEY((iParam0 - 513), 0, 0, 0);
			iVar1 = ((iParam0 - 513) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 513)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar4, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 705 && iParam0 < 1281)
		{
			iVar5 = STATS::GET_PACKED_BOOL_STAT_KEY((iParam0 - 705), 1, 0, 0);
			iVar1 = ((iParam0 - 705) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 705)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar5, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 3111 && iParam0 < 3879)
		{
			iVar6 = STATS::GET_PACKED_TU_BOOL_STAT_KEY((iParam0 - 3111), 0, 1, iParam2);
			iVar1 = ((iParam0 - 3111) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 3111)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar6, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 2919 && iParam0 < 3111)
		{
			iVar7 = STATS::GET_PACKED_TU_BOOL_STAT_KEY((iParam0 - 2919), 0, 0, 0);
			iVar1 = ((iParam0 - 2919) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 2919)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar7, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 4207 && iParam0 < 4335)
		{
			iVar8 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 4207), 0, 1, iParam2, "_NGPSTAT_BOOL");
			iVar1 = ((iParam0 - 4207) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 4207)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar8, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 4335 && iParam0 < 4399)
		{
			iVar9 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 4335), 0, 0, 0, "_NGPSTAT_BOOL");
			iVar1 = ((iParam0 - 4335) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 4335)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar9, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 6029 && iParam0 < 6413)
		{
			iVar10 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 6029), 0, 1, iParam2, "_NGTATPSTAT_BOOL");
			iVar1 = ((iParam0 - 6029) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 6029)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar10, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 7385 && iParam0 < 7641)
		{
			iVar11 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 7385), 0, 1, iParam2, "_NGDLCPSTAT_BOOL");
			iVar1 = ((iParam0 - 7385) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 7385)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar11, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 7321 && iParam0 < 7385)
		{
			iVar12 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 7321), 0, 0, 0, "_NGDLCPSTAT_BOOL");
			iVar1 = ((iParam0 - 7321) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 7321)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar12, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 9361 && iParam0 < 9553)
		{
			iVar13 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 9361), 0, 1, iParam2, "_DLCBIKEPSTAT_BOOL");
			iVar1 = ((iParam0 - 9361) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 9361)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar13, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 15369 && iParam0 < 15561)
		{
			iVar14 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 15369), 0, 1, iParam2, "_DLCGUNPSTAT_BOOL");
			iVar1 = ((iParam0 - 15369) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 15369)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar14, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 15562 && iParam0 < 15946)
		{
			iVar15 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 15562), 0, 1, iParam2, "_GUNTATPSTAT_BOOL");
			iVar1 = ((iParam0 - 15562) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 15562)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar15, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 15946 && iParam0 < 16010)
		{
			iVar16 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 15946), 0, 1, iParam2, "_DLCSMUGCHARPSTAT_BOOL");
			iVar1 = ((iParam0 - 15946) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 15946)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar16, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 18098 && iParam0 < 18162)
		{
			iVar17 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 18098), 0, 1, iParam2, "_GANGOPSPSTAT_BOOL");
			iVar1 = ((iParam0 - 18098) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 18098)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar17, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 22066 && iParam0 < 22194)
		{
			iVar18 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 22066), 0, 1, iParam2, "_BUSINESSBATPSTAT_BOOL");
			iVar1 = ((iParam0 - 22066) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 22066)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar18, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 24962 && iParam0 < 25538)
		{
			iVar19 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 24962), 0, 1, iParam2, "_ARENAWARSPSTAT_BOOL");
			iVar1 = ((iParam0 - 24962) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 24962)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar19, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 26810 && iParam0 < 27258)
		{
			iVar20 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 26810), 0, 1, iParam2, "_CASINOPSTAT_BOOL");
			iVar1 = ((iParam0 - 26810) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 26810)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar20, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 28098 && iParam0 < 28354)
		{
			iVar21 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 28098), 0, 1, iParam2, "_CASINOHSTPSTAT_BOOL");
			iVar1 = ((iParam0 - 28098) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 28098)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar21, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 28355 && iParam0 < 28483)
		{
			iVar22 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 28355), 0, 1, iParam2, "_HEIST3TATTOOSTAT_BOOL");
			iVar1 = ((iParam0 - 28355) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 28355)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar22, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 30227 && iParam0 < 30355)
		{
			iVar23 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 30227), 0, 1, iParam2, "_SU20PSTAT_BOOL");
			iVar1 = ((iParam0 - 30227) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 30227)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar23, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 30355 && iParam0 < 30483)
		{
			iVar24 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 30355), 0, 1, iParam2, "_SU20TATTOOSTAT_BOOL");
			iVar1 = ((iParam0 - 30355) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 30355)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar24, bParam1, iVar1, iParam3);
		}
		else if (iParam0 >= 30515 && iParam0 < 30707)
		{
			iVar25 = STATS::_GET_NGSTAT_BOOL_HASH((iParam0 - 30515), 0, 1, iParam2, "_HISLANDPSTAT_BOOL");
			iVar1 = ((iParam0 - 30515) - STATS::_STAT_GET_PACKED_BOOL_MASK((iParam0 - 30515)) * 64);
			iVar0 = STATS::STAT_SET_BOOL_MASKED(iVar25, bParam1, iVar1, iParam3);
		}
		return iVar0;
	}

	int packed_int(int iParam0, int iParam1, int iParam2, int iParam3)
	{
		int iVar0;
		int iVar1;
		int uVar2;

		if (iParam2 == -1)
		{
			iParam2 = character();
		}
		if (iParam1 < 0)
		{
			iParam1 = 255;
		}
		iVar0 = 0;
		iVar1 = 0;
		if (iParam0 >= 384 && iParam0 < 457)
		{
			iVar0 = STATS::GET_PACKED_INT_STAT_KEY((iParam0 - 384), 0, 1, iParam2);
			iVar1 = ((iParam0 - 384) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 384)) * 8) * 8;
		}
		else if (iParam0 >= 457 && iParam0 < 513)
		{
			iVar0 = STATS::GET_PACKED_INT_STAT_KEY((iParam0 - 457), 1, 1, iParam2);
			iVar1 = ((iParam0 - 457) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 457)) * 8) * 8;
		}
		else if (iParam0 >= 1281 && iParam0 < 1305)
		{
			iVar0 = STATS::GET_PACKED_INT_STAT_KEY((iParam0 - 1281), 0, 0, 0);
			iVar1 = ((iParam0 - 1281) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 1281)) * 8) * 8;
		}
		else if (iParam0 >= 1305 && iParam0 < 1361)
		{
			iVar0 = STATS::GET_PACKED_INT_STAT_KEY((iParam0 - 1305), 1, 0, 0);
			iVar1 = ((iParam0 - 1305) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 1305)) * 8) * 8;
		}
		else if (iParam0 >= 1393 && iParam0 < 2919)
		{
			iVar0 = STATS::GET_PACKED_TU_INT_STAT_KEY((iParam0 - 1393), 0, 1, iParam2);
			iVar1 = ((iParam0 - 1393) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 1393)) * 8) * 8;
		}
		else if (iParam0 >= 1361 && iParam0 < 1393)
		{
			iVar0 = STATS::GET_PACKED_TU_INT_STAT_KEY((iParam0 - 1361), 0, 0, 0);
			iVar1 = ((iParam0 - 1361) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 1361)) * 8) * 8;
		}
		else if (iParam0 >= 3879 && iParam0 < 4143)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 3879), 0, 1, iParam2, "_NGPSTAT_INT");
			iVar1 = ((iParam0 - 3879) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 3879)) * 8) * 8;
		}
		else if (iParam0 >= 4143 && iParam0 < 4207)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 4143), 0, 0, 0, "_NGPSTAT_INT");
			iVar1 = ((iParam0 - 4143) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 4143)) * 8) * 8;
		}
		else if (iParam0 >= 4399 && iParam0 < 6028)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 4399), 0, 1, iParam2, "_LRPSTAT_INT");
			iVar1 = ((iParam0 - 4399) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 4399)) * 8) * 8;
		}
		else if (iParam0 >= 6413 && iParam0 < 7262)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 6413), 0, 1, iParam2, "_APAPSTAT_INT");
			iVar1 = ((iParam0 - 6413) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 6413)) * 8) * 8;
		}
		else if (iParam0 >= 7262 && iParam0 < 7313)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 7262), 0, 1, iParam2, "_LR2PSTAT_INT");
			iVar1 = ((iParam0 - 7262) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 7262)) * 8) * 8;
		}
		else if (iParam0 >= 7681 && iParam0 < 9361)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 7681), 0, 1, iParam2, "_BIKEPSTAT_INT");
			iVar1 = ((iParam0 - 7681) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 7681)) * 8) * 8;
		}
		else if (iParam0 >= 9553 && iParam0 < 15265)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 9553), 0, 1, iParam2, "_IMPEXPPSTAT_INT");
			iVar1 = ((iParam0 - 9553) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 9553)) * 8) * 8;
		}
		else if (iParam0 >= 15265 && iParam0 < 15369)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 15265), 0, 1, iParam2, "_GUNRPSTAT_INT");
			iVar1 = ((iParam0 - 15265) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 15265)) * 8) * 8;
		}
		else if (iParam0 >= 16010 && iParam0 < 18098)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 16010), 0, 1, iParam2, "_DLCSMUGCHARPSTAT_INT");
			iVar1 = ((iParam0 - 16010) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 16010)) * 8) * 8;
		}
		else if (iParam0 >= 18162 && iParam0 < 19018)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 18162), 0, 1, iParam2, "_GANGOPSPSTAT_INT");
			iVar1 = ((iParam0 - 18162) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 18162)) * 8) * 8;
		}
		else if (iParam0 >= 19018 && iParam0 < 22066)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 19018), 0, 1, iParam2, "_BUSINESSBATPSTAT_INT");
			iVar1 = ((iParam0 - 19018) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 19018)) * 8) * 8;
		}
		else if (iParam0 >= 22194 && iParam0 < 24962)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 22194), 0, 1, iParam2, "_ARENAWARSPSTAT_INT");
			iVar1 = ((iParam0 - 22194) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 22194)) * 8) * 8;
		}
		else if (iParam0 >= 25538 && iParam0 < 26810)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 25538), 0, 1, iParam2, "_CASINOPSTAT_INT");
			iVar1 = ((iParam0 - 25538) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 25538)) * 8) * 8;
		}
		else if (iParam0 >= 27258 && iParam0 < 28098)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 27258), 0, 1, iParam2, "_CASINOHSTPSTAT_INT");
			iVar1 = ((iParam0 - 27258) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 27258)) * 8) * 8;
		}
		else if (iParam0 >= 28483 && iParam0 < 30227)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 28483), 0, 1, iParam2, "_SU20PSTAT_INT");
			iVar1 = ((iParam0 - 28483) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 28483)) * 8) * 8;
		}
		else if (iParam0 >= 30483 && iParam0 < 30515)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 30483), 0, 1, iParam2, "_HISLANDPSTAT_INT");
			iVar1 = ((iParam0 - 30483) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 30483)) * 8) * 8;
		}
		else if (iParam0 >= 7641 && iParam0 < 7681)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 7641), 0, 1, iParam2, "_NGDLCPSTAT_INT");
			iVar1 = ((iParam0 - 7641) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 7641)) * 8) * 8;
		}
		else if (iParam0 >= 7313 && iParam0 < 7321)
		{
			iVar0 = STATS::_GET_NGSTAT_INT_HASH((iParam0 - 7313), 0, 0, 0, "_NGDLCPSTAT_INT");
			iVar1 = ((iParam0 - 7313) - STATS::_STAT_GET_PACKED_INT_MASK((iParam0 - 7313)) * 8) * 8;
		}
		uVar2 = STATS::STAT_SET_MASKED_INT(iVar0, iParam1, iVar1, 8, iParam3);
		return uVar2;
	}

	void change_session(int id) {
		*script_global(SessionUNK1).as<int*>() = 0;
		*script_global(SessionType).as<int*>() = id;
		*script_global(InitSession).at(2).as<int*>() = 0;
		*script_global(SessionSCTV).as<int*>() = 0;
		*script_global(InitSession).as<int*>() = 1;
		*script_global(SessionUNK2).as<int*>() = 0;
		*script_global(SessionUNK3).as<int*>() = -1;
	}

	bool is_context_active(char* text)
	{
		HUD::BEGIN_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(text);
		return HUD::END_TEXT_COMMAND_IS_THIS_HELP_MESSAGE_BEING_DISPLAYED(0);
	}

	void gui::dx_on_tick()
	{
		if (ImGui::Begin("Recovery Suite EX 1.54", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::BeginMenu("Character Options")) {

				if (ImGui::Button("Delete Character 1")) {
					STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CHAR_ISACTIVE"), FALSE, TRUE);
					STATS::STAT_SAVE(0, 0, 3, 0);
					notification("Done!");
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This option will delete the character from the Slot 1.");

				ImGui::Separator();

				if (ImGui::Button("Delete Character 2")) {
					STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CHAR_ISACTIVE"), FALSE, TRUE);
					STATS::STAT_SAVE(0, 0, 3, 0);
					notification("Done!");
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This option will delete the character from the Slot 2.");

				ImGui::Separator();

				if (ImGui::Button("Delete Both Characters")) {
					STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CHAR_ISACTIVE"), FALSE, TRUE);
					STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CHAR_ISACTIVE"), FALSE, TRUE);
					STATS::STAT_SAVE(0, 0, 3, 0);
					notification("Done!");
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This option will delete the character from both Slots (1 & 2).");

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Session Options")) {

				if (ImGui::BeginMenu("Leave Online")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							*script_global(SessionUNK1).as<int*>() = -1;
							*script_global(SessionUNK4).as<int*>() = 14;
							*script_global(InitSession).at(2).as<int*>() = -1;
							*script_global(InitSession).as<int*>() = 1;
							*script_global(SessionUNK3).as<int*>() = 3;
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Choose Character")) {

					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							*script_global(SessionUNK1).as<int*>() = 0;
							*script_global(InitSession).at(2).as<int*>() = 0;
							*script_global(InitSession).as<int*>() = 65;
							*script_global(SessionUNK2).as<int*>() = 1;
							*script_global(SessionUNK3).as<int*>() = 4;
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Go to Creator Mode")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							*script_global(SessionUNK1).as<int*>() = 2;
							*script_global(InitSession).at(2).as<int*>() = 2;
							*script_global(InitSession).as<int*>() = 1;
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Go to GTA Online")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(-1);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Find Public Session")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(0);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Start New Public")) {

					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(1);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Closed Crew Session")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(2);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Crew Session")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(3);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Closed Friends")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(6);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Find Friends Session")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(9);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Start a Solo Session")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(10);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Invite Only Session")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(11);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Join Crew Session")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(12);
						});
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Join SocialClub TV")) {
					
					if (ImGui::Button("Go!")) {
						g_fiber_pool->queue_job([] {
							change_session(13);
							*script_global(SessionSCTV).as<int*>() = 1;
						});
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Misc Toggles")) {

				ImGui::Checkbox("Enable RP Loop", &g_gui.rp_loop);

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Unstable. Only use this option to level up your Crew Rank.");

				ImGui::Separator();

				ImGui::Checkbox("Enable Anti-AFK", &g_gui.anti_afk);

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Rank Options")) {

				ImGui::SetNextItemWidth(47);
				ImGui::SameLine(50.0f, 100.0f);
				ImGui::InputInt("Rank 1-8000", &g_gui.rank, 0, 0);

				ImGui::Separator();

				ImGui::RadioButton("R* Gift Admin", &g_gui.rp_mode, 1);

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This method requires changing session.");

				ImGui::SameLine();

				ImGui::RadioButton("Char XP FM", &g_gui.rp_mode, 2);

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This method is instant, you don't need to change session.");

				ImGui::SameLine();

				ImGui::RadioButton("Char Rank FM", &g_gui.rp_mode, 3);

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Also instant.");

				ImGui::Spacing();
				ImGui::Spacing();

				ImGui::SameLine(45.0f, 100.0f);
				if (ImGui::Button("Set Character Rank")) {
					g_fiber_pool->queue_job([]
						{
							if (character() == 0) {
								if (g_gui.rp_mode == 1) {
									if (g_gui.rank <= 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_SET_RP_GIFT_ADMIN"), 1, TRUE);
										STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), 1);
										notification("RP Set on Character 1! Change Session.");
									}
									if (g_gui.rank >= 2) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_SET_RP_GIFT_ADMIN"), rp_value(g_gui.rank), TRUE);
										STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), rp_value(g_gui.rank));
										notification("RP Set on Character 1! Change Session.");
									}
								}
								if (g_gui.rp_mode == 2) {
									if (g_gui.rank <= 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_XP_FM"), 1, TRUE);
										STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), 1);
										notification("RP Set on Character 1!");
									}
									if (g_gui.rank >= 2) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_XP_FM"), rp_value(g_gui.rank), TRUE);
										STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), rp_value(g_gui.rank));
										notification("RP Set on Character 1!");
									}
								}
								if (g_gui.rp_mode == 3) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_RANK_FM_SA"), g_gui.rank, TRUE);
									STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), rp_value(g_gui.rank));
									notification("Rank Set on Character 1!");
								}
							}
							if (character() == 1) {
								if (g_gui.rp_mode == 1) {
									if (g_gui.rank <= 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_SET_RP_GIFT_ADMIN"), 1, TRUE);
										STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), 1);
										notification("RP Set on Character 2! Change Session.");
									}
									if (g_gui.rank >= 2) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_SET_RP_GIFT_ADMIN"), rp_value(g_gui.rank), TRUE);
										STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), rp_value(g_gui.rank));
										notification("RP Set on Character 2! Change Session.");
									}
								}
								if (g_gui.rp_mode == 2) {
									if (g_gui.rank <= 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_XP_FM"), 1, TRUE);
										STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), 1);
										notification("RP Set on Character 2!");
									}
									if (g_gui.rank >= 2) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_XP_FM"), rp_value(g_gui.rank), TRUE);
										STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), rp_value(g_gui.rank));
										notification("RP Set on Character 2!");
									}
								}
								if (g_gui.rp_mode == 3) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_RANK_FM_SA"), g_gui.rank, TRUE);
									STATS::STAT_INCREMENT(RAGE_JOAAT("MPPLY_GLOBALXP"), rp_value(g_gui.rank));
									notification("Rank Set on Character 2!");
								}
							}
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("After you select the method, press this button.");

				ImGui::Separator();

				ImGui::Spacing();

				ImGui::SameLine(63.0f, 100.0f);
				if (ImGui::Button("Set Crew Rank")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_0"), rp_value(g_gui.rank), TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_1"), rp_value(g_gui.rank), TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_2"), rp_value(g_gui.rank), TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_3"), rp_value(g_gui.rank), TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_4"), rp_value(g_gui.rank), TRUE);
						Memory::set_value<int>({ 0x08, 0x10C8, 0x888 }, 1);
						script::get_current()->yield(100ms);
						Memory::set_value<int>({ 0x08, 0x10C8, 0x888 }, 0);
						STATS::STAT_SAVE(0, 0, 3, 0);
					});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Instant and permanent.");

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Heist Helper")) {

				if (ImGui::BeginMenu("Casino Heist")) {

					if (ImGui::BeginMenu("Approach Type")) {

						if (ImGui::Button("1 - Stealth")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_APPROACH"), 1, TRUE);
									notification("Approach Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_APPROACH"), 1, TRUE);
									notification("Approach Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("2 - Trickery")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_APPROACH"), 2, TRUE);
									notification("Approach Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_APPROACH"), 2, TRUE);
									notification("Approach Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("3 - Loud")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_APPROACH"), 3, TRUE);
									notification("Approach Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_APPROACH"), 3, TRUE);
									notification("Approach Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Heist Target")) {

						if (ImGui::Button("0 - Money")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 0, TRUE);
									notification("Target Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 0, TRUE);
									notification("Target Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("1 - Gold")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 1, TRUE);
									notification("Target Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 1, TRUE);
									notification("Target Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("2 - Art")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 2, TRUE);
									notification("Target Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 2, TRUE);
									notification("Target Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("3 - Diamond")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 3, TRUE);
									notification("Target Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 3, TRUE);
									notification("Target Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Disrupt Ship")) {

						if (ImGui::Button("Level 0 (None)")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_DISRUPTSHIP"), 0, TRUE);
									notification("Disrupt Ship Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_DISRUPTSHIP"), 0, TRUE);
									notification("Disrupt Ship Level Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Level 1")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_DISRUPTSHIP"), 1, TRUE);
									notification("Disrupt Ship Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_DISRUPTSHIP"), 1, TRUE);
									notification("Disrupt Ship Level Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Level 2")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_DISRUPTSHIP"), 2, TRUE);
									notification("Disrupt Ship Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_DISRUPTSHIP"), 2, TRUE);
									notification("Disrupt Ship Level Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Level 3")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_DISRUPTSHIP"), 3, TRUE);
									notification("Disrupt Ship Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_DISRUPTSHIP"), 3, TRUE);
									notification("Disrupt Ship Level Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Access Keys Level")) {

						if (ImGui::Button("Level 0 (None)")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_KEYLEVELS"), 0, TRUE);
									notification("Key Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_KEYLEVELS"), 0, TRUE);
									notification("Key Level Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Level 1")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_KEYLEVELS"), 1, TRUE);
									notification("Key Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_KEYLEVELS"), 1, TRUE);
									notification("Key Level Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Level 2")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_KEYLEVELS"), 2, TRUE);
									notification("Key Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_KEYLEVELS"), 2, TRUE);
									notification("Key Level Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Body Armor Levels")) {

						if (ImGui::Button("Level 0 (None)")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BODYARMORLVL"), 0, TRUE);
									notification("Body Armor Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BODYARMORLVL"), 0, TRUE);
									notification("Body Armor Level Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Level 1")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BODYARMORLVL"), 1, TRUE);
									notification("Body Armor Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BODYARMORLVL"), 1, TRUE);
									notification("Body Armor Level Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Level 2")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BODYARMORLVL"), 2, TRUE);
									notification("Body Armor Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BODYARMORLVL"), 2, TRUE);
									notification("Body Armor Level Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Level 3")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BODYARMORLVL"), 3, TRUE);
									notification("Body Armor Level Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BODYARMORLVL"), 3, TRUE);
									notification("Body Armor Level Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Weapon Crew Team")) {

						if (ImGui::Button("1 - Karl Abolaji")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWWEAP"), 1, TRUE);
									notification("Weapon Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWWEAP"), 1, TRUE);
									notification("Weapon Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("2 - Gustavo Motta")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWWEAP"), 2, TRUE);
									notification("Weapon Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWWEAP"), 2, TRUE);
									notification("Weapon Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("3 - Charlie Reed")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWWEAP"), 3, TRUE);
									notification("Weapon Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWWEAP"), 3, TRUE);
									notification("Weapon Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("4 - Chester McCoy")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWWEAP"), 4, TRUE);
									notification("Weapon Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWWEAP"), 4, TRUE);
									notification("Weapon Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("5 - Patrick McReary")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWWEAP"), 5, TRUE);
									notification("Weapon Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWWEAP"), 5, TRUE);
									notification("Weapon Crew Team Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Driver Crew Team")) {

						if (ImGui::Button("1 - Karim Deniz")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWDRIVER"), 1, TRUE);
									notification("Driver Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWDRIVER"), 1, TRUE);
									notification("Driver Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("2 - Taliana Martinez")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWDRIVER"), 2, TRUE);
									notification("Driver Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWDRIVER"), 2, TRUE);
									notification("Driver Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("3 - Eddie Toh")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWDRIVER"), 3, TRUE);
									notification("Driver Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWDRIVER"), 3, TRUE);
									notification("Driver Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("4 - Zach Nelson")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWDRIVER"), 4, TRUE);
									notification("Driver Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWDRIVER"), 4, TRUE);
									notification("Driver Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("5 - Chester McCoy")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWDRIVER"), 5, TRUE);
									notification("Driver Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWDRIVER"), 5, TRUE);
									notification("Driver Crew Team Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Hacker Crew Team")) {

						if (ImGui::Button("1 - Rickie Lukens")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWHACKER"), 1, TRUE);
									notification("Hacker Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWHACKER"), 1, TRUE);
									notification("Hacker Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("2 - Christian Feltz")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWHACKER"), 2, TRUE);
									notification("Hacker Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWHACKER"), 2, TRUE);
									notification("Hacker Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("3 - Yohan Blair")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWHACKER"), 3, TRUE);
									notification("Hacker Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWHACKER"), 3, TRUE);
									notification("Hacker Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("4 - Avi Schwartzman")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWHACKER"), 4, TRUE);
									notification("Hacker Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWHACKER"), 4, TRUE);
									notification("Hacker Crew Team Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("5 - Page Harris")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWHACKER"), 5, TRUE);
									notification("Hacker Crew Team Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWHACKER"), 5, TRUE);
									notification("Hacker Crew Team Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Heist Vehicles")) {

						if (ImGui::Button("Kit #1")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_VEHS"), 0, TRUE);
									notification("Vehicle Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_VEHS"), 0, TRUE);
									notification("Vehicle Kit Set!");
								}
								});
						}

						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Includes: Issi Classic, Retinue MK II, Sultan Classic, Manchez, Zhaba");

						ImGui::Separator();

						if (ImGui::Button("Kit #2")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_VEHS"), 1, TRUE);
									notification("Vehicle Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_VEHS"), 1, TRUE);
									notification("Vehicle Kit Set!");
								}
								});
						}

						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Includes: Asbo, Drift Yosemite, Guantlet Classic, Stryder, Vagrant");

						ImGui::Separator();

						if (ImGui::Button("Kit #3")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_VEHS"), 2, TRUE);
									notification("Vehicle Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_VEHS"), 2, TRUE);
									notification("Vehicle Kit Set!");
								}
								});
						}

						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Includes: Kanjo, Sugoi, Ellie, Defiler, Outlaw");

						ImGui::Separator();

						if (ImGui::Button("Kit #4")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_VEHS"), 3, TRUE);
									notification("Vehicle Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_VEHS"), 3, TRUE);
									notification("Vehicle Kit Set!");
								}
								});
						}

						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Includes: Sentinel Classic, Jugular, Komoda, Lectro, Everon");

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Heist Weapons")) {

						if (ImGui::Button("Kit #1")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_WEAPS"), 0, TRUE);
									notification("Heist Weapon Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_WEAPS"), 0, TRUE);
									notification("Heist Weapon Kit Set!");
								}
								});
						}

						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Includes: Shotgun, Rifle, SMG, SMG MK II, Shotgun");

						ImGui::Separator();

						if (ImGui::Button("Kit #2")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_WEAPS"), 1, TRUE);
									notification("Heist Weapon Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_WEAPS"), 1, TRUE);
									notification("Heist Weapon Kit Set!");
								}
								});
						}

						if (ImGui::IsItemHovered())
							ImGui::SetTooltip("Includes: Revolver, Shotgun, Shotgun, Rifle MK II, Combat MG");

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Heist Masks")) {

						if (ImGui::Button("1 - Geometric Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 1, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 1, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("2 - Hunter Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 2, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 2, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("3 - Oni Half Mask Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 3, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 3, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("4 - Emoji Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 4, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 4, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("5 - Ornate Skull Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 5, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 5, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("6 - Lucky Fruit Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 6, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 6, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("7 - Gurilla Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 7, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 7, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("8 - Clown Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 8, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 8, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("9 - Animal Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 9, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 9, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("10 - Riot Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 10, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 10, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("11 - Oni Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 11, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 11, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("12 - Hockey Set")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_MASKS"), 12, TRUE);
									notification("Heist Masks Kit Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_MASKS"), 12, TRUE);
									notification("Heist Masks Kit Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Hard Approach/Last Approach")) {

						if (ImGui::BeginMenu("Hard Approach")) {

							if (ImGui::Button("1 - Stealth")) {
								g_fiber_pool->queue_job([] {
									if (character() == 0) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_HARD_APPROACH"), 1, TRUE);
										notification("Hard Approach Set!");
									}
									if (character() == 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_HARD_APPROACH"), 1, TRUE);
										notification("Hard Approach Set!");
									}
									});
							}

							ImGui::Separator();

							if (ImGui::Button("2 - Trickery")) {
								g_fiber_pool->queue_job([] {
									if (character() == 0) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_HARD_APPROACH"), 2, TRUE);
										notification("Hard Approach Set!");
									}
									if (character() == 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_HARD_APPROACH"), 2, TRUE);
										notification("Hard Approach Set!");
									}
									});
							}

							ImGui::Separator();

							if (ImGui::Button("3 - Loud")) {
								g_fiber_pool->queue_job([] {
									if (character() == 0) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_HARD_APPROACH"), 3, TRUE);
										notification("Hard Approach Set!");
									}
									if (character() == 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_HARD_APPROACH"), 3, TRUE);
										notification("Hard Approach Set!");
									}
									});
							}

							ImGui::EndMenu();
						}

						ImGui::Separator();

						if (ImGui::BeginMenu("Last Approach")) {

							if (ImGui::Button("Reset Last Approach")) {
								g_fiber_pool->queue_job([] {
									if (character() == 0) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_LAST_APPROACH"), 0, TRUE);
										notification("Last Approach Reset!");
									}
									if (character() == 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_LAST_APPROACH"), 0, TRUE);
										notification("Last Approach Reset!");
									}
									});
							}

							ImGui::Separator();

							if (ImGui::Button("1 - Stealth")) {
								g_fiber_pool->queue_job([] {
									if (character() == 0) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_LAST_APPROACH"), 1, TRUE);
										notification("Last Approach Set!");
									}
									if (character() == 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_LAST_APPROACH"), 1, TRUE);
										notification("Last Approach Set!");
									}
									});
							}

							ImGui::Separator();

							if (ImGui::Button("2 - Trickery")) {
								g_fiber_pool->queue_job([] {
									if (character() == 0) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_LAST_APPROACH"), 2, TRUE);
										notification("Last Approach Set!");
									}
									if (character() == 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_LAST_APPROACH"), 2, TRUE);
										notification("Last Approach Set!");
									}
									});
							}

							ImGui::Separator();

							if (ImGui::Button("3 - Loud")) {
								g_fiber_pool->queue_job([] {
									if (character() == 0) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_LAST_APPROACH"), 3, TRUE);
										notification("Last Approach Set!");
									}
									if (character() == 1) {
										STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_LAST_APPROACH"), 3, TRUE);
										notification("Last Approach Set!");
									}
									});
							}

							ImGui::EndMenu();
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Misc Options")) {

						if (ImGui::Button("Skip the First Board")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BITSET1"), -1, TRUE);
									notification("Done!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BITSET1"), -1, TRUE);
									notification("Done!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Skip the Second Board")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_BITSET0"), -1, TRUE);
									notification("Done!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_BITSET0"), -1, TRUE);
									notification("Done!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Reset Heist Cooldown")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_COMPLETEDPOSIX"), 1, TRUE);
									STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_H3_COOLDOWN"), 1, TRUE);
									notification("Cooldown Reset!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_COMPLETEDPOSIX"), 1, TRUE);
									STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_H3_COOLDOWN"), 1, TRUE);
									notification("Cooldown Reset!");
								}
								});
						}

						ImGui::Separator();

						if (ImGui::Button("Remove Lester Cut")) {
							g_fiber_pool->queue_job([] {
								if (character() == 0) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWWEAP"), 6, TRUE);
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWDRIVER"), 6, TRUE);
									STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_CREWHACKER"), 6, TRUE);
									notification("Stats Set!");
								}
								if (character() == 1) {
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWWEAP"), 6, TRUE);
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWDRIVER"), 6, TRUE);
									STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_CREWHACKER"), 6, TRUE);
									notification("Stats Set!");
								}
								});
						}

						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Cayo Perico Heist")) {

					if (ImGui::Button("Skip Heist Prep")) {
						g_fiber_pool->queue_job([] {
							if (character() == 0) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_BS_GEN"), 131071, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_WEAPONS"), 1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_WEP_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_ARM_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_HEL_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_TARGET"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_BOLTCUT"), 4424, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_UNIFORM"), 5256, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_GRAPPEL"), 5156, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_TROJAN"), 1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_CASH_I"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_CASH_I_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_CASH_C"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_CASH_C_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_COKE_I"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_COKE_I_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_COKE_C"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_COKE_C_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_GOLD_I"), 16777215, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_GOLD_I_SCOPED"), 16777215, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_GOLD_C"), 255, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_GOLD_C_SCOPED"), 255, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_WEED_I"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_WEED_I_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_WEED_C"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_WEED_C_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_PAINT"), 127, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_PAINT_SCOPED"), 127, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_GOLD_V"), 598484, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_PAINT_V"), 448863, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4_PROGRESS"), 124271, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4_MISSIONS"), 65535, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4_PLAYTHROUGH_STATUS"), 40000, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Done!");
							}
							if (character() == 1) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_BS_GEN"), 131071, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_WEAPONS"), 1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_WEP_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_ARM_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_HEL_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_TARGET"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_BOLTCUT"), 4424, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_UNIFORM"), 5256, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_GRAPPEL"), 5156, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_TROJAN"), 1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_CASH_I"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_CASH_I_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_CASH_C"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_CASH_C_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_COKE_I"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_COKE_I_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_COKE_C"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_COKE_C_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_GOLD_I"), 16777215, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_GOLD_I_SCOPED"), 16777215, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_GOLD_C"), 255, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_GOLD_C_SCOPED"), 255, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_WEED_I"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_WEED_I_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_WEED_C"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_WEED_C_SCOPED"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_PAINT"), 127, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_PAINT_SCOPED"), 127, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_GOLD_V"), 598484, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_PAINT_V"), 448863, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4_PROGRESS"), 124271, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4_MISSIONS"), 65535, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4_PLAYTHROUGH_STATUS"), 40000, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Done!");
							}
						});
					}

					ImGui::Separator();

					if (ImGui::Button("Enable Hard Mode")) {
						g_fiber_pool->queue_job([] {
							if (character() == 0) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4_PROGRESS"), 131055, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Done!");
							}
							if (character() == 1) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4_PROGRESS"), 131055, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Done!");
							}
						});
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Casino Options")) {

				if (ImGui::BeginMenu("Slot Machines")) {

					if (ImGui::BeginMenu("Slot Machines Sound Options")) {

						if (ImGui::CollapsingHeader("Angel and Knight")) {
							if (ImGui::Button("Remove Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 9;
										});
									notification("Sound Removed! Before leaving the machine, restore the sound.");
									});
							}
							ImGui::SameLine();
							if (ImGui::Button("Restore Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 1;
										});
									notification("Sound Restored!");
									});
							}
						}

						if (ImGui::CollapsingHeader("Impotent Rage")) {
							if (ImGui::Button("Remove Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 9;
										});
									notification("Sound Removed! Before leaving the machine, restore the sound.");
									});
							}
							ImGui::SameLine();
							if (ImGui::Button("Restore Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 2;
										});
									notification("Sound Restored!");
									});
							}
						}

						if (ImGui::CollapsingHeader("Space Rangers")) {
							if (ImGui::Button("Remove Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 9;
										});
									notification("Sound Removed! Before leaving the machine, restore the sound.");
									});
							}
							ImGui::SameLine();
							if (ImGui::Button("Restore Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 3;
										});
									notification("Sound Restored!");
									});
							}
						}

						if (ImGui::CollapsingHeader("Fame or Shame")) {
							if (ImGui::Button("Remove Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 9;
										});
									notification("Sound Removed! Before leaving the machine, restore the sound.");
									});
							}
							ImGui::SameLine();
							if (ImGui::Button("Restore Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 4;
										});
									notification("Sound Restored!");
									});
							}
						}

						if (ImGui::CollapsingHeader("Deity of the Sun")) {
							if (ImGui::Button("Remove Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 9;
										});
									notification("Sound Removed! Before leaving the machine, restore the sound.");
									});
							}
							ImGui::SameLine();
							if (ImGui::Button("Restore Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 5;
										});
									notification("Sound Restored!");
									});
							}
						}

						if (ImGui::CollapsingHeader("Twilight Knife")) {
							if (ImGui::Button("Remove Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 9;
										});
									notification("Sound Removed! Before leaving the machine, restore the sound.");
									});
							}
							ImGui::SameLine();
							if (ImGui::Button("Restore Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 6;
										});
									notification("Sound Restored!");
									});
							}
						}

						if (ImGui::CollapsingHeader("Diamond Miner")) {
							if (ImGui::Button("Remove Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 9;
										});
									notification("Sound Removed! Before leaving the machine, restore the sound.");
									});
							}
							ImGui::SameLine();
							if (ImGui::Button("Restore Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 7;
										});
									notification("Sound Restored!");
									});
							}
						}

						if (ImGui::CollapsingHeader("Evacuator")) {
							if (ImGui::Button("Remove Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 9;
										});
									notification("Sound Removed! Before leaving the machine, restore the sound.");
									});
							}
							ImGui::SameLine();
							if (ImGui::Button("Restore Sound")) {
								g_fiber_pool->queue_job([] {
									gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
										auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
										auto local_219 = script_local(slots_thread, 219);
										auto local_2131 = script_local(slots_thread, 2131).as<int*>();

										*local_219.at(*local_2131, 21).at(17).as<int*>() = 8;
										});
									notification("Sound Restored!");
									});
							}
						}

						ImGui::EndMenu();
					}

					ImGui::Separator();

					ImGui::Checkbox("Trigger Slot Machine Transaction", &g_gui.force_trans);

					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("This option will trigger a transaction without playing.");

					ImGui::Separator();

					ImGui::Checkbox("Auto-Play Slot Machine", &g_gui.auto_play);

					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Enabling this option will automatically play the slot machine for you.");

					ImGui::Separator();

					ImGui::Checkbox("Always Jackpot in Slot Machines", &g_gui.always_jackpot);

					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("After enabling this option you'll always win in any Slot Machine.");

					ImGui::Separator();

					ImGui::Checkbox("Enable Instant Spinning", &g_gui.fast_spin);

					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Enabling this option will instantly spin the machine.");

					ImGui::Separator();

					ImGui::Checkbox("Always 2,500,000$ Prize", &g_gui.slots_bet);

					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("This option will set the minimum bet to 500 so you can bet up to 2,500 and then win 2,500,000$ in any slot machine.");

					ImGui::Separator();

					ImGui::Checkbox("Force Stop Animation", &g_gui.stop_anim);

					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("When you enable this option, you won't need to wait for the animation to play again.");

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Blackjack")) {

					ImGui::Checkbox("Auto-Play Blackjack", &g_gui.auto_play_blackjack);

					ImGui::Separator();

					ImGui::Checkbox("Rig Blackjack", &g_gui.rig_blackjack);

					ImGui::EndMenu();
				}

				ImGui::EndMenu();

			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Unlock Options")) {

				if (ImGui::Button("Unlock All Stuff")) {
					g_fiber_pool->queue_job([]
						{
							if (character() == 0) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_GV_BS_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_CLOTHES_RM_BS_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_WEAPON_GV_BS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_WEAPON_GV_BS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_WEAPON_GV_BS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_WEAPON_RM_BS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_WEAPON_RM_BS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADMIN_WEAPON_RM_BS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_4_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_5_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_6_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_7_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_8_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_9_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_10_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_11_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_1_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_2_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_3_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_4_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_5_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_6_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_7_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_8_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_9_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_10_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CLOTHES_11_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_32"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_33"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_34"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_35"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_36"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_37"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_38"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_39"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_40"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_41"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_42"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_43"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_44"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TATTOO_FM_UNLOCKS_45"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AIR_LAUNCHES_OVER_40M"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_5STAR_WANTED_AVOIDANCE"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ACTIVATE_2_PERSON_KEY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ALL_ROLES_HEIST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_BUY_EVERY_GUN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_CARS_EXPORTED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_CONTROL_CROWDS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_COMPLETEDAILYOBJ"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_COMPLETEDAILYOBJTOTAL"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTALDAYCOMPLETED"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTALWEEKCOMPLETED"), 400, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTALMONTHCOMPLETED"), 1800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CONSECUTIVEDAYCOMPLETED"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CONSECUTIVEWEEKCOMPLETED"), 4, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CONSECUTIVEMONTHCOMPLETE"), 1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_COMPLETEDAILYOBJSA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_COMPLETEDAILYOBJTOTALSA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTALDAYCOMPLETEDSA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTALWEEKCOMPLETEDSA"), 400, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTALMONTHCOMPLETEDSA"), 1800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CONSECUTIVEDAYCOMPLETEDSA"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CONSECUTIVEWEEKCOMPLETEDSA"), 4, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CONSECUTIVEMONTHCOMPLETESA"), 1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DAILYOBJCOMPLETEDSA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DAILYOBJCOMPLETED"), 100, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_DAILYOBJMONTHBONUS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_DAILYOBJWEEKBONUS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_DAILYOBJWEEKBONUSSA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_DAILYOBJMONTHBONUSSA"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DO_HEIST_AS_MEMBER"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DO_HEIST_AS_THE_LEADER"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_COMPLETE_HEIST_NOT_DIE"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_MATCHING_OUTFIT_HEIST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_DRIVELESTERCAR5MINS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DROPOFF_CAP_PACKAGES"), 100, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FINISH_HEIST_NO_DAMAGE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FINISH_HEIST_SETUP_JOB"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FINISH_HEISTS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_DIFFERENT_RACES"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_DIFFERENT_DM"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FM25DIFFERENTDM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FM25DIFFERENTRACES"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FM25DIFITEMSCLOTHES"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_HAIR"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_HAIR_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_HAIR_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_HAIR_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_HAIR_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_HAIR_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_HAIR_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_HAIR_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_JBIB"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_JBIB_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_JBIB_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_JBIB_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_JBIB_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_JBIB_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_JBIB_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_JBIB_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_LEGS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_LEGS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_LEGS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_LEGS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_LEGS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_LEGS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_LEGS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_LEGS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_FEET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_FEET_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_FEET_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_FEET_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_FEET_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_FEET_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_FEET_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_FEET_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_BERD"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_BERD_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_BERD_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_BERD_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_BERD_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_BERD_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_BERD_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_BERD_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_PROPS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_USED_OUTFIT"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMFURTHESTWHEELIE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FM6DARTCHKOUT"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_DM_3KILLSAMEGUY"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_DM_KILLSTREAK"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_DM_STOLENKILL"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_DM_TOTALKILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_DM_WINS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_GOLF_BIRDIES"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FM_GOLF_HOLE_IN_1"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_GOLF_WON"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_GTA_RACES_WON"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_RACE_LAST_FIRST"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_RACES_FASTEST_LAP"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_SHOOTRANG_CT_WON"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FM_SHOOTRANG_GRAN_WON"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_SHOOTRANG_RT_WON"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_SHOOTRANG_TG_WON"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_TDM_MVP"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_TDM_WINS"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FM_TENNIS_5_SET_WINS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_TENNIS_ACE"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FM_TENNIS_STASETWIN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FM_TENNIS_WON"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMATTGANGHQ"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMBASEJMP"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMBBETWIN"), 50000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMCRATEDROPS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMDRIVEWITHOUTCRASH"), 30, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMFULLYMODDEDCAR"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMHORDWAVESSURVIVE"), 10, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMKILL3ANDWINGTARACE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMKILLBOUNTY"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMKILLSTREAKSDM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMMOSTKILLSGANGHIDE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMMOSTKILLSSURVIVE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMPICKUPDLCCRATE1ST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMRACEWORLDRECHOLDER"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMRALLYWONDRIVE"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMRALLYWONNAV"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMREVENGEKILLSDM"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMSHOOTDOWNCOPHELI"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMTATTOOALLBODYPARTS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMWINAIRRACE"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMWINALLRACEMODES"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMWINCUSTOMRACE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FMWINEVERYGAMEMODE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMWINRACETOPOINTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FMWINSEARACE"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_HOLD_UP_SHOPS"), 20, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_KILL_CARRIER_CAPTURE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_KILL_PSYCHOPATHS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_KILL_TEAM_YOURSELF_LTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_LAPDANCES"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_LESTERDELIVERVEHICLES"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_MENTALSTATE_TO_NORMAL"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_NIGHTVISION_KILLS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_NO_HAIRCUTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_ODISTRACTCOPSNOEATH"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_ONLY_PLAYER_ALIVE_LTS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_PARACHUTE_JUMPS_20M"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_PARACHUTE_JUMPS_50M"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_PASSENGERTIME"), 4, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_PICKUP_CAP_PACKAGES"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_RACES_WON"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_SECURITY_CARS_ROBBED"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_SPLIT_HEIST_TAKE_EVENLY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_STORE_20_CAR_IN_GARAGES"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_TAKEDOWNSMUGPLANE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_TIME_IN_HELICOPTER"), 4, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_TRADE_IN_YOUR_PROPERTY"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_VEHICLES_JACKEDR"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_WIN_AT_DARTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_WIN_CAPTURE_DONT_DYING"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_WIN_CAPTURES"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_WIN_GOLD_MEDAL_HEISTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_WIN_LAST_TEAM_STANDINGS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BOTTLE_IN_POSSESSION"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_MELEECHLENGECOMPLETED"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_HEADSHOTCHLENGECOMPLETED"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_NAVYREVOLVERCOMPLETED"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CARS_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_NEAR_MISS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CARS_COPS_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BIKES_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HELIS_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PLANES_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BOATS_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_QUADBIKE_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NO_CARS_REPAIR"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_VEHICLES_SPRAYED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_CRASHES_CARS"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_CRASHES_BIKES"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_CRASHES_QUADBIKES"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BAILED_FROM_VEHICLE"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CARMOD_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CARMOD_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CARMOD_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CARMOD_4_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CARMOD_5_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CARMOD_6_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_CARMOD_7_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_VEHICLE_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_VEHICLE_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_WEAP_ADDON_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_WEAP_ADDON_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_WEAP_ADDON_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_WEAP_ADDON_4_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_WEAP_ADDON_5_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_WEAP_UNLOCKED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_WEAP_UNLOCKED2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_10_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_11_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_12_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_1_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_2_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_3_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_4_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_5_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_6_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_7_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_8_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_9_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_KIT_FM_PURCHASE9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WANTED_LEVEL_TIME"), 7200000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WANTED_LEVEL_TIME5STAR"), 7200000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WEAP_FM_PURCHASE"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WEAP_FM_PURCHASE2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_BERD"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_BERD_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_BERD_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_BERD_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_BERD_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_BERD_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_BERD_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_DECL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_FEET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_FEET_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_FEET_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_FEET_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_FEET_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_FEET_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_FEET_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_FEET_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_JBIB"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_JBIB_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_JBIB_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_JBIB_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_JBIB_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_JBIB_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_JBIB_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_JBIB_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_LEGS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_LEGS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_LEGS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_LEGS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_LEGS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_LEGS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_LEGS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_LEGS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_OUTFIT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_PROPS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL2_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_SPECIAL_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_TEETH"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_TEETH_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_TEETH_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_ACQUIRED_TORSO"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_BERD"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_BERD_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_BERD_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_BERD_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_BERD_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_BERD_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_BERD_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_BERD_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_DECL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_FEET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_FEET_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_FEET_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_FEET_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_FEET_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_FEET_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_FEET_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_FEET_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_HAIR"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_HAIR_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_HAIR_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_HAIR_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_HAIR_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_HAIR_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_HAIR_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_HAIR_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_JBIB"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_JBIB_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_JBIB_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_JBIB_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_JBIB_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_JBIB_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_JBIB_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_JBIB_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_LEGS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_LEGS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_LEGS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_LEGS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_LEGS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_LEGS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_LEGS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_LEGS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_OUTFIT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_PROPS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL2_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_SPECIAL_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_TEETH"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_TEETH_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_TEETH_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLTHS_AVAILABLE_TORSO"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_32"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_33"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_34"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_35"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_36"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_37"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_38"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_39"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_40"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_41"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_42"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_43"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_44"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_45"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_46"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_47"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_48"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_49"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_50"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_51"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_52"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_53"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_54"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_55"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_56"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_57"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_58"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_59"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_60"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_61"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_62"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_63"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_64"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_65"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_66"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_67"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_68"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_69"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_70"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_71"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_72"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_73"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_74"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_75"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_76"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_77"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_78"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_79"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_80"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_81"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_82"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_83"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_84"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_85"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_86"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_87"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_88"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_89"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_90"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_91"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_92"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_93"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_94"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_95"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_96"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_97"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_98"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_99"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_100"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_101"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_102"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_103"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_104"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_105"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_106"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_107"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_108"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_109"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_110"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_111"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_112"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_113"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_114"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_115"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_116"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_117"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_118"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_119"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_120"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_121"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_122"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_123"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_124"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_125"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_126"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_127"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_128"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_129"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_130"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_131"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_132"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_133"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_134"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_135"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_136"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_137"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_138"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_139"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_140"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_141"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_142"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_143"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_144"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_145"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_146"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_147"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_148"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_149"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_150"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_151"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_152"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_153"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_154"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_155"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_156"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_157"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_158"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_159"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_160"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_161"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_162"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_163"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_164"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_165"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_166"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_167"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_168"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_169"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_170"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_171"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_172"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_173"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_174"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_175"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_176"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_177"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_178"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_179"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_180"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_181"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_182"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_183"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_184"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_185"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_186"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_187"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_188"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_189"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_190"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_191"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_192"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_193"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_194"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_195"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_196"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_197"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_198"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_199"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_200"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_201"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_202"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_203"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_204"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_ACQUIRED_205"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_32"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_33"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_34"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_35"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_36"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_37"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_38"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_39"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_40"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_41"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_42"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_43"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_44"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_45"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_46"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_47"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_48"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_49"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_50"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_51"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_52"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_53"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_54"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_55"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_56"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_57"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_58"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_59"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_60"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_61"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_62"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_63"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_64"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_65"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_66"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_67"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_68"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_69"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_70"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_71"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_72"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_73"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_74"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_75"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_76"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_77"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_78"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_79"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_80"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_81"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_82"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_83"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_84"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_85"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_86"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_87"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_88"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_89"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_90"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_91"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_92"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_93"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_94"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_95"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_96"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_97"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_98"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_99"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_100"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_101"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_102"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_103"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_104"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_105"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_106"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_107"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_108"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_109"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_110"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_111"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_112"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_113"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_114"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_115"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_116"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_117"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_118"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_119"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_120"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_121"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_122"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_123"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_124"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_125"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_126"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_127"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_128"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_129"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_130"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_131"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_132"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_133"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_134"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_135"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_136"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_137"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_138"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_139"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_140"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_141"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_142"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_143"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_144"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_145"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_146"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_147"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_148"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_149"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_150"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_151"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_152"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_153"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_154"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_155"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_156"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_157"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_158"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_159"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_160"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_161"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_162"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_163"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_164"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_165"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_166"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_167"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_168"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_169"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_170"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_171"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_172"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_173"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_174"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_175"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_176"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_177"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_178"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_179"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_180"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_181"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_182"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_183"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_184"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_185"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_186"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_187"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_188"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_189"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_190"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_191"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_192"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_193"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_194"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_195"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_196"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_197"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_198"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_199"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_200"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_201"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_202"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_203"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_204"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DLC_APPAREL_USED_205"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GRENADE_ENEMY_KILLS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MICROSMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ASLTSMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ASLTRIFLE_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRBNRIFLE_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ADVRIFLE_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CMBTMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ASLTMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_RPG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_KILLS_PLAYERS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LONGEST_WHEELIE_DIST"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MOST_ARM_WRESTLING_WINS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MOST_FLIPS_IN_ONE_JUMP"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MOST_SPINS_IN_ONE_JUMP"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_SLIPSTREAMS_IN_RACE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_TURBO_STARTS_IN_RACE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PASS_DB_PLAYER_KILLS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PISTOL_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PLAYER_HEADSHOTS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_RACES_WON"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SAWNOFF_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCRIPT_INCREASE_DRIV"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCRIPT_INCREASE_FLY"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCRIPT_INCREASE_LUNG"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCRIPT_INCREASE_MECH"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCRIPT_INCREASE_SHO"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCRIPT_INCREASE_STAM"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCRIPT_INCREASE_STL"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCRIPT_INCREASE_STRN"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_CREATOR_STAMINA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_CREATOR_SHOOTING"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_CREATOR_STRENGTH"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_CREATOR_STEALTH"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_CREATOR_FLYING"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_CREATOR_BIKE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_CREATOR_LUNG"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_CREATOR_MECHANIC"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_STKYBMB_ENEMY_KILLS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_UNARMED_ENEMY_KILLS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_USJS_FOUND"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_USJS_FOUND_MASK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_USJS_TOTAL_COMPLETED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_USJS_COMPLETED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_USJS_COMPLETED_MASK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WEAP_FM_ADDON_PURCH"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WEAP_FM_ADDON_PURCH2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WEAP_FM_ADDON_PURCH3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WEAP_FM_ADDON_PURCH4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WEAP_FM_ADDON_PURCH5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRDEADLINE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_ABILITY_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_ABILITY_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_ABILITY_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_ABILITY_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_ABILITY_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_ABILITY_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BUY_COMPLETE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BUY_UNDERTAKEN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_SELL_COMPLETE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_SELL_UNDERTAKEN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_CONTRA_EARNINGS"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BIKER_BUY_COMPLET"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BIKER_BUY_UNDERTA"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BIKER_SELL_COMPLET"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BIKER_SELL_UNDERTA"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_COMPLET1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_UNDERTA1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_COMPLET1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_UNDERTA1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_COMPLET2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_UNDERTA2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_COMPLET2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_UNDERTA2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_COMPLET3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_UNDERTA3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_COMPLET3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_UNDERTA3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_COMPLET4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_UNDERTA4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_COMPLET4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_UNDERTA4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_COMPLET5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_BUY_UNDERTA5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_COMPLET5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_BIKER_SELL_UNDERTA5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SELL_COMPLETBC"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SELL_UNDERTABC"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_COMPLETBC1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_UNDERTABC1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_COMPLETBC2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_UNDERTABC2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_COMPLETBC3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_UNDERTABC3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_COMPLETBC4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_UNDERTABC4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_COMPLETBC5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SEL_UNDERTABC5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SELL_EARNINGS0"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SELL_EARNINGS1"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SELL_EARNINGS2"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SELL_EARNINGS3"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SELL_EARNINGS4"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_BKR_SELL_EARNINGS5"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_IE_EXPORT_COMPLETED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_IE_MISSION_EARNINGS"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_HANGAR_BUY_UNDETAK"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_HANGAR_BUY_COMPLET"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_HANGAR_SEL_UNDETAK"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_HANGAR_SEL_COMPLET"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LFETIME_HANGAR_EARNINGS"), 20000000, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_0"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_1"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_3"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_4"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_5"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_6"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_7"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_8"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PILOT_ASPASSEDLESSON_9"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PILOT_SCHOOL_MEDAL_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SR_HIGHSCORE_1"), 950, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SR_HIGHSCORE_2"), 2450, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SR_HIGHSCORE_3"), 3450, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SR_HIGHSCORE_4"), 3350, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SR_HIGHSCORE_5"), 4150, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SR_HIGHSCORE_6"), 600, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SR_TARGETS_HIT"), 300, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SR_WEAPON_BIT_SET"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_SR_TIER_1_REWARD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_SR_TIER_3_REWARD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_SR_INCREASE_THROW_CAP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_COMPLET_HEIST_MEM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_COMPLET_HEIST_1STPER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_FLEECA_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_HST_ORDER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_HST_SAME_TEAM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_HST_ULT_CHAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_HUMANE_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_PACIFIC_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_PRISON_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_SERIESA_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_PLANNING_DONE_PRINT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_PLANNING_DONE_HELP_0"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_PLANNING_DONE_HELP_1"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_PRE_PLAN_DONE_HELP_0"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_CUTS_DONE_FINALE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_CUTS_DONE_ORNATE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_CUTS_DONE_BIOLAB"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_CUTS_DONE_PRISON"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_CUTS_DONE_NARCOTIC"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_CUTS_DONE_TUTORIAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_AWARD_DONE_PREP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HEIST_AWARD_BOUGHT_IN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_FLEECA_PREP_1"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_FLEECA_PREP_2"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_FLEECA_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PRISON_PLANE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PRISON_BUS"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PRISON_STATION"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PRISON_UNFINISHED_BIZ"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PRISON_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_HUMANE_KEY_CODES"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_HUMANE_ARMORDILLOS"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_HUMANE_EMP"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_HUMANE_VALKYRIE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_HUMANE_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_NARC_COKE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_NARC_TRASH_TRUCK"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_NARC_BIKERS"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_NARC_WEED"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_NARC_STEAL_METH"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_NARC_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PACIFIC_TRUCKS"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PACIFIC_WITSEC"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PACIFIC_HACK"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PACIFIC_BIKES"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PACIFIC_CONVOY"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PACIFIC_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_ALLINORDER"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_LOYALTY"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_CRIMMASMD"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_LOYALTY2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_LOYALTY3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_CRIMMASMD2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_CRIMMASMD3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_SUPPORT"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_GANGOPS_IAA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_GANGOPS_SUBMARINE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_GANGOPS_MISSILE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GANGOPS_HEIST_STATUS"), -229384, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GANGOPS_FM_MISSION_PROG"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GANGOPS_FLOW_NOTIFICATIONS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GANGOPS_FLOW_MISSION_PROG"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WAM_FLOW_VEHICLE_BS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GANGOPS_FLOW_IMPEXP_NUM"), 32, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NO_BOUGHT_YUM_SNACKS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DANCE_TO_SOLOMUN"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DANCE_TO_TALEOFUS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DANCE_TO_DIXON"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DANCE_TO_BLKMAD"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_CLUB_DRUNK"), 200, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_CLUB_COORD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_CLUB_HOTSPOT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_CLUB_CLUBBER"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NIGHTCLUB_PLAYER_APPEAR"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMUNIQUEPLYSINCLUB"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DANCETODIFFDJS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DANCE_COMBO_DURATION_MINS"), 240, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SOLCOUNTER"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOSCOUNTER"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DIXCOUNTER"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BMCOUNTER"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DANCEPERFECTOWNCLUB"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NIGHTCLUB_HOTSPOT_TIME_MS"), 3600000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BUSINESSHUB_CONT_TOTAL"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BUSINESSHUB_CONT_MISSION"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NIGHTCLUB_CONT_TOTAL"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NIGHTCLUB_CONT_MISSION"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_HUB_GOODS_SOLD"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LIFETIME_HUB_GOODS_MADE"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FMBB_FLOW_NOTIFICATIONS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FMBB_PCM_PROGRESS"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_BEGINNER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FIELD_FILLER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ARMCHAIR_RACER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_LEARNER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_SUNDAY_DRIVER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_THE_ROOKIE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_BUMP_AND_RUN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_GEAR_HEAD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_DOOR_SLAMMER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_HOT_LAP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ARENA_AMATEUR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_PAINT_TRADER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_SHUNTER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_JOCK"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_WARRIOR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_T_BONE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_MAYHEM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_WRECKER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_CRASH_COURSE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ARENA_LEGEND"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_PEGASUS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_CONTACT_SPORT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_UNSTOPPABLE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_WATCH_YOUR_STEP"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_TOWER_OFFENSE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_READY_FOR_WAR"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_THROUGH_A_LENS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_SPINNER"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_YOUMEANBOOBYTRAPS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_MASTER_BANDITO"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_SITTING_DUCK"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_CROWDPARTICIPATION"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_KILL_OR_BE_KILLED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_MASSIVE_SHUNT"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_YOURE_OUTTA_HERE"), 200, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_WEVE_GOT_ONE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_ARENA_WAGEWORKER"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_TIME_SERVED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_TOP_SCORE"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_CAREER_WINNER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_SP"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_SP_LIFETIME"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_SKILL_LEVEL"), 20, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_AP"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_AP_TIER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_AP_LIFETIME"), 5055000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_CARRER_UNLK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_THEME_SCIFI"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_THEME_APOC"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_THEME_CONS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_PASS_THE_BOMB"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_DETONATION"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_ARCADE_RACE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_CTF"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_TAG_TEAM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_DESTR_DERBY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_CARNAGE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_MONSTER_JAM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_W_GAMES_MASTERS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_L_PASS_THE_BOMB"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_L_DETONATION"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_L_ARCADE_RACE"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_L_CTF"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_L_TAG_TEAM"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_L_DESTR_DERBY"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_L_CARNAGE"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_L_MONSTER_JAM"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_L_GAMES_MASTERS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_OF_CHAMP_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_SPECTATOR_KILLS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_LIFETIME_KILLS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_LIFETIME_DEATHS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_CARRER_WINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_CARRER_WINT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_MATCHES_PLYD"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_MATCHES_PLYDT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_SPEC_BOX_TIME_MS"), 86400000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_SPECTATOR_DRONE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_SPECTATOR_CAMS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_SMOKE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_DRINK"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_MONSTER3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_MONSTER4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_MONSTER5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_CERBERUS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_CERBERUS2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_CERBERUS3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_BRUISER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_BRUISER2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_BRUISER3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_SLAMVAN4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_SLAMVAN5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_SLAMVAN6"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_BRUTUS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_BRUTUS2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_BRUTUS3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_SCARAB"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_SCARAB2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_SCARAB3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_DOMINATOR4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_DOMINATOR5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_DOMINATOR6"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_IMPALER2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_IMPALER3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_IMPALER4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_ISSI4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_ISSI5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_ISSI6"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_IMPERATOR"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_IMPERATOR2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_IMPERATOR3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_ZR380"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_ZR3802"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_ZR3803"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_DEATHBIKE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_DEATHBIKE2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_VEH_DEATHBIKE3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_BS_TRINKET_TICKERS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_BS_TRINKET_SAVED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NO_BOUGHT_HEALTH_SNACKS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NO_BOUGHT_EPIC_SNACKS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_OF_ORANGE_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MP_CHAR_ARMOUR_1_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MP_CHAR_ARMOUR_2_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MP_CHAR_ARMOUR_3_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MP_CHAR_ARMOUR_4_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MP_CHAR_ARMOUR_5_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_OF_BOURGE_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CIGARETTES_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_1_WHITE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_1_RED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_1_BLUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_2_WHITE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_2_RED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_2_BLUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_3_WHITE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_3_RED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_3_BLUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_4_WHITE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_4_RED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWORK_TYPE_4_BLUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_AWD_FM_CR_DM_MADE"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_AWD_FM_CR_PLAYED_BY_PEEP"), 10000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_AWD_FM_CR_RACES_MADE"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_AWD_FM_CR_MISSION_SCORE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_NUM_CAPTURES_CREATED"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_LTS_CREATED"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_FM_MISSION_LIKES"), 5000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_ACT_PHN"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_ACT_PH2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_ACT_PH3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_ACT_PH4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_ACT_PH5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_VEH_TX1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_ACT_PH6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_ACT_PH7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_ACT_PH8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_ACT_PH9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_CUT_DONE"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FM_CUT_DONE_2"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_FM_INTRO_CUT_DONE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_FM_INTRO_MISS_DONE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_SHOOTINGRANGE_SEEN_TUT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_TENNIS_SEEN_TUTORIAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_DARTS_SEEN_TUTORIAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_ARMWRESTLING_SEEN_TUTORIAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HAS_WATCHED_BENNY_CUTSCE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_LOW_FLOW_CS_DRV_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_LOW_FLOW_CS_TRA_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_LOW_FLOW_CS_FUN_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_LOW_FLOW_CS_PHO_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_LOW_FLOW_CS_FIN_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_LOW_BEN_INTRO_CS_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LOWRIDER_FLOW_COMPLETE"), 9, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LOW_FLOW_CURRENT_PROG"), 9, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LOW_FLOW_CURRENT_CALL"), 9, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_MORGUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_DELUXO"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_SERVERFARM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_IAABASE_FIN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_STEALOSPREY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_FOUNDRY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_RIOTVAN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_SUBMARINECAR"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_SUBMARINE_FIN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_PREDATOR"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_BMLAUNCHER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_BCCUSTOM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_STEALTHTANKS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_SPYPLANE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_FINALE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_FINALE_P2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGOP_FINALE_P3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SNIPERRFL_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HVYSNIPER_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_SPENT_DEATHMAT"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_SPENT_RACES"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_MISSION_CREATO"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP_FIRST_PERSON_CAM_TIME"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_RACES_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_RACES_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_CUSTOM_RACES_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_DEATHMATCH_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_DEATHMATCH_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TDEATHMATCH_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TDEATHMATCH_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_SHOOTINGRANGE_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_SHOOTINGRANGE_LOSSES"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TENNIS_MATCHES_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TENNIS_MATCHES_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GOLF_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GOLF_LOSSES"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DARTS_TOTAL_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DARTS_TOTAL_MATCHES"), 750, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_SHOOTINGRANGE_TOTAL_MATCH"), 800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_BJ_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_BJ_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_RACE_2_POINT_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_RACE_2_POINT_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_JOBS"), MISC::GET_RANDOM_INT_IN_RANGE(500000, 1250000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_SELLING_VEH"), MISC::GET_RANDOM_INT_IN_RANGE(500000, 2000000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_BETTING"), MISC::GET_RANDOM_INT_IN_RANGE(5000, 50000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_GOOD_SPORT"), MISC::GET_RANDOM_INT_IN_RANGE(5000, 50000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_PICKED_UP"), MISC::GET_RANDOM_INT_IN_RANGE(1000, 30000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_SHARED"), MISC::GET_RANDOM_INT_IN_RANGE(800, 10000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_JOBSHARED"), MISC::GET_RANDOM_INT_IN_RANGE(1000, 50000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_ROCKSTAR_AWARD"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_WEAPON_ARMOR"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_VEH_MAINTENANCE"), MISC::GET_RANDOM_INT_IN_RANGE(10000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_STYLE_ENT"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_PROPERTY_UTIL"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_JOB_ACTIVITY"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_CONTACT_SERVICE"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_HEALTHCARE"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_DROPPED_STOLEN"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_SHARED"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_SPENT_JOBSHARED"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTAL_PLAYING_TIME"), 2147483647, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME"), 2147483647, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME_NEW"), 2147483647, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MP_FIRST_PERSON_CAM_TIME"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_KILLS_PLAYERS"), 2500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DEATHS_PLAYER"), 1660, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MPPLY_KILL_DEATH_RATIO"), 1.50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_KILLS_COP"), 7800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_KILLS_SWAT"), 8000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_STOLEN_COP_VEHICLE"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_STOLEN_CARS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_STOLEN_BIKES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_STOLEN_BOATS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_STOLEN_HELIS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_STOLEN_PLANES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_STOLEN_QUADBIKES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NUMBER_STOLEN_BICYCLES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NO_TIMES_WANTED_LEVEL"), 5000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_STARS_ATTAINED"), 5000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_STARS_EVADED"), 4000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTAL_CHASE_TIME"), 432000000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP0_LAST_CHASE_TIME"), 432000000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP0_LONGEST_CHASE_TIME"), 432000000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP0_TOTAL_TIME_MAX_STARS"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_VEHEXPORTED"), 10000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTAL_NO_SHOPS_HELD_UP"), 1000, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FIRST_TIME1"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FIRST_TIME2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FIRST_TIME3"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FIRST_TIME4"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FIRST_TIME5"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FIRST_TIME6"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ALL_IN_ORDER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_SUPPORTING_ROLE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_LEADER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_SURVIVALIST"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_ODD_JOBS"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_VCM_FLOW_CS_RSC_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_VCM_FLOW_CS_BWL_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_VCM_FLOW_CS_MTG_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_VCM_FLOW_CS_OIL_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_VCM_FLOW_CS_DEF_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_VCM_FLOW_CS_FIN_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CAS_VEHICLE_REWARD"), FALSE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_VCM_FLOW_PROGRESS"), 1839072, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_VCM_STORY_PROGRESS"), 0, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_SCOPEOUT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_CREWEDUP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_MOVINGON"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_PROMOCAMP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_GUNMAN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_SMASHNGRAB"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_INPLAINSI"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_UNDETECTED"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ALLROUND"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ELITETHEIF"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_PRO"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_SUPPORTACT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_SHAFTED"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_COLLECTOR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_DEADEYE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_PISTOLSATDAWN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_TRAFFICAVOI"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_CANTCATCHBRA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_WIZHARD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_APEESCAPE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_MONKEYKIND"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_AQUAAPE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_KEEPFAITH"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_TRUELOVE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_NEMESIS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_FRIENDZONED"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_PREPARATION"), 40, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_ASLEEPONJOB"), 20, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_DAICASHCRAB"), 100000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_BIGBRO"), 40, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_SHARPSHOOTER"), 40, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_RACECHAMP"), 40, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_BATSWORD"), 1000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_COINPURSE"), 950000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_ASTROCHIMP"), 3000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_MASTERFUL"), 40000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCGW_NUM_WINS_GANG_0"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCGW_NUM_WINS_GANG_1"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCGW_NUM_WINS_GANG_2"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SCGW_NUM_WINS_GANG_3"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_SCGW_WON_NO_DEATHS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_IAP_MAX_MOON_DIST"), 2147483647, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_IAP_CHALLENGE_0"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_IAP_CHALLENGE_1"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_IAP_CHALLENGE_2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_IAP_CHALLENGE_3"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_IAP_CHALLENGE_4"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_IAP_GOLD_TANK"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HAS_TAKEN_PEYOTE_BEFORE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SIGNAL_JAMMERS_COLLECTED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GANGOPS_FLOW_BITSET_MISS0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GANGOPS_FM_BITSET_MISS0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GANGOPS_FLOW_PASSED_BITSET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AT_FLOW_VEHICLE_BS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AT_FLOW_MISSION_PROGRESS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AT_FLOW_IMPEXP_NUM"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AT_FLOW_BITSET_MISSIONS0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WVM_FLOW_VEHICLE_BS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WVM_FLOW_BITSET_MISSIONS0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WVM_FLOW_MISSION_PROGRESS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_WVM_FLOW_IMPEXP_NUM"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CH_ARC_CAB_CLAW_TROPHY"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CH_ARC_CAB_LOVE_TROPHY"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NIGHTCLUB_JOBS_DONE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HUB_SALES_COMPLETED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NIGHTCLUB_VIP_APPEAR"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGATTACK_CITY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGATTACK_COUNTRY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGATTACK_LOST"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GANGATTACK_VAGOS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GHKILLS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HORDELVL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HORDKILLS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_UNIQUECRATES"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BJWINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HORDEWINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MCMWINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_GANGHIDWINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRARMWREST"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRBASEJUMP"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRDARTS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRDM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRGANGHIDE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRGOLF"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRHORDE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRMISSION"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRSHOOTRNG"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRPILOTSCHOOL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRDISTRICTCOPS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRTENNIS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CRHEIST"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_RACES"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CP_STOLEN_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CP_WELCOMETUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CP_WEA_AMBUSH"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_HOOKER_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_DRUGS_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_RIVAL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_SELLCARTUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_HOLDUP_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_TRIGGER_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_WELCOME"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_GUNSHOPTUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_ARMORYTUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_HOSTAGE_STEAL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_STEAL_BIKES"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_AIRPORTTAKE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PROP_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_SHIPMENTSTEAL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_SHOWROOM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_STEALVEHICLE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_VANRESCUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_BANKHEIST1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_COUNTHEIST1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HOLDUPS_BITSET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AMBIENT_MIS_BS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MEMBERSMARKEDFORDEATH"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MCKILLS"), 2500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MCDEATHS"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_RIVALPRESIDENTKILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_RIVALCEOANDVIPKILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MELEEKILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLUBHOUSECONTRACTSCOMPLETE"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLUBHOUSECONTRACTEARNINGS"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLUBWORKCOMPLETED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLUBCHALLENGESCOMPLETED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MEMBERCHALLENGESCOMPLETED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CAS_HEIST_NOTS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CAS_HEIST_FLOW"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_ACCESSPOINTS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_POI"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_KEYLEVELS"), 2, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3OPT_TARGET"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_H3_COOLDOWN"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_COMPLETEDPOSIX"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3MAN_MISSIONBS"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_REV_DA_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_REV_DA_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_SHATCHET_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_SHATCHET_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_MOLOTOV_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_MOLOTOV_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MOLOTOV_FM_AMMO_BOUGHT"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MOLOTOV_FM_AMMO_CURRENT"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_STEALTH_1A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_STEALTH_2B_RAPP"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_STEALTH_2C_SIDE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_STEALTH_3A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_STEALTH_4A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_STEALTH_5A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_SUBTERFUGE_1A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_SUBTERFUGE_2A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_SUBTERFUGE_2B"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_SUBTERFUGE_3A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_SUBTERFUGE_3B"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_SUBTERFUGE_4A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_SUBTERFUGE_5A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_DIRECT_1A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_DIRECT_2A1"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_DIRECT_2A2"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_DIRECT_2BP"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_DIRECT_2C"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_DIRECT_3A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_DIRECT_4A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_CR_DIRECT_5A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_BOARD_DIALOGUE0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_BOARD_DIALOGUE1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_BOARD_DIALOGUE2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H3_VEHICLESUSED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_ORDER"), 100, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_FURIA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_MINITANK"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_YOSEMITE2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_ZHABA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_IMORGEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_SULTAN2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_VAGRANT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_VSTR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_STRYDER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_SUGOI"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_KANJO"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_FORMULA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_FORMULA2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELP_JB7002"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWRK_FM_AMMO_BOUGHT"), 9999, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_FIREWRK_FM_AMMO_CURRENT"), 9999, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MUSKET_FM_AMMO_BOUGHT"), 9999, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MUSKET_FM_AMMO_CURRENT"), 9999, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_FIREWRK_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_FIREWRK_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_MUSKET_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_MUSKET_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CE_SP_FLOW_NOTIFICATIONS"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_PLANE_ACCESS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_BOAT_ACCESS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_HELI_ACCESS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_DOES_PLAYER_HAVE_VEH_ACCESS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_KINGOFHILLWINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_KINGOFHILLLOSS"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TIME_AS_A_PASSENGER"), 14400000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TIMEINHELI"), 14400000, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_RACE_MODDED_CAR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_DRIVE_RALLY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PLAY_GTA_RACE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PLAY_FOOT_RACE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PLAY_TEAM_DM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PLAY_VEHICLE_DM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PLAY_MISSION_CONTACT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PLAY_A_PLAYLIST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PLAY_POINT_TO_POINT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PLAY_ONE_ON_ONE_DM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PLAY_ONE_ON_ONE_RACE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_SURV_A_BOUNTY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_SET_WANTED_LVL_ON_PLAY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_GANG_BACKUP_GANGS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_GANG_BACKUP_LOST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_GANG_BACKUP_VAGOS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_CALL_MERCENARIES"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_PHONE_MECH_DROP_CAR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_GONE_OFF_RADAR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_FILL_TITAN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_MOD_CAR_USING_APP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_BUY_APPARTMENT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_BUY_INSURANCE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_BUY_GARAGE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_ENTER_FRIENDS_HOUSE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_CALL_STRIPPER_HOUSE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_CALL_FRIEND"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_SEND_FRIEND_REQUEST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_CL_W_WANTED_PLAYER_TV"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CRMISSION"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_MCMWIN"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CRHORDE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_HORDEWINS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_BEAMHACK"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_COUNT_HOTRING_RACE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_PACKAGE_1_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_PACKAGE_2_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_PACKAGE_3_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_PACKAGE_4_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_PACKAGE_5_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_PACKAGE_6_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_PACKAGE_7_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_PACKAGE_8_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_PACKAGE_9_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_NO_FM_PACKAGES_COL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_HEALTH_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_FM_HEALTH_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_XMAS_NORM_CLOTHES_SAVED"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_XMAS_NORM_CLOTHES_TOP"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_XMAS_NORM_CLOTHES_LOWERS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_XMAS_NORM_CLOTHES_FEET"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_VALENTINES_REC_CLOTHES"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_XMAS_NORM_CLOTHES_SPECIALS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_UNLOCK_EXCLUS_CONTENT"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_KINGOFQUB3D"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_QUBISM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_QUIBITS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_GODOFQUB3D"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_GOFOR11TH"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ELEVENELEVEN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MOVIE_PROPS_COLLECTED"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_YACHT_MISSION_PROG"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_YACHT_MISSION_FLOW"), 21845, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CASINO_DECORATION_GIFT_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WEAP_UNLOCKED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WEAP_UNLOCKED2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WEAP_ADDON_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WEAP_ADDON_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WEAP_ADDON_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_WEAP_ADDON_4_UNLCK"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_INTELGATHER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_COMPOUNDINFILT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_LOOT_FINDER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_MAX_DISRUPT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_THE_ISLAND_HEIST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_GOING_ALONE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_TEAM_WORK"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_MIXING_UP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_PRO_THIEF"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_CAT_BURGLAR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ONE_OF_THEM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_GOLDEN_GUN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ELITE_THIEF"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_PROFESSIONAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_HELPING_OUT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_COURIER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_PARTY_VIBES"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_HELPING_HAND"), TRUE, TRUE); 
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_AWD_ELEVENELEVEN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_LOSTANDFOUND"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_SUNSET"), 1800000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_TREASURE_HUNTER"), 1000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_WRECK_DIVING"), 1000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_KEINEMUSIK"), 1800000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_PALMS_TRAX"), 1800000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_MOODYMANN"), 1800000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_FILL_YOUR_BAGS"), 200000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_AWD_WELL_PREPARED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4_H4_DJ_MISSIONS"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_COMPLETE_H4_F_USING_VETIR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_COMPLETE_H4_F_USING_LONGFIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_COMPLETE_H4_F_USING_ANNIH"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_COMPLETE_H4_F_USING_ALKONOS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP0_COMPLETE_H4_F_USING_PATROLB"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_BS_GEN"), 131071, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_BS_ENTR"), 63, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_BS_ABIL"), 63, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_WEAPONS"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_WEP_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_ARM_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_HEL_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_TARGET"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_GRAPPEL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_UNIFORM"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_BOLTCUT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4CNF_APPROACH"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_CASH_V"), 89000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_WEED_V"), 148200, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_COKE_V"), 223800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_GOLD_V"), 328800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_PAINT_V"), 175200, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4_PROGRESS"), 130667, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_CASH_I_SCOPED"), 6559764, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_CASH_C_SCOPED"), 28, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_WEED_I_SCOPED"), 42656, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_COKE_I_SCOPED"), 720971, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_GOLD_C_SCOPED"), 131, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4LOOT_PAINT_SCOPED"), 48, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4_MISSIONS"), 65535, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_H4_PLAYTHROUGH_STATUS"), 7000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_SUBMARINE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_STRATEGIC_BOMBER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_SMUGGLER_PLANE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_STEALTH_HELI"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PATROL_BOAT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_SMUGGLER_BOAT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_TEQUILA"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PEARL_NECKLACE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_PINK_DIAMOND"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_MADRAZO_FILES"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CR_SAPHIREPANSTAT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DB_KILLS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PASS_DB_KILLS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TIRES_POPPED_BY_GUNSHOT"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HIGHEST_SKITTLES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SHOTS"), 50000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP0_WEAPON_ACCURACY"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HITS_PEDS_VEHICLES"), 25000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_KILLS"), 50000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_HEADSHOTS"), 10000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_KILLS_ARMED"), 18000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP0_KILLS_IN_FREE_AIM"), 8890, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_KILLS_STEALTH"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_SUCCESSFUL_COUNTERS"), 800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DM_HIGHEST_KILLSTREAK"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DEATHS"), 10000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DIED_IN_EXPLOSION"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DIED_IN_FALL"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DIED_IN_FIRE"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DIED_IN_ROAD"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_DIED_IN_DROWNING"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_NO_PHOTOS_TAKEN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LAP_DANCED_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PROSTITUTES_FREQUENTED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BOUNTPLACED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_BOUNTSONU"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MC_CONTRIBUTION_POINTS"), 1000, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Unlock All Done on Character 1!");
							}
							if (character() == 1) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_GV_BS_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_CLOTHES_RM_BS_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_WEAPON_GV_BS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_WEAPON_GV_BS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_WEAPON_GV_BS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_WEAPON_RM_BS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_WEAPON_RM_BS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADMIN_WEAPON_RM_BS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_4_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_5_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_6_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_7_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_8_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_9_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_10_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_11_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_1_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_2_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_3_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_4_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_5_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_6_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_7_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_8_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_9_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_10_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CLOTHES_11_OWNED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_32"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_33"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_34"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_35"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_36"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_37"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_38"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_39"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_40"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_41"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_42"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_43"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_44"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TATTOO_FM_UNLOCKS_45"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AIR_LAUNCHES_OVER_40M"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_5STAR_WANTED_AVOIDANCE"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ACTIVATE_2_PERSON_KEY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ALL_ROLES_HEIST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_BUY_EVERY_GUN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_CAR_BOMBS_ENEMY_KILLS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_CARS_EXPORTED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_CONTROL_CROWDS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_COMPLETEDAILYOBJ"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_COMPLETEDAILYOBJTOTAL"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTALDAYCOMPLETED"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTALWEEKCOMPLETED"), 400, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTALMONTHCOMPLETED"), 1800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CONSECUTIVEDAYCOMPLETED"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CONSECUTIVEWEEKCOMPLETED"), 4, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CONSECUTIVEMONTHCOMPLETE"), 1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_COMPLETEDAILYOBJSA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_COMPLETEDAILYOBJTOTALSA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTALDAYCOMPLETEDSA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTALWEEKCOMPLETEDSA"), 400, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTALMONTHCOMPLETEDSA"), 1800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CONSECUTIVEDAYCOMPLETEDSA"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CONSECUTIVEWEEKCOMPLETEDSA"), 4, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CONSECUTIVEMONTHCOMPLETESA"), 1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DAILYOBJCOMPLETEDSA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DAILYOBJCOMPLETED"), 100, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_DAILYOBJMONTHBONUS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_DAILYOBJWEEKBONUS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_DAILYOBJWEEKBONUSSA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_DAILYOBJMONTHBONUSSA"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DO_HEIST_AS_MEMBER"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DO_HEIST_AS_THE_LEADER"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_COMPLETE_HEIST_NOT_DIE"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_MATCHING_OUTFIT_HEIST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_DRIVELESTERCAR5MINS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DROPOFF_CAP_PACKAGES"), 100, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FINISH_HEIST_NO_DAMAGE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FINISH_HEIST_SETUP_JOB"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FINISH_HEISTS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_DIFFERENT_RACES"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_DIFFERENT_DM"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FM25DIFFERENTDM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FM25DIFFERENTRACES"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FM25DIFITEMSCLOTHES"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_HAIR"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_HAIR_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_HAIR_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_HAIR_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_HAIR_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_HAIR_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_HAIR_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_HAIR_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_JBIB"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_JBIB_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_JBIB_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_JBIB_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_JBIB_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_JBIB_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_JBIB_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_JBIB_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_LEGS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_LEGS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_LEGS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_LEGS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_LEGS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_LEGS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_LEGS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_LEGS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_FEET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_FEET_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_FEET_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_FEET_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_FEET_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_FEET_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_FEET_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_FEET_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_BERD"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_BERD_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_BERD_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_BERD_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_BERD_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_BERD_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_BERD_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_BERD_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_PROPS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_USED_OUTFIT"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMFURTHESTWHEELIE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FM6DARTCHKOUT"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_DM_3KILLSAMEGUY"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_DM_KILLSTREAK"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_DM_STOLENKILL"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_DM_TOTALKILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_DM_WINS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_GOLF_BIRDIES"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FM_GOLF_HOLE_IN_1"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_GOLF_WON"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_GTA_RACES_WON"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_RACE_LAST_FIRST"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_RACES_FASTEST_LAP"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_SHOOTRANG_CT_WON"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FM_SHOOTRANG_GRAN_WON"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_SHOOTRANG_RT_WON"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_SHOOTRANG_TG_WON"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_TDM_MVP"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_TDM_WINS"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FM_TENNIS_5_SET_WINS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_TENNIS_ACE"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FM_TENNIS_STASETWIN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FM_TENNIS_WON"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMATTGANGHQ"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMBASEJMP"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMBBETWIN"), 50000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMCRATEDROPS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMDRIVEWITHOUTCRASH"), 30, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMFULLYMODDEDCAR"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMHORDWAVESSURVIVE"), 10, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMKILL3ANDWINGTARACE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMKILLBOUNTY"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMKILLSTREAKSDM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMMOSTKILLSGANGHIDE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMMOSTKILLSSURVIVE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMPICKUPDLCCRATE1ST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMRACEWORLDRECHOLDER"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMRALLYWONDRIVE"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMRALLYWONNAV"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMREVENGEKILLSDM"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMSHOOTDOWNCOPHELI"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMTATTOOALLBODYPARTS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMWINAIRRACE"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMWINALLRACEMODES"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMWINCUSTOMRACE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FMWINEVERYGAMEMODE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMWINRACETOPOINTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FMWINSEARACE"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_HOLD_UP_SHOPS"), 20, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_KILL_CARRIER_CAPTURE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_KILL_PSYCHOPATHS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_KILL_TEAM_YOURSELF_LTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_LAPDANCES"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_LESTERDELIVERVEHICLES"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_MENTALSTATE_TO_NORMAL"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_NIGHTVISION_KILLS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_NO_HAIRCUTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_ODISTRACTCOPSNOEATH"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_ONLY_PLAYER_ALIVE_LTS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_PARACHUTE_JUMPS_20M"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_PARACHUTE_JUMPS_50M"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_PASSENGERTIME"), 4, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_PICKUP_CAP_PACKAGES"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_RACES_WON"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_SECURITY_CARS_ROBBED"), 25, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_SPLIT_HEIST_TAKE_EVENLY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_STORE_20_CAR_IN_GARAGES"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_TAKEDOWNSMUGPLANE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_TIME_IN_HELICOPTER"), 4, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_TRADE_IN_YOUR_PROPERTY"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_VEHICLES_JACKEDR"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_WIN_AT_DARTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_WIN_CAPTURE_DONT_DYING"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_WIN_CAPTURES"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_WIN_GOLD_MEDAL_HEISTS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_WIN_LAST_TEAM_STANDINGS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BOTTLE_IN_POSSESSION"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_MELEECHLENGECOMPLETED"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_HEADSHOTCHLENGECOMPLETED"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_NAVYREVOLVERCOMPLETED"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CARS_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_NEAR_MISS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CARS_COPS_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BIKES_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HELIS_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PLANES_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BOATS_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_QUADBIKE_EXPLODED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NO_CARS_REPAIR"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_VEHICLES_SPRAYED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_CRASHES_CARS"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_CRASHES_BIKES"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_CRASHES_QUADBIKES"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BAILED_FROM_VEHICLE"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CARMOD_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CARMOD_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CARMOD_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CARMOD_4_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CARMOD_5_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CARMOD_6_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_CARMOD_7_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_VEHICLE_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_VEHICLE_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_WEAP_ADDON_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_WEAP_ADDON_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_WEAP_ADDON_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_WEAP_ADDON_4_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_WEAP_ADDON_5_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_WEAP_UNLOCKED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_WEAP_UNLOCKED2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_10_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_11_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_12_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_1_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_2_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_3_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_4_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_5_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_6_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_7_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_8_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_9_FM_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_KIT_FM_PURCHASE9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WANTED_LEVEL_TIME"), 7200000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WANTED_LEVEL_TIME5STAR"), 7200000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WEAP_FM_PURCHASE"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WEAP_FM_PURCHASE2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_BERD"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_BERD_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_BERD_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_BERD_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_BERD_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_BERD_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_BERD_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_DECL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_FEET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_FEET_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_FEET_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_FEET_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_FEET_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_FEET_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_FEET_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_FEET_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_JBIB"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_JBIB_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_JBIB_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_JBIB_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_JBIB_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_JBIB_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_JBIB_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_JBIB_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_LEGS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_LEGS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_LEGS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_LEGS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_LEGS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_LEGS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_LEGS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_LEGS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_OUTFIT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_PROPS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL2_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_SPECIAL_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_TEETH"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_TEETH_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_TEETH_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_ACQUIRED_TORSO"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_BERD"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_BERD_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_BERD_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_BERD_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_BERD_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_BERD_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_BERD_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_BERD_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_DECL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_FEET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_FEET_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_FEET_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_FEET_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_FEET_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_FEET_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_FEET_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_FEET_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_HAIR"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_HAIR_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_HAIR_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_HAIR_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_HAIR_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_HAIR_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_HAIR_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_HAIR_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_JBIB"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_JBIB_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_JBIB_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_JBIB_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_JBIB_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_JBIB_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_JBIB_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_JBIB_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_LEGS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_LEGS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_LEGS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_LEGS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_LEGS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_LEGS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_LEGS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_LEGS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_OUTFIT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_PROPS_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL2_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_SPECIAL_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_TEETH"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_TEETH_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_TEETH_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLTHS_AVAILABLE_TORSO"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_32"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_33"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_34"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_35"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_36"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_37"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_38"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_39"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_40"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_41"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_42"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_43"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_44"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_45"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_46"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_47"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_48"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_49"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_50"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_51"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_52"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_53"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_54"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_55"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_56"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_57"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_58"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_59"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_60"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_61"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_62"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_63"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_64"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_65"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_66"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_67"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_68"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_69"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_70"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_71"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_72"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_73"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_74"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_75"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_76"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_77"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_78"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_79"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_80"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_81"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_82"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_83"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_84"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_85"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_86"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_87"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_88"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_89"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_90"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_91"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_92"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_93"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_94"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_95"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_96"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_97"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_98"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_99"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_100"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_101"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_102"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_103"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_104"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_105"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_106"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_107"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_108"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_109"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_110"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_111"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_112"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_113"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_114"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_115"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_116"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_117"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_118"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_119"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_120"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_121"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_122"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_123"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_124"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_125"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_126"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_127"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_128"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_129"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_130"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_131"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_132"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_133"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_134"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_135"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_136"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_137"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_138"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_139"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_140"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_141"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_142"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_143"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_144"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_145"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_146"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_147"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_148"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_149"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_150"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_151"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_152"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_153"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_154"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_155"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_156"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_157"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_158"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_159"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_160"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_161"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_162"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_163"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_164"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_165"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_166"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_167"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_168"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_169"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_170"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_171"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_172"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_173"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_174"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_175"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_176"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_177"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_178"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_179"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_180"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_181"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_182"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_183"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_184"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_185"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_186"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_187"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_188"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_189"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_190"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_191"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_192"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_193"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_194"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_195"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_196"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_197"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_198"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_199"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_200"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_201"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_202"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_203"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_204"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_ACQUIRED_205"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_17"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_18"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_19"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_20"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_21"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_22"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_23"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_24"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_25"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_26"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_27"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_28"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_29"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_30"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_31"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_32"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_33"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_34"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_35"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_36"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_37"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_38"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_39"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_40"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_41"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_42"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_43"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_44"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_45"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_46"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_47"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_48"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_49"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_50"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_51"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_52"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_53"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_54"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_55"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_56"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_57"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_58"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_59"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_60"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_61"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_62"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_63"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_64"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_65"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_66"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_67"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_68"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_69"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_70"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_71"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_72"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_73"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_74"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_75"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_76"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_77"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_78"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_79"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_80"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_81"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_82"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_83"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_84"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_85"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_86"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_87"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_88"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_89"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_90"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_91"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_92"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_93"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_94"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_95"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_96"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_97"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_98"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_99"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_100"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_101"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_102"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_103"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_104"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_105"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_106"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_107"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_108"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_109"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_110"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_111"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_112"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_113"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_114"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_115"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_116"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_117"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_118"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_119"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_120"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_121"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_122"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_123"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_124"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_125"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_126"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_127"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_128"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_129"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_130"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_131"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_132"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_133"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_134"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_135"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_136"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_137"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_138"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_139"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_140"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_141"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_142"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_143"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_144"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_145"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_146"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_147"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_148"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_149"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_150"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_151"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_152"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_153"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_154"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_155"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_156"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_157"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_158"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_159"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_160"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_161"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_162"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_163"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_164"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_165"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_166"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_167"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_168"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_169"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_170"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_171"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_172"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_173"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_174"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_175"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_176"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_177"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_178"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_179"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_180"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_181"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_182"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_183"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_184"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_185"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_186"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_187"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_188"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_189"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_190"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_191"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_192"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_193"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_194"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_195"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_196"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_197"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_198"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_199"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_200"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_201"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_202"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_203"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_204"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DLC_APPAREL_USED_205"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GRENADE_ENEMY_KILLS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MICROSMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ASLTSMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ASLTRIFLE_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRBNRIFLE_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ADVRIFLE_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CMBTMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ASLTMG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_RPG_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_KILLS_PLAYERS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LONGEST_WHEELIE_DIST"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MOST_ARM_WRESTLING_WINS"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MOST_FLIPS_IN_ONE_JUMP"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MOST_SPINS_IN_ONE_JUMP"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_SLIPSTREAMS_IN_RACE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_TURBO_STARTS_IN_RACE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PASS_DB_PLAYER_KILLS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PISTOL_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PLAYER_HEADSHOTS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_RACES_WON"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SAWNOFF_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCRIPT_INCREASE_DRIV"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCRIPT_INCREASE_FLY"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCRIPT_INCREASE_LUNG"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCRIPT_INCREASE_MECH"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCRIPT_INCREASE_SHO"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCRIPT_INCREASE_STAM"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCRIPT_INCREASE_STL"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCRIPT_INCREASE_STRN"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_CREATOR_STAMINA"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_CREATOR_SHOOTING"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_CREATOR_STRENGTH"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_CREATOR_STEALTH"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_CREATOR_FLYING"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_CREATOR_BIKE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_CREATOR_LUNG"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_CREATOR_MECHANIC"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_STKYBMB_ENEMY_KILLS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_UNARMED_ENEMY_KILLS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_USJS_FOUND"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_USJS_FOUND_MASK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_USJS_TOTAL_COMPLETED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_USJS_COMPLETED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_USJS_COMPLETED_MASK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WEAP_FM_ADDON_PURCH"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WEAP_FM_ADDON_PURCH2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WEAP_FM_ADDON_PURCH3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WEAP_FM_ADDON_PURCH4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WEAP_FM_ADDON_PURCH5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRDEADLINE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_ABILITY_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_ABILITY_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_ABILITY_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_ABILITY_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_ABILITY_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_ABILITY_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BUY_COMPLETE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BUY_UNDERTAKEN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_SELL_COMPLETE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_SELL_UNDERTAKEN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_CONTRA_EARNINGS"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BIKER_BUY_COMPLET"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BIKER_BUY_UNDERTA"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BIKER_SELL_COMPLET"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BIKER_SELL_UNDERTA"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_COMPLET1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_UNDERTA1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_COMPLET1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_UNDERTA1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_COMPLET2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_UNDERTA2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_COMPLET2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_UNDERTA2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_COMPLET3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_UNDERTA3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_COMPLET3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_UNDERTA3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_COMPLET4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_UNDERTA4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_COMPLET4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_UNDERTA4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_COMPLET5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_BUY_UNDERTA5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_COMPLET5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_BIKER_SELL_UNDERTA5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SELL_COMPLETBC"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SELL_UNDERTABC"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_COMPLETBC1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_UNDERTABC1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_COMPLETBC2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_UNDERTABC2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_COMPLETBC3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_UNDERTABC3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_COMPLETBC4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_UNDERTABC4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_COMPLETBC5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SEL_UNDERTABC5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SELL_EARNINGS0"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SELL_EARNINGS1"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SELL_EARNINGS2"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SELL_EARNINGS3"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SELL_EARNINGS4"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_BKR_SELL_EARNINGS5"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_IE_EXPORT_COMPLETED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_IE_MISSION_EARNINGS"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_HANGAR_BUY_UNDETAK"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_HANGAR_BUY_COMPLET"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_HANGAR_SEL_UNDETAK"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_HANGAR_SEL_COMPLET"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LFETIME_HANGAR_EARNINGS"), 20000000, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_0"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_1"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_3"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_4"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_5"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_6"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_7"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_8"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PILOT_ASPASSEDLESSON_9"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PILOT_SCHOOL_MEDAL_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_PILOT_SCHOOL_MEDAL_9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SR_HIGHSCORE_1"), 950, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SR_HIGHSCORE_2"), 2450, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SR_HIGHSCORE_3"), 3450, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SR_HIGHSCORE_4"), 3350, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SR_HIGHSCORE_5"), 4150, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SR_HIGHSCORE_6"), 600, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SR_TARGETS_HIT"), 300, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SR_WEAPON_BIT_SET"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_SR_TIER_1_REWARD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_SR_TIER_3_REWARD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_SR_INCREASE_THROW_CAP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_COMPLET_HEIST_MEM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_COMPLET_HEIST_1STPER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_FLEECA_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_HST_ORDER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_HST_SAME_TEAM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_HST_ULT_CHAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_HUMANE_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_PACIFIC_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_PRISON_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_SERIESA_FIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_PLANNING_DONE_PRINT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_PLANNING_DONE_HELP_0"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_PLANNING_DONE_HELP_1"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_PRE_PLAN_DONE_HELP_0"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_CUTS_DONE_FINALE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_CUTS_DONE_ORNATE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_CUTS_DONE_BIOLAB"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_CUTS_DONE_PRISON"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_CUTS_DONE_NARCOTIC"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_CUTS_DONE_TUTORIAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_AWARD_DONE_PREP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HEIST_AWARD_BOUGHT_IN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_FLEECA_PREP_1"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_FLEECA_PREP_2"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_FLEECA_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PRISON_PLANE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PRISON_BUS"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PRISON_STATION"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PRISON_UNFINISHED_BIZ"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PRISON_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_HUMANE_KEY_CODES"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_HUMANE_ARMORDILLOS"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_HUMANE_EMP"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_HUMANE_VALKYRIE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_HUMANE_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_NARC_COKE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_NARC_TRASH_TRUCK"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_NARC_BIKERS"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_NARC_WEED"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_NARC_STEAL_METH"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_NARC_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PACIFIC_TRUCKS"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PACIFIC_WITSEC"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PACIFIC_HACK"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PACIFIC_BIKES"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PACIFIC_CONVOY"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PACIFIC_FINALE"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_ALLINORDER"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_LOYALTY"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_CRIMMASMD"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_LOYALTY2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_LOYALTY3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_CRIMMASMD2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_CRIMMASMD3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GANGOPS_SUPPORT"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_GANGOPS_IAA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_GANGOPS_SUBMARINE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MPPLY_AWD_GANGOPS_MISSILE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GANGOPS_HEIST_STATUS"), -229384, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GANGOPS_FM_MISSION_PROG"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GANGOPS_FLOW_NOTIFICATIONS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GANGOPS_FLOW_MISSION_PROG"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WAM_FLOW_VEHICLE_BS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GANGOPS_FLOW_IMPEXP_NUM"), 32, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NO_BOUGHT_YUM_SNACKS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DANCE_TO_SOLOMUN"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DANCE_TO_TALEOFUS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DANCE_TO_DIXON"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DANCE_TO_BLKMAD"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_CLUB_DRUNK"), 200, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_CLUB_COORD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_CLUB_HOTSPOT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_CLUB_CLUBBER"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NIGHTCLUB_PLAYER_APPEAR"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMUNIQUEPLYSINCLUB"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DANCETODIFFDJS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DANCE_COMBO_DURATION_MINS"), 240, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SOLCOUNTER"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOSCOUNTER"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DIXCOUNTER"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BMCOUNTER"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DANCEPERFECTOWNCLUB"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NIGHTCLUB_HOTSPOT_TIME_MS"), 3600000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BUSINESSHUB_CONT_TOTAL"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BUSINESSHUB_CONT_MISSION"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NIGHTCLUB_CONT_TOTAL"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NIGHTCLUB_CONT_MISSION"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_HUB_GOODS_SOLD"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LIFETIME_HUB_GOODS_MADE"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FMBB_FLOW_NOTIFICATIONS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FMBB_PCM_PROGRESS"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_BEGINNER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FIELD_FILLER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ARMCHAIR_RACER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_LEARNER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_SUNDAY_DRIVER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_THE_ROOKIE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_BUMP_AND_RUN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_GEAR_HEAD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_DOOR_SLAMMER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_HOT_LAP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ARENA_AMATEUR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_PAINT_TRADER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_SHUNTER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_JOCK"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_WARRIOR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_T_BONE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_MAYHEM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_WRECKER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_CRASH_COURSE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ARENA_LEGEND"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_PEGASUS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_CONTACT_SPORT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_UNSTOPPABLE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_WATCH_YOUR_STEP"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_TOWER_OFFENSE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_READY_FOR_WAR"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_THROUGH_A_LENS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_SPINNER"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_YOUMEANBOOBYTRAPS"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_MASTER_BANDITO"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_SITTING_DUCK"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_CROWDPARTICIPATION"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_KILL_OR_BE_KILLED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_MASSIVE_SHUNT"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_YOURE_OUTTA_HERE"), 200, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_WEVE_GOT_ONE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_ARENA_WAGEWORKER"), 20000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_TIME_SERVED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_TOP_SCORE"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_CAREER_WINNER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_SP"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_SP_LIFETIME"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_SKILL_LEVEL"), 20, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_AP"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_AP_TIER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_AP_LIFETIME"), 5055000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_CARRER_UNLK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_THEME_SCIFI"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_THEME_APOC"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_THEME_CONS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_PASS_THE_BOMB"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_DETONATION"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_ARCADE_RACE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_CTF"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_TAG_TEAM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_DESTR_DERBY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_CARNAGE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_MONSTER_JAM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_W_GAMES_MASTERS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_L_PASS_THE_BOMB"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_L_DETONATION"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_L_ARCADE_RACE"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_L_CTF"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_L_TAG_TEAM"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_L_DESTR_DERBY"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_L_CARNAGE"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_L_MONSTER_JAM"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_L_GAMES_MASTERS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_OF_CHAMP_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_SPECTATOR_KILLS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_LIFETIME_KILLS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_LIFETIME_DEATHS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_CARRER_WINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_CARRER_WINT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_MATCHES_PLYD"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_MATCHES_PLYDT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_SPEC_BOX_TIME_MS"), 86400000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_SPECTATOR_DRONE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_SPECTATOR_CAMS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_SMOKE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_DRINK"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_MONSTER3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_MONSTER4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_MONSTER5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_CERBERUS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_CERBERUS2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_CERBERUS3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_BRUISER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_BRUISER2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_BRUISER3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_SLAMVAN4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_SLAMVAN5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_SLAMVAN6"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_BRUTUS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_BRUTUS2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_BRUTUS3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_SCARAB"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_SCARAB2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_SCARAB3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_DOMINATOR4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_DOMINATOR5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_DOMINATOR6"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_IMPALER2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_IMPALER3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_IMPALER4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_ISSI4"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_ISSI5"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_ISSI6"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_IMPERATOR"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_IMPERATOR2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_IMPERATOR3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_ZR380"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_ZR3802"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_ZR3803"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_DEATHBIKE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_DEATHBIKE2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_VEH_DEATHBIKE3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_BS_TRINKET_TICKERS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_BS_TRINKET_SAVED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NO_BOUGHT_HEALTH_SNACKS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NO_BOUGHT_EPIC_SNACKS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_OF_ORANGE_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MP_CHAR_ARMOUR_1_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MP_CHAR_ARMOUR_2_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MP_CHAR_ARMOUR_3_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MP_CHAR_ARMOUR_4_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MP_CHAR_ARMOUR_5_COUNT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_OF_BOURGE_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CIGARETTES_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_1_WHITE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_1_RED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_1_BLUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_2_WHITE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_2_RED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_2_BLUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_3_WHITE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_3_RED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_3_BLUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_4_WHITE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_4_RED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWORK_TYPE_4_BLUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_AWD_FM_CR_DM_MADE"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_AWD_FM_CR_PLAYED_BY_PEEP"), 10000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_AWD_FM_CR_RACES_MADE"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_AWD_FM_CR_MISSION_SCORE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_NUM_CAPTURES_CREATED"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_LTS_CREATED"), 25, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_FM_MISSION_LIKES"), 5000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_ACT_PHN"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_ACT_PH2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_ACT_PH3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_ACT_PH4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_ACT_PH5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_VEH_TX1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_ACT_PH6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_ACT_PH7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_ACT_PH8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_ACT_PH9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_CUT_DONE"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FM_CUT_DONE_2"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_FM_INTRO_CUT_DONE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_FM_INTRO_MISS_DONE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_SHOOTINGRANGE_SEEN_TUT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_TENNIS_SEEN_TUTORIAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_DARTS_SEEN_TUTORIAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_ARMWRESTLING_SEEN_TUTORIAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HAS_WATCHED_BENNY_CUTSCE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_LOW_FLOW_CS_DRV_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_LOW_FLOW_CS_TRA_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_LOW_FLOW_CS_FUN_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_LOW_FLOW_CS_PHO_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_LOW_FLOW_CS_FIN_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_LOW_BEN_INTRO_CS_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LOWRIDER_FLOW_COMPLETE"), 9, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LOW_FLOW_CURRENT_PROG"), 9, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LOW_FLOW_CURRENT_CALL"), 9, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_MORGUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_DELUXO"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_SERVERFARM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_IAABASE_FIN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_STEALOSPREY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_FOUNDRY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_RIOTVAN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_SUBMARINECAR"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_SUBMARINE_FIN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_PREDATOR"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_BMLAUNCHER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_BCCUSTOM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_STEALTHTANKS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_SPYPLANE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_FINALE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_FINALE_P2"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGOP_FINALE_P3"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SNIPERRFL_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HVYSNIPER_ENEMY_KILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_SPENT_DEATHMAT"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_SPENT_RACES"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_MISSION_CREATO"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP_FIRST_PERSON_CAM_TIME"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_RACES_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_RACES_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_CUSTOM_RACES_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_DEATHMATCH_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_DEATHMATCH_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TDEATHMATCH_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TDEATHMATCH_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_SHOOTINGRANGE_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_SHOOTINGRANGE_LOSSES"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TENNIS_MATCHES_WON"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TENNIS_MATCHES_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GOLF_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GOLF_LOSSES"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DARTS_TOTAL_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DARTS_TOTAL_MATCHES"), 750, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_SHOOTINGRANGE_TOTAL_MATCH"), 800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_BJ_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_BJ_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_RACE_2_POINT_WINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_RACE_2_POINT_LOST"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_JOBS"), MISC::GET_RANDOM_INT_IN_RANGE(500000, 1250000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_SELLING_VEH"), MISC::GET_RANDOM_INT_IN_RANGE(500000, 2000000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_BETTING"), MISC::GET_RANDOM_INT_IN_RANGE(5000, 50000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_GOOD_SPORT"), MISC::GET_RANDOM_INT_IN_RANGE(5000, 50000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_PICKED_UP"), MISC::GET_RANDOM_INT_IN_RANGE(1000, 30000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_SHARED"), MISC::GET_RANDOM_INT_IN_RANGE(800, 10000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_JOBSHARED"), MISC::GET_RANDOM_INT_IN_RANGE(1000, 50000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_ROCKSTAR_AWARD"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_WEAPON_ARMOR"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_VEH_MAINTENANCE"), MISC::GET_RANDOM_INT_IN_RANGE(10000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_STYLE_ENT"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_PROPERTY_UTIL"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_JOB_ACTIVITY"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_CONTACT_SERVICE"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_HEALTHCARE"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_DROPPED_STOLEN"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_SHARED"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_SPENT_JOBSHARED"), MISC::GET_RANDOM_INT_IN_RANGE(50000, 300000), TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTAL_PLAYING_TIME"), 2147483647, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME"), 2147483647, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME_NEW"), 2147483647, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MP_FIRST_PERSON_CAM_TIME"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_KILLS_PLAYERS"), 2500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DEATHS_PLAYER"), 1660, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MPPLY_KILL_DEATH_RATIO"), 1.50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_KILLS_COP"), 7800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_KILLS_SWAT"), 8000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_STOLEN_COP_VEHICLE"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_STOLEN_CARS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_STOLEN_BIKES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_STOLEN_BOATS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_STOLEN_HELIS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_STOLEN_PLANES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_STOLEN_QUADBIKES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NUMBER_STOLEN_BICYCLES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NO_TIMES_WANTED_LEVEL"), 5000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_STARS_ATTAINED"), 5000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_STARS_EVADED"), 4000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTAL_CHASE_TIME"), 432000000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP1_LAST_CHASE_TIME"), 432000000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP1_LONGEST_CHASE_TIME"), 432000000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP1_TOTAL_TIME_MAX_STARS"), 432000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_VEHEXPORTED"), 10000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTAL_NO_SHOPS_HELD_UP"), 1000, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FIRST_TIME1"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FIRST_TIME2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FIRST_TIME3"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FIRST_TIME4"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FIRST_TIME5"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FIRST_TIME6"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ALL_IN_ORDER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_SUPPORTING_ROLE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_LEADER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_SURVIVALIST"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_ODD_JOBS"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_VCM_FLOW_CS_RSC_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_VCM_FLOW_CS_BWL_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_VCM_FLOW_CS_MTG_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_VCM_FLOW_CS_OIL_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_VCM_FLOW_CS_DEF_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_VCM_FLOW_CS_FIN_SEEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CAS_VEHICLE_REWARD"), FALSE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_VCM_FLOW_PROGRESS"), 1839072, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_VCM_STORY_PROGRESS"), 0, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_SCOPEOUT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_CREWEDUP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_MOVINGON"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_PROMOCAMP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_GUNMAN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_SMASHNGRAB"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_INPLAINSI"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_UNDETECTED"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ALLROUND"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ELITETHEIF"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_PRO"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_SUPPORTACT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_SHAFTED"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_COLLECTOR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_DEADEYE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_PISTOLSATDAWN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_TRAFFICAVOI"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_CANTCATCHBRA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_WIZHARD"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_APEESCAPE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_MONKEYKIND"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_AQUAAPE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_KEEPFAITH"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_TRUELOVE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_NEMESIS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_FRIENDZONED"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_PREPARATION"), 40, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_ASLEEPONJOB"), 20, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_DAICASHCRAB"), 100000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_BIGBRO"), 40, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_SHARPSHOOTER"), 40, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_RACECHAMP"), 40, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_BATSWORD"), 1000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_COINPURSE"), 950000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_ASTROCHIMP"), 3000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_MASTERFUL"), 40000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCGW_NUM_WINS_GANG_0"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCGW_NUM_WINS_GANG_1"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCGW_NUM_WINS_GANG_2"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SCGW_NUM_WINS_GANG_3"), 50, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_SCGW_WON_NO_DEATHS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_IAP_MAX_MOON_DIST"), 2147483647, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_IAP_CHALLENGE_0"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_IAP_CHALLENGE_1"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_IAP_CHALLENGE_2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_IAP_CHALLENGE_3"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_IAP_CHALLENGE_4"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_IAP_GOLD_TANK"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HAS_TAKEN_PEYOTE_BEFORE"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SIGNAL_JAMMERS_COLLECTED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GANGOPS_FLOW_BITSET_MISS0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GANGOPS_FM_BITSET_MISS0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GANGOPS_FLOW_PASSED_BITSET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AT_FLOW_VEHICLE_BS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AT_FLOW_MISSION_PROGRESS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AT_FLOW_IMPEXP_NUM"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AT_FLOW_BITSET_MISSIONS0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WVM_FLOW_VEHICLE_BS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WVM_FLOW_BITSET_MISSIONS0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WVM_FLOW_MISSION_PROGRESS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_WVM_FLOW_IMPEXP_NUM"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CH_ARC_CAB_CLAW_TROPHY"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CH_ARC_CAB_LOVE_TROPHY"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NIGHTCLUB_JOBS_DONE"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HUB_SALES_COMPLETED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NIGHTCLUB_VIP_APPEAR"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGATTACK_CITY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGATTACK_COUNTRY"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGATTACK_LOST"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GANGATTACK_VAGOS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GHKILLS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HORDELVL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HORDKILLS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_UNIQUECRATES"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BJWINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HORDEWINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MCMWINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_GANGHIDWINS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRARMWREST"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRBASEJUMP"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRDARTS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRDM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRGANGHIDE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRGOLF"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRHORDE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRMISSION"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRSHOOTRNG"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRPILOTSCHOOL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRDISTRICTCOPS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRTENNIS"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CRHEIST"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_RACES"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CP_STOLEN_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CP_WELCOMETUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CP_WEA_AMBUSH"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_HOOKER_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_DRUGS_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_RIVAL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_SELLCARTUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_HOLDUP_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_TRIGGER_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_WELCOME"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_GUNSHOPTUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_ARMORYTUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_HOSTAGE_STEAL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_STEAL_BIKES"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_AIRPORTTAKE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PROP_TUT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_SHIPMENTSTEAL"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_SHOWROOM"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_STEALVEHICLE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_VANRESCUE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_BANKHEIST1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_COUNTHEIST1"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HOLDUPS_BITSET"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AMBIENT_MIS_BS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MEMBERSMARKEDFORDEATH"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MCKILLS"), 2500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MCDEATHS"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_RIVALPRESIDENTKILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_RIVALCEOANDVIPKILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MELEEKILLS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLUBHOUSECONTRACTSCOMPLETE"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLUBHOUSECONTRACTEARNINGS"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLUBWORKCOMPLETED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLUBCHALLENGESCOMPLETED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MEMBERCHALLENGESCOMPLETED"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CAS_HEIST_NOTS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CAS_HEIST_FLOW"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_ACCESSPOINTS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_POI"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_KEYLEVELS"), 2, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3OPT_TARGET"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_H3_COOLDOWN"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_COMPLETEDPOSIX"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3MAN_MISSIONBS"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_REV_DA_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_REV_DA_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_SHATCHET_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_SHATCHET_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_MOLOTOV_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_MOLOTOV_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MOLOTOV_FM_AMMO_BOUGHT"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MOLOTOV_FM_AMMO_CURRENT"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_STEALTH_1A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_STEALTH_2B_RAPP"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_STEALTH_2C_SIDE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_STEALTH_3A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_STEALTH_4A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_STEALTH_5A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_SUBTERFUGE_1A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_SUBTERFUGE_2A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_SUBTERFUGE_2B"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_SUBTERFUGE_3A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_SUBTERFUGE_3B"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_SUBTERFUGE_4A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_SUBTERFUGE_5A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_DIRECT_1A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_DIRECT_2A1"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_DIRECT_2A2"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_DIRECT_2BP"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_DIRECT_2C"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_DIRECT_3A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_DIRECT_4A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_CR_DIRECT_5A"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_BOARD_DIALOGUE0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_BOARD_DIALOGUE1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_BOARD_DIALOGUE2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H3_VEHICLESUSED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_ORDER"), 100, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_FURIA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_MINITANK"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_YOSEMITE2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_ZHABA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_IMORGEN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_SULTAN2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_VAGRANT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_VSTR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_STRYDER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_SUGOI"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_KANJO"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_FORMULA"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_FORMULA2"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELP_JB7002"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES0"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES3"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES4"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES5"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES6"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES7"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES8"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES9"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES10"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES11"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES12"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES13"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES14"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES15"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_XMASLIVERIES16"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWRK_FM_AMMO_BOUGHT"), 9999, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_FIREWRK_FM_AMMO_CURRENT"), 9999, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MUSKET_FM_AMMO_BOUGHT"), 9999, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MUSKET_FM_AMMO_CURRENT"), 9999, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_FIREWRK_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_FIREWRK_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_MUSKET_IN_POSSESSION"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_MUSKET_AQUIRED_AS_GIFT"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CE_SP_FLOW_NOTIFICATIONS"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_PLANE_ACCESS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_BOAT_ACCESS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_HELI_ACCESS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_DOES_PLAYER_HAVE_VEH_ACCESS"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_KINGOFHILLWINS"), 500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_KINGOFHILLLOSS"), 250, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TIME_AS_A_PASSENGER"), 14400000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TIMEINHELI"), 14400000, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_RACE_MODDED_CAR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_DRIVE_RALLY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PLAY_GTA_RACE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PLAY_FOOT_RACE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PLAY_TEAM_DM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PLAY_VEHICLE_DM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PLAY_MISSION_CONTACT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PLAY_A_PLAYLIST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PLAY_POINT_TO_POINT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PLAY_ONE_ON_ONE_DM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PLAY_ONE_ON_ONE_RACE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_SURV_A_BOUNTY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_SET_WANTED_LVL_ON_PLAY"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_GANG_BACKUP_GANGS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_GANG_BACKUP_LOST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_GANG_BACKUP_VAGOS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_CALL_MERCENARIES"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_PHONE_MECH_DROP_CAR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_GONE_OFF_RADAR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_FILL_TITAN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_MOD_CAR_USING_APP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_BUY_APPARTMENT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_BUY_INSURANCE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_BUY_GARAGE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_ENTER_FRIENDS_HOUSE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_CALL_STRIPPER_HOUSE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_CALL_FRIEND"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_SEND_FRIEND_REQUEST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_CL_W_WANTED_PLAYER_TV"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CRMISSION"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_MCMWIN"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CRHORDE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_HORDEWINS"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_BEAMHACK"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_COUNT_HOTRING_RACE"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_PACKAGE_1_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_PACKAGE_2_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_PACKAGE_3_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_PACKAGE_4_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_PACKAGE_5_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_PACKAGE_6_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_PACKAGE_7_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_PACKAGE_8_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_PACKAGE_9_COLLECT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_NO_FM_PACKAGES_COL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_HEALTH_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_FM_HEALTH_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_XMAS_NORM_CLOTHES_SAVED"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_XMAS_NORM_CLOTHES_TOP"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_XMAS_NORM_CLOTHES_LOWERS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_XMAS_NORM_CLOTHES_FEET"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_VALENTINES_REC_CLOTHES"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_XMAS_NORM_CLOTHES_SPECIALS"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_UNLOCK_EXCLUS_CONTENT"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_KINGOFQUB3D"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_QUBISM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_QUIBITS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_GODOFQUB3D"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_GOFOR11TH"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ELEVENELEVEN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MOVIE_PROPS_COLLECTED"), 10, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_YACHT_MISSION_PROG"), 0, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_YACHT_MISSION_FLOW"), 21845, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CASINO_DECORATION_GIFT_1"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WEAP_UNLOCKED"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WEAP_UNLOCKED2"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WEAP_ADDON_1_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WEAP_ADDON_2_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WEAP_ADDON_3_UNLCK"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_WEAP_ADDON_4_UNLCK"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_INTELGATHER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_COMPOUNDINFILT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_LOOT_FINDER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_MAX_DISRUPT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_THE_ISLAND_HEIST"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_GOING_ALONE"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_TEAM_WORK"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_MIXING_UP"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_PRO_THIEF"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_CAT_BURGLAR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ONE_OF_THEM"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_GOLDEN_GUN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ELITE_THIEF"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_PROFESSIONAL"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_HELPING_OUT"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_COURIER"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_PARTY_VIBES"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_HELPING_HAND"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_AWD_ELEVENELEVEN"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_LOSTANDFOUND"), 500000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_SUNSET"), 1800000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_TREASURE_HUNTER"), 1000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_WRECK_DIVING"), 1000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_KEINEMUSIK"), 1800000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_PALMS_TRAX"), 1800000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_MOODYMANN"), 1800000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_FILL_YOUR_BAGS"), 200000000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_AWD_WELL_PREPARED"), 50, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4_H4_DJ_MISSIONS"), -1, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_COMPLETE_H4_F_USING_VETIR"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_COMPLETE_H4_F_USING_LONGFIN"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_COMPLETE_H4_F_USING_ANNIH"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_COMPLETE_H4_F_USING_ALKONOS"), TRUE, TRUE);
								STATS::STAT_SET_BOOL(RAGE_JOAAT("MP1_COMPLETE_H4_F_USING_PATROLB"), TRUE, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_BS_GEN"), 131071, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_BS_ENTR"), 63, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_BS_ABIL"), 63, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_WEAPONS"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_WEP_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_ARM_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_HEL_DISRP"), 3, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_TARGET"), 5, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_GRAPPEL"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_UNIFORM"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_BOLTCUT"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4CNF_APPROACH"), -1, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_CASH_V"), 89000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_WEED_V"), 148200, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_COKE_V"), 223800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_GOLD_V"), 328800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_PAINT_V"), 175200, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4_PROGRESS"), 130667, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_CASH_I_SCOPED"), 6559764, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_CASH_C_SCOPED"), 28, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_WEED_I_SCOPED"), 42656, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_COKE_I_SCOPED"), 720971, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_GOLD_C_SCOPED"), 131, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4LOOT_PAINT_SCOPED"), 48, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4_MISSIONS"), 65535, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_H4_PLAYTHROUGH_STATUS"), 7000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_SUBMARINE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_STRATEGIC_BOMBER"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_SMUGGLER_PLANE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_STEALTH_HELI"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PATROL_BOAT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_SMUGGLER_BOAT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_TEQUILA"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PEARL_NECKLACE"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_PINK_DIAMOND"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_MADRAZO_FILES"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CR_SAPHIREPANSTAT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DB_KILLS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PASS_DB_KILLS"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TIRES_POPPED_BY_GUNSHOT"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HIGHEST_SKITTLES"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SHOTS"), 50000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP1_WEAPON_ACCURACY"), 100, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HITS_PEDS_VEHICLES"), 25000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_KILLS"), 50000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_HEADSHOTS"), 10000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_KILLS_ARMED"), 18000, TRUE);
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP1_KILLS_IN_FREE_AIM"), 8890, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_KILLS_STEALTH"), 1500, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_SUCCESSFUL_COUNTERS"), 800, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DM_HIGHEST_KILLSTREAK"), 30, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DEATHS"), 10000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DIED_IN_EXPLOSION"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DIED_IN_FALL"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DIED_IN_FIRE"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DIED_IN_ROAD"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_DIED_IN_DROWNING"), 2000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_NO_PHOTOS_TAKEN"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LAP_DANCED_BOUGHT"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PROSTITUTES_FREQUENTED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BOUNTPLACED"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_BOUNTSONU"), 1000, TRUE);
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MC_CONTRIBUTION_POINTS"), 1000, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Unlock All Done on Character 2!");
							}
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Set your Character Rank to 200 or more before using this option.");

				ImGui::Separator();

				if (ImGui::Button("Unlock Extra Stuff")) {
					g_fiber_pool->queue_job([]
						{
							for (int i = 0; i <= 30707; i++) {
								packed_bool(i, TRUE, -1, TRUE);
							}
							for (int i = 7387; i <= 7457; i++) {
								packed_bool(i, FALSE, -1, TRUE);
							}
							packed_bool(7466, FALSE, -1, TRUE);
							for (int i = 7621; i <= 7627; i++) {
								packed_bool(i, FALSE, -1, TRUE);
							}
							for (int i = 15441; i <= 15446; i++) {
								packed_bool(i, FALSE, -1, TRUE);
							}
							for (int i = 18100; i <= 18105; i++) {
								packed_bool(i, FALSE, -1, TRUE);
							}
							packed_bool(15995, FALSE, -1, TRUE);
							packed_bool(15548, FALSE, -1, TRUE);
							for (int i = 25241; i <= 25243; i++) {
								packed_bool(i, FALSE, -1, TRUE);
							}
							packed_bool(25518, FALSE, -1, TRUE);
							packed_bool(25519, FALSE, -1, TRUE);
							for (int i = 30321; i <= 30323; i++) {
								packed_bool(i, FALSE, -1, TRUE);
							}
							packed_int(7315, 6, -1, TRUE);
							packed_int(18981, 4, -1, TRUE);
							packed_int(18988, 24, -1, TRUE);
							packed_int(22032, 100, -1, TRUE);
							packed_int(22050, 100, -1, TRUE);
							packed_int(22051, 100, -1, TRUE);
							packed_int(22052, 100, -1, TRUE);
							packed_int(22053, 100, -1, TRUE);
							packed_int(22054, 100, -1, TRUE);
							packed_int(22055, 100, -1, TRUE);
							packed_int(22056, 100, -1, TRUE);
							packed_int(22057, 100, -1, TRUE);
							packed_int(22058, 100, -1, TRUE);
							packed_int(22063, 20, -1, TRUE);
							STATS::STAT_SAVE(0, 0, 3, 0);
							notification("Unlock Done!");
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This option will unlock a lot of extra stuff like clothing, permanent bunker research unlocks, etc.");

				ImGui::Separator();

				if (ImGui::Button("Unlock Returning Player Content")) {
					g_fiber_pool->queue_job([] {
						*script_global(UnlockExclusiveContent).as<int*>() = 2;
						*script_global(UnlockExclusiveContent + 1).as<int*>() = 2;
						notification("Content Enabled!");
					});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This option will enable the OG T-Shirts.");

				ImGui::Separator();

				if (ImGui::Button("Unlock All Achievements")) {
					g_fiber_pool->queue_job([] {
						for (int i = 1; i <= 77; i++) {
							PLAYER::GIVE_ACHIEVEMENT_TO_PLAYER(i);
							notification("All Achievements Unlocked!");
						}
					});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("It may take a while.");

				ImGui::Separator();

				if (ImGui::Button("Unlock Gender Change")) {
					g_fiber_pool->queue_job([]
						{
							if (character() == 0) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ALLOW_GENDER_CHANGE"), 52, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Done. Go to Change Appearance to change your gender.");
							}
							if (character() == 1) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ALLOW_GENDER_CHANGE"), 52, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Done. Go to Change Appearance to change your gender.");
							}
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Go to Change Appearance to change your gender.");

				ImGui::Separator();

				if (ImGui::Button("Unlock Penthouse Decorations")) {
					g_fiber_pool->queue_job([] {
						*script_global(Tunables).at(27179).as<int*>() = 100000;
						*script_global(Tunables).at(27180).as<int*>() = 5000;
						*script_global(Tunables).at(27181).as<int*>() = 1000;
						*script_global(Tunables).at(27182).as<int*>() = 200;
						*script_global(Tunables).at(27183).as<int*>() = 10000;
						*script_global(Tunables).at(27184).as<int*>() = 20500;
						*script_global(Tunables).at(27185).as<int*>() = 20500;
						*script_global(Tunables).at(27186).as<int*>() = 20500;
						*script_global(Tunables).at(27187).as<int*>() = 20500;
						*script_global(Tunables).at(27188).as<int*>() = 20500;
						*script_global(Tunables).at(27189).as<int*>() = 20500;
						*script_global(Tunables).at(27190).as<int*>() = 20500;
						*script_global(Tunables).at(27191).as<int*>() = 20500;
						*script_global(Tunables).at(27192).as<int*>() = 20500;
						*script_global(Tunables).at(27193).as<int*>() = 20500;
						*script_global(Tunables).at(27194).as<int*>() = 20500;
						*script_global(Tunables).at(27195).as<int*>() = 20500;
						*script_global(Tunables).at(27196).as<int*>() = 2500;
						*script_global(Tunables).at(27197).as<int*>() = 2500;
						*script_global(Tunables).at(27198).as<int*>() = 2500;
						*script_global(Tunables).at(27199).as<int*>() = 2500;
						*script_global(Tunables).at(27200).as<int*>() = 2500;
						*script_global(Tunables).at(27201).as<int*>() = 2500;
						*script_global(Tunables).at(27202).as<int*>() = 25500;
						*script_global(Tunables).at(27203).as<int*>() = 26500;
						*script_global(Tunables).at(27204).as<int*>() = 24500;
						*script_global(Tunables).at(27205).as<int*>() = 26000;
						*script_global(Tunables).at(27206).as<int*>() = 23500;
						*script_global(Tunables).at(27207).as<int*>() = 22500;
						*script_global(Tunables).at(27208).as<int*>() = 27800;
						*script_global(Tunables).at(27209).as<int*>() = 28500;
						*script_global(Tunables).at(27210).as<int*>() = 29500;
						*script_global(Tunables).at(27211).as<int*>() = 29500;
						*script_global(Tunables).at(27212).as<int*>() = 28500;
						*script_global(Tunables).at(27213).as<int*>() = 28500;
						*script_global(Tunables).at(27214).as<int*>() = 28500;
						*script_global(Tunables).at(27215).as<int*>() = 28500;
						*script_global(Tunables).at(27216).as<int*>() = 28500;
						*script_global(Tunables).at(27217).as<int*>() = 28500;
						*script_global(Tunables).at(27218).as<int*>() = 28500;
						*script_global(Tunables).at(27219).as<int*>() = 28500;
						*script_global(Tunables).at(27220).as<int*>() = 7800;
						*script_global(Tunables).at(27221).as<int*>() = 7800;
						*script_global(Tunables).at(27222).as<int*>() = 7800;
						*script_global(Tunables).at(27223).as<int*>() = 7800;
						*script_global(Tunables).at(27224).as<int*>() = 7800;
						*script_global(Tunables).at(27225).as<int*>() = 7800;
						*script_global(Tunables).at(27226).as<int*>() = 4100;
						*script_global(Tunables).at(27227).as<int*>() = 4100;
						*script_global(Tunables).at(27228).as<int*>() = 22500;
						*script_global(Tunables).at(27229).as<int*>() = 0;
						*script_global(Tunables).at(27230).as<int*>() = 34000;
						*script_global(Tunables).at(27231).as<int*>() = 34000;
						*script_global(Tunables).at(27232).as<int*>() = 31000;
						*script_global(Tunables).at(27233).as<int*>() = 32500;
						*script_global(Tunables).at(27234).as<int*>() = 35000;
						*script_global(Tunables).at(27235).as<int*>() = 35000;
						*script_global(Tunables).at(27236).as<int*>() = 31500;
						*script_global(Tunables).at(27237).as<int*>() = 31500;
						*script_global(Tunables).at(27238).as<int*>() = 31500;
						*script_global(Tunables).at(27239).as<int*>() = 31500;
						*script_global(Tunables).at(27240).as<int*>() = 30000;
						*script_global(Tunables).at(27241).as<int*>() = 30000;
						*script_global(Tunables).at(27242).as<int*>() = 30000;
						*script_global(Tunables).at(27243).as<int*>() = 30000;
						*script_global(Tunables).at(27244).as<int*>() = 25500;
						*script_global(Tunables).at(27245).as<int*>() = 25500;
						*script_global(Tunables).at(27246).as<int*>() = 25500;
						*script_global(Tunables).at(27247).as<int*>() = 25500;
						*script_global(Tunables).at(27248).as<int*>() = 82000;
						*script_global(Tunables).at(27249).as<int*>() = 115000;
						*script_global(Tunables).at(27250).as<int*>() = 3500;
						*script_global(Tunables).at(27251).as<int*>() = 3500;
						*script_global(Tunables).at(27252).as<int*>() = 3500;
						*script_global(Tunables).at(27253).as<int*>() = 3500;
						*script_global(Tunables).at(27254).as<int*>() = 8700;
						*script_global(Tunables).at(27255).as<int*>() = 9300;
						*script_global(Tunables).at(27256).as<int*>() = 29000;
						*script_global(Tunables).at(27257).as<int*>() = 29000;
						*script_global(Tunables).at(27258).as<int*>() = 29000;
						*script_global(Tunables).at(27259).as<int*>() = 29000;
						*script_global(Tunables).at(27260).as<int*>() = 29000;
						*script_global(Tunables).at(27261).as<int*>() = 15500;
						*script_global(Tunables).at(27262).as<int*>() = 15500;
						*script_global(Tunables).at(27263).as<int*>() = 15500;
						*script_global(Tunables).at(27264).as<int*>() = 15500;
						*script_global(Tunables).at(27265).as<int*>() = 15500;
						*script_global(Tunables).at(27266).as<int*>() = 25600;
						*script_global(Tunables).at(27267).as<int*>() = 5700;
						*script_global(Tunables).at(27268).as<int*>() = 0;
						*script_global(Tunables).at(27269).as<int*>() = 27000;
						*script_global(Tunables).at(27270).as<int*>() = 19700;
						*script_global(Tunables).at(27271).as<int*>() = 43500;
						*script_global(Tunables).at(27272).as<int*>() = 43500;
						*script_global(Tunables).at(27273).as<int*>() = 9400;
						*script_global(Tunables).at(27274).as<int*>() = 9500;
						*script_global(Tunables).at(27275).as<int*>() = 9000;
						*script_global(Tunables).at(27276).as<int*>() = 72500;
						*script_global(Tunables).at(27277).as<int*>() = 4200;
						*script_global(Tunables).at(27278).as<int*>() = 4200;
						*script_global(Tunables).at(27279).as<int*>() = 18400;
						*script_global(Tunables).at(27280).as<int*>() = 58000;
						*script_global(Tunables).at(27281).as<int*>() = 11200;
						*script_global(Tunables).at(27282).as<int*>() = 11200;
						*script_global(Tunables).at(27283).as<int*>() = 11200;
						*script_global(Tunables).at(27284).as<int*>() = 11200;
						*script_global(Tunables).at(27285).as<int*>() = 2400;
						*script_global(Tunables).at(27286).as<int*>() = 2400;
						*script_global(Tunables).at(27287).as<int*>() = 2400;
						*script_global(Tunables).at(27288).as<int*>() = 3750;
						*script_global(Tunables).at(27289).as<int*>() = 2750;
						*script_global(Tunables).at(27290).as<int*>() = 8650;
						*script_global(Tunables).at(27291).as<int*>() = 8650;
						*script_global(Tunables).at(27292).as<int*>() = 8650;
						*script_global(Tunables).at(27293).as<int*>() = 25600;
						*script_global(Tunables).at(27294).as<int*>() = 22500;
						*script_global(Tunables).at(27295).as<int*>() = 18200;
						*script_global(Tunables).at(27296).as<int*>() = 18200;
						*script_global(Tunables).at(27297).as<int*>() = 11500;
						*script_global(Tunables).at(27298).as<int*>() = 6850;
						*script_global(Tunables).at(27299).as<int*>() = 5750;
						*script_global(Tunables).at(27300).as<int*>() = 5950;
						*script_global(Tunables).at(27301).as<int*>() = 8100;
						*script_global(Tunables).at(27302).as<int*>() = 8100;
						*script_global(Tunables).at(27303).as<int*>() = 8100;
						*script_global(Tunables).at(27304).as<int*>() = 8100;
						*script_global(Tunables).at(27305).as<int*>() = 4750;
						*script_global(Tunables).at(27306).as<int*>() = 4850;
						*script_global(Tunables).at(27307).as<int*>() = 2750;
						*script_global(Tunables).at(27308).as<int*>() = 3150;
						*script_global(Tunables).at(27309).as<int*>() = 6800;
						*script_global(Tunables).at(27310).as<int*>() = 5500;
						*script_global(Tunables).at(27311).as<int*>() = 5500;
						*script_global(Tunables).at(27312).as<int*>() = 750;
						*script_global(Tunables).at(27313).as<int*>() = 750;
						*script_global(Tunables).at(27314).as<int*>() = 750;
						*script_global(Tunables).at(27315).as<int*>() = 25000;
						*script_global(Tunables).at(27316).as<int*>() = 1200;
						*script_global(Tunables).at(27317).as<int*>() = 2400;
						*script_global(Tunables).at(27318).as<int*>() = 27500;
						*script_global(Tunables).at(27319).as<int*>() = 7800;
						*script_global(Tunables).at(27320).as<int*>() = 7800;
						*script_global(Tunables).at(27321).as<int*>() = 7800;
						*script_global(Tunables).at(27322).as<int*>() = 7800;
						*script_global(Tunables).at(27323).as<int*>() = 7800;
						*script_global(Tunables).at(27324).as<int*>() = 7800;
						*script_global(Tunables).at(27325).as<int*>() = 7800;
						*script_global(Tunables).at(27326).as<int*>() = 7800;
						*script_global(Tunables).at(27327).as<int*>() = 12000;
						*script_global(Tunables).at(27328).as<int*>() = 29500;
						*script_global(Tunables).at(27329).as<int*>() = 41500;
						*script_global(Tunables).at(27330).as<int*>() = 41500;
						*script_global(Tunables).at(27331).as<int*>() = 41500;
						*script_global(Tunables).at(27332).as<int*>() = 41500;
						*script_global(Tunables).at(27333).as<int*>() = 24500;
						*script_global(Tunables).at(27334).as<int*>() = 12000;
						*script_global(Tunables).at(27335).as<int*>() = 12000;
						*script_global(Tunables).at(27336).as<int*>() = 12000;
						*script_global(Tunables).at(27337).as<int*>() = 40000;
						*script_global(Tunables).at(27338).as<int*>() = 18500;
						*script_global(Tunables).at(27339).as<int*>() = 18500;
						*script_global(Tunables).at(27340).as<int*>() = 18500;
						*script_global(Tunables).at(27341).as<int*>() = 18500;
						*script_global(Tunables).at(27342).as<int*>() = 17400;
						*script_global(Tunables).at(27343).as<int*>() = 68000;
						*script_global(Tunables).at(27344).as<int*>() = 28300;
						*script_global(Tunables).at(27345).as<int*>() = 52500;
						*script_global(Tunables).at(27346).as<int*>() = 52500;
						*script_global(Tunables).at(27347).as<int*>() = 28000;
						*script_global(Tunables).at(27348).as<int*>() = 17500;
						*script_global(Tunables).at(27349).as<int*>() = 17500;
						*script_global(Tunables).at(27350).as<int*>() = 17500;
						*script_global(Tunables).at(27351).as<int*>() = 11000;
						*script_global(Tunables).at(27352).as<int*>() = 24600;
						*script_global(Tunables).at(27353).as<int*>() = 24600;
						*script_global(Tunables).at(27354).as<int*>() = 28500;
						*script_global(Tunables).at(27355).as<int*>() = 28500;
						*script_global(Tunables).at(27356).as<int*>() = 24500;
						*script_global(Tunables).at(27357).as<int*>() = 24500;
						*script_global(Tunables).at(27358).as<int*>() = 24500;
						*script_global(Tunables).at(27359).as<int*>() = 24500;
						*script_global(Tunables).at(27360).as<int*>() = 86000;
						*script_global(Tunables).at(27361).as<int*>() = 9400;
						*script_global(Tunables).at(27362).as<int*>() = 8700;
						*script_global(Tunables).at(27363).as<int*>() = 6400;
						*script_global(Tunables).at(27364).as<int*>() = 42500;
						*script_global(Tunables).at(27365).as<int*>() = 45000;
						*script_global(Tunables).at(27366).as<int*>() = 26500;
						*script_global(Tunables).at(27367).as<int*>() = 26500;
						*script_global(Tunables).at(27368).as<int*>() = 26500;
						*script_global(Tunables).at(27369).as<int*>() = 18500;
						*script_global(Tunables).at(27370).as<int*>() = 19500;
						*script_global(Tunables).at(27371).as<int*>() = 18000;
						*script_global(Tunables).at(27372).as<int*>() = 28000;
						*script_global(Tunables).at(27373).as<int*>() = 25000;
						*script_global(Tunables).at(27374).as<int*>() = 14500;
						*script_global(Tunables).at(27375).as<int*>() = 14500;
						*script_global(Tunables).at(27376).as<int*>() = 14500;
						*script_global(Tunables).at(27377).as<int*>() = 14500;
						*script_global(Tunables).at(27378).as<int*>() = 7800;
						*script_global(Tunables).at(27379).as<int*>() = 24500;
						*script_global(Tunables).at(27380).as<int*>() = 25000;
						*script_global(Tunables).at(27381).as<int*>() = 24500;
						*script_global(Tunables).at(27382).as<int*>() = 24000;
						*script_global(Tunables).at(27383).as<int*>() = 23500;
						*script_global(Tunables).at(27384).as<int*>() = 15700;
						*script_global(Tunables).at(27385).as<int*>() = 16700;
						*script_global(Tunables).at(27386).as<int*>() = 24000;
						*script_global(Tunables).at(27387).as<int*>() = 23500;
						*script_global(Tunables).at(27388).as<int*>() = 25000;
						*script_global(Tunables).at(27389).as<int*>() = 30000;
						*script_global(Tunables).at(27390).as<int*>() = 27800;
						*script_global(Tunables).at(27391).as<int*>() = 5600;
						*script_global(Tunables).at(27392).as<int*>() = 5600;
						*script_global(Tunables).at(27393).as<int*>() = 5600;
						*script_global(Tunables).at(27394).as<int*>() = 5600;
						*script_global(Tunables).at(27395).as<int*>() = 3450;
						*script_global(Tunables).at(27396).as<int*>() = 7300;
						*script_global(Tunables).at(27397).as<int*>() = 24500;
						*script_global(Tunables).at(27398).as<int*>() = 24500;
						*script_global(Tunables).at(27399).as<int*>() = 24500;
						*script_global(Tunables).at(27400).as<int*>() = 24500;
						*script_global(Tunables).at(27401).as<int*>() = 24500;
						*script_global(Tunables).at(27402).as<int*>() = 24500;
						*script_global(Tunables).at(27403).as<int*>() = 24500;
						*script_global(Tunables).at(27404).as<int*>() = 24500;
						*script_global(Tunables).at(27405).as<int*>() = 25500;
						*script_global(Tunables).at(27406).as<int*>() = 25500;
						*script_global(Tunables).at(27407).as<int*>() = 25500;
						*script_global(Tunables).at(27408).as<int*>() = 25500;
						*script_global(Tunables).at(27409).as<int*>() = 25500;
						*script_global(Tunables).at(27410).as<int*>() = 20500;
						*script_global(Tunables).at(27411).as<int*>() = 5600;
						*script_global(Tunables).at(27412).as<int*>() = 5600;
						*script_global(Tunables).at(27413).as<int*>() = 5600;
						*script_global(Tunables).at(27414).as<int*>() = 5600;
						*script_global(Tunables).at(27415).as<int*>() = 18400;
						*script_global(Tunables).at(27416).as<int*>() = 18400;
						*script_global(Tunables).at(27417).as<int*>() = 18400;
						*script_global(Tunables).at(27418).as<int*>() = 17600;
						*script_global(Tunables).at(27419).as<int*>() = 26000;
						*script_global(Tunables).at(27420).as<int*>() = 31000;
						*script_global(Tunables).at(27421).as<int*>() = 4800;
						*script_global(Tunables).at(27422).as<int*>() = 4800;
						*script_global(Tunables).at(27423).as<int*>() = 35500;
						*script_global(Tunables).at(27424).as<int*>() = 35500;
						*script_global(Tunables).at(27425).as<int*>() = 41000;
						*script_global(Tunables).at(27426).as<int*>() = 35000;
						*script_global(Tunables).at(27427).as<int*>() = 30000;
						*script_global(Tunables).at(27428).as<int*>() = 24000;
						*script_global(Tunables).at(27429).as<int*>() = 25600;
						*script_global(Tunables).at(27430).as<int*>() = 71000;
						*script_global(Tunables).at(27431).as<int*>() = 95000;
						*script_global(Tunables).at(27432).as<int*>() = 95000;
						*script_global(Tunables).at(27433).as<int*>() = 95000;
						*script_global(Tunables).at(27434).as<int*>() = 95000;
						*script_global(Tunables).at(27435).as<int*>() = 24500;
						*script_global(Tunables).at(27436).as<int*>() = 23500;
						*script_global(Tunables).at(27437).as<int*>() = 24500;
						*script_global(Tunables).at(27438).as<int*>() = 23500;
						*script_global(Tunables).at(27439).as<int*>() = 24500;
						*script_global(Tunables).at(27440).as<int*>() = 23500;
						*script_global(Tunables).at(27441).as<int*>() = 18400;
						*script_global(Tunables).at(27442).as<int*>() = 18400;
						*script_global(Tunables).at(27443).as<int*>() = 18400;
						*script_global(Tunables).at(27444).as<int*>() = 18400;
						*script_global(Tunables).at(27445).as<int*>() = 600;
						*script_global(Tunables).at(27446).as<int*>() = 600;
						*script_global(Tunables).at(27447).as<int*>() = 600;
						*script_global(Tunables).at(27448).as<int*>() = 450;
						*script_global(Tunables).at(27449).as<int*>() = 450;
						*script_global(Tunables).at(27450).as<int*>() = 450;
						*script_global(Tunables).at(27451).as<int*>() = 1500;
						*script_global(Tunables).at(27452).as<int*>() = 1500;
						*script_global(Tunables).at(27453).as<int*>() = 340;
						*script_global(Tunables).at(27454).as<int*>() = 0;
						*script_global(Tunables).at(27455).as<int*>() = 0;
						*script_global(Tunables).at(27456).as<int*>() = 0;
						*script_global(Tunables).at(27457).as<int*>() = 0;
						*script_global(Tunables).at(27458).as<int*>() = 0;
						*script_global(Tunables).at(27459).as<int*>() = 0;
						*script_global(Tunables).at(27460).as<int*>() = 0;
						*script_global(Tunables).at(27461).as<int*>() = 0;
						*script_global(Tunables).at(27462).as<int*>() = 0;
						*script_global(Tunables).at(27463).as<int*>() = 16500;
						*script_global(Tunables).at(27464).as<int*>() = 16500;
						*script_global(Tunables).at(27465).as<int*>() = 70500;
						*script_global(Tunables).at(27466).as<int*>() = 12500;
						*script_global(Tunables).at(27467).as<int*>() = 75000;
						*script_global(Tunables).at(27468).as<int*>() = 14600;
						*script_global(Tunables).at(27469).as<int*>() = 5000;
						*script_global(Tunables).at(27470).as<int*>() = 15000;
						*script_global(Tunables).at(27471).as<int*>() = 15000;
						*script_global(Tunables).at(27472).as<int*>() = 15000;
						*script_global(Tunables).at(27473).as<int*>() = 95000;
						*script_global(Tunables).at(27474).as<int*>() = 95000;
						*script_global(Tunables).at(27475).as<int*>() = 0;
						*script_global(Tunables).at(27476).as<int*>() = 0;
						*script_global(Tunables).at(27477).as<int*>() = 0;
						*script_global(Tunables).at(27478).as<int*>() = 0;
						*script_global(Tunables).at(27479).as<int*>() = 0;
						*script_global(Tunables).at(27480).as<int*>() = 0;
						*script_global(Tunables).at(27481).as<int*>() = 24500;
						*script_global(Tunables).at(27482).as<int*>() = 24500;
						*script_global(Tunables).at(27483).as<int*>() = 22500;
						*script_global(Tunables).at(27484).as<int*>() = 22500;
						*script_global(Tunables).at(27485).as<int*>() = 22500;
						*script_global(Tunables).at(27486).as<int*>() = 22500;
						*script_global(Tunables).at(27487).as<int*>() = 22500;
						*script_global(Tunables).at(27488).as<int*>() = 22500;
						*script_global(Tunables).at(27489).as<int*>() = 13200;
						*script_global(Tunables).at(27490).as<int*>() = 15000;
						*script_global(Tunables).at(27491).as<int*>() = 3500;
						*script_global(Tunables).at(27492).as<int*>() = 3500;
						*script_global(Tunables).at(27493).as<int*>() = 2750;
						*script_global(Tunables).at(27494).as<int*>() = 13400;
						*script_global(Tunables).at(27495).as<int*>() = 13400;
						*script_global(Tunables).at(27496).as<int*>() = 27500;
						*script_global(Tunables).at(27497).as<int*>() = 76000;
						*script_global(Tunables).at(27498).as<int*>() = 76000;
						*script_global(Tunables).at(27499).as<int*>() = 76000;
						*script_global(Tunables).at(27500).as<int*>() = 17500;
						*script_global(Tunables).at(27501).as<int*>() = 17500;
						*script_global(Tunables).at(27502).as<int*>() = 17500;
						*script_global(Tunables).at(27503).as<int*>() = 17500;
						*script_global(Tunables).at(27504).as<int*>() = 17500;
						*script_global(Tunables).at(27505).as<int*>() = 17500;
						*script_global(Tunables).at(27506).as<int*>() = 17500;
						*script_global(Tunables).at(27507).as<int*>() = 17500;
						*script_global(Tunables).at(27508).as<int*>() = 17500;
						*script_global(Tunables).at(27509).as<int*>() = 17500;
						*script_global(Tunables).at(27510).as<int*>() = 17500;
						*script_global(Tunables).at(27511).as<int*>() = 17500;
						*script_global(Tunables).at(27512).as<int*>() = 37500;
						*script_global(Tunables).at(27513).as<int*>() = 3400;
						*script_global(Tunables).at(27514).as<int*>() = 3400;
						*script_global(Tunables).at(27515).as<int*>() = 3400;
						*script_global(Tunables).at(27516).as<int*>() = 45000;
						*script_global(Tunables).at(27517).as<int*>() = 45000;
						*script_global(Tunables).at(27518).as<int*>() = 45000;
						*script_global(Tunables).at(27519).as<int*>() = 45000;
						*script_global(Tunables).at(27520).as<int*>() = 45000;
						*script_global(Tunables).at(27521).as<int*>() = 45000;
						*script_global(Tunables).at(27522).as<int*>() = 45000;
						*script_global(Tunables).at(27523).as<int*>() = 45000;
						*script_global(Tunables).at(27524).as<int*>() = 24500;
						*script_global(Tunables).at(27525).as<int*>() = 24500;
						*script_global(Tunables).at(27526).as<int*>() = 24500;
						notification("Decorations Unlocked!");
					});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Go to casino shop after enabling it.");

				ImGui::Separator();

				if (ImGui::Button("Unlock Casino Shop Stuff")) {
					g_fiber_pool->queue_job([] {
						for (int i = 26567; i <= 27141; i++)
						{
							*script_global(Tunables).at(i).as<int*>() = TRUE;
						}
						notification("Shop Unlocked!");
					});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Go to casino shop after enabling it.");

				ImGui::Separator();

				if (ImGui::Button("Force Cloud Save")) {
					g_fiber_pool->queue_job([]
						{
							STATS::STAT_SAVE(0, 0, 3, 0);
							notification("Character Saved!");
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This option will trigger a cloud save.");

				ImGui::EndMenu();

			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Stats Editor")) {

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Arena Career Wins (Solo)", &g_gui.value_1, 0, 0); ImGui::SameLine(); if (ImGui::Button("[1] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_CARRER_WINS"), g_gui.value_1, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_CARRER_WINS"), g_gui.value_1, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Arena Career Wins (Team)", &g_gui.value_2, 0, 0); ImGui::SameLine(); if (ImGui::Button("[2] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_CARRER_WINT"), g_gui.value_2, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_CARRER_WINT"), g_gui.value_2, TRUE);
							notification("Done!");
						}
						});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Arena Total Kills", &g_gui.value_3, 0, 0); ImGui::SameLine(); if (ImGui::Button("[3] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_LIFETIME_KILLS"), g_gui.value_3, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_LIFETIME_KILLS"), g_gui.value_3, TRUE);
							notification("Done!");
						}
						});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Arena Total Deaths", &g_gui.value_4, 0, 0); ImGui::SameLine();	if (ImGui::Button("[4] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_LIFETIME_DEATHS"), g_gui.value_4, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_LIFETIME_DEATHS"), g_gui.value_4, TRUE);
							notification("Done!");
						}
						});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Arena Total Matches Played (Solo)", &g_gui.value_5, 0, 0); ImGui::SameLine(); if (ImGui::Button("[5] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_MATCHES_PLYD"), g_gui.value_5, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_MATCHES_PLYD"), g_gui.value_5, TRUE);
							notification("Done!");
						}
						});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Arena Total Matches Played (Team)", &g_gui.value_6, 0, 0); ImGui::SameLine(); if (ImGui::Button("[6] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARENAWARS_MATCHES_PLYDT"), g_gui.value_6, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARENAWARS_MATCHES_PLYDT"), g_gui.value_6, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Races Won", &g_gui.value_7, 0, 0); ImGui::SameLine(); if (ImGui::Button("[7] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_RACES_WON"), g_gui.value_7, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Races Lost", &g_gui.value_8, 0, 0); ImGui::SameLine(); if (ImGui::Button("[8] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_RACES_LOST"), g_gui.value_8, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Custom Races Won", &g_gui.value_9, 0, 0); ImGui::SameLine(); if (ImGui::Button("[9] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_CUSTOM_RACES_WON"), g_gui.value_9, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Deathmatch Lost", &g_gui.value_10, 0, 0); ImGui::SameLine(); if (ImGui::Button("[10] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_DEATHMATCH_LOST"), g_gui.value_10, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Deathmatch Won", &g_gui.value_11, 0, 0); ImGui::SameLine(); if (ImGui::Button("[11] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_DEATHMATCH_WON"), g_gui.value_11, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Team Deathmatch Lost", &g_gui.value_12, 0, 0); ImGui::SameLine(); if (ImGui::Button("[12] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TDEATHMATCH_LOST"), g_gui.value_12, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Team Deathmatch Won", &g_gui.value_13, 0, 0); ImGui::SameLine(); if (ImGui::Button("[13] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TDEATHMATCH_WON"), g_gui.value_13, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Shooting Range Wins", &g_gui.value_14, 0, 0); ImGui::SameLine(); if (ImGui::Button("[14] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_SHOOTINGRANGE_WINS"), g_gui.value_14, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Shooting Range Losses", &g_gui.value_15, 0, 0); ImGui::SameLine(); if (ImGui::Button("[15] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_SHOOTINGRANGE_LOSSES"), g_gui.value_15, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Tennis Matches Won", &g_gui.value_16, 0, 0); ImGui::SameLine(); if (ImGui::Button("[16] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TENNIS_MATCHES_WON"), g_gui.value_16, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Tennis Matches Lost", &g_gui.value_17, 0, 0); ImGui::SameLine(); if (ImGui::Button("[17] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TENNIS_MATCHES_LOST"), g_gui.value_17, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Golf Wins", &g_gui.value_18, 0, 0); ImGui::SameLine(); if (ImGui::Button("[18] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GOLF_WINS"), g_gui.value_18, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Golf Losses", &g_gui.value_19, 0, 0); ImGui::SameLine(); if (ImGui::Button("[19] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_GOLF_LOSSES"), g_gui.value_19, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Dart Wins", &g_gui.value_20, 0, 0); ImGui::SameLine(); if (ImGui::Button("[20] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DARTS_TOTAL_WINS"), g_gui.value_20, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Dart Matches", &g_gui.value_21, 0, 0); ImGui::SameLine(); if (ImGui::Button("[21] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DARTS_TOTAL_MATCHES"), g_gui.value_21, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Shooting Range Matches", &g_gui.value_22, 0, 0); ImGui::SameLine(); if (ImGui::Button("[22] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_SHOOTINGRANGE_TOTAL_MATCH"), g_gui.value_22, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Parachuting Wins", &g_gui.value_23, 0, 0); ImGui::SameLine(); if (ImGui::Button("[23] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_BJ_WINS"), g_gui.value_23, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Parachuting Losses", &g_gui.value_24, 0, 0); ImGui::SameLine(); if (ImGui::Button("[24] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_BJ_LOST"), g_gui.value_24, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Race-2-Point Wins", &g_gui.value_25, 0, 0); ImGui::SameLine(); if (ImGui::Button("[25] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_RACE_2_POINT_WINS"), g_gui.value_25, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Race-2-Point Losses", &g_gui.value_26, 0, 0); ImGui::SameLine(); if (ImGui::Button("[26] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_RACE_2_POINT_LOST"), g_gui.value_26, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Earned Money from Jobs", &g_gui.value_27, 0, 0); ImGui::SameLine(); if (ImGui::Button("[27] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_JOBS"), g_gui.value_27, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_JOBS"), g_gui.value_27, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Earned Money from Selling Vehicles", &g_gui.value_28, 0, 0); ImGui::SameLine(); if (ImGui::Button("[28] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_SELLING_VEH"), g_gui.value_28, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_SELLING_VEH"), g_gui.value_28, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Earned Money from Betting", &g_gui.value_29, 0, 0); ImGui::SameLine(); if (ImGui::Button("[29] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_BETTING"), g_gui.value_29, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_BETTING"), g_gui.value_29, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Earned Money from Being a Good Player", &g_gui.value_30, 0, 0); ImGui::SameLine(); if (ImGui::Button("[30] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_GOOD_SPORT"), g_gui.value_30, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_GOOD_SPORT"), g_gui.value_30, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Picked Up Money", &g_gui.value_31, 0, 0); ImGui::SameLine(); if (ImGui::Button("[31] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_PICKED_UP"), g_gui.value_31, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_PICKED_UP"), g_gui.value_31, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Shared Money", &g_gui.value_32, 0, 0); ImGui::SameLine(); if (ImGui::Button("[32] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_SHARED"), g_gui.value_32, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_SHARED"), g_gui.value_32, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Job Shared Money", &g_gui.value_33, 0, 0); ImGui::SameLine(); if (ImGui::Button("[33] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_JOBSHARED"), g_gui.value_33, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_JOBSHARED"), g_gui.value_33, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Earned from Rockstar Awards", &g_gui.value_34, 0, 0); ImGui::SameLine(); if (ImGui::Button("[34] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_ROCKSTAR_AWARD"), g_gui.value_34, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_ROCKSTAR_AWARD"), g_gui.value_34, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Arena Spectator Kills", &g_gui.value_35, 0, 0); ImGui::SameLine(); if (ImGui::Button("[35] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ARN_SPECTATOR_KILLS"), g_gui.value_35, TRUE);
							notification("Done!");
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ARN_SPECTATOR_KILLS"), g_gui.value_35, TRUE);
							notification("Done!");
						}
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Killed Players", &g_gui.value_36, 0, 0); ImGui::SameLine(); if (ImGui::Button("[36] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_KILLS_PLAYERS"), g_gui.value_36, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Deaths by Players", &g_gui.value_37, 0, 0); ImGui::SameLine(); if (ImGui::Button("[37] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DEATHS_PLAYER"), g_gui.value_37, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Money Earned", &g_gui.value_38, 0, 0); ImGui::SameLine(); if (ImGui::Button("[38] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_EVC"), g_gui.value_38, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Money Spent", &g_gui.value_39, 0, 0); ImGui::SameLine(); if (ImGui::Button("[39] Set Value")) {
					g_fiber_pool->queue_job([] {
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_SVC"), g_gui.value_39, TRUE);
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Playing Time (in milliseconds)", &g_gui.value_40, 0, 0); ImGui::SameLine(); if (ImGui::Button("[40] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTAL_PLAYING_TIME"), g_gui.value_40, TRUE);
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTAL_PLAYING_TIME"), g_gui.value_40, TRUE);
						}
						notification("Done!");
					});
				}

				ImGui::Separator();

				ImGui::SetNextItemWidth(106);
				ImGui::InputInt("Total Multiplayer Playing Time (in milliseconds)", &g_gui.value_41, 0, 0); ImGui::SameLine(); if (ImGui::Button("[41] Set Value")) {
					g_fiber_pool->queue_job([] {
						if (character() == 0) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME"), g_gui.value_41, TRUE);
							STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME_NEW"), g_gui.value_41, TRUE);
						}
						if (character() == 1) {
							STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME"), g_gui.value_41, TRUE);
							STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME_NEW"), g_gui.value_41, TRUE);
						}
						notification("Done!");
					});
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Misc Options")) {

				if (ImGui::Button("Set Char Creation Date")) {
					g_fiber_pool->queue_job([] {
						dates date{ 2013, 9, 17, 20, 30, 0, 0 };
						STATS::STAT_SET_DATE(RAGE_JOAAT("MP0_CHAR_DATE_CREATED"), &date, 7, TRUE);
						STATS::STAT_SET_DATE(RAGE_JOAAT("MP1_CHAR_DATE_CREATED"), &date, 7, TRUE);
						STATS::STAT_SET_DATE(RAGE_JOAAT("MP0_CHAR_DATE_RANKUP"), &date, 7, TRUE);
						STATS::STAT_SET_DATE(RAGE_JOAAT("MP1_CHAR_DATE_RANKUP"), &date, 7, TRUE);
						STATS::STAT_SET_DATE(RAGE_JOAAT("MPPLY_STARTED_MP"), &date, 7, TRUE);
						STATS::STAT_SET_DATE(RAGE_JOAAT("MPPLY_NON_CHEATER_CASH"), &date, 7, TRUE);
						STATS::STAT_SET_DATE(RAGE_JOAAT("MP0_CHAR_LAST_PLAY_TIME"), &date, 7, TRUE);
						STATS::STAT_SET_DATE(RAGE_JOAAT("MP1_CHAR_LAST_PLAY_TIME"), &date, 7, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_IN_LOBBY"), INT_MAX, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLOUD_TIME_CHAR_CREATED"), 1379449800, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLOUD_TIME_CHAR_CREATED"), 1379449800, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MP0_PS_TIME_CHAR_CREATED"), 1379449800, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MP1_PS_TIME_CHAR_CREATED"), 1379449800, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_TIME_SPENT_FREEMODE"), INT_MAX, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("LEADERBOARD_PLAYING_TIME"), INT_MAX, TRUE); // found in mpstatssetup.
						STATS::STAT_SET_INT(RAGE_JOAAT("MP0_TOTAL_PLAYING_TIME"), INT_MAX, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MP1_TOTAL_PLAYING_TIME"), INT_MAX, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME_NEW"), INT_MAX, TRUE); // found in mpstatssetup.
						STATS::STAT_SET_INT(RAGE_JOAAT("MP_PLAYING_TIME"), INT_MAX, TRUE); // found in mpstatssetupui.
						STATS::STAT_SET_INT(RAGE_JOAAT("MP0_LONGEST_PLAYING_TIME"), INT_MAX, TRUE);
						STATS::STAT_SET_INT(RAGE_JOAAT("MP1_LONGEST_PLAYING_TIME"), INT_MAX, TRUE);
						STATS::STAT_SAVE(0, 0, 3, 0);
						notification("Done!");
					});
				}

				ImGui::Separator();

				if (ImGui::Button("Add 8 years to the Total Playing Time")) {
					g_fiber_pool->queue_job([] {
						for (int i = 0; i <= 115; i++) {
							if (character() == 0) {
								STATS::STAT_INCREMENT(RAGE_JOAAT("MP0_TOTAL_PLAYING_TIME"), INT_MAX);
								STATS::STAT_INCREMENT(RAGE_JOAAT("MP_PLAYING_TIME_NEW"), INT_MAX);
								STATS::STAT_INCREMENT(RAGE_JOAAT("MP_PLAYING_TIME"), INT_MAX);
								STATS::STAT_INCREMENT(RAGE_JOAAT("LEADERBOARD_PLAYING_TIME"), INT_MAX);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Process Done!");
							}
							if (character() == 1) {
								STATS::STAT_INCREMENT(RAGE_JOAAT("MP1_TOTAL_PLAYING_TIME"), INT_MAX);
								STATS::STAT_INCREMENT(RAGE_JOAAT("MP_PLAYING_TIME_NEW"), INT_MAX);
								STATS::STAT_INCREMENT(RAGE_JOAAT("MP_PLAYING_TIME"), INT_MAX);
								STATS::STAT_INCREMENT(RAGE_JOAAT("LEADERBOARD_PLAYING_TIME"), INT_MAX);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Process Done!");
							}
						}
					});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Use this option to surpass the 24 days limit of the total playing time.");

				ImGui::Separator();

				if (ImGui::Button("Clear Player Mental State")) {
					g_fiber_pool->queue_job([]
						{
							if (character() == 0) {
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP0_PLAYER_MENTAL_STATE"), 0, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Mental State Cleared on Character 1!");
							}
							if (character() == 1) {
								STATS::STAT_SET_FLOAT(RAGE_JOAAT("MP1_PLAYER_MENTAL_STATE"), 0, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Mental State Cleared on Character 2!");
							}
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Instant effect, You don't need to change session.");

				ImGui::Separator();

				if (ImGui::Button("Clear Total Earned/Spent Stats")) {
					g_fiber_pool->queue_job([]
						{
							STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_EVC"), (uint64_t)0, TRUE);
							STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_SVC"), (uint64_t)0, TRUE);
							STATS::STAT_SAVE(0, 0, 3, 0);
							notification("Stats Cleared!");
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Buggy feature, use at your own risk.");

				ImGui::Separator();

				if (ImGui::Button("Fix Total Earned with Betting Stat")) {
					g_fiber_pool->queue_job([]
						{
							if (character() == 0) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_MONEY_EARN_BETTING"), MISC::GET_RANDOM_INT_IN_RANGE(5000, 50000), TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Stat Set!");
							}
							if (character() == 1) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_MONEY_EARN_BETTING"), MISC::GET_RANDOM_INT_IN_RANGE(5000, 50000), TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Stat Set!");
							}
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("This option will fix the money earned as bet (will fix the 'Unavailable' issue).");

				ImGui::Separator();

				if (ImGui::Button("Maximise Club Popularity")) {
					g_fiber_pool->queue_job([]
						{
							if (character() == 0) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLUB_POPULARITY"), 1000, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Club Popularity Maxed on Character 1!");
							}
							if (character() == 1) {
								STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLUB_POPULARITY"), 1000, TRUE);
								STATS::STAT_SAVE(0, 0, 3, 0);
								notification("Club Popularity Maxed on Character 2!");
							}
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("It will max the popularity without doing promote missions.");

				ImGui::Separator();

				if (ImGui::Button("Skip Actual Cutscene")) {
					g_fiber_pool->queue_job([]
						{
							CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Some cutscenes can't be skipped (MC/Office cutscenes).");

				ImGui::Separator();

				if (ImGui::Button("Unhide Weapons from Gunlocker")) {
					g_fiber_pool->queue_job([] {
						for (int i = 7378; i <= 7457; i++) {
							packed_bool(i, FALSE, -1, 1);
						}

						packed_bool(7466, FALSE, -1, 1);

						for (int i = 7621; i <= 7627; i++) {
							packed_bool(i, FALSE, -1, 1);
						}

						for (int i = 15441; i <= 15446; i++) {
							packed_bool(i, FALSE, -1, 1);
						}

						for (int i = 18100; i <= 18105; i++) {
							packed_bool(i, FALSE, -1, 1);
						}

						packed_bool(15995, FALSE, -1, 1);
						packed_bool(15548, FALSE, -1, 1);

						for (int i = 25241; i <= 25243; i++) {
							packed_bool(i, FALSE, -1, 1);
						}

						packed_bool(25518, FALSE, -1, 1);
						packed_bool(25519, FALSE, -1, 1);
						STATS::STAT_SAVE(0, 0, 3, 0);
						notification("Done! Check your Weapons.");
					});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Use this option to unhide your weapons.");

				ImGui::Separator();

				if (ImGui::Button("Bail from Session")) {
					g_fiber_pool->queue_job([] {
						NETWORK::NETWORK_BAIL(49, 0, 0);
						notification("Done!");
					});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Use this option to kick yourself from the session.");

				ImGui::Separator();

				if (ImGui::Button("Close Game"))
				{
					exit(0);
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Close your game.");

				ImGui::Separator();

				if (ImGui::Button("Unload Menu"))
				{
					g_fiber_pool->queue_job([]
						{
							g_running = false;
							notification("Mod Unloaded.");
						});
				}

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Unloads the menu, you can also inject again later.");

				ImGui::EndMenu();

			}
		}
		ImGui::End();
	}

	void gui::script_init()
	{
	}

	void gui::delayed_loops()
	{
	}

	void gui::script_on_tick()
	{
		if (g_gui.m_opened)
		{			
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
		}
		if (g_gui.anti_afk) {
			g_fiber_pool->queue_job([]
				{
					*script_global(Tunables).at(87).as<int*>() = INT_MAX;
					*script_global(Tunables).at(88).as<int*>() = INT_MAX;
					*script_global(Tunables).at(89).as<int*>() = INT_MAX;
					*script_global(Tunables).at(90).as<int*>() = INT_MAX;
					*script_global(Tunables).at(7785).as<int*>() = INT_MAX;
					*script_global(Tunables).at(7786).as<int*>() = INT_MAX;
					*script_global(Tunables).at(7787).as<int*>() = INT_MAX;
					*script_global(Tunables).at(7788).as<int*>() = INT_MAX;
					*script_global(IdleTimer).at(IdleTimer_Offset_1).as<int*>() = 0;
					*script_global(IdleTimer).at(IdleTimer_Offset_2).as<int*>() = 0;
					*script_global(IdleTimerUNK1).as<int*>() = 0;
					*script_global(IdleTimerUNK2).as<int*>() = 0;
				});
		}
		if (g_gui.rp_loop) {
			g_fiber_pool->queue_job([]
				{
					if (!HUD::IS_PAUSE_MENU_ACTIVE() && NETWORK::NETWORK_IS_SESSION_ACTIVE()) {
						Memory::set_value<int>({ 0x08, 0x10C8, 0x888 }, 1);
						script::get_current()->yield();
						Memory::set_value<int>({ 0x08, 0x10C8, 0x888 }, 0);
						script::get_current()->yield();
						*script_global(Tunables).at(1).as<float*>() = 500;
						STATS::STAT_SAVE(0, 0, 3, 0);
					}
				});
		}
		if (g_gui.always_jackpot) {
			g_fiber_pool->queue_job([] {

				// Basically, Always win the slots, not worth mentioning it.

				gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
					auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
					for (int i = 1; i <= 195; i++) {
						auto local1354 = script_local(slots_thread, 1354).at(1);
						int break_value = *local1354.at(i).as<int*>();
						if (break_value != 64) {
							*local1354.at(i).as<int*>() = 6;
						}
					}
					});
				});
		}
		if (g_gui.always_jackpot_2) {
			g_fiber_pool->queue_job([] {

				// Variant #2 - This variant is a good alternative if you don't want to change the entire "Probability Table" that you can find inside the scripts.
				// How it works? The first three (local_2097.f_1, 2, 3) are related with the slots you can find in the machine.
				// The "63" value is basically the 63th position of the "Probability Table", the 63th position (or is it 64th?) will give you the "6", 6 = Best prize, Jackpot.
				// You ask, what's this probability table? That's the probability table -> https://imgur.com/a/rBidKj9
				// But, there's a "downside", if you only change the first three values, the slots icons will tell that you lost but in reality, you won but don't get fooled.
				// You will probably think "this way it will be more safer, correct?", no, incorrect. Let's not forget we're messing with locals, in this case, they're just icons.
				// Rockstar knows you're winning and it's being sent to the server.
				// So, the last three values (1082130432) are basically the "Jackpot" icons, you're not obliged to use the last three, it's just to make it look 'cool'.
				// You NEED to run this in a LOOP, always. The good part is, you don't need to leave the casino if you want to disable the Jackpot option.

				gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
					auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
					*script_local(slots_thread, 2097).at(1).as<unsigned int*>() = 63;
					*script_local(slots_thread, 2097).at(2).as<unsigned int*>() = 63;
					*script_local(slots_thread, 2097).at(3).as<unsigned int*>() = 63;
					//////////////////////////////////////////////////////////////////
					*script_local(slots_thread, 2097).at(5).as<unsigned int*>() = 1082130432;
					*script_local(slots_thread, 2097).at(6).as<unsigned int*>() = 1082130432;
					*script_local(slots_thread, 2097).at(7).as<unsigned int*>() = 1082130432;
				});
			});
		}
		if (g_gui.bet_multiplier) {
			g_fiber_pool->queue_job([] {

				// Change all the bet multipliers to 1000x so even if you get the worst prize from the slots, you'll earn the same value as you would get with a jackpot!

				gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
					auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
					for (int i = 1; i <= 10; i++) {
						auto local1354 = script_local(slots_thread, 1354).at(278);
						*local1354.at(i).as<int*>() = 1000;
					}
				});
			});
		}
		if (g_gui.slots_bet) {
			g_fiber_pool->queue_job([]
				{

					// Always Win 2,500,00 in ANY MACHINE, no need to use Deity or Diamond anymore.
					// Explaining a bit more about the code, the value 500 means the minimum bet
					// the 5 is the multiplier, let me explain better, when you sit in a machine, you have two options, SPACE to increase bet the and TAB to change to the max BET, the local_2134 is responsible for that.

					gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
						auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
						auto local_219 = script_local(slots_thread, 219);
						auto local_2131 = script_local(slots_thread, 2131).as<int*>();

						*local_219.at(*local_2131, 21).at(16).as<int*>() = 500;
						*script_local(slots_thread, 2134).as<int*>() = 5;
					});
				});
		}
		if (g_gui.increase_chips_limit) {
			g_fiber_pool->queue_job([] {

				// Used to increase the chips limit, from 2B to 2.1B, not that useful but feel free to give a shot.

				*script_global(262145).at(26469).as<int*>() = INT_MAX;
			});
		}
		if (g_gui.cas_cooldown_bypass) {
			g_fiber_pool->queue_job([] {

				// Variant #1: The most popular and maybe detected (?) variant, i prefer this one, it's straight and... less buggy, but, if you're scared of using it, use the other variants.

				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CASINO_CHIPS_WON_GD"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CASINO_CHIPS_WONTIM"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CASINO_CHIPS_PUR_GD"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CASINO_CHIPS_PURTIM"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CASINO_MEM_BONUS"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_0"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_1"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_2"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_3"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_4"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_5"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_6"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_7"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_8"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_GMBLNG_9"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_0"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_1"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_2"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_3"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_4"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_5"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_6"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_7"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_8"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_TIME_PLYING_9"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CASINO_BAN_TIME"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CASINO_GMBLNG_GD"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_CUR_GMBLNG_GD"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_CUR_GMBLNG_PX"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24_PX"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_TRHSLD"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24_1"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24_2"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24_3"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24_4"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24_5"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24_6"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24_7"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_GMBLNG_L24_8"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_CUR_GMBLNG_HR"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CAS_24H_GMBLNG_PX"), 0, TRUE);
				STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CHIPS_COL_TIME"), 0, TRUE);
				});
		}
		if (g_gui.cas_cooldown_bypass_2) {

			// Variant #2: this variant is interesting, i like it because that's the most simple way to bypass the cooldown, how it works? it's simple, the original value is 2880.
			// 2880 SECONDS = 48 MINUTES IRL, so, these tuneables are used as a timer to reset the WON_GD and PUR_GD stats, yes, that's exactly how it works, set them to 0 or -1 (way better and faster?)
			// then, after changing the value, you don't need to do anything, just sit and rest, the game will reset them for you!
			// but, there's a catch, if you're the type of player that like to abuse from methods, be careful, sometimes, you'll get "banned" from casino, and the only way to get "unbanned" is to use the above variant, the #1.

			*script_global(262145).at(26470).as<int*>() = 0;
			*script_global(262145).at(26471).as<int*>() = 0;
		}
		if (g_gui.cas_cooldown_bypass_3) {

			// Variant #3: this variant is... simple and i wouldn't recommend it, the default value is 5,000,000, so, how it works? well, let's say, normally you can only earn up to 5M max and then
			// you need to wait 48 minutes to play again (yes, those 2880 above will help with that), anyway, you need to set to a higher value, maybe INT_MAX or any value you want.
			// you're the boss, you decide the value, it can be useful to be used as a 'limit' if you want to prevent people from abusing it, it's really useful.
			// the downside is, let's say you set this tuneable to 2,000,000,000 and you earned 2B of chips, you'll need to wait the cooldown, so, you can use the above variant with this, up to you.

			*script_global(262145).at(26473).as<int*>() = INT_MAX;

		}
		if (g_gui.stop_anim) {
			g_fiber_pool->queue_job([] {

				// This will try to stop the character animation while playing the slot machine, it works but it can be buggy sometimes, try it.

				gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
					auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
					*script_local(slots_thread, 1648).as<int*>() = 4;
					});
			});
		}
		if (g_gui.force_trans) {
			g_fiber_pool->queue_job([] {

				// My favorite way to earn fast chips! Run this code in a loop (fastest loop you got) then enable with Jackpot then see the magic.
				// But, there's a big problem with it, i would recommend before using it, please mute the game or... disable the slots sounds because it will get loud, really loud.

				gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
					auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
					*script_local(slots_thread, 1644).as<int*>() = 50341673;
					});
			});
		}
		if (g_gui.fast_spin) {
			g_fiber_pool->queue_job([] {

				// You already know about it or you probably already seen it somewhere so i won't give much details about it.

				gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
					auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
					auto local_3407 = script_local(slots_thread, 3407).at(PLAYER::PLAYER_ID(), 11).at(10).as<int*>();
					if (*local_3407 == 3) {
						*script_local(slots_thread, 3407).at(PLAYER::PLAYER_ID(), 11).at(10).as<int*>() = 6;
					}
					});
			});
		}
		if (g_gui.auto_play) {
			g_fiber_pool->queue_job([] {

				// Basically, it will Auto-Play the slot machine.

				gta_util::execute_as_script(RAGE_JOAAT("CASINO_SLOTS"), [] {
					auto slots_thread = gta_util::find_script_thread(RAGE_JOAAT("CASINO_SLOTS"));
					auto local_1644 = script_local(slots_thread, 1644).as<int*>();
					auto local_3407 = script_local(slots_thread, 3407).at(PLAYER::PLAYER_ID(), 11).at(10).as<int*>();
					if (*local_3407 == 0) {
						*script_local(slots_thread, 1644).as<int*>() = 8;
					}
					});
			});
		}
		if (g_gui.auto_play_blackjack) {
			g_fiber_pool->queue_job([] {

				// A very "complex" code to Auto-Play blackjack, quick note, use it with Anti-AFK.

				gta_util::execute_as_script(RAGE_JOAAT("BLACKJACK"), [] {
					auto blackjack_thread = gta_util::find_script_thread(RAGE_JOAAT("BLACKJACK"));
					auto current_table = script_local(blackjack_thread, 1782).at(PLAYER::PLAYER_ID(), 8).at(4).as<int*>();
					auto all_buttons_ready = script_local(blackjack_thread, 2039).at(874).at(693).as<int*>();

					if (is_context_active("BJACK_BET") && *all_buttons_ready == 5) {
						*script_local(blackjack_thread, 1782).at(PLAYER::PLAYER_ID(), 8).as<int*>() = 36864;
					}
					});
			});
		}
		if (g_gui.rig_blackjack) {
			g_fiber_pool->queue_job([] {

				// A basic rig blackjack code but, you won't need to spend any of your chips because it will make the bet FREE and you'll always win 2,500,000 chips, always.

				gta_util::execute_as_script(RAGE_JOAAT("BLACKJACK"), [] {
					auto blackjack_thread = gta_util::find_script_thread(RAGE_JOAAT("BLACKJACK"));
					auto current_table = script_local(blackjack_thread, 1782).at(PLAYER::PLAYER_ID(), 8).at(4).as<int*>();
					auto cur_card = script_local(blackjack_thread, 122).at(1).at(*current_table, 211);

					if (*current_table != -1) {
						*cur_card.at(1).as<int*>() = 1;
						*script_local(blackjack_thread, 2042).as<int*>() = 1;
						*cur_card.at(2).as<int*>() = 0;
						*script_local(blackjack_thread, 2043).as<int*>() = 0;
						*cur_card.at(3).as<int*>() = 23;
						*script_local(blackjack_thread, 2044).as<int*>() = 23;
						*cur_card.at(4).as<int*>() = 0;
						*script_local(blackjack_thread, 2045).as<int*>() = 0;
						*script_local(blackjack_thread, 2039).at(244).as<int*>() = 0;
						*script_local(blackjack_thread, 2039).at(246).as<int*>() = 0;
						*script_local(blackjack_thread, 3626).at(14).as<int*>() = 2500000;
						*script_local(blackjack_thread, 122).at(1580).at(1).as<int*>() = 0;
					}
					});
			});
		}
		if (g_gui.bypass_cooldown_err) {
			g_fiber_pool->queue_job([] {

				// A Basic message bypass, not worth mentioning it.

				int message_id = *script_global(MessageID_1).as<int*>();

				if (message_id == 20 || message_id == 4) {
					*script_global(MessageID_2).as<int*>() = 0;
				}
			});
		}
	}

	void gui::script_func()
	{
		g_gui.script_init();
		while (true)
		{
			g_gui.increase_chips_limit = true;
			g_gui.cas_cooldown_bypass = true;
			g_gui.bypass_cooldown_err = true;
			g_gui.script_on_tick();
			script::get_current()->yield();
		}
	}

	void gui::delayed_func()
	{
		g_gui.script_init();
		while (true)
		{
			g_gui.delayed_loops();
			script::get_current()->yield(6000ms);
		}
	}
}
