#include "DirectInitializer.h"
#include <iostream>


namespace DirectInitializer {
    IDXGIAdapter1* _dxgiAdapter = nullptr;
    IDXGIFactory4* _dxgiFactory = nullptr;
    ID3D12Device* _device = nullptr;
    IDXGISwapChain3* _swapChain = nullptr;
    ID3D12CommandQueue* _commandQueue = nullptr;
    ID3D12DescriptorHeap* _rtvDescriptorHeap = nullptr;
    ID3D12Resource* _renderTargets[_frameBufferCount];
    ID3D12CommandAllocator* _commandAllocator[_frameBufferCount];
    ID3D12GraphicsCommandList* _commandList = nullptr;
    ID3D12Fence* _fence[_frameBufferCount];
    HANDLE _fenceEvent = nullptr;
    UINT64 _fenceValue[_frameBufferCount];
    int _frameIndex = 0;
    int _rtvDescriptorSize = 0;


    void DirectInitializer::Initialize() {
        for (int i = 0; i < _frameBufferCount; ++i) {
            _renderTargets[i] = nullptr;
            _commandAllocator[i] = nullptr;
            _fence[i] = nullptr;
        }


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
}




//bool DirectInitializer::CreateSwapChain() 
//{
//    //// -- Create the Swap Chain (double/tripple buffering) -- //
//
//    //DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
//    //backBufferDesc.Width = Width; // buffer width
//    //backBufferDesc.Height = Height; // buffer height
//    //backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each chanel)
//
//    //// describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
//    //DXGI_SAMPLE_DESC sampleDesc = {};
//    //sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)
//
//    //// Describe and create the swap chain.
//    //DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
//    //swapChainDesc.BufferCount = frameBufferCount; // number of buffers we have
//    //swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
//    //swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
//    //swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
//    //swapChainDesc.OutputWindow = hwnd; // handle to our window
//    //swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
//    //swapChainDesc.Windowed = !FullScreen; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps
//
//    //IDXGISwapChain* tempSwapChain;
//
//    //dxgiFactory->CreateSwapChain(
//    //    commandQueue, // the queue will be flushed once the swap chain is created
//    //    &swapChainDesc, // give it the swap chain description we created above
//    //    &tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
//    //);
//
//    //swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);
//
//    //frameIndex = swapChain->GetCurrentBackBufferIndex();
//}

//IDXGISwapChain3* DirectInitializer::GetSwapChain() 
//{
//
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
