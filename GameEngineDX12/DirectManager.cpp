#include "DirectManager.h"
#include "DirectInitializer.h"  
#include <iostream> 

namespace DirectManager {
    void InitializeDirectX() 
    {

    }

    void Update() 
    {
        
    }

    void UpdatePipeline()
    {
        HRESULT hr;

        int frameIndex = DirectInitializer::_frameIndex;
        DirectManager::WaitForPreviousFrame();
        /*std::cout << frameIndex << std::endl;*/

        // we want to wait for the gpu to finish executing the command list before we start releasing everything
        hr = DirectInitializer::_commandAllocators[frameIndex]->Reset();
        if (FAILED(hr))
        {
            std::cout << "Failed to reset Command Allocator" << std::endl;
            return;
        }

        // Clear Command list 
        hr = DirectInitializer::_commandList->Reset(DirectInitializer::_commandAllocators[frameIndex], NULL);
        if (FAILED(hr))
        {
            std::cout << "Failed to reset Command List" << std::endl;
            return;
        }

        // transition the "frameIndex" render target from the present state to the render target state so the command list draws to it starting from here
        DirectInitializer::_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(DirectInitializer::_renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

        // here we again get the handle to our current render target view so we can set it as the render target in the output merger stage of the pipeline
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(DirectInitializer::_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, DirectInitializer::_rtvDescriptorSize);

        // set the render target for the output merger stage (the output of the pipeline)
        DirectInitializer::_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

        // Clear the render target by using the ClearRenderTargetView command
        const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        DirectInitializer::_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

        // transition the "frameIndex" render target from the render target state to the present state. If the debug layer is enabled, you will receive a
        // warning if present is called on the render target when it's not in the present state
        DirectInitializer::_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(DirectInitializer::_renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

        hr = DirectInitializer::_commandList->Close();
        if (FAILED(hr))
        {
            std::cout << "Failed to close Command List" << std::endl;
            /*return;*/
        }
    }

    void Render() 
    {
        HRESULT hr;

        int frameIndex = DirectInitializer::_frameIndex;

        UpdatePipeline(); // update the pipeline by sending commands to the commandqueue

        // create an array of command lists (only one command list here)
        ID3D12CommandList* ppCommandLists[] = { DirectInitializer::_commandList }; // Prefer vector

        // execute the array of command lists
        DirectInitializer::_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists); // Prefer size

        // this command goes in at the end of our command queue. we will know when our command queue 
        // has finished because the fence value will be set to "fenceValue" from the GPU since the command
        // queue is being executed on the GPU
        hr = DirectInitializer::_commandQueue->Signal(DirectInitializer::_fences[frameIndex], DirectInitializer::_fencesValue[frameIndex]);
        if (FAILED(hr))
        {
            std::cout << "Failed to trying get signal gpu finishing" << std::endl;
            return;
        }

        // present the current backbuffer
        hr = DirectInitializer::_swapChain->Present(0, 0);
        if (FAILED(hr))
        {
            return;
        }
    }

    void WaitForPreviousFrame()
    {

    }
}
