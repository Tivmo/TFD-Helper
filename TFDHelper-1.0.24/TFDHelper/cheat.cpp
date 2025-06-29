#include "cheat.h"
#include "magic_enum/magic_enum.hpp"



//#define IS_DEBUG_VERSION




namespace Cheat
{
#define IsKeyPressed(key) ([](int k){ SHORT s = GetAsyncKeyState(k); return (s & 0x8001) == 0x8001; })(key)
#define IsKeyHeld(key) (GetAsyncKeyState(key) & 0x8000)

	__int64 (*oPostRender)(void* ViewportClient, void* DebugCanvas) = nullptr;

	bool UpdateCache = false;
	float BoundsScale = 1.0f;

	CONTROLLER_STATE g_Controllers[4];

	SDK::FVector2D ScreenMiddle = { 0, 0 };

	CSimpleIniA ini;

	TFD::UWorld* GWorld = nullptr;
	TFD::AM1PlayerControllerInGame* LocalPlayerController = nullptr;
	TFD::AM1Player* LocalPlayerCharacter = nullptr;
	TFD::UCanvas* Canvas = nullptr;
	TFD::UM1MissionControlComponent* Missions = nullptr;
	TFD::AActor* Aimbot_Target = nullptr;
	int Aimbot_BoneIndex = 0;
	TFD::UM1WeaponSlotControlComponent* WeaponSlot = nullptr;

	SDK::FLinearColor ColorWhite = { 1.0f, 1.0f, 1.0f, 1.0f };
	SDK::FLinearColor ColorRed = { 1.0f, 0, 0, 1.0f };
	SDK::FLinearColor ColorDarkRed = { 0.5f, 0.0f, 0.0f, 0.8f };
	SDK::FLinearColor ColorGreen = { 0, 1.0f, 0, 1.0f };
	SDK::FLinearColor ColorMana = { 0.184f, 0.702f, 1.0f, 1.0f };
	SDK::FLinearColor ColorCommon = { 0, 0.922f, 0.29f, 1.0f };
	SDK::FLinearColor ColorUncommon = { 0.2f, 0.29f, 1.0f, 1.0f };
	SDK::FLinearColor ColorRare = { 0.761f, 0.063f, 0.961, 1.0f };
	SDK::FLinearColor ColorUltimate = { 1.0f, 0.247f, 0.247f, 1.0f };
	SDK::FLinearColor ColorGold = { 1.0f, 0.882f, 0, 1.0f };

	std::unordered_map<int, std::vector<int>> IDBoneMap = { };
	std::unordered_map<int, std::string> IDNameMap = { };
	std::unordered_map<int, std::string> PresetsMap = { };
	int CurrentPresetIndex = 0;

	TFD::FVector RotatorToVector(const TFD::FRotator& R)
	{
		float CP = std::cos(R.Pitch * 3.14159265f / 180.f);
		float SP = std::sin(R.Pitch * 3.14159265f / 180.f);
		float CY = std::cos(R.Yaw * 3.14159265f / 180.f);
		float SY = std::sin(R.Yaw * 3.14159265f / 180.f);

		return TFD::FVector{ CP * CY, CP * SY, SP };
	}

	bool TrySpawnGenericLoot = false;
	bool TrySpawnVaultLoot = false;

	__int64 PostRender(void* self, void* canvas)
	{
		//std::cout << "PostRender called" << std::endl;
		//return oPostRender(self, canvas);

		static bool Init = false;
		if (!Init)
		{
			//std::cout << "PostRender called once." << std::endl;
			Canvas = static_cast<TFD::UCanvas*>(canvas);
			LoadCacheData();
			//ToggleMemFeature(0);
			//ToggleMemFeature(1);
			//ToggleMemFeature(2);
			Init = true;
		}

		if (!IsValidUWorld())
		{
			Render::R_End();
			return oPostRender(self, canvas);
		}

		if (!IsCheatReady())
		{
			Render::R_End();
			return oPostRender(self, canvas);
		}

		if ((int)(static_cast<TFD::UM1GameInstance*>(GWorld->OwningGameInstance)->ConnectionState) != 10)
		{
			Render::R_End();
			return oPostRender(self, canvas);
		}

		ScreenMiddle.X = (Canvas->SizeX / 2.0f) / (Canvas->SizeX / Canvas->ClipX);
		ScreenMiddle.Y = (Canvas->SizeY / 2.0f) / (Canvas->SizeY / Canvas->ClipY);
		static bool PresetFirstCheck = true;
		if (PresetFirstCheck && PresetsMap.empty())
		{
			PresetFirstCheck = false;
			//PresetRefresh();
		}
		UpdateControllerState();

		Menu::HandleKeybinds();

		if (UpdateCache)
		{
			SaveCacheData();
			UpdateCache = false;
		}

		if (CFG::cfg_Actors_EnableESP)
			PlayerEnemyESP();

		if (CFG::cfg_Mission_EnableMissionESP)
			MissionESPTeleport();

		if (CFG::cfg_Mission_EnableMissionAutoRestart)
			AutoRestartMission();


		if (CFG::cfg_Loot_EnableItemESP || CFG::cfg_Loot_EnableLootVacuum || TrySpawnGenericLoot || TrySpawnVaultLoot)
			Loot();

		if (CFG::cfg_Aim_EnableAimbot)
			Aimbot();

		if (CFG::cfg_Aim_EnableModifyGrapple)
			ModifyGrapple();

		/*if (CFG::cfg_Aim_NoReload)
			InstantReload();

		if (CFG::cfg_Aim_NoRecoilAndSpread)
			NoRecoilAndSpread();

		if (CFG::cfg_Aim_RapidFire)
			RapidFire();*/

		if (CFG::cfg_Aim_NoRecoilAndSpread || CFG::cfg_Aim_NoReload || CFG::cfg_Aim_RapidFire)
			WeaponModifications();

		if (PresetsMap.empty())
			PresetRefresh();

		if (CFG::cfg_Hotswap_EnableOverlay)
			PresetOverlay();

		if (CFG::cfg_Mission_EnableInstantInfiltration)
			InstantInfiltration();

		Render::R_End();
		return oPostRender(self, canvas);
	}

	bool IsValidUWorld()
	{
		if (GWorld == nullptr)
		{
#ifdef IS_DEBUG_VERSION
			std::cout << "[Cheat] World is null.\n";
#endif
			GWorld = *reinterpret_cast<TFD::UWorld**>(TFD::BASE + SDK::Offsets::GWorld);
			if (GWorld && GWorld->IsA(TFD::UWorld::StaticClass()))
			{
#ifdef IS_DEBUG_VERSION
				std::cout << "[Cheat] Found World\n";
#endif
				return true;
			}
			else
			{
#ifdef IS_DEBUG_VERSION
				std::cout << "[Cheat] World is null and GWorld not pointing to valid UWorld.\n";
#endif
				GWorld = nullptr;
				return false;
			}
		}
		else
		{
			TFD::UWorld* CheckWorld = *reinterpret_cast<TFD::UWorld**>(TFD::BASE + SDK::Offsets::GWorld);
			if (!CheckWorld)
			{
#ifdef IS_DEBUG_VERSION
				std::cout << "[Cheat] World was not null but GWorld not currently pointing to valid UWorld.\n";
#endif
				GWorld = nullptr;
				return false;
			}
			if (CheckWorld->IsA(TFD::UWorld::StaticClass()))
			{
				if (GWorld != CheckWorld)
				{
#ifdef IS_DEBUG_VERSION
					std::cout << "[Cheat] World is not null and GWorld is pointing to different UWorld.\n";
#endif
					GWorld = CheckWorld;
				}
				return true;
			}
		}

		return false;
	}

