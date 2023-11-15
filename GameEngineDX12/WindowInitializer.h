#pragma once
#include <Windows.h>

namespace WindowInitializer {
    extern HWND _hwnd;
    extern LPCTSTR _windowName;
    extern LPCTSTR _windowTitle;
    extern int _width;
    extern int _height;
    extern bool _fullScreen;

    bool Initialize(HINSTANCE* hInstance, int showWnd, int width, int height, bool fullscreen);
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
