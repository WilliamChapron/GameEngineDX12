#include <Windows.h>
#include <iostream>
#include "stdafx.h"   

// Variables de la fenêtre
HWND _hwnd = nullptr;
LPCTSTR _windowName = L"GameEngineDX12App";
LPCTSTR _windowTitle = L"GameEngineDX12";
int _width = 800;
int _height = 600;
bool _fullScreen = false;

// Variables de DirectInitializer
IDXGIAdapter1* _dxgiAdapter = nullptr;
IDXGIFactory4* _dxgiFactory = nullptr;
ID3D12Device* _device = nullptr;
IDXGISwapChain3* _swapChain = nullptr;
ID3D12CommandQueue* _commandQueue = nullptr;
ID3D12DescriptorHeap* _rtvDescriptorHeap = nullptr;
ID3D12Resource* _firstRenderTarget;
ID3D12Resource* _secondRenderTarget;
ID3D12Resource* _thirdRenderTarget;
ID3D12CommandAllocator* _firstCommandAllocator;
ID3D12CommandAllocator* _secondCommandAllocator;
ID3D12CommandAllocator* _thirdCommandAllocator;
ID3D12GraphicsCommandList* _commandList = nullptr;
ID3D12Fence* _firstFence;
ID3D12Fence* _secondFence;
ID3D12Fence* _thirdFence;
int _frameIndex = 0;
int _rtvDescriptorSize = 0;
HANDLE _fenceEvent = nullptr;
UINT64 _firstFenceValue = 0;
UINT64 _secondFenceValue = 0;
UINT64 _thirdFenceValue = 0;

// Variables de EngineManager
HINSTANCE _hInstance = nullptr;  // Modifié pour enlever le pointeur
int _nShowCmd = 0;
FILE* _pConsole = nullptr;


bool CreateDXGIFactory(IDXGIFactory4*& dxgiFactory);
bool CreateDXGIAdapterAndDevice(IDXGIFactory4* dxgiFactory, IDXGIAdapter1*& dxgiAdapter, ID3D12Device*& device);
bool CreateCommandQueue(ID3D12Device* device, ID3D12CommandQueue*& commandQueue);
bool CreateSwapChain(IDXGIFactory4* dxgiFactory, ID3D12CommandQueue* commandQueue, IDXGISwapChain3*& swapChain, HWND hwnd, int width, int height, bool fullScreen);
bool CreateDescriptorHeapAndRenderTargets(ID3D12Device* device, IDXGISwapChain3* swapChain, ID3D12DescriptorHeap*& rtvDescriptorHeap, ID3D12Resource*& firstRenderTarget, ID3D12Resource*& secondRenderTarget, ID3D12Resource*& thirdRenderTarget);
bool CreateCommandAllocators(ID3D12Device* device, ID3D12CommandAllocator*& firstCommandAllocator, ID3D12CommandAllocator*& secondCommandAllocator, ID3D12CommandAllocator*& thirdCommandAllocator);
bool CreateCommandList(ID3D12Device* device, ID3D12CommandAllocator* firstCommandAllocator, ID3D12GraphicsCommandList*& commandList);
bool CreateFencesAndFenceEvent(ID3D12Device* device, ID3D12Fence*& firstFence, ID3D12Fence*& secondFence, ID3D12Fence*& thirdFence, HANDLE& fenceEvent);

// Déclarations de fonctions
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void InitializeWindow(HINSTANCE hInstance, int nShowCmd);
void Cleanup();
void MainLoop();

// Déclarations de fonctions de DirectManager
bool InitializeDirectX();
void Update();
void UpdatePipeline();
void Render();
void WaitForPreviousFrame();




bool CreateDXGIFactory(IDXGIFactory4*& dxgiFactory) {
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

    if (FAILED(hr) || dxgiFactory == nullptr) {
        std::cout << "Failed to create DXGI Factory" << std::endl;
        return false;
    }

    if (SUCCEEDED(hr) && dxgiFactory != nullptr) {
        std::cout << "Success to create DXGI Factory" << std::endl;
    }

    return true;
}


