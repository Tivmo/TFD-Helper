#pragma once
#include "SDK/Basic.hpp"
#include "SDK/CoreUObject_classes.hpp"
#include "SDK/CoreUObject_structs.hpp"


namespace TFD
{
	using namespace SDK;

	extern uintptr_t BASE;
	extern uintptr_t SIZE;

	static const char* GNamesSig = "\x4C\x8D\x35\x00\x00\x00\x00\x0F\x95\x44\x24\x2C\x48\x2B\xCB";
	static const char* GNamesMask = "xxx????xxxxxxx";

	static const char* NGSSig = "\x48\x8B\x45\xA0\x0F\x10\x05\x00\x00\x00\x00\x41\xBE\x00\x02\x00\x00\x0F\x11\x00\xF2\x0F\x10\x0D\x00\x00\x00\x00\xF2";
	static const char* NGSMask = "xxxxxxx????xxxxxxxxxxxxx????x";
	
	static const char* UMPOSCGetAccountSig = "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x8B\x10\xFF\x92\x18\x03";
	static const char* UMPOSCGetAccountMask = "xxxxxxxxxx????xxxxxxxxxx";
	typedef UObject* (__fastcall* tUMPOSCGetAccount)(UObject* PrivateOnlineServiceComponent);
	extern tUMPOSCGetAccount native_GetUM1Account;
	// 40 53 48 83 EC 20 48 8B D9 E8 ?? ?? ?? ?? 48 8B C8 48 8B 10 FF 92 18 03

	//static const char* ActorMiniGamePlaySig = "\x40\x53\x48\x83\xEC\x30\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x8B\x03\x48\x8D\x54\x24\x20\x48\x8B\xCB\xC6\x44\x24\x20\x00\x48\xC7\x44\x24\x28\x00\x00\x00\x00\xFF\x90\x08\x07\x00\x00\x48\x8B";
	//static const char* ActorMiniGamePlayMask = "xxxxxxxxxx????xxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	// 40 53 48 83 EC 30 48 8B D9 E8 ?? ?? ?? ?? 48 8B CB E8 ?? ?? ?? ?? 48 8B 03 48 8D 54 24 20 48 8B CB C6 44 24 20 00 48 C7 44 24 28 00 00 00 00 FF 90 08 07 00 00 48 8B

	//static const char* CanInteractSig = "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x83\xB9\x30\x04\x00\x00\x00\x48\x8B\xF2";
	//static const char* CanInteractMask = "xxxxxxxxxxxxxxxxxxxxxxxxx";

	//static const char* NoSpreadSig = "\x48\x8B\x86\xD0\x00\x00\x00\xF3\x0F\x11\xBE\x08";
	//static const char* NoSpreadMask = "xxxxxxxxxxxx";
	//static uint8_t NoSpreadOriginal[8] = { 0xF3, 0x41, 0x0F, 0x5D, 0xC4, 0x0F, 0x28, 0xF8 };
	//static uint8_t NoSpreadCheat[8] = { 0xF3, 0x45, 0x0F, 0x5C, 0xC9, 0x90, 0x90, 0x90 };
	//extern uint8_t* NoSpreadAddress;

	//static const char* NoRecoilSig = "\x84\xC0\x74\x00\x48\x8B\xBD\xF0\x00\x00\x00\x8B\x9D";
	//static const char* NoRecoilMask = "xxx?xxxxxxxxx";
	//extern uint8_t* NoRecoilAddress;

	//static const char* RapidFireSig = "\x72\x00\xF3\x0F\x10\x87\x00\x00\x00\x00\x48";
	//static const char* RapidFireMask = "x?xxxx????x";
	//extern uint8_t* RapidFireAddress;

	//static uint8_t bytes_Recoil[2] = { 0x74, 0x75 };
	//static uint8_t bytes_RapidFire[2] = { 0x72, 0x77 };

	enum Init_Code
	{
		Success,
		FatalError,
		ModuleBase,
		GNames,
		GObjects,
		PostRender
	};

	Init_Code InitSDK(int procid, uintptr_t base, uintptr_t size);

	static ULONG64* GetPostRender(void* vp);
	bool MemoryCompare(const BYTE* bData, const BYTE* bSig, const char* szMask);
	uintptr_t SearchSignature(int procID, uintptr_t base, uintptr_t size, const char* sig, const char* mask);
	uintptr_t SearchForGObjects(uintptr_t base);
	void SearchForGWorld(uintptr_t base, uintptr_t size);
	template<typename T>
	T* FindAlignedValueInProcess(T Value, const std::string& Sectionname = ".data", int32_t Alignment = alignof(T), bool bSearchAllSections = false);
	template<typename T>
	T* FindAlignedValueInProcessInRange(T Value, int32_t Alignment, uintptr_t StartAddress, uint32_t Range);
	bool IsAddressValidGObjects(const uintptr_t Address);
	std::pair<uintptr_t, DWORD> GetSectionByName(uintptr_t ImageBase, const std::string& ReqestedSectionName);
	bool IsBadReadPtr(const void* p);


	//typedef UFunction* (__fastcall* tFindFunctionChecked)(UObject* ThisObject, FName InName, EIncludeSuperFlag IncludeSuper);
	//extern tFindFunctionChecked native_FindFunctionChecked;

	//typedef void (__fastcall* tInitCheatManager)(UObject* ThisObject);
	//extern tInitCheatManager native_InitCheatManager;

	//                      AM1FieldInteractableActorMiniGame*    -V
	//typedef void(__fastcall* tActorMiniGameBeginPlay)(UObject* ThisObject);
	//extern tActorMiniGameBeginPlay native_ActorMiniGameBeginPlay;

	//typedef bool(__fastcall* tInteractableActorCanActivate)(UObject* ThisObject, UObject* InPC);
	//extern tInteractableActorCanActivate native_oInteractableActorCanActivate;
	//bool hook_InteractableActorCanActivate(UObject* ThisObj, UObject* InPC);


	const struct FChunkedFixedUObjectArrayLayout
	{
		const int ObjectsOffset = 0x0;
		const int MaxElementsOffset = 0x10;
		const int NumElementsOffset = 0x14;
		const int MaxChunksOffset = 0x18;
		const int NumChunksOffset = 0x1C;
	};

	// Start 0x28
	class UEngine : public UObject
	{
	public:
		uint8 Pad_ConsoleClass[0xF0];
		//class UFont* SmallFont; // 0x0058(0x0008)
		TSubclassOf<class UConsole> ConsoleClass; // 0x0118(0x0008)
		uint8 Pad_GameViewport[0x8F8];
		class UGameViewportClient* GameViewport; // 0x0A18(0x0008)
	public:
		static class UEngine* GetEngine();

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"Engine">();
		}
		static class UEngine* GetDefaultObj()
		{
			return GetDefaultObjImpl<UEngine>();
		}
	};
	// Start 0x28
	class UWorld final : public UObject
	{
	public:
		uint8 Pad_PersistentLevel[0x8]; // 0x28 8
		class ULevel* PersistentLevel; // 0x30(0x0008) Need
		uint8 Pad_GameState[0x128];	// 0x38 128
		class AGameStateBase* GameState; // 0x0160(0x0008) Need
		uint8 Pad_Levels[0x10]; // 0x168 10
		TArray<class ULevel*> Levels; // 0x0178(0x0010) Need
		uint8  Pad_OwningGameInstance[0x38]; // 0x188 38
		class UGameInstance* OwningGameInstance; // 0x01C0(0x0008) Need
		
	public:
		static class UWorld* GetWorld();

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"World">();
		}
		static class UWorld* GetDefaultObj()
		{
			return GetDefaultObjImpl<UWorld>();
		}
	};

	typedef void(__fastcall* tCreateCheatWidget)(UWorld* World);
	extern tCreateCheatWidget native_CreateCheatWidget;

	// 0x0220 (0x0248 - 0x0028)
	class AActor : public UObject
	{
	public:
		uint8                                         Pad_Children[0x118]; // 0x28 118
		TArray<class AActor*>                         Children; // 0x0140(0x0010) Need
		uint8                                         Pad_AActor_Class[0xF8]; // 0x150 F8

		bool K2_SetActorLocation(const struct FVector& NewLocation, bool bSweep, struct FHitResult* SweepHitResult, bool bTeleport);
		float GetDistanceTo(const class AActor* OtherActor) const;
		struct FVector K2_GetActorLocation() const;
		struct FRotator K2_GetActorRotation() const;
		void GetActorBounds(bool bOnlyCollidingComponents, struct FVector* Origin, struct FVector* BoxExtent, bool bIncludeFromChildActors) const;
	};

	// 0x02C8 (0x02F0 - 0x0028)
	class ULevel final : public UObject
	{
	public:
		uint8  Pad_Actors[0x70]; // 0x28 70
		class TArray<class TFD::AActor*> Actors; // 0x0098(0x0010)
		uint8  Pad_WorldSettings[0x1C8]; // 0xA8 1C8
		class AWorldSettings* WorldSettings; // 0x0270(0x0008)
		
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"Level">();
		}
		static class ULevel* GetDefaultObj()
		{
			return GetDefaultObjImpl<ULevel>();
		}
	};
	

	// Start 0x248
	class AInfo : public AActor
	{
	};

	// 0x0058 (0x02A0 - 0x0248)
	class AGameStateBase : public AInfo
	{
	public:
		uint8 Pad_AGameStateBase_Class[0x58]; // 0x248 58
	};

	// 0x0008 (0x0030 - 0x0028)
	class USubsystem : public UObject
	{
	public:
		uint8 Pad_USubsystem[0x8];
	};

	class UGameInstanceSubsystem : public USubsystem
	{
	};

	// Predefined struct FScriptDelegate
	// 0x0010 (0x0010 - 0x0000)
	struct FScriptDelegate
	{
	public:
		FWeakObjectPtr                                Object;                                            // 0x0000(0x0008)(NOT AUTO-GENERATED PROPERTY)
		FName                                         FunctionName;                                      // 0x0008(0x0008)(NOT AUTO-GENERATED PROPERTY)
	};

	// Predefined struct TDelegate
	// 0x0010 (0x0010 - 0x0000)
	template<typename FunctionSignature>
	class TDelegate
	{
	public:
		struct InvalidUseOfTDelegate                  TemplateParamIsNotAFunctionSignature;              // 0x0000(0x0000)(NOT AUTO-GENERATED PROPERTY)
		uint8                                         Pad_0[0x10];                                       // 0x0000(0x0010)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	// Predefined struct TDelegate<Ret(Args...)>
	// 0x0010 (0x0010 - 0x0000)
	template<typename Ret, typename... Args>
	class TDelegate<Ret(Args...)>
	{
	public:
		FScriptDelegate                               BoundFunction;                                     // 0x0000(0x0010)(NOT AUTO-GENERATED PROPERTY)
	};

	// Predefined struct TMulticastInlineDelegate
	// 0x0010 (0x0010 - 0x0000)
	template<typename FunctionSignature>
	class TMulticastInlineDelegate
	{
	public:
		struct InvalidUseOfTMulticastInlineDelegate   TemplateParamIsNotAFunctionSignature;              // 0x0000(0x0010)(NOT AUTO-GENERATED PROPERTY)
	};

	// Predefined struct TMulticastInlineDelegate<Ret(Args...)>
	// 0x0000 (0x0000 - 0x0000)
	template<typename Ret, typename... Args>
	class TMulticastInlineDelegate<Ret(Args...)>
	{
	public:
		TArray<FScriptDelegate>                       InvocationList;                                    // 0x0000(0x0010)(NOT AUTO-GENERATED PROPERTY)
	};

	struct FSubsystemCollectionBase
	{
		void* __vftable;										// 0x00
		TMap<SDK::UClass*, USubsystem*> SubsystemMap;				// 0x08
		TMap<SDK::UClass*, TArray<USubsystem*>> SubsystemArrayMap;	// 0x58
		SDK::UClass* BaseType;
		SDK::UObject* Outer;
		bool bPopulating;										// 0xB8
		uint8 Pad_FSubsystemCollectionBase[0x7];				// 0xB9
	};

	// 0x0008 (0x0008 - 0x0000)
	struct FDelegateHandle
	{
		uint64 ID;
	};
	// 0x000C (0x000C - 0x0000)
	struct FCachedMaxCapacity
	{
		int CachedCapacity;
		FDelegateHandle StatChangedEventDelegateHandle;
	};

	// 0x0198 (0x01C0 - 0x0028)
	class UGameInstance : public UObject
	{
	public:
		uint8 Pad_LocalPlayers[0x10]; // 0x28 10
		TArray<class ULocalPlayer*>  LocalPlayers; // 0x0038(0x0010)
		uint8 Pad_UGameInstance_Class[0xB8]; // 0x48
		struct FSubsystemCollectionBase SubsystemCollection; // 0x100 (0x1C0)
	};

	static_assert(sizeof(FSubsystemCollectionBase) == 0xC0, "Wrong size");

	// 0x0020 (0x0048 - 0x0028)
	class UPlayer : public UObject
	{
	public:
		uint8 Pad_PlayerController[0x8]; // 0x28 8
		class APlayerController* PlayerController; // 0x0030(0x0008)
		uint8 Pad_UPlayer_Class[0x10]; // 0x38 10

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"Player">();
		}
		static class UPlayer* GetDefaultObj()
		{
			return GetDefaultObjImpl<UPlayer>();
		}
	};

	// 0x0010 (0x0038 - 0x0028)
	class UScriptViewportClient : public UObject
	{
	public:
		uint8 Pad_UScriptViewportClient_Class[0x10]; // 0x0028(0x0010)
	};
	// 0x0378 (0x03B0 - 0x0038)
	class UGameViewportClient : public UScriptViewportClient
	{
	public:
		uint8 Pad_ViewportConsole[0x8]; // 0x0038(0x0008)
		class UConsole* ViewportConsole; // 0x0040(0x0008)
	};
	// 0x0230 (0x0278 - 0x0048)
	class ULocalPlayer : public UPlayer
	{
	public:
		uint8 Pad_ViewportClient[0x30]; // 0x48 30
		class UGameViewportClient* ViewportClient; // 0x0078(0x0008)
		uint8 Pad_ULocalPlayer_Class[0x1F8]; // 0x80 1F8
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"LocalPlayer">();
		}
	};
	// 0x0118 (0x0360 - 0x0248)
	class APlayerState : public AInfo
	{
	public:
		uint8 Pad_APlayerState_Class[0x118];
	};
	// 0x0080 (0x03E0 - 0x0360)
	class AM1PlayerState final : public APlayerState
	{
	public:
		class UM1MissionControlComponent* MissionControlComponent; // 0x0360(0x0008)

	public:
		void ServerCheatSetIsCreator(bool InbCreator);

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1PlayerState">();
		}
	};

	// 0x0088 (0x02D0 - 0x0248)
	class AController : public AActor
	{
	public:
		uint8 Pad_PlayerState[0x8]; // 0x0248(0x0008)
		class APlayerState* PlayerState; // 0x0250(0x0008)
		uint8 Pad_Pawn[0x30]; // 0x258
		class APawn* Pawn; // 0x0288(0x0008)
		uint8 Pad_Character[0x8]; // 0x290 8
		class ACharacter* Character; // 0x0298(0x0008)
		uint8 Pad_AController_Class[0x30]; // 0x2A0 30

		void SetControlRotation(const struct FRotator& NewRotation);
		bool LineOfSightTo(const class AActor* Other, const struct FVector& ViewPoint, bool bAlternateChecks) const;

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"Controller">();
		}
		static class AController* GetDefaultObj()
		{
			return GetDefaultObjImpl<AController>();
		}
	};
	// 0x3808 (0x3A50 - 0x0248)
	class APlayerCameraManager : public AActor
	{
	public:
		struct FVector GetCameraLocation() const;
		struct FRotator GetCameraRotation() const;

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"PlayerCameraManager">();
		}
	};
	// 0x04B8 (0x0788 - 0x02D0)
	class APlayerController : public AController
	{
	public:
		uint8 Pad_Player[0x8]; // 0x2D0 8
		class UPlayer* Player; // 0x02D8(0x0008)
		uint8 Pad_PlayerCameraManager[0x10]; // 0x2E0 10
		class APlayerCameraManager* PlayerCameraManager; // 0x02F0(0x0008)
		uint8 Pad_CheatManager[0x78]; // 0x2F8
		class UCheatManager* CheatManager;// 0x0370(0x0008)
		TSubclassOf<class UCheatManager> CheatClass;// 0x0378(0x0008)
		uint8 Pad_APlayerController_Class[0x408]; // 0x380

		

		bool GetMousePosition(float* LocationX, float* LocationY) const;
		void EnableCheats();

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"PlayerController">();
		}
	};

	struct alignas(0x08) FKey final
	{
	public:
		class FName                                   KeyName;                                           // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_8[0x10];                                       // 0x0008(0x0010)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct FVector_NetQuantize : public FVector
	{
		using FVector::FVector;
	};

	struct FVector_NetQuantizeNormal : public FVector
	{
		using FVector::FVector;
	};

	struct FActorInstanceHandle final
	{
	public:
		TWeakObjectPtr<class AActor>                  Actor;
		uint8                                         Pad_99[0x10];
	};

	struct FHitResult final
	{
	public:
		int32                                         FaceIndex;
		float                                         Time;
		float                                         Distance;
		struct FVector_NetQuantize                    Location;
		struct FVector_NetQuantize                    ImpactPoint;
		struct FVector_NetQuantizeNormal              Normal;
		struct FVector_NetQuantizeNormal              ImpactNormal;
		struct FVector_NetQuantize                    TraceStart;
		struct FVector_NetQuantize                    TraceEnd;
		float                                         PenetrationDepth;
		int32                                         MyItem;
		int32                                         Item;
		uint8                                         ElementIndex;
		uint8                                         bBlockingHit : 1;
		uint8                                         bStartPenetrating : 1;
		uint8                                         Pad_9A[0x2];
		TWeakObjectPtr<class UPhysicalMaterial>       PhysMaterial;
		struct FActorInstanceHandle                   HitObjectHandle;
		TWeakObjectPtr<class UPrimitiveComponent>     Component;
		class FName                                   BoneName;
		class FName                                   MyBoneName;
	};

	class UInputSettings final : public UObject
	{
	public:
		uint8 Pad_ConsoleKeys[0x118];
		TArray<struct FKey> ConsoleKeys; // 0x0140(0x0010)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"InputSettings">();
		}
		static class UInputSettings* GetDefaultObj()
		{
			return GetDefaultObjImpl<UInputSettings>();
		}
	};

	class UCheatManagerExtension : public UObject
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"CheatManagerExtension">();
		}
	};

	// 0x0060 (0x0088 - 0x0028)
	class ADebugCameraController : public APlayerController
	{
	public:
		uint8 Pad_ADebugCameraController[0x60];                                     // 0x0868(0x0038)(Fixing Struct Size After Last Property [ Dumper-7 ])

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"DebugCameraController">();
		}
	};


	// 0x0060 (0x0088 - 0x0028)
	class UCheatManager : public UObject
	{
	public:
		//uint8 Pad_UCheatManager_Class[0x60];
		class ADebugCameraController* DebugCameraControllerRef;                          // 0x0028(0x0008)(ZeroConstructor, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		TSubclassOf<class ADebugCameraController>     DebugCameraControllerClass;                        // 0x0030(0x0008)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_38[0x40];                                      // 0x0038(0x0040)(Fixing Size After Last Property [ Dumper-7 ])
		TArray<class UCheatManagerExtension*>         CheatManagerExtensions;                            // 0x0078(0x0010)(ZeroConstructor, Transient, Protected, UObjectWrapper, NativeAccessSpecifierProtected)

	/*public:
		void BugIt(const class FString& ScreenShotDescription);
		void BugItGo(float X, float Y, float Z, float Pitch, float Yaw, float Roll);
		void BugItStringCreator(const struct FVector& ViewLocation, const struct FRotator& ViewRotation, class FString* GoString, class FString* LocString);
		void ChangeSize(float F);
		void CheatScript(const class FString& ScriptName);
		void DamageTarget(float DamageAmount);
		void DebugCapsuleSweep();
		void DebugCapsuleSweepCapture();
		void DebugCapsuleSweepChannel(ECollisionChannel Channel);
		void DebugCapsuleSweepClear();
		void DebugCapsuleSweepComplex(bool bTraceComplex);
		void DebugCapsuleSweepPawn();
		void DebugCapsuleSweepSize(float HalfHeight, float Radius);
		void DestroyAll(TSubclassOf<class AActor> AClass);
		void DestroyAllPawnsExceptTarget();
		void DestroyPawns(TSubclassOf<class APawn> AClass);
		void DestroyServerStatReplicator();
		void DestroyTarget();
		void DisableDebugCamera();
		void DumpChatState();
		void DumpOnlineSessionState();
		void DumpPartyState();
		void DumpVoiceMutingState();
		void EnableDebugCamera();
		void FlushLog();
		void Fly();
		void FreezeFrame(float Delay);
		void Ghost();*/
		//void God();
		/*void InvertMouse();
		void LogLoc();
		void OnlyLoadLevel(class FName PackageName);
		void OnPlayerEndPlayed(class AActor* Player, EEndPlayReason EndPlayReason);
		void PlayersOnly();
		void ReceiveEndPlay();
		void ReceiveInitCheatManager();
		void ServerToggleAILogging();
		void SetMouseSensitivityToDefault();
		void SetWorldOrigin();
		void Slomo(float NewTimeDilation);
		void SpawnServerStatReplicator();
		void StreamLevelIn(class FName PackageName);
		void StreamLevelOut(class FName PackageName);
		void Summon(const class FString& ClassName);
		void Teleport();
		void TestCollisionDistance();
		void ToggleAILogging();*/
		//void ToggleDebugCamera();
		/*void ToggleServerStatReplicatorClientOverwrite();
		void ToggleServerStatReplicatorUpdateStatNet();
		void UpdateSafeArea();
		void ViewActor(class FName ActorName);
		void ViewClass(TSubclassOf<class AActor> DesiredClass);
		void ViewPlayer(const class FString& S);
		void ViewSelf();
		void Walk();*/

		//class APlayerController* GetPlayerController() const;

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"CheatManager">();
		}
	};


	// 0x0070 (0x02B8 - 0x0248)
	class APawn : public AActor
	{
	public:
		uint8 Pad_APawn_Class[0x70];
	};
	// 0x0248 (0x0500 - 0x02B8)
