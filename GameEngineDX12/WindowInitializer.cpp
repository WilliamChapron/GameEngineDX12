#include "WindowInitializer.h"

namespace WindowInitializer {
    HWND _hwnd = nullptr;
    LPCTSTR _windowName = L"";
    LPCTSTR _windowTitle = L"";
    int _width = 0;
    int _height = 0;
    bool _fullScreen = false;

    bool Initialize(HINSTANCE* hInstance, int showWnd, int width, int height, bool fullscreen) {

        _hwnd = nullptr;
        _windowName = L"GameEngineDX12App";
        _windowTitle = L"GameEngineDX12";
        _width = width;
        _height = height;
        _fullScreen = fullscreen;


        if (fullscreen) {
            HMONITOR hmon = MonitorFromWindow(_hwnd, MONITOR_DEFAULTTONEAREST);
            MONITORINFO mi = { sizeof(mi) };
            GetMonitorInfo(hmon, &mi);
            WindowInitializer::_width = mi.rcMonitor.right - mi.rcMonitor.left;
            WindowInitializer::_height = mi.rcMonitor.bottom - mi.rcMonitor.top;
        }

        

        WNDCLASSEX wc;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = NULL;
        wc.cbWndExtra = NULL;
        wc.hInstance = (*hInstance);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = _windowName;
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(&wc)) {
            MessageBox(NULL, L"Erreur d'enregistrement de la classe", L"Erreur", MB_OK | MB_ICONERROR);
            return false;
        }

        _hwnd = CreateWindowEx(NULL, _windowName, _windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, (*hInstance), NULL);

        if (!_hwnd) {
            MessageBox(NULL, L"Erreur de création de la fenêtre", L"Erreur", MB_OK | MB_ICONERROR);
            return false;
        }

        if (fullscreen) {
            SetWindowLong(_hwnd, GWL_STYLE, 0);
        }

        ShowWindow(_hwnd, showWnd);
        UpdateWindow(_hwnd);

        return true;
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
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
}
