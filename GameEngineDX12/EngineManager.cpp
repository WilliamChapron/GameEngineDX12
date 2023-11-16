#include "EngineManager.h"
#include "DirectInitializer.h"
#include "WindowInitializer.h"

namespace EngineManager {
    HINSTANCE* _hInstance = nullptr;
    int _nShowCmd = 0;
    FILE* _pConsole = nullptr;
    int _frameIndex = 0;


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
                //return;
                /*HRESULT hr = DirectInitializer::_commandAllocators[_frameIndex]->Reset();
                if (FAILED(hr))
                {
                    
                }*/
                // Main loop logic
                //    // initialize direct3d
                //if (!InitD3D())
                //{
                //    MessageBox(0, L"Failed to initialize direct3d 12",
                //        L"Error", MB_OK);
                //    Cleanup();
                //    return 1;
                //}

                //    // we want to wait for the gpu to finish executing the command list before we start releasing everything
                //    WaitForPreviousFrame();

                //// close the fence event
                //CloseHandle(fenceEvent);
                /*DirectInitializer::_commandList->Close();*/
            }
        }
    }
}



