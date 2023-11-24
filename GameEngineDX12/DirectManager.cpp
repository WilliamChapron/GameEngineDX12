#include "DirectManager.h"
#include "DirectInitializer.h"  
#include <iostream>


#define SAFE_RELEASE(p) if (p != nullptr) { (p)->Release(); (p) = nullptr; }


namespace DirectManager {
    void InitializeDirectX() 
    {

    }

    void Update() 
    {
        
    }

    void WaitForPreviousFrame()
    {
        //HRESULT hr;
        //int frameIndex = DirectInitializer::_frameIndex;

        //// swap the current rtv buffer index so we draw on the correct buffer
        //DirectInitializer::_frameIndex = DirectInitializer::_swapChain->GetCurrentBackBufferIndex();

        //// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
        //// the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
        //if (DirectInitializer::_fences[frameIndex]->GetCompletedValue() < DirectInitializer::_fencesValue[frameIndex])
        //{
        //    // we have the fence create an event which is signaled once the fence's current value is "fenceValue"
        //    hr = DirectInitializer::_fences[frameIndex]->SetEventOnCompletion(DirectInitializer::_fencesValue[frameIndex], DirectInitializer::_fenceEvent);
        //    if (FAILED(hr))
        //    {
        //        std::cout << "Failed To event Completion" << std::endl;
        //        return;
        //    }

        //    // We will wait until the fence has triggered the event that its current value has reached "fenceValue".
        //    // Once its value has reached "fenceValue", we know the command queue has finished executing
        //    WaitForSingleObject(DirectInitializer::_fenceEvent, INFINITE);
        //    std::cout << "WaitForPreviousFrame: Waited for GPU to finish" << std::endl;
        //}
        //else
        //{
        //    std::cout << "WaitForPreviousFrame: GPU has already finished" << std::endl;
        //}

        //// increment fenceValue for next frame
        //DirectInitializer::_fencesValue[frameIndex]++;
    }


