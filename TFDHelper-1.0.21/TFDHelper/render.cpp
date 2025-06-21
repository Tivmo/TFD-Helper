#include "render.h"

namespace Render
{
#define PAD(base, alignment)	(((base)+(alignment)-1) & ~((alignment)-1))
#define PADP(base, alignment)	((void *) PAD((intptr_t) (base), (alignment)))

	void RenderOverlay()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
		ImGui::Begin("Overlay", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
		auto pDrawList = ImGui::GetWindowDrawList();

		void* data = Commands.cmds;
		bool isDone = false;
		while (!isDone)
		{
			data = PADP(data, sizeof(void*));
			switch (*static_cast<const int*>(data))
			{
			case RD_TEXT:
			{
				rd_text* cmd;
				cmd = static_cast<rd_text*>(data);
				ImVec2 textSize = ImGui::CalcTextSize(cmd->txt);
				if (cmd->bg)
				{
					pDrawList->AddRectFilled(ImVec2((cmd->x - (textSize.x / 2)) - 1, cmd->y - 1),
						ImVec2((cmd->x + (textSize.x / 2)) + 1, cmd->y + textSize.y + 1),
						ImColor(0.0f, 0.0f, 0.0f, 0.35f));
				}
				pDrawList->AddText(ImVec2(cmd->x - (textSize.x / 2), cmd->y),
					ImColor(cmd->color[0], cmd->color[1], cmd->color[2], cmd->color[3]),
					(char*)cmd->txt);
				data = static_cast<void*>(cmd + 1);
				break;
			}
			case RD_RECT:
			{
				rd_rect* cmd;
				cmd = static_cast<rd_rect*>(data);
				if (cmd->x > 0 && cmd->x < io.DisplaySize.x && cmd->y > 0 && cmd->y < io.DisplaySize.y)
					if (cmd->w > 0 && cmd->w < io.DisplaySize.x && cmd->h > 0 && cmd->h < io.DisplaySize.y)
						pDrawList->AddRect(ImVec2(cmd->x, cmd->y), ImVec2(cmd->w, cmd->h), ImColor(cmd->color[0], cmd->color[1], cmd->color[2], cmd->color[3]));
				data = static_cast<void*>(cmd + 1);
				break;
			}
			case RD_FILLRECT:
			{
				rd_fillrect* cmd;
				cmd = static_cast<rd_fillrect*>(data);
				pDrawList->AddRectFilled(ImVec2(cmd->x, cmd->y), ImVec2(cmd->w, cmd->h),
					ImColor(cmd->color[0], cmd->color[1], cmd->color[2], cmd->color[3]));
				data = static_cast<void*>(cmd + 1);
				break;
			}
			case RD_LINE:
			{
				rd_line* cmd;
				cmd = static_cast<rd_line*>(data);
				pDrawList->AddLine(ImVec2(cmd->x, cmd->y), ImVec2(cmd->x2, cmd->y2),
					ImColor(cmd->color[0], cmd->color[1], cmd->color[2], cmd->color[3]), 1.0f);
				data = static_cast<void*>(cmd + 1);
				break;
			}
			case RD_CIRCLE:
			{
				rd_circle* cmd = static_cast<rd_circle*>(data);
				if (cmd->radius > 0.0f &&
					cmd->x - cmd->radius < io.DisplaySize.x &&
					cmd->x + cmd->radius > 0 &&
					cmd->y - cmd->radius < io.DisplaySize.y &&
					cmd->y + cmd->radius > 0)
				{
					pDrawList->AddCircle(
						ImVec2(cmd->x, cmd->y),
						cmd->radius,
						ImColor(cmd->color[0], cmd->color[1], cmd->color[2], cmd->color[3]),
						0,
						1.5f
					);
				}
				data = static_cast<void*>(cmd + 1);
				break;
			}
			case RD_END:
			default:
				isDone = true;
				break;
			}
		}
		ImGui::End();

	}


	static void* R_GetCmdBuffer(int bytes)
	{
		rd_Commands* cmdList = &Commands;
		if (!cmdList)
		{
			return nullptr;
		}

		bytes = PAD(bytes, sizeof(void*));

		if (cmdList->used + bytes > sizeof(cmdList->cmds))
		{
			return nullptr;
		}
		void* buffer = cmdList->cmds + cmdList->used;
		cmdList->used += bytes;

		return buffer;
	}

