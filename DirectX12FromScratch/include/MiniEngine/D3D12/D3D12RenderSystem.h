#pragma once

# include <dxgi1_4.h>
# include "Window.h"
# include "MiniEngine/RenderSystem.h"
# include "MiniEngine/D3D12/D3D12CommandQueue.h"
# include "MiniEngine/D3D12/D3D12RootSignature.h"
# include "MiniEngine/D3D12/D3D12HLSLShader.h"
# include "MiniEngine/D3D12/D3D12RenderWindow.h"
# include "MiniEngine/D3D12/D3D12RenderableModel.h"
# include "MiniEngine/D3D12/D3D12ConstantBuffer.h"

namespace MiniEngine
{
    class D3D12Device;

    class D3D12RenderSystem : public RenderSystem
    {
    public:
        D3D12RenderSystem();
        ~D3D12RenderSystem();

        bool                    init();
        bool                    initDevice();
        bool                    initCommandQueue();
        bool                    initFence();
        bool                    initRootSignature();

        IDXGIFactory4           *getFactory();
        D3D12Device             *getDevice();
        D3D12CommandQueue       *getCommandQueue();
        D3D12Fence              *getFence();
        D3D12RootSignature      *getRootSignature();

        D3D12ConstantBuffer     *createConstantBuffer(unsigned int size, unsigned int nb);

        D3D12RenderWindow       *createRenderWindow(Window *window);
        D3D12HLSLShader         *createHLSLShader(std::string const &filename, std::string const &entry);
		D3D12RenderableModel    *loadModel(GraphicPipeline &pipeline, std::string const &filename);

    protected:
        IDXGIFactory4       *_factory;
        D3D12Device         *_device;
        D3D12CommandQueue   *_commandQueue;
        D3D12Fence          *_fence;
        D3D12RootSignature  *_rootSignature;
    };
}