#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>
#include <chrono>


namespace AMLEngine
{
    struct IPosition
    {
        int X;
        int Y;
    };

    struct Colors
    {
        typedef struct
        {
            float r;
            float g;
            float b;

        } Color;

        const Color RED;

        Colors() : RED{ 1,0,0 }
        {

        }

    };

    class Core
    {
    public:




    public:

        static Colors& COLORS()
        {
            static Colors colors;
            return colors;
        }

        typedef GLFWwindow* GLFWwindowPtr;
        typedef void (*InputHandlerPtr)(GLFWwindowPtr);
        typedef void (*RenderLoopPtr)(Core&);
        typedef void (*ErrorHandlerPtr)(int, const char*);
        typedef std::chrono::nanoseconds GLWFDuration;
        typedef std::chrono::milliseconds GLWFMilliseconds;
        typedef long long GLWFTypeLL;

    private:
        const unsigned int SCR_WIDTH = 800;
        const unsigned int SCR_HEIGHT = 600;

        GLFWwindowPtr window;
        RenderLoopPtr renderLoopPtr;
        InputHandlerPtr inputHandlerPtr;
        std::exception initException;
        bool isInit;
        GLWFDuration elapsed;
        // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
        {
            // make sure the viewport matches the new window dimensions; note that width and 
            // height will be significantly larger than specified on retina displays.

            setupOrtho(width,height);


            glViewport(0, 0, width, height);
        }
        static void error_callback(int error, const char* description)
        {
            std::cerr << "Error: " << description << "\n";
        }

        static void setupOrtho(int width,int height)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
        }

    public:
        IPosition getWindowCenter()
        {
            IPosition pos;

            glfwGetWindowSize(window, &pos.X, &pos.Y);
            pos.X /= 2;
            pos.Y /= 2;

            return pos;
        }
        static struct Draw
        {
            static void Circle(int x, int y, int radius, const AMLEngine::Colors::Color& color)
            {
                float theta;

                glColor3f(color.r, color.g, color.b);
                glBegin(GL_POLYGON);

                for (int i = 0;i < 360;i++)
                {
                    theta = i * 0.01745f;
                    glVertex2f(x + radius * cos(theta), y + radius * sin(theta));
                }


                glEnd();
                glFlush();
            }
        };
        const GLFWwindowPtr getWindow() const
        {
            return  window;
        }
        const std::exception* getExceptionPtr() const
        {
            return isInit ? nullptr : &initException;
        }

        void setRenderLoop(RenderLoopPtr rLoopPtr)
        {
            assert(rLoopPtr != nullptr);
            renderLoopPtr = rLoopPtr;
        }
        void setErrorHandler(ErrorHandlerPtr eHandlerPtr)
        {
            assert(eHandlerPtr != nullptr);
            glfwSetErrorCallback(eHandlerPtr);
        }
        void setInputHandler(InputHandlerPtr iHandlerPtr)
        {
            assert(iHandlerPtr != nullptr);
            inputHandlerPtr = iHandlerPtr;
        }

        const GLWFTypeLL getDeltaTimeMS() const
        {
            GLWFTypeLL deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
            return deltaTime;
        }

        void run()
        {
            //precondition
            if (!isInit || !renderLoopPtr || !inputHandlerPtr)
            {
                return;
            }

            auto m_BeginFrame = std::chrono::high_resolution_clock::now();
            auto m_EndFrame = m_BeginFrame;
            // unsigned frame_count_per_second = 0;
            setupOrtho(SCR_WIDTH,SCR_HEIGHT);
             //main loop
            while (!glfwWindowShouldClose(window))
            {

                auto loop_elapsed = m_EndFrame - m_BeginFrame;
                elapsed = loop_elapsed;

                // input
                // -----
                inputHandlerPtr(window);
         
                // render
                // ------
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                renderLoopPtr(*this);
                // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
                // -------------------------------------------------------------------------------
                glfwSwapBuffers(window);
                glfwPollEvents();

                m_BeginFrame = m_EndFrame;
                m_EndFrame = std::chrono::high_resolution_clock::now();
            }
        }

        Core()
        {
            renderLoopPtr = nullptr;
            inputHandlerPtr = nullptr;
            window = nullptr;
            isInit = false;
            elapsed = std::chrono::system_clock::duration::min();
            // glfw: initialize and configure
          // ------------------------------

            glfwSetErrorCallback(error_callback);

            if (!glfwInit())
            {
                // return -1;
                initException = std::move(std::exception("Unable to glfwInit"));
                return;
            }

            //TODO: read from cfg file
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            // glfw window creation
            // --------------------
            window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLSandBox", NULL, NULL);
            if (window == NULL)
            {
                //std::cout << "Failed to create GLFW window" << std::endl;
                //glfwTerminate();
                //return -1;
                initException = std::move(std::exception("Unable to glfwCreateWindow"));
                return;
            }
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, Core::framebuffer_size_callback);


            // glad: load all OpenGL function pointers
            // ---------------------------------------
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                // std::cout << "Failed to initialize GLAD" << std::endl;
                // return -1;
                initException = std::move(std::exception("Unable to gladLoadGLLoader"));

                return;
            }

            isInit = true;
        }

        // glfw: terminate, clearing all previously allocated GLFW resources.
       // ------------------------------------------------------------------
        ~Core()
        {
            glfwTerminate();
        }
    };

}