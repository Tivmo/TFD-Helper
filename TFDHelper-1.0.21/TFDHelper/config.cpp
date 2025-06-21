#include "config.h"
#include <iostream>


namespace CFG
{
	CSimpleIniA ini;

	int			cfg_ShowMenuKey = VK_INSERT;

	bool		cfg_Actors_EnableESP = false;
	bool		cfg_Actors_DrawPlayerNames = false;
	bool		cfg_Actors_DrawPlayerBoxes = false;
	bool        cfg_Actors_DrawPlayerCircles = false;
	bool		cfg_Actors_DrawPlayerLines = false;
	bool		cfg_Actors_DrawEnemyNames = false;
	bool		cfg_Actors_DrawEnemyBoxes = false;
	bool        cfg_Actors_DrawEnemyCircles = false;
	bool		cfg_Actors_DrawEnemyLines = false; 
	float 		cfg_ESP_ShapeSize = 2.0f;
	float 		cfg_ESP_ShapeDistanceScale = 100.0f;

	bool		cfg_Loot_EnableItemESP = false;
	int			cfg_Loot_ItemFilterLevel = 1;
	bool		cfg_Loot_DrawItemBoxes = false;
	bool        cfg_Loot_DrawItemCircles = false;
	bool		cfg_Loot_DrawItemNames = false;
	//bool		cfg_Loot_DrawItemLines = false;
	bool		cfg_Loot_DrawVaults = false;
	bool		cfg_Loot_DrawSupplyResources = false;
	bool		cfg_Loot_DrawVoidVesselBox = false;
	bool		cfg_Loot_EnableLootVacuum = false;
	float		cfg_Loot_LootVacuumRange = 1000.0f;
	int			cfg_Loot_LootVacuumKey = 0x54;
	int			cfg_Loot_SpawnLootKey = VK_F5;
	int			cfg_Loot_SpawnVaultRewardKey = VK_F6;

	bool		cfg_Aim_EnableAimbot = false;
	bool		cfg_Aim_EnableAimbotHold = true;
	bool		cfg_Aim_EnableAimbotToggle = false;
	int			cfg_Aim_AimbotHoldKey = 0x02;
	int			cfg_Aim_AimbotToggleKey = 0x59;
	bool		cfg_Aim_AimbotUseController = false;
	float		cfg_Aim_AimbotFOV = 140.0f;
	float		cfg_Aim_AimbotSpeed = 36.0f;
	bool		cfg_Aim_AimbotLineOfSight = false;
	//bool		cfg_Aim_NoSpread = false;
	//bool		cfg_Aim_NoRecoil = false;
	bool		cfg_Aim_NoRecoilAndSpread = false;
	bool		cfg_Aim_NoReload = false;
	bool		cfg_Aim_RapidFire = false;
	float		cfg_Aim_FireRate = 1.0f;

	float		cfg_Extra_TimeScale = 2.0f;
	int			cfg_Extra_TimeScaleKey = VK_F2;
	int			cfg_Extra_TimeScaleHoldKey = VK_CONTROL;
	bool		cfg_Extra_EnableUEConsole = false;

	bool		cfg_Mission_EnableMissionESP = false;
	bool		cfg_Mission_EnableMissionAutoRestart = false;
	float		cfg_Mission_MissionAutoRestartDelay = 5.0f;
	int			cfg_Mission_MissionAutoRestartKey = VK_F3;
	int			cfg_Mission_MissionTeleportKey = VK_F4;
	bool		cfg_Mission_EnableInstantInfiltration = false;
	int			cfg_Mission_InstantInfiltrationKey = VK_F7;
	//bool		cfg_Mission_EnableAutoInstantInfiltration = false;

	bool		cfg_Hotswap_EnableOverlay = false;
	int			cfg_Hotswap_PresetSelectKey = VK_F8;
	int 		cfg_ResearchQuantity = 1;
	//std::vector<int> cfg_Hotswap_Presets = { -1, -1, -1, -1, -1, -1 };

	bool		cfg_Debug_ShowConsole = false;

