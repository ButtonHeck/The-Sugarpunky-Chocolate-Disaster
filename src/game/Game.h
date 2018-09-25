#ifndef GAME_H
#define GAME_H
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "game/Settings.h"
#include "game/TextureUnits.h"
#include "game/Options.h"
#include "game/ScreenResolution.h"
#include "game/ScreenBuffer.h"
#include "game/DepthmapBuffer.h"
#include "input/KeyboardManager.h"
#include "input/MouseInputManager.h"
#include "game/SaveLoadManager.h"
#include "graphics/Camera.h"
#include "graphics/Renderer.h"
#include "graphics/Frustum.h"
#include "graphics/ShaderManager.h"
#include "graphics/FontManager.h"
#include "graphics/CoordinateSystemRenderer.h"
#include "graphics/Skybox.h"
#include "graphics/TextureManager.h"
#include "generator/UnderwaterQuadMapGenerator.h"
#include "model/Model.h"
#include "timer/Timer.h"
#include "timer/BenchmarkTimer.h"

class Game
{
public:
  Game(GLFWwindow* window, glm::vec3& cursorDir, Camera& camera, Options& options, ScreenResolution& screenResolution);
  ~Game();
  void setupVariables();
  void prepareTerrain();
  void drawFrameObjects(glm::mat4& projectionView);
  void drawFrameObjectsDepthmap();
  void loop();
private:
  //context and hardware
  ScreenResolution& screenResolution;
  GLFWwindow* window;
  Timer CPU_timer;
  unsigned long updateCount = 0;

  //camera and related stuff
  Camera& camera;
  glm::vec3& cursorToViewportDirection;
  Frustum viewFrustum;
  glm::mat4 projection;

  //input and options
  Options& options;
  KeyboardManager keyboard;
  MouseInputManager mouseInput;

  //graphics
  ShaderManager shaderManager;
  Renderer renderer;
  TextureLoader textureLoader;
  TextureManager textureManager;
  CoordinateSystemRenderer csRenderer;
  ScreenBuffer screenBuffer;
  DepthmapBuffer depthmapBuffer;

  //world
  WaterMapGenerator* waterMapGenerator;
  HillsMapGenerator* hillMapGenerator;
  BaseMapGenerator* baseMapGenerator;
  BuildableMapGenerator* buildableMapGenerator;
  SaveLoadManager* saveLoadManager;
  PlantGenerator* plantGenerator;
  UnderwaterQuadMapGenerator underwaterQuadGenerator;
  Skybox skybox;

  //GUI and text
  FontManager* fontManager;

  //multithreading
  std::thread* waterAnimationThread;
  std::thread* meshIndirectUpdateThread;
  volatile bool meshesIndirectDataReady = false, meshesIndirectDataNeed = false;
  volatile bool waterThreadHasUpdated = false, waterThreadUpdatePermitted = true;
#ifdef _DEBUG
  bool waterThreadAnimationIsWorking = true;
#endif
};

#endif // GAME_H
