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


    constexpr int _frameBufferCount = 3; // Nombre de tampons, 2 pour la double mise en m�moire tampon, 3 pour la triple mise en m�moire tampon

    extern IDXGIAdapter1* _dxgiAdapter; // Carte graphique, avec laquelle nous pourrons communiquer

    extern IDXGIFactory4* _dxgiFactory; // Cr�ateur d'instances pour les objets dxgi

    extern ID3D12Device* _device; // Syst�me interm�diaire pour stocker/traiter nos donn�es graphiques

    extern IDXGISwapChain3* _swapChain; // Cha�ne d'�changes (swapchain)

    extern ID3D12CommandQueue* _commandQueue; // Conteneur pour les listes de commandes

    extern ID3D12DescriptorHeap* _rtvDescriptorHeap; // Un tas de descripteurs pour contenir des ressources comme les cibles de rendu

    extern ID3D12Resource* _renderTargets[_frameBufferCount]; // Nombre de cibles de rendu �gal au nombre de tampons

    extern ID3D12CommandAllocator* _commandAllocators[_frameBufferCount]; // Nous voulons suffisamment d'allocateurs pour chaque tampon * nombre de threads (nous n'avons qu'un seul thread)

    extern ID3D12GraphicsCommandList* _commandList; // Une liste de commandes dans laquelle nous pouvons enregistrer des commandes, puis les ex�cuter pour rendre l'image

    extern ID3D12Fence* _fences[_frameBufferCount]; // Un objet qui est verrouill� pendant l'ex�cution de notre liste de commandes par le GPU. Nous en avons besoin autant que d'allocateurs (plus si nous voulons savoir quand le GPU a termin� avec une ressource)

    extern HANDLE _fenceEvent; // Une poign�e vers un �v�nement lorsque notre barri�re est d�verrouill�e par le GPU

    extern UINT64 _fencesValue[_frameBufferCount]; // Cette valeur est incr�ment�e � chaque image. Chaque barri�re aura sa propre valeur

    extern int _frameIndex; // Indice actuel de la cible de rendu sur laquelle nous sommes

    extern int _rtvDescriptorSize; // Taille du descripteur de la cible de rendu sur le p�riph�rique (toutes les cibles avant et arri�re auront la m�me taille)

};



