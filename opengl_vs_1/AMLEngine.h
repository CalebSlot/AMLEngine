#pragma once
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>
#include <chrono>

#include <functional>



namespace AMLEngine
{


    struct ISize
    {
        int WIDTH;
        int HEIGHT;
    };

    struct IPosition
    {
        int X;
        int Y;

        bool operator == (const IPosition& other)
        {
            if (&other == this)
            {
                return true;
            }
            return other.X == X && other.Y == Y;
        }

    };
    struct FPosition3
    {
        float X;
        float Y;
        float Z;
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
       class Keyboard;

    public:
        //UTILITY CLASS TO HANDLE A SCENE
        //THE ENGINE USER SHOULD IMPLEMENT THE PROPER S class and do something like this
        //Scene<SceneImpl> myScene;

        template <class S>
        class Scene : S
        {

        public:


            void create(AMLEngine::Core& oCore)
            {
                this->OnCreate(oCore);
            }
            void render(AMLEngine::Core& oCore)
            {
                this->OnRender(oCore);
            }
            void update()
            {
                this->OnUpdate();
            }
            void input(const AMLEngine::Core::Keyboard& oKeyboard)
            {
                this->OnInput(oKeyboard);
            }
            void destroy(AMLEngine::Core& oCore)
            {
                this->destroy(oCore);
            }
        };

        struct Timer
        {
            std::chrono::steady_clock::time_point   m_BeginTime;
            std::chrono::steady_clock::time_point   m_EndTime;

            Timer()
            {
                m_BeginTime = std::chrono::high_resolution_clock::now();
                m_EndTime = m_BeginTime;
            }

            float GetTime()
            {
                m_EndTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> elapsedTime = m_EndTime - m_BeginTime;

                m_BeginTime = m_EndTime;

                const auto fElapsed = elapsedTime.count();

                return fElapsed;
            }
        };
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

        typedef std::function<void(const Keyboard&)> KeyboardInputHandlerPtr;
        typedef std::function<void(Core&)> RenderHandlerPtr;
        typedef std::function<void(void)> UpdateHandlerPtr;

       // typedef void (*KeyboardInputHandlerPtr)(const Keyboard&);
       // typedef void (*RenderHandlerPtr)(Core&);
       // typedef void (*UpdateHandlerPtr)();
        typedef void (*ErrorHandlerPtr)(int, const char*);

        typedef std::chrono::duration<float> DurationSeconds;
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

            bool plus() const
            {
                return glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS;
            }
            bool minus() const
            {
                return glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS;
            }
        };

      
    private:
        FrameLimit m_eRenderLimit = FrameLimit::NONE;
        const unsigned int SCR_WIDTH = 800;
        const unsigned int SCR_HEIGHT = 600;
        const char * c_title = "OpenGLTestBed";
        const float FPS_60 = 1.0f / 60.0f;
        const float FPS_30 = 1.0f / 30.0f;
        bool isInit = false;

        GLFWwindowPtr window;
        RenderHandlerPtr renderHandlerPtr;
        KeyboardInputHandlerPtr inputHandlerPtr;
        UpdateHandlerPtr updateHandlerPtr;
        std::exception initException;
        DurationSeconds  m_durationMainLoop;
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
            //this works only above versions
            //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);


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

       

        void renderFallback(AMLEngine::Core& core)
        {

        }
        void inputFallback(AMLEngine::Core::Keyboard& keyboard)
        {

        }
        void updateFallback()
        {

        }
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
        ISize getWindowSize()
        {
            ISize size;

            glfwGetWindowSize(window, &size.WIDTH, &size.HEIGHT);
         
            return size;
        }
        struct Draw
        {
            static void Square(int x, int y,size_t side, const AMLEngine::Colors::Color& color)
            {
                float halfSide = side / 2;
                glColor3f(color.r, color.g, color.b);
                glBegin(GL_QUADS);
                    glVertex2f(x - halfSide, y- halfSide);
                    glVertex2f(x - halfSide, y+ halfSide);
                    glVertex2f(x + halfSide, y+ halfSide);
                    glVertex2f(x + halfSide, y- halfSide);
                glEnd();
            }
            static void Circle(int x, int y, size_t radius, const AMLEngine::Colors::Color& color)
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
              //  glFlush();
            }
            static void Curve(AMLEngine::FPosition3* positions,size_t numPoints,size_t numPoints_subCurve,size_t interp_steps, const AMLEngine::Colors::Color& color,size_t connect_prevs = 0,bool draw_points = false,float point_size = 5.0f)
            {
                
                const float f_interp_steps = (float)interp_steps;
                if (numPoints_subCurve > numPoints)
                {
                    numPoints_subCurve = numPoints;
                }

                int step = (int)numPoints_subCurve - (int)connect_prevs;
                if (step < 0)
                {
                    step = 0;
                }

                glColor3f(color.r, color.g, color.b);

                for (int i = 0; (i + step) < numPoints; i += step) {
   
                    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, numPoints_subCurve, &positions[i].X);
                    glEnable(GL_MAP1_VERTEX_3);

                    glBegin(GL_LINE_STRIP);
                    {
                        for (int i = 0; i < (int)interp_steps; i++) {
                            glEvalCoord1f(((float)i) / f_interp_steps);
                        }
                    }
                    glEnd();
                }

