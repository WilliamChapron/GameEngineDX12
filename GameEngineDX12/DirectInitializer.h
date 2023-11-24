#include "stdafx.h"



namespace DirectInitializer {

    struct RenderTarget {
        ID3D12Resource* resource;
        bool isActive;
    };

    struct CommandAllocator {
        ID3D12CommandAllocator* allocator;
        bool isActive;
    };

    struct AStruct {
        bool* b;
    };

    struct Fence {
        ID3D12Fence* fence;
        bool isActive;
    };


    bool Initialize();  

    void Destructor();


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

    bool CreateCommandList();
    ID3D12GraphicsCommandList* GetCommandList();

    bool CreateFencesAndFenceEvent();
    ID3D12Fence* GetFence(int index);
    HANDLE GetFenceEvent();

    UINT64 GetFenceValue(int index);

    int GetFrameIndex();
    int GetRtvDescriptorSize();


    constexpr int _frameBufferCount = 3; // Nombre de tampons, 2 pour la double mise en mémoire tampon, 3 pour la triple mise en mémoire tampon

    extern IDXGIAdapter1* _dxgiAdapter;
    extern IDXGIFactory4* _dxgiFactory;
    extern ID3D12Device* _device;
    extern IDXGISwapChain3* _swapChain;
    extern ID3D12CommandQueue* _commandQueue;
    extern ID3D12DescriptorHeap* _rtvDescriptorHeap;
    extern std::vector<RenderTarget> _renderTargets;
    extern std::vector<CommandAllocator> _commandAllocators;
    extern ID3D12CommandAllocator* _commandAllocator;
    extern ID3D12GraphicsCommandList* _commandList;
    extern std::vector<Fence> _fences;
    extern int _frameIndex;
    extern int _rtvDescriptorSize;

    extern HANDLE _fenceEvent; 
    extern std::vector<UINT64> _fencesValue; 


};



