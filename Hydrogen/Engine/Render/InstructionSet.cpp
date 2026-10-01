#include "InstructionSet.h"


namespace Hydrogen
{

    InstructionSet::InstructionSet()  noexcept
        : m_ReadPtr(0)
    {}

    InstructionSet::~InstructionSet() noexcept
    {
        m_Instructions.clear();
        m_ReadPtr        = 0;
    }


    InstructionSet::InstructionSet(
        InstructionSet&& pOther
    ) noexcept
        : m_Instructions(std::move(pOther.m_Instructions)), m_ReadPtr(pOther.m_ReadPtr)
    {
        pOther.m_ReadPtr = 0;
    }

    InstructionSet& InstructionSet::operator=(
        InstructionSet&& pOther
    ) noexcept
    {
        m_Instructions = std::move(pOther.m_Instructions);
        m_ReadPtr        = pOther.m_ReadPtr;
        pOther.m_ReadPtr = 0;

        return *this;
    }

    //Operations on Set:
    InstructionSet InstructionSet::operator+(
        InstructionSet&& pOther
    ) noexcept
    {
        return InstructionSet();
    }


    /*
        Purpose: Append another set to this one
    */

    void InstructionSet::Append(
        InstructionSet&& pOther
    ) noexcept
    {
        m_Instructions.insert(
            m_Instructions.end(),
               pOther.m_Instructions.begin(),
                pOther.m_Instructions.end()
        );


        pOther.m_Instructions.clear();
        pOther.m_ReadPtr = 0;
    }


    /*
        Purpose: Resets the Instruction Set (Does NOT Deallocate any memory)         
    */
    void InstructionSet::Reset() noexcept
    {
        m_ReadPtr = 0;
        m_Instructions.clear();
    }


    /*
        Purpose: Deletes the Set (Deallocates memory)
    */

    void InstructionSet::Free() noexcept
    {
        m_Instructions.clear();
        m_ReadPtr = 0;
    }

    /*
        Purpose: Push an Instruction to the set
    */
    void InstructionSet::PushInstruction(
        const Instruction& pInstruction 
    ) noexcept
    {
        if (m_Instructions.size() && m_ReadPtr < m_Instructions.size())
        {
            m_Instructions.at(m_ReadPtr++) = pInstruction;
            return;
        }
        
        m_Instructions.emplace_back(pInstruction);
        m_ReadPtr++;
    }

    void InstructionSet::PushInstruction(
        Instruction&&      pInstruction
    ) noexcept
    {
        if (m_Instructions.size() && m_ReadPtr < m_Instructions.size())
        {
            m_Instructions.at(m_ReadPtr++) = std::move(pInstruction);
            return;
        }
        
        m_Instructions.emplace_back(pInstruction);
        m_ReadPtr++;
    }

    /*
        Purpose: Return an unused instruction object
    */
    Instruction& InstructionSet::NewInstruction() noexcept
    {
        if (m_Instructions.size() && m_ReadPtr < m_Instructions.size())
            return m_Instructions.at(m_ReadPtr++);
        
        m_Instructions.emplace_back();
        return m_Instructions.at(m_ReadPtr++);
    }

};