#include "Logger.h"
#include "OGLRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui_impl_glfw.h>
#include <iostream>

OGLRenderer::OGLRenderer(GLFWwindow *window) { mRenderData.rdWindow = window; }

bool OGLRenderer::init(unsigned int width, unsigned int height) {

  /* required for perspective */
  mRenderData.rdWidth = width;
  mRenderData.rdHeight = height;

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    Logger::log(1, "%s: Could not create window\n", __FUNCTION__);
    return false;
  }
  if (!GLAD_GL_VERSION_4_6) {
    Logger::log(1, "%s: OpenGL its not 4.6\n", __FUNCTION__);
    return false;
  }
  if (!mFramebuffer.init(width, height)) {
    Logger::log(1, "%s: Error create FrameBuffer\n", __FUNCTION__);
    return false;
  }
  if (!mTex.loadTexture("textures/crate.png")) {
    Logger::log(1, "%s: Error Load Texture\n", __FUNCTION__);
    return false;
  }
  mVertexBuffer.init();
  if (!mShader.loadShaders("shader/basic.vert", "shader/basic.frag")) {
    Logger::log(1, "%s: Error loader Shaders\n", __FUNCTION__);
    return false;
  }
  if (!mChangedShader.loadShaders("shader/changed.vert",
                                  "shader/changed.frag")) {
    Logger::log(1, "%s: Error loader Shaders\n", __FUNCTION__);
    return false;
  }
  if (!mGltfShader.loadShaders("shader/gltf.vert", "shader/gltf.frag")) {
    return false;
  }

  /* add backface culling and depth test already here */
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glLineWidth(3.0);

  mGltfModel = std::make_shared<GltfModel>();
  // std::string modelFilename = "assets/woman/Woman.gltf";
  // std::string modelTexFilename = "assets/woman/Woman.png";
  std::string modelFilename = "assets/woman/Woman.gltf";
  std::string modelTexFilename = "assets/woman/Woman.png";
  if (!mGltfModel->loadModel(mRenderData, modelFilename, modelTexFilename)) {
    return false;
  }
  mGltfModel->uploadIndexBuffer();

  mUniformBuffer.init();
  Logger::log(1, "%s: uniform buffer successfully created\n", __FUNCTION__);

  mUserInterface.init(mRenderData);

  std::cout << "Shader Program: " << mChangedShader.GetShaderProgram();
  return true;
}

void OGLRenderer::setSize(unsigned int width, unsigned int height) {
  mRenderData.rdWidth = width;
  mRenderData.rdHeight = height;
  mFramebuffer.resize(width, height);

  glViewport(0, 0, width, height);
}

void OGLRenderer::uploadData(OGLMesh vertexData) {
  mRenderData.rdTriangleCount = vertexData.vertices.size();
  mVertexBuffer.uploadData(vertexData);
}

void OGLRenderer::draw() {
  mGltfModel->uploadVertexBuffers();
  double tickTime = glfwGetTime();
  mRenderData.rdTickDiff = tickTime - lastTickTime;
  handleMovementKeys();
  static float prevFrameStartTime = 0.0;
  float frameStartTime = glfwGetTime();
  while (mRenderData.rdWidth == 0 || mRenderData.rdHeight == 0) {
    glfwGetFramebufferSize(mRenderData.rdWindow, &mRenderData.rdWidth,
                           &mRenderData.rdHeight);
    glfwWaitEvents();
  }
  mFramebuffer.bind();
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  mMatrixGenerateTimer.start();
  mProjectionMatrix = glm::perspective(
      glm::radians(static_cast<float>(mRenderData.rdFieldOfView)),
      static_cast<float>(mRenderData.rdWidth) /
          static_cast<float>(mRenderData.rdHeight),
      0.1f, 100.f);
  float t = glfwGetTime();
  glm::mat4 view = glm::mat4(1.0);

  if (mRenderData.rdUseChangedShader) {
    mChangedShader.use();
  } else {
    int vertexColorLocation =
        glGetUniformLocation(mShader.GetShaderProgram(), "ourColor");
    glUseProgram(mShader.GetShaderProgram());
    glUniform4f(vertexColorLocation, mRenderData.rShader, mRenderData.gShader,
                mRenderData.bShader, 1.0f);
    mShader.use();
  }
  mViewMatrix = mCamera.getViewMatrix(mRenderData) * view;
  mRenderData.rdMatrixGenerateTime = mMatrixGenerateTimer.stop();
  mMatrixUploadTimer.start();
  mUniformBuffer.uploadUboData(mViewMatrix, mProjectionMatrix);
  mRenderData.rdMatrixUploadTime = mMatrixUploadTimer.stop();

  mTex.bind();
  mVertexBuffer.bind();
  mVertexBuffer.draw(GL_TRIANGLES, 0, mRenderData.rdTriangleCount);
  int ligthColor =
      glGetUniformLocation(mGltfShader.GetShaderProgram(), "lightColor");
  glUseProgram(mGltfShader.GetShaderProgram());
  glUniform3f(ligthColor, mRenderData.rShader, mRenderData.gShader,
              mRenderData.bShader);
  int ligthPosLocation =
      glGetUniformLocation(mGltfShader.GetShaderProgram(), "lightPos");
  glUseProgram(mGltfShader.GetShaderProgram());
  glUniform3f(ligthPosLocation, mRenderData.xLight, mRenderData.yLight,
              mRenderData.zLight);
  int ambientStrengthShader =
      glGetUniformLocation(mGltfShader.GetShaderProgram(), "ambientStrength");
  glUniform1f(ambientStrengthShader, mRenderData.ambStr);
  mGltfShader.use();
  mGltfModel->draw();
  mVertexBuffer.unbind();
  mTex.unbind();
  mFramebuffer.unbind();
  mFramebuffer.drawToScreen();
  mUIGenerateTimer.start();
  mUserInterface.createFrame(mRenderData);
  mRenderData.rdUIGenerateTime = mUIGenerateTimer.stop();
  mUIDrawTimer.start();
  mUserInterface.render();
  mRenderData.rdUIDrawTime = mUIDrawTimer.stop();
  mRenderData.rdFrameTime = frameStartTime - prevFrameStartTime;
  prevFrameStartTime = frameStartTime;
  lastTickTime = tickTime;
}

