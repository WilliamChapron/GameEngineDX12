



// Inclusion des en-têtes nécessaires
#include <Windows.h>  // Inclut les en-têtes de l'API Windows
#include "stdafx.h"   // Inclut un en-tête local, peut contenir des dépendances supplémentaires

#include "WindowInitializer.h"
#include "EngineManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WindowInitializer window(&hInstance, nShowCmd, 800, 600, false);
    EngineManager engine(&hInstance);


    return 0;
}