	bool IsCheatReady()
	{
		std::string Name = GWorld->Name.ToString();
		if (Name != "" && Name != "None")
		{
			if (Name != "Lobby_P" && Name != "Level_Transition")
			{
				if (GWorld->OwningGameInstance && GWorld->OwningGameInstance->IsA(TFD::UM1GameInstance::StaticClass()))
				{
					if (GWorld->OwningGameInstance->LocalPlayers && GWorld->OwningGameInstance->LocalPlayers[0] && GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController)
					{
						if (GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController->IsA(TFD::AM1PlayerControllerInGame::StaticClass()))
						{
							TFD::AM1PlayerControllerInGame* PC = static_cast<TFD::AM1PlayerControllerInGame*>(GWorld->OwningGameInstance->LocalPlayers[0]->PlayerController);
							if (PC->Character && PC->Character->IsA(TFD::AM1Player::StaticClass()) && PC->ActorManager_Subsystem && PC->ActorManager_Subsystem->IsA(TFD::UM1ActorManagerSubsystem::StaticClass()))
							{
								LocalPlayerController = PC;
								LocalPlayerCharacter = static_cast<TFD::AM1Player*>(PC->Character);
								if (PC->PlayerState && PC->PlayerState->IsA(TFD::AM1PlayerState::StaticClass()))
									Missions = static_cast<TFD::AM1PlayerState*>(PC->PlayerState)->MissionControlComponent;
								else
									Missions = nullptr;
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}



	bool WorldToScreen(const SDK::FVector& worldLoc, SDK::FVector2D* screenPos)
	{
		bool inView = TFD::UGameplayStatics::ProjectWorldToScreen(LocalPlayerController, worldLoc, screenPos, false);
		if (inView && screenPos->X > 0 && screenPos->X < Canvas->SizeX && screenPos->Y > 0 && screenPos->Y < Canvas->SizeY)
			return true;
		return false;
	}

	void PlayerEnemyESP()
	{
		TFD::UM1ActorManagerSubsystem* Actors = LocalPlayerController->ActorManager_Subsystem;
		if (Actors->Characters.IsValid() && Actors->Characters.Num() > 0)
		{
			int StartNumber = Actors->Characters.Num();
			for (int i = 0; i < Actors->Characters.Num(); i++)
			{
				if (!GWorld || !LocalPlayerController || !LocalPlayerController->ActorManager_Subsystem || !Actors || !Actors->Characters)
					return;
				if (Actors->Characters.Num() != StartNumber)
					return;
				if (!Actors->Characters.IsValidIndex(i))
					continue;
				TFD::AM1Character* p = Actors->Characters[i];
				if (p)
				{
					if (p == LocalPlayerCharacter)
						continue;
					if (p->IsDead())
						continue;
					if (p->IsA(TFD::AM1Player::StaticClass()))
					{
						TFD::AM1Player* player = static_cast<TFD::AM1Player*>(p);
						SDK::FVector2D ScreenPos = { -1, -1 };
						if (WorldToScreen(p->K2_GetActorLocation(), &ScreenPos))
						{
							if (CFG::cfg_Actors_DrawPlayerNames)
							{
								if (player && player->PlayerName)
								{
									std::string playerNameStr = player->PlayerName.ToString();
									if (!playerNameStr.empty())
									{
										// Debug logging
										//std::cout << "before R_DrawText" << std::endl;
										Render::R_DrawText(ScreenPos.X, ScreenPos.Y, player->PlayerName, &ColorGreen);
										//std::cout << "after R_DrawText" << std::endl;
									}
								}
							}
							if (CFG::cfg_Actors_DrawPlayerBoxes)
							{
								if (player->Mesh && player->Mesh->SkeletalMesh)
								{
									float ODistance = player->GetDistanceTo(LocalPlayerController->Pawn) / CFG::cfg_ESP_ShapeDistanceScale;
									if (ODistance > 0)
									{
										float BoxSize = CFG::cfg_ESP_ShapeSize / ODistance;
										BoxSize = std::clamp(BoxSize, 8.0f, 80.0f);

										Render::R_DrawRect(
											ScreenPos.X - BoxSize / 1.5f,
											ScreenPos.Y - BoxSize,
											ScreenPos.X + BoxSize / 1.5f,
											ScreenPos.Y + BoxSize,
											&ColorGreen);

									}
								}
							}
							if (CFG::cfg_Actors_DrawPlayerCircles)
							{
								if (player->Mesh && player->Mesh->SkeletalMesh)
								{
									float ODistance = player->GetDistanceTo(LocalPlayerController->Pawn) / CFG::cfg_ESP_ShapeDistanceScale;
									if (ODistance > 0)
									{
										float Radius = CFG::cfg_ESP_ShapeSize / ODistance;
										Radius = std::clamp(Radius, 4.0f, 60.0f);
										Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, Radius, &ColorGreen);
									}
								}
							}
							if (CFG::cfg_Actors_DrawPlayerLines)
							{
								if (player->Mesh && player->Mesh->SkeletalMesh)
								{
									SDK::FVector2D LocalScreenPos;
									if (WorldToScreen(LocalPlayerCharacter->K2_GetActorLocation(), &LocalScreenPos))
									{
										Render::R_DrawLine(LocalScreenPos.X, LocalScreenPos.Y, ScreenPos.X, ScreenPos.Y, &ColorGreen);
									}

								}
							}
						}

					}
					else if (p->IsA(TFD::AM1Monster::StaticClass()) || p->CharacterAttribute->IsA(TFD::UM1MonsterAttribute::StaticClass()))
					{
						//TFD::AM1Monster* monster = static_cast<TFD::AM1Monster*>(p);
						SDK::FVector2D ScreenPos = { -1, -1 };
						if (WorldToScreen(p->K2_GetActorLocation(), &ScreenPos))
						{
							/*SDK::FLinearColor color = ColorDarkRed;
							if (LocalPlayerController->LineOfSightTo(p, SDK::FVector{ 0, 0, 0 }, false))
								color = ColorRed;*/

							if (CFG::cfg_Actors_DrawEnemyNames)
							{
								if (!IDNameMap.contains(p->CharacterId.ID))
								{
									if (!p->InfoWidgetComponent || !p->InfoWidgetComponent->ActorWidget.Get())
										continue;
									TFD::UM1UIActorWidget* Base = p->InfoWidgetComponent->ActorWidget.Get();
									if (Base)
									{
										if (Base->IsA(TFD::UM1UICharacterInfoBase::StaticClass()))
										{
											TFD::UM1UICharacterInfoBase* Info = static_cast<TFD::UM1UICharacterInfoBase*>(Base);
											if (Info)
											{
												if (Info->TB_Name && Info->TB_Name->Text.TextData && Info->TB_Name->Text.TextData->TextSource)
												{
													//std::cout << std::hex << Info->TB_Name->Text.TextData->TextSource << std::dec << std::endl;
													std::string name = Info->TB_Name->Text.TextData->TextSource.ToString();
													int id = p->CharacterId.ID;
													IDNameMap.insert({ id, name });
													UpdateCache = true;
												}
											}
										}

									}
								}
								else
								{
									Render::R_DrawText(ScreenPos.X, ScreenPos.Y, IDNameMap[(int)p->CharacterId.ID], &ColorRed);
								}
							}
							if (CFG::cfg_Actors_DrawEnemyBoxes)
							{
								if (p->Mesh && p->Mesh->SkeletalMesh)
								{
									float ODistance = p->GetDistanceTo(LocalPlayerController->Pawn) / CFG::cfg_ESP_ShapeDistanceScale;
									if (ODistance > 0)
									{
										float BoxSize = CFG::cfg_ESP_ShapeSize / ODistance;
										BoxSize = std::clamp(BoxSize, 8.0f, 80.0f);

										Render::R_DrawRect(
											ScreenPos.X - BoxSize / 1.5f,
											ScreenPos.Y - BoxSize,
											ScreenPos.X + BoxSize / 1.5f,
											ScreenPos.Y + BoxSize,
											&ColorRed);
									}
								}
							}
							if (CFG::cfg_Actors_DrawEnemyCircles)
							{
								if (p->Mesh && p->Mesh->SkeletalMesh)
								{
									float ODistance = p->GetDistanceTo(LocalPlayerController->Pawn) / CFG::cfg_ESP_ShapeDistanceScale;
									if (ODistance > 0)
									{
										float Radius = CFG::cfg_ESP_ShapeSize / ODistance;
										Radius = std::clamp(Radius, 4.0f, 60.0f);
										Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, Radius, &ColorRed);
									}
								}
							}
							if (CFG::cfg_Actors_DrawEnemyLines)
							{
								if (p->Mesh && p->Mesh->SkeletalMesh)
								{
									SDK::FVector2D LocalScreenPos;
									if (WorldToScreen(LocalPlayerCharacter->K2_GetActorLocation(), &LocalScreenPos))
									{
										Render::R_DrawLine(LocalScreenPos.X, LocalScreenPos.Y, ScreenPos.X, ScreenPos.Y, &ColorRed);
									}

								}
							}
						}
					}
				}
			}
		}
	}

	void MultiplyDrops(TFD::AActor* Actor, int Amount, bool isVault)
	{
		if (!Actor || !LocalPlayerController || Amount <= 0)
			return;

		TFD::FVector ActorPos = Actor->K2_GetActorLocation();
		TFD::FVector PlayerPos = LocalPlayerCharacter->K2_GetActorLocation();
		float Distance = ActorPos.GetDistanceTo(PlayerPos);

		if (Distance > CFG::cfg_Loot_ContainerDropRange)
			return;

		for (int i = 0; i < Amount; ++i)
		{
			if (isVault)
			{
				auto* Vault = static_cast<TFD::AM1MiniGameActor*>(Actor);
				Vault->ServerDropItems(LocalPlayerController);
			}
			else
			{
				auto* Generic = static_cast<TFD::AM1FieldInteractableActor*>(Actor);
				LocalPlayerController->ServerRequestFieldObjectDropItems(Generic);
			}
		}
	}


	void Loot()
	{
		if (GWorld->Levels.IsValid())
		{
			static TFD::UM1LocalGameInstanceSubsystem* GISystem = nullptr;
			for (int i = 0; i < GWorld->Levels.Num(); ++i)
			{
				if (GWorld->Levels.IsValidIndex(i))
				{
					TFD::ULevel* Level = GWorld->Levels[i];
					if (!Level) continue;

					for (int j = 0; j < Level->Actors.Num(); ++j)
					{
						TFD::AActor* Actor = (TFD::AActor*)Level->Actors[j];
						if (!Actor)
							continue;

						if (TrySpawnVaultLoot)
						{
							if (Actor->IsA(TFD::AM1MiniGameActor::StaticClass()))
							{
								GISystem = TFD::UM1LocalGameInstanceSubsystem::GetSystem(GWorld);
								if (GISystem && GISystem->MiniGamePlayer && GISystem->MiniGamePlayer->CurrentMiniGame)
								{
									auto* Vault = static_cast<TFD::AM1MiniGameActor*>(Actor);
									if (Vault->MiniGameTid.ID == GISystem->MiniGamePlayer->CurrentMiniGame->MiniGameTemplateId.ID)
									{
										int Amount = CFG::cfg_Loot_MultiplyDrops ? CFG::cfg_Loot_SpawnCount : 1;
										MultiplyDrops(Vault, Amount, true);
									}
								}
								continue;
							}
						}

						if (CFG::cfg_Loot_DrawVaults)
						{
							if (Actor->IsA(TFD::AM1FieldInteractableActorMiniGame::StaticClass()))
							{
								SDK::FVector2D ScreenPosa = { -1, -1 };
								SDK::FVector WorldPositiona = Actor->K2_GetActorLocation();
								if (WorldToScreen(WorldPositiona, &ScreenPosa))
								{
									static std::string VaultText = "Coded Vault";
									Render::R_DrawText(ScreenPosa.X, ScreenPosa.Y, VaultText, &ColorGold, true);
								}
								continue;
							}
						}
						if (CFG::cfg_Loot_DrawSupplyResources)
						{
							if (Actor->IsA(TFD::AM1FieldInteractableActor_Hit::StaticClass()))
							{
								if (Actor->GetFullName().contains("VulgusBox"))
								{
									SDK::FVector2D ScreenPos = { -1, -1 };
									SDK::FVector WorldPosition = Actor->K2_GetActorLocation();
									if (WorldToScreen(WorldPosition, &ScreenPos))
									{
										static std::string RBText = "Resource Box";
										Render::R_DrawText(ScreenPos.X, ScreenPos.Y, RBText, &ColorUncommon, true);
									}

									TFD::FVector PlayerPosition = LocalPlayerCharacter->K2_GetActorLocation();
									float Distance = WorldPosition.GetDistanceTo(PlayerPosition);
									if (Distance <= CFG::cfg_Loot_ContainerDropRange && CFG::cfg_Loot_MultiplyDrops)
									{
										if (WorldToScreen(WorldPosition, &ScreenPos))
										{
											float ODistance = Distance;
											if (ODistance > 0)
											{
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 3.0f, &ColorWhite);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 5.0f, &ColorGreen);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 7.0f, &ColorMana);
											}
										}
									}

									if (TrySpawnGenericLoot)
									{
										int Amount = CFG::cfg_Loot_MultiplyDrops ? CFG::cfg_Loot_SpawnCount : 1;
										MultiplyDrops(Actor, Amount, false);
									}
									continue;
								}
							}
							else if (Actor->IsA(TFD::AM1FieldInteractableActor_Interaction::StaticClass()))
							{
								if (Actor->GetFullName().contains("MilitarySupplies"))
								{
									SDK::FVector2D ScreenPos = { -1, -1 };
									SDK::FVector WorldPosition = Actor->K2_GetActorLocation();
									if (WorldToScreen(WorldPosition, &ScreenPos))
									{
										static std::string SPText = "Supplies";
										Render::R_DrawText(ScreenPos.X, ScreenPos.Y, SPText, &ColorCommon, true);
									}

									TFD::FVector PlayerPosition = LocalPlayerCharacter->K2_GetActorLocation();
									float Distance = WorldPosition.GetDistanceTo(PlayerPosition);
									if (Distance <= CFG::cfg_Loot_ContainerDropRange && CFG::cfg_Loot_MultiplyDrops)
									{
										if (WorldToScreen(WorldPosition, &ScreenPos))
										{
											float ODistance = Distance;
											if (ODistance > 0)
											{
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 3.0f, &ColorWhite);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 5.0f, &ColorGreen);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 7.0f, &ColorMana);
											}
										}
									}

									if (TrySpawnGenericLoot)
									{
										int Amount = CFG::cfg_Loot_MultiplyDrops ? CFG::cfg_Loot_SpawnCount : 1;
										MultiplyDrops(Actor, Amount, false);
									}
									continue;
								}
							}
						}

						if (CFG::cfg_Loot_DrawVoidVesselBox)
						{
							if (Actor->IsA(TFD::AM1FieldInteractableActor_Interaction::StaticClass()))
							{
								std::string aName = Actor->GetFullName();
								SDK::FVector2D ScreenPos = { -1, -1 };
								SDK::FVector WorldPosition = Actor->K2_GetActorLocation();
								if (aName.contains("CompanionVault_TypeB_Item_C"))
								{
									if (WorldToScreen(WorldPosition, &ScreenPos))
									{
										static std::string SPText = "Experimental Box";
										Render::R_DrawText(ScreenPos.X, ScreenPos.Y, SPText, &ColorCommon, true);
									}

									TFD::FVector PlayerPosition = LocalPlayerCharacter->K2_GetActorLocation();
									float Distance = WorldPosition.GetDistanceTo(PlayerPosition);
									if (Distance <= CFG::cfg_Loot_ContainerDropRange && CFG::cfg_Loot_MultiplyDrops)
									{
										if (WorldToScreen(WorldPosition, &ScreenPos))
										{
											float ODistance = Distance;
											if (ODistance > 0)
											{
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 3.0f, &ColorWhite);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 5.0f, &ColorGreen);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 7.0f, &ColorMana);
											}
										}
									}

									if (TrySpawnGenericLoot)
									{
										int Amount = CFG::cfg_Loot_MultiplyDrops ? CFG::cfg_Loot_SpawnCount : 1;
										MultiplyDrops(Actor, Amount, false);
									}
									continue;
								}
								if (aName.contains("CompanionVault_TypeA_Broken_C"))
								{
									if (WorldToScreen(WorldPosition, &ScreenPos))
									{
										static std::string SPText = "Broken Box";
										Render::R_DrawText(ScreenPos.X, ScreenPos.Y, SPText, &ColorCommon, true);
									}

									TFD::FVector PlayerPosition = LocalPlayerCharacter->K2_GetActorLocation();
									float Distance = WorldPosition.GetDistanceTo(PlayerPosition);
									if (Distance <= CFG::cfg_Loot_ContainerDropRange && CFG::cfg_Loot_MultiplyDrops)
									{
										if (WorldToScreen(WorldPosition, &ScreenPos))
										{
											float ODistance = Distance;
											if (ODistance > 0)
											{
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 3.0f, &ColorWhite);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 5.0f, &ColorGreen);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 7.0f, &ColorMana);
											}
										}
									}

