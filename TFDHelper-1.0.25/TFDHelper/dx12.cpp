#include "dx12.h"
#include "render.h"
#include "menu.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//#define IS_DEBUG_VERSION




namespace DX12
{
	Present12 oPresent = nullptr;
	CreateCommandQueue oCreateCommandQueue = nullptr;
	CreateSwapChain oCreateSwapChain = nullptr;
	uint64_t* MethodsTable = nullptr;

	std::unordered_map<IDXGISwapChain*, ID3D12CommandQueue*> g_SwapChainToQueue;

	char dlldir[512] = { 0 };
	WNDCLASSEX WindowClass = { 0 };
	HWND WindowHwnd = nullptr;
	bool isOverlayReady = false;
	bool ImGui_Initialised = false;

	namespace Process {
		DWORD ID = 0;
		HANDLE Handle = nullptr;
		HWND Hwnd = nullptr;
		HMODULE Module = nullptr;
		WNDPROC WndProc = nullptr;
		int WindowWidth = 0;
		int WindowHeight = 0;
		LPCSTR Title = nullptr;
		LPCSTR ClassName = nullptr;
		LPCSTR Path = nullptr;
	}

	namespace DirectX12Interface {
		ID3D12Device* Device = nullptr;
		ID3D12DescriptorHeap* DescriptorHeapBackBuffers = nullptr;
		ID3D12DescriptorHeap* DescriptorHeapImGuiRender = nullptr;
		ID3D12GraphicsCommandList* CommandList = nullptr;
		ID3D12CommandQueue* CommandQueue = nullptr;

		uint64_t BuffersCounts = 0;
		_FrameContext* FrameContext = nullptr;
	}

	bool InitWindow() {

		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = DefWindowProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = GetModuleHandle(NULL);
		WindowClass.hIcon = NULL;
		WindowClass.hCursor = NULL;
		WindowClass.hbrBackground = NULL;
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = "MJ";
		WindowClass.hIconSm = NULL;
		RegisterClassEx(&WindowClass);
		WindowHwnd = CreateWindow(WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
		if (WindowHwnd == NULL) {
			return false;
		}
		return true;
	}

	bool DX12::DeleteWindow() {
		DestroyWindow(WindowHwnd);
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		if (WindowHwnd != NULL) {
			return false;
		}
		return true;
	}

	bool DX12::CreateHook(uint16_t Index, void** Original, void* Function) {
		assert(_index >= 0 && _original != NULL && _function != NULL);
		void* target = (void*)MethodsTable[Index];
		if (MH_CreateHook(target, Function, Original) != MH_OK || MH_EnableHook(target) != MH_OK) {
			return false;
		}
		return true;
	}

	void DisableHook(uint16_t Index) {
		assert(Index >= 0);
		MH_DisableHook((void*)MethodsTable[Index]);
	}

	void DisableAll() {
		MH_DisableHook(MH_ALL_HOOKS);
		free(MethodsTable);
		MethodsTable = NULL;
	}

	bool Init() {
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] InitWindow\n";
#endif
		if (InitWindow() == false) {
			return false;
		}

#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Getting modules.\n";
#endif
		HMODULE D3D12Module = GetModuleHandle("d3d12.dll");
		HMODULE DXGIModule = GetModuleHandle("dxgi.dll");
		if (D3D12Module == NULL || DXGIModule == NULL) {
			DeleteWindow();
			return false;
		}
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Getting DXGIFactoryAddress.\n";
#endif
		void* CreateDXGIFactory = GetProcAddress(DXGIModule, "CreateDXGIFactory");
		if (CreateDXGIFactory == NULL) {
			DeleteWindow();
			return false;
		}
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Making Factory.\n";
#endif
		IDXGIFactory* Factory;
		if (((long(__stdcall*)(const IID&, void**))(CreateDXGIFactory))(__uuidof(IDXGIFactory), (void**)&Factory) < 0) {
			DeleteWindow();
			return false;
		}
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Getting Adapter.\n";
#endif
		IDXGIAdapter* Adapter;
		if (Factory->EnumAdapters(0, &Adapter) == DXGI_ERROR_NOT_FOUND) {
			DeleteWindow();
			return false;
		}
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Getting CreateDevice.\n";
#endif
		void* D3D12CreateDevice = GetProcAddress(D3D12Module, "D3D12CreateDevice");
		if (D3D12CreateDevice == NULL) {
			DeleteWindow();
			return false;
		}
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Creating device.\n";
#endif
		ID3D12Device* Device;
		if (((long(__stdcall*)(IUnknown*, D3D_FEATURE_LEVEL, const IID&, void**))(D3D12CreateDevice))(Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&Device) < 0) {
			DeleteWindow();
			return false;
		}
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Creating command queue.\n";
#endif
		D3D12_COMMAND_QUEUE_DESC QueueDesc;
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = 0;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		QueueDesc.NodeMask = 0;

		ID3D12CommandQueue* CommandQueue;
		if (Device->CreateCommandQueue(&QueueDesc, __uuidof(ID3D12CommandQueue), (void**)&CommandQueue) < 0) {
			DeleteWindow();
			return false;
		}
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Creating command allocator.\n";
#endif
		ID3D12CommandAllocator* CommandAllocator;
		if (Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&CommandAllocator) < 0) {
			DeleteWindow();
			return false;
		}
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Creating command list.\n";
#endif
		ID3D12GraphicsCommandList* CommandList;
		if (Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&CommandList) < 0) {
			DeleteWindow();
			return false;
		}

		DXGI_RATIONAL RefreshRate;
		RefreshRate.Numerator = 60;
		RefreshRate.Denominator = 1;

		DXGI_MODE_DESC BufferDesc;
		BufferDesc.Width = 100;
		BufferDesc.Height = 100;
		BufferDesc.RefreshRate = RefreshRate;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC SampleDesc;
		SampleDesc.Count = 1;
		SampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = SampleDesc;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.OutputWindow = WindowHwnd;
		SwapChainDesc.Windowed = 1;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Creating swapchain.\n";
