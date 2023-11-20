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
    ID3D12Resource* _renderTargets[_frameBufferCount];
    ID3D12CommandAllocator* _commandAllocators[_frameBufferCount];
    ID3D12GraphicsCommandList* _commandList = nullptr;
    ID3D12Fence* _fences[_frameBufferCount];
    HANDLE _fenceEvent = nullptr;
    UINT64 _fencesValue[_frameBufferCount];
    int _frameIndex = 0;
    int _rtvDescriptorSize = 0;

    void DirectInitializer::Destructor()
    {
        // Destroy Instance
    }


    bool DirectInitializer::Initialize() {
        for (int i = 0; i < _frameBufferCount; ++i) {
            _renderTargets[i] = nullptr;
            _commandAllocators[i] = nullptr;
        }


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

        for (int i = 0; i < _frameBufferCount; i++)
        {
            HRESULT hr = _swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]));

            if (_renderTargets[i] == nullptr || FAILED(hr))
            {
                return false;
                std::cout << "Failed to create the Render Target number " << i+1 << " on " << _frameBufferCount  << std::endl;
            }
            if (_renderTargets[i] != nullptr && SUCCEEDED(hr))
            {
                std::cout << "Success to create the Render Target number " << i+1 << " on " << _frameBufferCount << std::endl;
            }

            _device->CreateRenderTargetView(_renderTargets[i], nullptr, rtvHandle);

            rtvHandle.Offset(1, _rtvDescriptorSize);
        }
        return true;

    }
    
    ID3D12DescriptorHeap* DirectInitializer::GetDescriptorHeap() 
    {
        return _rtvDescriptorHeap;
    }

    ID3D12Resource* DirectInitializer::GetRenderTarget(int index)
    {
        return _renderTargets[index];
    }

    bool DirectInitializer::CreateCommandAllocators() 
    {
        for (int i = 0; i < _frameBufferCount; i++)
        {
            HRESULT hr = _device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_commandAllocators[i]));
            if (FAILED(hr) || _commandAllocators[i] == nullptr)
            {
                std::cout << "Failed to create the Command Allocators number " << i + 1 << " on " << _frameBufferCount << std::endl;
                return false;

            }
            if (SUCCEEDED(hr) && _commandAllocators[i] != nullptr)
            {
                std::cout << "Success to create the Command Allocators number " << i + 1 << " on " << _frameBufferCount << std::endl;
            }
        }
        return true;
    }

    ID3D12CommandAllocator* DirectInitializer::GetCommandAllocator(int index) 
    {
        return _commandAllocators[index];
    }

    
    bool DirectInitializer::CreateCommandList() 
    {
        HRESULT hr = _device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _commandAllocators[0], NULL, IID_PPV_ARGS(&_commandList));
        if (FAILED(hr) || _commandList == nullptr)
        {
            std::cout << "Failed to create the Command List"<< std::endl;
            return false;

        }
        if (SUCCEEDED(hr) && _commandList != nullptr)
        {
            std::cout << "Success to create the Command List" << std::endl;
        }
    }
    
    ID3D12GraphicsCommandList* DirectInitializer::GetCommandList() 
    {
        return _commandList;
    }

    bool CreateFencesAndFenceEvent() 
    {
        for (int i = 0; i < _frameBufferCount; i++)
        {
            HRESULT hr = _device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fences[i]));
            if (FAILED(hr) || _fences[i] == nullptr)
            {
                std::cout << "Failed to create the Fence number " << i + 1 << " on " << _frameBufferCount << std::endl;
                return false;

            }
            if (SUCCEEDED(hr) || _fences[i] != nullptr)
            {
                std::cout << "Success to create the Fence number " << i + 1 << " on " << _frameBufferCount << std::endl;
            }
            _fencesValue[i] = 0; // set the initial fence value to 0
        }

        // create a handle to a fence event from WIN API 
        _fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (_fenceEvent == nullptr)
        {
            std::cout << "Failed to create the Fence Event" << std::endl;
            return false;
        }
        if (_fenceEvent != nullptr)
        {
            std::cout << "Success to create the Fence Event" << std::endl;
        }

        return true;
    }

    ID3D12Fence* DirectInitializer::GetFence(int index) 
    {
        return _fences[index];
    }

    HANDLE DirectInitializer::GetFenceEvent() 
    {
        return _fenceEvent;
    }

    
    UINT64 DirectInitializer::GetFenceValue(int index) 
    {
        return _fencesValue[index];
    }

    int DirectInitializer::GetFrameIndex() {
        return _frameIndex;
    }

    int DirectInitializer::GetRtvDescriptorSize() 
    {
        return _rtvDescriptorSize;
    }
}