bool CreateDXGIAdapterAndDevice(IDXGIFactory4* dxgiFactory, IDXGIAdapter1*& dxgiAdapter, ID3D12Device*& device) {
    int adapterIndex = 0;
    bool adapterFound = false;

    while (dxgiFactory->EnumAdapters1(adapterIndex, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC1 desc;
        dxgiAdapter->GetDesc1(&desc);

        // If Software adapter, move to the next one
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            adapterIndex++;
            continue;
        }

        // Test if the device is compatible with the required version
        HRESULT hr = D3D12CreateDevice(
            dxgiAdapter,
            D3D_FEATURE_LEVEL_11_0,
            _uuidof(ID3D12Device),
            reinterpret_cast<void**>(&device)
        );

        // Device Creation State
        if (SUCCEEDED(hr) && device != nullptr) {
            std::cout << "Success to create D3D12 device." << std::endl;
            adapterFound = true;
            break;
        }

        if (FAILED(hr) || device == nullptr) {
            std::cout << "Failed to create D3D12 device / continue try find other adapter" << std::endl;
        }

        adapterIndex++;
    }

    // Adapter Creation State
    if (dxgiAdapter != nullptr) {
        std::cout << "Success to find and store DXGIAdapter." << std::endl;
    }
    else {
        std::cout << "Failed to find DXGI adapter." << std::endl;
        return false;
    }

    return true;
}

bool CreateCommandQueue(ID3D12Device* device, ID3D12CommandQueue*& commandQueue) {
    // Command Queue Description
    D3D12_COMMAND_QUEUE_DESC cqDesc = {}; // default values

    HRESULT hr = device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&commandQueue));

    // Command Queue Creation State
    if (FAILED(hr) || commandQueue == nullptr) {
        std::cout << "Failed to create Device Command Queue." << std::endl;
        return false;
    }

    if (SUCCEEDED(hr) && commandQueue != nullptr) {
        std::cout << "Success to create Device Command Queue." << std::endl;
        return true;
    }

    return true;
}


bool CreateSwapChain(IDXGIFactory4* dxgiFactory, ID3D12CommandQueue* commandQueue, IDXGISwapChain3*& swapChain, HWND hwnd, int width, int height, bool fullScreen) {
    DXGI_MODE_DESC backBufferDesc = {};
    backBufferDesc.Width = width;
    backBufferDesc.Height = height;
    backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = 1;

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 3;
    swapChainDesc.BufferDesc = backBufferDesc;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc = sampleDesc;
    swapChainDesc.Windowed = !fullScreen;

    IDXGISwapChain* tempSwapChain;
    dxgiFactory->CreateSwapChain(
        commandQueue,
        &swapChainDesc,
        &tempSwapChain
    );

    swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);

    // Swap Chain Creation State
    if (swapChain == nullptr) {
        std::cout << "Failed to create Device Swap Chain." << std::endl;
        return false;
    }

    if (swapChain != nullptr) {
        _frameIndex = swapChain->GetCurrentBackBufferIndex();
        std::cout << "Success to create Device Swap Chain." << std::endl;
        return true;
    }

    return true;
}


