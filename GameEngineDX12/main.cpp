

//https://www.braynzarsoft.net/viewtutorial/q16390-04-directx-12-braynzar-soft-tutorials

 /*
    ***********************
    CODE STYLE DESCRIPTION
    ***********************
*/

// - Pascal Case For Class 
// - Pascal Case For Functions
// - Camel Case For Var 
// - Add _type after normal var (facultatif for not important type but don't need to put "_" if no type write) 
// - Add type_ before class member var (facultatif for not important type but need always put the "_") 
// - Don't need specifiy _ or type with args var
// - Multiple Line commentary for Functions or Methods decription, like : ( /* - Commentary - */ )
// - One line commentary for one line Operation, next to the line, like : ( // $Commentary )
// - To Think commentary : #TOTHINK - $
// - To Do commentary : #TODO - $

/*
    CODE STYLE DESCRIPTION
*/



#include <Windows.h>  
#include "stdafx.h"   
#include "WindowInitializer.h"
#include "DirectInitializer.h"
#include "EngineManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {


    DirectInitializer* direct3dInstance = new DirectInitializer();

    WindowInitializer* windowInstance = new WindowInitializer(&hInstance, nShowCmd, 800, 600, false);
    windowInstance->Initialize(hInstance, nShowCmd, 800, 600, false, direct3dInstance);

    EngineManager* engineInstance = new EngineManager(&hInstance);


    // initialize direct3d
    if (!direct3dInstance)
    {
        MessageBox(0, L"Failed to initialize direct3d 12",L"Error", MB_OK);
        direct3dInstance->Cleanup();
        return 1;
    }


    //// we want to wait for the gpu to finish executing the command list before we start releasing everything
    //direct3dInstance->WaitForPreviousFrame();

    //// close the fence event
    //direct3dInstance->CloseHandle(fenceEvent);



    return 0;
}