									if (TrySpawnGenericLoot)
									{
										int Amount = CFG::cfg_Loot_MultiplyDrops ? CFG::cfg_Loot_SpawnCount : 1;
										MultiplyDrops(Actor, Amount, false);
									}
									continue;
								}
								if (aName.contains("CompanionVault_TypeA_C"))
								{
									if (WorldToScreen(WorldPosition, &ScreenPos))
									{
										static std::string SPText = "Special Box";
										Render::R_DrawText(ScreenPos.X, ScreenPos.Y, SPText, &ColorCommon, true);
									}

									TFD::FVector PlayerPosition = LocalPlayerCharacter->K2_GetActorLocation();
									float Distance = WorldPosition.GetDistanceTo(PlayerPosition);
									if (Distance <= CFG::cfg_Loot_ContainerDropRange && CFG::cfg_Loot_MultiplyDrops)
									{
										if (WorldToScreen(WorldPosition, &ScreenPos))
										{
											float ODistance = Distance;
											if (ODistance > 0)
											{
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 3.0f, &ColorWhite);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 5.0f, &ColorGreen);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 7.0f, &ColorMana);
											}
										}
									}

									if (TrySpawnGenericLoot)
									{
										int Amount = CFG::cfg_Loot_MultiplyDrops ? CFG::cfg_Loot_SpawnCount : 1;
										MultiplyDrops(Actor, Amount, false);
									}
									continue;
								}
								if (aName.contains("CompanionVault_TypeB_Data_C"))
								{
									if (WorldToScreen(WorldPosition, &ScreenPos))
									{
										static std::string SPText = "Genetic Box";
										Render::R_DrawText(ScreenPos.X, ScreenPos.Y, SPText, &ColorCommon, true);
									}

									TFD::FVector PlayerPosition = LocalPlayerCharacter->K2_GetActorLocation();
									float Distance = WorldPosition.GetDistanceTo(PlayerPosition);
									if (Distance <= CFG::cfg_Loot_ContainerDropRange && CFG::cfg_Loot_MultiplyDrops)
									{
										if (WorldToScreen(WorldPosition, &ScreenPos))
										{
											float ODistance = Distance;
											if (ODistance > 0)
											{
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 3.0f, &ColorWhite);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 5.0f, &ColorGreen);
												Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, 7.0f, &ColorMana);
											}
										}
									}

									if (TrySpawnGenericLoot)
									{
										int Amount = CFG::cfg_Loot_MultiplyDrops ? CFG::cfg_Loot_SpawnCount : 1;
										MultiplyDrops(Actor, Amount, false);
									}
									continue;
								}
							}
						}

						if (!Actor->IsA(TFD::AM1DroppedItem::StaticClass()))
							continue;

						TFD::AM1DroppedItem* Item = static_cast<TFD::AM1DroppedItem*>(Actor);
						if (Item->IsObtained() || Item->bBeingPickedLocally || Item->bTriedSetToObtained || Item->bObtainRequestedOnClient)
							continue;

						SDK::FVector2D ScreenPos = { -1, -1 };
						SDK::FVector WorldPosition = Actor->K2_GetActorLocation();
						bool OnScreen = WorldToScreen(WorldPosition, &ScreenPos);

						if (Item->DropItemInfo.ItemBox.Type == TFD::EM1ItemType::Equipment
							|| Item->DropItemInfo.ItemBox.Type == TFD::EM1ItemType::Consumable
							|| Item->DropItemInfo.ItemBox.Type == TFD::EM1ItemType::Rune)
						{
							bool isFiltered = true;
							SDK::FLinearColor color = ColorWhite;
							std::string Text = "Unknown";

							if (CFG::cfg_Loot_ItemFilterLevel == 1)
							{
								if (Item->IsA(TFD::ABP_EquipTier01DroppedItem_C::StaticClass()))
								{
									color = ColorUncommon;
									Text = "T1 Item";
									isFiltered = false;
								}
								else if (Item->IsA(TFD::ABP_ResourceTier01DroppedItem_C::StaticClass()))
								{
									color = ColorUncommon;
									Text = "T1 Resource";
									isFiltered = false;
								}
								else if (Item->IsA(TFD::ABP_RuneTier01DroppedItem_C::StaticClass()))
								{
									color = ColorUncommon;
									Text = "T1 Rune";
									isFiltered = false;
								}
							}
							if (CFG::cfg_Loot_ItemFilterLevel <= 2)
							{
								if (Item->IsA(TFD::ABP_EquipTier02DroppedItem_C::StaticClass()))
								{
									color = ColorRare;
									Text = "T2 Item";
									isFiltered = false;
								}
								else if (Item->IsA(TFD::ABP_ResourceTier02DroppedItem_C::StaticClass()))
								{
									color = ColorRare;
									Text = "T2 Resource";
									isFiltered = false;
								}
								else if (Item->IsA(TFD::ABP_RuneTier02DroppedItem_C::StaticClass()))
								{
									color = ColorRare;
									Text = "T2 Rune";
									isFiltered = false;
								}
							}
							if (CFG::cfg_Loot_ItemFilterLevel <= 3)
							{
								if (Item->IsA(TFD::ABP_EquipTier03DroppedItem_C::StaticClass()))
								{
									color = ColorGold;
									Text = "T3 Item";
									isFiltered = false;
								}
								else if (Item->IsA(TFD::ABP_ResourceTier03DroppedItem_C::StaticClass()))
								{
									color = ColorGold;
									Text = "T3 Resource";
									isFiltered = false;
								}
								else if (Item->IsA(TFD::ABP_RuneTier03DroppedItem_C::StaticClass()))
								{
									color = ColorGold;
									Text = "T3 Rune";
									isFiltered = false;
								}
								else if (Item->IsA(TFD::ABP_RuneTier04DroppedItem_C::StaticClass()))
								{
									color = ColorUltimate;
									Text = "T4 Rune";
									isFiltered = false;
								}
							}


							if (!isFiltered)
							{
								if (OnScreen && CFG::cfg_Loot_DrawItemNames)
								{
									Render::R_DrawText(ScreenPos.X, ScreenPos.Y, Text, &color);
								}

								if (OnScreen && CFG::cfg_Loot_DrawItemBoxes)
								{
									float ODistance = Item->GetDistanceTo(LocalPlayerController->Pawn) / 100.0f;
									if (ODistance > 0)
									{
										float Width = std::clamp(20.0f / ODistance, 3.0f, 20.0f);
										float Height = std::clamp(20.0f / ODistance, 3.0f, 20.0f);
										Render::R_DrawRect(ScreenPos.X - Width, ScreenPos.Y - Height, ScreenPos.X + Width, ScreenPos.Y + Height, &color);
									}
								}

								if (OnScreen && CFG::cfg_Loot_DrawItemCircles)
								{
									float ODistance = Item->GetDistanceTo(LocalPlayerController->Pawn) / 100.0f;
									if (ODistance > 0)
									{
										float Radius = std::clamp(20.0f / ODistance, 3.0f, 20.0f);
										Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, Radius, &color);
									}
								}

								if (CFG::cfg_Loot_EnableLootVacuum)
								{
									SDK::FVector player = LocalPlayerCharacter->K2_GetActorLocation();
									float Distance = WorldPosition.GetDistanceTo(player);
									if (Distance > 150 && Distance < CFG::cfg_Loot_LootVacuumRange)
										Item->K2_SetActorLocation(player, false, nullptr, true);
								}

							}

						}
						else if (Item->DropItemInfo.ItemBox.Type == TFD::EM1ItemType::InstantUse)
						{
							float hp = (float)Cheat::LocalPlayerCharacter->StatComponent->GetStatValue(TFD::EM1StatType::Stat_CurrentHp).Value / 10000.0f;
							float maxhp = (float)Cheat::LocalPlayerCharacter->StatComponent->GetStatValue(TFD::EM1StatType::Stat_MaxHp).Value / 10000.0f;
							float mana = (float)Cheat::LocalPlayerCharacter->StatComponent->GetStatValue(TFD::EM1StatType::Stat_CurrentMp).Value / 10000.0f;
							float maxmana = (float)Cheat::LocalPlayerCharacter->StatComponent->GetStatValue(TFD::EM1StatType::Stat_MaxMp).Value / 10000.0f;
							bool shouldVacuum = false;
							SDK::FLinearColor color = ColorUltimate;
							std::string Text = "Unknown InstantUse";

							if (Item->IsA(TFD::ABP_HealthOrbDroppedItem_C::StaticClass()))
							{
								color = ColorRed;
								Text = "Health";
								if (hp < maxhp * 0.65f)
									shouldVacuum = true;
							}
							else if (Item->IsA(TFD::ABP_ManaOrbDroppedItem_C::StaticClass()))
							{
								color = ColorMana;
								Text = "Mana";
								if (mana < maxmana * 0.65f)
									shouldVacuum = true;
							}
							else if (Item->IsA(TFD::ABP_AmmoEnhancedDroppedItem_C::StaticClass()))
							{
								Text = "Enhanced Ammo";
								if(GetSpareRounds(TFD::EM1RoundsType::EnhancedRounds,120))
									shouldVacuum = true;
							}
							else if (Item->IsA(TFD::ABP_AmmoGeneralDroppedItem_C::StaticClass()))
							{
								Text = "General Ammo";
								if (GetSpareRounds(TFD::EM1RoundsType::GeneralRounds, 200))
									shouldVacuum = true;
							}
							else if (Item->IsA(TFD::ABP_AmmoHighpowerDroppedItem_C::StaticClass()))
							{
								Text = "Highpower Ammo";
								if (GetSpareRounds(TFD::EM1RoundsType::HighpowerRounds, 2))
									shouldVacuum = true;
							}
							else if (Item->IsA(TFD::ABP_AmmoImpactDroppedItem_C::StaticClass()))
							{
								Text = "Impact Ammo";
								if (GetSpareRounds(TFD::EM1RoundsType::ImpactRounds, 25))
									shouldVacuum = true;
							}
							else
							{
								Text = Item->Class->GetFullName();
							}

							if (OnScreen && CFG::cfg_Loot_DrawItemNames)
							{
								Render::R_DrawText(ScreenPos.X, ScreenPos.Y, Text, &color);
							}

							if (OnScreen && CFG::cfg_Loot_DrawItemBoxes)
							{
								float ODistance = Item->GetDistanceTo(LocalPlayerController->Pawn) / 100.0f;
								if (ODistance > 0)
								{
									float Width = std::clamp(20.0f / ODistance, 3.0f, 20.0f);
									float Height = std::clamp(20.0f / ODistance, 3.0f, 20.0f);
									Render::R_DrawRect(ScreenPos.X - Width, ScreenPos.Y - Height, ScreenPos.X + Width, ScreenPos.Y + Height, &color);
								}
							}

							if (OnScreen && CFG::cfg_Loot_DrawItemCircles)
							{
								float ODistance = Item->GetDistanceTo(LocalPlayerController->Pawn) / 100.0f;
								if (ODistance > 0)
								{
									float Radius = std::clamp(20.0f / ODistance, 3.0f, 20.0f);
									Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, Radius, &color);
								}
							}

							if (shouldVacuum && CFG::cfg_Loot_EnableLootVacuum)
							{
								SDK::FVector player = LocalPlayerCharacter->K2_GetActorLocation();
								float Distance = WorldPosition.GetDistanceTo(player);
								if (Distance > 150 && Distance < CFG::cfg_Loot_LootVacuumRange)
									Item->K2_SetActorLocation(player, false, nullptr, true);
							}

						}
						else if (Item->DropItemInfo.ItemBox.Type == TFD::EM1ItemType::Currency)
						{
							SDK::FLinearColor color = ColorDarkRed;
							std::string Text = "Unknown Currency";

							if (Item->IsA(TFD::ABP_GoldDroppedItem_C::StaticClass()))
							{
								color = ColorGold;
								Text = "Gold";
							}
							else if (Item->IsA(TFD::ABP_KuiperShardDroppedItem_C::StaticClass()))
							{
								color = ColorMana;
								Text = "Kuiper Shard";
							}

							if (OnScreen && CFG::cfg_Loot_DrawItemNames)
							{
								Render::R_DrawText(ScreenPos.X, ScreenPos.Y, Text, &color);
							}

							if (OnScreen && CFG::cfg_Loot_DrawItemBoxes)
							{
								float ODistance = Item->GetDistanceTo(LocalPlayerController->Pawn) / 100.0f;
								if (ODistance > 0)
								{
									float Width = std::clamp(20.0f / ODistance, 3.0f, 20.0f);
									float Height = std::clamp(20.0f / ODistance, 3.0f, 20.0f);
									Render::R_DrawRect(ScreenPos.X - Width, ScreenPos.Y - Height, ScreenPos.X + Width, ScreenPos.Y + Height, &color);
								}
							}

							if (OnScreen && CFG::cfg_Loot_DrawItemCircles)
							{
								float ODistance = Item->GetDistanceTo(LocalPlayerController->Pawn) / 100.0f;
								if (ODistance > 0)
								{
									float Radius = std::clamp(20.0f / ODistance, 3.0f, 20.0f);
									Render::R_DrawCircle(ScreenPos.X, ScreenPos.Y, Radius, &color);
								}
							}

							if (CFG::cfg_Loot_EnableLootVacuum)
							{
								SDK::FVector player = LocalPlayerCharacter->K2_GetActorLocation();
								float Distance = WorldPosition.GetDistanceTo(player);
								if (Distance > 150 && Distance < CFG::cfg_Loot_LootVacuumRange)
									Item->K2_SetActorLocation(player, false, nullptr, true);
							}
						}

					}
				}
			}
		}
	}

	void Aimbot()
	{
		static int currentTargetID;

		if (CFG::cfg_Aim_EnableAimbotHold)
		{
			if (!CFG::cfg_Aim_AimbotUseController && !IsKeyHeld(CFG::cfg_Aim_AimbotHoldKey))
			{
				currentTargetID = 0;
				Aimbot_BoneIndex = -1;
				return;
			}
			if (CFG::cfg_Aim_AimbotUseController)
			{
				bool isAiming = false;
				for (DWORD i = 0; i < 4; i++)
				{
					if (g_Controllers[i].bConnected)
					{
						if (g_Controllers[i].state.Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
						{
							isAiming = true;
						}
					}
				}
				if (!isAiming)
				{
					currentTargetID = 0;
					Aimbot_BoneIndex = -1;
					return;
				}
			}
		}
		else if (CFG::cfg_Aim_EnableAimbotToggle && !CFG::cfg_Aim_EnableAimbot)
		{
			currentTargetID = 0;
			Aimbot_BoneIndex = -1;
			return;
		}

		if (currentTargetID && Aimbot_Target && Aimbot_BoneIndex != -1)
		{
			if (Aimbot_Target->IsA(TFD::AM1Character::StaticClass()))
			{
				TFD::AM1Character* AimTarget = static_cast<TFD::AM1Character*>(Aimbot_Target);
				if (!AimTarget->IsDead() && LocalPlayerController->LineOfSightTo(AimTarget, SDK::FVector{ 0, 0, 0 }, false))
				{
					if (AimTarget->Mesh && AimTarget->Mesh->BoneArray.IsValid() && AimTarget->Mesh->BoneArray.IsValidIndex(Aimbot_BoneIndex))
					{
						SDK::FMatrix ComponentMatrix = TFD::UKismetMathLibrary::Conv_TransformToMatrix(AimTarget->Mesh->K2_GetComponentToWorld());
						SDK::FTransform bone = AimTarget->Mesh->BoneArray[Aimbot_BoneIndex];
						SDK::FMatrix BoneMatrix = TFD::UKismetMathLibrary::Conv_TransformToMatrix(bone);
						SDK::FMatrix WorldMatrix = TFD::UKismetMathLibrary::Multiply_MatrixMatrix(BoneMatrix, ComponentMatrix);
						SDK::FTransform WorldPosition = TFD::UKismetMathLibrary::Conv_MatrixToTransform(WorldMatrix);
						SDK::FRotator Angles = TFD::UKismetMathLibrary::FindLookAtRotation(LocalPlayerController->PlayerCameraManager->GetCameraLocation(), WorldPosition.Translation);
						LocalPlayerController->SetControlRotation(
							TFD::UKismetMathLibrary::RInterpTo(LocalPlayerController->PlayerCameraManager->GetCameraRotation(), Angles, ((TFD::UGameplayStatics*)TFD::UGameplayStatics::StaticClass())->GetWorldDeltaSeconds(GWorld), CFG::cfg_Aim_AimbotSpeed));
						return;
					}

				}
			}
			else if (Aimbot_Target->IsA(TFD::AM1AbilityActor::StaticClass()))
			{

				SDK::FRotator Angles = TFD::UKismetMathLibrary::FindLookAtRotation(LocalPlayerController->PlayerCameraManager->GetCameraLocation(), Aimbot_Target->K2_GetActorLocation());
				LocalPlayerController->SetControlRotation(
					TFD::UKismetMathLibrary::RInterpTo(LocalPlayerController->PlayerCameraManager->GetCameraRotation(), Angles, ((TFD::UGameplayStatics*)TFD::UGameplayStatics::StaticClass())->GetWorldDeltaSeconds(GWorld), CFG::cfg_Aim_AimbotSpeed));
				return;
			}
			currentTargetID = 0;
			Aimbot_BoneIndex = -1;
			Aimbot_Target = nullptr;
		}

		Aimbot_Target = GetClosestEnemy(currentTargetID);
	}

	TFD::AActor* GetClosestEnemy(int& ID)
	{

		TFD::AActor* ret = nullptr;
		float closestDistance = INFINITY;
		Aimbot_BoneIndex = -1;
		if (LocalPlayerController->ActorManager_Subsystem->Characters.IsValid() && LocalPlayerController->ActorManager_Subsystem->Characters.Num() > 0)
		{
			int StartNumber = LocalPlayerController->ActorManager_Subsystem->Characters.Num();
			for (int i = 0; i < LocalPlayerController->ActorManager_Subsystem->Characters.Num(); i++)
			{
				if (LocalPlayerController->ActorManager_Subsystem->Characters.Num() != StartNumber)
					return nullptr;
				if (!LocalPlayerController->ActorManager_Subsystem->Characters.IsValidIndex(i))
					continue;
				TFD::AM1Character* p = LocalPlayerController->ActorManager_Subsystem->Characters[i];
				if (p)
				{
					if (p->IsDead())
						continue;
					if (p->IsA(TFD::AM1Monster::StaticClass()) || p->CharacterAttribute->IsA(TFD::UM1MonsterAttribute::StaticClass()))
					{
						if (!LocalPlayerController->LineOfSightTo(p, SDK::FVector{ 0, 0, 0 }, false))
						{
							if (p->Children.Num() > 0)
							{
								bool any = false;
								for (int c = 0; c < p->Children.Num(); c++)
								{
									if (p->Children[c]->IsA(TFD::AM1AbilityActor::StaticClass()))
										if (p->Children[c]->Class->GetFullName().contains("Immunity") || p->Children[c]->Class->GetFullName().contains("JudgementEye"))
											if (LocalPlayerController->LineOfSightTo(p->Children[c], SDK::FVector{ 0, 0, 0 }, false))
												any = true;
								}
								if (!any)
									continue;
							}
							else
								continue;
						}
						if (!IDBoneMap.contains(p->CharacterId.ID))
						{
							if (p->Mesh && p->Mesh->BoneArray.IsValid() && p->Mesh->BoneArray.Num() > 0)
							{
								std::vector<int> bones = { };
								for (int j = 0; j < p->Mesh->BoneArray.Num(); j++)
								{
									if (p->Mesh->BoneArray.IsValidIndex(j))
									{
										if (p->Mesh->GetBoneName(j).ToString().contains("Weakness") || p->Mesh->GetBoneName(j).ToString().contains("-Head") || p->Mesh->GetBoneName(j).ToString().contains("_head"))
										{
											bones.push_back(j);
										}
									}
								}
								if (bones.size() > 0)
								{
									IDBoneMap.insert({ p->CharacterId.ID, bones });
									UpdateCache = true;
								}
							}
						}
						else
						{
							if (p->Mesh && p->Mesh->BoneArray.IsValid() && p->Mesh->BoneArray.Num() > 0)
							{
								SDK::FMatrix ComponentMatrix = TFD::UKismetMathLibrary::Conv_TransformToMatrix(p->Mesh->K2_GetComponentToWorld());
								std::vector<int> indexes = IDBoneMap[p->CharacterId.ID];
								for (int j = 0; j < indexes.size(); j++)
								{
									if (p->Mesh->BoneArray.IsValidIndex(indexes[j]))
									{
										SDK::FTransform bone = p->Mesh->BoneArray[indexes[j]];
										SDK::FMatrix BoneMatrix = TFD::UKismetMathLibrary::Conv_TransformToMatrix(bone);
										SDK::FMatrix WorldMatrix = TFD::UKismetMathLibrary::Multiply_MatrixMatrix(BoneMatrix, ComponentMatrix);
										SDK::FTransform WorldPosition = TFD::UKismetMathLibrary::Conv_MatrixToTransform(WorldMatrix);
										SDK::FVector2D BoneScreenPos = { -1, -1 };
										if (WorldToScreen(WorldPosition.Translation, &BoneScreenPos))
										{
											float distance = TFD::UKismetMathLibrary::Distance2D(ScreenMiddle, BoneScreenPos);
											if (distance > CFG::cfg_Aim_AimbotFOV)
												continue;
											if (distance < closestDistance)
											{
												closestDistance = distance;
												ID = p->Index;
												Aimbot_BoneIndex = indexes[j];
												ret = p;
											}
										}
									}
								}
							}

							if (p->Children.Num() > 0)
							{
								for (int a = 0; a < p->Children.Num(); a++)
								{
									if (p->Children[a]->IsA(TFD::AM1AbilityActor::StaticClass()))
									{
										if (!(p->Children[a]->Class->GetFullName().contains("Immun")) && !(p->Children[a]->Class->GetFullName().contains("JudgementEye")))
											continue;
										SDK::FVector2D ScreenPos = { -1, -1 };
										if (WorldToScreen(p->Children[a]->K2_GetActorLocation(), &ScreenPos))
										{
											float distance = TFD::UKismetMathLibrary::Distance2D(ScreenMiddle, ScreenPos);
											if (distance > CFG::cfg_Aim_AimbotFOV)
												continue;
											if (distance < closestDistance)
											{
												closestDistance = distance;
												ID = p->Children[a]->Index;
												Aimbot_BoneIndex = 999;
												ret = p->Children[a];
											}
										}
									}
								}
							}

						}
					}
				}
			}
		}
		return ret;
	}

	void SaveCacheData()
	{
		std::ofstream outFile("cache.dat", std::ios::binary);

		if (!outFile) {
			return;
		}
		outFile.clear();
		// Write the size of the map first
		size_t mapSize = IDBoneMap.size();
		outFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

		// Write each key-vector pair
		for (const auto& pair : IDBoneMap) {
			// Write the key
			outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));

			// Write the size of the vector
			size_t vectorSize = pair.second.size();
			outFile.write(reinterpret_cast<const char*>(&vectorSize), sizeof(vectorSize));

			// Write the vector elements
			outFile.write(reinterpret_cast<const char*>(pair.second.data()), vectorSize * sizeof(int));
		}

		size_t size = IDNameMap.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

		// Write each key-value pair
		for (const auto& pair : IDNameMap) {
			// Write the key
			outFile.write(reinterpret_cast<const char*>(&pair.first), sizeof(pair.first));

			// Write the length of the string value
			size_t strLength = pair.second.size();
			outFile.write(reinterpret_cast<const char*>(&strLength), sizeof(strLength));

			// Write the string value
			outFile.write(pair.second.data(), strLength);
		}

		outFile.close();

	}
	void LoadCacheData()
	{
		std::unordered_map<int, std::vector<int>> bonemap;
		std::unordered_map<int, std::string> map;
		std::ifstream inFile("cache.dat", std::ios::binary);

		if (!inFile) {
			return;
		}

		size_t mapSize = 0;
		inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

		for (size_t i = 0; i < mapSize; ++i) {
			int key;
			size_t vectorSize;
			std::vector<int> value;

			// Read the key
			inFile.read(reinterpret_cast<char*>(&key), sizeof(key));

			// Read the size of the vector
			inFile.read(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));

			// Resize the vector to hold the elements
			value.resize(vectorSize);

			// Read the vector elements
			inFile.read(reinterpret_cast<char*>(value.data()), vectorSize * sizeof(int));

			// Insert into the map
			bonemap[key] = value;
		}

		// Read the size of the map
		size_t size = 0;
		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

		for (size_t i = 0; i < size; ++i) {
			int key;
			size_t strLength;
			std::string value;

			// Read the key
			inFile.read(reinterpret_cast<char*>(&key), sizeof(key));

			// Read the length of the string value
			inFile.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));

			// Read the string value
			value.resize(strLength);
			inFile.read(&value[0], strLength);

			// Insert into the map
			map[key] = value;
		}
		inFile.close();

		IDBoneMap = bonemap;
		IDNameMap = map;
	}

	HRESULT UpdateControllerState()
	{
		DWORD dwResult;
		for (DWORD i = 0; i < 4; i++)
		{
			// Simply get the state of the controller from XInput.
			dwResult = XInputGetState(i, &g_Controllers[i].state);

			if (dwResult == ERROR_SUCCESS)
				g_Controllers[i].bConnected = true;
			else
				g_Controllers[i].bConnected = false;
		}

		return S_OK;
	}

	void MissionESPTeleport()
	{
		if (Missions != nullptr && Missions->AvailableMissions)
		{
			if (Missions->AvailableMissions.IsValid())
			{
				TFD::AActor* TPTarget = nullptr;
				float closestDistance = INFINITY;
				for (int i = 0; i < Missions->AvailableMissions.Num(); i++)
				{
					if (Missions->AvailableMissions.IsValidIndex(i))
					{
						TFD::AM1MissionActor* Actor = Missions->AvailableMissions[i];
						if (Actor)
						{
							SDK::FVector2D ScreenPos = { -1, -1 };
							if (WorldToScreen(Actor->K2_GetActorLocation(), &ScreenPos))
							{
								if (CFG::cfg_Mission_EnableMissionESP)
								{
									if (Actor->MissionData)
									{
										std::string MissionName = Actor->MissionData->MissionDataRowName.ToString();
										if (!MissionName.empty())
										{
											//char bufferlol[128];
											//sprintf_s(bufferlol, "Mission %s", Actor->MissionData->MissionDataRowName.ToString().c_str());
											Render::R_DrawText(ScreenPos.X, ScreenPos.Y, MissionName, &ColorGold, true);
										}
									}
								}

								float distance = TFD::UKismetMathLibrary::Distance2D(ScreenMiddle, ScreenPos);
								if (distance > 300)
									continue;
								if (distance < closestDistance)
								{
									closestDistance = distance;
									TPTarget = Actor;
								}
							}
						}
					}
				}
				if (IsKeyPressed(CFG::cfg_Mission_MissionTeleportKey) && TPTarget != nullptr)
				{
					LocalPlayerCharacter->TeleportHandler->ServerMoveToTeleportToLocation(TPTarget->K2_GetActorLocation(), TPTarget->K2_GetActorRotation());
				}
			}
		}
	}

	void AutoRestartMission()
	{
		if (Missions == nullptr)
			return;

		static std::chrono::steady_clock::time_point LastMissionEndTime;
		static bool bMissionCompleted = false;
		Render::R_DrawText(ScreenMiddle.X, 50, "Mission Auto-Restart Enabled", &ColorRed);

		if (bMissionCompleted)
			Render::R_DrawText(ScreenMiddle.X, 64, "Mission Completed", &ColorRed);
		else
			Render::R_DrawText(ScreenMiddle.X, 64, "No Mission Completed", &ColorRed);


		//if (!Missions->ActivatedMissions.IsValid() || !Missions->LastActivatedMissions.IsValid())
		//	return;

		//Render::R_DrawText(ScreenMiddle.X, 78, "Pointers valid", &ColorRed);

		// Check if the player has completed a mission and is not in a new one
		if (Missions->LastActivatedMissions.IsValid())
		{
			if (!bMissionCompleted && !Missions->ActivatedMissions.IsValid())
			{
				// Store the time when the mission ended
				LastMissionEndTime = std::chrono::steady_clock::now();
				bMissionCompleted = true;
			}
		}

		// If we are in the waiting period after mission completion
		if (bMissionCompleted)
		{

			if (Missions->ActivatedMissions.IsValid())
			{
				bMissionCompleted = false;
				return;
			}
			// Get the current time
			auto Now = std::chrono::steady_clock::now();

			// Calculate the elapsed time in seconds
			double ElapsedSeconds = std::chrono::duration<double>(Now - LastMissionEndTime).count();

			char bufferlol[32];
			sprintf_s(bufferlol, "Elapsed Time %f", ElapsedSeconds);
			Render::R_DrawText(ScreenMiddle.X, 92, bufferlol, &ColorRed);

			// Check if 5 seconds have passed
			if (ElapsedSeconds >= CFG::cfg_Mission_MissionAutoRestartDelay)
			{
				// Reset the flag
				Missions->ServerRestartLastPlayedMission();
				bMissionCompleted = false;
			}
		}
	}

	void WeaponModifications()
	{
		if (!LocalPlayerCharacter)
			return;

		auto* WeaponControl = LocalPlayerCharacter->WeaponSlotControl;
		if (!WeaponControl)
			return;

		auto* Weapon = WeaponControl->ActivatedWeaponSlot.WeaponSlot.Weapon;
		if (!Weapon)
			return;

		if (CFG::cfg_Aim_RapidFire && Weapon->FireLoopComponent)
		{
			auto& Params = Weapon->FireLoopComponent->CurrFireParams;
			if (!Params.IsSet())
				return;

			float& FireInterval = Params.GetValueRef().FireInterval;
			float& ElapsedTime = Weapon->FireLoopComponent->ElapsedTimeAfterFire;

			FireInterval -= FireInterval * (CFG::cfg_Aim_FireRate / 100.0f);
			ElapsedTime -= ElapsedTime * (CFG::cfg_Aim_FireRate / 100.0f);
		}

		if (CFG::cfg_Aim_NoReload && Weapon->RoundsComponent)
		{
			if (Weapon->RoundsComponent->CurrentRounds < 3)
			{
				Weapon->RoundsComponent->ClientFillCurrentRoundByServer();
			}
		}

		if (CFG::cfg_Aim_NoRecoilAndSpread && Weapon->SprayPatternComponent)
		{
			auto* Spray = Weapon->SprayPatternComponent;

			Spray->CrosshairSizeBase = 0.0f;
			Spray->bApplySpreadSize = false;
			Spray->CurrentSpreadSize = 0.0f;
			Spray->RecoilData->RecoilApplyInterpSpeed = 0.000001f;
			Spray->RecoilData->RecoilRecoverInterpSpeed = 0.000001f;
			Spray->ZoomRecoilData->RecoilApplyInterpSpeed = 0.000001f;
			Spray->ZoomRecoilData->RecoilRecoverInterpSpeed = 0.000001f;
		}
	}
	/*if (Spray->RecoilData)
	{
		if (Spray->RecoilData->RecoilApplyInterpSpeed > 0.000001f)
			Spray->RecoilData->RecoilApplyInterpSpeed = 0.000001f;
		if (Spray->RecoilData->RecoilRecoverInterpSpeed > 0.000001f)
			Spray->RecoilData->RecoilRecoverInterpSpeed = 0.000001f;
	}

	if (Spray->ZoomRecoilData)
	{
		if (Spray->ZoomRecoilData->RecoilApplyInterpSpeed > 0.000001f)
			Spray->ZoomRecoilData->RecoilApplyInterpSpeed = 0.000001f;
		if (Spray->ZoomRecoilData->RecoilRecoverInterpSpeed > 0.000001f)
			Spray->ZoomRecoilData->RecoilRecoverInterpSpeed = 0.000001f;
	}*/

	void ModifyGrapple()
	{
		if (!Cheat::GWorld || !LocalPlayerCharacter || !LocalPlayerController)
			return;

		if (!LocalPlayerCharacter || !LocalPlayerCharacter->WireCaster || !LocalPlayerCharacter->WeaponSlotControl)
			return;

		auto* Caster = LocalPlayerCharacter->WireCaster;
		auto* Dest = Caster->DestPoint;
		auto* AbilityComponent = LocalPlayerCharacter->WeaponSlotControl->Ability_Component;
		auto* Attribute = Caster->Attribute;

		if (!AbilityComponent)
			return;

		TFD::UM1WireSkillAbility* WireAbility = nullptr;
		TFD::UM1SkillAbility* Skill = nullptr;

		for (auto* Ability : AbilityComponent->RegisteredAbilities)
		{
			if (Ability && Ability->IsA(TFD::UM1WireSkillAbility::StaticClass()))
			{
				WireAbility = static_cast<TFD::UM1WireSkillAbility*>(Ability);
				if (WireAbility)
				{
					if (Dest)
					{
						const TFD::FVector CamLoc = LocalPlayerController->PlayerCameraManager->GetCameraLocation();
						const TFD::FRotator CamRot = LocalPlayerController->PlayerCameraManager->GetCameraRotation();
						const TFD::FVector Forward = RotatorToVector(CamRot);
						const TFD::FVector End = CamLoc + Forward * CFG::cfg_Aim_GrappleRange;
						TFD::FHitResult Hit{};
						Hit.Distance = CFG::cfg_Aim_GrappleRange;
						*reinterpret_cast<TFD::FVector*>(&Hit.Location) = End;
						*reinterpret_cast<TFD::FVector*>(&Hit.ImpactPoint) = End;
						*reinterpret_cast<TFD::FVector*>(&Hit.TraceStart) = CamLoc;
						*reinterpret_cast<TFD::FVector*>(&Hit.TraceEnd) = End;
						Hit.bBlockingHit = false;
						Hit.bStartPenetrating = false;
						const TFD::FVector DestOffest = TFD::FVector{ 0.0f, 0.0f, 0.0f };;
						const TFD::FVector TerrainOffset = TFD::FVector{ 0.0f, 0.0f, 0.0f };;
						float AimPitch = CamRot.Pitch;
						Dest->bValidLandingSpot = true;
						Dest->bAllowedAirDestination = true;
					}
					if (Attribute)
					{
						Attribute->HookFireSpeed = 100000.0f;
					}
					Caster->bValidLandingSpot = true;
					WireAbility->FireMinDistance = 0.0f;
					WireAbility->FireMaxDistance = CFG::cfg_Aim_GrappleRange;
					WireAbility->bAllowedAirDestination = true;
				}
			}
		}
	}




	void CreateConsole()
	{
		TFD::UEngine* Engine = TFD::UEngine::GetEngine();
		if (!Engine->GameViewport->ViewportConsole)
		{
			TFD::UInputSettings::GetDefaultObj()->ConsoleKeys[0].KeyName = TFD::UKismetStringLibrary::Conv_StringToName(L"F1");
			Engine->GameViewport->ViewportConsole = static_cast<TFD::UConsole*>(TFD::UGameplayStatics::SpawnObject(Engine->ConsoleClass, Engine->GameViewport));
		}
	}
	void CreateCheatManager()
	{
		LocalPlayerController->CheatManager = static_cast<TFD::UCheatManager*>(TFD::UGameplayStatics::SpawnObject(LocalPlayerController->CheatClass, LocalPlayerController));
	}
	void ToggleCheatUI()
	{
		LocalPlayerController->EnableCheats();
		//static_cast<TFD::UM1Cheat*>(LocalPlayerController->CheatManager)->ToggleUI();
	}

	void PresetOverlay()
	{

		if (!PresetsMap.empty())
		{
			int YPos = 20;
			for (int i = 0; i < PresetsMap.size(); i++)
			{
				std::string buffer;

				if (PresetsMap.contains(i))
					buffer = std::format("Preset {}: {}", i + 1, PresetsMap[i]);
				else
					buffer = std::format("Preset {}: None", i + 1);

				if (i == CurrentPresetIndex)
					Render::R_DrawText(270, YPos, buffer, &ColorGold, true);
				else
					Render::R_DrawText(270, YPos, buffer, &ColorWhite);

				YPos += 14;
			}
		}
	}

	void PresetRefresh()
	{
		if (!LocalPlayerController || !LocalPlayerController->PrivateOnlineServiceComponent)
			return;

		TFD::UM1Account* Account = static_cast<TFD::UM1Account*>(TFD::native_GetUM1Account(LocalPlayerController->PrivateOnlineServiceComponent));
		if (!Account)
			return;
		if (Account->Preset)
		{
			for (const auto& Pair : Account->Preset->PresetSlotByIndex)
			{

				TFD::FM1PresetSlot Value = Pair.Value();
				std::string PresetName = Value.PresetName.ToString();
				if (PresetName.empty())
				{
					PresetName = std::format("Preset {}", Value.PresetIndex);
				}
				PresetsMap.insert({ Value.PresetIndex, PresetName });
			}
		}
	}

	void PresetActivate()
	{

		if (!LocalPlayerController || !LocalPlayerController->PrivateOnlineServiceComponent)
			return;

		if (!PresetsMap.empty() && CurrentPresetIndex < PresetsMap.size())
		{
			for (TFD::UM1PrivateOnlineSubService* Subserv : LocalPlayerController->PrivateOnlineServiceComponent->SubServices)
			{
				if (!Subserv)
					continue;
				if (Subserv->IsA(TFD::UM1PrivateOnlineServicePreset::StaticClass()) && Subserv->bIsReady == true)
				{
					static_cast<TFD::UM1PrivateOnlineServicePreset*>(Subserv)->ServerRequestApplyPreset(CurrentPresetIndex);
					break;
				}
			}
		}

	}

	void InstantInfiltration()
	{
		if (!Cheat::GWorld || !LocalPlayerController)
			return;

		auto* PlayerState = static_cast<TFD::AM1PlayerState*>(LocalPlayerController->PlayerState);
		if (!PlayerState || !PlayerState->MissionControlComponent)
			return;

		TFD::UM1MissionControlComponent* MCC = PlayerState->MissionControlComponent;
		if (!MCC || MCC->ActivatedMissions.Num() == 0 || MCC->SubServices.Num() == 0)
			return;

		for (TFD::AM1MissionActor* MissionActor : MCC->ActivatedMissions)
		{
			if (!MissionActor)
				continue;

			auto* TaskActor = MissionActor->ProgressInfo.ActivatedTaskActor;
			if (!TaskActor || !TaskActor->IsA(TFD::AM1MissionTaskActorDestructionVulgusPost::StaticClass()))
				continue;

			auto* VPost = static_cast<TFD::AM1MissionTaskActorDestructionVulgusPost*>(TaskActor);
			if (!VPost)
				continue;

			for (TFD::UM1MissionTaskService* MCCSub : MCC->SubServices)
			{
				if (!MCCSub || !MCCSub->bJoined)
					continue;

				auto* MCCInt = static_cast<TFD::UM1MissionTaskServiceInteraction*>(MCCSub);
				if (!MCCInt)
					continue;

				for (TFD::AM1MissionTargetInteraction* Target : VPost->MissionTargets)
				{
					if (!Target ||
						Target->CurrentState == TFD::EM1MissionTargetState::Destructed ||
						Target->CurrentState == TFD::EM1MissionTargetState::Deactivated)
						continue;

					auto now = std::chrono::steady_clock::now();
					if (std::chrono::duration_cast<std::chrono::milliseconds>(now - AutoInstantInfilStartTime).count() > 500)
					{
						AutoInstantInfilStartTime = now;
						MCCInt->ServerRequestMissionTargetBeginInteraction(Target, LocalPlayerController);
						return;
					}
				}
			}
		}
	}


	void ResearchBookmarkedItems()
	{
		if (!Cheat::LocalPlayerController || !Cheat::LocalPlayerController->PrivateOnlineServiceComponent)
			return;

		TFD::UM1LocalGameInstanceSubsystem* LS = TFD::UM1LocalGameInstanceSubsystem::GetSystem(Cheat::GWorld);
		if (!LS || !LS->ResearchSystem)
			return;

		TFD::UM1ResearchSystem* RS = LS->ResearchSystem;

		if (Cheat::LocalPlayerController->PrivateOnlineServiceComponent->IsA(TFD::UM1PrivateOnlineServiceComponent::StaticClass()))
		{
			for (TFD::UM1PrivateOnlineSubService* Subserv : Cheat::LocalPlayerController->PrivateOnlineServiceComponent->SubServices)
			{
				if (!Subserv)
					continue;

				if (Subserv->IsA(TFD::UM1PrivateOnlineServiceResearch::StaticClass()) && Subserv->bIsReady)
				{
					for (const auto& RDTID : RS->BookmarkResearchTids)
					{
						static_cast<TFD::UM1PrivateOnlineServiceResearch*>(Subserv)->ServerRequestStartResearch(RDTID, CFG::cfg_ResearchQuantity);
					}
				}
			}
		}
	}

	bool GetSpareRounds(TFD::EM1RoundsType RoundsType, int RoundsPerLoot)
	{
		if (!LocalPlayerCharacter || !LocalPlayerCharacter->RoundsComponent)
			return false;

		int CurrentSpareRounds = 0;
		int MaxRoundCapacity = 0;

		CurrentSpareRounds = LocalPlayerCharacter->RoundsComponent->CurrentSpareRounds[(int)RoundsType];

		for (const auto& Capacities : LocalPlayerCharacter->RoundsComponent->CachedMaxCapacities)
		{
			if (Capacities.Key() == RoundsType)
			{
				MaxRoundCapacity = Capacities.Value().CachedCapacity;
				break;
			}
		}

		if (CurrentSpareRounds <= (MaxRoundCapacity - RoundsPerLoot))
			return true;
		else
			return false;
	}

	//Old Versions, Not being used but incase whatever//
	/*void ToggleMemFeature(int what)
	{
		DWORD old;
		if (what == 0)
		{
			VirtualProtect(TFD::NoSpreadAddress, sizeof(uint8_t) * 8, PAGE_EXECUTE_READWRITE, &old);
			if (CFG::cfg_Aim_NoSpread)
				memcpy(TFD::NoSpreadAddress, &TFD::NoSpreadCheat, sizeof(uint8_t) * 8);
			else
				memcpy(TFD::NoSpreadAddress, &TFD::NoSpreadOriginal, sizeof(uint8_t) * 8);
			VirtualProtect(TFD::NoSpreadAddress, sizeof(uint8_t) * 8, old, NULL);
		}
		if (what == 1)
		{
			VirtualProtect(TFD::NoRecoilAddress, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
			if (CFG::cfg_Aim_NoRecoil)
				memcpy(TFD::NoRecoilAddress, &TFD::bytes_Recoil[1], sizeof(uint8_t));
			else
				memcpy(TFD::NoRecoilAddress, &TFD::bytes_Recoil[0], sizeof(uint8_t));
			VirtualProtect(TFD::NoRecoilAddress, sizeof(uint8_t), old, NULL);
		}
		if (what == 2)
		{
			VirtualProtect(TFD::RapidFireAddress, sizeof(uint8_t), PAGE_EXECUTE_READWRITE, &old);
			if (CFG::cfg_Aim_RapidFire)
				memcpy(TFD::RapidFireAddress, &TFD::bytes_RapidFire[1], sizeof(uint8_t));
			else
				memcpy(TFD::RapidFireAddress, &TFD::bytes_RapidFire[0], sizeof(uint8_t));
			VirtualProtect(TFD::RapidFireAddress, sizeof(uint8_t), old, NULL);
		}
	}
	void RapidFire()
	{
		if (!LocalPlayerCharacter || !LocalPlayerCharacter->WeaponSlotControl)
			return;

		auto* Weapon = LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon;
		if (!Weapon || !Weapon->FireLoopComponent)
			return;

		auto& Params = Weapon->FireLoopComponent->CurrFireParams;
		if (!Params.IsSet())
			return;

		float& FireInterval = Params.GetValueRef().FireInterval;
		float& ElapsedTime = Weapon->FireLoopComponent->ElapsedTimeAfterFire;

		FireInterval -= FireInterval * (CFG::cfg_Aim_FireRate / 100.0f);
		ElapsedTime -= ElapsedTime * (CFG::cfg_Aim_FireRate / 100.0f);
	}


	void NoRecoil()
	{
		if (!LocalPlayerCharacter)
			return;
		char buffer[300];
		if (LocalPlayerCharacter->WeaponSlotControl)
		{
			if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon)
			{
				if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent)
				{
					if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData)
					{
						if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData->RecoilApplyInterpSpeed > 0.000001f)
							LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData->RecoilApplyInterpSpeed = 0.000001f;
						if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData->RecoilRecoverInterpSpeed > 0.000001f)
							LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData->RecoilRecoverInterpSpeed = 0.000001f;
					}

					if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData)
					{
						if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData->RecoilApplyInterpSpeed > 0.000001f)
							LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData->RecoilApplyInterpSpeed = 0.000001f;
						if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData->RecoilRecoverInterpSpeed > 0.000001f)
							LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData->RecoilRecoverInterpSpeed = 0.000001f;
					}
				}
			}
		}
	}


	void DoRapidFire()
	{
		static bool foundWeapon = false;
		if (!foundWeapon)
		{
			for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
			{
				SDK::UObject* Obj = SDK::UObject::GObjects->GetByIndex(i);

				if (!Obj)
					continue;

				if (Obj->Flags & SDK::EObjectFlags::BeginDestroyed ||
					Obj->Flags & SDK::EObjectFlags::BeingRegenerated ||
					Obj->Flags & SDK::EObjectFlags::FinishDestroyed ||
					Obj->Flags & SDK::EObjectFlags::NeedInitialization ||
					Obj->Flags & SDK::EObjectFlags::WillBeLoaded)
					continue;

				if (Obj->IsA(TFD::UM1WeaponSlotControlComponent::StaticClass()))
				{
					TFD::UM1WeaponSlotControlComponent* Wep = static_cast<TFD::UM1WeaponSlotControlComponent*>(Obj);
					if (!Wep)
						continue;
					if (!Wep->Player_Owner)
						continue;
					if (Wep->Player_Owner == LocalPlayerCharacter)
					{
						WeaponSlot = Wep;
						foundWeapon = true;
						break;
					}
				}
			}
		}
		else
		{
			if (!WeaponSlot || WeaponSlot->Player_Owner != LocalPlayerCharacter)
			{
				foundWeapon = false;
				return;
			}
			else
			{
				if (WeaponSlot->Flags & SDK::EObjectFlags::BeginDestroyed ||
					WeaponSlot->Flags & SDK::EObjectFlags::BeingRegenerated ||
					WeaponSlot->Flags & SDK::EObjectFlags::FinishDestroyed ||
					WeaponSlot->Flags & SDK::EObjectFlags::NeedInitialization ||
					WeaponSlot->Flags & SDK::EObjectFlags::WillBeLoaded)
				{
					foundWeapon = false;
					WeaponSlot = nullptr;
					return;
				}
				if (WeaponSlot->ActivatedWeaponSlot.WeaponSlot.Weapon)
				{
					if (WeaponSlot->ActivatedWeaponSlot.WeaponSlot.Weapon->FireLoopComponent)
					{
						WeaponSlot->ActivatedWeaponSlot.WeaponSlot.Weapon->FireLoopComponent->ElapsedTimeAfterFire = 0.0f;
					}
				}
			}
		}
	}
	void InstantReload()
	{
		if (!LocalPlayerCharacter)
			return;

		if (LocalPlayerCharacter->WeaponSlotControl)
		{
			if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon)
			{
				if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->RoundsComponent)
				{
					if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->RoundsComponent->CurrentRounds < 3)
					{
						LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->RoundsComponent->ClientFillCurrentRoundByServer();
					}
				}
			}
		}
	}

	void NoRecoilAndSpread()
	{
		if (!LocalPlayerCharacter)
			return;

		if (LocalPlayerCharacter->WeaponSlotControl)
		{
			if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon)
			{
				if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent)
				{
					if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->bApplySpreadSize == true)
						LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->bApplySpreadSize = false;
					if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->CrosshairSizeBase > 1.0f)
						LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->CrosshairSizeBase = 1.0f;
					if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->CurrentSpreadSize > 1.0f)
						LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->CurrentSpreadSize = 1.0f;
					if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData)
					{
						if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData->RecoilApplyInterpSpeed > 0.000001f)
							LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData->RecoilApplyInterpSpeed = 0.000001f;
						if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData->RecoilRecoverInterpSpeed > 0.000001f)
							LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->RecoilData->RecoilRecoverInterpSpeed = 0.000001f;
					}

					if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData)
					{
						if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData->RecoilApplyInterpSpeed > 0.000001f)
							LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData->RecoilApplyInterpSpeed = 0.000001f;
						if (LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData->RecoilRecoverInterpSpeed > 0.000001f)
							LocalPlayerCharacter->WeaponSlotControl->ActivatedWeaponSlot.WeaponSlot.Weapon->SprayPatternComponent->ZoomRecoilData->RecoilRecoverInterpSpeed = 0.000001f;
					}
				}
			}
		}
	}*/
}