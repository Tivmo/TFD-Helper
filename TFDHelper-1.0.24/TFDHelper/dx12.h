#pragma once

#include <windows.h>
#include <psapi.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "MinHook/Include/MinHook.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx12.h"
#include "ImGui/imgui_impl_win32.h"
#include <fstream>

#include "SDK.h"
#include "cheat.h"

namespace DX12
{
	typedef HRESULT(APIENTRY* Present12) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef HRESULT(APIENTRY* CreateCommandQueue)(ID3D12Device* pDevice, const D3D12_COMMAND_QUEUE_DESC* pDesc, REFIID riid, void** ppCommandQueue);
	typedef HRESULT(APIENTRY* CreateSwapChain)(IDXGIFactory* pFactory, IUnknown* pDeviceOrQueue, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain);
	//typedef void(APIENTRY* ExecuteCommandLists)(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);

	bool InitWindow();

	bool DeleteWindow();

	bool CreateHook(uint16_t Index, void** Original, void* Function);

	void DisableHook(uint16_t Index);

	void DisableAll();

	bool Init();

	HRESULT APIENTRY hkPresent(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
	HRESULT APIENTRY hkCreateCommandQueue(ID3D12Device* pDevice, const D3D12_COMMAND_QUEUE_DESC* pDesc, REFIID riid, void** ppCommandQueue);
	HRESULT APIENTRY hkCreateSwapChain(IDXGIFactory* pFactory, IUnknown* pDeviceOrQueue, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain);
	//void hkExecuteCommandLists(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists);

	extern DX12::Present12 oPresent;
	extern DX12::CreateCommandQueue oCreateCommandQueue;
	extern DX12::CreateSwapChain oCreateSwapChain;
	//extern DX12::ExecuteCommandLists oExecuteCommandLists;
	extern uint64_t* MethodsTable;
	extern std::unordered_map<IDXGISwapChain*, ID3D12CommandQueue*> g_SwapChainToQueue;

	extern bool isOverlayReady;
	extern char dlldir[512];
	extern WNDCLASSEX WindowClass;
	extern HWND WindowHwnd;

	namespace Process {
		extern DWORD ID;
		extern HANDLE Handle;
		extern HWND Hwnd;
		extern HMODULE Module;
		extern WNDPROC WndProc;
		extern int WindowWidth;
		extern int WindowHeight;
		extern LPCSTR Title;
		extern LPCSTR ClassName;
		extern LPCSTR Path;
	}

	namespace DirectX12Interface {
		extern ID3D12Device* Device;
		extern ID3D12DescriptorHeap* DescriptorHeapBackBuffers;
		extern ID3D12DescriptorHeap* DescriptorHeapImGuiRender;
		extern ID3D12GraphicsCommandList* CommandList;
		extern ID3D12CommandQueue* CommandQueue;

		struct _FrameContext {
			ID3D12CommandAllocator* CommandAllocator;
			ID3D12Resource* Resource;
			D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
		};

		extern uint64_t BuffersCounts;
		extern _FrameContext* FrameContext;
	}

	struct ExampleAppConsole
	{
		char                  InputBuf[256];
		ImVector<char*>       Items;
		ImVector<const char*> Commands;
		ImVector<char*>       History;
		int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
		ImGuiTextFilter       Filter;
		bool                  AutoScroll;
		bool                  ScrollToBottom;

		ExampleAppConsole()
		{
			//IMGUI_DEMO_MARKER("Examples/Console");
			ClearLog();
			memset(InputBuf, 0, sizeof(InputBuf));
			HistoryPos = -1;

			// "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
			Commands.push_back("help");
			Commands.push_back("history");
			Commands.push_back("clear");
			Commands.push_back("find");
			AutoScroll = true;
			ScrollToBottom = false;
			AddLog("--Start of Console History--");
		}
		~ExampleAppConsole()
		{
			ClearLog();
			for (int i = 0; i < History.Size; i++)
				free(History[i]);
		}

		// Portable helpers
		static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
		static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
		static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
		static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

		void    ClearLog()
		{
			for (int i = 0; i < Items.Size; i++)
				free(Items[i]);
			Items.clear();
		}

		void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			// FIXME-OPT
			char buf[1024];
			va_list args;
			va_start(args, fmt);
			vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
			buf[IM_ARRAYSIZE(buf) - 1] = 0;
			va_end(args);
			Items.push_back(Strdup(buf));
		}

		void    Draw(const char* title)
		{
			ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
			ImGui::Begin(title);

			if (ImGui::SmallButton("Clear")) { ClearLog(); }
			ImGui::SameLine();
			bool copy_to_clipboard = ImGui::SmallButton("Copy");
			//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

			ImGui::Separator();

			// Options menu
			if (ImGui::BeginPopup("Options"))
			{
				ImGui::Checkbox("Auto-scroll", &AutoScroll);
				ImGui::EndPopup();
			}

			// Options, Filter
			if (ImGui::Button("Options"))
				ImGui::OpenPopup("Options");
			ImGui::SameLine();
			Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
			ImGui::Separator();

			// Reserve enough left-over height for 1 separator + 1 input text
			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
			{
				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::Selectable("Clear")) ClearLog();
					ImGui::EndPopup();
				}
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
				if (copy_to_clipboard)
					ImGui::LogToClipboard();
				for (int i = 0; i < Items.Size; i++)
				{
					const char* item = Items[i];
					if (!Filter.PassFilter(item))
						continue;

					ImVec4 color;
					bool has_color = false;
					if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
					else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
					if (has_color)
						ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::TextUnformatted(item);
					if (has_color)
						ImGui::PopStyleColor();
				}
				if (copy_to_clipboard)
					ImGui::LogFinish();

				if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
					ImGui::SetScrollHereY(1.0f);
				ScrollToBottom = false;

				ImGui::PopStyleVar();
			}
			ImGui::EndChild();
			ImGui::Separator();