	void R_DrawText(float x, float y, UC::FString text, SDK::FLinearColor* color, bool background)
	{
		void* buffer = R_GetCmdBuffer(sizeof(rd_text));
		if (!buffer)
		{
			return;
		}
		rd_text* cmd = static_cast<rd_text*>(buffer);
		if (!cmd)
		{
			cmd->cmdID = RD_END;
			return;
		}
		cmd->cmdID = RD_TEXT;
		cmd->x = x;
		cmd->y = y;
		std::string str = text.ToString();
		str.copy(cmd->txt, sizeof(cmd->txt) - 1);
		cmd->txt[str.size()] = '\0';
		cmd->color[0] = color->R;
		cmd->color[1] = color->G;
		cmd->color[2] = color->B;
		cmd->color[3] = color->A;
		cmd->bg = background;
	}

	void R_DrawText(float x, float y, std::string text, SDK::FLinearColor* color, bool background)
	{
		void* buffer = R_GetCmdBuffer(sizeof(rd_text));
		if (!buffer)
		{
			return;
		}
		rd_text* cmd = static_cast<rd_text*>(buffer);
		if (!cmd)
		{
			cmd->cmdID = RD_END;
			return;
		}
		cmd->cmdID = RD_TEXT;
		cmd->x = x;
		cmd->y = y;
		std::string str = text;
		str.copy(cmd->txt, sizeof(cmd->txt) - 1);
		cmd->txt[str.size()] = '\0';
		cmd->color[0] = color->R;
		cmd->color[1] = color->G;
		cmd->color[2] = color->B;
		cmd->color[3] = color->A;
		cmd->bg = background;
	}

	void R_DrawRect(float x, float y, float w, float h, SDK::FLinearColor* color)
	{
		void* buffer = R_GetCmdBuffer(sizeof(rd_rect));
		if (!buffer)
		{
			return;
		}
		rd_rect* cmd = static_cast<rd_rect*>(buffer);
		if (!cmd)
		{
			cmd->cmdID = RD_END;
			return;
		}
		cmd->cmdID = RD_RECT;
		cmd->x = x;
		cmd->y = y;
		cmd->w = w;
		cmd->h = h;
		cmd->color[0] = color->R;
		cmd->color[1] = color->G;
		cmd->color[2] = color->B;
		cmd->color[3] = color->A;
	}

	void R_DrawFillRect(float x, float y, float w, float h, SDK::FLinearColor* color)
	{
		rd_fillrect* cmd;
		cmd = static_cast<rd_fillrect*>(R_GetCmdBuffer(sizeof(*cmd)));
		if (!cmd)
			return;
		cmd->cmdID = RD_FILLRECT;
		cmd->x = x;
		cmd->y = y;
		cmd->w = w;
		cmd->h = h;
		cmd->color[0] = color->R;
		cmd->color[1] = color->G;
		cmd->color[2] = color->B;
		cmd->color[3] = color->A;
	}

	void R_DrawLine(float x, float y, float x2, float y2, SDK::FLinearColor* color)
	{
		rd_line* cmd;
		cmd = static_cast<rd_line*>(R_GetCmdBuffer(sizeof(*cmd)));
		if (!cmd)
			return;
		cmd->cmdID = RD_LINE;
		cmd->x = x;
		cmd->y = y;
		cmd->x2 = x2;
		cmd->y2 = y2;
		cmd->color[0] = color->R;
		cmd->color[1] = color->G;
		cmd->color[2] = color->B;
		cmd->color[3] = color->A;
	}

	void R_DrawCircle(float x, float y, float radius, SDK::FLinearColor* color)
	{
		rd_circle* cmd = static_cast<rd_circle*>(R_GetCmdBuffer(sizeof(*cmd)));
		if (!cmd)
			return;
		cmd->cmdID = RD_CIRCLE;
		cmd->x = x;
		cmd->y = y;
		cmd->radius = radius;
		cmd->color[0] = color->R;
		cmd->color[1] = color->G;
		cmd->color[2] = color->B;
		cmd->color[3] = color->A;
	}


	void R_End()
	{
		rd_end* cmd;
		cmd = static_cast<rd_end*>(R_GetCmdBuffer(sizeof(*cmd)));
		if (!cmd)
			return;
		cmd->cmdID = RD_END;
		Commands.used = 0;
	}


}