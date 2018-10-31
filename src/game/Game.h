#ifndef GAME_H
#define GAME_H
#include <thread>
#include <memory>
#include "util/Settings.h"
#include "graphics/ScreenBuffer.h"
#include "graphics/DepthmapBuffer.h"
#include "input/KeyboardManager.h"
#include "input/MouseInputManager.h"
#include "game/SaveLoadManager.h"
#include "graphics/RendererStateManager.h"
#include "graphics/Frustum.h"
#include "graphics/ShaderManager.h"
#include "graphics/TextManager.h"
#include "graphics/CoordinateSystemRenderer.h"
#include "graphics/TextureManager.h"
#include "util/Timer.h"
#include "util/BenchmarkTimer.h"
#include "game/world/Scene.h"

class Camera;
class Options;
class ScreenResolution;

class Game
{
public:
  Game(GLFWwindow* window, Camera& camera, Options& options, ScreenResolution& screenResolution);
  virtual ~Game();
  void setup();
  void loop();

private:
  void drawFrameObjects(glm::mat4& projectionView);
  void recreate();
  void updateDepthmap();
  void saveState();
  void loadState();

  //context and hardware
  ScreenResolution& screenResolution;
  GLFWwindow* window;
  Timer CPU_timer;
  unsigned long updateCount = 0;

  //camera and related stuff
  Camera& camera;
  Frustum viewFrustum;
  glm::mat4 projection;

  //input and options
  Options& options;
  KeyboardManager keyboard;
  MouseInputManager& mouseInput;

  //graphics
  ShaderManager shaderManager;
  TextureLoader textureLoader;
  TextureManager textureManager;
  CoordinateSystemRenderer csRenderer;
  ScreenBuffer screenBuffer;
  DepthmapBuffer depthmapBuffer;

  //world
  Scene scene;
  SaveLoadManager saveLoadManager;

  //GUI and text
  TextManager textManager;

  //multithreading
  void setupThreads();
  std::unique_ptr<std::thread> waterAnimator;
  std::unique_ptr<std::thread> meshIndirectBufferUpdater;
  volatile bool meshBufferReady = false, meshBufferNeedUpdate = false;
  volatile bool waterKeyFrameReady = false, waterNeedNewKeyFrame = true;
};

#endif // GAME_H
