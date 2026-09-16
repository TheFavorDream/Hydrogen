/*

    Light base Class of the Renderer

*/

#pragma once
#include "../Common.h"
#include "../VecMath/Math.h"
#include "../VecMath/Vector/Vectors.h" 
#include "../Render/Vulkan/UniformBuffer.h"
#include <string>

namespace Hydrogen
{

    /*
        Light Base Class
    */
    class Light
    {
    public:

        HYD  Light() = default;
        HYD ~Light() = default;

        HYD Light(
            std::string pName,
            VecF3       pPosition,
            VecF3       pColor,
            VecF3       pAmbient   = VecF3(0.05f),
            VecF3       pDiffuse   = VecF3(1.0f),
            VecF3       pSpecular  = VecF3(1.0f)  
        ) noexcept;

        HYD Light(const Light&)            = default;
        HYD Light(Light&&)                 = default;

        HYD Light& operator=(const Light&) = default;
        HYD Light& operator=(Light&&)      = default;


        HYD inline std::string Name()       const noexcept {return m_Name;}
        HYD inline VecF3&       Position()  noexcept {return m_Position;}
        HYD inline VecF3&       Color()     noexcept {return m_Color;}
        HYD inline VecF3&       Ambient()   noexcept {return m_Ambient;}
        HYD inline VecF3&       Diffuse()   noexcept {return m_Diffuse;}
        HYD inline VecF3&       Specular()  noexcept {return m_Specular;}

    private:
        std::string  m_Name      = "Unknown";
        VecF3        m_Position  = VecF3(0.0f); //In the World Space
        VecF3        m_Color     = VecF3(1.0f);
        VecF3        m_Ambient   = VecF3(0.05f);
        VecF3        m_Diffuse   = VecF3(1.0f); 
        VecF3        m_Specular  = VecF3(1.0f);  

    private:
        friend class LightCollection;
    };


    /*
        Light Collection:
    */

    class LightCollection
    {
    public:

        HYD  LightCollection() noexcept;
        HYD ~LightCollection() noexcept;

        HYD LightCollection(const LightCollection& pOther) noexcept;
        HYD LightCollection(LightCollection&&      pOther) noexcept;

        //Asign 
        HYD LightCollection& operator=(const LightCollection& pOther) noexcept;
        HYD LightCollection& operator=(LightCollection&&      pOther) noexcept;
        
        /*
            Iterators
        */

        HYD inline std::vector<Light>::iterator begin() {return m_Lights.begin();}
        HYD inline std::vector<Light>::iterator end()   {return m_Lights.end();}


        /*
            Purpose: Create the Collection(Allocate Descriptor Sets and uniform buffers)
        */

        HYD uint32 CreateCollection(
            uint32 pSet,
            uint32 pDescSetID
        ) noexcept;

        /*
            Purpose: Add a new Light to the Collection
        */

        HYD uint32 CreateLight(
            Light&& pNewLight
        ) noexcept;

        /*
            Purpose: Bind the Entire Light Collection to the specified Descriptor Set
        */
        HYD void BindCollection(
            const Internal::Vulkan::PipelineLayout& pPipelineLayout 
        ) noexcept;   

        /*
            Access An Light inside the Collection
        */
        HYD const Light& AccessLight(
            uint32 pIndex
        ) const noexcept; 

        HYD Light& EditLight(
            uint32 pIndex
        ) noexcept; 

        /*
            Purpose: Uploads Light data in uniform buffer:
        */

        HYD void UploadData() noexcept;

        HYD inline uint32 GetLightCount() const {return m_LightCount;}

    private:
        std::vector<Light>               m_Lights;
        UniformRef                       m_UniBuffer;
        uint32                           m_LightCount      = 0;    
        HYD_ID_SPACE                     m_DescriptorPool  = 0;
        HYD_ID_SPACE                     m_DescriptorSetID = 0;
        bool                             m_IsDirty         = false;
    private:
        friend class Scene;
    };

};