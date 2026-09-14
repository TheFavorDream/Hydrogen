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

        HYD void        GenerateGrid() noexcept;
        HYD Instruction Render()       noexcept;
        HYD void        DestroyGrid()  noexcept;

        HYD void SetFog(
            float pQuad,
            float pLinear,
            float pConstant 
        ) noexcept;

    private:

        struct GridUniformData
        {
            alignas(16) glm::mat4 View;
            alignas(16) glm::mat4 Projection;
            alignas(16) VecF3     CameraPos;
            alignas(16) VecF3     FogCoefficient; 
        };

        GridUniformData m_UniData;

    private:
        float      m_Spacing = 1.0f;
        UniformRef m_Uniform;
    };

};