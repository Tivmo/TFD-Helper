#include "SDK/Basic.hpp"
#include "SDK.h"
//#include "SDK/M1_parameters.hpp"


namespace TFD
{
	//tFindFunctionChecked native_FindFunctionChecked = nullptr;
	//tInitCheatManager native_InitCheatManager = nullptr;
	//tCreateCheatWidget native_CreateCheatWidget = nullptr;
	//tActorMiniGameBeginPlay native_ActorMiniGameBeginPlay = nullptr;
	//tInteractableActorCanActivate native_oInteractableActorCanActivate = nullptr;

	tUMPOSCGetAccount native_GetUM1Account = nullptr;

	uintptr_t BASE = 0x0;
	uintptr_t SIZE = 0x0;

	//uint8_t* NoSpreadAddress = 0;
	//uint8_t* NoRecoilAddress = 0;
	//uint8_t* RapidFireAddress = 0;

	/*UFunction* FindFunctionChecked(UObject* thisObject, FName InName, EIncludeSuperFlag a3)
	{
		return native_FindFunctionChecked(thisObject, InName, a3);
	}

	bool hook_InteractableActorCanActivate(UObject* ThisObj, UObject* InPC)
	{
		std::cout << "Checking if player can interact with: " << ThisObj->GetFullName().c_str() << std::endl;
		if (ThisObj->IsA(TFD::AM1FieldInteractableActorMiniGame::StaticClass()))
		{
			return true;
		}
		return native_oInteractableActorCanActivate(ThisObj, InPC);
	}*/

	/* ACTUAL TFD UE DEFINITIONS */

	class UEngine* UEngine::GetEngine()
	{
		static UEngine* GEngine = nullptr;

		if (GEngine)
			return GEngine;

		/* (Re-)Initialize if GEngine is nullptr */
		for (int i = 0; i < UObject::GObjects->Num(); i++)
		{
			UObject* Obj = UObject::GObjects->GetByIndex(i);

			if (!Obj)
				continue;

			if (Obj->IsA(UEngine::StaticClass()) && !Obj->IsDefaultObject())
			{
				GEngine = static_cast<UEngine*>(Obj);
				break;
			}
		}

		return GEngine;
	}

	class UWorld* UWorld::GetWorld()
	{
		static UWorld* World = nullptr;
		static int WorldIndex = -1;

		if (World == nullptr)
		{
			for (int i = 0; i < UObject::GObjects->Num(); i++)
			{
				UObject* Obj = UObject::GObjects->GetByIndex(i);

				if (!Obj)
					continue;

				if (Obj->Flags & EObjectFlags::LoadCompleted && Obj->IsA(UWorld::StaticClass()) && !Obj->IsDefaultObject())
				{
					
					WorldIndex = i;
					World = static_cast<UWorld*>(Obj);
					//std::cout << "DescentInternal - World Found: " << std::hex << World << std::dec << " with Index: " << WorldIndex << "\n";
					break;
				}
			}
			return World;
		}
		else
		{
			UObject* Obj = UObject::GObjects->GetByIndex(WorldIndex);

			if (!Obj)
			{
				//std::cout << "DescentInternal - World Obj Invalid: " << std::hex << World << std::dec << "\n";
				World = nullptr;
				WorldIndex = -1;
				return World;
			}
			else
			{
				if (!(Obj->Flags & SDK::EObjectFlags::LoadCompleted) || !Obj->IsA(UWorld::StaticClass()) || Obj->IsDefaultObject())
				{
					//std::cout << "DescentInternal - World Obj Not World or is DefaultObject: " << std::hex << World << std::dec << "\n";
					World = nullptr;
					WorldIndex = -1;
					return World;
				}
			}
			return World;
		}
	}


	void UM1PrivateOnlineServiceComponent::ServerChangePlayer(const struct FM1TemplateId& InCharacterTid)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1PrivateOnlineServiceComponent", "ServerChangePlayer");

		M1PrivateOnlineServiceComponent_ServerChangePlayer Parms{};

