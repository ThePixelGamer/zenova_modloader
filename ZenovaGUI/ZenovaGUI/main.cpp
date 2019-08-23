// dear imgui: standalone example application for SDL2 + DirectX 11
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

//Appx
#include <iostream>
#include "AppUtils.h"
#include "ProcessUtils.h"
#include "utils.h"

//UI
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_syswm.h>

AppUtils::AppDebugger* app;

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;

// Forward declarations of launcher functions
int Handler(int);
HRESULT launchAppxPackage(LPCWSTR, PDWORD);
void LaunchMinecraft(bool);
std::wstring GetMinecraftApplicationId();

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

PACKAGE_EXECUTION_STATE currentState = PACKAGE_EXECUTION_STATE::PES_UNKNOWN;

// Main code
int wmain(int argumentsSize, wchar_t** arguments) {
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    // Setup window
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+DirectX11 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hwnd = (HWND)wmInfo.info.win.window;

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd)) {
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

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//Initalize Launcher Stuff
	if(!app) {
		app = new AppUtils::AppDebugger(AppUtils::GetMinecraftPackageId());
	}
	//SetStateChangeCallback(Handler);

    // Main loop
    bool done = false;
    while (!done) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            if(ImGui::Button("Launch Minecraft"))
				show_another_window = true;

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window) {
			show_another_window = false;

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

// Launcher functions
int Handler(int state) {
	currentState = (PACKAGE_EXECUTION_STATE)state;
	//update launcher button
	return 0;
}

HRESULT launchAppxPackage(LPCWSTR packageFamilyName, PDWORD applicationId) {
	HRESULT result = CoInitialize(nullptr);
	if (SUCCEEDED(result)) {
		{
			CComPtr<IApplicationActivationManager> applicationActivationManager;
			result = CoCreateInstance(CLSID_ApplicationActivationManager, NULL, CLSCTX_LOCAL_SERVER, IID_IApplicationActivationManager, (LPVOID*)&applicationActivationManager);
			if (SUCCEEDED(result)) {
				result = CoAllowSetForegroundWindow(applicationActivationManager, NULL);
				if (SUCCEEDED(result)) {
					result = applicationActivationManager->ActivateApplication(packageFamilyName, NULL, AO_NONE, applicationId);
					if (SUCCEEDED(result)) {
						std::wcout << applicationId << std::endl;
						return S_OK;
					}
					else {
						std::wcerr << L"Error in ActivateApplication 0x" << std::hex << result << std::endl;
					}
				}
				else {
					std::wcerr << L"Error in CoAllowSetForegroundWindow 0x" << std::hex << result << std::endl;
				}
			}
			else {
				std::wcerr << L"Error in CoCreateInstance 0x" << std::hex << result << std::endl;
			}
		}

		CoUninitialize();
	}
	else {
		std::wcerr << L"Error in CoInitialize 0x" << std::hex << result << std::endl;
	}

	return E_UNEXPECTED;
}

void LaunchMinecraft(bool forceRestart) {
	HRESULT hresult = S_OK;

	if(!app) return;

	if(SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
		if(app->GetPackageExecutionState() != PES_UNKNOWN) {
			if(forceRestart) {
				app->TerminateAllProcesses();
				hresult = app->GetHRESULT();
				if(hresult != S_OK) {
					//std::cout << "Failed to restart Minecraft, HRESULT: " << hresult << std::endl;
					//system("PAUSE");
					return;
				}
			}
			else
				return;
		}

		std::wstring ModLoaderPath = Util::GetCurrentDirectory();
		if(ModLoaderPath.length() == 0) {
			//std::cout << "Failed to get the current directory" << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;//  E_FAIL;
		}

		ModLoaderPath += L"ZenovaModLoader.exe";
		if(!PathFileExists(ModLoaderPath.c_str())) {
			//std::cout << "Couldn't find ZenovaModLoader.exe" << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;//  E_FAIL;
		}

		app->EnableDebugging(ModLoaderPath);
		hresult = app->GetHRESULT();
		if(hresult != S_OK) {
			//std::cout << "Could not enable debugging, HRESULT: " << hresult << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;//  hresult;
		}

		DWORD dwProcessId = 0;
		std::wstring ApplicationId = AppUtils::GetMinecraftApplicationId();
		if(ApplicationId.length() == 0) return;// E_FAIL;
		hresult = AppUtils::LaunchApplication(ApplicationId.c_str(), &dwProcessId);
		if(hresult != S_OK) {
			//std::cout << "Failed to launch Minecraft, HRESULT: " << hresult << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;
		}
		else {
			//std::cout << "Sucessfully launched Minecraft with mods\n";
		}

		app->DisableDebugging();
		hresult = app->GetHRESULT();
		if(hresult != S_OK) {
			//std::cout << "Could not disable debugging, HRESULT: " << hresult << std::endl;
			//system("PAUSE");
			CoUninitialize();
			return;//  hresult;
		}

		//system("PAUSE");
		CoUninitialize();
	}

	return;//  S_OK;
}

std::wstring GetMinecraftApplicationId() {
	std::wstring appId;

	ATL::CRegKey key, subKey;
	LPTSTR szBuffer = new WCHAR[1024], szSubBuffer = new WCHAR[1024];
	DWORD dwBufferLen = 1024, dwSubBufferLen = 1024;
	DWORD index = 0;

	LRESULT result = ERROR_SUCCESS;

	result = key.Open(HKEY_CLASSES_ROOT, L"", KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE | KEY_READ);
	if(result != ERROR_SUCCESS) return appId;

	for(; key.EnumKey(index, szBuffer, &dwBufferLen, NULL) == ERROR_SUCCESS; dwBufferLen = 1024, index++) {
		std::wstring keyName(szBuffer);
		if(keyName.substr(0, 4) != L"AppX") continue;

		result = subKey.Open(HKEY_CLASSES_ROOT, keyName.c_str(), KEY_QUERY_VALUE | KEY_READ);
		if(result != ERROR_SUCCESS) continue;

		dwSubBufferLen = 1024;
		result = subKey.QueryStringValue(L"", szSubBuffer, &dwSubBufferLen);
		if(result != ERROR_SUCCESS) continue;

		std::wstring keyValue(szSubBuffer);
		if(keyValue.find(MINECRAFT_DEFAULT_NAME) == keyValue.npos) continue;

		dwSubBufferLen = 1024;
		result = subKey.Open(HKEY_CLASSES_ROOT, (keyName + L"\\Application").c_str(), KEY_QUERY_VALUE | KEY_READ);
		if(result != ERROR_SUCCESS) continue;

		result = subKey.QueryStringValue(L"AppUserModelID", szSubBuffer, &dwSubBufferLen);
		if(result != ERROR_SUCCESS) continue;

		appId = szSubBuffer;

		break;
	}

	subKey.Close();
	key.Close();

	return appId;
}

// Helper functions

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
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}