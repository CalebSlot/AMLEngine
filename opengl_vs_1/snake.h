#pragma once
#include "AMLEngine.h"


//this is a simple snake game (constrained  movement only the snake can move up down left right inside the map,
//no opposite directions allowed, keep directions on key press)



class BrainSnake1;

template<class Brain = BrainSnake1>
class Snake
{
    friend Brain;

private:
   
    enum class Direction
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    enum class SnakeState
    {
        HOME,
        THINKING,
        MOVING,
        MOVED,
        COLLIDED_AREA,
        COLLIDED_BODY,
        COLLIDED_ITEM
    };

    Direction m_eDirectionSnake;
    SnakeState m_eState;
    float m_fElapsed;
    float m_fSpeed_Snake;
    AMLEngine::IPosition m_oIPosSnake;
    AMLEngine::IPosition m_oIPosSnakePrev;
    AMLEngine::IPosition m_vIPosSnake[100];
    AMLEngine::Colors::Color m_oColorSnake;
    int m_iLenSnake;
    AMLEngine::ISize m_moveArea;
  
   
    
    
public:

    Snake(AMLEngine::IPosition startPosition,AMLEngine::ISize moveArea,AMLEngine::Colors::Color snakeColor,size_t startLen) : m_eDirectionSnake(Direction::NONE)
    {
        m_eState = SnakeState::HOME;
        m_fElapsed = 0.0f;
        m_fSpeed_Snake = 0.2f;
        m_oIPosSnake  = startPosition;
        m_oIPosSnakePrev = startPosition;
        m_oColorSnake = snakeColor;
        m_iLenSnake = startLen;
        m_moveArea = moveArea;
       

        for (int i = 0;i < m_iLenSnake;i++)
        {
            m_vIPosSnake[i] = m_oIPosSnake;
        }
    }

    void UpdateMoveArea(AMLEngine::ISize moveArea)
    {
        m_moveArea = moveArea;
    }
  
 

    void Update(float deltaTime)
    {
        m_fElapsed += deltaTime;
       
        if (m_fElapsed >= m_fSpeed_Snake)
        {
            SnakeState previousState = m_eState;
            SnakeState nextState     = m_eState;

            do 
            {
                nextState = Brain::Update(*this, previousState);

                previousState = m_eState;
                m_eState = nextState;
            } 
            while 
                (nextState != SnakeState::THINKING);

            m_fElapsed = 0.0f;
        }
    }
   
    void Input(const AMLEngine::Core::Keyboard& oKeyboard)
    {
        if (oKeyboard.up() && m_eDirectionSnake != Direction::DOWN)
        {

            m_eDirectionSnake = Direction::UP;
            return;
        }

        if (oKeyboard.down() && m_eDirectionSnake != Direction::UP)
        {

            m_eDirectionSnake = Direction::DOWN;
            return;
        }

        if (oKeyboard.left() && m_eDirectionSnake != Direction::RIGHT)
        {

            m_eDirectionSnake = Direction::LEFT;
            return;
        }

        if (oKeyboard.right() && m_eDirectionSnake != Direction::LEFT)
        {

            m_eDirectionSnake = Direction::RIGHT;
            return;
        }
    }

    void Render()
    {
        int side = m_moveArea.HEIGHT / 64;

        for (int i = 0;i < m_iLenSnake;i++)
        {
            AMLEngine::Core::Draw::Square(m_vIPosSnake[i].X, m_vIPosSnake[i].Y, side, m_oColorSnake);
        }
    }
};  

class SnakeScene
{

private:

   

    std::unique_ptr<Snake<BrainSnake1>> m_oSnake;
    AMLEngine::Core::Timer m_oTimer;

protected:

    void OnCreate(AMLEngine::Core& core)
    {
        AMLEngine::IPosition startPosition = core.getWindowCenter();
        AMLEngine::ISize moveArea = core.getWindowSize();
        AMLEngine::Colors::Color snakeColor = AMLEngine::Core::COLORS().GREEN;
        size_t startLen = 20;
        m_oSnake = std::make_unique<Snake<BrainSnake1>>(startPosition,moveArea, snakeColor, startLen);
        m_oTimer.GetTime();
    }

