#include "AMLEngine.h"

void processInputScene1(const AMLEngine::Core::Keyboard& keyboard);
void processInputScene2(const AMLEngine::Core::Keyboard& keyboard);
void renderLoopScene1(AMLEngine::Core& amlngine);
void renderLoopScene2(AMLEngine::Core& amlngine);

void errorHandler(int, const char*);

float g_speed_movement = 2000.0f;
float g_speed_zoom     = 100.0f;
float g_elapsed        = 0.0f;
bool  g_close = false;
AMLEngine::IPosition g_pos = { 0,0 };
float g_radius = 0.0f;
AMLEngine::Core::FrameLimit fLimit = AMLEngine::Core::FrameLimit::FPS_60;
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

DrawColor g_drawColor = DrawColor::RED;
Direction g_direction = Direction::NONE;
Size g_size = Size::NONE;

int main()
{
        
        AMLEngine::Core ame(800,600,"OpenGLTestBed");

        if (ame.getExceptionPtr() != nullptr)
        {
            std::cout << ame.getExceptionPtr()->what() << std::endl;
            return -1;
        }


        ame.setFrameLimit(fLimit);
        ame.setErrorHandler(&errorHandler);

        ame.setInputHandler(&processInputScene2);
        ame.setRenderLoop(&renderLoopScene2);

        ame.run();
   
        
    return 0;
}
void errorHandler(int eCode,const char* description)
{
    std::cerr <<"ERROR: "<< eCode << " " << description;
}

void switchScene(AMLEngine::Core& ame,size_t scene)
{

    if (scene == 1)
    {
        ame.setRenderLoop(&renderLoopScene1);
        ame.setInputHandler(&processInputScene1);
        return;
    }
    if (scene == 2)
    {
        ame.setRenderLoop(&renderLoopScene2);
        ame.setInputHandler(&processInputScene2);
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
   
    std::cout << g_elapsed << " " <<  ame.getFrameTime() <<  "\n";

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
            switchScene(ame, 1);
            return;
        }
        
    std::cout << g_elapsed << " " << ame.getFrameTime() <<  "\n";

    AMLEngine::ISize size = ame.getWindowSize();
    int radius   = size.HEIGHT / 16;
    int diameter = radius * 2;

    int extraSpaceY = size.HEIGHT - diameter * 8;
    int yPos = size.HEIGHT - (extraSpaceY/2) + radius;

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
