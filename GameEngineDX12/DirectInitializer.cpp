#include "DirectInitializer.h"
#include <iostream>

DirectInitializer::DirectInitializer() :
    _dxgiAdapter(nullptr),
    _dxgiFactory(nullptr),
    _device(nullptr),
    _swapChain(nullptr),
    _commandQueue(nullptr),
    _rtvDescriptorHeap(nullptr),
    _commandList(nullptr),
    _fenceEvent(nullptr)
{
    // Initialisation des tableaux à nullptr
    for (int i = 0; i < _frameBufferCount; ++i) {
        _renderTargets[i] = nullptr;
        _commandAllocator[i] = nullptr;
        _fence[i] = nullptr;
    }

}

DirectInitializer::~DirectInitializer() 
{

}

void DirectInitializer::Initialize() {
    CreateDXGIFactory();
    CreateDXGIAdapterAndDevice();
    CreateCommandQueue();
}

void DirectInitializer::Update() {
}

void DirectInitializer::UpdatePipeline() {
}

void DirectInitializer::Render() {
}

void DirectInitializer::Cleanup() {
}

void DirectInitializer::WaitForPreviousFrame() {

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
}

ID3D12CommandQueue* DirectInitializer::GetCommandQueue() {
    return _commandQueue;
}




//bool DirectInitializer::CreateSwapChain() 
//{
//
//}
//
//IDXGISwapChain3* DirectInitializer::GetSwapChain() {
//}
//
//bool DirectInitializer::CreateDescriptorHeap() {
//}
//
//ID3D12DescriptorHeap* DirectInitializer::GetDescriptorHeap() {
//}
//
//bool DirectInitializer::CreateRenderTarget(int index) {
//}
//
//ID3D12Resource* DirectInitializer::GetRenderTarget(int index) {
//}
//
//bool DirectInitializer::CreateCommandAllocator(int index) {
//}
//
//ID3D12CommandAllocator* DirectInitializer::GetCommandAllocator(int index) {
//}
//
//bool DirectInitializer::CreateCommandList() {
//}
//
//ID3D12GraphicsCommandList* DirectInitializer::GetCommandList() {
//}
//
//bool DirectInitializer::CreateFence(int index) {
//}
//
//ID3D12Fence* DirectInitializer::GetFence(int index) {
//}
//
//HANDLE DirectInitializer::CreateFenceEvent() {
//}
//
//HANDLE DirectInitializer::GetFenceEvent() {
//}
//
//UINT64 DirectInitializer::GetFenceValue(int index) {
//}
//
//int DirectInitializer::GetFrameIndex() {
//}
//
//int DirectInitializer::GetRtvDescriptorSize() {
//}