			// Command-line
			bool reclaim_focus = false;
			ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
			if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
			{
				char* s = InputBuf;
				Strtrim(s);
				if (s[0])
					ExecCommand(s);
				strcpy(s, "");
				reclaim_focus = true;
			}

			// Auto-focus on window apparition
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

			ImGui::End();
		}

		void FindObjects(std::string cmd)
		{
			std::string NameToSearch = cmd.substr(cmd.find_first_of(" ")+1);
			AddLog("Searching for '%s'...", NameToSearch.c_str());
			for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
			{
				SDK::UObject* Obj = SDK::UObject::GObjects->GetByIndex(i);

				if (!Obj)
					continue;

				if (Obj->IsDefaultObject())
					continue;

				if (Obj->GetFullName().contains(NameToSearch))
				{
					AddLog("%s - 0x%p", Obj->GetFullName().c_str(), (void*)Obj);
				}
			}
		}

		void    ExecCommand(const char* command_line)
		{
			AddLog("# %s\n", command_line);

			// Insert into history. First find match and delete it so it can be pushed to the back.
			// This isn't trying to be smart or optimal.
			HistoryPos = -1;
			for (int i = History.Size - 1; i >= 0; i--)
				if (Stricmp(History[i], command_line) == 0)
				{
					free(History[i]);
					History.erase(History.begin() + i);
					break;
				}
			History.push_back(Strdup(command_line));
			std::string cmd = std::string(command_line);
			// Process command
			if (cmd == "clear")
			{
				ClearLog();
			}
			else if (cmd == "help")
			{
				AddLog("Commands:");
				for (int i = 0; i < Commands.Size; i++)
					AddLog("- %s", Commands[i]);
			}
			else if (cmd == "history")
			{
				int first = History.Size - 10;
				for (int i = first > 0 ? first : 0; i < History.Size; i++)
					AddLog("%3d: %s\n", i, History[i]);
			}
			else if (cmd.starts_with("find "))
			{
				FindObjects(cmd);
			}
			else
			{
				AddLog("Unknown command: '%s'\n", command_line);
			}

			// On command input, we scroll to bottom even if AutoScroll==false
			ScrollToBottom = true;
		}

		// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
		{
			ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
			return console->TextEditCallback(data);
		}

		int     TextEditCallback(ImGuiInputTextCallbackData* data)
		{
			//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
			switch (data->EventFlag)
			{
			case ImGuiInputTextFlags_CallbackCompletion:
			{
				// Example of TEXT COMPLETION

				// Locate beginning of current word
				const char* word_end = data->Buf + data->CursorPos;
				const char* word_start = word_end;
				while (word_start > data->Buf)
				{
					const char c = word_start[-1];
					if (c == ' ' || c == '\t' || c == ',' || c == ';')
						break;
					word_start--;
				}

				// Build a list of candidates
				ImVector<const char*> candidates;
				for (int i = 0; i < Commands.Size; i++)
					if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
						candidates.push_back(Commands[i]);

				if (candidates.Size == 0)
				{
					// No match
					AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
				}
				else if (candidates.Size == 1)
				{
					// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0]);
					data->InsertChars(data->CursorPos, " ");
				}
				else
				{
					// Multiple matches. Complete as much as we can..
					// So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
					int match_len = (int)(word_end - word_start);
					for (;;)
					{
						int c = 0;
						bool all_candidates_matches = true;
						for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
							if (i == 0)
								c = toupper(candidates[i][match_len]);
							else if (c == 0 || c != toupper(candidates[i][match_len]))
								all_candidates_matches = false;
						if (!all_candidates_matches)
							break;
						match_len++;
					}

					if (match_len > 0)
					{
						data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
						data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
					}

					// List matches
					AddLog("Possible matches:\n");
					for (int i = 0; i < candidates.Size; i++)
						AddLog("- %s\n", candidates[i]);
				}

				break;
			}
			case ImGuiInputTextFlags_CallbackHistory:
			{
				// Example of HISTORY
				const int prev_history_pos = HistoryPos;
				if (data->EventKey == ImGuiKey_UpArrow)
				{
					if (HistoryPos == -1)
						HistoryPos = History.Size - 1;
					else if (HistoryPos > 0)
						HistoryPos--;
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					if (HistoryPos != -1)
						if (++HistoryPos >= History.Size)
							HistoryPos = -1;
				}

				// A better implementation would preserve the data on the current input line along with cursor position.
				if (prev_history_pos != HistoryPos)
				{
					const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, history_str);
				}
			}
			}
			return 0;
		}
	};

	static ExampleAppConsole console;

}