#pragma once
#include "AMLEngine.h"
#include <functional>
#include <algorithm>
//here my game scene
#include "snake_scene.h"

using namespace std::placeholders;
class Demos
{

private:

   
    enum class DrawColor
    {
        RED,
        GREEN,
        BLUE
    };

    enum class Direction
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum class Size
    {
        NONE,
        PLUS,
        MINUS
    };

    //DEMOS VARIABLES
    float m_fSpeed_Circle                   = 2000.0f;
    float m_fSpeed_Zoom                     = 100.0f;
    float m_fElapsed_0                      = 0.0f;
    bool  m_bClose                          = false;
    AMLEngine::IPosition m_oIPosCircle      = { 0,0 };
    float m_fRadius                         = 0.0f;
    AMLEngine::Core::FrameLimit m_fLimit    = AMLEngine::Core::FrameLimit::FPS_60;
    DrawColor m_eDrawColor                  = DrawColor::RED;
    Direction m_eDirectionCircle            = Direction::NONE;
    Size m_eSize                            = Size::NONE;
    AMLEngine::FPosition3 m_vFPosControlPoints[20];
    float m_fElapsed_1 = -1;
    float m_fPointSize = 5.0f;

    int   m_iPoints_subcurve = 4;
    int   m_iConnect_prevs = 1;
    int   m_iNumPoints = 20;
    float m_fInterp_steps = 100;
    bool  m_bDrawPoints = true;
    bool  m_bRecalc = false;


    AMLEngine::Core::Timer m_oTimer;

    //ENGINE
    AMLEngine::Core& ame;


    //Snake Scene
    AMLEngine::Core::Scene<SnakeScene> m_oSnakeScene;

    //todo: move as engine funfuction or leave here as scenemanager (to build upon this demos class)
    template <template <class> class S,class T>
    void setRenderLoop(S<T>& scene)
    {
        
        ame.setRenderLoop([this,&scene](AMLEngine::Core& core) 
        {
            this->switchScene(core,1);
            scene.render(core);
        });

    }
    template <template <class> class S, class T>
    void setInputHandler(S<T>& scene)
    {
        ame.setInputHandler(std::bind(&S<T>::input, &scene, _1));
    }


public:



    Demos(AMLEngine::Core& core) : ame(core)
    {

        if(ame.getExceptionPtr())
        {
            std::cout << ame.getExceptionPtr()->what() << std::endl;
        }

    }

    void Play()
    {

        ame.setFrameLimit(m_fLimit);
        ame.setErrorHandler(&errorHandler);

        m_oSnakeScene.create(ame);

        setInputHandler(m_oSnakeScene);
        setRenderLoop(m_oSnakeScene);

        m_oTimer.GetTime();

       
        ame.run();
    }

private:
    static void errorHandler(int eCode, const char* description)
    {
        std::cerr << "ERROR: " << eCode << " " << description;
    }
    void switchScene(AMLEngine::Core& ame, size_t scene)
    {
        const auto fElapsed = m_oTimer.GetTime();

        m_fElapsed_0 += fElapsed;
        m_fElapsed_1 += fElapsed;

        std::cout << m_fElapsed_0 << " ";

        if (m_fElapsed_0 >= 30)
        {
            m_fElapsed_0 = 0.0f;
            m_fElapsed_1 = 0.0f;

            if (scene == 1)
            {
                ame.setRenderLoop(std::bind(&Demos::renderLoopScene1, this, _1));
                ame.setInputHandler(std::bind(&Demos::processInputScene1, this, _1));
            } else
            if (scene == 2)
            {
                ame.setRenderLoop(std::bind(&Demos::renderLoopScene2, this, _1));
                ame.setInputHandler(std::bind(&Demos::processInputScene2, this, _1));
            } else
            if (scene == 3)
            {
                m_oSnakeScene.create(ame);
                setRenderLoop(m_oSnakeScene);
                setInputHandler(m_oSnakeScene);
            }
        }

    }

    
  


    static bool compare(AMLEngine::FPosition3 a, AMLEngine::FPosition3 b) {
        if (a.X < b.X)
            return 1;
        else
            return 0;
    }