	void LoadCFG()
	{
		ini.SetUnicode();
		if (ini.LoadFile("cfg.ini") < 0)
		{
			std::cout << "No cfg, making new\n";
			try
			{
				SaveCFG();
			}
			catch (...)
			{
				std::cout << "cfg fucked up\n";
			}
		}
		else
		{
			LoadValue("Actors", "EnableESP", cfg_Actors_EnableESP);
			LoadValue("Actors", "DrawPlayerNames", cfg_Actors_DrawPlayerNames);
			LoadValue("Actors", "DrawPlayerBoxes", cfg_Actors_DrawPlayerBoxes);
			LoadValue("Actors", "DrawPlayerCircles", cfg_Actors_DrawPlayerCircles);
			LoadValue("Actors", "DrawPlayerLines", cfg_Actors_DrawPlayerLines);
			LoadValue("Actors", "DrawEnemyNames", cfg_Actors_DrawEnemyNames);
			LoadValue("Actors", "DrawEnemyBoxes", cfg_Actors_DrawEnemyBoxes);
			LoadValue("Actors", "DrawEnemyCircles", cfg_Actors_DrawEnemyCircles);
			LoadValue("Actors", "DrawEnemyLines", cfg_Actors_DrawEnemyLines);
			LoadValue("Actors", "ShapeSize", cfg_ESP_ShapeSize);
			LoadValue("Actors", "ShapeDistanceScale", cfg_ESP_ShapeDistanceScale);

			LoadValue("Loot", "EnableItemESP", cfg_Loot_EnableItemESP);
			LoadValue("Loot", "ItemFilterLevel", cfg_Loot_ItemFilterLevel);
			LoadValue("Loot", "DrawItemBoxes", cfg_Loot_DrawItemBoxes);
			LoadValue("Loot", "DrawItemBoxes", cfg_Loot_DrawItemCircles);
			LoadValue("Loot", "DrawItemNames", cfg_Loot_DrawItemNames);
			//LoadValue("Loot", "DrawItemLines", cfg_Loot_DrawItemLines);
			LoadValue("Loot", "DrawVaults", cfg_Loot_DrawVaults);
			LoadValue("Loot", "DrawSupplyResources", cfg_Loot_DrawSupplyResources);
			LoadValue("Loot", "DrawVoidVesselBox", cfg_Loot_DrawVoidVesselBox);
			LoadValue("Loot", "EnableLootVacuum", cfg_Loot_EnableLootVacuum);
			LoadValue("Loot", "LootVacuumRange", cfg_Loot_LootVacuumRange);
			LoadValue("Loot", "LootVacuumKey", cfg_Loot_LootVacuumKey);
			LoadValue("Loot", "SpawnLootKey", cfg_Loot_SpawnLootKey);
			LoadValue("Loot", "SpawnVaultRewardKey", cfg_Loot_SpawnVaultRewardKey);

			LoadValue("Aimbot", "EnableAimbot", cfg_Aim_EnableAimbot);
			LoadValue("Aimbot", "EnableAimbotHold", cfg_Aim_EnableAimbotHold);
			LoadValue("Aimbot", "EnableAimbotToggle", cfg_Aim_EnableAimbotToggle);
			LoadValue("Aimbot", "AimbotHoldKey", cfg_Aim_AimbotHoldKey);
			LoadValue("Aimbot", "AimbotToggleKey", cfg_Aim_AimbotToggleKey);
			LoadValue("Aimbot", "AimbotUseController", cfg_Aim_AimbotUseController);
			LoadValue("Aimbot", "AimbotDistance", cfg_Aim_AimbotFOV);
			LoadValue("Aimbot", "AimbotSpeed", cfg_Aim_AimbotSpeed);
			LoadValue("Aimbot", "AimbotLineOfSight", cfg_Aim_AimbotLineOfSight);
			//LoadValue("Aimbot", "NoSpread", cfg_Aim_NoSpread);
			//LoadValue("Aimbot", "NoRecoil", cfg_Aim_NoRecoil);
			LoadValue("Aimbot", "NoReload", cfg_Aim_NoReload);
			LoadValue("Aimbot", "NoRecoilAndSpread", cfg_Aim_NoRecoilAndSpread);
			LoadValue("Aimbot", "RapidFire", cfg_Aim_RapidFire);

			LoadValue("Extra", "TimeScale", cfg_Extra_TimeScale);
			LoadValue("Extra", "TimeScaleKey", cfg_Extra_TimeScaleKey);
			LoadValue("Extra", "TimeScaleHoldKey", cfg_Extra_TimeScaleHoldKey);

			LoadValue("Missions", "EnableMissionESP", cfg_Mission_EnableMissionESP);
			LoadValue("Missions", "EnableMissionAutoRestart", cfg_Mission_EnableMissionAutoRestart);
			LoadValue("Missions", "MissionAutoRestartDelay", cfg_Mission_MissionAutoRestartDelay);
			LoadValue("Missions", "MissionAutoRestartKey", cfg_Mission_MissionAutoRestartKey);
			LoadValue("Missions", "MissionTeleportKey", cfg_Mission_MissionTeleportKey);

			LoadValue("Hotswap", "EnableOverlay", cfg_Hotswap_EnableOverlay);
			LoadValue("Hotswap", "PresetSelectKey", cfg_Hotswap_PresetSelectKey);
			//LoadValue("Hotswap", "Preset_1", cfg_Hotswap_Presets[0]);
			//LoadValue("Hotswap", "Preset_2", cfg_Hotswap_Presets[1]);
			//LoadValue("Hotswap", "Preset_3", cfg_Hotswap_Presets[2]);
			//LoadValue("Hotswap", "Preset_4", cfg_Hotswap_Presets[3]);
			//LoadValue("Hotswap", "Preset_5", cfg_Hotswap_Presets[4]);
			//LoadValue("Hotswap", "Preset_6", cfg_Hotswap_Presets[5]);
		}
	}

