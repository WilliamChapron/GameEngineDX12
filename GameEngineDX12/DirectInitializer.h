#include "stdafx.h"

namespace DirectInitializer {
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

    extern IDXGIAdapter1* _dxgiAdapter; // Carte graphique, avec laquelle nous pourrons communiquer

    extern IDXGIFactory4* _dxgiFactory; // Créateur d'instances pour les objets dxgi

    extern ID3D12Device* _device; // Système intermédiaire pour stocker/traiter nos données graphiques

    extern IDXGISwapChain3* _swapChain; // Chaîne d'échanges (swapchain)

    extern ID3D12CommandQueue* _commandQueue; // Conteneur pour les listes de commandes

    extern ID3D12DescriptorHeap* _rtvDescriptorHeap; // Un tas de descripteurs pour contenir des ressources comme les cibles de rendu

    extern ID3D12Resource* _renderTargets[_frameBufferCount]; // Nombre de cibles de rendu égal au nombre de tampons

    extern ID3D12CommandAllocator* _commandAllocators[_frameBufferCount]; // Nous voulons suffisamment d'allocateurs pour chaque tampon * nombre de threads (nous n'avons qu'un seul thread)

    extern ID3D12GraphicsCommandList* _commandList; // Une liste de commandes dans laquelle nous pouvons enregistrer des commandes, puis les exécuter pour rendre l'image

    extern ID3D12Fence* _fences[_frameBufferCount]; // Un objet qui est verrouillé pendant l'exécution de notre liste de commandes par le GPU. Nous en avons besoin autant que d'allocateurs (plus si nous voulons savoir quand le GPU a terminé avec une ressource)

    extern HANDLE _fenceEvent; // Une poignée vers un événement lorsque notre barrière est déverrouillée par le GPU

    extern UINT64 _fencesValue[_frameBufferCount]; // Cette valeur est incrémentée à chaque image. Chaque barrière aura sa propre valeur

    extern int _frameIndex; // Indice actuel de la cible de rendu sur laquelle nous sommes

    extern int _rtvDescriptorSize; // Taille du descripteur de la cible de rendu sur le périphérique (toutes les cibles avant et arrière auront la même taille)

};