#endif
		IDXGISwapChain* SwapChain;
		if (Factory->CreateSwapChain(CommandQueue, &SwapChainDesc, &SwapChain) < 0) {
			DeleteWindow();
			return false;
		}
#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Copying vtable functions.\n";
#endif
		MethodsTable = (uint64_t*)::calloc(170, sizeof(uint64_t));
		memcpy(MethodsTable, *(uint64_t**)Device, 44 * sizeof(uint64_t));
		//memcpy(MethodsTable + 44, *(uint64_t**)CommandQueue, 19 * sizeof(uint64_t));
		memcpy(MethodsTable + 44 + 19, *(uint64_t**)CommandAllocator, 9 * sizeof(uint64_t));
		memcpy(MethodsTable + 44 + 19 + 9, *(uint64_t**)CommandList, 60 * sizeof(uint64_t));
		memcpy(MethodsTable + 44 + 19 + 9 + 60, *(uint64_t**)SwapChain, 18 * sizeof(uint64_t));
		memcpy(MethodsTable + 150, *(uint64_t**)Factory, 20 * sizeof(uint64_t));

		MH_Initialize();
		Device->Release();
		Device = NULL;
		CommandQueue->Release();
		CommandQueue = NULL;
		CommandAllocator->Release();
		CommandAllocator = NULL;
		CommandList->Release();
		CommandList = NULL;
		SwapChain->Release();
		SwapChain = NULL;
		DeleteWindow();
//#ifdef IS_DEBUG_VERSION
		//Sleep(1000);
		std::cout << "[DX12] Finished init.\n";