#pragma pack(push, 0x1)
	class alignas(0x10) ACharacter : public APawn
	{
	public:
		class USkeletalMeshComponent* Mesh; // 0x02B8(0x0008)
		uint8 Pad_ACharacter_Class[0x238]; // 0x2C0 240
	};
#pragma pack(pop)

	static_assert(offsetof(ACharacter, Mesh) == 0x2B8, "Bad alignment");
	static_assert(offsetof(ACharacter, Pad_ACharacter_Class) == 0x2C0, "Bad alignment");

	// 0x0080 (0x00A8 - 0x0028)
	class UActorComponent : public UObject
	{
	public:
		uint8 Pad_UActorComponent_Class[0x80];
		void Deactivate();
		bool IsActive() const;

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"ActorComponent">();
		}
	};
	// 0x0148 (0x01F0 - 0x00A8)
	class alignas(0x10) USceneComponent : public UActorComponent
	{
	public:
		uint8                                         Pad_382[0x148];
	public:
		struct FTransform K2_GetComponentToWorld() const;
	};
	// 0x0280 (0x0470 - 0x01F0)
	class UPrimitiveComponent : public USceneComponent
	{
	public:
		uint8 Pad_UPrimitiveComponent_Class[0x280];
	};
	// 0x0040 (0x04B0 - 0x0470)
#pragma pack(push, 0x1)
	class alignas(0x10) UMeshComponent : public UPrimitiveComponent
	{
	public:
		uint8 Pad_UMeshComponent_Class[0x40];
	};
#pragma pack(pop)

	// 0x00A0 (0x00C8 - 0x0028)
	class UStreamableRenderAsset : public UObject
	{
	public:
		uint8 Pad_28[0xA0];
	};

	// 0x0008 (0x00D0 - 0x00C8)
	class USkinnedAsset : public UStreamableRenderAsset
	{
	public:
		uint8  Pad_C8[0x8];
	};

	struct FBoxSphereBounds final
	{
	public:
		struct FVector                                Origin;                                            // 0x0000(0x000C)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector                                BoxExtent;                                         // 0x000C(0x000C)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         SphereRadius;                                      // 0x0018(0x0004)(Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// 0x03C8 (0x0498 - 0x00D0)
	class USkeletalMesh : public USkinnedAsset
	{
	public:
		struct FBoxSphereBounds GetBounds() const;
	};

	// 0x02E0 (0x0790 - 0x04B0)
	class USkinnedMeshComponent : public UMeshComponent
	{
	public:
		uint8 Pad_Asset[0x8];
		class USkeletalMesh* SkeletalMesh;
		uint8 Pad_BoneArray[0x58]; // 0x58 <-- In Dumped SDK the class starts at 0x4B0 but the first field is listed as starting at 0x4A8? 
		TArray<FTransform> BoneArray; // 0x0508(0x0010)
		uint8 Pad_USkinnedMeshComponent_Class[0x268]; // 0x278 0x518
		class FName GetBoneName(int32 BoneIndex) const;
		int32 GetNumBones() const;
	};
	// 0x0690 (0x0E20 - 0x0790)
	class USkeletalMeshComponent : public USkinnedMeshComponent
	{
	public:
		class USkeletalMesh* SkeletalMeshAsset;
	};
	// 0x0000 (0x0028 - 0x0028)
	class UBlueprintFunctionLibrary : public UObject
	{
	};
	// 0x0000 (0x0028 - 0x0028)
	class UGameplayStatics final : public UBlueprintFunctionLibrary
	{
	public:
		static class APlayerController* GetPlayerController(const class UObject* WorldContextObject, int32 PlayerIndex);
		static bool ProjectWorldToScreen(class APlayerController* Player, const struct FVector& WorldPosition, struct FVector2D* ScreenPosition, bool bPlayerViewportRelative);
		static double GetWorldDeltaSeconds(const class UObject* WorldContextObject);
		static class UObject* SpawnObject(TSubclassOf<class UObject> ObjectClass, class UObject* Outer_0);


	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"GameplayStatics">();
		}
		static class UGameplayStatics* GetDefaultObj()
		{
			return GetDefaultObjImpl<UGameplayStatics>();
		}
	};
	// 0x0000 (0x0028 - 0x0028)
	class UKismetMathLibrary final : public UBlueprintFunctionLibrary
	{
	public:
		static struct FTransform Conv_MatrixToTransform(const struct FMatrix& InMatrix);
		static struct FMatrix Conv_TransformToMatrix(const struct FTransform& Transform);
		static struct FRotator FindLookAtRotation(const struct FVector& Start, const struct FVector& Target);
		static struct FMatrix Multiply_MatrixMatrix(const struct FMatrix& A, const struct FMatrix& B);
		static struct FRotator RInterpTo(const struct FRotator& Current, const struct FRotator& Target, float DeltaTime, float InterpSpeed);
		static double Distance2D(const struct FVector2D& V1, const struct FVector2D& V2);

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"KismetMathLibrary">();
		}
		static class UKismetMathLibrary* GetDefaultObj()
		{
			return GetDefaultObjImpl<UKismetMathLibrary>();
		}
	};
	// 0x02A8 (0x02D0 - 0x0028)
	class UCanvas final : public UObject
	{
	public:
		float                                         OrgX; // 0x0028(0x0004)
		float                                         OrgY; // 0x002C(0x0004)
		float                                         ClipX; // 0x0030(0x0004)
		float                                         ClipY; // 0x0034(0x0004)
		uint8                                         Pad_SizeX[0x8]; // 0x38
		int32                                         SizeX; // 0x0040(0x0004)
		int32                                         SizeY; // 0x0044(0x0004)
		uint8                                         Pad_UCanvas_Class[0x288]; // 0x48

		void K2_DrawLine(const struct FVector2D& ScreenPositionA, const struct FVector2D& ScreenPositionB, float Thickness, const struct FLinearColor& RenderColor);
		void K2_DrawText(class UFont* RenderFont, const class FString& RenderText, const struct FVector2D& ScreenPosition, const struct FVector2D& Scale, const struct FLinearColor& RenderColor, float Kerning, const struct FLinearColor& ShadowColor, const struct FVector2D& ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, const struct FLinearColor& OutlineColor);

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"Canvas">();
		}
		static class UCanvas* GetDefaultObj()
		{
			return GetDefaultObjImpl<UCanvas>();
		}
	};
	// 0x01E0 (0x0428 - 0x0248)
	class AWorldSettings : public AInfo
	{
	public:
		uint8 Pad_TimeDilation[0x130]; // 0x248
		float TimeDilation; // 0x0378(0x0004)
	};

	class UFont final : public UObject
	{
	public:
		uint8                                         Pad_ADE[0x8]; // 0x0028(0x0008)
	};

	// Class GameplayTasks.GameplayTasksComponent
	// 0x0078 (0x0120 - 0x00A8)
	class UGameplayTasksComponent : public UActorComponent
	{
	public:
		uint8                                         Pad_A8[0x14];                                      // 0x00A8(0x0014)(Fixing Size After Last Property [ Dumper-7 ])
		uint8                                         BitPad_BC_0 : 1;                                   // 0x00BC(0x0001)(Fixing Bit-Field Size Between Bits [ Dumper-7 ])
		uint8                                         bIsNetDirty : 1;                                   // 0x00BC(0x0001)(BitIndex: 0x01, PropSize: 0x0001 (NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic))
		uint8                                         Pad_BD[0x3];                                       // 0x00BD(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
		TArray<class UGameplayTask*>                  TaskPriorityQueue;                                 // 0x00C0(0x0010)(ZeroConstructor, Protected, UObjectWrapper, NativeAccessSpecifierProtected)
		uint8                                         Pad_D0[0x10];                                      // 0x00D0(0x0010)(Fixing Size After Last Property [ Dumper-7 ])
		TArray<class UGameplayTask*>                  TickingTasks;                                      // 0x00E0(0x0010)(ZeroConstructor, Protected, UObjectWrapper, NativeAccessSpecifierProtected)
		TArray<class UGameplayTask*>                  KnownTasks;                                        // 0x00F0(0x0010)(ZeroConstructor, Transient, Protected, UObjectWrapper, NativeAccessSpecifierProtected)
		TMulticastInlineDelegate<void(const struct FGameplayResourceSet& NewlyClaimed, const struct FGameplayResourceSet& FreshlyReleased)> OnClaimedResourcesChange;                          // 0x0100(0x0010)(BlueprintVisible, ZeroConstructor, InstancedReference, NativeAccessSpecifierPublic)
		TArray<class UGameplayTask*>                  SimulatedTasks;                                    // 0x0110(0x0010)(Net, ZeroConstructor, RepNotify, UObjectWrapper, NativeAccessSpecifierPrivate)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"GameplayTasksComponent">();
		}
		static class UGameplayTasksComponent* GetDefaultObj()
		{
			return GetDefaultObjImpl<UGameplayTasksComponent>();
		}
	};



	enum class EM1OnlineServiceConnectionState : uint8
	{
		NotLoggedIn = 0,
		RequestedLogIn = 1,
		ReceivedLogInButNotJoined = 2,
		RequestedJoinGame = 3,
		InTransitionMapToJoinGame = 4,
		ReceivedJoinGame = 5,
		JoinedDedicatedServer = 6,
		ReceivedLoginIdFromDedicatedServer = 7,
		RequestedPlayerCharacterInfoByLoginId = 8,
		SentPlayerPawnToClient = 9,
		ReceivedPawnAndOkay = 10,
		EM1OnlineServiceConnectionState_MAX = 11,
	};
	// 0x0108 (0x02C8 - 0x01C0)
	class UM1GameInstance : public UGameInstance
	{
	public:
		EM1OnlineServiceConnectionState ConnectionState; // 0x01C0(0x0001)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1GameInstance">();
		}
		static class UM1GameInstance* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1GameInstance>();
		}
	};

	enum class EM1MouseVisibilityFlag : uint8
	{
		None = 0,
		VirtualCursor = 1,
		GameContentSelector = 2,
		Cinematic = 4,
		RingMenuOnGamepad = 8,
		LoginMenuWaitInput = 16,
		Navigation = 32,
		UICaptureMode = 64,
		Visible = 127,
		EM1MouseVisibilityFlag_MAX = 128,
	};
	DEFINE_ENUM_FLAG_OPERATORS(EM1MouseVisibilityFlag)

	class UM1InputSubsystem final : public UObject
	{
	public:
		uint8 Pad_28[0x30];                            // 0x0028(0x0030)(Fixing Size After Last Property [ Dumper-7 ])
		uint8 CurrentInputMethod;                      // 0x0058(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8 InputControlMode;                        // 0x0059(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8 Pad_5A[0x76];                            // 0x005A(0x0076)(Fixing Size After Last Property [ Dumper-7 ])
		bool bBlockUpdateCursors;                      // 0x00D0(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		bool bBlockNavigationStart;                    // 0x00D1(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		EM1MouseVisibilityFlag MouseVisibilityFlag;    // 0x00D2(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8 MouseCursorStateFlag;                    // 0x00D3(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8 Pad_D4[0x4];                             // 0x00D4(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1InputSubsystem">();
		}
	};

	struct FM1TemplateId final
	{
	public:
		int32 ID; // 0x0000(0x0004)
	};

	// 0x0188 (0x0910 - 0x0788)
	class AM1PlayerController : public APlayerController
	{
	public:
		uint8 Pad_ActorManager[0x78]; // 0x788
		class UM1ActorManagerSubsystem* ActorManager_Subsystem; // 0x0800(0x0008)
		uint8 Pad_PrivateOnlineServComp[0x10];
		class UM1PrivateOnlineServiceComponent* PrivateOnlineServiceComponent; // 0x0818(0x0008)
		uint8 Pad_AM1PlayerController_Class[0xF0]; // 0x820

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1PlayerController">();
		}
		static class AM1PlayerController* GetDefaultObj()
		{
			return GetDefaultObjImpl<AM1PlayerController>();
		}
	};

	enum class EM1PresetSlotType : uint8
	{
		Ranged_0 = 0,
		Ranged_1 = 1,
		Ranged_2 = 2,
		Reactor = 9,
		Necklace = 10,
		Earring = 11,
		Ring = 12,
		Bracelet = 13,
		Face = 20,
		Hair = 21,
		Fellow = 70,
		PlayerCharacter = 80,
		AltWeapon = 90,
		PickupWeapon = 91,
		Max = 99,
	};

	struct FM1PresetSlot final
	{
	public:
		int32                                         PresetIndex;                                       // 0x0000(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_4[0x4];                                        // 0x0004(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
		class FString                                 PresetName;                                        // 0x0008(0x0010)(ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_5[0x50];
		//TMap<EM1PresetSlotType, struct FM1PresetItem> PresetItemBySlotType;                              // 0x0018(0x0050)(NativeAccessSpecifierPublic)
	};

	class UM1AccountPreset final : public UObject
	{
	public:
		TMap<int32, struct FM1PresetSlot>             PresetSlotByIndex;                                 // 0x0028(0x0050)(NativeAccessSpecifierPrivate)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1AccountPreset">();
		}
	};

	class UM1AccountCharacter : public UObject
	{
	public:
		__int64 CharacterUniqueId;
		FM1TemplateId CharacterTid;
		FString Name;
		__int64 CombatPower;
		FDateTime CreatedTime;
		__int64 PlayTime;
	};

	class UM1Account : public UObject
	{
	public:
		__int64 AccountUid;
		FString LoginID;
		FString SimpleID;
		FString Token;
		__int64 NPSN;
		FString ConnectedGameServerIP;
		int ConnectedGameServerPort;
		int ConnectedGameServerIndex;
		FString ConnectedGameServerVersion;
		FString CountryName;
		FString DeviceName;
		FString OsType;
		FString OsName;
		FString ServiceId;
		FString RegionId;
		FString FieldSessionId;
		byte LoginPlatformType;
		FString LoginPlatformOnlineId;
		__int64 LoginPlatformUid;
		bool bCreator;
		bool bJoinedGame;
		bool bJustFinishedPrologue;
		TMap<FM1TemplateId, UM1AccountCharacter*> AllCharacters;
		UM1AccountCharacter* SelectedCharacter;
		__int64 ExtendedCharacterSlotCount; // 0x180
		uint8 Pad_UM1AccountPreset[0x170];	// 0x188
		UM1AccountPreset* Preset; // 0x2F8
	};

	// 0x0020 (0x00C8 - 0x00A8)
	class UM1ActorComponent : public UActorComponent
	{
	public:
		uint8 Pad_UM1ActorComp_Class[0x20];
	};

	class UM1PrivateOnlineSubService : public UObject
	{
	public:
		uint8                                         Pad_28[0x10];                                      // 0x0028(0x0010)(Fixing Size After Last Property [ Dumper-7 ])
		bool                                          bIsReady;                                          // 0x0038(0x0001)(Net, ZeroConstructor, IsPlainOldData, RepNotify, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_39[0x7];                                       // 0x0039(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	// 0x0120 (0x0148 - 0x0028)
	class UM1ResearchSystem final : public UObject
	{
	public:
		uint8                                         Pad_BookmarkResearchTids[0xA0];                    // 0x0028
		TArray<struct FM1TemplateId>                  BookmarkResearchTids;                              // 0x00C8(0x0010)(ZeroConstructor, Transient, NativeAccessSpecifierPrivate)
		uint8                                         Pad_UM1ResearchSystem[0x70];                       // 0x00D8

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1ResearchSystem">();
		}
	};

	// 0x0018 (0x0040 - 0x0028)
	class UM1MissionTaskService : public UObject
	{
	public:
		uint8                                         Pad_28[0x11];                                      // 0x0028
		bool                                          bJoined;                                           // 0x0039(0x0001)(Net, ZeroConstructor, IsPlainOldData, RepNotify, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		uint8                                         Pad_3A[0x6];                                       // 0x003A
	};

	class UM1PrivateOnlineServicePreset final : public UM1PrivateOnlineSubService
	{
	public:
		void ServerRequestApplyPreset(int32 InPresetIndex);
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1PrivateOnlineServicePreset">();
		}
	};

	// 0x08D8 (0x09A0 - 0x00C8)
	class UM1PrivateOnlineServiceComponent final : public UM1ActorComponent
	{
	public:
		TArray<UM1PrivateOnlineSubService*> SubServices;			// 0xC8
		uint8 Pad_UM1PrivateOnlineServiceComponent_Class[0x8C8];	// 0xD8
		TWeakObjectPtr<class UM1Account> CachedAccount; // 0x1F8
		void ServerChangePlayer(const struct FM1TemplateId& InCharacterTid);
	};

	// 0x0060 (0x00A0 - 0x0040)
	class UM1PrivateOnlineServiceResearch final : public UM1PrivateOnlineSubService
	{
	public:
		uint8                                         Pad_UM1PrivateOnlineServiceResearch[0x60];                                      // 0x0040(0x0060)(Fixing Struct Size After Last Property [ Dumper-7 ])
	public:
		void ServerRequestStartResearch(const struct FM1TemplateId& InResearchTemplateId, int32 InRepeatCount);
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1PrivateOnlineServiceResearch">();
		}
	};

	// 0x0080 (0x0128 - 0x00A8)
	class UM1HeartbeatTesterComponent final : public UActorComponent
	{
	public:
		uint8 Pad_2F4E[0x80];// 0x00A8(0x0080)
	};


	// 0x04A8 (0x0DB8 - 0x0910)
	class AM1PlayerControllerInGame : public AM1PlayerController
	{
	public:
		uint8 PadHeartbeat[0x208]; //0x910
		class UM1HeartbeatTesterComponent* HeartbeatTesterComponent; // 0x0B18(0x0008)
		void ServerRequestFieldObjectDropItems(class AM1FieldInteractableActor* InActor);
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1PlayerControllerInGame">();
		}
	};
	// 0x0150 (0x0180 - 0x0030)
	class UM1ActorManagerSubsystem final : public UGameInstanceSubsystem
	{
	public:
		TArray<class AM1Character*> Characters; // 0x0030(0x0010)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1ActorManagerSubsystem">();
		}
	};

	class UM1SpeedHackDetectorSubSystem final : public UGameInstanceSubsystem
	{
	public:
		uint8                                         Pad_30[0x8];                                       // 0x0030(0x0008)(Fixing Size After Last Property [ Dumper-7 ])
		int32                                         ServerHitchThreshholdCount;                        // 0x0038(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         ConsumeRoundsTimeDiscrepancyAllowanceRate;         // 0x003C(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         ClientTimeDiscrepancyAllowanceRate;                // 0x0040(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32                                         ConsumedRoundsTimeVerifierRequiredNums;            // 0x0044(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          bUseDetect;                                        // 0x0048(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_49[0x3];                                       // 0x0049(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
		float                                         MaxFocusRestoreTime;                               // 0x004C(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		int32                                         MaxFalsePositive;                                  // 0x0050(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		float                                         ResetFalsePositiveTime;                            // 0x0054(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		int32                                         CurrFalsePositive;                                 // 0x0058(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		float                                         CurrFalsePositiveTime;                             // 0x005C(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		float                                         CurrFocusRestoreTime;                              // 0x0060(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		bool                                          bFocusRestore;                                     // 0x0064(0x0001)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		bool                                          bTemperedTime;                                     // 0x0065(0x0001)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		bool                                          bDetectedHookingTimer;                             // 0x0066(0x0001)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_67[0x1];                                       // 0x0067(0x0001)(Fixing Size After Last Property [ Dumper-7 ])
		float                                         SumOfDeltaTime;                                    // 0x0068(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		float                                         LastTimeSpanToSeconds;                             // 0x006C(0x0004)(BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)

	//public:
	//	void ServerFindSuspectedSpeedHackUser();

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1SpeedHackDetectorSubSystem">();
		}
	};

	// 0x0118 (0x0140 - 0x0028)
	class UM1CharacterAttribute : public UObject
	{
	public:
		uint8 UM1CharacterAttribute_Class[0x118];

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1CharacterAttribute">();
		}
	};

	// 0x00D8 (0x0218 - 0x0140)
	class UM1MonsterAttribute : public UM1CharacterAttribute
	{
	public:
		uint8 UM1MonsterAttribute_Class[0xD8];
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1MonsterAttribute">();
		}
	};

	// 0x0760 (0x0C60 - 0x0500)
	class AM1Character : public ACharacter
	{
	public:
		uint8 Pad_InfoWidget[0x348]; // 0x4F8
		class UM1CharacterInfoWidgetComponent* InfoWidgetComponent; // 0x0840(0x0008) Need this
		uint8 Pad_StatComponent[0x18]; // 0x848
		class UM1StatComponent* StatComponent; // 0x0860(0x0008)
		class UM1AbilityComponent* AbilityComponent;                                  // 0x0868(0x0008)(ExportObject, ZeroConstructor, InstancedReference, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		uint8 Pad_CharacterAttribute[0x12]; // 0x870
		class UM1CharacterAttribute* CharacterAttribute;// 0x0888(0x0008)
		uint8 Pad_CharacterId[0x210]; // 0x890
		struct FM1TemplateId CharacterId; // 0x0AA0(0x0004) Need this
		uint8 Pad_AM1CharClass[0x1BC]; // 0xAA4

		bool IsDead() const;

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1Character">();
		}
	};
	// These show as errors but are correct when compiled
	//static_assert(offsetof(AM1Character, Pad_InfoWidget) == 0x4F8, "Bad alignment");
	//static_assert(offsetof(AM1Character, InfoWidgetComponent) == 0x7C8, "Bad alignment");
	//static_assert(offsetof(AM1Character, CharacterId) == 0x9F0, "Bad alignment");

	// 0x0770 (0x13D0 - 0x0C60)
	class AM1Player : public AM1Character
	{
	public:
		uint8 Pad_TeleportHandler[0x2B0]; // 0xC60
		class UM1TeleportHandlerComponent* TeleportHandler; // 0x0F10(0x0008)
		uint8 Pad_WeaponSlotControl[0x10]; // 0xF18
		class UM1WeaponSlotControlComponent* WeaponSlotControl; // 0x0F28(0x0008) Need this
		uint8 Pad_RoundsComponent[0x38];												// 0x0F30
		class UM1PlayerRoundsComponent* RoundsComponent;                                // 0x0F68(0x0008)(ExportObject, ZeroConstructor, Transient, InstancedReference, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8 Pad_PlayerName[0x48];														// 0x0F70
		class AM1WireCaster* WireCaster;                                        // 0x0FB0(0x0008)(Net, ZeroConstructor, RepNotify, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		class FString PlayerName; // 0xFB8
		bool                                          bInAir;                                            // 0x0FF6(0x0001)(Net, ZeroConstructor, IsPlainOldData, RepNotify, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
	public:
		bool IsInAir() const;
		bool IsMovingByWire() const;
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1Player">();
		}
	};
	// These show as errors but are correct when compiled
	//static_assert(offsetof(AM1Player, Pad_WeaponSlot) == 0xBB0, "Bad alignment");
	//static_assert(offsetof(AM1Player, WeaponSlotControl) == 0xDF8, "Bad alignment");
	//static_assert(offsetof(AM1Player, PlayerName) == 0xE90, "Bad alignment");
	//static_assert(offsetof(AM1Player, bPlayerInputEnabled) == 0xEE2, "Bad alignment");

	// 0x0220 (0x0E80 - 0x0C60)
