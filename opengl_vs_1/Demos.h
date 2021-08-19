#pragma once
#include "AMLEngine.h"
#include <functional>
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
    float g_speed_movement = 2000.0f;
    float g_speed_snake = 1000.0f;
    float g_speed_zoom = 100.0f;
    float g_elapsed = 0.0f;
    bool  g_close = false;
    AMLEngine::IPosition g_pos = { 0,0 };
    float g_radius = 0.0f;
    bool g_scene3Start = false;
    AMLEngine::IPosition g_snakePos = { 0,0 };
    AMLEngine::Colors::Color g_snakeColor = AMLEngine::Core::COLORS().GREEN;
    int g_snakeLen = 1;
    AMLEngine::Core::FrameLimit fLimit = AMLEngine::Core::FrameLimit::FPS_60;
    DrawColor g_drawColor = DrawColor::RED;
    Direction g_direction = Direction::NONE;
    Size g_size = Size::NONE;


    //ENGINE
    std::unique_ptr<AMLEngine::Core> ame;

public:



    Demos()
    {
        ame = std::make_unique<AMLEngine::Core>(800, 600, "AMLEngineDemos");
        if (ame->getExceptionPtr() != nullptr)
        {
            std::cout << ame->getExceptionPtr()->what() << std::endl;
        }

    }

    void Play()
    {

        ame->setFrameLimit(fLimit);
        ame->setErrorHandler(&errorHandler);
        ame->setInputHandler(std::bind(&Demos::processInputScene3,this,_1));
        ame->setRenderLoop(std::bind(&Demos::renderLoopScene3,this,_1));

        ame->run();
    }

