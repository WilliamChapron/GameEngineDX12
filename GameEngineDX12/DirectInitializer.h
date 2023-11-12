#include "stdafx.h"

class DirectInitializer {
public:
    DirectInitializer();
    ~DirectInitializer();

    bool Initialize();  
    void Update();      
    void UpdatePipeline();
    void Render();      
    void Cleanup();
    void WaitForPreviousFrame();


    // Get / Set 

    ID3D12Device* CreateDevice();
    ID3D12Device* GetDevice();

    IDXGISwapChain3* CreateSwapChain();
    IDXGISwapChain3* GetSwapChain();

    ID3D12CommandQueue* CreateCommandQueue();
    ID3D12CommandQueue* GetCommandQueue();

    ID3D12DescriptorHeap* CreateDescriptorHeap();
    ID3D12DescriptorHeap* GetDescriptorHeap();

    ID3D12Resource* CreateRenderTarget(int index);
    ID3D12Resource* GetRenderTarget(int index);

    ID3D12CommandAllocator* CreateCommandAllocator(int index);
    ID3D12CommandAllocator* GetCommandAllocator(int index);

    ID3D12GraphicsCommandList* CreateCommandList();
    ID3D12GraphicsCommandList* GetCommandList();

    ID3D12Fence* CreateFence(int index);
    ID3D12Fence* GetFence(int index);

    HANDLE CreateFenceEvent();
    HANDLE GetFenceEvent();

    UINT64 GetFenceValue(int index);

    int GetFrameIndex();
    int GetRtvDescriptorSize();

private:
    



    // direct3d stuff
    static const int frameBufferCount = 3;; // number of buffers we want, 2 for double buffering, 3 for tripple buffering

    ID3D12Device* device; // direct3d device

    IDXGISwapChain3* swapChain; // swapchain used to switch between render targets

    ID3D12CommandQueue* commandQueue; // container for command lists

    ID3D12DescriptorHeap* rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets

    ID3D12Resource* renderTargets[frameBufferCount]; // number of render targets equal to buffer count

    ID3D12CommandAllocator* commandAllocator[frameBufferCount]; // we want enough allocators for each buffer * number of threads (we only have one thread)

    ID3D12GraphicsCommandList* commandList; // a command list we can record commands into, then execute them to render the frame

    ID3D12Fence* fence[frameBufferCount];    // an object that is locked while our command list is being executed by the gpu. We need as many 
    //as we have allocators (more if we want to know when the gpu is finished with an asset)

    HANDLE fenceEvent; // a handle to an event when our fence is unlocked by the gpu

    UINT64 fenceValue[frameBufferCount]; // this value is incremented each frame. each fence will have its own value

    int frameIndex; // current rtv we are on

    int rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
};

