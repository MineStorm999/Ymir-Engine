#include "Renderer.hpp"
#include "Editor/ToolBarWindowManager.h"

bool Renderer::Initialize(nri::GraphicsAPI graphicsAPI)
{
    WindowManager::Init();

    m_vGeomStreamer = new VirtualGeometryStreamer();
    m_vGeomStreamer->Init();

    return true;
}

void Renderer::PrepareFrame(uint32_t frameIndex)
{

}

void Renderer::RenderFrame(uint32_t frameIndex)
{

}