    void UpdatePipeline()
    {
        Sleep(3000);
        HRESULT hr;
        //CD3DX12_RESOURCE_BARRIER transitionBarrier;

        //int frameIndex = DirectInitializer::_frameIndex;
        //std::cout << "frameindex" << frameIndex << std::endl;
        ///*DirectManager::WaitForPreviousFrame();*/
        

        HRESULT hrr;

        HRESULT dhr = DirectInitializer::_commandAllocator->Reset();
        if (FAILED(dhr))
        {
            if (FACILITY_WINDOWS == HRESULT_FACILITY(dhr))
                dhr = HRESULT_CODE(dhr);

            TCHAR* szErrMsg;
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dhr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szErrMsg, 0, NULL) != 0)
            {
                std::wcout << L"Failed to reset Command Allocator. Error code: " << dhr << L" - " << (LPWSTR)szErrMsg << std::endl;
                LocalFree(szErrMsg);
            }
        }

        if (SUCCEEDED(dhr)) {
            std::cout << "Success To Reset Command Allocator" << std::endl;
        }

        /*for (int i = 0; i < DirectInitializer::_frameBufferCount; ++i) {
            if (DirectInitializer::_commandAllocators[i].isActive) {
                std::cout << "isActive is true for Command Allocator number " << i + 1 << " on " << DirectInitializer::_frameBufferCount << std::endl;
            }
            else {
                std::cout << "isActive is false for Command Allocator number " << i + 1 << " on " << DirectInitializer::_frameBufferCount << std::endl;
            }
        }    */
        
        //std::cout << DirectInitializer::_commandAllocators[0].isActive << std::endl;
        //hr = DirectInitializer::_commandAllocators[0].allocator->Reset();
        //if (FAILED(hr))
        //{
        //    // Afficher un message détaillé sur l'erreur
        //    LPVOID errorMsg;
        //    FormatMessage(
        //        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        //        NULL,
        //        hr,
        //        0, // Default language
        //        (LPWSTR)&errorMsg,
        //        0,
        //        NULL
        //    );

        //    std::wcout << L"Failed to reset Command Allocator. Error code: " << hr << L" - " << (LPWSTR)errorMsg << std::endl;

        //    LocalFree(errorMsg);

        //    //return;
        //}
        //if (SUCCEEDED(hr)) {
        //    std::cout << "Success To Reset Command Allocator" << std::endl;
        //}

        
        
        
        //for (int i = 0; i < DirectInitializer::_frameBufferCount; ++i)
        //{
        //    std::string extractedTypeName;
        //    const char* rawTypeName = typeid(DirectInitializer::_commandAllocators[4]).name();
        //    const char* asteriskPosition = std::strchr(rawTypeName, '*');

        //    if (asteriskPosition != nullptr) {
        //        // Calcul de la longueur de la sous-chaîne avant le caractère '*'
        //        size_t lengthBeforeAsterisk = static_cast<size_t>(asteriskPosition - rawTypeName);

        //        // Extraction de la sous-chaîne avant le caractère '*'
        //        extractedTypeName = std::string(rawTypeName, lengthBeforeAsterisk);
        //    }


        //    
        //    const std::type_info& targetType = typeid(ID3D12CommandAllocator);
        //    std::cout <<  targetType.name() << " ----" << extractedTypeName << std::endl;


        





        //// Clear Command list 
        //hr = DirectInitializer::_commandList->Reset(DirectInitializer::_commandAllocators[frameIndex], NULL);
        //if (FAILED(hr))
        //{
        //    std::cout << "Failed to reset Command List" << std::endl;
        //    return;
        //}

        //// transition the "frameIndex" render target from the present state to the render target state so the command list draws to it starting from here
        //transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DirectInitializer::_renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        //DirectInitializer::_commandList->ResourceBarrier(1, &transitionBarrier);

        //// here we again get the handle to our current render target view so we can set it as the render target in the output merger stage of the pipeline
        //CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(DirectInitializer::_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, DirectInitializer::_rtvDescriptorSize);

        //// set the render target for the output merger stage (the output of the pipeline)
        //DirectInitializer::_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

        //// Clear the render target by using the ClearRenderTargetView command
        //const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        //DirectInitializer::_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

        //// transition the "frameIndex" render target from the render target state to the present state. If the debug layer is enabled, you will receive a
        //// warning if present is called on the render target when it's not in the present state
 
        //transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DirectInitializer::_renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        //DirectInitializer::_commandList->ResourceBarrier(1, &transitionBarrier);


        //hr = DirectInitializer::_commandList->Close();
        //if (FAILED(hr))
        //{
        //    std::cout << "Failed to close Command List" << std::endl;
        //    /*return;*/
        //}
    }

    void Render() 
    {
        /*HRESULT hr;

        int frameIndex = DirectInitializer::_frameIndex;*/

        UpdatePipeline(); // update the pipeline by sending commands to the commandqueue

        //// array of command list, at case there are Multi Threading
        //ID3D12CommandList* ppCommandLists[] = { DirectInitializer::_commandList }; // Prefer vector

        //// put command list in command queue
        //DirectInitializer::_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists); // Prefer size

        //// Try to execute commande queue, but wait gpu have finished to execute
        //hr = DirectInitializer::_commandQueue->Signal(DirectInitializer::_fences[frameIndex], DirectInitializer::_fencesValue[frameIndex]);
        //if (FAILED(hr))
        //{
        //    std::cout << "Failed to trying get signal gpu finishing, we have to wait" << std::endl;
        //    return;
        //}

        //// present the current backbuffer
        //hr = DirectInitializer::_swapChain->Present(0, 0);
        //if (FAILED(hr))
        //{
        //    std::cout << "Failed to render" << std::endl;
        //    return;
        //}
    }

    void Cleanup()
    {
        // wait for the gpu to finish all frames
        for (int i = 0; i < DirectInitializer::_frameBufferCount; ++i)
        {
            DirectInitializer::_frameIndex = i;
            WaitForPreviousFrame();
        }

        // get swapchain out of full screen before exiting
        BOOL fs = false;
        if (DirectInitializer::_swapChain->GetFullscreenState(&fs, NULL)) DirectInitializer::_swapChain->SetFullscreenState(false, NULL);

        SAFE_RELEASE(DirectInitializer::_device);
        SAFE_RELEASE(DirectInitializer::_swapChain);
        SAFE_RELEASE(DirectInitializer::_commandQueue);
        SAFE_RELEASE(DirectInitializer::_rtvDescriptorHeap);
        SAFE_RELEASE(DirectInitializer::_commandList);

        for (int i = 0; i < DirectInitializer::_frameBufferCount; ++i)
        {
            SAFE_RELEASE(DirectInitializer::_renderTargets[i].resource);
            SAFE_RELEASE(DirectInitializer::_commandAllocators[i].allocator);
            SAFE_RELEASE(DirectInitializer::_fences[i].fence);
        };
    }
}
