#include "menu.h"


namespace Menu
{

	bool ShowMenu = false;

	bool Menu_Loot_ESPSection = true;
	bool Menu_Loot_FeatureSection = false;
	bool Menu_Aim_AimSection = true;
	bool Menu_Aim_FeatureSection = false;

	bool cfg_ShowMenuKeyState = false;
	bool cfg_LootVacuumKeyState = false;
	bool cfg_AimbotHoldKeyState = false;
	bool cfg_AimbotToggleKeyState = false;
	bool cfg_TimescaleHoldKeyState = false;
	bool cfg_TimescaleToggleKeyState = false;
	bool cfg_MissionAutoRestartKeyState = false;
	bool cfg_MissionTeleportKeyState = false;
	bool cfg_InstantInfiltrationKeyState = false;
	bool cfg_SpawnLootKeyState = false;
	bool cfg_SpawnVaultRewardKeyState = false;
	bool cfg_ForceGrappleCallKeyState = false;
	bool cfg_ForcePullKeyState = false;
	bool cfg_HotswapKeyState = false;


	void HandleKeybinds()
	{
		//Need to add a check to see if the game window has focus too... also the imgui console... zzz
		if (!Menu::IsCurrentlyBindingKey())
		{
			if (ImGui::IsKeyPressed(CFG::cfg_Loot_LootVacuumKey))
				CFG::cfg_Loot_EnableLootVacuum = !CFG::cfg_Loot_EnableLootVacuum;

			if (CFG::cfg_Aim_EnableAimbotToggle && (ImGui::IsKeyPressed(CFG::cfg_Aim_AimbotToggleKey)))
				CFG::cfg_Aim_EnableAimbot = !CFG::cfg_Aim_EnableAimbot;

			static bool timeEnable = false;
			static bool isTimeHeld = false;
			if (!timeEnable && ImGui::IsKeyDown(CFG::cfg_Extra_TimeScaleHoldKey))
			{
				if (!isTimeHeld)
				{
					Cheat::GWorld->PersistentLevel->WorldSettings->TimeDilation = CFG::cfg_Extra_TimeScale;
					isTimeHeld = true;
				}
			}
			if (isTimeHeld && !ImGui::IsKeyDown(CFG::cfg_Extra_TimeScaleHoldKey))
			{
				Cheat::GWorld->PersistentLevel->WorldSettings->TimeDilation = 1.0f;
				isTimeHeld = false;
			}
			if (ImGui::IsKeyPressed(CFG::cfg_Extra_TimeScaleKey))
			{
				timeEnable = !timeEnable;
				if (timeEnable)
					Cheat::GWorld->PersistentLevel->WorldSettings->TimeDilation = CFG::cfg_Extra_TimeScale;
				else
					Cheat::GWorld->PersistentLevel->WorldSettings->TimeDilation = 1.0f;
			}

			if (ImGui::IsKeyPressed(CFG::cfg_Mission_MissionAutoRestartKey))
				CFG::cfg_Mission_EnableMissionAutoRestart = !CFG::cfg_Mission_EnableMissionAutoRestart;

			if (ImGui::IsKeyPressed(CFG::cfg_Mission_InstantInfiltrationKey))
				Cheat::InstantInfiltration();

			if (ImGui::IsKeyDown(CFG::cfg_Loot_SpawnLootKey))
				Cheat::TrySpawnGenericLoot = true;
			else
				Cheat::TrySpawnGenericLoot = false;

			if (ImGui::IsKeyDown(CFG::cfg_Loot_SpawnVaultRewardKey))
				Cheat::TrySpawnVaultLoot = true;
			else
				Cheat::TrySpawnVaultLoot = false;

			if (ImGui::IsKeyPressed(CFG::cfg_Hotswap_PresetSelectKey))
				Cheat::PresetActivate();

			if (ImGui::IsKeyPressed(VK_UP))
				Cheat::CurrentPresetIndex = Cheat::CurrentPresetIndex - 1 < 0 ? (Cheat::PresetsMap.size()-1) : Cheat::CurrentPresetIndex - 1;

			if (ImGui::IsKeyPressed(VK_DOWN))
				Cheat::CurrentPresetIndex = Cheat::CurrentPresetIndex + 1 > (Cheat::PresetsMap.size() - 1) ? 0 : Cheat::CurrentPresetIndex + 1;
		}
	}

