#ifndef GAME_H
#define GAME_H
#include <thread>
#include <memory>
#include <array>
#include <GL/glew.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include "../../src/util/DirectoriesSettings.h"
#include "../../src/util/Timer.h"
#include "../../src/util/BenchmarkTimer.h"
#include "../../src/input/KeyboardManager.h"
#include "../../src/game/SaveLoadManager.h"
#include "../../src/game/world/Scene.h"
#include "../../src/graphics/shaders/ShaderManager.h"
#include "../../src/graphics/textures/TextureManager.h"
#include "../../src/graphics/textures/BindlessTextureManager.h"
#include "../../src/graphics/Frustum.h"
#include "../../src/graphics/ShadowVolume.h"
#include "../../src/graphics/gui/TextManager.h"
#include "../../src/graphics/gui/CoordinateSystemRenderer.h"
#include "../../src/graphics/gui/FrustumRenderer.h"
#include "../../src/graphics/gui/ShadowVolumeRenderer.h"
#include "../../src/graphics/framebuffers/ScreenFramebuffer.h"
#include "../../src/graphics/framebuffers/DepthmapFramebuffer.h"
#include "../../src/graphics/framebuffers/WaterReflectionFramebuffer.h"
#include "../../src/graphics/framebuffers/WaterRefractionFramebuffer.h"

class Camera;
class Options;
class ScreenResolution;
class MouseInputManager;

class Game
{
public:
  Game(GLFWwindow* window, Camera& camera, Camera& shadowCamera, Options& options, const ScreenResolution& screenResolution);
  virtual ~Game();
  void setup();
  void loop();

private:
  void drawFrame(const glm::mat4& projectionView);
  void drawFrustumVisualizations(const glm::mat4 &projectionView);
  void drawFrameReflection();
  void drawFrameRefraction(const glm::mat4& projectionView);
  void recreate();
  void updateDepthmap();
  void saveState();
  void loadState();

  //context and hardware
  const ScreenResolution& screenResolution;
  GLFWwindow* window;
  Timer CPU_timer;
  unsigned long updateCount = 0;

  //camera and related stuff
  Camera& camera;
  Camera& shadowCamera;
  Frustum viewFrustum, cullingViewFrustum;
  std::array<Frustum, NUM_SHADOW_LAYERS> shadowCameraFrustums;
  std::array<FrustumRenderer, NUM_SHADOW_LAYERS-1> shadowFrustumRenderers;
  glm::mat4 projection, cullingProjection;
  std::array<glm::mat4, NUM_SHADOW_LAYERS> shadowProjections;

  //options
  Options& options;

  //graphics
  ShaderManager shaderManager;
  TextureLoader textureLoader;
  TextureManager textureManager;
  CoordinateSystemRenderer csRenderer;
  ScreenFramebuffer screenFramebuffer;
  DepthmapFramebuffer depthmapFramebuffer;
  WaterReflectionFramebuffer reflectionFramebuffer;
  WaterRefractionFramebuffer refractionFramebuffer;

  //world
  ShadowVolume shadowVolume;
  Scene scene;
  ShadowVolumeRenderer shadowVolumeRenderer;
  SaveLoadManager saveLoadManager;

  //input
  KeyboardManager keyboard;
  MouseInputManager& mouseInput;

  //GUI and text
  TextManager textManager;

  //multithreading
  void setupThreads();

  std::unique_ptr<std::thread> meshIndirectBufferUpdater;
  std::mutex modelIndirectUpdateThreadMutex;
  std::condition_variable modelsIndirectBufferNeedUpdateCV;
  std::atomic_bool modelsIndirectBufferPrepared, modelsIndirectBufferNeedUpdate;

  bool landIndirectBufferHasUpdated = false; //not used in any other threads, thus no need to declare as atomic
};

#endif // GAME_H