#pragma pack(push, 0x1)
	class alignas(0x10) AM1Monster : public AM1Character
	{
	public:
		//uint8 Pad_AM1Monster_Class[0x200]; // 0xCC0
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1Monster">();
		}
	};
#pragma pack(pop)



	class UDataAsset : public UObject
	{
	public:
		TSubclassOf<class UDataAsset> NativeClass; // 0x0028(0x0008)
	};

	// 0x0068 (0x0098 - 0x0030)
	class UMissionGraph : public UDataAsset
	{
	public:
		class FString Name_0; // 0x0030(0x0010)
		uint8 Pad_91[0x58]; // 0x0091(0x0007)
	};

	enum class EM1MissionSubType : uint8
	{
		Default = 0,
		Destruction = 1,
		Defense = 2,
		Supply = 3,
		Collection = 4,
		Assasination = 5,
		Extermination = 6,
		Occupation = 7,
		WaveDefense = 8,
		ContinuousSurvival = 9,
		DataCollection = 10,
		Explosion = 11,
		Steal = 12,
		Escort = 13,
		MultiCollection = 14,
		MultiAssasination = 15,
		VoidFragment = 16,
		VoidFusion = 17,
		DestructionVulgusPost = 18,
		EM1MissionSubType_MAX = 19,
	};

	enum class EM1MissionTargetState : uint8
	{
		Default = 0,
		BeingSpawned = 1,
		Spawned = 2,
		Destructed = 3,
		BeingDespawned = 4,
		Interaction = 5,
		Idle = 6,
		Respawn = 7,
		Deactivated = 8,
		EM1MissionTargetState_MAX = 9,
	};

	enum class EM1MissionActorState : uint8
	{
		None = 0,
		Activated = 1,
		Deactivated = 2,
		EM1MissionActorState_MAX = 3,
	};

	enum class EM1TaskEventActorRunResult : uint8
	{
		None = 0,
		Default = 1,
		Failed = 2,
		Succeeded = 3,
		EM1TaskEventActorRunResult_MAX = 4,
	};

	enum class EM1EventActorState : uint8
	{
		Default = 0,
		Activated = 1,
		Running = 2,
		Completed = 3,
		Deactivated = 4,
		EM1EventActorState_MAX = 5,
	};

	// NumValues: 0x0006
	enum class EM1RoundsType : uint8
	{
		None = 0,
		GeneralRounds = 1,
		EnhancedRounds = 2,
		ImpactRounds = 3,
		HighpowerRounds = 4,
		EM1RoundsType_MAX = 5,
	};

	// 0x0210 (0x0458 - 0x0248)
	class AM1TaskEventActor : public AActor
	{
	public:
		uint8 Pad_AM1TaskEventActor_Class[0x210]; // 0x03D0(0x0018)
	public:
		//void Activate(bool InActivated);
		//void BP_EventActorState(EM1EventActorState InState);
		void BP_RunResult(EM1TaskEventActorRunResult InRunResult);
		void TaskActorStateChanged(EM1EventActorState InState);
	};

	// 0x0388 (0x07E0 - 0x0458)
#pragma pack(push, 0x1)
	class alignas(0x10) AM1MissionTaskActor : public AM1TaskEventActor
	{
	public:
		uint8                                         Pad_MissionTask[0x108];							 // 0x0458
		class UM1MissionTask* MissionTask;                                       // 0x0560(0x0008)(Edit, ExportObject, ZeroConstructor, DisableEditOnTemplate, EditConst, InstancedReference, NoDestructor, PersistentInstance, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		int32                                         TaskIndex;                                         // 0x0568(0x0004)(Edit, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_WayPoints[0x7C];								 // 0x056C
		TArray<class AM1MissionTaskMoveWayPoint*>     WayPoints;                                         // 0x05E8(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPrivate)
		uint8                                         Pad_AM1MissionTaskActor[0x1E8];				     // 0x05F8
	};
