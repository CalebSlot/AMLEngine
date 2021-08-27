#pragma once
#include "AMLEngine.h"
//this is a simple snake game (constrained  movement only the snake can move up down left right inside the map,
//no opposite directions allowed, keep directions on key press)



class BrainSnake1;

template<class Brain = BrainSnake1>
class Snake
{
    friend Brain;
public:
    enum class Opcode
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    struct Command
    {
        Opcode opcodes[4];
        Opcode operator[](int index) const
        {
            return opcodes[index];
        }
        Opcode& operator[] (int index)
        {
            return opcodes[index];
        }
    };
private:
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

    Opcode m_eDirectionSnake;
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

            //here i execute the FSM
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
   
    void Input(const Command& directions)
    {
        Brain::Input(*this, directions);
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


