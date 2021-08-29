#pragma once
#include "AMLEngine.h"
#include "snake.h"
#include "snake_brain.h"

class SnakeScene
{

private:

    const std::string apple_file        = "apple_1.bmp";
    const std::string transparency_file = "transparency.txt";

    std::unique_ptr<Snake<BrainSnake1>> m_oSnake;
    AMLEngine::Core::Timer m_oTimer;
    AMLEngine::Core::Draw::ImagePtr fruit;

    AMLEngine::ISize fruitSize;
    AMLEngine::IPosition fruitPos;
    AMLEngine::IPosition fruitImgPos;
    AMLEngine::IRectangle m_oSnakeCollider;
    AMLEngine::IRectangle m_oFruitCollider;

    void RenderSnake()
    {
        m_oSnake->Render();
    }

    void RenderWalls()
    {

    }

    void RenderFruit(const AMLEngine::Core& core,const AMLEngine::IPosition& pos)
    {

        AMLEngine::IPosition posS = m_oSnake->GetHeadPosition();
        posS.X = posS.X - fruitPos.X;
        posS.Y = posS.Y - fruitPos.Y;
        posS.X /= 2;
        posS.Y /= 2;

        if (m_collisionFruitStay || m_collisionFruitExit)
        {
            AMLEngine::Core::Draw::Circle(fruitPos.X, fruitPos.Y,fruitSize.WIDTH/2,AMLEngine::Colors::GREEN);
            
            return;
        }
            core.enableTexturing();
            core.enableShadeModeSmooth();
            core.enableAlphaTest(AMLEngine::Core::ALPHA_FUN::GREATER, 0.0f);
            fruit->BlitImage(pos.X, pos.Y);
            core.enableShadeModeFlat();
            core.disableAlphaTest();
        
    }

protected:

    void OnCreate(AMLEngine::Core& core)
    {
        AMLEngine::IPosition startPosition    = core.getWindowCenter();
        AMLEngine::ISize moveArea             = core.getWindowSize();
        AMLEngine::Colors::FColor3 snakeColor = AMLEngine::Colors::GREEN;
        size_t startLen                       = 3;

        m_oSnake = std::make_unique<Snake<BrainSnake1>>(startPosition, moveArea, snakeColor, startLen);
      
        UpdateDynamicColliders();

        AMLEngine::Resources::TransparencyInfoLoader trans(transparency_file);
       
        fruit    = AMLEngine::Core::Draw::Image::CreateImage(apple_file);
        //set white color to be full transparent (set thr color.alpha channel to 1.0)
        
        AMLEngine::Resources::TransparencyInfoLoader::TEntry* tEntry = trans.Get(apple_file);
        if (tEntry == nullptr)
        {
            std::cout << " canno't load " << apple_file << " informations";
        }
        else
        {
            fruit->GetTextureHandle().SetColorKey(tEntry->color);
        }

        fruitSize.WIDTH  = static_cast<int>(fruit->GetTextureHandle().GetWidth());
        fruitSize.HEIGHT = static_cast<int>(fruit->GetTextureHandle().GetHeight());

        AMLEngine::Randoms::Init();

        AMLEngine::IRectangle rect;
        moveArea -= 40;
        rect.Set(startPosition, moveArea);
      
        fruitPos = GetRandomPosOutsidePos(rect, startPosition, fruitSize);
        fruitImgPos = fruitPos - fruitSize / 2;

        UpdateStaticColliders();

        m_oTimer.GetTime();

        //light brown
        core.setClearColor(AMLEngine::Colors::FColor3{138/255.0f,102/255.0f,56/255.0f});

    }

