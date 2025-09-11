#include "Window.h"
#include "Logger.h"

bool Window::init(unsigned int width, unsigned int height, std::string title) {
    if (!glfwInit()) {
        Logger::log(1, "%s: glfwInit() error\n", __FUNCTION__);
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE);
    /* set a "hint" for the NEXT window created*/
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!mWindow) {
        Logger::log(1, "%s: Could not create window\n", __FUNCTION__);
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(mWindow);
    Logger::log(1, "%s: Window successfully initialized\n", __FUNCTION__);
    
    mRenderer = std::make_unique<OGLRenderer>(mWindow);
    if (!mRenderer->init(width, height)) {
        glfwTerminate();
        return false;
    }

    mModel = std::make_unique<Model>();
    mModel->init();

    glfwSetWindowUserPointer(mWindow, mRenderer.get());

    glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* win,
        int width, int height) {
            auto renderer = static_cast<OGLRenderer*>(
                glfwGetWindowUserPointer(win));
            renderer->setSize(width, height);
        }
    );
    
    //CallBack Cerrar Ventana
    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* win) {
        auto thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(win));
        thisWindow->handleWindowCloseEvents();
        }
    );


    //CallBack Teclado
    glfwSetKeyCallback(mWindow, [](GLFWwindow* win, int key,
        int scancode, int action, int mods) {
            auto thisWindow = static_cast<Window*>(
                glfwGetWindowUserPointer(win));
            thisWindow->handleKeyEvents(key, scancode, action,
                mods);
        }
    );




    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* win,
        int button, int action, int mods) {
            auto renderer = static_cast<OGLRenderer*>
                (glfwGetWindowUserPointer(win));
            renderer->handleMouseButtonEvents(button, action, mods);
        }
    );
    glfwSetCursorPosCallback(mWindow, [](GLFWwindow* win,
        double xpos, double ypos) {
            auto renderer = static_cast<OGLRenderer*>
                (glfwGetWindowUserPointer(win));
            renderer->handleMousePositionEvents(xpos, ypos);
        }
    );

    return true;
}

void Window::mainLoop() {
    glfwSwapInterval(1);
    mRenderer->uploadData(mModel->getVertexData());
    
    while (!glfwWindowShouldClose(mWindow)) {
        mRenderer->draw();
        /* swap buffers */
        glfwSwapBuffers(mWindow);
        /* poll events in a loop */
        glfwPollEvents();
        
    }
}

void Window::cleanup() {
    Logger::log(1, "%s: Terminating Window\n", __FUNCTION__);
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Window::handleWindowCloseEvents() {
    Logger::log(1, "%s: Window got close event... bye!\n", __FUNCTION__);
}

void Window::handleKeyEvents(int key, int scancode, int action, int
    mods) {
    std::string actionName;
    switch (action) {
    case GLFW_PRESS:
        actionName = "pressed";
        break;
    case GLFW_RELEASE:
        actionName = "released";
        break;
    case GLFW_REPEAT:
        actionName = "repeated";
        break;
    default:
        actionName = "invalid";
        break;
    }
    const char* keyName = glfwGetKeyName(key, 0);
    Logger::log(1, "%s: key %s (key %i, scancode %i) %s\n",
        __FUNCTION__, keyName, key, scancode,
        actionName.c_str());
}

