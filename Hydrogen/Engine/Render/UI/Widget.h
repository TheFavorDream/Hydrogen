#pragma once

#include "../../Common.h"
#include "../../VecMath/Vector/Vectors.h"
#include <string>

namespace Hydrogen
{
namespace UI
{
    //Base Widget Class
    class Widget 
    {
    public:

        HYD Widget()          = default;
        HYD virtual ~Widget() = default;

        //Move & Copy
        HYD Widget(const Widget& pOther)  = default;
        HYD Widget(Widget&&      pOther)  = default; 

        //Assign Operators
        HYD Widget& operator=(const Widget& pOther) = default;
        HYD Widget& operator=(Widget&&      pOther) = default; 


        //Gets called in UI Render Loop
        HYD void virtual Render() noexcept = 0; 

    protected:
        std::string m_Name;
        VecI2       m_Size;
        VecI2       m_Position;
    
    };


    //ImGui Window
    class Canvas : public Widget
    {
    public:

        HYD  Canvas() {};
        HYD ~Canvas() {};

        HYD Canvas(const Canvas& ) = default;
        HYD Canvas(Canvas&& )      = default;

        HYD Canvas& operator=(const Canvas& ) = default;
        HYD Canvas& operator=(Canvas&& )      = default;



        HYD std::string& Name()     {return m_Name;}
        HYD VecI2&       Size()     {return m_Size;}
        HYD VecI2&       Position() {return m_Position;}


        HYD void Render() noexcept override;

    private:
        
    };

};
};