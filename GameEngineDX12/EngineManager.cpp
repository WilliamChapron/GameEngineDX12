#include "EngineManager.h"
#include "DirectInitializer.h"
#include "WindowInitializer.h"

namespace EngineManager {
    HINSTANCE* _hInstance = nullptr;
    int _nShowCmd = 0;
    FILE* _pConsole = nullptr;


    void Initialize(HINSTANCE* hInstance, int nShowCmd) {

        _hInstance = hInstance;

        // Create console
        AllocConsole();

        if (freopen_s(&_pConsole, "CONOUT$", "w", stdout) != 0) {
            MessageBox(0, L"Failed to redirect console output", L"Error", MB_OK);
            Cleanup();
            PostQuitMessage(1);
        }

        WindowInitializer::Initialize(hInstance, nShowCmd, 1200, 900, false);
        DirectInitializer::Initialize();

        MainLoop();
    }

    void Cleanup() {
        if (_pConsole) {
            fclose(_pConsole);
            FreeConsole();
        }
    }

    void MainLoop() {
        std::cout << "Main Loop Started" << std::endl;

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
                // Main loop logic
            }
        }
    }
}



