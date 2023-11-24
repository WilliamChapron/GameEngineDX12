#pragma once


namespace DirectManager {
    void InitializeDirectX();
    void Update();
    void UpdatePipeline();
    void Render();
    //void Cleanup();
    void WaitForPreviousFrame();
}