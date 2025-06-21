// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>

#include "dx12.h"
#include "cheat.h"
#include "config.h"
#include <iostream>
#include <fstream>



//#define IS_DEBUG_VERSION


DWORD WINAPI MainThread(LPVOID lpParameter) {

#ifdef IS_DEBUG_VERSION
	// Uncomment the next 4 lines to show a console window. Give the main game window focus before the 5 second sleep finishes or the DX12 will grab the wrong window size
	AllocConsole();
	FILE* outFile;
	errno_t err = freopen_s(&outFile, "CONOUT$", "w", stdout);
	if (err != 0) {
		std::cerr << "Error opening file" << std::endl;
		return 1;
	}
	//freopen_s(nullptr, "TFD_Log_In.log", "r", stdin);
#endif

	int procID = GetCurrentProcessId();
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	uintptr_t moduleBase = 0x0;
	uintptr_t moduleSize = 0x0;
	do
	{
		if (!strcmp(mEntry.szModule, "M1-Win64-Shipping.exe"))
		{
			moduleBase = (uintptr_t)mEntry.hModule;
			moduleSize = mEntry.modBaseSize;
		}
	} while (Module32Next(hModule, &mEntry));
	CloseHandle(hModule);

	uintptr_t NGSPtr = TFD::SearchSignature(procID, moduleBase, moduleSize, TFD::NGSSig, TFD::NGSMask);
	if (!NGSPtr)
	{
#ifdef IS_DEBUG_VERSION
		std::cout << "[Cheat] Failed to find NGS bypass.\n";
#endif
	}
	else
	{
		uintptr_t NGSByte = moduleBase + NGSPtr;
		DWORD old;
		byte bypass = 0x75;
		VirtualProtect((void*)NGSByte, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
		memcpy((void*)NGSByte, &bypass, sizeof(uint8_t));
		VirtualProtect((void*)NGSByte, sizeof(uint8_t), old, NULL);

		NGSByte = moduleBase + NGSPtr + 0x47;
		bypass = 0x84;
		VirtualProtect((void*)NGSByte, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
		memcpy((void*)NGSByte, &bypass, sizeof(uint8_t));
		VirtualProtect((void*)NGSByte, sizeof(uint8_t), old, NULL);
	}


	bool InitHook = false;
	while (InitHook == false) {
		if (DX12::Init() == true) {
			DX12::CreateHook(160, (void**)&DX12::oCreateSwapChain, DX12::hkCreateSwapChain);
			DX12::CreateHook(140, (void**)&DX12::oPresent, DX12::hkPresent);
			InitHook = true;
		}
	}

	bool WindowFocus = false;
	while (WindowFocus == false) {
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (GetCurrentProcessId() == ForegroundWindowProcessID) {

			DX12::Process::ID = GetCurrentProcessId();
			DX12::Process::Handle = GetCurrentProcess();
			DX12::Process::Hwnd = GetForegroundWindow();

			char TempClassName[MAX_PATH];
			GetClassName(DX12::Process::Hwnd, TempClassName, sizeof(TempClassName));
//#ifdef IS_DEBUG_VERSION
			//std::cout << "[Main]: " << TempClassName << std::endl;
//#endif
			//UnrealWindow
			if (strcmp(TempClassName, "UnrealWindow") == 0)
			{
				RECT TempRect;
				GetWindowRect(DX12::Process::Hwnd, &TempRect);
				DX12::Process::WindowWidth = TempRect.right - TempRect.left;
				DX12::Process::WindowHeight = TempRect.bottom - TempRect.top;

				char TempTitle[MAX_PATH];
				GetWindowText(DX12::Process::Hwnd, TempTitle, sizeof(TempTitle));
				DX12::Process::Title = TempTitle;


				DX12::Process::ClassName = TempClassName;

				char TempPath[MAX_PATH];
				GetModuleFileNameEx(DX12::Process::Handle, NULL, TempPath, sizeof(TempPath));
				DX12::Process::Path = TempPath;
				WindowFocus = true;
			}
		}
		Sleep(100);
	}

	DX12::isOverlayReady = true;

	bool InitCheat = false;
	while(InitCheat == false)
	{
		TFD::Init_Code status = TFD::InitSDK(procID, moduleBase, moduleSize);
		if (status != TFD::Init_Code::Success)
		{
#ifdef IS_DEBUG_VERSION
			std::cout << "Cheat Init Status: " << status << std::endl;
#endif
			Sleep(100);
		}
		else
			InitCheat = true;
	}

	return 0;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD windowProcessId;
	GetWindowThreadProcessId(hwnd, &windowProcessId);

	// Compare the process ID with the target process ID
	if (windowProcessId == static_cast<DWORD>(lParam)) {
		// Get the class name of the window
		char className[256];
		GetClassNameA(hwnd, className, sizeof(className));

		// Ignore console windows
		if (strcmp(className, "ConsoleWindowClass") == 0) {
			return TRUE; // Continue enumeration
		}

		// Check if the window is the main application window
		if (GetWindow(hwnd, GW_OWNER) == nullptr && IsWindowVisible(hwnd)) {
			// Main window found
			SetLastError(0); // Clear the last error to indicate success
			*reinterpret_cast<HWND*>(lParam) = hwnd;
			return FALSE; // Stop enumeration
		}
	}
	return TRUE; // Continue enumeration
}

HWND GetMainWindowHandle(DWORD processId) {
	HWND hwndMain = nullptr;
	LPARAM lParam = reinterpret_cast<LPARAM>(&hwndMain);

	// Enumerate all top-level windows
	EnumWindows(EnumWindowsProc, lParam);

	// If hwndMain is still nullptr, no main window was found
	if (hwndMain == nullptr && GetLastError() == 0) {
		return hwndMain;
	}

	return hwndMain;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		DX12::Process::Module = hModule;
		GetModuleFileNameA(hModule, DX12::dlldir, 512);
		for (size_t i = strlen(DX12::dlldir); i > 0; i--) { if (DX12::dlldir[i] == '\\') { DX12::dlldir[i + 1] = 0; break; } }
		CreateThread(0, 0, MainThread, 0, 0, 0);
		break;
	case DLL_PROCESS_DETACH:
		FreeLibraryAndExitThread(hModule, TRUE);
		DX12::DisableAll();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}

