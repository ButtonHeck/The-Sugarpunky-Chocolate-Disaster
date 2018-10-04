#ifndef GAME_H
#define GAME_H
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include "game/Settings.h"
#include "graphics/TextureUnits.h"
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
#include "graphics/TextRenderer.h"
#include "graphics/FontLoader.h"
#include "graphics/CoordinateSystemRenderer.h"
#include "graphics/Skybox.h"
#include "graphics/TextureManager.h"
#include "generator/UnderwaterQuadMapGenerator.h"
#include "model/Model.h"
#include "timer/Timer.h"
#include "timer/BenchmarkTimer.h"
#include "generator/PlantGeneratorFacade.h"

class Game
{
public:
  Game(GLFWwindow* window, Camera& camera, Options& options, ScreenResolution& screenResolution);
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
  Frustum viewFrustum;
  glm::mat4 projection;

  //input and options
  Options& options;
  KeyboardManager keyboard;
  MouseInputManager& mouseInput;

  //graphics
  ShaderManager shaderManager;
  Renderer renderer;
  TextureLoader textureLoader;
  TextureManager textureManager;
  CoordinateSystemRenderer csRenderer;
  ScreenBuffer screenBuffer;
  DepthmapBuffer depthmapBuffer;

  //world
  std::shared_ptr<WaterMapGenerator> waterMapGenerator;
  std::shared_ptr<HillsMapGenerator> hillMapGenerator;
  std::shared_ptr<BaseMapGenerator> baseMapGenerator;
  std::shared_ptr<BuildableMapGenerator> buildableMapGenerator;
  std::unique_ptr<SaveLoadManager> saveLoadManager;
  std::shared_ptr<PlantGeneratorFacade> plantGeneratorFacade;
  UnderwaterQuadMapGenerator underwaterQuadGenerator;
  Skybox skybox;

  //GUI and text
  FontLoader fontLoader;
  TextRenderer textRenderer;

  //multithreading
  void setupThreads();
  std::unique_ptr<std::thread> waterAnimator;
  std::unique_ptr<std::thread> meshIndirectBufferUpdater;
  volatile bool meshBufferReady = false, meshBufferNeedUpdate = false;
  volatile bool waterKeyFrameReady = false, waterNeedNewKeyFrame = true;
#ifdef _DEBUG
  bool waterAnimatorIsWorking = true;
#endif
};

#endif // GAME_H