    void OnRender(AMLEngine::Core& core)
    {

        std::cout << core.getFrameTime() << "\n";

        AMLEngine::ISize moveArea = core.getWindowSize();
    
        m_oSnake->UpdateMoveArea(moveArea);
        m_oSnake->Update(m_oTimer.GetTime());
        m_oSnake->Render();

        //RENDER OBSTACLES
        int side = moveArea.HEIGHT / 64;
        int hside = side / 2;
        int numBlocks = moveArea.HEIGHT / side + 6;

        for (int i = 0; i < numBlocks; i++)
        {
            AMLEngine::Core::Draw::Square(hside, hside + i * side, side, AMLEngine::Core::COLORS().RED);
            AMLEngine::Core::Draw::Square(moveArea.WIDTH - hside, hside + i * side, side, AMLEngine::Core::COLORS().RED);

        }

        numBlocks = moveArea.WIDTH / side + 6;

        for (int i = 0; i < numBlocks; i++)
        {
            AMLEngine::Core::Draw::Square(hside + i * side, hside, side, AMLEngine::Core::COLORS().RED);
            AMLEngine::Core::Draw::Square(hside + i * side, moveArea.HEIGHT - hside, side, AMLEngine::Core::COLORS().RED);

        }
       

    }

    //Fixded step update
    void OnUpdate()
    {

    }
    void OnInput(const AMLEngine::Core::Keyboard& oKeyboard)
    {
        m_oSnake->Input(oKeyboard);

    }
    void OnDestroy(AMLEngine::Core& core)
    {

    }
};

class BrainSnake1
{

public:

    static Snake<BrainSnake1>::SnakeState Update(Snake<BrainSnake1>& snake, Snake<BrainSnake1>::SnakeState previousState)
    {

        AMLEngine::ISize size                           = snake.m_moveArea;
        Snake<BrainSnake1>::Direction m_eDirectionSnake = snake.m_eDirectionSnake;
        int side = size.HEIGHT / 64;
        const Snake<BrainSnake1>::SnakeState currentState  = snake.m_eState;

        Snake<BrainSnake1>::SnakeState nextState = Snake<BrainSnake1>::SnakeState::THINKING;


        switch (currentState)
        {
             case Snake<BrainSnake1>::SnakeState::HOME:
             {
                    //i'm leaving home.. maybe
             }
             break;
             case Snake<BrainSnake1>::SnakeState::THINKING:
             {
                    //if no directions i'm still at home
                    if (m_eDirectionSnake == Snake<BrainSnake1>::Direction::NONE)
                    {
                      return Snake<BrainSnake1>::SnakeState::HOME;
                    }

                    nextState = Snake<BrainSnake1>::SnakeState::MOVING;
             }
             break;
             case Snake<BrainSnake1>::SnakeState::MOVING:
             {
           
                 AMLEngine::IPosition newPosition = snake.m_oIPosSnake;
                 if (m_eDirectionSnake == Snake<BrainSnake1>::Direction::RIGHT)
                 {
                     newPosition.X += side;
                 }
                 else
                     if (m_eDirectionSnake == Snake<BrainSnake1>::Direction::LEFT)
                     {
                         newPosition.X -= side;
                     }
                     else
                         if (m_eDirectionSnake == Snake<BrainSnake1>::Direction::UP)
                         {
                             newPosition.Y -= side;
                         }
                         else
                             if (m_eDirectionSnake == Snake<BrainSnake1>::Direction::DOWN)
                             {
                                 newPosition.Y += side;
                             }

                 snake.m_oIPosSnakePrev = snake.m_oIPosSnake;
                 snake.m_oIPosSnake = newPosition;

                 //collision on boundig box
                 if ((newPosition.X <= side || newPosition.X >= size.WIDTH - side) || (newPosition.Y <= side || newPosition.Y >= size.HEIGHT - side))
                 {
                     return Snake<BrainSnake1>::SnakeState::COLLIDED_AREA;
                 }
                 //collision on own body
                 const size_t len = snake.m_iLenSnake;
                 const AMLEngine::IPosition* positions = &snake.m_vIPosSnake[0];
                 for (int i = len - 1;i > 1; i--)
                 {
                     if (newPosition == positions[i])
                     {
                         return Snake<BrainSnake1>::SnakeState::COLLIDED_BODY;
                     }
                 }

                 nextState = Snake<BrainSnake1>::SnakeState::MOVED;
             }
             break;
             //TODO:GAME_OVER on main scene 
             case Snake<BrainSnake1>::SnakeState::COLLIDED_AREA:
             case Snake<BrainSnake1>::SnakeState::COLLIDED_BODY:
             {
                     //reset positions
                     snake.m_oIPosSnake = snake.m_oIPosSnakePrev; 
             }
             break;
             case Snake<BrainSnake1>::SnakeState::MOVED:
             {
                 //update positions
                 const size_t len = snake.m_iLenSnake;
                 AMLEngine::IPosition* positions = &snake.m_vIPosSnake[0];

                 for (int i = 0;i < len - 1;i++)
                 {
                     positions[i] = positions[i + 1];
                 }

                 positions[len - 1] = snake.m_oIPosSnake;
             }
             break;
        }
   
        return nextState;
        
    }
};