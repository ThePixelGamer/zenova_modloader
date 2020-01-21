#include "ZenovaCommon.h"

//UI
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <imgui_internal.h>

#include <process.h>
#include <stdio.h> 
#include <conio.h> 
#include <thread>
#include "../../Utils/utils.h"

#define WIDTH 1024
#define HEIGHT 720

// Data
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
void ShowLog(bool*);

int EnterGUILoop();

AppUtils::AppDebugger* app;

// Main code
int main(int argumentsSize, char** arguments) {
	CoInitialize(NULL); //to get "app" working

	if(!app) {
		std::wstring minecraft_pid = AppUtils::GetMinecraftPackageId();
		ZenovaCommon::logger.AddLog("%s\n", Util::WSTRING_TO_STRING(minecraft_pid).c_str());
		app = new AppUtils::AppDebugger(minecraft_pid);
		app->setStateChangeCallback(StateChangeCallbackFunc);
		StateChangeCallbackFunc(app->GetPackageExecutionState());
	}

	int ret = 0;

	if(strcmp(arguments[0], (Util::WSTRING_TO_STRING(Util::GetCurrentDirectory()) + "FastZenovaLaunch.exe").c_str()) == 0) {
		LaunchMinecraft(true);
	}
	else {
		ret = EnterGUILoop();
	}

	CoUninitialize();

	return ret;
}

int EnterGUILoop() {
	// Setup SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
		std::cout << "Error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	// Setup window
	SDL_Window* window = SDL_CreateWindow("Zenova", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, (SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	HWND hwnd = (HWND)wmInfo.info.win.window;

	// Initialize Direct3D
	if(!CreateDeviceD3D(hwnd)) {
		CleanupDeviceD3D();
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForD3D(window);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	// Our state
	bool show_app_log = true;
	bool launchMinecraft = false;
	bool show_mods_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	while(!ZenovaCommon::done) {
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);
			if(event.type == SDL_QUIT)
				ZenovaCommon::done = true;
			if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				ZenovaCommon::done = true;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if(show_app_log)     ShowLog(&show_app_log);

		{
			ImGui::Begin("App Launching Stuff");
			if(ImGui::Button("Launch Minecraft"))
				launchMinecraft = true;

			ImGui::Text("State: %s", ZenovaCommon::currentState.c_str());

			ImGui::End();
		}

		if(launchMinecraft) {
			launchMinecraft = false;
			LaunchMinecraft(true);
		}

		// Rendering
		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0); // Present with vsync
		//g_pSwapChain->Present(0, 0); // Present without vsync
	}

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

#define BUF_SIZE 256
TCHAR szName[]=TEXT("Global\\PIDForDebugger");

DWORD dwProcessId = 0;

// Launcher functions
void LaunchMinecraft(bool forceRestart) {
	HRESULT hresult = S_OK;

	if(!app) return;

	if(SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
		if(app->GetPackageExecutionState() != PES_UNKNOWN) {
			if(forceRestart) {
				app->TerminateAllProcesses();
				hresult = app->GetHRESULT();
				if(hresult != S_OK) {
					ZenovaCommon::logger.AddLog("%s%d\n", "Failed to restart Minecraft, HRESULT: ", hresult);
					return;
				}
			}
			else
				return;
		}

		std::wstring ModLoaderPath = Util::GetCurrentDirectory();
		if(ModLoaderPath.length() == 0) {
			ZenovaCommon::logger.AddLog("%s\n", "Failed to get the current directory");
			CoUninitialize();
			return;//  E_FAIL;
		}

		ModLoaderPath += L"ZenovaModLoader.exe";
		if(!PathFileExists(ModLoaderPath.c_str())) {
			ZenovaCommon::logger.AddLog("%s\n", "Couldn't find ZenovaModLoader.exe");
			CoUninitialize();
			return;//  E_FAIL;
		}

		app->EnableDebugging(ModLoaderPath);
		hresult = app->GetHRESULT();
		if(hresult != S_OK) {
			ZenovaCommon::logger.AddLog("%s%x\n", "Could not enable debugging, HRESULT: ", hresult);
			CoUninitialize();
			return;//  hresult;
		}

		typedef void (*ZD_Void)();
		typedef void (*ZD_WString)(const std::wstring&);
		typedef void (*ZD_DwordRef)(std::reference_wrapper<DWORD>);

		//std::thread debugger((ZD_Void)(ZenovaDebugger));
		//debugger.detach();

		std::wstring ApplicationId = AppUtils::GetMinecraftApplicationId();
		if(ApplicationId.length() == 0) return;// E_FAIL;
		hresult = AppUtils::LaunchApplication(ApplicationId.c_str(), &dwProcessId);
		if(hresult != S_OK) {
			ZenovaCommon::logger.AddLog("%s%x\n", "Failed to launch Minecraft, HRESULT: ", hresult);
			CoUninitialize();
			return;
		}

		app->DisableDebugging(); //could be commented out for a temp perma injection (till the user reboots)
		hresult = app->GetHRESULT();
		if(hresult != S_OK) {
			ZenovaCommon::logger.AddLog("%s%x\n", "Could not disable debugging, HRESULT: ", hresult);
			CoUninitialize();
			return;//  hresult;
		}
		
		ZenovaCommon::logger.AddLog("%s\n", "Sucessfully launched Minecraft with mods");

		//system("PAUSE");
		CoUninitialize();
	}

	return;//  S_OK;
}

void ShowLog(bool* p_open) {
	// We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
	// Most of the contents of the window will be added by the log.Draw() call.
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("Zenova Logger", p_open);
	ImGui::End();

	// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
	ZenovaCommon::logger.Draw("Zenova Logger", p_open);
}

bool CreateDeviceD3D(HWND hWnd) {
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0,};
	if(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D() {
	CleanupRenderTarget();
	if(g_pSwapChain) {
		g_pSwapChain->Release(); g_pSwapChain = NULL;
	}
	if(g_pd3dDeviceContext) {
		g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL;
	}
	if(g_pd3dDevice) {
		g_pd3dDevice->Release(); g_pd3dDevice = NULL;
	}
}

void CreateRenderTarget() {
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget() {
	if(g_mainRenderTargetView) {
		g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL;
	}
}