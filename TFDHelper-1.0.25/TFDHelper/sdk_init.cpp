#include "sdk.h"
#include "cheat.h"
#include <TlHelp32.h>

//#define IS_DEBUG_VERSION

namespace TFD
{
	using namespace SDK;
	Init_Code InitSDK(int procID, uintptr_t moduleBase, uintptr_t moduleSize)
	{
		try
		{
			BASE = moduleBase;
			SIZE = moduleSize;

#ifdef IS_DEBUG_VERSION
			//Sleep(1000);
			std::cout << "[Cheat] Base: 0x" << std::hex << BASE << " - Size: 0x" << SIZE << std::dec << std::endl;
			std::cout << "[Cheat] Searching for GNames.\n";
#endif
			uintptr_t GNamePtr = SearchSignature(procID, moduleBase, moduleSize, GNamesSig, GNamesMask);
			if (!GNamePtr)
			{
#ifdef IS_DEBUG_VERSION
				std::cout << "[Cheat] Failed to find GNames.\n";
#endif
				return Init_Code::GNames;
			}
			GNamePtr = moduleBase + GNamePtr;
			uintptr_t GNameOffsetAddress = GNamePtr + 0x3;
			uintptr_t GNameOffsetRelative = *reinterpret_cast<uintptr_t*>(GNameOffsetAddress);
			uintptr_t GNameAddress = (GNamePtr + 0x7) + GNameOffsetRelative;
			int GNameOffset = GNameAddress - moduleBase;
			SDK::Offsets::GNames = GNameOffset;

			SDK::FName::InitInternal();
		
#ifdef IS_DEBUG_VERSION
			std::cout << "[Cheat] GName Offset: " << std::hex << SDK::Offsets::GNames << std::dec << "\n";
			std::cout << "[Cheat] GName Addr: " << std::hex << reinterpret_cast<void*>(moduleBase + SDK::Offsets::GNames) << std::dec << "\n";
			//Sleep(1000);
#endif
			int GObjsPtr = SearchForGObjects(moduleBase);
			if (!GObjsPtr)
			{
#ifdef IS_DEBUG_VERSION
				std::cout << "[Cheat] Failed to find GObjects.\n";
#endif
				return Init_Code::GObjects;
			}
			SDK::Offsets::GObjects = GObjsPtr;
			SDK::UObject::GObjects.InitGObjects();
#ifdef IS_DEBUG_VERSION
			std::cout << "[Cheat] GObj Offset: " << std::hex << SDK::Offsets::GObjects << std::dec << "\n";
			std::cout << "[Cheat] GObj Addr: " << std::hex << reinterpret_cast<void*>(moduleBase + SDK::Offsets::GObjects) << std::dec << "\n";
			//Sleep(1000);
#endif

			TFD::UM1LocalGameInstanceSubsystem* SubSystem = nullptr;
			bool NGSBypass = false;
			while (!NGSBypass)
			{
				for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
				{
					SDK::UObject* Obj = SDK::UObject::GObjects->GetByIndex(i);
					if (!Obj)
						continue;

					if (Obj->IsA(TFD::UM1LocalGameInstanceSubsystem::StaticClass()) && !Obj->IsDefaultObject())
					{
						SubSystem = static_cast<TFD::UM1LocalGameInstanceSubsystem*>(Obj);
						if (SubSystem)
						{
							if (SubSystem->NgsSystem)
							{
								std::cout << "[Cheat] NgsSystem Before: " << SubSystem->NgsSystem->bEnabled << " - " << SubSystem->NgsSystem->bStartCompleted << " - " << SubSystem->NgsSystem->bStarted << " - " << SubSystem->NgsSystem->bInitializeCompleted << "\n";
								SubSystem->NgsSystem->bEnabled = true;
								SubSystem->NgsSystem->bStartCompleted = true;
								SubSystem->NgsSystem->bStarted = true;
								SubSystem->NgsSystem->bInitializeCompleted = true;

								std::cout << "[Cheat] NgsSystem After: " << SubSystem->NgsSystem->bEnabled << " - " << SubSystem->NgsSystem->bStartCompleted << " - " << SubSystem->NgsSystem->bStarted << " - " << SubSystem->NgsSystem->bInitializeCompleted << "\n";
								NGSBypass = true;
							}
						}
					}
				}
				Sleep(1);
			}



			/* NOSPREAD AND RECOIL CAN BE REPLACED WITH THE CODE NOOBGUNNER SHARED IN DISCORD */
//			uintptr_t SpreadPtr = SearchSignature(procID, moduleBase, moduleSize, NoSpreadSig, NoSpreadMask);
//			if (!SpreadPtr)
//			{
//#ifdef IS_DEBUG_VERSION
//				std::cout << "[Cheat] Failed to find NoSpread.\n";
//#endif
//			}
//			else
//			{
//				SpreadPtr = moduleBase + SpreadPtr;
//				NoSpreadAddress = (reinterpret_cast<uint8_t*>(SpreadPtr) - 0x9);
//#ifdef IS_DEBUG_VERSION
//				std::cout << "[Cheat] Found NoSpread: " << std::hex << SpreadPtr << std::dec << "\n";
//				//Sleep(1000);
//#endif
//			}
//			uintptr_t RecoilPtr = SearchSignature(procID, moduleBase, moduleSize, NoRecoilSig, NoRecoilMask);
//			if (!RecoilPtr)
//			{
//#ifdef IS_DEBUG_VERSION
//				std::cout << "[Cheat] Failed to find NoRecoil.\n";
//#endif
//			}
//			else
//			{
//				RecoilPtr = moduleBase + RecoilPtr;
//				NoRecoilAddress = reinterpret_cast<uint8_t*>(RecoilPtr + 2);
//#ifdef IS_DEBUG_VERSION
//				std::cout << "[Cheat] Found NoRecoil: " << std::hex << RecoilPtr << std::dec << "\n";
//#endif
//			}

#ifdef IS_DEBUG_VERSION
			std::cout << "[Cheat] Searching for GWorld.\n";
#endif
			while (!SDK::Offsets::GWorld)
			{
				SearchForGWorld(moduleBase, moduleSize);
				if (!SDK::Offsets::GWorld)
				{
					Sleep(1);
				}
			}

			

			/* NATIVE TFD GAME FUNCTIONS NOT EXPOSED BY SDK*/
			/*uintptr_t FindFuncPtr = moduleBase + 0x419D8B0;
			TFD::native_FindFunctionChecked = (TFD::tFindFunctionChecked)FindFuncPtr;

			uintptr_t NativeFunc = moduleBase + 0x34765B0;
			TFD::native_InitCheatManager = (TFD::tInitCheatManager)NativeFunc;

			uintptr_t CreateCheatWidgetPtr = moduleBase + 0x3950D70;
			TFD::native_CreateCheatWidget = (TFD::tCreateCheatWidget)CreateCheatWidgetPtr;

			uintptr_t ActorMiniGameBeginPlayPtr = SearchSignature(procID, moduleBase, moduleSize, ActorMiniGamePlaySig, ActorMiniGamePlayMask);
			if (ActorMiniGameBeginPlayPtr)
			{
				std::cout << "[Cheat] Found MinigamePlay\n";
				TFD::native_ActorMiniGameBeginPlay = (TFD::tActorMiniGameBeginPlay)(moduleBase + ActorMiniGameBeginPlayPtr);
			}

			uintptr_t CanInteractPtr = SearchSignature(procID, moduleBase, moduleSize, CanInteractSig, CanInteractMask);
			if (CanInteractPtr)
			{
				CanInteractPtr = moduleBase + CanInteractPtr;
				if (MH_CreateHook(reinterpret_cast<void*>(CanInteractPtr), &hook_InteractableActorCanActivate, reinterpret_cast<void**>(&native_oInteractableActorCanActivate)) || MH_EnableHook(reinterpret_cast<void*>(CanInteractPtr)) != MH_OK) {
					std::cout << "[Cheat] Failed to hook CanInteract\n";
				}
			}*/
			uintptr_t GetAccountPtr = SearchSignature(procID, moduleBase, moduleSize, UMPOSCGetAccountSig, UMPOSCGetAccountMask);
			if (GetAccountPtr)
			{
				GetAccountPtr = moduleBase + GetAccountPtr;
				TFD::native_GetUM1Account = (TFD::tUMPOSCGetAccount)GetAccountPtr;
			}
			else
				std::cout << "[Cheat] Failed to get UM1PrivateOnlineServiceComponent::GetAccount from pattern. \n";

			Sleep(1000);

			//SDK::Offsets::GWorld = GWorldPtr;
#ifdef IS_DEBUG_VERSION
			std::cout << "[Cheat] Found GWorld Offset: 0x" << std::hex << SDK::Offsets::GWorld << std::dec << "\n";
			std::cout << "[Cheat] Hooking PostRender.\n";
#endif
			bool isPostRenderHooked = false;
			do
			{
				//TFD::UWorld* world = *reinterpret_cast<TFD::UWorld**>(SDK::Offsets::GWorld);
				TFD::UWorld* world = TFD::UWorld::GetWorld();
				if (world && world->IsA(TFD::UWorld::StaticClass()))
				{
					//Sleep(1000);
					TFD::ULocalPlayer* LocalPl = (TFD::ULocalPlayer*)((TFD::UGameplayStatics*)TFD::UGameplayStatics::StaticClass())->GetPlayerController(world, 0)->Player;
					if (LocalPl)
					{
						//Sleep(1000);
						TFD::UGameViewportClient* ViewportClient = LocalPl->ViewportClient;
						if (ViewportClient)
						{
							//Sleep(1000);
							ULONG64* Func = GetPostRender((void*)ViewportClient);
							if (Func)
							{
								Cheat::oPostRender = (decltype(Cheat::oPostRender))*Func;
								//std::cout << std::hex << "Func " << Func << std::endl;
								//std::cout << std::hex << "oPostRender " << Cheat::oPostRender << std::endl;
								__int64 ptr = (__int64)Cheat::PostRender;
								//std::cout << std::hex << "My PostRender " << ptr << std::endl;
								DWORD old;
								VirtualProtect(Func, sizeof(void*), PAGE_EXECUTE_READWRITE, &old);
								memcpy(Func, &ptr, 8);
								VirtualProtect(Func, sizeof(void*), old, NULL);

								isPostRenderHooked = true;
							}
						}
					}
				}
				else
				{
					//Sleep(1000);
				}
			} while (!isPostRenderHooked);
#ifdef IS_DEBUG_VERSION
			std::cout << "[Cheat] PostRender hooked.\n";
#endif

			CFG::LoadCFG();

			return Init_Code::Success;
		}
		catch (...)
		{
			return Init_Code::FatalError;

		}

	}