	bool IsCurrentlyBindingKey()
	{
		if (cfg_ShowMenuKeyState
			|| cfg_LootVacuumKeyState
			|| cfg_AimbotHoldKeyState
			|| cfg_AimbotToggleKeyState
			|| cfg_TimescaleHoldKeyState
			|| cfg_TimescaleToggleKeyState
			|| cfg_MissionAutoRestartKeyState
			|| cfg_MissionTeleportKeyState
			|| cfg_SpawnLootKeyState
			|| cfg_SpawnVaultRewardKeyState)
			return true;
		return false;
	}

	void RenderMenu()
	{
		if (ShowMenu)
		{
			if (ImGui::Begin("TFD Helper - DescendantInternal", &ShowMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (ImGui::BeginTabBar("Tabs"))
				{
					if (ImGui::BeginTabItem("Actors"))
					{
						if (ImGui::BeginTable("Settings", 2, ImGuiTableFlags_SizingFixedSame))
						{
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enable Actor ESP: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EAESP", &CFG::cfg_Actors_EnableESP);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Player Boxes: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EPBESP", &CFG::cfg_Actors_DrawPlayerBoxes);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Player Circles: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EPCESP", &CFG::cfg_Actors_DrawPlayerCircles);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Player Names: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EPNESP", &CFG::cfg_Actors_DrawPlayerNames);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Player Lines: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EPLESP", &CFG::cfg_Actors_DrawPlayerLines);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enemy Boxes: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EEBESP", &CFG::cfg_Actors_DrawEnemyBoxes);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enemy Circles: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EECESP", &CFG::cfg_Actors_DrawEnemyCircles);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enemy Names: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EENESP", &CFG::cfg_Actors_DrawEnemyNames);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enemy Lines: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EELESP", &CFG::cfg_Actors_DrawEnemyLines);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enable Modify Grapples: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EMG", &CFG::cfg_Aim_EnableModifyGrapple);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Grapple Range: ");
							ImGui::TableNextColumn();
							ImGui::SliderFloat("##GRM", &CFG::cfg_Aim_GrappleRange, 1000.0f, 20000.0f); 
							ImGui::EndTable();
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Loot"))
					{
						if (ImGui::CollapsingHeader("ESP", NULL, ImGuiTreeNodeFlags_DefaultOpen))
						{
							if (ImGui::BeginTable("ESPSettings", 2, ImGuiTableFlags_SizingFixedSame))
							{
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Enable Loot ESP: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EIESP", &CFG::cfg_Loot_EnableItemESP);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Item Boxes: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EIBESP", &CFG::cfg_Loot_DrawItemBoxes);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Item Circles: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EICESP", &CFG::cfg_Loot_DrawItemCircles);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Item Names: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EINESP", &CFG::cfg_Loot_DrawItemNames);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								/*ImGui::Text("Enable Item Lines: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EILESP", &CFG::cfg_Loot_DrawItemLines);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();*/

								ImGui::Text("Vaults: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EVESP", &CFG::cfg_Loot_DrawVaults);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Resources: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##ERESP", &CFG::cfg_Loot_DrawSupplyResources);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Void Vessels: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EVVESP", &CFG::cfg_Loot_DrawVoidVesselBox);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Loot Rarity Filter: ");
								ImGui::TableNextColumn();
								ImGui::SliderInt("##IRF", &CFG::cfg_Loot_ItemFilterLevel, 1, 3);
								ImGui::EndTable();
							}
						}
						if (ImGui::CollapsingHeader("Features", NULL))
						{
							if (ImGui::BeginTable("FeatureSettings", 2, ImGuiTableFlags_SizingFixedSame))
							{
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Enable Item Vacuum: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EIV", &CFG::cfg_Loot_EnableLootVacuum);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Vacuum Range: ");
								ImGui::TableNextColumn();
								ImGui::SliderFloat("##IVR", &CFG::cfg_Loot_LootVacuumRange, 151.0f, 80000.0f);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Vacuum Hotkey:");
								ImGui::TableNextColumn();
								ImGui::Hotkey("##IVKEY", &CFG::cfg_Loot_LootVacuumKey, cfg_LootVacuumKeyState, ImVec2(180, 24));
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Spawn Generic Loot Drops Hotkey:");
								ImGui::TableNextColumn();
								ImGui::Hotkey("##ILDKEY", &CFG::cfg_Loot_SpawnLootKey, cfg_SpawnLootKeyState, ImVec2(180, 24));
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Spawn Current Vault Rewards Hotkey:");
								ImGui::TableNextColumn();
								ImGui::Hotkey("##IVRKEY", &CFG::cfg_Loot_SpawnVaultRewardKey, cfg_SpawnVaultRewardKeyState, ImVec2(180, 24));
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Loot Container Range Check: ");
								ImGui::TableNextColumn();
								ImGui::SliderFloat("##LCRC", &CFG::cfg_Loot_ContainerDropRange, 250.0f, 2000.0f);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Enable Multiply Drops: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EMD", &CFG::cfg_Loot_MultiplyDrops); 
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Multiply Drop Count: ");
								ImGui::TableNextColumn();
								ImGui::SliderInt("##MDC", &CFG::cfg_Loot_SpawnCount, 1, 100);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Research Quantity:");
								ImGui::TableNextColumn();
								ImGui::SliderInt("##RQSL", &CFG::cfg_ResearchQuantity, 1, 2000);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Adjust Quantity:");
								ImGui::TableNextColumn();
								if (ImGui::Button("-1", ImVec2(30, 30)))
									CFG::cfg_ResearchQuantity = (CFG::cfg_ResearchQuantity > 2) ? CFG::cfg_ResearchQuantity - 1 : 1;

								ImGui::SameLine();
								if (ImGui::Button("+1", ImVec2(30, 30)))
									CFG::cfg_ResearchQuantity = (CFG::cfg_ResearchQuantity < 2000) ? CFG::cfg_ResearchQuantity + 1 : 2000;

								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Start Research:");
								ImGui::TableNextColumn();
								if (ImGui::Button("Start Research", ImVec2(120, 30)))
									Cheat::ResearchBookmarkedItems();
								ImGui::EndTable();
							}
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Aimbot"))
					{
						if (ImGui::CollapsingHeader("Aimbot", NULL, ImGuiTreeNodeFlags_DefaultOpen))
						{
							if (ImGui::BeginTable("AimSettings", 2, ImGuiTableFlags_SizingFixedSame))
							{
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Enable Aimbot: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EA", &CFG::cfg_Aim_EnableAimbot);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Aimbot Hold Mode: ");
								ImGui::TableNextColumn();
								if (ImGui::Checkbox("##EAH", &CFG::cfg_Aim_EnableAimbotHold))
								{
									if (CFG::cfg_Aim_EnableAimbotHold)
										CFG::cfg_Aim_EnableAimbotToggle = false;
								}
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Aimbot Hold Key: ");
								ImGui::TableNextColumn();
								ImGui::Hotkey("##AHKEY", &CFG::cfg_Aim_AimbotHoldKey, cfg_AimbotHoldKeyState, ImVec2(180, 24));
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Aimbot Toggle Mode: ");
								ImGui::TableNextColumn();
								if (ImGui::Checkbox("##EAT", &CFG::cfg_Aim_EnableAimbotToggle))
								{
									if (CFG::cfg_Aim_EnableAimbotToggle)
										CFG::cfg_Aim_EnableAimbotHold = false;
								}
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Toggle Key: ");
								ImGui::TableNextColumn();
								ImGui::Hotkey("##ATKEY", &CFG::cfg_Aim_AimbotToggleKey, cfg_AimbotToggleKeyState, ImVec2(180, 24));
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Enable Controller Support: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EACS", &CFG::cfg_Aim_AimbotUseController);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Max FOV: ");
								ImGui::TableNextColumn();
								ImGui::SliderFloat("##ASD", &CFG::cfg_Aim_AimbotFOV, 1.0f, 1000.0f);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Aim Speed: ");
								ImGui::TableNextColumn();
								ImGui::SliderFloat("##ASPD", &CFG::cfg_Aim_AimbotSpeed, 1.0f, 100.0f);
								ImGui::EndTable();
							}
						}
						if (ImGui::CollapsingHeader("Features", NULL))
						{
							if (ImGui::BeginTable("AimFeatures", 2, ImGuiTableFlags_SizingFixedSame))
							{
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Enable No Recoil and No Spread: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EANS", &CFG::cfg_Aim_NoRecoilAndSpread);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Enable Auto Reload: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EAAR", &CFG::cfg_Aim_NoReload);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Enable Rapid Fire: ");
								ImGui::TableNextColumn();
								ImGui::Checkbox("##EARF", &CFG::cfg_Aim_RapidFire);
								ImGui::TableNextRow();
								ImGui::TableNextColumn();
								ImGui::Text("Rapid Fire Rate: ");
								ImGui::TableNextColumn();
								ImGui::SliderFloat("##RFR", &CFG::cfg_Aim_FireRate, 1.0f, 100.0f);
								ImGui::EndTable();
							}
						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Missions"))
					{
						if (ImGui::BeginTable("Settings", 2, ImGuiTableFlags_SizingFixedSame))
						{
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enable Mission ESP: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EMESP", &CFG::cfg_Mission_EnableMissionESP);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enable Mission Auto-Restart: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EMEAR", &CFG::cfg_Mission_EnableMissionAutoRestart);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Auto-Restart Delay (Seconds): ");
							ImGui::TableNextColumn();
							ImGui::SliderFloat("##MARD", &CFG::cfg_Mission_MissionAutoRestartDelay, 0.25, 30.0f);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Auto-Restart Toggle Key: ");
							ImGui::TableNextColumn();
							ImGui::Hotkey("##MARTK", &CFG::cfg_Mission_MissionAutoRestartKey, cfg_MissionAutoRestartKeyState, ImVec2(180, 24));
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Mission Teleport Key");
							ImGui::TableNextColumn();
							ImGui::Hotkey("##MTK", &CFG::cfg_Mission_MissionTeleportKey, cfg_MissionTeleportKeyState, ImVec2(180, 24));
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enable Instant Infiltration: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EAII", &CFG::cfg_Mission_EnableInstantInfiltration);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Instant Infiltration Key");
							ImGui::TableNextColumn();
							ImGui::Hotkey("##IIK", &CFG::cfg_Mission_InstantInfiltrationKey, cfg_InstantInfiltrationKeyState, ImVec2(180, 24));

