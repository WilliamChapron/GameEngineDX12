

//https://www.braynzarsoft.net/viewtutorial/q16390-04-directx-12-braynzar-soft-tutorials

// Inclusion des en-têtes nécessaires
#include <Windows.h>  // Inclut les en-têtes de l'API Windows
#include "stdafx.h"   // Inclut un en-tête local, peut contenir des dépendances supplémentaires

#include "WindowInitializer.h"
#include "DirectInitializer.h"
#include "EngineManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {


    DirectInitializer* direct3dInstance = new DirectInitializer();

    WindowInitializer* windowInstance = new WindowInitializer(&hInstance, nShowCmd, 800, 600, false);
    windowInstance->Initialize(hInstance, nShowCmd, 800, 600, false, direct3dInstance);

    EngineManager* engineInstance = new EngineManager(&hInstance);


    //    // initialize direct3d
    //if (!direct3dInstance)
    //{
    //    MessageBox(0, L"Failed to initialize direct3d 12",
    //        L"Error", MB_OK);
    //    direct3dInstance->Cleanup();
    //    return 1;
    //}


    //// we want to wait for the gpu to finish executing the command list before we start releasing everything
    //direct3dInstance->WaitForPreviousFrame();

    //// close the fence event
    //direct3dInstance->CloseHandle(fenceEvent);



    return 0;
}