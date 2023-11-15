#include "stdafx.h"

namespace DirectInitializer {
    void Initialize();  
    void Update();      
    void UpdatePipeline();
    void Render();      
    void Cleanup();
    void WaitForPreviousFrame();


    // Get / Set 

    bool CreateDXGIFactory();
    IDXGIFactory4* GetDXGIFactory();

    bool CreateDXGIAdapterAndDevice();
    IDXGIAdapter1* GetDXGIAdapter();
    ID3D12Device* GetDevice();

    bool CreateCommandQueue();
    ID3D12CommandQueue* GetCommandQueue();


    bool CreateSwapChain();
    IDXGISwapChain3* GetSwapChain();


    bool CreateDescriptorHeapAndRenderTargets();
    ID3D12DescriptorHeap* GetDescriptorHeap();
    ID3D12Resource* GetRenderTarget(int index);

    bool CreateCommandAllocators();
    ID3D12CommandAllocator* GetCommandAllocator(int index);

    //bool CreateCommandList();
    //ID3D12GraphicsCommandList* GetCommandList();

    //bool CreateFence(int index);
    //ID3D12Fence* GetFence(int index);

    //HANDLE CreateFenceEvent();
    //HANDLE GetFenceEvent();

    //UINT64 GetFenceValue(int index);

    //int GetFrameIndex();
    //int GetRtvDescriptorSize();


    constexpr int _frameBufferCount = 3; // number of buffers we want, 2 for double buffering, 3 for tripple buffering

    extern IDXGIAdapter1* _dxgiAdapter;// Adapter graphics cards, we will be able to communicate with

    extern IDXGIFactory4* _dxgiFactory; // Instance creator for dxgi objects

    extern ID3D12Device* _device; // intermediate system to store/work our graphics data

    extern IDXGISwapChain3* _swapChain; // swapchain

    extern ID3D12CommandQueue* _commandQueue; // container for command lists

    extern ID3D12DescriptorHeap* _rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets

    extern ID3D12Resource* _renderTargets[_frameBufferCount]; // number of render targets equal to buffer count

    extern ID3D12CommandAllocator* _commandAllocators[_frameBufferCount]; // we want enough allocators for each buffer * number of threads (we only have one thread)

    extern ID3D12GraphicsCommandList* _commandList; // a command list we can record commands into, then execute them to render the frame

    extern ID3D12Fence* _fences[_frameBufferCount];    // an object that is locked while our command list is being executed by the gpu. We need as many 
    //as we have allocators (more if we want to know when the gpu is finished with an asset)

    extern HANDLE _fenceEvent; // a handle to an event when our fence is unlocked by the gpu

    extern UINT64 _fenceValues[_frameBufferCount]; // this value is incremented each frame. each fence will have its own value

    extern int _frameIndex; // current rtv we are on

    extern int _rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
};



