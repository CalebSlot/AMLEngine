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
    AMLEngine::ISize halfFruit;
    AMLEngine::IPosition fruitPos;
    AMLEngine::IPosition fruitImgPos;
    AMLEngine::IRectangle m_oSnakeCollider;
    AMLEngine::IRectangle m_oFruitCollider;

    void RenderSnake(const AMLEngine::Core& core)
    {
        m_oSnake->Render(core.getIntegrationTime());
    }

    void RenderWalls()
    {

    }
    bool m_bRenderFruitEated, m_bRenderFruitSpawned;
    void RenderFruit(const AMLEngine::Core& core,const AMLEngine::IPosition& pos)
    {


        if (m_bRenderFruitEated)
        {
            AMLEngine::Core::Draw::Circle(fruitPos.X, fruitPos.Y,fruitSize.WIDTH/2,AMLEngine::Colors::GREEN);
            return;
        }

        if (m_bRenderFruitSpawned)
        {
            core.enableTexturing();
            core.enableShadeModeSmooth();
            core.enableAlphaTest(AMLEngine::Core::ALPHA_FUN::GREATER, 0.0f);
            fruit->BlitImage(pos.X, pos.Y);
            core.enableShadeModeFlat();
            core.disableAlphaTest();
            return;
        }
    }