bool CreateDescriptorHeapAndRenderTargets(ID3D12Device* device, IDXGISwapChain3* swapChain, ID3D12DescriptorHeap*& rtvDescriptorHeap, ID3D12Resource*& firstRenderTarget, ID3D12Resource*& secondRenderTarget, ID3D12Resource*& thirdRenderTarget) {
    // Creating 3 descriptors
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = 3;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT hr = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));

    // Descriptor Heap Creation State
    if (rtvDescriptorHeap == nullptr || FAILED(hr)) {
        std::cout << "Failed to create Descriptor Heap." << std::endl;
        return false;
    }

    if (rtvDescriptorHeap != nullptr && SUCCEEDED(hr)) {
        std::cout << "Success to create Descriptor Heap." << std::endl;
    }

    UINT rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    // Create Render target Views

    // First Render Target
    hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&firstRenderTarget));

    if (firstRenderTarget == nullptr || FAILED(hr)) {
        std::cout << "Failed to create the First Render Target" << std::endl;
        return false;
    }

    if (firstRenderTarget != nullptr && SUCCEEDED(hr)) {
        std::cout << "Success to create the First Render Target" << std::endl;
    }

    device->CreateRenderTargetView(firstRenderTarget, nullptr, rtvHandle);
    rtvHandle.Offset(1, rtvDescriptorSize);

    // Second Render Target
    hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&secondRenderTarget));

    if (secondRenderTarget == nullptr || FAILED(hr)) {
        std::cout << "Failed to create the Second Render Target" << std::endl;
        return false;
    }

    if (secondRenderTarget != nullptr && SUCCEEDED(hr)) {
        std::cout << "Success to create the Second Render Target" << std::endl;
    }

    device->CreateRenderTargetView(secondRenderTarget, nullptr, rtvHandle);
    rtvHandle.Offset(1, rtvDescriptorSize);

    // Third Render Target
    hr = swapChain->GetBuffer(2, IID_PPV_ARGS(&thirdRenderTarget));

    if (thirdRenderTarget == nullptr || FAILED(hr)) {
        std::cout << "Failed to create the Third Render Target" << std::endl;
        return false;
    }

    if (thirdRenderTarget != nullptr && SUCCEEDED(hr)) {
        std::cout << "Success to create the Third Render Target" << std::endl;
    }

    device->CreateRenderTargetView(thirdRenderTarget, nullptr, rtvHandle);
    rtvHandle.Offset(1, rtvDescriptorSize);

    return true;
}

bool CreateCommandAllocators(ID3D12Device* device, ID3D12CommandAllocator*& firstCommandAllocator, ID3D12CommandAllocator*& secondCommandAllocator, ID3D12CommandAllocator*& thirdCommandAllocator) {

    // First Command Allocator
    HRESULT hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&firstCommandAllocator));

    if (FAILED(hr) || firstCommandAllocator == nullptr) {
        std::cout << "Failed to create the first Command Allocator." << std::endl;
        return false;
    }

    std::cout << "Success to create the first Command Allocator." << std::endl;

    // Second Command Allocator
    HRESULT hrr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&secondCommandAllocator));

    if (FAILED(hrr) || secondCommandAllocator == nullptr) {
        std::cout << "Failed to create the second Command Allocator." << std::endl;
        return false;
    }

    std::cout << "Success to create the second Command Allocator." << std::endl;

    // Third Command Allocator
    HRESULT hrrr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&thirdCommandAllocator));

    if (FAILED(hrrr) || thirdCommandAllocator == nullptr) {
        std::cout << "Failed to create the third Command Allocator." << std::endl;
        return false;
    }

    std::cout << "Success to create the third Command Allocator." << std::endl;

    return true;
}


bool CreateCommandList(ID3D12Device* device, ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList*& commandList) {
    // #TODO test if COmmand Allocator Reset Doesn't work
    HRESULT hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, NULL, IID_PPV_ARGS(&commandList));
    if (FAILED(hr) || commandList == nullptr) {
        std::cout << "Failed to create the Command List" << std::endl;
        return false;
    }

    std::cout << "Success to create the Command List" << std::endl;

    HRESULT dhr = commandAllocator->Reset();
    if (FAILED(dhr)) {
        if (FACILITY_WINDOWS == HRESULT_FACILITY(dhr))
            dhr = HRESULT_CODE(dhr);

        TCHAR* szErrMsg;
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dhr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szErrMsg, 0, NULL) != 0) {
            std::wcout << L"Failed to reset Command Allocator. Error code: " << dhr << L" - " << (LPWSTR)szErrMsg << std::endl;
            LocalFree(szErrMsg);
        }
    }

    if (SUCCEEDED(dhr)) {
        std::cout << "Success To Reset Command Allocator" << std::endl;
    }

    return true;
}


