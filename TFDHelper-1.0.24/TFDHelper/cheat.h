#pragma once

#include "SDK.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <TlHelp32.h>
#include "SimpleIni.h"
#include <bitset>
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#include "dx12.h"
#include "render.h"
#include "config.h"
#include "menu.h"
#include <chrono>


namespace Cheat
{

	extern TFD::AM1Player* LocalPlayerCharacter;
	extern TFD::AM1PlayerControllerInGame* LocalPlayerController;
	extern TFD::UM1MissionControlComponent* Missions;
	extern TFD::UWorld* GWorld;


	extern bool TrySpawnGenericLoot;
	extern bool TrySpawnVaultLoot;
	extern float BoundsScale;
	extern int CurrentPresetIndex;

	extern std::unordered_map<int, std::string> PresetsMap;
	static std::chrono::steady_clock::time_point AutoInstantInfilStartTime = std::chrono::steady_clock::now();

	struct CONTROLLER_STATE
	{
		XINPUT_STATE state;
		bool bConnected;
	};
	HRESULT UpdateControllerState();

	extern __int64 (*oPostRender)(void* ViewportClient, void* DebugCanvas);
	__int64 PostRender(void* self, void* canvas);

	// No-spread and No-recoil memory writing
	//void ToggleMemFeature(int what);

	// Both of these must return true before any other cheat functions are called
	bool IsValidUWorld();
	bool IsCheatReady();

	// Do all of the keybind checks in this function!
	//void HandleKeybinds();

	bool WorldToScreen(const SDK::FVector& worldLoc, SDK::FVector2D* screenPos);

	


	bool IsMovingByWire();
	bool IsInAir();
	void PlayerEnemyESP();
	void Loot();
	void Aimbot();
	void PresetOverlay();
	void PresetRefresh();
	void PresetActivate();
	void ResearchBookmarkedItems();
	void InstantInfiltration();
	TFD::AActor* GetClosestEnemy(int& id);
	void SaveCacheData();
	void LoadCacheData();

	void MissionESPTeleport();
	void AutoRestartMission();

	void WeaponModifications();
	void ModifyGrapple();
	void MultiplyDrops(TFD::AActor* Actor, int Amount, bool isVault);
	bool GetSpareRounds(TFD::EM1RoundsType RoundsType, int RoundsPerLoot);

	//void RapidFire();
	//void InstantReload();
	//void NoRecoilAndSpread();

	void CreateConsole();
	void CreateCheatManager();
	void ToggleCheatUI();

}