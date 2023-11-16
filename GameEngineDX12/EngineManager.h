// EngineManager.h

#pragma once

#include <Windows.h>
#include <iostream>

namespace EngineManager {
    extern HINSTANCE* _hInstance;
    extern int _nShowCmd;
    extern FILE* _pConsole;
    extern int frameIndex;

    void Initialize(HINSTANCE* hInstance, int nShowCmd);
    void Cleanup();
    void MainLoop();


}