//#endif
		return true;
	}

	LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		//if (Menu::ShowMenu) {// && !Menu::IsCurrentlyBindingKey()) {
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		//	return true;
		//}
		return CallWindowProc(Process::WndProc, hwnd, uMsg, wParam, lParam);
	}

	HRESULT APIENTRY hkPresent(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags) {
		static std::chrono::steady_clock::time_point FirstHookTime;

		ID3D12CommandQueue* queue = nullptr;
		if (g_SwapChainToQueue.size() == 0)
		{
			std::cout << "[DX12] hkPreset has no command queues\n";
			return oPresent(pSwapChain, SyncInterval, Flags);
		}
		else
		{
			if (g_SwapChainToQueue.contains(pSwapChain))
			{
				queue = g_SwapChainToQueue[pSwapChain];
			}
			else
			{
				std::cout << "[DX12] hkPreset has invalid command queue\n";
			}
		}

		if(!isOverlayReady)
			return oPresent(pSwapChain, SyncInterval, Flags);


		if (!ImGui_Initialised) {
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)&DirectX12Interface::Device))) {
				ImGui::CreateContext();

				ImGuiIO& io = ImGui::GetIO(); (void)io;
				ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

				DXGI_SWAP_CHAIN_DESC Desc;
				pSwapChain->GetDesc(&Desc);
				Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
				Desc.OutputWindow = Process::Hwnd;
				Desc.Windowed = ((GetWindowLongPtr(Process::Hwnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

				DirectX12Interface::BuffersCounts = Desc.BufferCount;
				DirectX12Interface::FrameContext = new DirectX12Interface::_FrameContext[DirectX12Interface::BuffersCounts];

				D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
				DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				DescriptorImGuiRender.NumDescriptors = DirectX12Interface::BuffersCounts;
				DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				if (DirectX12Interface::Device->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&DirectX12Interface::DescriptorHeapImGuiRender)) != S_OK)
					return oPresent(pSwapChain, SyncInterval, Flags);

				ID3D12CommandAllocator* Allocator;
				if (DirectX12Interface::Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
					return oPresent(pSwapChain, SyncInterval, Flags);

				for (size_t i = 0; i < DirectX12Interface::BuffersCounts; i++) {
					DirectX12Interface::FrameContext[i].CommandAllocator = Allocator;
				}

				if (DirectX12Interface::Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&DirectX12Interface::CommandList)) != S_OK ||
					DirectX12Interface::CommandList->Close() != S_OK)
					return oPresent(pSwapChain, SyncInterval, Flags);

				D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
				DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				DescriptorBackBuffers.NumDescriptors = DirectX12Interface::BuffersCounts;
				DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				DescriptorBackBuffers.NodeMask = 1;

				if (DirectX12Interface::Device->CreateDescriptorHeap(&DescriptorBackBuffers, IID_PPV_ARGS(&DirectX12Interface::DescriptorHeapBackBuffers)) != S_OK)
					return oPresent(pSwapChain, SyncInterval, Flags);

				const auto RTVDescriptorSize = DirectX12Interface::Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = DirectX12Interface::DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

				for (size_t i = 0; i < DirectX12Interface::BuffersCounts; i++) {
					ID3D12Resource* pBackBuffer = nullptr;
					DirectX12Interface::FrameContext[i].DescriptorHandle = RTVHandle;
					pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
					DirectX12Interface::Device->CreateRenderTargetView(pBackBuffer, nullptr, RTVHandle);
					DirectX12Interface::FrameContext[i].Resource = pBackBuffer;
					RTVHandle.ptr += RTVDescriptorSize;
				}

				//int my_image_width = 0;
				//int my_image_height = 0;
				//ID3D12Resource* my_texture = NULL;

				//g_pd3dSrvDescHeapAlloc.Create(DirectX12Interface::Device, DirectX12Interface::DescriptorHeapBackBuffers);
				//g_pd3dSrvDescHeapAlloc.Alloc(&LogoCPUHandle, &LogoGPUHandle);

				//LoadTextureFromMemory(Logo, sizeof(Logo), DirectX12Interface::Device, LogoCPUHandle, &my_texture, &my_image_width, &my_image_height);

				ImGui_ImplWin32_Init(Process::Hwnd);
				ImGui_ImplDX12_Init(DirectX12Interface::Device, DirectX12Interface::BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM, DirectX12Interface::DescriptorHeapImGuiRender, DirectX12Interface::DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(), DirectX12Interface::DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
				ImGui_ImplDX12_CreateDeviceObjects();
				ImGui::GetIO().ImeWindowHandle = Process::Hwnd;

				Process::WndProc = (WNDPROC)SetWindowLongPtr(Process::Hwnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);
			}
			Menu::Init();
			ImGui_Initialised = true;
			FirstHookTime = std::chrono::steady_clock::now();
		}

		static bool RenderReady = false;
		if (!RenderReady)
		{
			auto Now = std::chrono::steady_clock::now();

			if (std::chrono::duration<double>(Now - FirstHookTime).count() < 5.0f)
				return oPresent(pSwapChain, SyncInterval, Flags);
			else
				RenderReady = true;
		}
		DirectX12Interface::CommandQueue = queue;

		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsKeyPressed(VK_INSERT))
		{
			Menu::ShowMenu = !Menu::ShowMenu;
			if (Menu::ShowMenu)
				io.WantCaptureMouse = true;
			else
				io.WantCaptureMouse = false;
			if (!Menu::ShowMenu)
				CFG::SaveCFG();
		}

		io.MouseDrawCursor = Menu::ShowMenu;

		/* OVERLAY FOR ESP AND OTHER DRAWING */
		Render::RenderOverlay();

		/* CHEAT MENU */
		Menu::RenderMenu();

		//if (ShowMenu)
		//	console.Draw("Console");


		ImGui::EndFrame();

