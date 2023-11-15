#pragma once
#include <Windows.h>

class DirectInitializer;

//class Manager 
//{
//private:
//    static Manager* pInstance;
//    Manager() {}
//
//public:
//    static void Init() 
//    {
//        if (pInstance != nullptr)
//            return;
//
//        pInstance = new Manager();
//    }
//
//    static Manager* Get() 
//    {
//        return pInstance;
//    }
//
//private:
//    int i;
//    int j;
//
//public:
//    int function();
//
//};
//
//
//Manager::Init();
//Manager::Get()->function();

class WindowInitializer {
public:
    WindowInitializer(HINSTANCE *hInstance, int showWnd, int width, int height, bool fullscreen);
    ~WindowInitializer();
    bool Initialize(HINSTANCE *hInstance, int showWnd, int width, int height, bool fullscreen);
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND hwnd;
    LPCTSTR windowName;
    LPCTSTR windowTitle;
    int width;
    int height;
    bool fullScreen;
};