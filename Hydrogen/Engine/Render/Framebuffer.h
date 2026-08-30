#pragma once
#include "../Common.h"
#include "Vulkan/FrameBuffer.h"


namespace Hydrogen
{

    class Framebuffer final
    {
    public:

        HYD  Framebuffer() noexcept;
        HYD ~Framebuffer() noexcept;

        HYD Framebuffer(const Framebuffer& pOther) noexcept;
        HYD Framebuffer(Framebuffer&& pOther)      noexcept;

        HYD Framebuffer operator=(const Framebuffer& pOther) noexcept;
        HYD Framebuffer operator=(Framebuffer&& pOther)      noexcept;

        


    private:
    
        
    };

};