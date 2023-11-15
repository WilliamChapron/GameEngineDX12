#include "stdafx.h"

class DirectInitializer {
public:
    DirectInitializer();
    ~DirectInitializer();

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


    //bool CreateSwapChain();
    //IDXGISwapChain3* GetSwapChain();


    //bool CreateDescriptorHeap();
    //ID3D12DescriptorHeap* GetDescriptorHeap();

    //bool CreateRenderTarget(int index);
    //ID3D12Resource* GetRenderTarget(int index);

    //bool CreateCommandAllocator(int index);
    //ID3D12CommandAllocator* GetCommandAllocator(int index);

    //bool CreateCommandList();
    //ID3D12GraphicsCommandList* GetCommandList();

    //bool CreateFence(int index);
    //ID3D12Fence* GetFence(int index);

    //HANDLE CreateFenceEvent();
    //HANDLE GetFenceEvent();

    //UINT64 GetFenceValue(int index);

    //int GetFrameIndex();
    //int GetRtvDescriptorSize();

private:
    




    static const int _frameBufferCount = 3;; // number of buffers we want, 2 for double buffering, 3 for tripple buffering

    IDXGIAdapter1* _dxgiAdapter;// Adapter graphics cards, we will be able to communicate with

    IDXGIFactory4* _dxgiFactory; // Instance creator for dxgi objects

    ID3D12Device* _device; // intermediate system to store/work our graphics data

    IDXGISwapChain3* _swapChain; // swapchain

    ID3D12CommandQueue* _commandQueue; // container for command lists

    ID3D12DescriptorHeap* _rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets

    ID3D12Resource* _renderTargets[_frameBufferCount]; // number of render targets equal to buffer count

    ID3D12CommandAllocator* _commandAllocator[_frameBufferCount]; // we want enough allocators for each buffer * number of threads (we only have one thread)

    ID3D12GraphicsCommandList* _commandList; // a command list we can record commands into, then execute them to render the frame

    ID3D12Fence* _fence[_frameBufferCount];    // an object that is locked while our command list is being executed by the gpu. We need as many 
    //as we have allocators (more if we want to know when the gpu is finished with an asset)

    HANDLE _fenceEvent; // a handle to an event when our fence is unlocked by the gpu

    UINT64 _fenceValue[_frameBufferCount]; // this value is incremented each frame. each fence will have its own value

    int _frameIndex; // current rtv we are on

    int _rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)
};