#pragma pack(pop)
	struct FM1MissionTaskData final
	{
	public:
		class FName Name; // 0x0000(0x0008)
		class UM1MissionTask* Task; // 0x0008(0x0008)
		TSubclassOf<class AM1MissionTaskActor> TaskActorClass; // 0x0010(0x0008)
	};
	// 0x0150 (0x01E8 - 0x0098)
	class UM1DataMission final : public UMissionGraph
	{
	public:
		uint8 Pad_DataRowName[0x20];
		class FName MissionDataRowName; // 0x00B8(0x0008)
		EM1MissionSubType MissionSubType; // 0x00C0(0x0001)
		uint8 Pad_Tasks[0xF]; // 0xC1
		TArray<struct FM1MissionTaskData> Tasks; // 0x00D0(0x0010)
	};

	struct FM1MissionTaskLink final
	{
	public:
		class FName                                   TaskName;                                          // 0x0000(0x0008)(Edit, ZeroConstructor, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		int32                                         LinkIndex;                                         // 0x0008(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_C[0x4];                                        // 0x000C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
		class AM1MissionTaskActor* InstancedTaskActor;                                // 0x0010(0x0008)(Edit, ZeroConstructor, EditConst, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		TSubclassOf<class AM1MissionTaskActor>        TaskActorClass;                                    // 0x0018(0x0008)(Edit, ZeroConstructor, EditConst, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// 0x0060 (0x0060 - 0x0000)
	struct FM1MissionProgressInfo final
	{
	public:
		class AM1MissionTaskActor* ActivatedTaskActor;                                // 0x0000(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		class AM1MissionTaskActor* LastTaskActor;                                     // 0x0008(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		EM1MissionActorState                          MissionActorState;                                 // 0x0010(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_11[0x3];                                       // 0x0011(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
		int32                                         ActivatedTaskIndex;                                // 0x0014(0x0004)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		TArray<class UM1MissionControlComponent*>     JoinedControls;                                    // 0x0018(0x0010)(ExportObject, ZeroConstructor, Transient, ContainsInstancedReference, NativeAccessSpecifierPrivate)
		TArray<class AM1Player*>                      DeadJoinedPlayers;                                 // 0x0028(0x0010)(ZeroConstructor, Transient, NativeAccessSpecifierPrivate)
		struct FDateTime                              BeginTime;                                         // 0x0038(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		double                                        PlayedTime;                                        // 0x0040(0x0008)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		double                                        PenaltyTime;                                       // 0x0048(0x0008)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_50[0x10];                                      // 0x0050(0x0010)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	// 0x0390 (0x05D8 - 0x0248)
	class AM1MissionActor final : public AActor
	{
	public:
		uint8                                         Pad_MissionData[0x30];						     // 0x0248
		class UM1DataMission* MissionData;                                       // 0x0278(0x0008)(Edit, ZeroConstructor, EditConst, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		uint8                                         Pad_TaskLinks[0x10];								 // 0x0280
		TArray<struct FM1MissionTaskLink>             TaskLinks;                                         // 0x0290(0x0010)(Edit, EditFixedSize, ZeroConstructor, EditConst, NativeAccessSpecifierPrivate)
		uint8                                         Pad_ProgressInfo[0xC8];						     // 0x02A0
		struct FM1MissionProgressInfo                 ProgressInfo;                                      // 0x0368(0x0060)(Net, Transient, RepNotify, ContainsInstancedReference, NativeAccessSpecifierPrivate)
		uint8                                         Pad_CoolTimeComponent[0x1C8];                      // 0x03C8
		class UM1CoolTimeComponent* CoolTimeComponent;                                 // 0x0590(0x0008)(Edit, ExportObject, ZeroConstructor, EditConst, InstancedReference, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_AM1MissionActor_Class[0x40];                   // 0x0598
	};


	struct FM1WeaponSlot final
	{
	public:
		uint8 Pad_Weapon[0x10];
		class AM1Weapon* Weapon; // 0x0010(0x0008)
	};

	struct FM1ActivatedWeaponSlot final
	{
	public:
		uint8                                         Pad_2593[0x8];
		struct FM1WeaponSlot                          WeaponSlot; // 0x0008(0x0018)
		uint8                                         Pad_2594[0x70];
	};

	// 0x0008 (0x00D0 - 0x00C8)
	class UM1CharacterComponent : public UM1ActorComponent
	{
	public:
		class AM1Character* Character_Owner;
	};

	// Class M1.M1AbilityComponent
	// 0x0100 (0x0220 - 0x0120)
	class UM1AbilityComponent final : public UGameplayTasksComponent
	{
	public:
		uint8                                         Pad_120[0x18];                                     // 0x0120(0x0018)(Fixing Size After Last Property [ Dumper-7 ])
		TArray<class UM1Ability*>                     RegisteredAbilities;                               // 0x0138(0x0010)(ZeroConstructor, Transient, Protected, NativeAccessSpecifierProtected)
		TArray<uint32>                                UniqueIDsToResetRepKey;                            // 0x0148(0x0010)(ZeroConstructor, Transient, Protected, NativeAccessSpecifierProtected)
		uint8                                         Pad_158[0xA8];                                     // 0x0158(0x00A8)(Fixing Size After Last Property [ Dumper-7 ])
		class UM1AbilityEventService* AblityEventService;                                // 0x0200(0x0008)(ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_208[0x18];                                     // 0x0208(0x0018)(Fixing Struct Size After Last Property [ Dumper-7 ])

	public:
		void NetMulticast_SpawnFXsForAbility(const class UM1Ability* InAbilityCDO, class AActor* InAbilityInstigator, const TArray<class AActor*>& InTargets, class FName InName, const struct FM1FXParam& InFXParam, const struct FTransform& InTransform, float InAOEScaleMultiplier);
		void ServerCancelAbility(const struct FM1AbilityId& Handle);

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1AbilityComponent">();
		}
		static class UM1AbilityComponent* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1AbilityComponent>();
		}
	};
	// 0x0158 (0x0228 - 0x00D0)
	class UM1WeaponSlotControlComponent final : public UM1CharacterComponent
	{
	public:
		uint8 Pad_ActiveSlot[0x20]; //0xD0
		struct FM1ActivatedWeaponSlot ActivatedWeaponSlot; // 0x00F0(0x0040)
		uint8 Pad_Player_Owner[0x30];// 0x0180
		class AM1Player* Player_Owner; // 0x01B0(0x0008)
		class UM1AbilityComponent* Ability_Component;                                 // 0x01B8(0x0008)(ExportObject, ZeroConstructor, Transient, InstancedReference, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		class UM1PlayerStatComponent* PlayerStat_Component;                           // 0x01C0(0x0008)(ExportObject, ZeroConstructor, Transient, InstancedReference, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)

		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1WeaponSlotControlComponent">();
		}
	};

	// 0x0130 (0x0200 - 0x00D0)
	class alignas(0x10) UM1TeleportHandlerComponent final : public UM1CharacterComponent
	{
	public:
		//void ClientCancelPretendTeleportTo(const struct FVector& InLocation, const struct FRotator& InRotation, bool bTeleportResult);
		//void ClientFailedTeleport();
		//void ClientMoveImmediately();
		//void ClientReceiveRequestedTeleportInfo(const struct FM1RequestedTeleportInfo& InInfo);
		//void NetMulticastPretendTeleportTo();
		//void OnMontageEnded(class UAnimMontage* InMontage, bool InInterrupted);
		//void OnMontageStarted(class UAnimMontage* InMontage);
		//void OnRep_TeleportInfo();
		//void ServerCancelPretendTeleportTo(const struct FVector& InLocation, const struct FRotator& InRotation);
		//void ServerFinishTeleportProcess();
		void ServerMoveToTeleportToLocation(const struct FVector& InLocation, const struct FRotator& InRotation);
		//void ServerRequestEnterMap(const struct FVector& InLocation, const struct FRotator& InRotation);
		//void ServerRequestTeleport(const class FString& InContext, const struct FVector& InLocation, const struct FRotator& InRotation, bool InTeleportAtSequence, EM1TeleportExecuteType InType);
		//void ServerRequestTeleportByActor(const class FString& InContext, const class AActor* InLocationActor, bool InTeleportAtSequence, EM1TeleportExecuteType InType);

	};
	// 0x00E8 (0x0330 - 0x0248)
	class AM1Actor : public AActor
	{
	public:
		uint8 Pad_AM1Actor_Class[0xE8];

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1Actor">();
		}
	};

	// 0x0008 (0x0008 - 0x0000)
	struct alignas(0x08) FTableRowBase
	{
	public:
		uint8                                         Pad_0[0x8];                                        // 0x0000(0x0008)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	enum class EM1FireState : uint8
	{
		Default = 0,
		ZoomIn = 1,
		Ready = 2,
		Unknown = 3,
		Firing = 4,
		ZoomOut = 5,
		EM1FireState_MAX = 6
	};

	// 0x0008 (0x0008 - 0x0000)
	struct FM1RecoilInfo final
	{
	public:
		float                                         Pitch;                                             // 0x0000(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         Yaw;                                               // 0x0004(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// 0x0030 (0x0038 - 0x0008)
	struct FM1RangedWeaponRecoilData final : public FTableRowBase
	{
	public:
		class FString                                 StringId;                                          // 0x0008(0x0010)(Edit, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         RecoilApplyInterpSpeed;                            // 0x0018(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         RecoilRecoverInterpSpeed;                          // 0x001C(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         RecoilRecoverStartDelay;                           // 0x0020(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         RecoilResetTimeAfterFire;                          // 0x0024(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		TArray<struct FM1RecoilInfo>                  RecoilInfos;                                       // 0x0028(0x0010)(Edit, ZeroConstructor, NativeAccessSpecifierPublic)
	};

	struct FM1TickStepper
	{
		float MinimumStepInterval;
		float StepInterval;
		float ElapsedTime;
	};

	// 0x0160 (0x0490 - 0x0330)
	class AM1Weapon : public AM1Actor
	{
	public:
		uint8 Pad_FireLoopComponent[0x18]; // 0x330
		class UM1WeaponFireLoopComponent* FireLoopComponent; // 0x0348(0x0008)
		uint8 Pad_RoundsComponent[0x10]; // 0x350
		class UM1WeaponSprayPatternComponent* SprayPatternComponent;                             // 0x0360(0x0008)(ExportObject, ZeroConstructor, InstancedReference, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		class UM1WeaponRoundsComponent* RoundsComponent; // 0x0368(0x0008)
		uint8 Pad_WeaponLevel[0x78]; // 0x368
		uint32_t WeaponLevel; // 0x3E0 0x4
		EM1FireState FireState; // 0x3E4 0x1
		uint8 Pad_PlayerOwner[0x3]; // 0x3E5
		class AM1Player* PlayerOwner; // 0x03E8(0x0008)
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1Weapon">();
		}
	};

	// 0x0010 (0x00D8 - 0x00C8)
	class UM1WeaponComponent : public UM1ActorComponent
	{
	public:
		uint8 Pad_UM1WeaponComp_Class[0x10];
	};


	struct FM1WeaponBurstFireParams
	{
		int Count;
		float Delay;
		float Interval;
	};

	struct FM1WeaponFireAnimParams
	{
		float PlayRate;
		float PlayTime;
	};

	struct FM1WeaponFireParams
	{
		float FireInterval;
		FM1WeaponBurstFireParams BurstFire;
		FM1WeaponFireAnimParams Anim;
	};

	// 0x0090 (0x0168 - 0x00D8)
	class UM1WeaponFireLoopComponent : public UM1WeaponComponent
	{
	public:
		uint8                                         Pad_110[0x48];                                     // 0x00D8
		bool										  bAttacking;                                        // 0x0120(0x0001)
		uint8                                         Pad_CurrFireParams[0x3];                           // 0x0121
		TOptional<FM1WeaponFireParams>				  CurrFireParams;									 // 0x0124(0x001C)(Transient, NativeAccessSpecifierPrivate)
		float										  ElapsedTimeAfterFire;							     // 0x0140(0x0004)
		uint8                                         Pad_UM1WeaponFireLoopComponent[0x24];              // 0x0144

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1WeaponFireLoopComponent">();
		}
	};

	// 0x0028 (0x0100 - 0x00D8)
	class UM1WeaponRoundsComponent final : public UM1WeaponComponent
	{
	public:
		uint8 Pad_CurrentRounds[0x14]; // 0x0D8
		int32 CurrentRounds; // 0x00EC(0x0004)

		void ClientFillCurrentRoundByServer();
	};

	// 0x0038 (0x0110 - 0x00D8)
	class UM1WeaponSprayPatternComponent final : public UM1WeaponComponent
	{
	public:
		uint8                                         Pad_CrosshairSizeBase[0xC];					  // 0x00D8
		float										  CrosshairSizeBase;							  // 0x00E4(0x0004)
		struct FM1RangedWeaponRecoilData* RecoilData;									  // 0x00E8(0x0008)
		struct FM1RangedWeaponRecoilData* ZoomRecoilData;								  // 0x00F0(0x0008)
		bool										  bApplySpreadSize;								  // 0x00F8(0x0001)
		uint8                                         Pad_CurrentSpreadSize[0xF];					  // 0x00F9
		float										  CurrentSpreadSize;						      // 0x0108(0x0004)                        
		uint8                                         Pad_UM1WeaponSprayPatternComponent[0x4];        // 0x010C
	};

	// 0x0448 (0x0768 - 0x0330)
	class AM1AbilityActor : public AM1Actor
	{
	public:
		uint8 Pad_AM1AbilityActor_Class[0x448];
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1AbilityActor">();
		}
	};


	enum class EM1MiniGameResult : uint8
	{
		Unknown = 0,
		Fail = 1,
		Success = 2,
		Cancel = 3,
		ForceCancel = 4,
		EM1MiniGameResult_MAX = 5,
	};

	enum class EM1MiniGameType : uint8
	{
		None = 0,
		TimingRing = 1,
		EM1MiniGameType_MAX = 2,
	};


	enum class EM1MiniGameDifficulty : uint8
	{
		None = 0,
		Normal = 1,
		Hard = 2,
		VeryHard = 3,
		EM1MiniGameDifficulty_MAX = 4,
	};

	// 0x0010 (0x0038 - 0x0028)
	class UM1MiniGame : public UObject
	{
	public:
		struct FM1TemplateId MiniGameTemplateId;
		struct FM1TemplateId CharacterTemplateId;
		struct FM1TemplateId FieldDifficultyTemplateId;
		EM1MiniGameType Type;
		EM1MiniGameDifficulty Difficulty;
	};

	// 0x0020 (0x0048 - 0x0028)
	class UM1MiniGameSystem final : public UObject
	{
	public:
		uint8 Pad_28[0x18];
		class UM1MiniGame* CurrentMiniGame; // 0x0040(0x0008)
	};

	struct FM1MiniGameResult final
	{
		struct FM1TemplateId                          MiniGameTemplateId;
		struct FM1TemplateId                          FieldDifficultyTemplateId;
		EM1MiniGameResult                             Result;
		uint8                                         Pad_9[0x3];
	};

	// 0x0030 (0x0278 - 0x0248)
	class AM1MiniGameActor final : public AActor
	{
	public:
		struct FM1TemplateId                          MiniGameTid;// 0x0248(0x0004)
		struct FM1TemplateId                          FieldDifficultyTid;// 0x024C(0x0004)
		uint8                                         Pad_250[0x18];// 0x0250(0x0018)

	public:
		void ClientStartMiniGame(const struct FM1TemplateId& InMiniGameTid, const struct FM1TemplateId& InPlayerTid, const struct FM1TemplateId& InFieldDifficultyTid, const EM1MiniGameDifficulty InDifficulty);
		void ClientStopMiniGame();
		void ServerDropItems(class AController* InInstigator);
		void ServerOnMiniGameEnded(const struct FM1MiniGameResult& InResult);

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1MiniGameActor">();
		}
	};

	class UM1NgsSystem : public UObject
	{
	public:
		bool bStarted;
		bool bStartCompleted;
		bool bInitializeCompleted;		// 0x2A
		uint8 Pad_Stuff[0x5D];
		bool bReservedNeedGuideLink;	// 0x88
		bool bPatchCompleted;			// 0x89
		bool bEnabled;					// 0x8A
	};


	class UM1LocalGameInstanceSubsystem final : public UGameInstanceSubsystem
	{
	public:
		uint8 Pad_30[0x18];                                      // 0x0030(0x0018)(Fixing Size After Last Property [ Dumper-7 ])
		void* WorldSystem;                                       // 0x0048(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* SquadSystem;                                       // 0x0050(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* GameGuideSystem;                                   // 0x0058(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* WebGuideSystem;                                    // 0x0060(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		class UM1ResearchSystem* ResearchSystem;                 // 0x0068(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* WidgetSavedSettingSystem;                          // 0x0070(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* ActorVisibilitySystem;                             // 0x0078(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* LoadingScreenSystem;                               // 0x0080(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* NotificationSystem;                                // 0x0088(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* InputSystem;                                       // 0x0090(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* ChattingSystem;                                    // 0x0098(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* VoiceChatSystem;                                   // 0x00A0(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* EventSystem;                                       // 0x00A8(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8 Pad_C0[0x18];										 // 0x00C0(0x0008)(Fixing Size After Last Property [ Dumper-7 ])
		void* ToyFunnelSystem;                                   // 0x00C8(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		class UM1NgsSystem* NgsSystem;                                         // 0x00D0(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* GameplayClientDataStorage;                         // 0x00D8(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* HttpUtilSystem;                                    // 0x00E0(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* OptionSubsystem;                                   // 0x00E8(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* DataPatchSystem;                                   // 0x00F0(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* UserReportSystem;                                  // 0x00F8(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* ContentsUnlockSystem;                              // 0x0100(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* SocialSystem;                                      // 0x0108(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* BattlePassChallengeSystem;                         // 0x0110(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* MatchingSystem;                                    // 0x0118(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		class UM1MiniGameSystem* MiniGamePlayer;                                    // 0x0120(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* RequestSupportSystem;                              // 0x0128(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* ClientSavedDataSystem;                             // 0x0130(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* RuntimeContext;                                    // 0x0138(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		void* AutoEquipmentReplacementSystem;                    // 0x0140(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)

	public:
		static class UM1LocalGameInstanceSubsystem* GetSystem(const class UObject* WorldContextObject);

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1LocalGameInstanceSubsystem">();
		}
		static class UM1LocalGameInstanceSubsystem* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1LocalGameInstanceSubsystem>();
		}
	};



	enum class ESlateVisibility : uint8
	{
		Visible = 0,
		Collapsed = 1,
		Hidden = 2,
		HitTestInvisible = 3,
		SelfHitTestInvisible = 4,
		ESlateVisibility_MAX = 5,
	};
	class UVisual : public UObject
	{
	};
	// 0x0108 (0x0130 - 0x0028)
	class UWidget : public UVisual
	{
	public:
		uint8 Pad_UWidget_Class[0x108];
		void SetIsEnabled(bool bInIsEnabled);
		void SetVisibility(ESlateVisibility InVisibility);
	};
	// 0x0120 (0x0250 - 0x0130)
	class UUserWidget : public UWidget
	{
	public:
		uint8 Pad_UUserWidget_Class[0x120];
	};

	// 0x04D8 (0x0728 - 0x0250)
	class UM1UIWidget : public UUserWidget
	{
	public:
		uint8 Pad_UM1UIWidget_Class[0x4D8];
	};

	// 0x0030 (0x0758 - 0x0728)
	class UM1UIActorWidget : public UM1UIWidget
	{
	public:
		uint8 Pad_UM1UIActorWidget_Class[0x30];
	};

	// 0x0230 (0x0420 - 0x01F0)
	class UM1ActorWidgetComponent : public USceneComponent
	{
	public:
		uint8 Pad_ActorWidget[0x48];
		TWeakObjectPtr<class UM1UIActorWidget> ActorWidget; // 0x0238(0x0008)
		uint8 Pad_UM1ActorWidgetComponent_Class[0x1E0]; // 0x240
	};
	// 0x0030 (0x0440 - 0x0410)
	class UM1CharacterInfoWidgetComponent final : public UM1ActorWidgetComponent
	{
	public:
		uint8 Pad_UM1CharacterInfoWidgetComponent_Class[0x30];
	};
	// 0x00C0 (0x0818 - 0x0758)
	class UM1UICharacterInfoBase : public UM1UIActorWidget
	{
	public:
		uint8 Pad_TB_Name[0x8];
		class UM1TextBlock* TB_Name; // 0x0760(0x0008)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1UICharacterInfoBase">();
		}
	};

	class UTextLayoutWidget : public UWidget
	{
	public:
		uint8 Pad_UTextLayoutWidget_Class[0x20];
	};

	struct FSlateFontInfo final
	{
	public:
		class UObject* FontObject; // 0x0000(0x0008)
	};

	// 0x01B0 (0x0300 - 0x0150)
	class UTextBlock : public UTextLayoutWidget
	{
	public:
		class FText Text; // 0x0150(0x0018)
		uint8 Pad_UTextBlock_Class[0x198]; // 0x168
	};

