#pragma once
#include "SimpleIni.h"
#include <vector>


namespace CFG
{

	void LoadCFG();
	void SaveCFG();

	extern int cfg_ShowMenuKey;

	extern bool		cfg_Actors_EnableESP;
	extern bool		cfg_Actors_DrawPlayerNames;
	extern bool		cfg_Actors_DrawPlayerBoxes;
	extern bool     cfg_Actors_DrawPlayerCircles;
	extern bool		cfg_Actors_DrawPlayerLines;
	extern bool		cfg_Actors_DrawEnemyNames;
	extern bool		cfg_Actors_DrawEnemyBoxes;
	extern bool     cfg_Actors_DrawEnemyCircles;
	extern bool		cfg_Actors_DrawEnemyLines;
	extern float    cfg_ESP_ShapeSize;
	extern float    cfg_ESP_ShapeDistanceScale;

	extern bool		cfg_Loot_EnableItemESP;
	extern int		cfg_Loot_ItemFilterLevel;
	extern bool		cfg_Loot_DrawItemBoxes;
	extern bool     cfg_Loot_DrawItemCircles;
	extern bool		cfg_Loot_DrawItemNames;
	//extern bool		cfg_Loot_DrawItemLines;
	extern bool		cfg_Loot_DrawVaults;
	extern bool		cfg_Loot_DrawSupplyResources;
	extern bool		cfg_Loot_DrawVoidVesselBox;
	extern bool		cfg_Loot_EnableLootVacuum;
	extern float	cfg_Loot_LootVacuumRange;
	extern int		cfg_Loot_LootVacuumKey;
	extern int		cfg_Loot_SpawnLootKey;
	extern int		cfg_Loot_SpawnVaultRewardKey;

	extern bool		cfg_Aim_EnableAimbot;
	extern bool		cfg_Aim_EnableAimbotHold;
	extern bool		cfg_Aim_EnableAimbotToggle;
	extern int		cfg_Aim_AimbotHoldKey;
	extern int		cfg_Aim_AimbotToggleKey;
	extern bool		cfg_Aim_AimbotUseController;
	extern float	cfg_Aim_AimbotFOV;
	extern float	cfg_Aim_AimbotSpeed;
	//extern bool		cfg_Aim_NoSpread;
	//extern bool		cfg_Aim_NoRecoil;
	extern bool		cfg_Aim_NoRecoilAndSpread;
	extern bool		cfg_Aim_NoReload;
	
	extern bool		cfg_Aim_RapidFire;
	extern float	cfg_Aim_FireRate;
	
	extern float	cfg_Extra_TimeScale;
	extern int		cfg_Extra_TimeScaleKey;
	extern int		cfg_Extra_TimeScaleHoldKey;
	extern bool		cfg_Extra_EnableUEConsole;

	extern bool		cfg_Mission_EnableMissionESP;
	extern bool		cfg_Mission_EnableMissionAutoRestart;
	extern float	cfg_Mission_MissionAutoRestartDelay;
	extern int		cfg_Mission_MissionAutoRestartKey;
	extern int		cfg_Mission_MissionTeleportKey;
	extern bool		cfg_Mission_EnableInstantInfiltration;
	//extern bool		cfg_Mission_EnableAutoInstantInfiltration;
	extern int		cfg_Mission_InstantInfiltrationKey;

	extern bool		cfg_Hotswap_EnableOverlay;
	extern int		cfg_Hotswap_PresetSelectKey;
	extern int		cfg_ResearchQuantity;
	//extern std::vector<int>		cfg_Hotswap_Presets;

	/*
	* cfg_HotSwapOverlay = ini.GetBoolValue("Preset", "HotSwapOverlay", cfg_HotSwapOverlay);
			cfg_PresetSelectKey = (int)ini.GetDoubleValue("Preset", "PresetSelectKey", cfg_PresetSelectKey);
			cfg_HotSwapPreset[0] = (int)ini.GetDoubleValue("Preset", "HotSwapPreset1", cfg_HotSwapPreset[0]);
			cfg_HotSwapPreset[1] = (int)ini.GetDoubleValue("Preset", "HotSwapPreset2", cfg_HotSwapPreset[1]);
			cfg_HotSwapPreset[2] = (int)ini.GetDoubleValue("Preset", "HotSwapPreset3", cfg_HotSwapPreset[2]);
			cfg_HotSwapPreset[3] = (int)ini.GetDoubleValue("Preset", "HotSwapPreset4", cfg_HotSwapPreset[3]);
			cfg_HotSwapPreset[4] = (int)ini.GetDoubleValue("Preset", "HotSwapPreset5", cfg_HotSwapPreset[4]);
			cfg_HotSwapPreset[5] = (int)ini.GetDoubleValue("Preset", "HotSwapPreset6", cfg_HotSwapPreset[5]);
	*/

	extern bool		cfg_Debug_ShowConsole;

	template <typename T>
	void SaveValue(const char* Section, const char* Name, T &Value);
	template <typename T>
	void LoadValue(const char* Section, const char* Name, T &Value);
}