void OGLRenderer::cleanup() {
  mShader.cleanup();
  mChangedShader.cleanup();
  mTex.cleanup();
  mVertexBuffer.cleanup();
  mFramebuffer.cleanup();
  mUniformBuffer.cleanup();
  mUserInterface.cleanup();
  mGltfModel->cleanup();
  mGltfModel.reset();
  mGltfShader.cleanup();
}

void OGLRenderer::handleKeyEvents(int key, int scancode, int action, int mods) {
  int i = glfwGetKey(mRenderData.rdWindow, GLFW_KEY_SPACE);
  if (i == GLFW_PRESS)
    mRenderData.rdUseChangedShader = !mRenderData.rdUseChangedShader;
}

void OGLRenderer::handleMouseButtonEvents(int button, int action, int mods) {
  ImGuiIO &io = ImGui::GetIO();
  if (button >= 0 && button < ImGuiMouseButton_COUNT) {
    io.AddMouseButtonEvent(button, action == GLFW_PRESS);
    Logger::log(1, "EventMouse");
  }
  if (io.WantCaptureMouse) {
    return;
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    mMouseLock = !mMouseLock;
    if (mMouseLock) {
      glfwSetInputMode(mRenderData.rdWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(mRenderData.rdWindow, GLFW_RAW_MOUSE_MOTION,
                         GLFW_TRUE);
      }
    } else {
      glfwSetInputMode(mRenderData.rdWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
}

void OGLRenderer::handleMousePositionEvents(double xPos, double yPos) {
  ImGuiIO &io = ImGui::GetIO();
  io.AddMousePosEvent((float)xPos, (float)yPos);
  if (io.WantCaptureMouse) {
    return;
  }

  int mouseMoveRelX = static_cast<int>(xPos) - mMouseXPos;
  int mouseMoveRelY = static_cast<int>(yPos) - mMouseYPos;
  if (mMouseLock) {
    mRenderData.rdViewAzimuth += mouseMoveRelX / 10.0;
    if (mRenderData.rdViewAzimuth < 0.0) {
      mRenderData.rdViewAzimuth += 360.0;
    }
    if (mRenderData.rdViewAzimuth >= 360.0) {
      mRenderData.rdViewAzimuth -= 360.0;
    }
    mRenderData.rdViewElevation -= mouseMoveRelY / 10.0;
    if (mRenderData.rdViewElevation > 89.0) {
      mRenderData.rdViewElevation = 89.0;
    }
    if (mRenderData.rdViewElevation < -89.0) {
      mRenderData.rdViewElevation = -89.0;
    }
  }
  mMouseXPos = static_cast<int>(xPos);
  mMouseYPos = static_cast<int>(yPos);
}

void OGLRenderer::handleMovementKeys() {
  mRenderData.rdMoveForward = 0;
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_W) == GLFW_PRESS) {
    mRenderData.rdMoveForward += 1;
  }
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_S) == GLFW_PRESS) {
    mRenderData.rdMoveForward -= 1;
  }
  mRenderData.rdMoveRight = 0;
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_A) == GLFW_PRESS) {
    mRenderData.rdMoveRight -= 1;
  }
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_D) == GLFW_PRESS) {
    mRenderData.rdMoveRight += 1;
  }
  mRenderData.rdMoveUp = 0;
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_E) == GLFW_PRESS) {
    mRenderData.rdMoveUp += 1;
  }
  if (glfwGetKey(mRenderData.rdWindow, GLFW_KEY_Q) == GLFW_PRESS) {
    mRenderData.rdMoveUp -= 1;
  }
}
