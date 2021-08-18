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
        const Color GREEN;
        const Color BLUE;

        Colors() : RED{ 1,0,0 }, GREEN{ 0,1,0 },BLUE {0,0,1}
        {

        }

    };



    class Core
    {


    public:


        enum class FrameLimit
        {
            NONE,
            FPS_30,
            FPS_60
        };

        static Colors& COLORS()
        {
            static Colors colors;
            return colors;
        }

        typedef GLFWwindow* GLFWwindowPtr;
        class Keyboard;
        typedef void (*KeyboardInputHandlerPtr)(const Keyboard&);
        typedef void (*RenderLoopPtr)(Core&);
        typedef void (*ErrorHandlerPtr)(int, const char*);
        typedef std::chrono::nanoseconds DurationNano;
        typedef long long DurationLong;
        typedef float DurationFloat;

        class Keyboard
        {
            friend class Core;
            GLFWwindowPtr window;

            void setWindow(GLFWwindowPtr w)
            {
                window = w;
            }

        public:
           

            bool escape() const
            {
                return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
            }

            bool r() const
            {
                return glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
            }

            bool g() const
            {
                return glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;
            }

            bool b() const
            {
                return glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS;
            }

            bool up() const
            {
                return glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
            }

            bool down() const
            {
                return glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
            }

            bool left() const
            {
                return glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
            }

            bool right() const
            {
                return glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
            }
        };

      
    private:
        FrameLimit m_eRenderLimit = FrameLimit::NONE;
        const unsigned int SCR_WIDTH = 800;
        const unsigned int SCR_HEIGHT = 600;
        const char * c_title = "OpenGLTestBed";
        const double FPS_60 = 1.0 / 60.0;
        const double FPS_30 = 1.0 / 30.0;
        bool isInit = false;

        GLFWwindowPtr window;
        RenderLoopPtr renderLoopPtr;
        KeyboardInputHandlerPtr inputHandlerPtr;
        std::exception initException;
        DurationNano  m_durationMainLoop;
        DurationFloat m_durationFrameLoop;
        Keyboard keyboard;


        bool Core_init()
        {
           
            // glfw: initialize and configure
          // ------------------------------

            glfwSetErrorCallback(error_callback);

            if (!glfwInit())
            {
                // return -1;
                initException = std::move(std::exception("Unable to glfwInit"));
                return false;
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
            window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, c_title, NULL, NULL);
            if (window == NULL)
            {
                //std::cout << "Failed to create GLFW window" << std::endl;
                //glfwTerminate();
                //return -1;
                initException = std::move(std::exception("Unable to glfwCreateWindow"));
                return false;
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

                return false;
            }

            keyboard.setWindow(window);

            return true;
        }


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

       

       
        void setFrameLimit(FrameLimit eRenderLimit)
        {
            m_eRenderLimit = eRenderLimit;
        }

        void closeWindow()
        {
            glfwSetWindowShouldClose(window, true);
        }

        IPosition getWindowCenter()
        {
            IPosition pos;

            glfwGetWindowSize(window, &pos.X, &pos.Y);
            pos.X /= 2;
            pos.Y /= 2;

            return pos;
        }
        struct Draw
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
        const Keyboard& getKeyboard() const
        {
            return *(&keyboard);
        }
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
        void setInputHandler(KeyboardInputHandlerPtr iHandlerPtr)
        {
            assert(iHandlerPtr != nullptr);
            inputHandlerPtr = iHandlerPtr;
        }

        const DurationLong getDeltaTimeMS() const
        {
            DurationLong deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(m_durationMainLoop).count();
            return deltaTime;
        }
        const DurationFloat getDeltaTimeS() const
        {
            return std::chrono::duration<float>(m_durationMainLoop).count();
        }
        const DurationFloat getFrameTime() const
        {
            return m_durationFrameLoop;
        }
        void run()
        {
            //precondition
            if (!isInit || !renderLoopPtr || !inputHandlerPtr)
            {
                return;
            }

            //TODO: add fixed pipe or shader
            setupOrtho(SCR_WIDTH,SCR_HEIGHT);
             //main loop


            //TODO:: add different paths on different configurations (different whiless.)
            //DOING
            //FREE PFS
            if (m_eRenderLimit == FrameLimit::NONE)
            {
                auto m_BeginFrame = std::chrono::high_resolution_clock::now();
                auto m_EndFrame = m_BeginFrame;

                while (!glfwWindowShouldClose(window))
                {

                    auto loop_elapsed = m_EndFrame - m_BeginFrame;
                    m_durationMainLoop  = loop_elapsed;
                    m_durationFrameLoop = std::chrono::duration<float>(m_durationMainLoop).count();
                    
                    // input
                    // -----
                    inputHandlerPtr(keyboard);

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
            //FIXED PFS
            else
            {
                double renderLimit = FPS_60;

                switch(m_eRenderLimit)
                { 
                case(FrameLimit::FPS_30) :
                    renderLimit = FPS_30;
                    break;
                case(FrameLimit::FPS_60) :
                    renderLimit = FPS_60;
                    break;
                }
            
                auto m_BeginTime     = std::chrono::high_resolution_clock::now();
                auto m_EndTime       = m_BeginTime;
                auto m_LastFrameTime = m_BeginTime;

                while (!glfwWindowShouldClose(window))
                {

                    auto loop_elapsed = m_EndTime - m_BeginTime;
                    m_durationMainLoop = loop_elapsed;

                    // input
                    // -----
                    inputHandlerPtr(keyboard);

                    // render
                    // ------
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);

                    std::chrono::duration<float> renderTime = m_EndTime - m_LastFrameTime;
                    m_durationFrameLoop = renderTime.count();

                    if (m_durationFrameLoop >= renderLimit)
                    {
                        renderLoopPtr(*this);
                        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
                        // -------------------------------------------------------------------------------
                        glfwSwapBuffers(window);
                        m_LastFrameTime = std::chrono::high_resolution_clock::now();
                    }

                 
                    glfwPollEvents();

                    m_BeginTime = m_EndTime;
                    m_EndTime = std::chrono::high_resolution_clock::now();
                }
            }
        }

        Core(size_t WIDTH, size_t HEIGHT, std::string title) :
            SCR_WIDTH(WIDTH),
            SCR_HEIGHT(HEIGHT),
            c_title(title.c_str()),
            m_durationMainLoop(std::chrono::system_clock::duration::min()),
            m_durationFrameLoop(0.0f),
            renderLoopPtr(nullptr),
            inputHandlerPtr(nullptr),
            window(nullptr)


        {
            isInit = Core_init();
        }

        Core() :
            m_durationMainLoop(std::chrono::system_clock::duration::min()),
            m_durationFrameLoop(0.0f),
            renderLoopPtr(nullptr),
            inputHandlerPtr(nullptr),
            window(nullptr)
           
        {
            isInit = Core_init();
        }

        // glfw: terminate, clearing all previously allocated GLFW resources.
       // ------------------------------------------------------------------
        ~Core()
        {
            glfwTerminate();
        }
    };

}