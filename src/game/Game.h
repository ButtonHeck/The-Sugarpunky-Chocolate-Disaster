#pragma once

#include <GL/glew.h>
#include "DirectoriesSettings"
#include "Timer"
#include "BenchmarkTimer"
#include "KeyboardManager"
#include "SaveLoadManager"
#include "Scene"
#include "ShaderManager"
#include "TextureManager"
#include "BindlessTextureManager"
#include "Frustum"
#include "ShadowVolume"
#include "TextManager"
#include "CoordinateSystemRenderer"
#include "FrustumRenderer"
#include "ShadowVolumeRenderer"
#include "ScreenFramebuffer"
#include "DepthmapFramebuffer"
#include "WaterReflectionFramebuffer"
#include "WaterRefractionFramebuffer"

#include <thread>
#include <memory>
#include <array>
#include <atomic>
#include <mutex>
#include <condition_variable>

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
