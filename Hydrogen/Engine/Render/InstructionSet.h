/*
    This files contains the Definition of Render Instruction and Instruction Sets
*/

#pragma once


#include "Vulkan/Buffer.h"
#include "Vulkan/Pipeline.h"
#include "../Core/ResourcePool.h"
#include "../Common.h"
#include "Vulkan/VkEnumReDefs.h"
#include "../VecMath/Matrix/MatDef.h"

namespace Hydrogen
{

    /*
        Render Instruction
    */
	struct Instruction
	{
        //Vertex & Index Buffers
		VertexBufferRef           Vertices;
		IndexBufferRef            Indices;
		GraphicsPipelineRef       Pipeline;

        //Descriptors to be bound
        std::vector<std::pair<HYD_ID_SPACE, DescriptorBindInfo>>    DescriptorBinds;
        std::vector<std::pair<PipelineLayoutRef, PushConstantData>> PushConstants;

        PrimitiveTopology         Topology;


         Instruction() = default;
        ~Instruction() = default;

		Instruction(const Instruction& ) 			= default;
		Instruction& operator=(const Instruction& ) = default;

		Instruction(Instruction&& pOther)
			: Vertices(         std::move(pOther.Vertices)),
		  	  Indices(          std::move(pOther.Indices)),
		  	  Pipeline(         std::move(pOther.Pipeline)),
		  	  DescriptorBinds(  std::move(pOther.DescriptorBinds)),
              PushConstants(    std::move(pOther.PushConstants)),
              Topology(         pOther.Topology)
			{}
		
	};


    /*
        Instruction Set: Instruction Sets are allocated from Instruction Set Pools.
    */

    class InstructionSet
    {
    public:

        HYD  InstructionSet() noexcept;
        HYD ~InstructionSet() noexcept;


        HYD InstructionSet(const InstructionSet& )            noexcept;
        HYD InstructionSet& operator=(const InstructionSet& ) noexcept;


        HYD InstructionSet(InstructionSet&& pOther)            noexcept;
        HYD InstructionSet& operator=(InstructionSet&& pOther) noexcept;

        //Operations on Set:
        HYD InstructionSet operator+(InstructionSet&& pOther) noexcept;


        //Iterators:
        HYD inline std::vector<Instruction>::iterator begin() {return m_Instructions.begin();}
        HYD inline std::vector<Instruction>::iterator end()   {return m_Instructions.end();}


        /*
            Purpose: Append another set to this one
        */

        HYD void Append(
            InstructionSet&& pOther
        ) noexcept;


        /*
            Purpose: Resets the Instruction Set (Does NOT Deallocate any memory)         
        */
        HYD void Reset() noexcept;


        /*
            Purpose: Deletes the Set (Deallocates memory)
        */

        HYD void Free() noexcept;

        /*
            Purpose: Push an Instruction to the set
        */
        HYD void PushInstruction(
            const Instruction& pInstruction 
        ) noexcept; 

        HYD void PushInstruction(
            Instruction&&      pInstruction
        ) noexcept;


        /*
            Purpose: Return an unused instruction object
        */
        HYD Instruction& NewInstruction() noexcept;

    private:
        std::vector<Instruction> m_Instructions;
        uint32                   m_ReadPtr;

    };

};