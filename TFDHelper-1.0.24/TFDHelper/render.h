#pragma once
#include <cstddef>
#include "SDK.h"
#include "cheat.h"
#include "ImGui/imgui.h"



namespace Render
{
	/* FUNCTIONS */
	void RenderOverlay();

	static void* R_GetCmdBuffer(int bytes);
	void R_DrawText(float x, float y, UC::FString text, SDK::FLinearColor* color, bool background = false);
	void R_DrawText(float x, float y, std::string text, SDK::FLinearColor* color, bool background = false);
	void R_DrawRect(float x, float y, float w, float h, SDK::FLinearColor* color);
	void R_DrawFillRect(float x, float y, float w, float h, SDK::FLinearColor* color);
	void R_DrawLine(float x, float y, float x2, float y2, SDK::FLinearColor* color);
	void R_DrawCircle(float x, float y, float radius, SDK::FLinearColor* color);
	void R_End();

	/* TYPE OF RENDER COMMANDS */
	enum rd_cmdIDs
	{
		RD_END,
		RD_TEXT,
		RD_RECT,
		RD_FILLRECT,
		RD_LINE,
		RD_CIRCLE
	};

	/* STORES RENDER COMMANDS */
	struct rd_Commands
	{
		byte cmds[0x80000];
		int used;
	};

	static rd_Commands Commands;

	/* RENDER COMMANDS DATA */
	struct rd_cmd
	{
		int cmdID;
	};

	struct rd_text : rd_cmd
	{
		char txt[32];
		float x, y;
		float color[4];
		bool bg;
	};

	struct rd_rect : rd_cmd
	{
		float x, y, w, h;
		float color[4];
	};

	struct rd_fillrect : rd_cmd
	{
		float x, y, w, h;
		float color[4];
	};

	struct rd_line : rd_cmd
	{
		float x, y, x2, y2;
		float color[4];
	};

	struct rd_circle : rd_cmd
	{
		float x, y;
		float radius;
		float color[4];
	};

	struct rd_end : rd_cmd
	{

	};

}