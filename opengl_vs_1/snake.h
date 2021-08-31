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
    enum class SnakeState
    {
        HOME,
        THINKING,
        MOVING,
        MOVED,
        COLLIDED_AREA,
        COLLIDED_BODY,
        EATING_FRUIT,
        GROWING_1,
        GROWED
    };
private:


    Opcode m_eDirectionSnake;
    SnakeState m_eState;
    SnakeState m_eStatePrev;
    float m_fElapsed;
    float m_fUpdateAITime;
    int m_iSideGrowth;
    AMLEngine::IPosition m_oIPosSnake;
    AMLEngine::IPosition m_oIPosSnakePrev;
    AMLEngine::IPosition m_oIPosSnakeForw;
    AMLEngine::IPosition m_vIPosSnake[1000];

    AMLEngine::Colors::FColor3 m_oColorSnake;
    int m_iLenSnake;
    AMLEngine::IRectangle m_moveArea;

public:

    SnakeState GetState() const
    {
        return m_eStatePrev;
    }
    void SetState(SnakeState state)
    {
        m_eStatePrev = m_eState;
        m_eState     = state;
    }
    Snake(AMLEngine::IPosition startPosition,AMLEngine::IRectangle moveArea,AMLEngine::Colors::FColor3 snakeColor,size_t startLen) : m_eDirectionSnake(Opcode::NONE)
    {
        m_eState     = SnakeState::HOME;
        m_eStatePrev = m_eState;
        m_iSideGrowth = 0;
        m_fElapsed = 0.0f;
        m_fUpdateAITime = 0.15f;
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

    void UpdateMoveArea(AMLEngine::IRectangle moveArea)
    {
        m_moveArea = moveArea;
    }
  
    void Update()
    {
        SnakeState previousState = m_eState;
        SnakeState nextState = m_eState;

        do
        {
            nextState = Brain::Update(*this, previousState);

            previousState = m_eState;

            m_eState = nextState;
        } while
            (nextState != SnakeState::THINKING);

        m_eStatePrev = previousState;
    }

    void Update(float deltaTime)
    {
        m_fElapsed += deltaTime;
       
        if (m_fElapsed >= m_fUpdateAITime)
        {
            Update();

            m_fElapsed = 0.0f;
        }
    }
   
    void Input(const Command& directions)
    {
        Brain::Input(*this, directions);
    }

    AMLEngine::ISize GetHeadSize() const
    {
        int side = m_moveArea.GetHeight() / 64;
        return { side ,side };
    }

    AMLEngine::IRectangle GetBoundingBox()
    {

        AMLEngine::IPosition topLeft     = m_oIPosSnake;
        AMLEngine::IPosition bottomRight = m_oIPosSnake;

        for (int i = 0;i < m_iLenSnake;i++)
        {
            if (m_vIPosSnake[i].X < topLeft.X)
            {
                topLeft.X = m_vIPosSnake[i].X;
            }
            if (m_vIPosSnake[i].Y < topLeft.Y)
            {
                topLeft.Y = m_vIPosSnake[i].Y;
            }
            if (m_vIPosSnake[i].X > bottomRight.X)
            {
                bottomRight.X = m_vIPosSnake[i].X;
            }
            if (m_vIPosSnake[i].Y > bottomRight.Y)
            {
                bottomRight.Y = m_vIPosSnake[i].Y;
            }
        }
        int side = m_moveArea.GetHeight() / 64;
        int halfSide = side / 2;
        topLeft     -= halfSide;
        bottomRight += halfSide;
        return { topLeft,bottomRight};
    }

    const AMLEngine::IRectangle& GetMoveArea() const
    {
        return m_moveArea;
    }
    const AMLEngine::IPosition&  GetHeadPosition() const
    {
        return m_oIPosSnake;
    }

    void Render(float deltaTime)
    {
        int side = m_moveArea.GetHeight() / 64;

        for (int i = 0;i < m_iLenSnake;i++)
        {
            AMLEngine::Core::Draw::Square(m_vIPosSnake[i].X, m_vIPosSnake[i].Y, side, m_oColorSnake);
        }
    }
};  


