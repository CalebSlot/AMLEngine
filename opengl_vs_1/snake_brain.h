#pragma once
#include "snake.h"


//a brain for a snake
class BrainSnake1
{

public:

    //input logic here
    static void Input(Snake<BrainSnake1>& snake, const Snake<BrainSnake1>::Command& directions)
    {
        if (directions[0] == Snake<BrainSnake1>::Opcode::UP && snake.m_eDirectionSnake != Snake<BrainSnake1>::Opcode::DOWN)
        {

            snake.m_eDirectionSnake = Snake<BrainSnake1>::Opcode::UP;
            return;
        }

        if (directions[1] == Snake<BrainSnake1>::Opcode::DOWN && snake.m_eDirectionSnake != Snake<BrainSnake1>::Opcode::UP)
        {

            snake.m_eDirectionSnake = Snake<BrainSnake1>::Opcode::DOWN;
            return;
        }

        if (directions[2] == Snake<BrainSnake1>::Opcode::LEFT && snake.m_eDirectionSnake != Snake<BrainSnake1>::Opcode::RIGHT)
        {

            snake.m_eDirectionSnake = Snake<BrainSnake1>::Opcode::LEFT;
            return;
        }

        if (directions[3] == Snake<BrainSnake1>::Opcode::RIGHT && snake.m_eDirectionSnake != Snake<BrainSnake1>::Opcode::LEFT)
        {

            snake.m_eDirectionSnake = Snake<BrainSnake1>::Opcode::RIGHT;
            return;
        }
    }

    //FSM here
    static void Update(Snake<BrainSnake1>& snake, float deltaTime)
    {


        AMLEngine::IRectangle rect = snake.m_moveArea;
        AMLEngine::ISize      size = rect.GetSize();
        Snake<BrainSnake1>::Opcode m_eDirectionSnake = snake.m_eDirectionSnake;
        int side = size.HEIGHT / 64;
        

      
      
        snake.m_fElapsed += deltaTime;
        Snake<BrainSnake1>::SnakeState previousState = snake.m_eState;
        if (snake.m_fElapsed >= snake.m_fUpdateAITime)
        {
          
            Snake<BrainSnake1>::SnakeState currentState = snake.m_eState;
            Snake<BrainSnake1>::SnakeState nextState    = Snake<BrainSnake1>::SnakeState::THINKING;


            for (int fastStep = 0;fastStep < snake.m_iSideGrowth + 1;fastStep++)
            {
                do
                {
                    nextState = Snake<BrainSnake1>::SnakeState::THINKING;

                    switch (currentState)
                    {
                    case Snake<BrainSnake1>::SnakeState::HOME:
                    {
              
                    }
                    break;
                    case Snake<BrainSnake1>::SnakeState::THINKING:
                    {
                        //if no directions i'm still at home
                        if (m_eDirectionSnake == Snake<BrainSnake1>::Opcode::NONE)
                        {
                            nextState = Snake<BrainSnake1>::SnakeState::HOME;
                        }
                        else
                        {
                            nextState = Snake<BrainSnake1>::SnakeState::MOVING;
                        }
                    }
                    break;
                    case Snake<BrainSnake1>::SnakeState::MOVING:
                    {

                        AMLEngine::IPosition newPosition  = snake.m_oIPosSnake;
                        AMLEngine::IPosition nextPosition = snake.m_oIPosSnake;


                        if (m_eDirectionSnake == Snake<BrainSnake1>::Opcode::RIGHT)
                        {
                            newPosition.X += side;
                            nextPosition.X += 2 * side;
                        }
                        else
                            if (m_eDirectionSnake == Snake<BrainSnake1>::Opcode::LEFT)
                            {
                                newPosition.X -= side;
                                nextPosition.X -= 2 * side;
                            }
                            else
                                if (m_eDirectionSnake == Snake<BrainSnake1>::Opcode::UP)
                                {
                                    newPosition.Y -= side;
                                    nextPosition.Y -= 2 * side;
                                }
                                else
                                    if (m_eDirectionSnake == Snake<BrainSnake1>::Opcode::DOWN)
                                    {
                                        newPosition.Y += side;
                                        nextPosition.Y += 2 * side;
                                    }


                        snake.m_oIPosSnakePrev = snake.m_oIPosSnake;
                        snake.m_oIPosSnake     = newPosition;
                        snake.m_oIPosSnakeForw = nextPosition;

                        //point collision on boundig box (considering the bounding box at 0)
                        if ((newPosition.X <= rect.TOP_LEFT.X || newPosition.X >= rect.BOTTOM_RIGHT.X) || (newPosition.Y <= rect.TOP_LEFT.Y || newPosition.Y >= rect.BOTTOM_RIGHT.Y))
                        {
                            nextState = Snake<BrainSnake1>::SnakeState::COLLIDED_AREA;
                        }
                        else
                        {

                            nextState = Snake<BrainSnake1>::SnakeState::MOVED;
                            //collision on own body
                            const size_t len = snake.m_iLenSnake;
                            const AMLEngine::IPosition* positions = &snake.m_vIPosSnake[0];
                            for (int i = len - 1;i > 1; i--)
                            {
                                if (newPosition == positions[i])
                                {
                                    nextState = Snake<BrainSnake1>::SnakeState::COLLIDED_BODY;
                                    break;
                                }
                            }
                        }
                    }
                    break;
                    //TODO:GAME_OVER on main scene 
                    case Snake<BrainSnake1>::SnakeState::COLLIDED_AREA:
                    case Snake<BrainSnake1>::SnakeState::COLLIDED_BODY:
                    {
                        //reset positions
                        snake.m_oIPosSnakeForw = snake.m_oIPosSnake;
                        snake.m_oIPosSnake = snake.m_oIPosSnakePrev;

                    }
                    break;
                    case Snake<BrainSnake1>::SnakeState::EATING_FRUIT:
                    {
                        nextState = Snake<BrainSnake1>::SnakeState::GROWING_1;
                    }
                    break;
                    case Snake<BrainSnake1>::SnakeState::GROWING_1:
                    {
                        snake.m_iLenSnake += 1;
                        snake.m_oIPosSnakePrev = snake.m_oIPosSnake;
                        snake.m_oIPosSnake = snake.m_oIPosSnakeForw;
                        nextState = Snake<BrainSnake1>::SnakeState::GROWED;
                    }
                    break;
                    case Snake<BrainSnake1>::SnakeState::GROWED:
                    {
                        snake.m_fUpdateAITime -= 0.001 * snake.m_iLenSnake;

                        if (snake.m_fUpdateAITime < 0)
                        {
                            snake.m_fUpdateAITime = 0;
                            snake.m_iSideGrowth++;
                        }
                        snake.m_vIPosSnake[snake.m_iLenSnake - 1] = snake.m_oIPosSnake;
                      
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

                    previousState = snake.m_eState;

                    snake.m_eState = nextState;
                    currentState = snake.m_eState;
                } while
                    (nextState != Snake<BrainSnake1>::SnakeState::THINKING);

                snake.m_eStatePrev = previousState;
            }
            snake.m_fElapsed = 0.0f;
        }

    }

};