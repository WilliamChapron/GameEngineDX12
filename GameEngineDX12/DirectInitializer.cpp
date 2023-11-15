#include "DirectInitializer.h"
#include <iostream>

DirectInitializer::DirectInitializer() {
}

DirectInitializer::~DirectInitializer() {
}

//bool DirectInitializer::Initialize() {
//}

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
    std::cout << "Success to create DXGIFactory." << std::endl;
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

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            // Software adapter, move to the next one
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

        if (FAILED(hr)) {
            // Display an error message
            std::cout << "Failed to create D3D12 device." << std::endl;
            return false;
        }

        if (SUCCEEDED(hr)) {
            // Display a success message
            std::cout << "D3D12 device created successfully." << std::endl;
            adapterFound = true;
            break;
        }

        adapterIndex++;
    }

    if (!adapterFound) {
        // Display a message indicating that no suitable adapter was found
        std::cout << "No suitable DXGI adapter found." << std::endl;
        return false;
    }

    return true;
}

//IDXGIAdapter1* DirectInitializer::GetDXGIAdapter() {
//    return _dxgiAdapter;
//}
//
//
//
//ID3D12Device* DirectInitializer::GetDevice() {
//	/*return _device;*/
//}
//
//bool DirectInitializer::CreateSwapChain() {
//}
//
//IDXGISwapChain3* DirectInitializer::GetSwapChain() {
//}
//
//bool DirectInitializer::CreateCommandQueue() {
//}
//
//ID3D12CommandQueue* DirectInitializer::GetCommandQueue() {
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
