#pragma once


namespace DirectManager {
    void InitializeDirectX();
    void UpdateLogic();
    void RenderScene();
    void Cleanup();
    void WaitForFrame();
}