	uintptr_t SearchForGObjects(uintptr_t base)
	{
		uintptr_t SearchBase;
		DWORD SearchRange;
		const auto [DataSection, DataSize] = GetSectionByName(base, ".data");

		if (DataSection != 0x0 && DataSize != 0x0)
		{
			SearchBase = DataSection;
			SearchRange = DataSize;
		}
		else
		{
			return 0;
		}

		SearchRange -= 0x50;

		for (int i = 0; i < SearchRange; i += 0x4)
		{
			const uintptr_t CurrentAddress = SearchBase + i;

			if (IsAddressValidGObjects(CurrentAddress))
			{
				return (SearchBase + i) - base;

			}
		}
		return 0;
	}

	void SearchForGWorld(uintptr_t base, uintptr_t size)
	{
		uintptr_t SearchBase;
		DWORD SearchRange;
		const auto [DataSection, DataSize] = GetSectionByName(base, ".data");

		if (DataSection != 0x0 && DataSize != 0x0)
		{
			SearchBase = DataSection;
			SearchRange = DataSize;
		}
		else
		{
			return;
		}

		SearchRange -= 0x50;
		for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
		{
			SDK::UObject* obj = SDK::UObject::GObjects->GetByIndex(i);

			if (!obj || (obj->Flags & SDK::EObjectFlags::ClassDefaultObject))
				continue;

			if (obj->IsA(TFD::UWorld::StaticClass()))
			{
				void* worldPtr = static_cast<void*>(obj);
				void* Result = FindAlignedValueInProcess(worldPtr);

				if (Result)
				{
					SDK::Offsets::GWorld = reinterpret_cast<uintptr_t>(Result) - base;
					break;
				}

			}
		}
	}