	void SaveCFG()
	{

		SaveValue("Actors", "EnableESP", cfg_Actors_EnableESP);
		SaveValue("Actors", "DrawPlayerNames", cfg_Actors_DrawPlayerNames);
		SaveValue("Actors", "DrawPlayerBoxes", cfg_Actors_DrawPlayerBoxes);
		SaveValue("Actors", "DrawPlayerCircles", cfg_Actors_DrawPlayerCircles);
		SaveValue("Actors", "DrawPlayerLines", cfg_Actors_DrawPlayerLines);
		SaveValue("Actors", "DrawEnemyNames", cfg_Actors_DrawEnemyNames);
		SaveValue("Actors", "DrawEnemyBoxes", cfg_Actors_DrawEnemyBoxes);
		SaveValue("Actors", "DrawEnemyCircles", cfg_Actors_DrawEnemyCircles);
		SaveValue("Actors", "DrawEnemyLines", cfg_Actors_DrawEnemyLines);
		SaveValue("Actors", "ShapeSize", cfg_ESP_ShapeSize);
		SaveValue("Actors", "ShapeDistanceScale", cfg_ESP_ShapeDistanceScale);

		SaveValue("Loot", "EnableItemESP", cfg_Loot_EnableItemESP);
		SaveValue("Loot", "ItemFilterLevel", cfg_Loot_ItemFilterLevel);
		SaveValue("Loot", "DrawItemBoxes", cfg_Loot_DrawItemBoxes);
		SaveValue("Loot", "DrawItemBoxes", cfg_Loot_DrawItemCircles);
		SaveValue("Loot", "DrawItemNames", cfg_Loot_DrawItemNames);
		//SaveValue("Loot", "DrawItemLines", cfg_Loot_DrawItemLines);
		SaveValue("Loot", "DrawVaults", cfg_Loot_DrawVaults);
		SaveValue("Loot", "DrawSupplyResources", cfg_Loot_DrawSupplyResources);
		SaveValue("Loot", "DrawVoidVesselBox", cfg_Loot_DrawVoidVesselBox);
		SaveValue("Loot", "EnableLootVacuum", cfg_Loot_EnableLootVacuum);
		SaveValue("Loot", "LootVacuumRange", cfg_Loot_LootVacuumRange);
		SaveValue("Loot", "LootVacuumKey", cfg_Loot_LootVacuumKey);
		SaveValue("Loot", "SpawnLootKey", cfg_Loot_SpawnLootKey);
		SaveValue("Loot", "SpawnVaultRewardKey", cfg_Loot_SpawnVaultRewardKey);

		SaveValue("Aimbot", "EnableAimbot", cfg_Aim_EnableAimbot);
		SaveValue("Aimbot", "EnableAimbotHold", cfg_Aim_EnableAimbotHold);
		SaveValue("Aimbot", "EnableAimbotToggle", cfg_Aim_EnableAimbotToggle);
		SaveValue("Aimbot", "AimbotHoldKey", cfg_Aim_AimbotHoldKey);
		SaveValue("Aimbot", "AimbotToggleKey", cfg_Aim_AimbotToggleKey);
		SaveValue("Aimbot", "AimbotUseController", cfg_Aim_AimbotUseController);
		SaveValue("Aimbot", "AimbotDistance", cfg_Aim_AimbotFOV);
		SaveValue("Aimbot", "AimbotSpeed", cfg_Aim_AimbotSpeed);
		SaveValue("Aimbot", "AimbotLineOfSight", cfg_Aim_AimbotLineOfSight);
		//SaveValue("Aimbot", "NoSpread", cfg_Aim_NoSpread);
		//SaveValue("Aimbot", "NoRecoil", cfg_Aim_NoRecoil);
		SaveValue("Aimbot", "NoReload", cfg_Aim_NoReload);
		SaveValue("Aimbot", "NoRecoilAndSpread", cfg_Aim_NoRecoilAndSpread);
		SaveValue("Aimbot", "RapidFire", cfg_Aim_RapidFire);

		SaveValue("Extra", "TimeScale", cfg_Extra_TimeScale);
		SaveValue("Extra", "TimeScaleKey", cfg_Extra_TimeScaleKey);
		SaveValue("Extra", "TimeScaleHoldKey", cfg_Extra_TimeScaleHoldKey);

		SaveValue("Missions", "EnableMissionESP", cfg_Mission_EnableMissionESP);
		SaveValue("Missions", "EnableMissionAutoRestart", cfg_Mission_EnableMissionAutoRestart);
		SaveValue("Missions", "MissionAutoRestartDelay", cfg_Mission_MissionAutoRestartDelay);
		SaveValue("Missions", "MissionAutoRestartKey", cfg_Mission_MissionAutoRestartKey);
		SaveValue("Missions", "MissionTeleportKey", cfg_Mission_MissionTeleportKey);

		SaveValue("Hotswap", "EnableOverlay", cfg_Hotswap_EnableOverlay);
		SaveValue("Hotswap", "PresetSelectKey", cfg_Hotswap_PresetSelectKey);
		//SaveValue("Hotswap", "Preset_1", cfg_Hotswap_Presets[0]);
		//SaveValue("Hotswap", "Preset_2", cfg_Hotswap_Presets[1]);
		//SaveValue("Hotswap", "Preset_3", cfg_Hotswap_Presets[2]);
		//SaveValue("Hotswap", "Preset_4", cfg_Hotswap_Presets[3]);
		//SaveValue("Hotswap", "Preset_5", cfg_Hotswap_Presets[4]);
		//SaveValue("Hotswap", "Preset_6", cfg_Hotswap_Presets[5]);

		ini.SaveFile("cfg.ini");
	}


	template <typename T>
	void SaveValue(const char* Section, const char* Name, T &Value)
	{
		if constexpr (std::is_same_v<T, int>)
		{
			ini.SetDoubleValue(Section, Name, Value);
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			ini.SetBoolValue(Section, Name, Value);
		}
		else if constexpr (std::is_same_v<T, float>)
		{
			ini.SetDoubleValue(Section, Name, Value);
		}
	}

	template <typename T>
	void LoadValue(const char* Section, const char* Name, T& Value)
	{
		if constexpr (std::is_same_v<T, int>)
		{
			Value = (int)ini.GetDoubleValue(Section, Name, Value);
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			Value = ini.GetBoolValue(Section, Name, Value);
		}
		else if constexpr (std::is_same_v<T, float>)
		{
			Value = (float)ini.GetDoubleValue(Section, Name, Value);
		}
	}
}