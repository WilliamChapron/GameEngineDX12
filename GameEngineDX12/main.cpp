



// Inclusion des en-t�tes n�cessaires
#include <Windows.h>  // Inclut les en-t�tes de l'API Windows
#include "stdafx.h"   // Inclut un en-t�te local, peut contenir des d�pendances suppl�mentaires

#include "WindowInitializer.h"
#include "EngineManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WindowInitializer window(&hInstance, nShowCmd, 800, 600, false);
    EngineManager engine(&hInstance);


    return 0;
}