		Parms.InCharacterTid = std::move(InCharacterTid);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1WeaponRoundsComponent::ClientFillCurrentRoundByServer()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1WeaponRoundsComponent", "ClientFillCurrentRoundByServer");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	bool AM1Character::IsDead() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1Character", "IsDead");

		M1Character_IsDead Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	void UM1TeleportHandlerComponent::ServerMoveToTeleportToLocation(const struct FVector& InLocation, const struct FRotator& InRotation)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("UM1TeleportHandlerComponent", "ServerMoveToTeleportToLocation");

		M1Player_RequestTeleportAtSequence Parms{};

		Parms.InLocation = std::move(InLocation);
		Parms.InRotation = std::move(InRotation);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	bool AActor::K2_SetActorLocation(const struct FVector& NewLocation, bool bSweep, struct FHitResult* SweepHitResult, bool bTeleport)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Actor", "K2_SetActorLocation");

		Actor_K2_SetActorLocation Parms{};

		Parms.NewLocation = std::move(NewLocation);
		Parms.bSweep = bSweep;
		Parms.bTeleport = bTeleport;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		if (SweepHitResult != nullptr)
			*SweepHitResult = std::move(Parms.SweepHitResult);

		return Parms.ReturnValue;
	}

	float AActor::GetDistanceTo(const class AActor* OtherActor) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Actor", "GetDistanceTo");

		Actor_GetDistanceTo Parms{};

		Parms.OtherActor = OtherActor;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FVector AActor::K2_GetActorLocation() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Actor", "K2_GetActorLocation");

		Actor_K2_GetActorLocation Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	void AController::SetControlRotation(const struct FRotator& NewRotation)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Controller", "SetControlRotation");

		Controller_SetControlRotation Parms{};

		Parms.NewRotation = std::move(NewRotation);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	bool AController::LineOfSightTo(const class AActor* Other, const struct FVector& ViewPoint, bool bAlternateChecks) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Controller", "LineOfSightTo");

		Controller_LineOfSightTo Parms{};

		Parms.Other = Other;
		Parms.ViewPoint = std::move(ViewPoint);
		Parms.bAlternateChecks = bAlternateChecks;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FVector APlayerCameraManager::GetCameraLocation() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("PlayerCameraManager", "GetCameraLocation");

		PlayerCameraManager_GetCameraLocation Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FRotator APlayerCameraManager::GetCameraRotation() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("PlayerCameraManager", "GetCameraRotation");

		PlayerCameraManager_GetCameraRotation Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	bool APlayerController::GetMousePosition(float* LocationX, float* LocationY) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("PlayerController", "GetMousePosition");

		PlayerController_GetMousePosition Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		if (LocationX != nullptr)
			*LocationX = Parms.LocationX;

		if (LocationY != nullptr)
			*LocationY = Parms.LocationY;

		return Parms.ReturnValue;
	}

	void UActorComponent::Deactivate()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("ActorComponent", "Deactivate");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	bool UActorComponent::IsActive() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("ActorComponent", "IsActive");

		ActorComponent_IsActive Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FTransform USceneComponent::K2_GetComponentToWorld() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("SceneComponent", "K2_GetComponentToWorld");

		SceneComponent_K2_GetComponentToWorld Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	class FName USkinnedMeshComponent::GetBoneName(int32 BoneIndex) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("SkinnedMeshComponent", "GetBoneName");

		SkinnedMeshComponent_GetBoneName Parms{};

		Parms.BoneIndex = BoneIndex;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	class APlayerController* UGameplayStatics::GetPlayerController(const class UObject* WorldContextObject, int32 PlayerIndex)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("GameplayStatics", "GetPlayerController");

		GameplayStatics_GetPlayerController Parms{};

		Parms.WorldContextObject = WorldContextObject;
		Parms.PlayerIndex = PlayerIndex;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	bool UGameplayStatics::ProjectWorldToScreen(class APlayerController* Player, const struct FVector& WorldPosition, struct FVector2D* ScreenPosition, bool bPlayerViewportRelative)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("GameplayStatics", "ProjectWorldToScreen");

		GameplayStatics_ProjectWorldToScreen Parms{};

		Parms.Player = Player;
		Parms.WorldPosition = std::move(WorldPosition);
		Parms.bPlayerViewportRelative = bPlayerViewportRelative;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		if (ScreenPosition != nullptr)
			*ScreenPosition = std::move(Parms.ScreenPosition);

		return Parms.ReturnValue;
	}

	double UGameplayStatics::GetWorldDeltaSeconds(const class UObject* WorldContextObject)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("GameplayStatics", "GetWorldDeltaSeconds");

		GameplayStatics_GetWorldDeltaSeconds Parms{};

		Parms.WorldContextObject = WorldContextObject;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FTransform UKismetMathLibrary::Conv_MatrixToTransform(const struct FMatrix& InMatrix)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "Conv_MatrixToTransform");

		KismetMathLibrary_Conv_MatrixToTransform Parms{};

		Parms.InMatrix = std::move(InMatrix);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FMatrix UKismetMathLibrary::Conv_TransformToMatrix(const struct FTransform& Transform)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "Conv_TransformToMatrix");

		KismetMathLibrary_Conv_TransformToMatrix Parms{};

		Parms.Transform = std::move(Transform);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FRotator UKismetMathLibrary::FindLookAtRotation(const struct FVector& Start, const struct FVector& Target)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "FindLookAtRotation");

		KismetMathLibrary_FindLookAtRotation Parms{};

		Parms.Start = std::move(Start);
		Parms.Target = std::move(Target);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FMatrix UKismetMathLibrary::Multiply_MatrixMatrix(const struct FMatrix& A, const struct FMatrix& B)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "Multiply_MatrixMatrix");

		KismetMathLibrary_Multiply_MatrixMatrix Parms{};

		Parms.A = std::move(A);
		Parms.B = std::move(B);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FRotator UKismetMathLibrary::RInterpTo(const struct FRotator& Current, const struct FRotator& Target, float DeltaTime, float InterpSpeed)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "RInterpTo");

		KismetMathLibrary_RInterpTo Parms{};

		Parms.Current = std::move(Current);
		Parms.Target = std::move(Target);
		Parms.DeltaTime = DeltaTime;
		Parms.InterpSpeed = InterpSpeed;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	double UKismetMathLibrary::Distance2D(const struct FVector2D& V1, const struct FVector2D& V2)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetMathLibrary", "Distance2D");

		KismetMathLibrary_Distance2D Parms{};

		Parms.V1 = std::move(V1);
		Parms.V2 = std::move(V2);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	void UCanvas::K2_DrawLine(const struct FVector2D& ScreenPositionA, const struct FVector2D& ScreenPositionB, float Thickness, const struct FLinearColor& RenderColor)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Canvas", "K2_DrawLine");

		Canvas_K2_DrawLine Parms{};

		Parms.ScreenPositionA = std::move(ScreenPositionA);
		Parms.ScreenPositionB = std::move(ScreenPositionB);
		Parms.Thickness = Thickness;
		Parms.RenderColor = std::move(RenderColor);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UCanvas::K2_DrawText(class UFont* RenderFont, const class FString& RenderText, const struct FVector2D& ScreenPosition, const struct FVector2D& Scale, const struct FLinearColor& RenderColor, float Kerning, const struct FLinearColor& ShadowColor, const struct FVector2D& ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, const struct FLinearColor& OutlineColor)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Canvas", "K2_DrawText");

		Canvas_K2_DrawText Parms{};

		Parms.RenderFont = RenderFont;
		Parms.RenderText = std::move(RenderText);
		Parms.ScreenPosition = std::move(ScreenPosition);
		Parms.Scale = std::move(Scale);
		Parms.RenderColor = std::move(RenderColor);
		Parms.Kerning = Kerning;
		Parms.ShadowColor = std::move(ShadowColor);
		Parms.ShadowOffset = std::move(ShadowOffset);
		Parms.bCentreX = bCentreX;
		Parms.bCentreY = bCentreY;
		Parms.bOutlined = bOutlined;
		Parms.OutlineColor = std::move(OutlineColor);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	bool AM1DropContainer::IsObtained() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1DropContainer", "IsObtained");

		M1DropContainer_IsObtained Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	int32 USkinnedMeshComponent::GetNumBones() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("SkinnedMeshComponent", "GetNumBones");

		SkinnedMeshComponent_GetNumBones Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	void UM1MissionControlComponent::ServerCompleteCurrentTask()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerCompleteCurrentTask");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::ServerJoinAndBeginMissionBy(class AM1MissionActor* InMission)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerJoinAndBeginMissionBy");

		M1MissionControlComponent_ServerJoinAndBeginMissionBy Parms{};

		Parms.InMission = InMission;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::ServerJoinMissionBy(class AM1MissionActor* InMission, const struct FM1MissionJoinContextParam& InContext)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerJoinMissionBy");

		M1MissionControlComponent_ServerJoinMissionBy Parms{};

		Parms.InMission = InMission;
		Parms.InContext = std::move(InContext);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::ServerJumpToSpecificTask(int32 InTaskIndex)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerJumpToSpecificTask");

		M1MissionControlComponent_ServerJumpToSpecificTask Parms{};

		Parms.InTaskIndex = InTaskIndex;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::ServerRequestRestartMatchingAlone()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerRequestRestartMatchingAlone");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::ServerRestartLastPlayedMission()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerRestartLastPlayedMission");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::ServerStartMission(class AM1MissionActor* InMission)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerStartMission");

		M1MissionControlComponent_ServerStartMission Parms{};

		Parms.InMission = InMission;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::ServerStartMissionByTemplateID(const struct FM1TemplateId& InTemplateId)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerStartMissionByTemplateID");

		M1MissionControlComponent_ServerStartMissionByTemplateID Parms{};

		Parms.InTemplateId = std::move(InTemplateId);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::ServerTeleportToBestPlayer(bool bIgnoreDistance)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerTeleportToBestPlayer");

		M1MissionControlComponent_ServerTeleportToBestPlayer Parms{};

		Parms.bIgnoreDistance = bIgnoreDistance;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::ServerTrySucceedInMission(bool InNoDelay)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "ServerTrySucceedInMission");

		M1MissionControlComponent_ServerTrySucceedInMission Parms{};

		Parms.InNoDelay = InNoDelay;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::TryJoinMissionBy(class AM1MissionActor* InMission)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "TryJoinMissionBy");

		M1MissionControlComponent_TryJoinMissionBy Parms{};

		Parms.InMission = InMission;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::TryStartMission(const class FName& InName)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "TryStartMission");

		M1MissionControlComponent_TryStartMission Parms{};

		Parms.InName = InName;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionControlComponent::TryTeleportToBestPlayer(bool bIgnoreDistance)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionControlComponent", "TryTeleportToBestPlayer");

		M1MissionControlComponent_TryTeleportToBestPlayer Parms{};

		Parms.bIgnoreDistance = bIgnoreDistance;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void APlayerController::EnableCheats()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("PlayerController", "EnableCheats");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	void AM1PlayerState::ServerCheatSetIsCreator(bool InbCreator)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1PlayerState", "ServerCheatSetIsCreator");

		M1PlayerState_ServerCheatSetIsCreator Parms{};

		Parms.InbCreator = InbCreator;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void AM1TaskEventActor::BP_RunResult(EM1TaskEventActorRunResult InRunResult)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1TaskEventActor", "BP_RunResult");

		M1TaskEventActor_BP_RunResult Parms{};

		Parms.InRunResult = InRunResult;

		UObject::ProcessEvent(Func, &Parms);
	}

	void AM1TaskEventActor::TaskActorStateChanged(EM1EventActorState InState)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1TaskEventActor", "TaskActorStateChanged");

		M1TaskEventActor_TaskActorStateChanged Parms{};

		Parms.InState = InState;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	struct FRotator AActor::K2_GetActorRotation() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Actor", "K2_GetActorRotation");

		Actor_K2_GetActorRotation Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	/*void UM1Cheat::ToggleUI()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1Cheat", "ToggleUI");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	void UM1Cheat::InitCheatManager()
	{
		static class UFunction* Func = nullptr;
		SDK::FName FuncName;
		static bool IsInit = false;
		if (!IsInit)
		{
			FString str = FString(L"InitCheatManager");
			FuncName = UKismetStringLibrary::Conv_StringToName(str);
			IsInit = true;
		}

		if (Func == nullptr)
			Func = FindFunctionChecked(this, FuncName, EIncludeSuperFlag::IncludeSuper);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}*/

	class FName UKismetStringLibrary::Conv_StringToName(const class FString& InString)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("KismetStringLibrary", "Conv_StringToName");

		KismetStringLibrary_Conv_StringToName Parms{};

		Parms.InString = std::move(InString);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	class UObject* UGameplayStatics::SpawnObject(TSubclassOf<class UObject> ObjectClass, class UObject* Outer_0)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("GameplayStatics", "SpawnObject");

		GameplayStatics_SpawnObject Parms{};

		Parms.ObjectClass = ObjectClass;
		Parms.Outer_0 = Outer_0;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	struct FM1ScaledInteger UM1StatComponent::GetStatValue(const EM1StatType InStatType) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1StatComponent", "GetStatValue");

		M1StatComponent_GetStatValue Parms{};

		Parms.InStatType = InStatType;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	void AM1PlayerControllerInGame::ServerRequestFieldObjectDropItems(class AM1FieldInteractableActor* InActor)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1PlayerControllerInGame", "ServerRequestFieldObjectDropItems");

		M1PlayerControllerInGame_ServerRequestFieldObjectDropItems Parms{};

		Parms.InActor = InActor;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void AM1MiniGameActor::ClientStopMiniGame()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MiniGameActor", "ClientStopMiniGame");

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, nullptr);

		Func->FunctionFlags = Flgs;
	}

	void AM1MiniGameActor::ServerDropItems(class AController* InInstigator)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MiniGameActor", "ServerDropItems");

		M1MiniGameActor_ServerDropItems Parms{};

		Parms.InInstigator = InInstigator;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}


	// Function M1.M1MiniGameActor.ServerOnMiniGameEnded
	// (Final, Net, NetReliable, Native, Event, Private, NetServer)
	// Parameters:
	// struct FM1MiniGameResult                InResult                                               (ConstParm, Parm, ReferenceParm, NoDestructor, NativeAccessSpecifierPublic)

	void AM1MiniGameActor::ServerOnMiniGameEnded(const struct FM1MiniGameResult& InResult)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MiniGameActor", "ServerOnMiniGameEnded");

		M1MiniGameActor_ServerOnMiniGameEnded Parms{};

		Parms.InResult = std::move(InResult);

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UWidget::SetIsEnabled(bool bInIsEnabled)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Widget", "SetIsEnabled");

		Widget_SetIsEnabled Parms{};

		Parms.bInIsEnabled = bInIsEnabled;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UWidget::SetVisibility(ESlateVisibility InVisibility)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Widget", "SetVisibility");

		Widget_SetVisibility Parms{};

		Parms.InVisibility = InVisibility;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void AActor::GetActorBounds(bool bOnlyCollidingComponents, struct FVector* Origin, struct FVector* BoxExtent, bool bIncludeFromChildActors) const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("Actor", "GetActorBounds");

		Actor_GetActorBounds Parms{};

		Parms.bOnlyCollidingComponents = bOnlyCollidingComponents;
		Parms.bIncludeFromChildActors = bIncludeFromChildActors;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		if (Origin != nullptr)
			*Origin = std::move(Parms.Origin);

		if (BoxExtent != nullptr)
			*BoxExtent = std::move(Parms.BoxExtent);
	}

	struct FBoxSphereBounds USkeletalMesh::GetBounds() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("SkeletalMesh", "GetBounds");

		SkeletalMesh_GetBounds Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	class UM1LocalGameInstanceSubsystem* UM1LocalGameInstanceSubsystem::GetSystem(const class UObject* WorldContextObject)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("M1LocalGameInstanceSubsystem", "Get");

		M1LocalGameInstanceSubsystem_Get Parms{};

		Parms.WorldContextObject = WorldContextObject;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;

		return Parms.ReturnValue;
	}

	void AM1MiniGameActor::ClientStartMiniGame(const struct FM1TemplateId& InMiniGameTid, const struct FM1TemplateId& InPlayerTid, const struct FM1TemplateId& InFieldDifficultyTid, const EM1MiniGameDifficulty InDifficulty)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MiniGameActor", "ClientStartMiniGame");

		M1MiniGameActor_ClientStartMiniGame Parms{};

		Parms.InMiniGameTid = std::move(InMiniGameTid);
		Parms.InPlayerTid = std::move(InPlayerTid);
		Parms.InFieldDifficultyTid = std::move(InFieldDifficultyTid);
		Parms.InDifficulty = InDifficulty;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1PrivateOnlineServicePreset::ServerRequestApplyPreset(int32 InPresetIndex)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1PrivateOnlineServicePreset", "ServerRequestApplyPreset");

		M1PrivateOnlineServicePreset_ServerRequestApplyPreset Parms{};

		Parms.InPresetIndex = InPresetIndex;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1PrivateOnlineServiceResearch::ServerRequestStartResearch(const struct FM1TemplateId& InResearchTemplateId, int32 InRepeatCount)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1PrivateOnlineServiceResearch", "ServerRequestStartResearch");

		M1PrivateOnlineServiceResearch_ServerRequestStartResearch Parms{};

		Parms.InResearchTemplateId = std::move(InResearchTemplateId);
		Parms.InRepeatCount = InRepeatCount;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void UM1MissionTaskServiceInteraction::ServerRequestMissionTargetBeginInteraction(class AM1MissionTargetInteraction* InActor, class AM1PlayerControllerInGame* InAcceptor)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1MissionTaskServiceInteraction", "ServerRequestMissionTargetBeginInteraction");

		M1MissionTaskServiceInteraction_ServerRequestMissionTargetBeginInteraction Parms{};

		Parms.InActor = InActor;
		Parms.InAcceptor = InAcceptor;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void AM1WireCaster::ServerStartWireAction(const struct FHitResult& InHit, const float InAimPitch, const struct FVector& InDestOffest, const struct FVector& InTerrainOffset)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1WireCaster", "ServerStartWireAction");

		M1WireCaster_ServerStartWireAction Parms{};

		Parms.InHit = InHit;
		Parms.InAimPitch = InAimPitch;
		Parms.InDestOffest = InDestOffest;
		Parms.InTerrainOffset = InTerrainOffset;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void AM1WireCaster::NetMulticast_FireWireHook(const struct FHitResult& InHit, const struct FVector& InDestOffest, const struct FVector& InTerrainOffset, const bool bAllowedAir)
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1WireCaster", "ServerStartWireAction");

		M1WireCaster_NetMulticast_FireWireHook Parms{};

		Parms.InHit = InHit;
		Parms.InDestOffest = InDestOffest;
		Parms.InTerrainOffset = InTerrainOffset;
		Parms.bAllowedAir = bAllowedAir;

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	void AM1WireCaster::RequestStartAction()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1WireCaster", "RequestStartAction");

		M1WireCaster_RequestStartAction Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}

	/*void UM1SkillAbility::ClearCooltime()
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1SkillAbility", "ClearCooltime");

		M1SkillAbility_ClearCooltime Parms{};

		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;

		UObject::ProcessEvent(Func, &Parms);

		Func->FunctionFlags = Flgs;
	}*/

	bool AM1Player::IsInAir() const
	{
		static class UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = Class->GetFunction("M1Player", "IsInAir");
		M1Player_IsInAir Parms{};
		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;
		UObject::ProcessEvent(Func, &Parms);
		Func->FunctionFlags = Flgs;
		return Parms.ReturnValue;
	}

	bool AM1Player::IsMovingByWire() const
	{
		static class UFunction* Func = nullptr;
		if (Func == nullptr)
			Func = Class->GetFunction("M1Player", "IsMovingByWire");
		M1Player_IsMovingByWire Parms{};
		auto Flgs = Func->FunctionFlags;
		Func->FunctionFlags |= 0x400;
		UObject::ProcessEvent(Func, &Parms);
		Func->FunctionFlags = Flgs;
		return Parms.ReturnValue;
	}
}