							ImGui::EndTable();
						}
						if (Cheat::Missions != nullptr)
						{
							if (ImGui::CollapsingHeader("Available Missions"))
							{
								if (Cheat::Missions->AvailableMissions.IsValid())
								{
									char countbuffer[16];
									sprintf_s(countbuffer, "Missions: %i", Cheat::Missions->AvailableMissions.Num());
									ImGui::Text(countbuffer);
									for (int i = 0; i < Cheat::Missions->AvailableMissions.Num(); i++)
									{
										//TFD::AM1MissionActor* Actor = Cheat::Missions->AvailableMissions[i];
										if (Cheat::Missions->AvailableMissions[i])
										{
											char bufferlol[128];
											auto enumtostr = magic_enum::enum_name(Cheat::Missions->AvailableMissions[i]->MissionData->MissionSubType);
											sprintf_s(bufferlol, "Mission %s, Type: %s", Cheat::Missions->AvailableMissions[i]->MissionData->MissionDataRowName.ToString().c_str(), enumtostr.data());
											ImGui::Text(bufferlol);
											ImGui::PushID(i);
											if (ImGui::Button("Start"))
											{
												//Cheat::Missions->TryStartMission(Actor->MissionData->MissionDataRowName);
												Cheat::Missions->ServerStartMission(Cheat::Missions->AvailableMissions[i]);
											}
											ImGui::PopID();
											ImGui::SameLine();
											ImGui::PushID(i + Cheat::Missions->AvailableMissions.Num());
											if (ImGui::Button("Teleport To Mission"))
											{
												Cheat::LocalPlayerCharacter->TeleportHandler->ServerMoveToTeleportToLocation(Cheat::Missions->AvailableMissions[i]->K2_GetActorLocation(), Cheat::Missions->AvailableMissions[i]->K2_GetActorRotation());
											}
											ImGui::PopID();
										}
									}
								}
							}
							if (ImGui::CollapsingHeader("Active Missions"))
							{
								if (Cheat::Missions->ActivatedMissions.IsValid())
								{
									//static int TaskIndex = 0;
									char countbuffer[16];
									sprintf_s(countbuffer, "Missions: %i", Cheat::Missions->ActivatedMissions.Num());
									ImGui::Text(countbuffer);
									for (int i = 0; i < Cheat::Missions->ActivatedMissions.Num(); i++)
									{
										TFD::AM1MissionActor* Actor = Cheat::Missions->ActivatedMissions[i];
										if (Actor)
										{
											char bufferlol[128];
											sprintf_s(bufferlol, "Mission %s", Actor->MissionData->MissionDataRowName.ToString().c_str());
											ImGui::Text(bufferlol);
											if (ImGui::Button("Teleport To Mission"))
											{
												Cheat::LocalPlayerCharacter->TeleportHandler->ServerMoveToTeleportToLocation(Actor->K2_GetActorLocation(), Actor->K2_GetActorRotation());
											}
										}
									}
								}
							}
							if (ImGui::CollapsingHeader("Last Active Missions"))
							{
								if (Cheat::Missions->LastActivatedMissions.IsValid())
								{
									char countbuffer[16];
									sprintf_s(countbuffer, "Missions: %i", Cheat::Missions->LastActivatedMissions.Num());
									ImGui::Text(countbuffer);
									for (int i = 0; i < Cheat::Missions->LastActivatedMissions.Num(); i++)
									{
										TFD::AM1MissionActor* Actor = Cheat::Missions->LastActivatedMissions[i];
										if (Actor)
										{
											char bufferlol[128];
											auto enumtostr = magic_enum::enum_name(Cheat::Missions->AvailableMissions[i]->MissionData->MissionSubType);
											sprintf_s(bufferlol, "Mission %s, Type: %s", Cheat::Missions->AvailableMissions[i]->MissionData->MissionDataRowName.ToString().c_str(), enumtostr.data());
											ImGui::Text(bufferlol);
											if (ImGui::Button("Restart"))
											{
												Cheat::Missions->ServerRestartLastPlayedMission();
											}
										}
									}
								}
							}

						}
						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Extra"))
					{
						if (ImGui::BeginTable("Settings", 2, ImGuiTableFlags_SizingFixedSame))
						{

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Timescale: ");
							ImGui::TableNextColumn();
							ImGui::SliderFloat("##ASPD", &CFG::cfg_Extra_TimeScale, 1.0f, 10.0f);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Timescale Toggle Key: ");
							ImGui::TableNextColumn();
							ImGui::Hotkey("##MTTK", &CFG::cfg_Extra_TimeScaleHoldKey, cfg_TimescaleHoldKeyState, ImVec2(180, 24));
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Timescale Hold Key: ");
							ImGui::TableNextColumn();
							ImGui::Hotkey("##MTHK", &CFG::cfg_Extra_TimeScaleKey, cfg_TimescaleToggleKeyState, ImVec2(180, 24));
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enable UE Console: ");
							ImGui::TableNextColumn();
							if (ImGui::Checkbox("##EEC", &CFG::cfg_Extra_EnableUEConsole))
							{
								if (CFG::cfg_Extra_EnableUEConsole)
									Cheat::CreateConsole();
							}
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enable Preset Overlay: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##EPROVL", &CFG::cfg_Hotswap_EnableOverlay);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Preset Swap Key: ");
							ImGui::TableNextColumn();
							ImGui::Hotkey("##PHTKY", &CFG::cfg_Hotswap_PresetSelectKey, cfg_HotswapKeyState, ImVec2(180, 24));
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Refresh Presets: ");
							ImGui::TableNextColumn();
							if (ImGui::Button("Refresh"))
							{
								Cheat::PresetsMap.clear();
								Cheat::PresetRefresh();
							}
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("ESP Shape Size: ");
							ImGui::TableNextColumn();
							ImGui::SliderFloat("##ESPSS", &CFG::cfg_ESP_ShapeSize, 5.0f, 80.0f);
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Adjust Size: ");
							ImGui::TableNextColumn();
							if (ImGui::Button("-1", ImVec2(30, 30)))
								CFG::cfg_ESP_ShapeSize = (CFG::cfg_ESP_ShapeSize > 5.0f) ? CFG::cfg_ESP_ShapeSize - 1.0f : 5.0f;
							ImGui::SameLine();
							if (ImGui::Button("+1", ImVec2(30, 30)))
								CFG::cfg_ESP_ShapeSize = (CFG::cfg_ESP_ShapeSize < 80.0f) ? CFG::cfg_ESP_ShapeSize + 1.0f : 80.0f;
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("ESP Shape Distance Scale: ");
							ImGui::TableNextColumn();
							ImGui::SliderFloat("##ESPSDS", &CFG::cfg_ESP_ShapeDistanceScale, 1.0f, 800.0f);
							ImGui::EndTable();
						}
						ImGui::EndTabItem();
					}


					/*if (ImGui::BeginTabItem("Debug"))
					{
						if (ImGui::BeginTable("Settings", 2, ImGuiTableFlags_SizingFixedSame))
						{

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Enable Cheat Console: ");
							ImGui::TableNextColumn();
							ImGui::Checkbox("##ECCONSOLE", &CFG::cfg_Debug_ShowConsole);
							ImGui::EndTable();
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("fuck me up: ");
							ImGui::TableNextColumn();
							ImGui::SliderFloat("##ASGFD", &Cheat::BoundsScale, 0.01f, 5.0f);
							ImGui::EndTable();
						}

						LocalPlayerController->CheatClass = TFD::UM1Cheat::StaticClass();
						LocalPlayerController->CheatManager = static_cast<TFD::UM1Cheat*>(TFD::UGameplayStatics::SpawnObject(LocalPlayerController->CheatClass, LocalPlayerController));
						if (Cheat::LocalPlayerController->CheatManager)
						{
							TFD::native_InitCheatManager(LocalPlayerController->CheatManager);
							static_cast<TFD::UM1Cheat*>(LocalPlayerController->CheatManager)->CheatUI = static_cast<TFD::UM1UICheat*>(TFD::UGameplayStatics::SpawnObject(TFD::UM1UICheat::StaticClass(), LocalPlayerController->CheatManager));
						}
						if (ImGui::Button("Speed"))
						{
							for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
							{
								SDK::UObject* Obj = SDK::UObject::GObjects->GetByIndex(i);
								if (!Obj)
									continue;
								if (Obj->IsA(TFD::UM1SpeedHackDetectorSubSystem::StaticClass()))
								{
									console.AddLog("SpeedDetector: %p", (void*)Obj);
									//std::cout << "Speed " << std::hex << static_cast<void*>(Obj) << std::dec << std::endl;
								}
							}

						}
						ImGui::EndTabItem();
					}*/

					ImGui::EndTabBar();
				}
				ImGui::End();
			}
		}
	}

	bool IsKeyBinding()
	{
		if (cfg_ShowMenuKeyState
			|| cfg_LootVacuumKeyState
			|| cfg_AimbotHoldKeyState
			|| cfg_AimbotToggleKeyState
			|| cfg_TimescaleHoldKeyState
			|| cfg_TimescaleToggleKeyState
			|| cfg_MissionAutoRestartKeyState
			|| cfg_MissionTeleportKeyState
			|| cfg_SpawnLootKeyState
			|| cfg_SpawnVaultRewardKeyState
			|| cfg_HotswapKeyState)
			return true;
		return false;
	}

	void Init()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.Alpha = 1.0f;
		style.WindowPadding = ImVec2(10.0f, 8.300000190734863f);
		style.WindowRounding = 2.0f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.ChildRounding = 4.5f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 2.700000047683716f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(10.0f, 2.0f);
		style.FrameRounding = 0.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(8.0f, 7.0f);
		style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 18.5f;
		style.ScrollbarRounding = 4.199999809265137f;
		style.GrabMinSize = 10.0f;
		style.GrabRounding = 3.200000047683716f;
		style.TabRounding = 3.5f;
		style.TabBorderSize = 1.0f;
		style.TabMinWidthForCloseButton = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.7811158895492554f, 0.7811080813407898f, 0.7811080813407898f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 0.6351931095123291f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.8f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.4f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.7854077219963074f, 0.785399854183197f, 0.785399854183197f, 0.5400000214576721f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2117647081613541f, 0.2549019753932953f, 0.3019607961177826f, 0.4000000059604645f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.04313725605607033f, 0.0470588244497776f, 0.0470588244497776f, 0.6700000166893005f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0784313753247261f, 0.08235294371843338f, 0.09019608050584793f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7176470756530762f, 0.7843137383460999f, 0.843137264251709f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1974248588085175f, 0.1974248886108398f, 0.1974229067564011f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.2901960909366608f, 0.3176470696926117f, 0.3529411852359772f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.5490196138620377f, 0.5607843190431595f, 0.5764705926179886f, 0.8f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1372549086809158f, 0.1450980454683304f, 0.1568627506494522f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.09019608050584793f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.196078434586525f, 0.2156862765550613f, 0.239215686917305f, 0.3100000023841858f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1647058874368668f, 0.1764705926179886f, 0.1921568661928177f, 0.800000011920929f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.07450980693101883f, 0.08235294371843338f, 0.09019608050584793f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.239215686917305f, 0.3254902064800262f, 0.4235294163227081f, 0.7799999713897705f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.2745098173618317f, 0.3803921639919281f, 0.4980392158031464f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2901960909366608f, 0.3294117748737335f, 0.3764705955982208f, 0.2000000029802322f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.239215686917305f, 0.2980392277240753f, 0.3686274588108063f, 0.6700000166893005f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.1647058874368668f, 0.1764705926179886f, 0.1882352977991104f, 0.949999988079071f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.1176470592617989f, 0.125490203499794f, 0.1333333402872086f, 0.8619999885559082f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3294117748737335f, 0.407843142747879f, 0.501960813999176f, 0.800000011920929f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.2431372553110123f, 0.2470588237047195f, 0.2549019753932953f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);


	}

}

void ImGui::Hotkey(const char* label, int* k, bool& StateHolder, const ImVec2& size_arg)
{
	PushID(label);
	static bool ignoreFirst = false;

	if (!StateHolder)
	{
		if (Button(Menu::KeyNames[*k], size_arg))
		{
			StateHolder = true;
			ignoreFirst = true;
		}
	}
	else
	{
		Button("ESC To Cancel");
		for (auto& Key : Menu::KeyCodes)
		{
			if (IsKeyPressed(Key))
			{
				if (Key == VK_ESCAPE || Key == *k)
					StateHolder = false;
				else if (!ignoreFirst)
				{
					*k = Key;
					StateHolder = false;
				}
				else
					ignoreFirst = false;

				break;
			}
		}
	}
	PopID();
}