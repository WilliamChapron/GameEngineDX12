#include "WindowInitializer.h"
#include <Windows.h>

WindowInitializer::WindowInitializer(HINSTANCE *hInstance, int showWnd, int width, int height, bool fullscreen) : hwnd(nullptr), windowName(L"GameEngineDX12App"), windowTitle(L"GameEngineDX12"), width(width), height(height), fullScreen(fullscreen) {
    Initialize(*hInstance, showWnd, width, height, fullscreen);
}
WindowInitializer::~WindowInitializer() {
}

bool WindowInitializer::Initialize(HINSTANCE &hInstance, int showWnd, int width, int height, bool fullscreen) {
    if (fullscreen) {
        HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { sizeof(mi) };
        GetMonitorInfo(hmon, &mi);
        width = mi.rcMonitor.right - mi.rcMonitor.left;
        height = mi.rcMonitor.bottom - mi.rcMonitor.top;
    }

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = windowName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Erreur d'enregistrement de la classe", L"Erreur", MB_OK | MB_ICONERROR);
        return false;
    }

    hwnd = CreateWindowEx(NULL, windowName, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        MessageBox(NULL, L"Erreur de création de la fenêtre", L"Erreur", MB_OK | MB_ICONERROR);
        return false;
    }

    if (fullscreen) {
        SetWindowLong(hwnd, GWL_STYLE, 0);
    }

    ShowWindow(hwnd, showWnd);
    UpdateWindow(hwnd);

    return true;
}

void WindowInitializer::MainLoop() {
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
}

LRESULT CALLBACK WindowInitializer::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            if (MessageBox(0, L"Êtes-vous sûr de vouloir quitter ?", L"Vraiment ?", MB_YESNO | MB_ICONQUESTION) == IDYES)
                DestroyWindow(hwnd);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
