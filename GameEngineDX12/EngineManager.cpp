// EngineManager.cpp

#include "EngineManager.h"

#include "DirectInitializer.h"
#include "WindowInitializer.h"


    

EngineManager::EngineManager(HINSTANCE* hInstance, int nShowCmd) : hInstance(hInstance), nShowCmd(nShowCmd) {
    // Create console
    AllocConsole();

    if (freopen_s(&pConsole, "CONOUT$", "w", stdout) != 0) {
        MessageBox(0, L"Failed to redirect console output", L"Error", MB_OK);
        Cleanup();
        PostQuitMessage(1);
    }

    Initialize();
    MainLoop();
}

EngineManager::~EngineManager() {
    Cleanup();
}

void EngineManager::Initialize() {
   
    windowInstance = new WindowInitializer(hInstance, nShowCmd, 800, 600, false);
    if (!windowInstance) {
        std::cout << "Windows Instance not initialized correctly" << std::endl;

    }
    windowInstance->Initialize(hInstance, nShowCmd, 1200, 900, false);

    direct3dInstance = new DirectInitializer();
    if (!direct3dInstance) {
        std::cout << "DirectX 3d Instance not initialized correctly" << std::endl;
        /*MessageBox(0, L"Failed to initialize direct3d 12", L"Error", MB_OK);*/
        direct3dInstance->Cleanup();
        PostQuitMessage(1);
    }
    direct3dInstance->Initialize();



}

void EngineManager::Cleanup() {
    if (pConsole) {
        fclose(pConsole);
        FreeConsole();
    }

    delete direct3dInstance;
    delete windowInstance;
}

void EngineManager::MainLoop() {
    std::cout << "Main Loop Started" << std::endl;
    // Logique de la boucle principale du jeu
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            //// we want to wait for the gpu to finish executing the command list before we start releasing everything
            //direct3dInstance->WaitForPreviousFrame();

            //// close the fence event
            //direct3dInstance->CloseHandle(fenceEvent);
        }
    }
}
