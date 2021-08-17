#include "AMLEngine.h"

void processInput(AMLEngine::Core::GLFWwindowPtr window);
void renderLoop(AMLEngine::Core& amlngine);
void errorHandler(int, const char*);
//float g_MS  = 0.0f;
//int   g_FPS = 0;

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

    const AMLEngine::Colors::Color& circleColor = AMLEngine::Core::COLORS().RED;

    AMLEngine::Core::Draw::Circle(pos.X, pos.Y, radius, circleColor);
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(AMLEngine::Core::GLFWwindowPtr window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