	template<typename T>
	T* FindAlignedValueInProcess(T Value, const std::string& Sectionname, int32_t Alignment, bool bSearchAllSections)
	{
		uintptr_t SearchStart = BASE;
		uintptr_t SearchRange = SIZE;

		if (!bSearchAllSections)
		{
			const auto [SectionStart, SectionSize] = GetSectionByName(BASE, Sectionname);

			if (SectionStart != 0x0 && SectionSize != 0x0)
			{
				SearchStart = SectionStart;
				SearchRange = SectionSize;
			}
			else
			{
				bSearchAllSections = true;
			}
		}

		T* Result = FindAlignedValueInProcessInRange(Value, Alignment, SearchStart, SearchRange);

		if (!Result && SearchStart != BASE)
			return FindAlignedValueInProcess(Value, Sectionname, Alignment, true);

		return Result;
	}

	template<typename T>
	T* FindAlignedValueInProcessInRange(T Value, int32_t Alignment, uintptr_t StartAddress, uint32_t Range)
	{
		constexpr int32_t ElementSize = sizeof(T);

		for (uint32_t i = 0x0; i < Range; i += Alignment)
		{
			T* TypedPtr = reinterpret_cast<T*>(StartAddress + i);

			if (*TypedPtr == Value)
				return TypedPtr;
		}

		return nullptr;
	}