protected:

    void OnCreate(AMLEngine::Core& core)
    {

        //LOAD RESOURCES (NOW ONLY TRANSPARENCY DATAS)
        AMLEngine::Resources::TransparencyInfoLoader trans(transparency_file);


        //SNAKE OBJECT POS / RENDER / COLLISION SETUP
        AMLEngine::IPosition startPosition    = core.getWindowCenter();
        AMLEngine::ISize moveArea             = core.getWindowSize();

        AMLEngine::IRectangle rectArea;
        rectArea.Set(startPosition, moveArea);

        AMLEngine::Colors::FColor3 snakeColor = AMLEngine::Colors::GREEN;
        size_t startLen                       = 3;

        m_oSnake = std::make_unique<Snake<BrainSnake1>>(startPosition, rectArea, snakeColor, startLen);
      
        UpdateDynamicColliders();


        //FRUIT OBJECT POS / RENDER / COLLISION SETUP
        fruit    = AMLEngine::Core::Draw::Image::CreateImage(apple_file);
        //set white color to be full transparent (set thr color.alpha channel to 1.0)
        
        AMLEngine::Resources::TransparencyInfoLoader::TEntry* tEntry = trans.Get(apple_file);
        if (tEntry == nullptr)
        {
            std::cout << " canno't load " << apple_file << " informations";
            fruitSize.WIDTH  = 20;
            fruitSize.HEIGHT = 20;
        }
        else
        {
            fruit->GetTextureHandle().SetColorKey(tEntry->color);
            fruitSize.WIDTH = static_cast<int>(fruit->GetTextureHandle().GetWidth());
            fruitSize.HEIGHT = static_cast<int>(fruit->GetTextureHandle().GetHeight());
        }
       
        halfFruit = fruitSize / 2;

        AMLEngine::IRectangle rect;
        moveArea -= halfFruit;
        rect.Set(startPosition, moveArea);
      
        fruitPos              = AMLEngine::Randoms::SimpleRandom::GetRandomPosInRectExtrudePosByOffset(rect, startPosition, fruitSize);
        fruitImgPos           = fruitPos - halfFruit;
        m_bRenderFruitEated   = false;
        m_bRenderFruitSpawned = true;

        UpdateStaticColliders();

        //TICK
        m_oTimer.GetTime();

        //BACKGROUNDlight brown
        core.setClearColor(AMLEngine::Colors::FColor3{138/255.0f,102/255.0f,56/255.0f});

    }

    void OnRender(AMLEngine::Core& core)
    {

        std::cout << "render FrameTime" << core.getFrameTime() << " IntegrationStep" << core.getIntegrationTime() <<"\n";


       
        //Render Colliders
#ifdef DEBUG

        AMLEngine::Core::Draw::Rectangle(m_oSnake->GetBoundingBox().TOP_LEFT.X -5, m_oSnake->GetBoundingBox().TOP_LEFT.Y -5, m_oSnake->GetBoundingBox().BOTTOM_RIGHT.X + 5, m_oSnake->GetBoundingBox().BOTTOM_RIGHT.Y +5, AMLEngine::Colors::RED);

        AMLEngine::IPosition pF = m_oFruitCollider.GetCenter();
        AMLEngine::IPosition pS = m_oSnakeCollider.GetCenter();
      //  AMLEngine::Core::Draw::Square(fruitPos.X, fruitPos.Y, m_oFruitCollider.GetWidth(), AMLEngine::Colors::BLUE);
        AMLEngine::Core::Draw::Circle(pF.X, pF.Y, m_oFruitCollider.GetWidth() /2, AMLEngine::Colors::BLUE);
        AMLEngine::Core::Draw::Square(pS.X, pS.Y, m_oSnakeCollider.GetWidth() + 5, AMLEngine::Colors::BLUE);
    
#endif
        //render images
        RenderFruit(core, fruitImgPos);

        //render the snake
        RenderSnake(core);

        AMLEngine::ISize moveArea = core.getWindowSize();
        int side = moveArea.HEIGHT / 64;
        
        //RENDER OBSTACLES

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
       // m_oSnake->Update();

        m_bRenderFruitSpawned = false;
        m_bRenderFruitEated = true;
    }

    void OnCollisionFruitStay()
    {
        m_bRenderFruitSpawned = false;
        m_bRenderFruitEated   = true;
    }

    void OnCollisionFruitExit()
    {
        
        AMLEngine::ISize     size     = m_oSnake->GetBoundingBox().GetSize();

        AMLEngine::IRectangle movArea = m_oSnake->GetMoveArea();
        movArea.TOP_LEFT.X     += fruitSize.WIDTH;
        movArea.TOP_LEFT.Y     += fruitSize.HEIGHT;
        movArea.BOTTOM_RIGHT.X -= fruitSize.WIDTH;
        movArea.BOTTOM_RIGHT.Y -= fruitSize.HEIGHT;

        
        do
        {
            fruitPos = AMLEngine::Randoms::SimpleRandom::GetRandomPosInRectExtrudePosByOffset(movArea, m_oSnake->GetHeadPosition(), size);

        } while (fruitPos.X < movArea.TOP_LEFT.X || fruitPos.Y < movArea.TOP_LEFT.Y || fruitPos.X > movArea.BOTTOM_RIGHT.X || fruitPos.Y > movArea.BOTTOM_RIGHT.Y);

        fruitImgPos = fruitPos - halfFruit;

        UpdateStaticColliders();

        m_bRenderFruitSpawned = true;
        m_bRenderFruitEated   = false;
    }


    bool m_collisionFruitEnter = false;
    bool m_collisionFruitStay  = false;
    //Fixded step update
    void OnUpdate(AMLEngine::Core& core)
    {

        float sceneTime = m_oTimer.GetTime();
        std::cout << "update  SceneTime " << sceneTime << "\n";

        AMLEngine::ISize moveArea = core.getWindowSize();
        int side = moveArea.HEIGHT / 64;
        int wallSide = side;

        moveArea -= wallSide;
        AMLEngine::IRectangle moveRect;
        moveRect.Set(core.getWindowCenter(), moveArea);
        //update the bounding area.. only cause i know is not fixed.. usually should be
        m_oSnake->UpdateMoveArea(moveRect);

        //update snake behaviours
        //brain1 will stop the movements of the snake if against walls or body part
        m_oSnake->Update(sceneTime);

        UpdateDynamicColliders();


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
        

         if (m_collisionFruitStay == true || m_collisionFruitEnter == true)
         {
                 m_collisionFruitEnter = false;
                 m_collisionFruitStay  = false;
                 OnCollisionFruitExit();
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
