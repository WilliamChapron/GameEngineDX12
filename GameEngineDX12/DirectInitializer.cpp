#include "DirectInitializer.h"
#include "WindowInitializer.h"
#include "EngineManager.h"
#include <iostream>

namespace DirectInitializer {
    IDXGIAdapter1* _dxgiAdapter = nullptr;
    IDXGIFactory4* _dxgiFactory = nullptr;
    ID3D12Device* _device = nullptr;
    IDXGISwapChain3* _swapChain = nullptr;
    ID3D12CommandQueue* _commandQueue = nullptr;
    ID3D12DescriptorHeap* _rtvDescriptorHeap = nullptr;
    RenderTarget _firstRenderTarget;
    RenderTarget _secondRenderTarget;
    RenderTarget _thirdRenderTarget;
    CommandAllocator _firstCommandAllocator;
    CommandAllocator _secondCommandAllocator;
    CommandAllocator _thirdCommandAllocator;
    ID3D12GraphicsCommandList* _commandList = nullptr;
    Fence _firstFence;
    Fence _secondFence;
    Fence _thirdFence;
    int _frameIndex = 0;
    int _rtvDescriptorSize = 0;
    HANDLE _fenceEvent = nullptr;
    UINT64 _firstFenceValue;
    UINT64 _secondFenceValue;
    UINT64 _thirdFenceValue;

    void DirectInitializer::Destructor()
    {
        // Destroy Instance
    }


    bool DirectInitializer::Initialize() {




        _firstCommandAllocator = { nullptr, false };
        _secondCommandAllocator = { nullptr, false };
        _thirdCommandAllocator = { nullptr, false };


        // If one instance don't init correctly, end program
        bool success;
        success = CreateDXGIFactory();
        success =  CreateDXGIAdapterAndDevice();
        success = CreateCommandQueue();
        success = CreateSwapChain();
        success = CreateDescriptorHeapAndRenderTargets();
        success = CreateCommandAllocators();
        success = CreateCommandList();
        success = CreateFencesAndFenceEvent();

        if (!success) return false;

        return true;

        
    }