private:
    static void errorHandler(int eCode, const char* description)
    {
        std::cerr << "ERROR: " << eCode << " " << description;
    }
    void switchScene(AMLEngine::Core& ame, size_t scene)
    {

        if (scene == 1)
        {
            ame.setRenderLoop(std::bind(&Demos::renderLoopScene1, this, _1));
            ame.setInputHandler(std::bind(&Demos::processInputScene1, this, _1));
            return;
        }
        if (scene == 2)
        {
            ame.setRenderLoop(std::bind(&Demos::renderLoopScene2, this, _1));
            ame.setInputHandler(std::bind(&Demos::processInputScene2, this, _1));
            return;
        }
        if (scene == 3)
        {
            ame.setRenderLoop(std::bind(&Demos::renderLoopScene3, this, _1));
            ame.setInputHandler(std::bind(&Demos::processInputScene3, this, _1));
            return;
        }
    }

    void renderLoopScene1(AMLEngine::Core& ame)
    {


        g_elapsed += ame.getFrameTime();

        if (g_elapsed > 30)
        {
            g_elapsed = 0.0f;
            switchScene(ame, 2);
            return;
        }

        std::cout << g_elapsed << " " << ame.getFrameTime() << "\n";

        if (g_close)
        {
            ame.closeWindow();
            return;
        }


        AMLEngine::IPosition pos = ame.getWindowCenter();
        int radius = pos.Y;

        const float s_dt = ame.getFrameTime();
        float step = s_dt * g_speed_movement;

        if (step < 1)
        {
            step = 1;
        }

        switch (g_size)
        {
        case Size::NONE:
            g_radius = radius;
            break;
        case Size::MINUS:
            g_radius -= s_dt * g_speed_zoom;
            break;
        case Size::PLUS:
            g_radius += s_dt * g_speed_zoom;
            break;
        }

        switch (g_direction)
        {
        case Direction::NONE:
            g_pos = pos;
            break;
        case Direction::UP:
            g_pos.Y -= step;
            break;
        case Direction::DOWN:
            g_pos.Y += step;
            break;
        case Direction::LEFT:
            g_pos.X -= step;
            break;
        case Direction::RIGHT:
            g_pos.X += step;
            break;
        }

        switch (g_drawColor)
        {
        case DrawColor::RED:
            AMLEngine::Core::Draw::Circle(g_pos.X, g_pos.Y, g_radius, AMLEngine::Core::COLORS().RED);
            break;
        case DrawColor::GREEN:
            AMLEngine::Core::Draw::Circle(g_pos.X, g_pos.Y, g_radius, AMLEngine::Core::COLORS().GREEN);
            break;
        case DrawColor::BLUE:
            AMLEngine::Core::Draw::Circle(g_pos.X, g_pos.Y, g_radius, AMLEngine::Core::COLORS().BLUE);
            break;
        }



    }

    void renderLoopScene2(AMLEngine::Core& ame)
    {

        g_elapsed += ame.getFrameTime();

        if (g_elapsed > 30)
        {
            g_elapsed = 0.0f;
            switchScene(ame, 3);
            return;
        }

        std::cout << g_elapsed << " " << ame.getFrameTime() << "\n";

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

                switch (g_drawColor)
                {
                case DrawColor::RED:
                    AMLEngine::Core::Draw::Square(xPos, yPos, diameter, AMLEngine::Core::COLORS().RED);
                    g_drawColor = DrawColor::GREEN;
                    break;
                case DrawColor::GREEN:
                    AMLEngine::Core::Draw::Circle(xPos, yPos, radius, AMLEngine::Core::COLORS().GREEN);
                    g_drawColor = DrawColor::BLUE;
                    break;
                case DrawColor::BLUE:
                    AMLEngine::Core::Draw::Circle(xPos, yPos, radius, AMLEngine::Core::COLORS().BLUE);
                    g_drawColor = DrawColor::RED;
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
            g_close = true;
            return;
        }


        if (keyboard.r())
        {
            g_drawColor = DrawColor::RED;
            return;
        }

        if (keyboard.g())
        {
            g_drawColor = DrawColor::GREEN;
            return;
        }

        if (keyboard.b())
        {
            g_drawColor = DrawColor::BLUE;
            return;
        }


        g_direction = Direction::NONE;
        g_size = Size::NONE;

        if (keyboard.plus())
        {
            g_size = Size::PLUS;

        }
        if (keyboard.minus())
        {
            g_size = Size::MINUS;

        }
        if (keyboard.up())
        {
            g_direction = Direction::UP;
            return;
        }

        if (keyboard.down())
        {
            g_direction = Direction::DOWN;
            return;
        }

        if (keyboard.left())
        {
            g_direction = Direction::LEFT;
            return;
        }

        if (keyboard.right())
        {
            g_direction = Direction::RIGHT;
            return;
        }


    }
    void processInputScene2(const AMLEngine::Core::Keyboard& keyboard)
    {

    }


    //this is a simple snake game (constrained  movement only the snake can move up down left right inside the map,
    //no opposite directions allowed, keep directions on key press)

    AMLEngine::IPosition g_snakePositions[100];

    void renderLoopScene3(AMLEngine::Core& core)
    {
        g_elapsed += core.getFrameTime();

        if (g_elapsed > 30)
        {
            g_elapsed = 0.0f;
            switchScene(core, 1);
            return;
        }



        std::cout << g_elapsed << " " << core.getFrameTime() << "\n";

        AMLEngine::ISize size = core.getWindowSize();
        int side = size.HEIGHT / 64;
        int hside = side / 2;

        if (!g_scene3Start)
        {
            g_snakeLen = 20;
            AMLEngine::IPosition pos = core.getWindowCenter();
            g_snakePos = pos;

            for (int i = 0;i < g_snakeLen;i++)
            {
                g_snakePositions[i] = pos;
            }

            AMLEngine::Core::Draw::Square(pos.X, pos.Y, side, g_snakeColor);
            g_scene3Start = true;
            return;
        }

        if (g_elapsed > 2.0)
        {
            bool canMove = true;

            AMLEngine::IPosition prevPosition = g_snakePos;

            if (g_direction == Direction::RIGHT)
            {
                g_snakePos.X += side;
            }
            else
                if (g_direction == Direction::LEFT)
                {
                    g_snakePos.X -= side;
                }
                else
                    if (g_direction == Direction::UP)
                    {
                        g_snakePos.Y -= side;
                    }
                    else
                        if (g_direction == Direction::DOWN)
                        {
                            g_snakePos.Y += side;
                        }

            if ((g_snakePos.X <= side || g_snakePos.X >= size.WIDTH - side) || (g_snakePos.Y <= side || g_snakePos.Y >= size.HEIGHT - side))
            {
                g_snakePos = prevPosition;
                canMove = false;
            }

            if (canMove)
            {
                for (int i = 0;i < g_snakeLen - 1;i++)
                {
                    g_snakePositions[i] = g_snakePositions[i + 1];
                }

                g_snakePositions[g_snakeLen - 1] = g_snakePos;
            }

        }



        int numBlocks = size.HEIGHT / side + 6;

        for (int i = 0; i < numBlocks; i++)
        {
            AMLEngine::Core::Draw::Square(hside, hside + i * side, side, AMLEngine::Core::COLORS().RED);
            AMLEngine::Core::Draw::Square(size.WIDTH - hside, hside + i * side, side, AMLEngine::Core::COLORS().RED);

        }

        numBlocks = size.WIDTH / side + 6;

        for (int i = 0; i < numBlocks; i++)
        {
            AMLEngine::Core::Draw::Square(hside + i * side, hside, side, AMLEngine::Core::COLORS().RED);
            AMLEngine::Core::Draw::Square(hside + i * side, size.HEIGHT - hside, side, AMLEngine::Core::COLORS().RED);

        }
        for (int i = 0;i < g_snakeLen;i++)
        {
            AMLEngine::Core::Draw::Square(g_snakePositions[i].X, g_snakePositions[i].Y, side, g_snakeColor);
        }


    }

    void processInputScene3(const AMLEngine::Core::Keyboard& keyboard)
    {

        if (keyboard.up() && g_direction != Direction::DOWN)
        {

            g_direction = Direction::UP;
            return;
        }

        if (keyboard.down() && g_direction != Direction::UP)
        {

            g_direction = Direction::DOWN;
            return;
        }

        if (keyboard.left() && g_direction != Direction::RIGHT)
        {

            g_direction = Direction::LEFT;
            return;
        }

        if (keyboard.right() && g_direction != Direction::LEFT)
        {

            g_direction = Direction::RIGHT;
            return;
        }

    }
};