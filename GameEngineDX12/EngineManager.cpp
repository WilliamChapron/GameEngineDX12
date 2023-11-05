#include "EngineManager.h"

EngineManager::EngineManager(HINSTANCE *hInstance) : window(*hInstance) 
{
    MainLoop();
}

EngineManager::~EngineManager() {
}



void EngineManager::MainLoop() {
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
            // Code du jeu ici
        }
    }
    // Ajoutez la logique de jeu ici
}
