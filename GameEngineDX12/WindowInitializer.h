#pragma once
#include <Windows.h>

class WindowInitializer {
public:
    WindowInitializer(HINSTANCE *hInstance, int showWnd, int width, int height, bool fullscreen);
    ~WindowInitializer();
    bool Initialize(HINSTANCE &hInstance, int showWnd, int width, int height, bool fullscreen);
    void MainLoop();
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND hwnd;
    LPCTSTR windowName;
    LPCTSTR windowTitle;
    int width;
    int height;
    bool fullScreen;
};