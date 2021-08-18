#include "AMLEngine.h"

void processInputScene1(const AMLEngine::Core::Keyboard& keyboard);
void renderLoopScene1(AMLEngine::Core& amlngine);
void errorHandler(int, const char*);
//float g_MS  = 0.0f;
//int   g_FPS = 0;
float g_speed = 2000;
bool  g_close = false;
AMLEngine::IPosition g_pos = { 0,0 };

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


DrawColor g_drawColor = DrawColor::RED;
Direction g_direction = Direction::NONE;

int main()
{
        
       AMLEngine::Core ame;

        if (ame.getExceptionPtr() != nullptr)
        {
            std::cout << ame.getExceptionPtr()->what() << std::endl;
            return -1;
        }


        ame.setFrameLimit(AMLEngine::Core::FrameLimit::FPS_60);
        ame.setErrorHandler(&errorHandler);

        ame.setInputHandler(&processInputScene1);
        ame.setRenderLoop(&renderLoopScene1);

        ame.run();
   
        
    return 0;
}
void errorHandler(int eCode,const char* description)
{
    std::cerr <<"ERROR: "<< eCode << " " << description;
}



void renderLoopScene1(AMLEngine::Core& ame)
{
 
    std::cout << ame.getDeltaTimeMS() <<  " " << ame.getFrameTime() << " " << ame.getDeltaTimeS() << "\n";

   

    if (g_close)
    {
        ame.closeWindow();
        return;
    }


    AMLEngine::IPosition pos = ame.getWindowCenter();
    int radius = pos.Y;
   
    const float s_dt = ame.getFrameTime();
    float step = s_dt * g_speed;
    
    if (step < 1)
    {
        step = 1;
    }

   switch (g_direction)
    {
      case Direction::NONE :
          g_pos = pos;
          break;
      case Direction::UP:
          g_pos.Y -= step;
          break;
      case Direction::DOWN :
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
        AMLEngine::Core::Draw::Circle(g_pos.X, g_pos.Y, radius, AMLEngine::Core::COLORS().RED);
        break;
     case DrawColor::GREEN:
        AMLEngine::Core::Draw::Circle(g_pos.X, g_pos.Y, radius, AMLEngine::Core::COLORS().GREEN);
        break;
     case DrawColor::BLUE:
        AMLEngine::Core::Draw::Circle(g_pos.X, g_pos.Y, radius, AMLEngine::Core::COLORS().BLUE);
        break;
    }
    

  
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInputScene1(const AMLEngine::Core::Keyboard& keyboard)
{
 
    if(keyboard.escape())
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

