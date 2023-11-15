// EngineManager.h

#pragma once

class DirectInitializer;
class WindowInitializer;
#include <Windows.h>
#include <iostream>

class EngineManager {
public:
    EngineManager(HINSTANCE* hInstance, int nShowCmd);
    ~EngineManager();

    void Initialize();
    void Cleanup();
    void MainLoop();

private:
    HINSTANCE* hInstance;
    int nShowCmd;
    DirectInitializer* direct3dInstance;
    WindowInitializer* windowInstance;
    FILE* pConsole;
};