	std::pair<uintptr_t, DWORD> GetSectionByName(uintptr_t ImageBase, const std::string& ReqestedSectionName)
	{
		if (ImageBase == 0)
			return { NULL, 0 };

		const PIMAGE_DOS_HEADER DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(ImageBase);
		const PIMAGE_NT_HEADERS NtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(ImageBase + DosHeader->e_lfanew);

		PIMAGE_SECTION_HEADER Sections = IMAGE_FIRST_SECTION(NtHeaders);

		DWORD TextSize = 0;

		for (int i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++)
		{
			IMAGE_SECTION_HEADER& CurrentSection = Sections[i];

			std::string SectionName = reinterpret_cast<const char*>(CurrentSection.Name);

			if (SectionName == ReqestedSectionName)
				return { (ImageBase + CurrentSection.VirtualAddress), CurrentSection.Misc.VirtualSize };
		}

		return { NULL, 0 };
	}

	bool IsAddressValidGObjects(const uintptr_t Address)
	{
		FChunkedFixedUObjectArrayLayout Layout;
		void* Objects = *reinterpret_cast<void**>(Address + Layout.ObjectsOffset);
		const int MaxElements = *reinterpret_cast<const int*>(Address + Layout.MaxElementsOffset);
		const int NumElements = *reinterpret_cast<const int*>(Address + Layout.NumElementsOffset);
		const int MaxChunks = *reinterpret_cast<const int*>(Address + Layout.MaxChunksOffset);
		const int NumChunks = *reinterpret_cast<const int*>(Address + Layout.NumChunksOffset);

		void** ObjectsPtrButDecrypted = reinterpret_cast<void**>(SDK::TUObjectArray::DecryptPtr(Objects));

		if (NumChunks > 0x14 || NumChunks < 0x1)
			return false;

		if (MaxChunks > 0x22F || MaxChunks < 0x6)
			return false;

		if (NumElements > MaxElements || NumChunks > MaxChunks)
			return false;

		const bool bNumChunksFitsNumElements = ((NumElements / 0x10000) + 1) == NumChunks || ((NumElements / 0x10400) + 1) == NumChunks;

		if (!bNumChunksFitsNumElements)
			return false;

		const bool bMaxChunksFitsMaxElements = (MaxElements / 0x10000) == MaxChunks || (MaxElements / 0x10400) == MaxChunks;

		if (!bMaxChunksFitsMaxElements)
			return false;

		if (!ObjectsPtrButDecrypted || IsBadReadPtr(ObjectsPtrButDecrypted))
			return false;

		for (int i = 0; i < NumChunks; i++)
		{
			if (!ObjectsPtrButDecrypted[i] || IsBadReadPtr(ObjectsPtrButDecrypted[i]))
				return false;
		}

		return true;
	}

	bool IsBadReadPtr(const void* p)
	{
		MEMORY_BASIC_INFORMATION mbi;

		if (VirtualQuery(p, &mbi, sizeof(mbi)))
		{
			constexpr DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
			bool b = !(mbi.Protect & mask);
			if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS))
				b = true;

			return b;
		}

		return true;
	};

	ULONG64* GetPostRender(void* vp)
	{
		if (vp)
		{
			__int64** vtable = NULL;
			int Index = 0;
			vtable = *reinterpret_cast<__int64***>(vp);
			if (vtable)
			{
				for (int i = 0; i < 0x200; i++)
				{
					BYTE* code = (BYTE*)vtable[i];
					for (int j = 0; j < 0x400; j++)
					{
						BYTE* p = code + j;
						if (
							(p[0] == 0x4C && p[1] == 0x8D && p[2] == 0x05) ||
							(p[0] == 0x48 && p[1] == 0x8D && p[2] == 0x15)
							)
						{
							int offset = *(int*)(p + 3);
							wchar_t* str = (wchar_t*)(p + 7 + offset);
							if (str == std::wstring(L"PAUSED"))
							{
								Index = i;
								goto ext;
							}
						}
					}
				}
			}
		ext:
			if (Index)
			{
				return (ULONG64*)&vtable[Index];
			}
		}
		return NULL;
	}

	bool MemoryCompare(const BYTE* bData, const BYTE* bSig, const char* szMask)
	{
		for (; *szMask; szMask++, bData++, bSig++)
			if (*szMask == 'x' && *bData != *bSig)
				return false;
		return true;
	}

	uintptr_t SearchSignature(int procID, uintptr_t base, uintptr_t size, const char* sig, const char* mask)
	{
		BYTE* data = new BYTE[size];
		SIZE_T bytesRead;

		Toolhelp32ReadProcessMemory(procID, (LPVOID)(base), data, size, &bytesRead);

		for (uintptr_t i = 0; i < size; i++) {
			if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
				delete[] data;
				return i;
			}
		}
		delete[] data;
		return NULL;
	}

}