    void OnRender(AMLEngine::Core& core)
    {

        std::cout << core.getFrameTime() << "\n";

        AMLEngine::ISize moveArea = core.getWindowSize();

        //update the bounding area.. only cause i know is not fixed.. usually should be
        m_oSnake->UpdateMoveArea(moveArea);

        //update snake behaviours
        //brain1 will stop the movements of the snake if against walls or body part
        m_oSnake->Update(m_oTimer.GetTime());
        
        UpdateDynamicColliders();

       
        //Render Colliders
#ifdef DEBUG
      //  AMLEngine::Core::Draw::Square(fruitPos.X, fruitPos.Y, m_oFruitCollider.GetWidth(), AMLEngine::Colors::BLUE);
        AMLEngine::Core::Draw::Circle(fruitPos.X, fruitPos.Y, m_oFruitCollider.GetWidth() /2, AMLEngine::Colors::BLUE);
        AMLEngine::Core::Draw::Square(m_oSnake->GetHeadPosition().X, m_oSnake->GetHeadPosition().Y, m_oSnake->GetHeadSize().WIDTH + 2, AMLEngine::Colors::BLUE);
#endif
        //render images
        RenderFruit(core, fruitImgPos);

        //render the snake
        RenderSnake();


        //Render Colliders
        // 
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

    //spawn system
    AMLEngine::IPosition GetRandomPosOutsidePos(const AMLEngine::IRectangle& mainArea,const AMLEngine::IPosition& pos, const AMLEngine::ISize offset)
    {

        int startX(0),endX(0),startY(0),endY(0);

       
        AMLEngine::IRectangle subs[4] = {mainArea,mainArea,mainArea,mainArea };



        subs[0].BOTTOM_RIGHT     = {pos.X - offset.WIDTH,mainArea.BOTTOM_RIGHT.Y};
      
        subs[1].TOP_LEFT         = {pos.X + offset.WIDTH,mainArea.BOTTOM_RIGHT.Y };
       
        subs[2].TOP_LEFT         = {pos.X - offset.WIDTH, mainArea.TOP_LEFT.Y };
        subs[2].BOTTOM_RIGHT     = {pos.X + offset.WIDTH, pos.Y - offset.HEIGHT};
            
        subs[3].TOP_LEFT         = {pos.X - offset.WIDTH, pos.Y + offset.HEIGHT};
        subs[3].BOTTOM_RIGHT     = {pos.X + offset.WIDTH,mainArea.BOTTOM_RIGHT.Y};

        int randomAreaIndex       = AMLEngine::Randoms::GetRandomNum(0,3);
        AMLEngine::IRectangle* rA = &subs[randomAreaIndex];

        startX = rA->TOP_LEFT.X;
        endX   = rA->BOTTOM_RIGHT.X;
        startY = rA->TOP_LEFT.Y;
        endY   = rA->BOTTOM_RIGHT.Y;

        AMLEngine::IPosition newPos = AMLEngine::Randoms::GetRandomPosFrom(startX, endX, startY, endY);
      
        return newPos;
    }

    void UpdateStaticColliders()
    {
        m_oFruitCollider.Set(fruitPos, fruitSize);
    }
    void UpdateDynamicColliders()
    {
        m_oSnakeCollider.Set(m_oSnake->GetHeadPosition(), m_oSnake->GetHeadSize());
    }

    void OnCollisionFruitEnter()
    {
        m_oSnake->SetState(Snake<BrainSnake1>::SnakeState::EATING_FRUIT);
        m_oSnake->Update();
    }

    void OnCollisionFruitStay()
    {

    }

    void OnCollisionFruitExit()
    {
        
        AMLEngine::ISize moveArea = m_oSnake->GetMoveArea();
        AMLEngine::IPosition pos;
        pos.X = moveArea.WIDTH / 2;
        pos.Y = moveArea.HEIGHT / 2;
        moveArea -= 40;
        AMLEngine::IRectangle rect;
        rect.Set(pos, moveArea);
        fruitPos = GetRandomPosOutsidePos(rect, m_oSnake->GetHeadPosition(), m_oSnake->GetHeadSize());
        fruitImgPos = fruitPos - fruitSize / 2;

        UpdateStaticColliders();
    }


    bool m_collisionFruitEnter = false;
    bool m_collisionFruitStay  = false;
    bool m_collisionFruitExit  = false;
    //Fixded step update
    void OnUpdate(float deltaTime)
    {
        
        std::cout << " integration step : " << deltaTime << "\n";

         if (m_oSnakeCollider.Intersect(m_oFruitCollider))
         {
             if (!m_collisionFruitEnter && !m_collisionFruitStay)
             {
                 m_collisionFruitEnter = true;
                 OnCollisionFruitEnter();
             }
             else
             {
                 m_collisionFruitEnter = false;
                 m_collisionFruitStay  = true;
                 OnCollisionFruitStay();
             }
         }
         else
         {
             if (m_collisionFruitExit == true)
             {
                 m_collisionFruitExit = false;
             }
             else
             if (m_collisionFruitStay == true)
             {
                 m_collisionFruitExit = true;
                 m_collisionFruitStay = false;
                 OnCollisionFruitExit();
             }
           
         }

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
