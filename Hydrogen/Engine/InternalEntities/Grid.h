#pragma once

#include "../Common.h"
#include "../Render/Vulkan/Pipeline.h"
#include "../Camera/Camera.h"
#include "../Geometry/Primitive.h"

namespace Hydrogen
{



    class Grid : public Primitive
    {
    public:

        HYD  Grid() noexcept;
        HYD ~Grid() noexcept;

        Grid(const Grid& )            = delete;
        Grid& operator=(const Grid& ) = delete;

        HYD Grid(Grid&& pOther)            noexcept;
        HYD Grid& operator=(Grid&& pOther) noexcept;

        HYD void GenerateGrid() noexcept;

    private:
        float m_Spacing = 1.0f;
    };

};