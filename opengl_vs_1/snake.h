#pragma once
#include "AMLEngine.h"


//this is a simple snake game (constrained  movement only the snake can move up down left right inside the map,
//no opposite directions allowed, keep directions on key press)
class SnakeScene
{

private:

    enum class Direction
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    float m_fElapsed;
    float m_fSpeed_Snake;
    bool m_bScene3Start = false;
    AMLEngine::IPosition m_oIPosSnake;
    AMLEngine::IPosition m_vIPosSnake[100];
    AMLEngine::Colors::Color m_oColorSnake;
    int m_iLenSnake;
    Direction m_eDirectionSnake;

    AMLEngine::Core::Timer m_oTimer;

protected:

    void OnCreate(AMLEngine::Core& core)
    {
        m_fElapsed = 0.0f;
        m_fSpeed_Snake = 0.2f;
        m_oIPosSnake = { 0,0 };
        m_oColorSnake = AMLEngine::Core::COLORS().GREEN;
        m_eDirectionSnake = Direction::NONE;

        m_iLenSnake = 20;
        AMLEngine::IPosition pos = core.getWindowCenter();
        m_oIPosSnake = pos;

        for (int i = 0;i < m_iLenSnake;i++)
        {
            m_vIPosSnake[i] = pos;
        }

        m_oTimer.GetTime();
    }

    void OnRender(AMLEngine::Core& core)
    {
        m_fElapsed += m_oTimer.GetTime();

        std::cout << core.getFrameTime() << "\n";

        AMLEngine::ISize size = core.getWindowSize();
        int side = size.HEIGHT / 64;
        int hside = side / 2;

        //TODO: move inside the snake behavior
        if (m_fElapsed >= m_fSpeed_Snake)
        {
            bool canMove = true;

            AMLEngine::IPosition prevPosition = m_oIPosSnake;

            if (m_eDirectionSnake == Direction::RIGHT)
            {
                m_oIPosSnake.X += side;
            }
            else
                if (m_eDirectionSnake == Direction::LEFT)
                {
                    m_oIPosSnake.X -= side;
                }
                else
                    if (m_eDirectionSnake == Direction::UP)
                    {
                        m_oIPosSnake.Y -= side;
                    }
                    else
                        if (m_eDirectionSnake == Direction::DOWN)
                        {
                            m_oIPosSnake.Y += side;
                        }

            //check collision on walls
            if ((m_oIPosSnake.X <= side || m_oIPosSnake.X >= size.WIDTH - side) || (m_oIPosSnake.Y <= side || m_oIPosSnake.Y >= size.HEIGHT - side))
            {
                m_oIPosSnake = prevPosition;
                canMove = false;
            }

            if (canMove)
            {

                if (m_eDirectionSnake == Direction::NONE)
                {
                    for (int i = 0;i < m_iLenSnake - 1;i++)
                    {
                        m_vIPosSnake[i] = m_vIPosSnake[i + 1];
                    }

                    m_vIPosSnake[m_iLenSnake - 1] = m_oIPosSnake;

                }
                else
                {
                    for (int i = m_iLenSnake - 1;i > 1; i--)
                    {
                        if (m_oIPosSnake == m_vIPosSnake[i])
                        {
                            m_oIPosSnake = prevPosition;
                            canMove = false;
                            break;
                        }
                    }

                    if (canMove)
                    {
                        for (int i = 0;i < m_iLenSnake - 1;i++)
                        {
                            m_vIPosSnake[i] = m_vIPosSnake[i + 1];
                        }

                        m_vIPosSnake[m_iLenSnake - 1] = m_oIPosSnake;
                    }
                }
            }


            m_fElapsed = 0.0f;
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
        for (int i = 0;i < m_iLenSnake;i++)
        {
            AMLEngine::Core::Draw::Square(m_vIPosSnake[i].X, m_vIPosSnake[i].Y, side, m_oColorSnake);
        }


    }

    //Fixded step update
    void OnUpdate()
    {

    }
    void OnInput(const AMLEngine::Core::Keyboard& oKeyboard)
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
    void OnDestroy(AMLEngine::Core& core)
    {

    }
};