#pragma pack(push, 0x1)
	class alignas(0x10) UM1TextBlock : public UTextBlock
	{
	public:
		uint8 Pad_UM1TextBlock_Class[0x18];// 0x0300(0x0018)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1TextBlock">();
		}
	};
#pragma pack(pop)

	class UM1UIStandaloneWidget : public UM1UIWidget
	{

	};

	class UM1UICheat final : public UM1UIStandaloneWidget
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1UICheat">();
		}
	};

	class UM1Cheat final : public UCheatManager
	{
	public:
		uint8                                         Pad_88[0x8];                                       // 0x0088(0x0008)(Fixing Size After Last Property [ Dumper-7 ])
		class UM1UICheat* CheatUI;                                           // 0x0090(0x0008)(ExportObject, ZeroConstructor, InstancedReference, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		//class UM1UICheatVarInfo* CheatVarInfoUI;                                    // 0x0098(0x0008)(ExportObject, ZeroConstructor, InstancedReference, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)

	public:
		/*void AAA();
		void ActivateStatGroup(const class FString& GroupName);
		void CameraDist(float NewDist);
		void CameraDitherOnOff(bool bDitherOn);
		void CheatUsingPreset(const class FString& PresetsFileName, const class FString& CommandPresetName);
		void CheckStringTable();
		void ClearGameGuideData();
		void ClearRanking(EM1LeaderBoardType InLeaderboardType);
		void ClearResearchSavedData();
		void ClearWorldSavedData();
		void ClientResponseRoundsInfo(const class FString& InDebugString);
		void ClientRounds(const class FString& InServerDebugStr);
		void ClientShowCheatMessage(const class FString& CheatMessage);
		void ClientSyncAuthorityCheatVars(const struct FM1CheatVarsAuthority& AuthorityVars);
		void CommandUIDelegate(const class FString& InParam);
		void CommandUIDelegateWithOneParam(const class FString& InParam, float FirstFloatParam);
		void CommandUIDelegateWithThreeParam(const class FString& InParam, float FirstFloatParam, float SecondFloatParam, float ThirdFloatParam);
		void CommandUIDelegateWithTwoParam(const class FString& InParam, float FirstFloatParam, float SecondFloatParam);
		void Crash1();
		void Crash2();
		void Crash3();
		void CreateDummyRanking(EM1LeaderBoardType InLeaderboardType, int32 InCount);
		void DisabledCharacterControlUnapplyDelay(bool bDisabled);
		void DumpActiveStats();
		void DumpGameplay();
		void DumpGameplayAllPlayer();
		void EnableGameplayDebugShapes(bool InEnabled);
		void EnableHighLevelCameraLag(bool bEnableHighLevelCameraLag);
		void EquipCharacterSkin(int32 InTemplateId, bool bEquip);
		void EquipFellowSkin(int32 InTemplateId, bool bEquip);
		void EquipFellowSkinPaint(int32 InSkinTid, int32 InPaintTid, int32 InSlotIdx, bool bEquip);
		void FindLog();
		void ForceStartLiveCoding();
		void HelloM1();
		void HideCrosshair(bool bHideCrosshair, bool bHideScopeZoom);
		void InvertX();
		void InvertY();
		void KickTest();
		void KickTestServer(class APlayerController* InPC);
		void LocalSpawnLocationDebug(bool InEnable);
		void NetworkFailTest();
		void OnM1PlayerEndPlayed(class AActor* Player, EEndPlayReason EndPlayReason);
		void OpenCountingToast(const class FString& InMessage, float InDuration);
		void OpenCredits();
		void OpenGameGuide(int32 InGroupId);
		void OpenNpc(int32 InFunctionNum);
		void OpenPopup(const class FName& InRowName);
		void OpenSystemToast(const class FString& InMessage);
		void OpenToast(const class FName& InRowName, const class FString& InParam);
		void OpenUI(const class FString& InWidgetPath);
		void PlayDialog(const class FName& InDialogId);
		void PlayDialogWithContext(const class FName& InDialogId, const EM1AudioPlayContext InContext);
		void PlayerRestoreDissolve();
		void PlayMiniGame(const int32 InMiniGameTid, const int32 InFieldDifficultyTid);
		void PlaySequence(const class FString& SequenceName, const class FString& HideCharacters, const class FString& AttachToPlayer, const class FString& ImitateWeaponFireDir, const class FString& DisableCameraCuts, const class FString& ActivateSkipButton);
		void PrintCurrentGamma();
		void ProfileBookmark();
		void ReportAll();
		void RequestForcedMatching(int32 Tid, bool bPrivate, const class FString& ModTidString);
		void RequestRoundsInfo();
		void ResetCamera();
		void ResetCheat();
		void ResetGamma();
		void SendSlackMessage(const class FString& Msg);
		void ServerDisabledCharacterControlUnapplyDelay(bool bDisabled);
		void ServerEnableGameplayDebugShapes(bool InEnabled);
		void ServerProfileBookmark();
		void ServerRequestRoundsInfo();
		void ServerResetCheat();
		void ServerRounds();
		void ServerSendSlackMessage(const class FString& Msg);
		void ServerSlowPlayer(const float Rate);
		void ServerSpawnLocationDebug(bool InEnabled);
		void ServerSyncAuthorityCheatVars(const class UWorld* InWorldContext);
		void ServerToggleNavUpdate();
		void SetAudioType(int32 InAudio);
		void SetDebugDrawScreenRectAtCenter(float RatioX, float RatioY);
		void SetExpiredTimeLimit(float InValue);
		void SetFloat(int32 Index_0, float Value);
		void SetGamma(const float InGamma);
		void SetGammaSeparately(const float InSlateGamma, const float InViewportGamma);
		void SetInt(int32 Index_0, int32 Value);
		void SetLocalVisibilityBlock(int32 InType, bool InSet);
		void ShowActionPermissionRange(bool bShow);
		void ShowAllActorWidgetComponentsParents();
		void ShowAuthorityCheatVarsInfo();
		void ShowFootIKDebug();
		void ShowServerRounds();
		void SkipPrologue(EM1PlayableCharacter InStartCharacter);
		void SkipSequence();
		void SlowPlayer(const float Rate);
		void SpawnLocationDebug(bool InEnable);
		void StopSequence(const class FString& SequenceName);
		void StopSequenceAll();
		void SucceedCurrentMiniGame();
		void SyncAuthorityCheatVars();
		void Test(float F);
		void TestLoadingScreen();
		void TestOpenFile(const class FString& Filename);
		void ToggleCaptureMode();
		void ToggleChattingUI();
		void ToggleDebugActorWidget();
		void ToggleDevelopmentUI();
		void ToggleDisplayDialogInfo();
		void ToggleDrawDebugByOutOfPlayableArea();
		void ToggleGameplayDebugView();
		void ToggleKeyBtnOnlyConsole();
		void ToggleLoadingScreen();
		void ToggleNavUpdate();
		void ToggleRandomCustomize();
		void ToggleShowMissionMonsterDefaultMarker();*/
		//void ToggleUI();
		/*void ToggleUIMinimal();
		void TranslateDisplayString(const class FString& InLocale);
		void UnlockAllNpcServices(bool InUnlockAll);*/
		//void InitCheatManager();


	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1Cheat">();
		}
		static class UM1Cheat* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1Cheat>();
		}
	};

	// 0x0028 (0x0050 - 0x0028)
	class UM1FieldObjectActivateBehavior : public UObject
	{
	public:
		uint8 Pad_UM1FieldObjectActivateBehavior_Class[0x28]; // 0x0049(0x0007)
	};
	class UM1FieldObjectActivateBehavior_MiniGame final : public UM1FieldObjectActivateBehavior
	{
	public:
		class AM1MiniGameActor* MiniGameActor; // 0x0050(0x0008)
		uint8 Pad_58[0x10]; // 0x0058(0x0010)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1FieldObjectActivateBehavior_MiniGame">();
		}
	};
	// 0x0108 (0x0438 - 0x0330)
	class AM1FieldInteractableActor : public AM1Actor
	{
	public:
		uint8 Pad_ActivateBehavior[0xF0]; // 0x0330
		class UM1FieldObjectActivateBehavior* ActivateBehavior; // 0x0420(0x0008)
		uint8 Pad_AM1FieldInteractableActor_Class[0x10]; // 0x0428
	};
	// 0x0018 (0x0450 - 0x0438)
	class AM1FieldInteractableActor_Interaction : public AM1FieldInteractableActor
	{
	public:
		uint8 Pad_AM1FieldIntActorInt_Class[0x18];
	};
	// 0x00010 (0x0460 - 0x0450)
	class AM1FieldInteractableActorMiniGame : public AM1FieldInteractableActor_Interaction
	{
	public:
		uint8 Pad_MiniGameDifficulty[0x8];
		EM1MiniGameDifficulty MiniGameDifficulty; // 0x0440(0x0001)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1FieldInteractableActorMiniGame">();
		}
	};

	enum class EM1ItemType : uint8
	{
		None = 0,
		Currency = 1,
		Equipment = 2,
		Consumable = 3,
		Rune = 4,
		Character = 5,
		InstantUse = 6,
		Customizing = 7,
		Title = 8,
		Record = 9,
		CurrencyInternal = 10,
		ItemSelector = 11,
		Max = 12,
	};

	struct FM1ItemTidBox final
	{
	public:
		EM1ItemType                                   Type;                                              // 0x0000(0x0001)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_1BF8[0x3];                                     // 0x0001(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
		int32                                         TemplateId;                                        // 0x0004(0x0004)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct FM1DropItemInfo final
	{
	public:
		bool                                          bPublicItem;                                       // 0x0000(0x0001)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_1[0x7];                                        // 0x0001(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
		class AActor* ItemOwner;                                         // 0x0008(0x0008)(ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		int32                                         Count;                                             // 0x0010(0x0004)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FM1ItemTidBox                          ItemBox;                                           // 0x0014(0x0008)(NoDestructor, NativeAccessSpecifierPublic)
		uint8                                         Pad_1C[0x4];                                       // 0x001C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
		int64                                         ItemOid;                                           // 0x0020(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int64                                         MonsterUid;                                        // 0x0028(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// 0x0398 (0x05E0 - 0x0248)
	class alignas(0x10) AM1DropContainer : public AActor
	{
	public:
		uint8                                         Pad_BeingPicked[0x278]; // 0x248
		bool                                          bBeingPickedLocally; // 0x04C0(0x0001) Need
		bool                                          bTriedSetToObtained; // 0x04C1(0x0001) Need
		uint8                                         Pad_AM1DropContainer_Class[0x11E];

		bool IsObtained() const;
	};

	// 0x0020 (0x0458 - 0x0438)
	class AM1FieldInteractableActor_Hit : public AM1FieldInteractableActor
	{
	public:
		uint8                                         AM1FieldInteractableActor_Hit_Class[0x20];
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1FieldInteractableActor_Hit">();
		}
	};

	// 0x0000 (0x0450 - 0x0450)
	class ABP_FieldInteractableBase_Hit_C : public AM1FieldInteractableActor_Hit
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_FieldInteractableBase_Hit_C">();
		}
	};

	class ABP_INTER_VulgusBox_C final : public ABP_FieldInteractableBase_Hit_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_INTER_VulgusBox_C">();
		}
	};

	// 0x0000 (0x0448 - 0x0448)
	class ABP_FieldInteractableBase_Interaction_C : public AM1FieldInteractableActor_Interaction
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_FieldInteractableBase_Interaction_C">();
		}
	};

	// 0x0000 (0x0448 - 0x0448)
	class ABP_INTER_MilitarySupplies_C final : public ABP_FieldInteractableBase_Interaction_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_INTER_MilitarySupplies_C">();
		}
	};

	// 0x00A0 (0x0680 - 0x05E0)
	class AM1DroppedItem : public AM1DropContainer
	{
	public:
		uint8                                         Pad_DropItemInfo[0x40];
		struct FM1DropItemInfo                        DropItemInfo; // 0x0620(0x0030)
		uint8                                         Pad_ObtainRequested[0x20]; // 0x0650(0x0020)
		bool                                          bObtainRequestedOnClient; // 0x0670(0x0001)
		uint8                                         Pad_AM1DroppedItem_Class[0xF]; // 0x0671(0x000F)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1DroppedItem">();
		}
		static class AM1DroppedItem* GetDefaultObj()
		{
			return GetDefaultObjImpl<AM1DroppedItem>();
		}
	};

	class ABP_DroppedItemBase_C : public AM1DroppedItem
	{
	};

	class ABP_AmmoEnhancedDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_AmmoEnhancedDroppedItem_C">();
		}
	};

	class ABP_AmmoGeneralDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_AmmoGeneralDroppedItem_C">();
		}
	};

	class ABP_AmmoHighpowerDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_AmmoHighpowerDroppedItem_C">();
		}
	};

	class ABP_AmmoImpactDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_AmmoImpactDroppedItem_C">();
		}
	};

	class ABP_HealthOrbDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_HealthOrbDroppedItem_C">();
		}
	};

	class ABP_ManaOrbDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_ManaOrbDroppedItem_C">();
		}
	};

	class ABP_EquipTier01DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_EquipTier01DroppedItem_C">();
		}
	};

	class ABP_EquipTier02DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_EquipTier02DroppedItem_C">();
		}
	};

	class ABP_EquipTier03DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_EquipTier03DroppedItem_C">();
		}
	};

	class ABP_ResourceTier01DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_ResourceTier01DroppedItem_C">();
		}
	};

	class ABP_ResourceTier02DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_ResourceTier02DroppedItem_C">();
		}
	};

	class ABP_ResourceTier03DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_ResourceTier03DroppedItem_C">();
		}
	};

	class ABP_RuneTier01DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_RuneTier01DroppedItem_C">();
		}
	};

	class ABP_RuneTier02DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_RuneTier02DroppedItem_C">();
		}
	};

	class ABP_RuneTier03DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_RuneTier03DroppedItem_C">();
		}
	};

	class ABP_RuneTier04DroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_RuneTier04DroppedItem_C">();
		}
	};

	class ABP_GoldDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_GoldDroppedItem_C">();
		}
	};

	class ABP_KuiperShardDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_KuiperShardDroppedItem_C">();
		}
	};

	class ABP_BuffOrbDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_BuffOrbDroppedItem_C">();
		}
	};

	class ABP_EmberDroppedItem_C final : public ABP_DroppedItemBase_C
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticBPGeneratedClassImpl<"BP_EmberDroppedItem_C">();
		}
	};




	class UM1MissionControlComponent final : public UActorComponent
	{
	public:
		uint8                                         Pad_SubServices[0x20];                             // 0x00A8
		TArray<class UM1MissionTaskService*>          SubServices;                                       // 0x00C8(0x0010)(ZeroConstructor, Transient, NativeAccessSpecifierPrivate)
		uint8                                         Pad_ActivatedMissions[0x130];                      // 0x00D8
		TArray<class AM1MissionActor*>                AvailableMissions;                                 // 0x0208(0x0010)(Net, ZeroConstructor, RepNotify, NativeAccessSpecifierPrivate)
		TArray<class AM1MissionActor*>                ActivatedMissions;                                 // 0x0218(0x0010)(Net, ZeroConstructor, RepNotify, NativeAccessSpecifierPrivate)
		TArray<class AM1MissionActor*>                LastActivatedMissions;							 // 0x0228(0x0010)(Net, ZeroConstructor, RepNotify, NativeAccessSpecifierPrivate)
		TWeakObjectPtr<class AM1MissionActor>         LastPlayedMissionActor;							 // 0x0238(0x0008)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_MissionResult[0x380];
		class UM1MissionResult* MissionResult;                                     // 0x05B8(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_UM1MissionControlComponent_Class[0x218];       // 0x05C0

	public:
		//void ClientCreateRSideWidgetForMission(const EM1UIGameRSideType& InType, class AActor* InActor);
		//void ClientInitMissionTotalScore(class AM1MissionActor* InMissionActor, int32 InTotalScore);
		//void ClientMissionCompleted(const class AM1MissionActor* InMissionActor, bool InSucceeded);
		//void ClientNotifyPauseReservedAutoExit();
		//void ClientNotifyRematchTogetherTimerStart(float InRemainTime);
		//void ClientNotifyReservedAutoExit(float InRemainTime);
		/*void ClientOnBeginRetriableTaskBoundary(const class AM1MissionTaskActor* InTaskActor);
		void ClientOnEventMissionTaskLastChance(const class AM1MissionTaskActor* InTaskActor);
		void ClientOnMissionPreTaskUpdated(const struct FM1MissionPreTaskProgressInfo& InInfo);
		void ClientOnPenaltyTimeAdded(double InPenaltyTimeAdded);
		void ClientOnScoreUpdatedByKillingMonster(class AM1MissionActor* InMissionActor, int32 InScore);
		void ClientOnScoreUpdatedBySucceededTask(class AM1MissionActor* InMissionActor, int32 InScore);
		void ClientOnTryCountChanged(int32 InCount);
		void ClientOtherPlayerJoinedSimpleInfo(const struct FM1SimpleMissionPlayerJoinInfoForRPC& InJoined, bool InAlreadyJoinedPlayer);
		void ClientOtherPlayerLeftSimpleInfo(const struct FM1SimpleMissionPlayerLeaveInfoForRPC& InLeft);
		void ClientPrepareReservedMission(const struct FM1TemplateId& ReservedMissionID);
		void ClientPrepareWorldMissionByID(const struct FM1TemplateId& InWorldMissionId);
		void ClientReceiveCountdownToMissionStart(float InCountdown);
		void ClientReceivedTaskNotification(TSubclassOf<class UM1MissionTaskNotification> InTaskNotification, const TArray<class FString>& InOptional);
		void ClientReceiveInGameStatistics(const struct FM1InGameStatistics& InStatistics);
		void ClientRemoveRSideWidgetForMission(const EM1UIGameRSideType& InType);
		void ClientReportLastPlayedMissionHistory(const class FString& InHistory, const class FString& InPlayTime);
		void ClientReportLastPlayedMissionHistoryBegin(const TArray<class FString>& InSpawnList);
		void ClientReportLastPlayedMissionHistoryEnd(const class FString& InPlayTime);
		void ClientReportLastPlayedMissionHistoryUpdate(const TArray<class FString>& InSpawnList);
		void ClientSetActivatedOrDeactivatedMission(const struct FM1MissionActivationInfo& InInfo);
		void ClientSetInstantTypeMissionOrVoidBattlePlay(bool InInstantTypeMissionOrVoidBattlePlay);
		void ClientSetMissionPlaytime(double InMissionPlaytime);
		void ClientSetMissionRewardedData(const struct FM1MissionRewardedWrapper& InMissionRewardedWrapper);
		void ClientShowCurrentMissionInfo_MissionActorInfo(const class FString& InInfo);
		void ClientShowCurrentMissionInfo_MissionTaskActorInfo(const class FString& InInfo);
		void ClientShowCurrentMissionInfo_PendingTaskEventsInfo(const TArray<class FString>& InList);
		void ClientShowCurrentMissionInfo_SuccessTaskEventsInfo(const TArray<class FString>& InList);
		void ClientUpdateMissionTaskStatus(const struct FM1MissionTaskStatus& InStatus);
		void NetMulticastWantsRematch(bool InWantsRematchTogether);
		void OnClickConfirmLeaveMission(class UM1UIWidget* InWidget);
		void OnRep_ActivatedMissions();
		void OnRep_AdditionalMonstersInfo();
		void OnRep_AvailableMissions();
		void OnRep_JoinedPlayers();
		void OnRep_LastActivatedMissions();
		void OnRep_RestrictPlayersInAreaInfo();
		void OnRep_ShowMissionInfo();*/
		//void ReportLastPlayedMissionHistory();
		//void ServerAllMissionAvailable();
		void ServerCompleteCurrentTask();
		//void ServerCompletePendingTaskEvents();
		//void ServerDestroyAllActorsExceptForSpawner();
		//void ServerExilePartyMemberAndReturnToCity();
		//void ServerFailCurrentTask();
		//void ServerForceInvokerOverlap(float InNewRadius);
		void ServerJoinAndBeginMissionBy(class AM1MissionActor* InMission);
		//void ServerJoinMission();
		void ServerJoinMissionBy(class AM1MissionActor* InMission, const struct FM1MissionJoinContextParam& InContext);
		void ServerJumpToSpecificTask(int32 InTaskIndex);
		//void ServerLeaveMission(EM1MissionEndReason InReason);
		//void ServerReportCurrentTaskActorStatus();
		//void ServerReportLastPlayedMissionHistory(class AM1MissionActor* InMissionActor);
		//void ServerRequestInGameStatistics();
		void ServerRequestRestartMatchingAlone();
		//void ServerRequestWantsRematch();
		void ServerRestartLastPlayedMission();
		//void ServerRunTaskActor(class AM1MissionTaskActor* InActor);
		//void ServerSetMissionTimeLimit(float InSecs);
		void ServerStartMission(class AM1MissionActor* InMission);
		void ServerStartMissionByTemplateID(const struct FM1TemplateId& InTemplateId);
		//void ServerStartReservedMission(const struct FM1TemplateId& ReservedMissionID);
		void ServerTeleportToBestPlayer(bool bIgnoreDistance);
		//void ServerToggleMissionTimeLimit();
		//void ServerToggleShowCurrentMissionInfo();
		void ServerTrySucceedInMission(bool InNoDelay);
		void TryJoinMissionBy(class AM1MissionActor* InMission);
		void TryStartMission(const class FName& InName);
		void TryTeleportToBestPlayer(bool bIgnoreDistance);

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1MissionControlComponent">();
		}
	};

	class UM1UIInventory final : public UM1UIWidget
	{
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1UIInventory">();
		}
	};

	class UKismetStringLibrary final : public UBlueprintFunctionLibrary
	{
	public:
		/*static class FString BuildString_Bool(const class FString& AppendTo, const class FString& Prefix, bool InBool, const class FString& Suffix);
		static class FString BuildString_Color(const class FString& AppendTo, const class FString& Prefix, const struct FLinearColor& InColor, const class FString& Suffix);
		static class FString BuildString_Double(const class FString& AppendTo, const class FString& Prefix, double InDouble, const class FString& Suffix);
		static class FString BuildString_Int(const class FString& AppendTo, const class FString& Prefix, int32 InInt, const class FString& Suffix);
		static class FString BuildString_IntVector(const class FString& AppendTo, const class FString& Prefix, const struct FIntVector& InIntVector, const class FString& Suffix);
		static class FString BuildString_Name(const class FString& AppendTo, const class FString& Prefix, class FName InName, const class FString& Suffix);
		static class FString BuildString_Object(const class FString& AppendTo, const class FString& Prefix, class UObject* InObj, const class FString& Suffix);
		static class FString BuildString_Rotator(const class FString& AppendTo, const class FString& Prefix, const struct FRotator& InRot, const class FString& Suffix);
		static class FString BuildString_Vector(const class FString& AppendTo, const class FString& Prefix, const struct FVector& InVector, const class FString& Suffix);
		static class FString BuildString_Vector2d(const class FString& AppendTo, const class FString& Prefix, const struct FVector2D& InVector2D, const class FString& Suffix);
		static class FString Concat_StrStr(const class FString& A, const class FString& B);
		static bool Contains(const class FString& SearchIn, const class FString& Substring, bool bUseCase, bool bSearchFromEnd);
		static class FString Conv_BoolToString(bool InBool);
		static class FString Conv_ByteToString(uint8 InByte);
		static class FString Conv_ColorToString(const struct FLinearColor& InColor);
		static class FString Conv_DoubleToString(double InDouble);
		static class FString Conv_InputDeviceIdToString(const struct FInputDeviceId& InDeviceId);
		static class FString Conv_Int64ToString(int64 InInt);
		static class FString Conv_IntPointToString(const struct FIntPoint& InIntPoint);
		static class FString Conv_IntToString(int32 InInt);
		static class FString Conv_IntVectorToString(const struct FIntVector& InIntVec);
		static class FString Conv_MatrixToString(const struct FMatrix& InMatrix);
		static class FString Conv_NameToString(class FName InName);
		static class FString Conv_ObjectToString(class UObject* InObj);
		static class FString Conv_PlatformUserIdToString(const struct FPlatformUserId& InPlatformUserId);
		static class FString Conv_RotatorToString(const struct FRotator& InRot);
		static void Conv_StringToColor(const class FString& InString, struct FLinearColor* OutConvertedColor, bool* OutIsValid);
		static double Conv_StringToDouble(const class FString& InString);
		static int32 Conv_StringToInt(const class FString& InString);
		static int64 Conv_StringToInt64(const class FString& InString);*/
		static class FName Conv_StringToName(const class FString& InString);
		/*static void Conv_StringToRotator(const class FString& InString, struct FRotator* OutConvertedRotator, bool* OutIsValid);
		static void Conv_StringToVector(const class FString& InString, struct FVector* OutConvertedVector, bool* OutIsValid);
		static void Conv_StringToVector2D(const class FString& InString, struct FVector2D* OutConvertedVector2D, bool* OutIsValid);
		static void Conv_StringToVector3f(const class FString& InString, struct FVector3f* OutConvertedVector, bool* OutIsValid);
		static class FString Conv_TransformToString(const struct FTransform& InTrans);
		static class FString Conv_Vector2dToString(const struct FVector2D& InVec);
		static class FString Conv_Vector3fToString(const struct FVector3f& InVec);
		static class FString Conv_VectorToString(const struct FVector& InVec);
		static int32 CullArray(const class FString& SourceString, TArray<class FString>* InArray);
		static bool EndsWith(const class FString& SourceString, const class FString& InSuffix, ESearchCase SearchCase);
		static bool EqualEqual_StriStri(const class FString& A, const class FString& B);
		static bool EqualEqual_StrStr(const class FString& A, const class FString& B);
		static int32 FindSubstring(const class FString& SearchIn, const class FString& Substring, bool bUseCase, bool bSearchFromEnd, int32 StartPosition);
		static TArray<class FString> GetCharacterArrayFromString(const class FString& SourceString);
		static int32 GetCharacterAsNumber(const class FString& SourceString, int32 Index_0);
		static class FString GetSubstring(const class FString& SourceString, int32 StartIndex, int32 Length);
		static bool IsEmpty(const class FString& InString);
		static bool IsNumeric(const class FString& SourceString);
		static class FString JoinStringArray(const TArray<class FString>& SourceArray, const class FString& Separator);
		static class FString Left(const class FString& SourceString, int32 Count);
		static class FString LeftChop(const class FString& SourceString, int32 Count);
		static class FString LeftPad(const class FString& SourceString, int32 ChCount);
		static int32 Len(const class FString& S);
		static bool MatchesWildcard(const class FString& SourceString, const class FString& Wildcard, ESearchCase SearchCase);
		static class FString Mid(const class FString& SourceString, int32 Start, int32 Count);
		static bool NotEqual_StriStri(const class FString& A, const class FString& B);
		static bool NotEqual_StrStr(const class FString& A, const class FString& B);
		static TArray<class FString> ParseIntoArray(const class FString& SourceString, const class FString& Delimiter, const bool CullEmptyStrings);
		static class FString Replace(const class FString& SourceString, const class FString& From, const class FString& To, ESearchCase SearchCase);
		static int32 ReplaceInline(class FString& SourceString, const class FString& SearchText, const class FString& ReplacementText, ESearchCase SearchCase);
		static class FString Reverse(const class FString& SourceString);
		static class FString Right(const class FString& SourceString, int32 Count);
		static class FString RightChop(const class FString& SourceString, int32 Count);
		static class FString RightPad(const class FString& SourceString, int32 ChCount);
		static bool Split(const class FString& SourceString, const class FString& InStr, class FString* LeftS, class FString* RightS, ESearchCase SearchCase, ESearchDir SearchDir);
		static bool StartsWith(const class FString& SourceString, const class FString& InPrefix, ESearchCase SearchCase);
		static class FString TimeSecondsToString(float InSeconds);
		static class FString ToLower(const class FString& SourceString);
		static class FString ToUpper(const class FString& SourceString);
		static class FString Trim(const class FString& SourceString);
		static class FString TrimTrailing(const class FString& SourceString);*/

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"KismetStringLibrary">();
		}
		static class UKismetStringLibrary* GetDefaultObj()
		{
			return GetDefaultObjImpl<UKismetStringLibrary>();
		}
	};

	// 0x0060 (0x0840 - 0x07E0)
	class AM1MissionTaskActorDestructionVulgusPost : public AM1MissionTaskActor
	{
	public:
		uint8                                         Pad_MissionTargets[0x10];										 // 0x07E0
		TArray<class AM1MissionTargetInteraction*>    MissionTargets;											     // 0x07F0(0x0010)(Edit, ZeroConstructor, EditConst, NativeAccessSpecifierPrivate)
		uint8                                         Pad_AM1MissionTaskActorDestructionVulgusPost_Class[0x40];      // 0x0800
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1MissionTaskActorDestructionVulgusPost">();
		}
	};
	static_assert(offsetof(AM1MissionTaskActorDestructionVulgusPost, MissionTargets) == 0x07F0, "Bad alignment");

	class UConsole final : public UObject
	{
	};

	enum class EM1StatType : uint8
	{
		Stat_MaxHp = 0,
		Stat_KnockbackSpeed = 1,
		Stat_KnockbackTime = 2,
		Stat_KnockdownTime = 3,
		Stat_RangeMax = 4,
		Stat_GravityScaleCoefficient = 5,
		Stat_CrosshairSizeMax = 6,
		Stat_Impulse = 7,
		Stat_AimOnDelay = 8,
		Stat_AimOffDelay = 9,
		Stat_JumpZVelocity = 10,
		Stat_CurrentEnergyShield = 11,
		Stat_InteractionRevive = 12,
		Stat_RunSpeed = 13,
		Stat_SprintSpeed = 14,
		Stat_ZoomMoveSpeed = 15,
		Stat_RoundsPerMagazine = 16,
		Stat_ReloadRounds = 17,
		Stat_FireInterval = 18,
		Stat_MaxMp = 19,
		Stat_MaxEnergyShield = 20,
		Stat_ATK = 21,
		Stat_ATKMin = 22,
		Stat_ATKMax = 23,
		Stat_DEF = 24,
		Stat_CriticalChance = 25,
		Stat_CriticalATKIncrease = 26,
		Stat_ShellCount = 27,
		Stat_RoundsConsume = 28,
		Stat_AimMoveSpeed = 29,
		Stat_WeaknessDamageIncrease = 30,
		Stat_SpreadZoomSize_Deprecated = 31,
		Stat_CrosshairMoveSizeModifier = 32,
		Stat_CrosshairFireSizeModifier = 33,
		Stat_CrosshairZoomFireSizeModifier = 34,
		Stat_CrosshairRecoverySpeed = 35,
		Stat_CrosshairChangeSpeed = 36,
		Stat_ClimbSpeed = 37,
		Stat_StressMaxRate_Deprecated = 38,
		Stat_StressDamageRate = 39,
		Stat_StressDecreaseRate = 40,
		Stat_NonCombatEntryTime_Deprecated = 41,
		Stat_StressDecreaseStartTime = 42,
		Stat_WalkSpeed = 43,
		Stat_MoveDestructivePower = 44,
		Stat_WeaponProjHitRadiusCoefficient = 45,
		Stat_EvadeTime = 46,
		Stat_EvadeRecoveryTime = 47,
		Stat_Domination = 48,
		Stat_Manipulation = 49,
		Stat_Fortitude = 50,
		Stat_CrosshairSizeMin = 51,
		Stat_ReloadSpeed = 52,
		Stat_ReloadCompleteDelay = 53,
		Stat_ReduceRangeStart = 54,
		Stat_ReduceRangeEnd = 55,
		Stat_ATKReductionByRange = 56,
		Stat_ATKExplosion = 57,
		Stat_ATKBlazer = 58,
		Stat_ATKGlacier = 59,
		Stat_ATKElectricity = 60,
		Stat_ATKDemonic = 61,
		Stat_DEFBlazer = 62,
		Stat_DEFGlacier = 63,
		Stat_DEFElectricity = 64,
		Stat_DEFDemonic = 65,
		Stat_RESCriticalChance = 66,
		Stat_ReloadTime = 67,
		Stat_Burst = 68,
		Stat_Crush = 69,
		Stat_Piercing = 70,
		Stat_FireBurstInterval = 71,
		Stat_ATKReductionByExplosionRange = 72,
		Stat_ZoomOutDelay = 73,
		Stat_Penetration = 74,
		Stat_PenetrationDefence = 75,
		Stat_WeaponDestructivePower = 76,
		Stat_SkillDestructivePower = 77,
		Stat_FireIncreaseSpread_Deprecated = 78,
		Stat_LifeTime_Deprecated = 79,
		Stat_BonusLifeTime_Deprecated = 80,
		Stat_RangeMin_Deprecated = 81,
		Stat_WireCooltimeCoefficient = 82,
		Stat_WireChargingTimeCoefficient = 83,
		Stat_KnockbackAttackRate = 84,
		Stat_KnockbackDefenceResistance = 85,
		Stat_KnockbackTimeResistance = 86,
		Stat_KnockbackSpeedResistance = 87,
		Stat_KnockdownAttackRate = 88,
		Stat_KnockdownXSpeed = 89,
		Stat_KnockdownZSpeed = 90,
		Stat_KnockdownDefenceResistance = 91,
		Stat_KnockdownXSpeedResistance = 92,
		Stat_KnockdownZSpeedResistance = 93,
		Stat_KnockdownTimeResistance = 94,
		Stat_MeleeSpecialAttackReduceCoolTime_Deprecated = 95,
		Stat_HitStopAttackPoint = 96,
		Stat_HitStopDefencePoint = 97,
		Stat_BerserkMoveSpeed = 98,
		Stat_BerserkMaxRate = 99,
		Stat_BerserkDamageRate = 100,
		Stat_BerserkDecreaseStartTime = 101,
		Stat_BerserkDecreaseRate = 102,
		Stat_BerserkMaintenanceTime = 103,
		Stat_NaturalRecoveryTickTimeHp = 104,
		Stat_NaturalRecoveryTickTimeMp = 105,
		Stat_NaturalRecoveryTickTimeEnergyShield = 106,
		Stat_NaturalRecoveryHp = 107,
		Stat_NaturalRecoveryMp = 108,
		Stat_NaturalRecoveryEnergyShield = 109,
		Stat_InBattleNaturalRecoveryHp = 110,
		Stat_InBattleNaturalRecoveryMp = 111,
		Stat_InBattleNaturalRecoveryEnergyShield = 112,
		Stat_CurrentHp = 113,
		Stat_CurrentMp = 114,
		Stat_MaxCommonGauge = 115,
		Stat_CurrentCommonGauge = 116,
		Stat_DEFInflection_VarX1_Deprecated = 117,
		Stat_DEFInflection_VarX2_Deprecated = 118,
		Stat_DEFInflection_VarX3_Deprecated = 119,
		Stat_WireStackCountIncrease = 120,
		Stat_RecoilCoefficient = 121,
		Stat_SkillCooltimeCoefficient = 122,
		Stat_SkillCostCoefficient = 123,
		Stat_SkillATKCoefficient = 124,
		Stat_FinalDamageCoefficient_Deprecated = 125,
		Stat_HpOrbRecoveryCoefficient_Deprecated = 126,
		Stat_MpOrbRecoveryCoefficient_Deprecated = 127,
		Stat_EmptyRoundsReloadTimeSpeed = 128,
		Stat_SkillDurationIncrease = 129,
		Stat_SkillDurationDecrease = 130,
		Stat_SkillScaleCoefficient = 131,
		Stat_WeaponChangingSpeed = 132,
		Stat_MaxGeneralRoundsCoefficient = 133,
		Stat_MaxEnhancedRoundsCoefficient = 134,
		Stat_MaxImpactRoundsCoefficient = 135,
		Stat_MaxHighpowerRoundsCoefficient = 136,
		Stat_NaturalRecoveryTickPR_Deprecated = 137,
		Stat_InBattleNaturalRecoveryPR_Deprecated = 138,
		Stat_NaturalRecoveryPR_Deprecated = 139,
		Stat_CrosshairJumpSizeModifier = 140,
		Stat_PhysicalATK = 141,
		Stat_NatureATK = 142,
		Stat_GiveHpRecoveryCoefficient = 143,
		Stat_TakeHpRecoveryCoefficient = 144,
		Stat_GiveMpRecoveryCoefficient = 145,
		Stat_TakeMpRecoveryCoefficient = 146,
		Stat_RWATKCoefficient = 147,
		Stat_GiveFinalDamageCoefficient = 148,
		Stat_TakeFinalDamageCoefficient = 149,
		Stat_LevelFactor = 150,
		Stat_SkillATKCorrection = 151,
		Stat_ItemLevelCorrection = 152,
		Stat_MaxStressGauge = 153,
		Stat_CurrentStressGauge = 154,
		Stat_MaxBerserkGauge = 155,
		Stat_CurrentBerserkGauge = 156,
		Stat_SkillATKPower = 157,
		Stat_SkillPowerCoefficient = 158,
		Stat_SkillPlainPowerCoefficient = 159,
		Stat_SkillBlazerPowerCoefficient = 160,
		Stat_SkillGlacierPowerCoefficient = 161,
		Stat_SkillElectricityPowerCoefficient = 162,
		Stat_SkillDemonicPowerCoefficient = 163,
		Stat_SkillFusionPowerCoefficient = 164,
		Stat_SkillSingularPowerCoefficient = 165,
		Stat_SkillDimensionPowerCoefficient = 166,
		Stat_SkillTechPowerCoefficient = 167,
		Stat_SkillPlainATKCoefficient = 168,
		Stat_SkillBlazerATKCoefficient = 169,
		Stat_SkillGlacierATKCoefficient = 170,
		Stat_SkillElectricityATKCoefficient = 171,
		Stat_SkillDemonicATKCoefficient = 172,
		Stat_SkillFusionATKCoefficient = 173,
		Stat_SkillSingularATKCoefficient = 174,
		Stat_SkillDimensionATKCoefficient = 175,
		Stat_SkillTechATKCoefficient = 176,
		Stat_SkillCriticalChance = 178,
		Stat_SkillCriticalATKIncrease = 179,
		Stat_SkillRESCriticalChance = 180,
		Stat_DEFSkill = 181,
		Stat_DEFPlain = 182,
		Stat_Accuracy = 183,
		Stat_ZoomAccuracy = 184,
		Stat_InvokeChanceElementalSE = 185,
		Stat_GiveShieldRecoveryCoefficient = 186,
		Stat_TakeShieldRecoveryCoefficient = 187,
		Stat_MaxSprintSpeed = 188,
		Stat_WeaponProjHitRadius = 189,
		Stat_DBNORescueTime = 190,
		Stat_RuneDropCoefficient = 191,
		Stat_EquipmentDropCoefficient = 192,
		Stat_ConsumableDropCoefficient = 193,
		Stat_GoldDropCoefficient = 194,
		Stat_QulipothiumDropCoefficient = 195,
		Stat_CharEXPCoefficient = 196,
		Stat_ProficiencyCoefficient = 197,
		Stat_TypeBossATKBonus = 198,
		Stat_TypeDarknessATKBonus = 199,
		Stat_TypeTruthATKBonus = 200,
		Stat_TypeImmortalATKBonus = 201,
		Stat_DBNODurationCoefficient = 202,
		Stat_LootDistanceCoefficient = 203,
		Stat_DoubleJumpZVelocity = 204,
		Stat_EciveSearchDistanceCoefficient = 205,
		Stat_EciveSearchRemainTimeCoefficient = 206,
		Stat_SubSkillATKPower = 207,
		Stat_SubSkillATKCoefficient = 208,
		Stat_TakeWeaknessDamageCoefficient_Deprecated = 209,
		Stat_RWChargeRateCoefficient = 210,
		Stat_RWDischargeRateCoefficient = 211,
		Stat_TypeBossSkillATKBonus = 212,
		Stat_TypeDarknessSkillATKBonus = 213,
		Stat_TypeTruthSkillATKBonus = 214,
		Stat_TypeImmortalSkillATKBonus = 215,
		Stat_BurstDelay = 216,
		Stat_DefaultStressStopDuration = 217,
		Stat_FieldTimingRing_LimitTimeRatio = 218,
		Stat_FieldTimingRing_NeedleRotationTimeRatio = 219,
		Stat_FieldTimingRing_ReduceSuccessCount = 220,
		Stat_FieldTimingRing_IncreaseTimingAngleMax = 221,
		Stat_FieldTimingRing_FailTimeRatio = 222,
		Stat_SkillVelocityAndRangeCoefficient = 223,
		Stat_StressMaxHpRandMinRate = 224,
		Stat_StressMaxHpRandMaxRate = 225,
		Stat_StressMaxHpCurrentRate = 226,
		Stat_ActionSpeedCoeffcient = 227,
		Stat_ProjectilePenetration = 228,
		Stat_ProjectilePenetrationDefence = 229,
		Stat_RageMaxRate = 230,
		Stat_RageDamageRate = 231,
		Stat_RageDecreaseRate = 232,
		Stat_MaxRageGauge = 233,
		Stat_CurrentRageGauge = 234,
		Stat_MultiHitChance = 235,
		Stat_AdvantageCoefficient = 236,
		Stat_MultiHitDamageIncrease = 237,
		MAX = 238,
	};

	// Enum M1Data.EM1StatModifierOp
	// NumValues: 0x0004
	enum class EM1StatModifierOp : uint8
	{
		Add = 0,
		Multiply = 1,
		Override = 2,
		Max = 3,
	};
	
	// Class M1.M1WireHook
	// 0x0130 (0x0460 - 0x0330)
	class AM1WireHook : public AM1Actor
	{
	public:
		uint8 Pad_330[0x8];                          // 0x0330(0x0008)
		class USkeletalMeshComponent* Mesh;          // 0x0338(0x0008)
		uint8 Pad_340[0x10];                         // 0x0340(0x0010) (CableComponent removed)
		class AM1WireCaster* WireCaster;             // 0x0350(0x0008)
		uint8 Pad_358[0x108];                        // 0x0358(0x0108) 
		class USkeletalMesh* DefaultMeshAsset;       // 0x0440(0x0008)
		uint8 Pad_448[0x18];                         // 0x0448(0x0018) (DefaultMaterialAsset, HookAnimClass, VisComponent)
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1WireHook">();
		}
		static class AM1WireHook* GetDefaultObj()
		{
			return GetDefaultObjImpl<AM1WireHook>();
		}
	};

	// Enum M1.EM1WireState
	// NumValues: 0x0007
	enum class EM1WireState : uint8
	{
		None = 0,
		ReadyToFire = 1,
		Firing = 2,
		ReadyToPull = 3,
		Pulling = 4,
		Cancel = 5,
		EM1WireState_MAX = 6,
	};

	// Class M1.M1RepSubObject
	// 0x0020 (0x0048 - 0x0028)
	class UM1RepSubObject : public UObject
	{
	public:
		uint8                                         Pad_28[0x14];                                      // 0x0028(0x0014)(Fixing Size After Last Property [ Dumper-7 ])
		class FName                                   DataRowName;                                       // 0x003C(0x0008)(Edit, BlueprintVisible, BlueprintReadOnly, Net, ZeroConstructor, DisableEditOnTemplate, EditConst, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_44[0x4];                                       // 0x0044(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])

	public:
		void Initialize();

		class FName GetDataRowName() const;
		bool HasAuthority() const;
		bool IsDataInitialized() const;

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1RepSubObject">();
		}
		static class UM1RepSubObject* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1RepSubObject>();
		}
	};

	// Class M1.M1Ability
	// 0x0260 (0x02A8 - 0x0048)
	class UM1Ability : public UM1RepSubObject
	{
	public:
		uint8 Pad_48[0x180];                          // 0x0048(0x0180) // Combined: Pad_48 + Pad_150 + Pad_1A0
		EM1StatType AOEScaleCoefficientStat;          // 0x01C8(0x0001)
		EM1StatType DurationIncreaseStat;             // 0x01C9(0x0001)
		EM1StatType DurationDecreaseStat;             // 0x01CA(0x0001)
		EM1StatType VelocityAndRangeScaleCoefficientStat; // 0x01CB(0x0001)
		uint8 Pad_1C[0xDC];                 // 0x01CC(0x00DC) // Combined: Pad_1CC + Pad_1F0 + Pad_258

	public:
		class AActor* GetAbilityInstigator() const;
		class AActor* GetOwnerActor() const;
		class AController* GetOwnerController() const;
		class APlayerController* GetOwnerPlayerController() const;
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1Ability">();
		}
		static class UM1Ability* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1Ability>();
		}
	};

	// Class M1.M1WireCasterAttribute
	// 0x00C0 (0x00E8 - 0x0028)
	class UM1WireCasterAttribute final : public UObject
	{
	public:
		FName AttachedToCharacterSocket;			 // 0x0028(0x0008)
		FName AttachedToWireCasterSocket;			 // 0x0030(0x0008)
		TSubclassOf<AActor> HookClass;				 // 0x0038(0x0008)
		TSubclassOf<AActor> DestPointClass;			 // 0x0040(0x0008)
		uint8 Pad_48[0x30];							 // 0x0048–0x0078
		float CablePlusLengthAtFiring;				 // 0x0078(0x0004)
		float CablePlusLengthAtPulling;				 // 0x007C(0x0004)
		float CablePlusLengthAtBacking;				 // 0x0080(0x0004)
		float HookRadius;							 // 0x0084(0x0004)
		float HookFireSpeed;						 // 0x0088(0x0004)
		float HookCancelSpeed;						 // 0x008C(0x0004)
		float PullingSpeed;							 // 0x0090(0x0004)
		float CancelLaunchForce;					 // 0x0094(0x0004)
		float FireReadyStateLimitTIme;				 // 0x0098(0x0004)
		float PullingAnimConditionMinDistance[3];	 // 0x009C(0x000C)
		FVector2D PullingAnimConditionAimPitchRange; // 0x00A8(0x0008)
		float ReadyToPullStateLimitTIme;             // 0x00B0(0x0004)
		float DetectTimeForOwnerMoveBlocked;         // 0x00B4(0x0004)
		float DetectMinSpeedForOwnerMoveBlocked;     // 0x00B8(0x0004)
		float LandingJumpUpForce;                    // 0x00BC(0x0004)
		float LandingJumpForwardForce;               // 0x00C0(0x0004)
		float DownActionPreLandingDuration;          // 0x00C4(0x0004)
		float CorrectSphereRadius;                   // 0x00C8(0x0004)
		float CorrectSweepDistance;                  // 0x00CC(0x0004)
		float CameraTurnScale;                       // 0x00D0(0x0004)
		float CameraLookUpScale;                     // 0x00D4(0x0004)
		uint8 Pad_D8[0x10];                          // 0x00D8–0x00E8
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1WireCasterAttribute">();
		}
		static class UM1WireCasterAttribute* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1WireCasterAttribute>();
		}
	};


	// Class M1.M1WireDestination
	// 0x00E8 (0x0330 - 0x0248)
	class AM1WireDestination : public AActor
	{
	public:
		uint8 Pad_248[0x08];                                       // 0x0248(0x0008)
		bool bValidLandingSpot;                                    // 0x0250(0x0001)
		bool bOverlappedAroundLandingBox;                          // 0x0251(0x0001)
		bool bOverlappedLandingBox;                                // 0x0252(0x0001)
		bool bAllowedAirDestination;                               // 0x0253(0x0001)
		uint8 LastestDestType[0x1];                                // 0x0254(0x0001)
		uint8 Pad_255[0x3];                                        // 0x0255(0x0003)
		FVector HitComponentLocationOffset;                        // 0x0258(0x000C)
		FVector PullingTerrainDestOffest;                          // 0x0264(0x000C)
		FHitResult HitInfo;                                        // 0x0270(0x009C)
		uint8 Pad_30C[0x24];                                       // 0x030C(0x0024)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1WireDestination">();
		}
		static class AM1WireDestination* GetDefaultObj()
		{
			return GetDefaultObjImpl<AM1WireDestination>();
		}
	};

	// Class M1.M1SkillAbility
	// 0x0098 (0x0340 - 0x02A8)
	class UM1SkillAbility : public UM1Ability
	{
	public:
		uint8 Pad_2A8[0x98];                          // 0x02A8(0x0098)
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1SkillAbility">();
		}
		static class UM1SkillAbility* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1SkillAbility>();
		}
	};

	// Class M1.M1WireCaster
	// 0x0130 (0x0378 - 0x0248)
	class AM1WireCaster : public AActor
	{
	public:
		uint8 Pad_248[0x8];												   // 0x0248(0x0008)
		TMulticastInlineDelegate<void(EM1WireState)> OnChangedWireState;   // 0x0250
		class USkeletalMeshComponent* Mesh;                                // 0x0260
		class UM1WireCasterAttribute* Attribute;                           // 0x0268
		class AM1Player* PlayerOwner;                                      // 0x0270
		class AM1WireHook* HookActor;                                      // 0x0278
		class AM1WireDestination* DestPoint;                               // 0x0280
		class UM1InteractionSubsystem* Interaction_Subsystem;              // 0x0288
		uint8 Pad_290[0x4];                                                // 0x0290
		EM1WireState WireState;                                            // 0x0294
		bool bValidLandingSpot;                                            // 0x0295
		bool bPullingLong;                                                 // 0x0296
		uint8 Pad_297[0x11];                                               // 0x0297
		class USkeletalMesh* DefaultMeshAsset;                             // 0x02A8
		uint8 Pad_2B0[0xC8];                                               // 0x02B0
	public:
		bool IsValidLandingSpot() const;
	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1WireCaster">();
		}
		static class AM1WireCaster* GetDefaultObj()
		{
			return GetDefaultObjImpl<AM1WireCaster>();
		}
	};

	class UM1PlayerRoundsComponent final : public UM1CharacterComponent
	{
	public:
		uint8                                         Pad_CurrentSpareRounds[0x10];						 // 0x00D0(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		TArray<int32>                                 CurrentSpareRounds;                                // 0x00E0(0x0010)(Net, ZeroConstructor, NativeAccessSpecifierPrivate)
		TMap<EM1RoundsType, FCachedMaxCapacity>		  CachedMaxCapacities;								 // 0x00F0(0x0050)(Net, ZeroConstructor, Transient, NativeAccessSpecifierPrivate)
		uint8                                         Pad_UM1PlayerRoundsComponent[0x28];                // 0x0140(0x0078)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};
	static_assert(offsetof(UM1PlayerRoundsComponent, Pad_CurrentSpareRounds) == 0x00D0, "Bad alignment");
	static_assert(offsetof(UM1PlayerRoundsComponent, CurrentSpareRounds) == 0x00E0, "Bad alignment");
	static_assert(offsetof(UM1PlayerRoundsComponent, CachedMaxCapacities) == 0x00F0, "Bad alignment");
	static_assert(offsetof(UM1PlayerRoundsComponent, Pad_UM1PlayerRoundsComponent) == 0x0140, "Bad alignment");

	// Class M1.M1WireSkillAbility
	// 0x0010 (0x0350 - 0x0340)
	class UM1WireSkillAbility : public UM1SkillAbility
	{
	public:
		float                                         FireMaxDistance;                                   // 0x0340(0x0004)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		float                                         FireMinDistance;                                   // 0x0344(0x0004)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		bool                                          bAllowedAirDestination;                            // 0x0348(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, Protected, HasGetValueTypeHash, NativeAccessSpecifierProtected)
		uint8                                         Pad_349[0x7];                                      // 0x0349(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1WireSkillAbility">();
		}
		static class UM1WireSkillAbility* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1WireSkillAbility>();
		}
	};

	// 0x0014 (0x0014 - 0x0000)
	struct FM1CoolTimer final
	{
	public:
		class FName                                   CoolTimeContext;                                   // 0x0000(0x0008)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		bool                                          bActivated;                                        // 0x0008(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		uint8                                         Pad_9[0x3];                                        // 0x0009(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
		float                                         CoolTimeDuration;                                  // 0x000C(0x0004)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		float                                         RemainingCoolTime;                                 // 0x0010(0x0004)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
	};

	// 0x0010 (0x0340 - 0x0330)
	class AM1StatBasedActor : public AM1Actor
	{
	public:
		uint8                                         Pad_AM1StatBasedActor_Class[0x10];                  // 0x0330(0x0008)(ExportObject, ZeroConstructor, Transient, InstancedReference, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
	};

	// 0x0400 (0x0740 - 0x0340)
	class AM1MissionTargetActor : public AM1StatBasedActor
	{
	public:
		uint8										  Pax_CurrentState[0x284];													// 0x0340					
		EM1MissionTargetState                         CurrentState; 															// 0x05C4(0x0001)
		uint8                                         Pad_AM1MissionTargetActor_Class[0x17B];                                   // 0x05C5
	};

	// 0x0000 (0x0040 - 0x0040)
	class UM1MissionTaskServiceInteraction final : public UM1MissionTaskService
	{
	public:
		void ServerRequestMissionTargetBeginInteraction(class AM1MissionTargetInteraction* InActor, class AM1PlayerControllerInGame* InAcceptor);
	};

	// 0x0028 (0x0768 - 0x0740)
	class AM1MissionTargetInteraction final : public AM1MissionTargetActor
	{
	public:
		uint8                                         Pad_AM1MissionTargetInteraction_Class[0x28];                                      // 0x0740
	};

	struct FM1ScaledInteger final
	{
	public:
		int64                                         Value;                                             // 0x0000(0x0008)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
	};

	// ScriptStruct NetCore.FastArraySerializerItem
	// 0x000C (0x000C - 0x0000)
	struct FFastArraySerializerItem
	{
	public:
		int32                                         ReplicationID;                                     // 0x0000(0x0004)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32                                         ReplicationKey;                                    // 0x0004(0x0004)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32                                         MostRecentArrayReplicationKey;                     // 0x0008(0x0004)(ZeroConstructor, IsPlainOldData, RepSkip, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// ScriptStruct M1.M1StatTypeAndValue
	// 0x000C (0x0018 - 0x000C)
	struct FM1StatTypeAndValue final : public FFastArraySerializerItem
	{
	public:
		EM1StatType                                   StatType;                                          // 0x000C(0x0001)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_D[0x3];                                        // 0x000D(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
		struct FM1ScaledInteger                       Value;                                             // 0x0010(0x0008)(Edit, BlueprintVisible, BlueprintReadOnly, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// 0x0448 (0x0510 - 0x00C8)
	class UM1StatComponent : public UM1ActorComponent
	{
	public:
		struct FM1ScaledInteger GetStatValue(const EM1StatType InStatType) const;

	};

	// Class M1.M1PlayerStatComponent
	// 0x01A8 (0x06C8 - 0x0520)
	class UM1PlayerStatComponent final : public UM1StatComponent
	{
	public:
		uint8                                         Pad_520[0x190];                                    // 0x0520(0x0190)(Fixing Size After Last Property [ Dumper-7 ])
		class AM1Player* Player_Owner;                                      // 0x06B0(0x0008)(ZeroConstructor, Transient, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
		TArray<struct FM1StatTypeAndValue>            IgnoreOverrideGaugeStats;                          // 0x06B8(0x0010)(Net, ZeroConstructor, NativeAccessSpecifierPrivate)

	public:
		static class UClass* StaticClass()
		{
			return StaticClassImpl<"M1PlayerStatComponent">();
		}
		static class UM1PlayerStatComponent* GetDefaultObj()
		{
			return GetDefaultObjImpl<UM1PlayerStatComponent>();
		}
	};


	struct M1PrivateOnlineServiceComponent_ServerChangePlayer final
	{
	public:
		struct FM1TemplateId InCharacterTid;
	};

	struct M1Character_IsDead final
	{
	public:
		bool ReturnValue;
	};

	struct M1Player_RequestTeleportAtSequence final
	{
	public:
		struct FVector InLocation;
		struct FRotator InRotation;
	};

	struct Actor_K2_SetActorLocation final
	{
	public:
		struct FVector                                NewLocation;
		bool                                          bSweep;
		uint8                                         Pad_357[0x3];
		struct FHitResult                             SweepHitResult;
		bool                                          bTeleport;
		bool                                          ReturnValue;
		uint8                                         Pad_358[0x2];
	};

	struct Actor_GetDistanceTo final
	{
	public:
		const class AActor* OtherActor;                                        // 0x0000(0x0008)(ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         ReturnValue;                                       // 0x0008(0x0004)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_373[0x4];                                      // 0x000C(0x0004)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct Actor_K2_GetActorLocation final
	{
	public:
		struct FVector                                ReturnValue;                                       // 0x0000(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct Controller_SetControlRotation final
	{
	public:
		struct FRotator                               NewRotation;                                       // 0x0000(0x000C)(ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	struct Controller_LineOfSightTo final
	{
	public:
		const class AActor* Other;                                             // 0x0000(0x0008)(ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector                                ViewPoint;                                         // 0x0008(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          bAlternateChecks;                                  // 0x0014(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          ReturnValue;                                       // 0x0015(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_767[0x2];                                      // 0x0016(0x0002)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct PlayerCameraManager_GetCameraLocation final
	{
	public:
		struct FVector                                ReturnValue;                                       // 0x0000(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct PlayerCameraManager_GetCameraRotation final
	{
	public:
		struct FRotator                               ReturnValue;                                       // 0x0000(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	struct PlayerController_GetMousePosition final
	{
	public:
		float                                         LocationX;                                         // 0x0000(0x0004)(Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         LocationY;                                         // 0x0004(0x0004)(Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          ReturnValue;                                       // 0x0008(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_7B4[0x3];                                      // 0x0009(0x0003)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct ActorComponent_IsActive final
	{
	public:
		bool                                          ReturnValue;                                       // 0x0000(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct SceneComponent_K2_GetComponentToWorld final
	{
	public:
		struct FTransform                             ReturnValue;                                       // 0x0000(0x0030)(Parm, OutParm, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct SkinnedMeshComponent_GetBoneName final
	{
	public:
		int32                                         BoneIndex;                                         // 0x0000(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class FName                                   ReturnValue;                                       // 0x0004(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct GameplayStatics_GetPlayerController final
	{
	public:
		const class UObject* WorldContextObject;                                // 0x0000(0x0008)(ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32                                         PlayerIndex;                                       // 0x0008(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_CD8[0x4];                                      // 0x000C(0x0004)(Fixing Size After Last Property [ Dumper-7 ])
		class APlayerController* ReturnValue;                                       // 0x0010(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct GameplayStatics_ProjectWorldToScreen final
	{
	public:
		class APlayerController* Player;                                            // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector                                WorldPosition;                                     // 0x0008(0x000C)(ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector2D                              ScreenPosition;                                    // 0x0014(0x0008)(Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          bPlayerViewportRelative;                           // 0x001C(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          ReturnValue;                                       // 0x001D(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_CF4[0x2];                                      // 0x001E(0x0002)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct GameplayStatics_GetWorldDeltaSeconds final
	{
	public:
		const class UObject* WorldContextObject;                                // 0x0000(0x0008)(ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		double                                        ReturnValue;                                       // 0x0008(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct KismetMathLibrary_Conv_MatrixToTransform final
	{
	public:
		struct FMatrix                                InMatrix;                                          // 0x0000(0x0040)(ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
		struct FTransform                             ReturnValue;                                       // 0x0040(0x0030)(Parm, OutParm, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct KismetMathLibrary_Conv_TransformToMatrix final
	{
	public:
		struct FTransform                             Transform;                                         // 0x0000(0x0030)(ConstParm, Parm, OutParm, ReferenceParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FMatrix                                ReturnValue;                                       // 0x0030(0x0040)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	struct KismetMathLibrary_FindLookAtRotation final
	{
	public:
		struct FVector                                Start;                                             // 0x0000(0x000C)(ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector                                Target;                                            // 0x000C(0x000C)(ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FRotator                               ReturnValue;                                       // 0x0018(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	struct KismetMathLibrary_Multiply_MatrixMatrix final
	{
	public:
		struct FMatrix                                A;                                                 // 0x0000(0x0040)(ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
		struct FMatrix                                B;                                                 // 0x0040(0x0040)(ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
		struct FMatrix                                ReturnValue;                                       // 0x0080(0x0040)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	struct KismetMathLibrary_RInterpTo final
	{
	public:
		struct FRotator                               Current;                                           // 0x0000(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
		struct FRotator                               Target;                                            // 0x000C(0x000C)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
		float                                         DeltaTime;                                         // 0x0018(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         InterpSpeed;                                       // 0x001C(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FRotator                               ReturnValue;                                       // 0x0020(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	struct KismetMathLibrary_Distance2D final
	{
	public:
		struct FVector2D                              V1;                                                // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector2D                              V2;                                                // 0x0008(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		double                                        ReturnValue;                                       // 0x0010(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct Canvas_K2_DrawLine final
	{
	public:
		struct FVector2D                              ScreenPositionA;                                   // 0x0000(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector2D                              ScreenPositionB;                                   // 0x0008(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         Thickness;                                         // 0x0010(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FLinearColor                           RenderColor;                                       // 0x0014(0x0010)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct Canvas_K2_DrawText final
	{
	public:
		class UFont* RenderFont;                                        // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class FString                                 RenderText;                                        // 0x0008(0x0010)(Parm, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector2D                              ScreenPosition;                                    // 0x0018(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector2D                              Scale;                                             // 0x0020(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FLinearColor                           RenderColor;                                       // 0x0028(0x0010)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		float                                         Kerning;                                           // 0x0038(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FLinearColor                           ShadowColor;                                       // 0x003C(0x0010)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector2D                              ShadowOffset;                                      // 0x004C(0x0008)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          bCentreX;                                          // 0x0054(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          bCentreY;                                          // 0x0055(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          bOutlined;                                         // 0x0056(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_E8E[0x1];                                      // 0x0057(0x0001)(Fixing Size After Last Property [ Dumper-7 ])
		struct FLinearColor                           OutlineColor;                                      // 0x0058(0x0010)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1DropContainer_IsObtained final
	{
	public:
		bool                                          ReturnValue;                                       // 0x0000(0x0001)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct SkinnedMeshComponent_GetNumBones final
	{
	public:
		int32                                         ReturnValue;                                       // 0x0000(0x0004)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1MiniGame_OnMiniGameFinished final
	{
	public:
		EM1MiniGameResult                             InResult;                                          // 0x0000(0x0001)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1LocalGameInstanceSubsystem_Get final
	{
	public:
		const class UObject* WorldContextObject;                                // 0x0000(0x0008)(ConstParm, Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class UM1LocalGameInstanceSubsystem* ReturnValue;                                       // 0x0008(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	enum class EM1MissionJoinContext : uint8
	{
		None = 0,
		Intentionally = 1,
		ByChance = 2,
		ByReserved = 3,
		EM1MissionJoinContext_MAX = 4,
	};

	struct FM1MissionJoinContextParam final
	{
	public:
		EM1MissionJoinContext                         JoinContext;                                       // 0x0000(0x0001)(ZeroConstructor, Transient, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
	};

	struct M1MissionControlComponent_ServerJoinAndBeginMissionBy final
	{
	public:
		class AM1MissionActor* InMission;                                         // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1MissionControlComponent_ServerJoinMissionBy final
	{
	public:
		class AM1MissionActor* InMission;                                         // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FM1MissionJoinContextParam             InContext;                                         // 0x0008(0x0001)(ConstParm, Parm, ReferenceParm, NoDestructor, NativeAccessSpecifierPublic)
		uint8                                         Pad_9[0x7];                                        // 0x0009(0x0007)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct M1MissionControlComponent_ServerJumpToSpecificTask final
	{
	public:
		int32                                         InTaskIndex;                                       // 0x0000(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1MissionControlComponent_ServerStartMission final
	{
	public:
		class AM1MissionActor* InMission;                                         // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1MissionControlComponent_ServerStartMissionByTemplateID final
	{
	public:
		struct FM1TemplateId                          InTemplateId;                                      // 0x0000(0x0004)(ConstParm, Parm, ReferenceParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1MissionControlComponent_ServerTeleportToBestPlayer final
	{
	public:
		bool                                          bIgnoreDistance;                                   // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// Function M1.M1MissionControlComponent.ServerTrySucceedInMission
	// 0x0001 (0x0001 - 0x0000)
	struct M1MissionControlComponent_ServerTrySucceedInMission final
	{
	public:
		bool                                          InNoDelay;                                         // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// Function M1.M1MissionControlComponent.TryJoinMissionBy
	// 0x0008 (0x0008 - 0x0000)
	struct M1MissionControlComponent_TryJoinMissionBy final
	{
	public:
		class AM1MissionActor* InMission;                                         // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// Function M1.M1MissionControlComponent.TryStartMission
	// 0x0008 (0x0008 - 0x0000)
	struct M1MissionControlComponent_TryStartMission final
	{
	public:
		class FName                                   InName;                                            // 0x0000(0x0008)(ConstParm, Parm, OutParm, ZeroConstructor, ReferenceParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	// Function M1.M1MissionControlComponent.TryTeleportToBestPlayer
	// 0x0001 (0x0001 - 0x0000)
	struct M1MissionControlComponent_TryTeleportToBestPlayer final
	{
	public:
		bool                                          bIgnoreDistance;                                   // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1PlayerState_ServerCheatSetIsCreator final
	{
	public:
		bool                                          InbCreator;                                        // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1TaskEventActor_BP_RunResult final
	{
	public:
		EM1TaskEventActorRunResult                    InRunResult;                                       // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1TaskEventActor_TaskActorStateChanged final
	{
	public:
		EM1EventActorState                            InState;                                           // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct Actor_K2_GetActorRotation final
	{
	public:
		struct FRotator                               ReturnValue;                                       // 0x0000(0x000C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	struct KismetStringLibrary_Conv_StringToName final
	{
	public:
		class FString                                 InString;                                          // 0x0000(0x0010)(Parm, ZeroConstructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class FName                                   ReturnValue;                                       // 0x0010(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct GameplayStatics_SpawnObject final
	{
	public:
		TSubclassOf<class UObject>                    ObjectClass;                                       // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class UObject* Outer_0;                                           // 0x0008(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class UObject* ReturnValue;                                       // 0x0010(0x0008)(Parm, OutParm, ZeroConstructor, ReturnParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1StatComponent_GetStatValue final
	{
	public:
		EM1StatType                                   InStatType;                                        // 0x0000(0x0001)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_1[0x7];                                        // 0x0001(0x0007)(Fixing Size After Last Property [ Dumper-7 ])
		struct FM1ScaledInteger                       ReturnValue;                                       // 0x0008(0x0008)(Parm, OutParm, ReturnParm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1PlayerControllerInGame_ServerRequestFieldObjectDropItems final
	{
	public:
		class AM1FieldInteractableActor* InActor;                                           // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1MiniGameActor_ServerDropItems final
	{
	public:
		class AController* InInstigator;                                      // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1MiniGameActor_ServerOnMiniGameEnded final
	{
	public:
		struct FM1MiniGameResult                      InResult;                                          // 0x0000(0x000C)(ConstParm, Parm, ReferenceParm, NoDestructor, NativeAccessSpecifierPublic)
	};

	struct Widget_SetIsEnabled final
	{
	public:
		bool                                          bInIsEnabled;                                      // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct Widget_SetVisibility final
	{
	public:
		ESlateVisibility                              InVisibility;                                      // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct Actor_GetActorBounds final
	{
	public:
		bool                                          bOnlyCollidingComponents;                          // 0x0000(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_1[0x3];                                        // 0x0001(0x0003)(Fixing Size After Last Property [ Dumper-7 ])
		struct FVector                                Origin;                                            // 0x0004(0x000C)(Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FVector                                BoxExtent;                                         // 0x0010(0x000C)(Parm, OutParm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		bool                                          bIncludeFromChildActors;                           // 0x001C(0x0001)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_1D[0x3];                                       // 0x001D(0x0003)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct SkeletalMesh_GetBounds final
	{
	public:
		struct FBoxSphereBounds                       ReturnValue;                                       // 0x0000(0x001C)(Parm, OutParm, ZeroConstructor, ReturnParm, IsPlainOldData, NoDestructor, NativeAccessSpecifierPublic)
	};

	struct M1MiniGameActor_ClientStartMiniGame final
	{
	public:
		struct FM1TemplateId                          InMiniGameTid;                                     // 0x0000(0x0004)(ConstParm, Parm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FM1TemplateId                          InPlayerTid;                                       // 0x0004(0x0004)(ConstParm, Parm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		struct FM1TemplateId                          InFieldDifficultyTid;                              // 0x0008(0x0004)(ConstParm, Parm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		EM1MiniGameDifficulty                         InDifficulty;                                      // 0x000C(0x0001)(ConstParm, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		uint8                                         Pad_D[0x3];                                        // 0x000D(0x0003)(Fixing Struct Size After Last Property [ Dumper-7 ])
	};

	struct M1PrivateOnlineServicePreset_ServerRequestApplyPreset final
	{
	public:
		int32                                         InPresetIndex;                                     // 0x0000(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1PrivateOnlineServiceResearch_ServerRequestStartResearch final
	{
	public:
		struct FM1TemplateId                          InResearchTemplateId;                              // 0x0000(0x0004)(Parm, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		int32                                         InRepeatCount;                                     // 0x0004(0x0004)(Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1MissionTaskServiceInteraction_ServerRequestMissionTargetBeginInteraction final
	{
	public:
		class AM1MissionTargetInteraction* InActor;                                           // 0x0000(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
		class AM1PlayerControllerInGame* InAcceptor;                                        // 0x0008(0x0008)(Parm, ZeroConstructor, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	};

	struct M1WireCaster_ServerStartWireAction final
	{
		FHitResult InHit;
		float InAimPitch;
		FVector InDestOffest;
		FVector InTerrainOffset;
	};

	struct M1WireCaster_NetMulticast_FireWireHook final
	{
		FHitResult InHit;
		FVector InDestOffest;
		FVector InTerrainOffset;
		bool bAllowedAir;
	};

	struct M1WireCaster_RequestStartAction final
	{
	};

	struct M1SkillAbility_ClearCooltime final
	{
	};

	struct M1Player_IsInAir final
	{
		bool                                          ReturnValue;
	};

	struct M1Player_IsMovingByWire final
	{
		bool                                          ReturnValue;
	};
}