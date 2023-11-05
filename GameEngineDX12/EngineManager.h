#pragma once

#include <Windows.h>

class EngineManager {
public:
    EngineManager(HINSTANCE *hInstance);
    ~EngineManager();


    // Boucle principale du jeu
    void MainLoop();

private:
    HINSTANCE window;
};

