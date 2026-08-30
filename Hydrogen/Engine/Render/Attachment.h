/*
    Attachments are special textures that can be attached to framebuffers as rendering targets 
*/


#pragma once

#include "../Common.h"
#include "Vulkan/Image.h"

namespace Hydrogen
{

    class Attachment final
    {
    public:
        
        HYD  Attachment() noexcept;
        HYD ~Attachment() noexcept;

        HYD Attachment(const Attachment& pOther) noexcept;
        HYD Attachment(Attachment&& pOther)      noexcept;

        HYD Attachment& operator=(const Attachment& pOther) noexcept;
        HYD Attachment& operator=(Attachment&& pOther)      noexcept;


        

    private:
    };

};