bool CreateFencesAndFenceEvent(ID3D12Device* device, ID3D12Fence*& firstFence, ID3D12Fence*& secondFence, ID3D12Fence*& thirdFence, HANDLE& fenceEvent) {
    HRESULT hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&firstFence));
    if (FAILED(hr) || firstFence == nullptr) {
        std::cout << "Failed to create the First Fence" << std::endl;
        return false;
    }
    std::cout << "Success to create the First Fence" << std::endl;

    hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&secondFence));
    if (FAILED(hr) || secondFence == nullptr) {
        std::cout << "Failed to create the Second Fence" << std::endl;
        return false;
    }
    std::cout << "Success to create the Second Fence" << std::endl;

    hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&thirdFence));
    if (FAILED(hr) || thirdFence == nullptr) {
        std::cout << "Failed to create the Third Fence" << std::endl;
        return false;
    }
    std::cout << "Success to create the Third Fence" << std::endl;

    fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (fenceEvent == nullptr) {
        std::cout << "Failed to create the Fence Event" << std::endl;
        return false;
    }
    std::cout << "Success to create the Fence Event" << std::endl;

    return true;
}





bool InitializeDirectX() {
    // If one instance doesn't init correctly, end program
    bool success = true;



    success = CreateDXGIFactory(_dxgiFactory) && success;
    success = CreateDXGIAdapterAndDevice(_dxgiFactory, _dxgiAdapter, _device) && success;
    success = CreateCommandQueue(_device, _commandQueue) && success;
    success = CreateSwapChain(_dxgiFactory, _commandQueue, _swapChain) && success;
    success = CreateDescriptorHeapAndRenderTargets(_device, _swapChain, _rtvDescriptorHeap, _firstRenderTarget, _secondRenderTarget, _thirdRenderTarget) && success;
    success = CreateCommandAllocators(_device, _firstCommandAllocator, _secondCommandAllocator, _thirdCommandAllocator) && success;
    success = CreateCommandList(_device, _firstCommandAllocator, _commandList) && success;
    success = CreateFencesAndFenceEvent(_device, _firstFence, _secondFence, _thirdFence, _fenceEvent) && success;


    if (!success) {

        return false;
    }

    return true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    InitializeWindow(hInstance, nShowCmd);
    MainLoop();
    Cleanup();  // Ajouté pour libérer les ressources
    return 0;
}

void InitializeWindow(HINSTANCE hInstance, int nShowCmd) {

    if (_fullScreen) {
        HMONITOR hmon = MonitorFromWindow(_hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO mi = { sizeof(mi) };
        GetMonitorInfo(hmon, &mi);
        _width = mi.rcMonitor.right - mi.rcMonitor.left;
        _height = mi.rcMonitor.bottom - mi.rcMonitor.top;
    }

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = _windowName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Failed to register window class", L"Error", MB_OK | MB_ICONERROR);
    }

    _hwnd = CreateWindowEx(
        NULL,
        _windowName,
        _windowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        _width, _height,
        NULL,
        NULL,
        _hInstance,
        NULL
    );

    if (!_hwnd) {
        MessageBox(NULL, L"Failed to create window", L"Error", MB_OK | MB_ICONERROR);
    }

    // Create console
    AllocConsole();

    if (freopen_s(&_pConsole, "CONOUT$", "w", stdout) != 0) {
        MessageBox(0, L"Failed to redirect console output", L"Error", MB_OK);
        Cleanup();
        PostQuitMessage(1);
    }

    ShowWindow(_hwnd, nShowCmd);
    UpdateWindow(_hwnd);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            if (MessageBox(0, L"Are you sure you want to quit?", L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
                DestroyWindow(hwnd);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Cleanup() {
    if (_pConsole) {
        fclose(_pConsole);
        FreeConsole();
    }
}

void MainLoop() {
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
            // Your rendering or game logic goes here
            // For now, we'll just sleep to prevent high CPU usage
            /*std::cout << "hello" << std::endl;*/
            Sleep(100);
        }
    }
}


//void CleanupDirectX() {
//    // Code de CleanupDirectX
//}
//
//void Update() {
//    // Code de Update
//}
//
//void Render() {
//    // Code de Render
//}
//
//void UpdatePipeline() {
//    // Code de UpdatePipeline
//}
//
//void WaitForPreviousFrame() {
//    // Code de WaitForPreviousFrame
//}