                if (draw_points)
                {
                    glPointSize(point_size);
                    glBegin(GL_POINTS);
                    for (int i = 0; i < numPoints; i++)
                    {
                        glVertex3fv(&positions[i].X);
                    }
                    glEnd();

                }

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

        void setRenderLoop(RenderHandlerPtr rLoopPtr)
        {
            assert(rLoopPtr != nullptr);
            renderHandlerPtr = rLoopPtr;
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
        void setUpdateHandler(UpdateHandlerPtr uHandlerPtr)
        {
            assert(uHandlerPtr != nullptr);
            updateHandlerPtr = uHandlerPtr;
        }
        private:
        //get the last loop cicle execution time in milliseconds 
        //if the frame rate is not fixed it has the same value as the frame rate (the last is in seconds)
        const DurationFloat getDeltaTimeMS() const
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(m_durationMainLoop).count();
        }
        //get the last loop cicle execution time in milliseconds 
        //if the frame rate is not fixed it has the same value as the frame rate
        const DurationFloat getDeltaTimeS() const
        {
            return m_durationMainLoop.count();
        }
        public:
        //get the last frame execution time in seconds
        //if the frame rate is not fixed it has the same value getDeltaTimeS()
        const DurationFloat getFrameTime() const
        {
            return m_durationFrameLoop;
        }
        void run()
        {
            //precondition
            if (!isInit)
            {
                return;
            }

            if (!renderHandlerPtr)
            {
                renderHandlerPtr = [](Core&) {

                };
            }

            if (!inputHandlerPtr)
            {
                inputHandlerPtr = [](const Core::Keyboard&) {

                };
            
            }
            if (!updateHandlerPtr)
            {
                updateHandlerPtr = []() {

                };
            }
            //TODO: add fixed pipe or shader
            setupOrtho(SCR_WIDTH,SCR_HEIGHT);
             //main loop


            //TODO:: add different paths on different configurations (different whiless.)
            //DOING
            //FREE PFS

            float counter = 0.0f;
            float fixedDt = 0.003f;

            if (m_eRenderLimit == FrameLimit::NONE)
            {
                auto m_BeginFrame = std::chrono::high_resolution_clock::now();
                auto m_EndFrame   = m_BeginFrame;
              
                while (!glfwWindowShouldClose(window))
                {
                    m_EndFrame          = std::chrono::high_resolution_clock::now();
                    m_durationMainLoop  = m_EndFrame - m_BeginFrame;
                    m_durationFrameLoop = m_durationMainLoop.count();
                    m_BeginFrame        = m_EndFrame;

                    counter += m_durationFrameLoop;

                    glfwPollEvents();
                    // input
                    // -----
                    inputHandlerPtr(keyboard);


                    //update
                    size_t steps = 0;
                    while (counter >= fixedDt)
                    {
                        updateHandlerPtr();

                        counter -= fixedDt;
                        steps++;
                    }

                    // render
                    // ------
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);

                    renderHandlerPtr(*this);
                    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
                    // -------------------------------------------------------------------------------
                    glfwSwapBuffers(window);
                 

                 
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
                    m_EndTime                               = std::chrono::high_resolution_clock::now();
                    m_durationMainLoop                      = m_EndTime - m_BeginTime;
                    std::chrono::duration<float> renderTime = m_EndTime - m_LastFrameTime;

                    m_durationFrameLoop                     = renderTime.count();
                    m_BeginTime = m_EndTime;

                    counter += m_durationMainLoop.count();


                    glfwPollEvents();
                    // input
                    // -----
                    inputHandlerPtr(keyboard);

                    //update
                    size_t steps = 0;
                    while (counter >= fixedDt)
                    {
                        updateHandlerPtr();

                        counter -= fixedDt;
                        steps++;
                    }

                    //render
                    // ------
                    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                    glClear(GL_COLOR_BUFFER_BIT);


                    if (m_durationFrameLoop >= renderLimit)
                    {

                        renderHandlerPtr(*this);
                        //glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
                        // -------------------------------------------------------------------------------
                        glfwSwapBuffers(window);
                        m_LastFrameTime = std::chrono::high_resolution_clock::now();
                    }

                   
                 
                }
            }
        }

        Core(size_t WIDTH, size_t HEIGHT, std::string title) :
            SCR_WIDTH(WIDTH),
            SCR_HEIGHT(HEIGHT),
            c_title(title.c_str()),
            m_durationMainLoop(std::chrono::system_clock::duration::min()),
            m_durationFrameLoop(0.0f),
            updateHandlerPtr(nullptr),
            renderHandlerPtr(nullptr),
            inputHandlerPtr(nullptr),
            window(nullptr)


        {
            isInit = Core_init();
        }

        Core() :
            m_durationMainLoop(std::chrono::system_clock::duration::min()),
            m_durationFrameLoop(0.0f),
            updateHandlerPtr(nullptr),
            renderHandlerPtr(nullptr),
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