    void renderLoopScene1(AMLEngine::Core& ame)
    {

        switchScene(ame, 2);

        std::cout << ame.getFrameTime() << "\n";

        if (m_bClose)
        {
            ame.closeWindow();
            return;
        }


        AMLEngine::IPosition pos = ame.getWindowCenter();
        int radius = pos.Y;

        const float s_dt = ame.getFrameTime();
        float step = s_dt * m_fSpeed_Circle;

        if (step < 1)
        {
            step = 1;
        }

        switch (m_eSize)
        {
        case Size::NONE:
            m_fRadius = radius;
            break;
        case Size::MINUS:
            m_fRadius -= s_dt * m_fSpeed_Zoom;
            break;
        case Size::PLUS:
            m_fRadius += s_dt * m_fSpeed_Zoom;
            break;
        }

        switch (m_eDirectionCircle)
        {
        case Direction::NONE:
            m_oIPosCircle = pos;
            break;
        case Direction::UP:
            m_oIPosCircle.Y -= step;
            break;
        case Direction::DOWN:
            m_oIPosCircle.Y += step;
            break;
        case Direction::LEFT:
            m_oIPosCircle.X -= step;
            break;
        case Direction::RIGHT:
            m_oIPosCircle.X += step;
            break;
        }

        switch (m_eDrawColor)
        {
        case DrawColor::RED:
            AMLEngine::Core::Draw::Circle(m_oIPosCircle.X, m_oIPosCircle.Y, m_fRadius, AMLEngine::Core::COLORS().RED);
            break;
        case DrawColor::GREEN:
            AMLEngine::Core::Draw::Circle(m_oIPosCircle.X, m_oIPosCircle.Y, m_fRadius, AMLEngine::Core::COLORS().GREEN);
            break;
        case DrawColor::BLUE:
            AMLEngine::Core::Draw::Circle(m_oIPosCircle.X, m_oIPosCircle.Y, m_fRadius, AMLEngine::Core::COLORS().BLUE);
            break;
        }

        if (m_fElapsed_1 >=3 || m_fElapsed_1 < 0)
        {
            m_bRecalc  = true;
            m_fElapsed_1 = 0;
        }
       
        if (m_bRecalc)
        {
            for (int i = 0;i < m_iNumPoints;i++)
            {

                m_vFPosControlPoints[i].X = 0 + (std::rand() % (300 - 0 + 1));
                m_vFPosControlPoints[i].Y = 0 + (std::rand() % (200 - 0 + 1));
                m_vFPosControlPoints[i].Z = 0;
            }
            std::sort(m_vFPosControlPoints, m_vFPosControlPoints + m_iNumPoints, compare);
            m_bRecalc = false;
        }
        
        AMLEngine::Core::Draw::Curve(m_vFPosControlPoints,m_iNumPoints, m_iPoints_subcurve,m_fInterp_steps,AMLEngine::Core::COLORS().BLUE, m_iConnect_prevs,true,m_fPointSize);
    }

    void renderLoopScene2(AMLEngine::Core& ame)
    {

     
        switchScene(ame, 3);
    
        std::cout << ame.getFrameTime() << "\n";

        AMLEngine::ISize size = ame.getWindowSize();
        int radius = size.HEIGHT / 16;
        int diameter = radius * 2;

        int extraSpaceY = size.HEIGHT - diameter * 8;
        int yPos = size.HEIGHT - (extraSpaceY / 2) + radius;

        for (int i = 0; i < 8; i++)
        {

            yPos -= diameter;
            int xPos = (size.WIDTH / 2) - (5 * diameter) + radius;

            for (int ii = 0; ii < 8; ii++)
            {
                xPos += diameter;

                switch (m_eDrawColor)
                {
                case DrawColor::RED:
                    AMLEngine::Core::Draw::Square(xPos, yPos, diameter, AMLEngine::Core::COLORS().RED);
                    m_eDrawColor = DrawColor::GREEN;
                    break;
                case DrawColor::GREEN:
                    AMLEngine::Core::Draw::Circle(xPos, yPos, radius, AMLEngine::Core::COLORS().GREEN);
                    m_eDrawColor = DrawColor::BLUE;
                    break;
                case DrawColor::BLUE:
                    AMLEngine::Core::Draw::Circle(xPos, yPos, radius, AMLEngine::Core::COLORS().BLUE);
                    m_eDrawColor = DrawColor::RED;
                    break;
                }


            }
        }
    }


    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInputScene1(const AMLEngine::Core::Keyboard& keyboard)
    {

        if (keyboard.escape())
        {
            m_bClose = true;
            return;
        }


        if (keyboard.r())
        {
            m_eDrawColor = DrawColor::RED;
            return;
        }

        if (keyboard.g())
        {
            m_eDrawColor = DrawColor::GREEN;
            return;
        }

        if (keyboard.b())
        {
            m_eDrawColor = DrawColor::BLUE;
            return;
        }


        m_eDirectionCircle = Direction::NONE;
        m_eSize = Size::NONE;

        if (keyboard.plus())
        {
            m_eSize = Size::PLUS;

        }
        if (keyboard.minus())
        {
            m_eSize = Size::MINUS;

        }
        if (keyboard.up())
        {
            m_eDirectionCircle = Direction::UP;
            return;
        }

        if (keyboard.down())
        {
            m_eDirectionCircle = Direction::DOWN;
            return;
        }

        if (keyboard.left())
        {
            m_eDirectionCircle = Direction::LEFT;
            return;
        }

        if (keyboard.right())
        {
            m_eDirectionCircle = Direction::RIGHT;
            return;
        }


    }
    void processInputScene2(const AMLEngine::Core::Keyboard& keyboard)
    {

    }


   
   
};