#ifdef IS_DEBUG_VERSION
		std::cout << "[DX12] hkPreset before frame context\n";
#endif
		DirectX12Interface::_FrameContext& CurrentFrameContext = DirectX12Interface::FrameContext[pSwapChain->GetCurrentBackBufferIndex()];
		CurrentFrameContext.CommandAllocator->Reset();

		D3D12_RESOURCE_BARRIER Barrier;
		Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		Barrier.Transition.pResource = CurrentFrameContext.Resource;
		Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		DirectX12Interface::CommandList->Reset(CurrentFrameContext.CommandAllocator, nullptr);
		DirectX12Interface::CommandList->ResourceBarrier(1, &Barrier);
		DirectX12Interface::CommandList->OMSetRenderTargets(1, &CurrentFrameContext.DescriptorHandle, FALSE, nullptr);
		DirectX12Interface::CommandList->SetDescriptorHeaps(1, &DirectX12Interface::DescriptorHeapImGuiRender);

#ifdef IS_DEBUG_VERSION
		std::cout << "[DX12] hkPreset before render\n";
#endif
		ImGui::Render();
#ifdef IS_DEBUG_VERSION
		std::cout << "[DX12] hkPreset before ImGui_ImplDX12_RenderDrawData\n";
#endif
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectX12Interface::CommandList);
#ifdef IS_DEBUG_VERSION
		std::cout << "[DX12] hkPreset before ResourceBarrier\n";
#endif
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		DirectX12Interface::CommandList->ResourceBarrier(1, &Barrier);
#ifdef IS_DEBUG_VERSION
		std::cout << "[DX12] hkPreset before Close\n";
#endif
		DirectX12Interface::CommandList->Close();
#ifdef IS_DEBUG_VERSION
		std::cout << "[DX12] hkPreset before ExecuteCommandLists\n";
#endif
		DirectX12Interface::CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&DirectX12Interface::CommandList));
#ifdef IS_DEBUG_VERSION
		std::cout << "[DX12] hkPreset before return\n";
#endif
		return oPresent(pSwapChain, SyncInterval, Flags);
	}

	HRESULT APIENTRY hkCreateCommandQueue(
		ID3D12Device* pDevice,
		const D3D12_COMMAND_QUEUE_DESC* pDesc,
		REFIID riid,
		void** ppCommandQueue)
	{
		HRESULT hr = DX12::oCreateCommandQueue(pDevice, pDesc, riid, ppCommandQueue);

		if (SUCCEEDED(hr) && ppCommandQueue && *ppCommandQueue) {
			ID3D12CommandQueue* pQueue = static_cast<ID3D12CommandQueue*>(*ppCommandQueue);
			// Store this queue somewhere (e.g., global or map by thread/device/etc.)
			DirectX12Interface::CommandQueue = pQueue;
		}

		return hr;
	}

	HRESULT APIENTRY hkCreateSwapChain(
		IDXGIFactory* pFactory,
		IUnknown* pDeviceOrQueue,
		DXGI_SWAP_CHAIN_DESC* pDesc,
		IDXGISwapChain** ppSwapChain)
	{
		HRESULT hr = DX12::oCreateSwapChain(pFactory, pDeviceOrQueue, pDesc, ppSwapChain);

		if (SUCCEEDED(hr) && ppSwapChain && *ppSwapChain) {
#ifdef IS_DEBUG_VERSION
			std::cout << "[DX12] hkCreateSwapChain called.\n";
#endif
			IDXGISwapChain* swap = *ppSwapChain;
			g_SwapChainToQueue[*ppSwapChain] = static_cast<ID3D12CommandQueue*>(pDeviceOrQueue);
			// Map this swapchain to pDeviceOrQueue (cast to ID3D12CommandQueue*)
		}

		return hr;
	}

}