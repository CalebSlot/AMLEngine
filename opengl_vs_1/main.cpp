#include "AMLEngine.h"

void processInput(AMLEngine::Core::GLFWwindowPtr window);
void renderLoop(AMLEngine::Core& amlngine);
void errorHandler(int, const char*);
//float g_MS  = 0.0f;
//int   g_FPS = 0;
float g_speed = 5;
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

        ame.setErrorHandler(&errorHandler);

        ame.setInputHandler(&processInput);
        ame.setRenderLoop(&renderLoop);

        ame.run();
   
        
    return 0;
}
void errorHandler(int eCode,const char* description)
{
    std::cerr <<"ERROR: "<< eCode << " " << description;
}



void renderLoop(AMLEngine::Core& ame)
{
    std::cout << ame.getDeltaTimeMS() << "\n";

    AMLEngine::IPosition pos = ame.getWindowCenter();
    int radius = pos.Y;
   



    switch (g_direction)
    {
      case Direction::NONE :
          g_pos = pos;
          break;
      case Direction::UP:
          g_pos.Y -= ame.getDeltaTimeMS() * g_speed;
          break;
      case Direction::DOWN :
          g_pos.Y += ame.getDeltaTimeMS() * g_speed;
          break;
      case Direction::LEFT:
          g_pos.X -= ame.getDeltaTimeMS() * g_speed;
          break;
      case Direction::RIGHT:
          g_pos.X += ame.getDeltaTimeMS() * g_speed;
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
void processInput(AMLEngine::Core::GLFWwindowPtr window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        g_drawColor = DrawColor::RED;
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        g_drawColor = DrawColor::GREEN;
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_B)== GLFW_PRESS)
    {
        g_drawColor = DrawColor::BLUE;
        return;
    }

  
    g_direction = Direction::NONE;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        g_direction = Direction::UP;
        //return;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        g_direction = Direction::DOWN;
        //return;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        g_direction = Direction::LEFT;
        //return;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        g_direction = Direction::RIGHT;
        //return;
    }

  
}

