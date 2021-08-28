#pragma once
#include "AMLEngine.h"
#include "snake.h"
#include "snake_brain.h"

class SnakeScene
{

private:

    const std::string apple_file = "apple_1.bmp";
    const std::string transparency_file = "transparency.txt";

    std::unique_ptr<Snake<BrainSnake1>> m_oSnake;
    AMLEngine::Core::Timer m_oTimer;
    AMLEngine::Core::Draw::ImagePtr fruit;

    AMLEngine::IPosition fruitCenter;
   
protected:

    void OnCreate(AMLEngine::Core& core)
    {
        AMLEngine::IPosition startPosition  = core.getWindowCenter();
        AMLEngine::ISize moveArea           = core.getWindowSize();
        AMLEngine::Colors::FColor3 snakeColor = AMLEngine::Colors::GREEN;
        size_t startLen                     = 20;

        m_oSnake = std::make_unique<Snake<BrainSnake1>>(startPosition, moveArea, snakeColor, startLen);

        AMLEngine::Resources::TransparencyInfoLoader trans("transparency.txt");
       
        fruit    = AMLEngine::Core::Draw::Image::CreateImage(apple_file);
        //set white color to be full transparent (set thr color.alpha channel to 1.0)
        fruit->GetTextureHandle().SetColorKey(trans.Get(apple_file)->color);

        fruitCenter = AMLEngine::IPosition
         { 
             static_cast<int>(fruit->GetTextureHandle().GetWidth() / 2) ,
             static_cast<int>(fruit->GetTextureHandle().GetHeight() / 2)
         };

        m_oTimer.GetTime();

        core.setClearColor(AMLEngine::Colors::FColor3{138/255.0f,102/255.0f,56/255.0f});

    }

    void OnRender(AMLEngine::Core& core)
    {

        std::cout << core.getFrameTime() << "\n";

        AMLEngine::ISize moveArea = core.getWindowSize();

        //update the bounding area.. only cause i know is not fixed.. usually should be
        m_oSnake->UpdateMoveArea(moveArea);

        //update snake behaviours
        m_oSnake->Update(m_oTimer.GetTime());
       
        //render images
        core.enableTexturing();
        core.enableShadeModeSmooth();
        core.enableAlphaTest(AMLEngine::Core::ALPHA_FUN::GREATER, 0.0f);
        fruit->BlitImage(moveArea.WIDTH / 2 - fruitCenter.X, moveArea.HEIGHT/2 - fruitCenter.Y);

        core.enableShadeModeFlat();
        core.disableAlphaTest();
        //render the snake
        m_oSnake->Render();

        //RENDER OBSTACLES
        int side      = moveArea.HEIGHT / 64;
        int hside     = side / 2;
        int numBlocks = moveArea.HEIGHT / side + 6;

        for (int i = 0; i < numBlocks; i++)
        {
            AMLEngine::Core::Draw::Square(hside, hside + i * side, side, AMLEngine::Colors::RED);
            AMLEngine::Core::Draw::Square(moveArea.WIDTH - hside, hside + i * side, side, AMLEngine::Colors::RED);

        }

        numBlocks = moveArea.WIDTH / side + 6;

        for (int i = 0; i < numBlocks; i++)
        {
            AMLEngine::Core::Draw::Square(hside + i * side, hside, side, AMLEngine::Colors::RED);
            AMLEngine::Core::Draw::Square(hside + i * side, moveArea.HEIGHT - hside, side, AMLEngine::Colors::RED);

        }


    }

    //Fixded step update
    void OnUpdate()
    {

    }

    //acquire keyboard input and map it to a know snake command
    void OnInput(const AMLEngine::Core::Keyboard& oKeyboard)
    {

        Snake<BrainSnake1>::Command directions = { Snake<BrainSnake1>::Opcode::NONE ,Snake<BrainSnake1>::Opcode::NONE ,Snake<BrainSnake1>::Opcode::NONE ,Snake<BrainSnake1>::Opcode::NONE };

        //MAP the keyboard input... can be everything .. to snake command

        if (oKeyboard.up())
        {
            directions[0] = Snake<BrainSnake1>::Opcode::UP;
        }
        if (oKeyboard.down())
        {
            directions[1] = Snake<BrainSnake1>::Opcode::DOWN;
        }
        if (oKeyboard.left())
        {
            directions[2] = Snake<BrainSnake1>::Opcode::LEFT;
        }
        if (oKeyboard.right())
        {
            directions[3] = Snake<BrainSnake1>::Opcode::RIGHT;
        }

        //acquire and validate inputs
        m_oSnake->Input(directions);

    }
    void OnDestroy(AMLEngine::Core& core)
    {

    }
};