    bool DirectInitializer::CreateDXGIFactory() {
        HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory));
        if (FAILED(hr)) {
            std::cout << "Failed to create DXGIFactory." << std::endl;
            return false;
        }
        if (_dxgiFactory != nullptr) {
            std::cout << "Success to create DXGIFactory." << std::endl;
        }

        return true;
    }

    IDXGIFactory4* DirectInitializer::GetDXGIFactory() {
        return _dxgiFactory;
    }

    bool DirectInitializer::CreateDXGIAdapterAndDevice() {
        int adapterIndex = 0;
        bool adapterFound = false;

        while (_dxgiFactory->EnumAdapters1(adapterIndex, &_dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {
            DXGI_ADAPTER_DESC1 desc;
            _dxgiAdapter->GetDesc1(&desc);

            // If Software adapter, move to the next one
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                adapterIndex++;
                continue;
            }

            // Test if the device is compatible with the required version
            HRESULT hr =
                D3D12CreateDevice(
                    _dxgiAdapter,
                    D3D_FEATURE_LEVEL_11_0,
                    _uuidof(ID3D12Device),
                    reinterpret_cast<void**>(&_device)
                );

            // Device Creation State
            if (SUCCEEDED(hr) && _device != nullptr) {
                std::cout << "Success to create D3D12 device." << std::endl;
                adapterFound = true;
                break;
            }
            if (FAILED(hr) || _device == nullptr) {
                std::cout << "Failed to create D3D12 device / continue try find other adapter" << std::endl;
            }
            adapterIndex++;
        }

        // Adapter Creation State
        if (_dxgiAdapter != nullptr) {
            std::cout << "Success to find and store DXGIAdapter." << std::endl;
        }
        else {
            std::cout << "Failed to find DXGI adapter." << std::endl;
            return false;
        }

        return true;
    }

    IDXGIAdapter1* DirectInitializer::GetDXGIAdapter() {
        return _dxgiAdapter;
    }

    ID3D12Device* DirectInitializer::GetDevice() {
        return _device;
    }

    bool DirectInitializer::CreateCommandQueue()
    {
        // Command Queue Description
        D3D12_COMMAND_QUEUE_DESC cqDesc = {}; // default values

        HRESULT hr = _device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&_commandQueue));
        if (FAILED(hr) || _commandQueue == nullptr)
        {
            std::cout << "Failed to create Device Command Queue." << std::endl;
            return false;
        }
        if (SUCCEEDED(hr) && _commandQueue != nullptr)
        {
            std::cout << "Success to create Device Command Queue." << std::endl;
            return true;
        }
        return true;
    }

    ID3D12CommandQueue* DirectInitializer::GetCommandQueue() {
        return _commandQueue;
    }

    bool DirectInitializer::CreateSwapChain()
    {


        DXGI_MODE_DESC backBufferDesc = {};
        backBufferDesc.Width = WindowInitializer::_width;
        backBufferDesc.Height = WindowInitializer::_height;
        backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;


        DXGI_SAMPLE_DESC sampleDesc = {};
        sampleDesc.Count = 1; 


        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = _frameBufferCount; 
        swapChainDesc.BufferDesc = backBufferDesc; 
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; 
        swapChainDesc.OutputWindow = WindowInitializer::_hwnd; 
        swapChainDesc.SampleDesc = sampleDesc; 
        swapChainDesc.Windowed = !WindowInitializer::_fullScreen; 

        IDXGISwapChain* tempSwapChain;

        _dxgiFactory->CreateSwapChain(
            _commandQueue, 
            &swapChainDesc, 
            &tempSwapChain 
        );

        _swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);



        if (_swapChain == nullptr)
        {
            std::cout << "Failed to create Device Swap Chain." << std::endl;
            return false;
        }
        if (_swapChain != nullptr)
        {
            _frameIndex = _swapChain->GetCurrentBackBufferIndex();

            std::cout << "Success to create Device Swap Chain." << std::endl;
            return true;
        }
        return true;
    }

    IDXGISwapChain3* DirectInitializer::GetSwapChain() 
    {
        return _swapChain;
    }

    bool DirectInitializer::CreateDescriptorHeapAndRenderTargets() 
    {
        
        // Creating 3 descriptors
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = _frameBufferCount; 
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; 

        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        HRESULT hr = _device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvDescriptorHeap));


        if (_rtvDescriptorHeap == nullptr || FAILED(hr))
        {
            return false;
            std::cout << "Failed to create Descriptor Heap." << std::endl;
        }
        if (_rtvDescriptorHeap != nullptr && SUCCEEDED(hr))
        {
            std::cout << "Success to create Descriptor Heap." << std::endl;
        }


        _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart()); 

        // Create Render target View 


        // First Render Target
        hr = DirectInitializer::_swapChain->GetBuffer(0, IID_PPV_ARGS(&_firstRenderTarget.resource));

        if (_firstRenderTarget.resource == nullptr || FAILED(hr)) {
            std::cout << "Failed to create the First Render Target" << std::endl;
            return false;
        }

        if (_firstRenderTarget.resource != nullptr && SUCCEEDED(hr)) {
            std::cout << "Success to create the First Render Target" << std::endl;
            _firstRenderTarget.isActive = true;
        }

        DirectInitializer::_device->CreateRenderTargetView(_firstRenderTarget.resource, nullptr, rtvHandle);
        rtvHandle.Offset(1, _rtvDescriptorSize);

        // Second Render Target
        hr = DirectInitializer::_swapChain->GetBuffer(1, IID_PPV_ARGS(&_secondRenderTarget.resource));

        if (_secondRenderTarget.resource == nullptr || FAILED(hr)) {
            std::cout << "Failed to create the Second Render Target" << std::endl;
            return false;
        }

        if (_secondRenderTarget.resource != nullptr && SUCCEEDED(hr)) {
            std::cout << "Success to create the Second Render Target" << std::endl;
            _secondRenderTarget.isActive = true;
        }

        DirectInitializer::_device->CreateRenderTargetView(_secondRenderTarget.resource, nullptr, rtvHandle);
        rtvHandle.Offset(1, _rtvDescriptorSize);

        // Third Render Target
        hr = DirectInitializer::_swapChain->GetBuffer(2, IID_PPV_ARGS(&_thirdRenderTarget.resource));

        if (_thirdRenderTarget.resource == nullptr || FAILED(hr)) {
            std::cout << "Failed to create the Third Render Target" << std::endl;
            return false;
        }

        if (_thirdRenderTarget.resource != nullptr && SUCCEEDED(hr)) {
            std::cout << "Success to create the Third Render Target" << std::endl;
            _thirdRenderTarget.isActive = true;
        }

        DirectInitializer::_device->CreateRenderTargetView(_thirdRenderTarget.resource, nullptr, rtvHandle);
        rtvHandle.Offset(1, _rtvDescriptorSize);

        return true;

    }
    
    ID3D12DescriptorHeap* DirectInitializer::GetDescriptorHeap() 
    {
        return _rtvDescriptorHeap;
    }

    ID3D12Resource* DirectInitializer::GetRenderTarget(int index)
    {
        switch (index)
        {
        case 0:
            return _firstRenderTarget.resource;
        case 1:
            return _secondRenderTarget.resource;
        case 2:
            return _thirdRenderTarget.resource;
        default:
            return nullptr;
        }
    }


    bool DirectInitializer::CreateCommandAllocators() {



        HRESULT hr = DirectInitializer::_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_firstCommandAllocator.allocator));

        if (FAILED(hr) || _firstCommandAllocator.allocator == nullptr) {
            std::cout << "Failed to create the first Command Allocator." << std::endl;
            return false;
        }

        std::cout << "Success to create the first Command Allocator." << std::endl;
        _firstCommandAllocator.isActive = true;

        HRESULT hrr = DirectInitializer::_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_secondCommandAllocator.allocator));

        if (FAILED(hrr) || _secondCommandAllocator.allocator == nullptr) {
            std::cout << "Failed to create the second Command Allocator." << std::endl;
            return false;
        }

        std::cout << "Success to create the second Command Allocator." << std::endl;
        _secondCommandAllocator.isActive = true;

        HRESULT hrrr = DirectInitializer::_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_thirdCommandAllocator.allocator));

        if (FAILED(hrrr) || _thirdCommandAllocator.allocator == nullptr) {
            std::cout << "Failed to create the third Command Allocator." << std::endl;
            return false;
        }

        std::cout << "Success to create the third Command Allocator." << std::endl;
        _thirdCommandAllocator.isActive = true;


        

        return true;
    }

    ID3D12CommandAllocator* DirectInitializer::GetCommandAllocator(int index) 
    {
        switch (index)
        {
        case 0:
            return _firstCommandAllocator.allocator;
        case 1:
            return _secondCommandAllocator.allocator;
        case 2:
            return _thirdCommandAllocator.allocator;
        default:
            return nullptr;
        }
    }

    
    bool DirectInitializer::CreateCommandList() 
    {
        HRESULT hr = _device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _firstCommandAllocator.allocator, NULL, IID_PPV_ARGS(&_commandList));
        if (FAILED(hr) || _commandList == nullptr)
        {
            std::cout << "Failed to create the Command List"<< std::endl;
            return false;

        }
        if (SUCCEEDED(hr) && _commandList != nullptr)
        {
            std::cout << "Success to create the Command List" << std::endl;
        }

        HRESULT dhr = _firstCommandAllocator.allocator->Reset();
        if (FAILED(dhr))
        {
            if (FACILITY_WINDOWS == HRESULT_FACILITY(dhr))
                dhr = HRESULT_CODE(dhr);

            TCHAR* szErrMsg;
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dhr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szErrMsg, 0, NULL) != 0)
            {
                std::wcout << L"Failed to reset Command Allocator. Error code: " << dhr << L" - " << (LPWSTR)szErrMsg << std::endl;
                LocalFree(szErrMsg);
            }
        }

        if (SUCCEEDED(dhr)) {
            /*std::cout << "is active" << DirectInitializer::_commandAllocators[i + 1].isActive << std::endl;*/
            std::cout << "Success To Reset Command Allocator" << std::endl;
        }


        return true;
    }
    
    ID3D12GraphicsCommandList* DirectInitializer::GetCommandList() 
    {
        return _commandList;
    }

    bool DirectInitializer::CreateFencesAndFenceEvent()
    {

        HRESULT hr = _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_firstFence.fence));
        if (FAILED(hr) || _firstFence.fence == nullptr) {
            std::cout << "Failed to create the First Fence" << std::endl;
            return false;
        }
        _firstFence.isActive = true;
        _firstFenceValue = 0;


        hr = _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_secondFence.fence));
        if (FAILED(hr) || _secondFence.fence == nullptr) {
            std::cout << "Failed to create the Second Fence" << std::endl;
            return false;
        }
        _secondFence.isActive = true;
        _secondFenceValue = 0;


        hr = _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_thirdFence.fence));
        if (FAILED(hr) || _thirdFence.fence == nullptr) {
            std::cout << "Failed to create the Third Fence" << std::endl;
            return false;
        }
        _thirdFence.isActive = true;
        _thirdFenceValue = 0;


        _fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (_fenceEvent == nullptr) {
            std::cout << "Failed to create the Fence Event" << std::endl;
            return false;
        }

        if (_fenceEvent != nullptr) {
            std::cout << "Success to create the Fence Event" << std::endl;
        }

        return true;
    }

    ID3D12Fence* DirectInitializer::GetFence(int index) 
    {
        switch (index)
        {
        case 0:
            return _firstFence.fence;
        case 1:
            return _secondFence.fence;
        case 2:
            return _thirdFence.fence;
        default:
            return nullptr;
        }
    }

    HANDLE DirectInitializer::GetFenceEvent() 
    {
        return _fenceEvent;
    }

    
    UINT64 DirectInitializer::GetFenceValue(int index) 
    {
        switch (index)
        {
        case 0:
            return _firstFenceValue;
        case 1:
            return _secondFenceValue;
        case 2:
            return _thirdFenceValue;
        default:
            return 0;
        }
    }

    int DirectInitializer::GetFrameIndex() {
        return _frameIndex;
    }

    int DirectInitializer::GetRtvDescriptorSize() 
    {
        return _rtvDescriptorSize;
    }
}















