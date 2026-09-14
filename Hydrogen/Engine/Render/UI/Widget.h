#pragma once

#include "../../Common.h"
#include "../../VecMath/Vector/Vectors.h"
#include <string>

namespace Hydrogen
{
namespace UI
{


    enum CanvasFlags
    {
        CANVAS_NORESIZE   = 1 << 0,
        CANVAS_NOCOLLAPSE = 1 << 1,
    };




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


        HYD std::string& Label()    {return m_Label;}
        HYD VecI2&       Size()     {return m_Size;}
        HYD VecI2&       Position() {return m_Position;}

    protected:
        std::string m_Label;
        VecI2       m_Size;
        VecI2       m_Position;
    
    };


    //ImGui Window
    class Canvas : public Widget
    {
    public:

        HYD  Canvas() = default;
        HYD ~Canvas() = default;
        
        HYD  Canvas(
            const std::string& pName, 
            VecI2              pSize,
            VecI2              pPosition = VecI2(INT32_MAX) 
        ) noexcept;



        HYD Canvas(const Canvas& ) = default;
        HYD Canvas(Canvas&& )      = default;

        HYD Canvas& operator=(const Canvas& ) = default;
        HYD Canvas& operator=(Canvas&& )      = default;

        HYD void Render() noexcept override;

        HYD void PushWidget(
            Ptr<Widget> pWidget
        ) noexcept;

    private:
        std::vector<Ptr<Widget>> m_Widgets;
    };


    class Button : public Widget
    {
    public:

        HYD  Button() = default;
        HYD ~Button() = default;
         
        HYD Button(
            const std::string& pLabel,
            VecI2              pPosition,
            VecI2              pSize    = VecI2(INT32_MAX) //Auto adjust
        ) noexcept;


        HYD Button(const Button& ) = default;
        HYD Button(Button&& )      = default;

        HYD Button& operator=(const Button& ) = default;
        HYD Button& operator=(Button&& )      = default;

        /*
            Purpose: Render the button:
        */
        HYD void  Render() noexcept override;

    private:   
        bool m_IsPressed = false;
    };

};
};