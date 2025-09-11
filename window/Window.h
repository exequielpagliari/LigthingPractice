#pragma once
#include <memory>
#include <string>
#include "OGLRenderer.h"
#include <GLFW/glfw3.h>
#include "Model.h"

class Window {
public:
    bool init(unsigned int width, unsigned int height, std::string title);
    void mainLoop();
    void cleanup();
    

private:
    GLFWwindow* mWindow = nullptr;
    void handleWindowCloseEvents();
    void handleKeyEvents(int key, int scancode, int action,
        int mods);
    //Variables para PosMouse
    double xpos, ypos;
    std::unique_ptr<OGLRenderer> mRenderer;
    std::unique_ptr<Model> mModel;
};