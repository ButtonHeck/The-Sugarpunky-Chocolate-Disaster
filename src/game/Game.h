#ifndef GAME_H
#define GAME_H
#include <thread>
#include <memory>
#include <array>
#include <GL/glew.h>
#include "util/DirectoriesSettings.h"
#include "util/Timer.h"
#include "util/BenchmarkTimer.h"
#include "input/KeyboardManager.h"
#include "input/MouseInputManager.h"
#include "game/SaveLoadManager.h"
#include "game/world/Scene.h"
#include "graphics/shaders/ShaderManager.h"
#include "graphics/textures/TextureManager.h"
#include "graphics/textures/BindlessTextureManager.h"
#include "graphics/Frustum.h"
#include "graphics/RendererStateManager.h"
#include "graphics/ShadowVolume.h"
#include "graphics/gui/TextManager.h"
#include "graphics/gui/CoordinateSystemRenderer.h"
#include "graphics/gui/FrustumRenderer.h"
#include "graphics/gui/ShadowVolumeRenderer.h"
#include "graphics/framebuffers/ScreenFramebuffer.h"
#include "graphics/framebuffers/DepthmapFramebuffer.h"
#include "graphics/framebuffers/WaterReflectionFramebuffer.h"
#include "graphics/framebuffers/WaterRefractionFramebuffer.h"

class Camera;
class Options;
class ScreenResolution;

class Game
{
public:
  Game(GLFWwindow* window, Camera& camera, Camera& shadowCamera, Options& options, ScreenResolution& screenResolution);
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
  ScreenResolution& screenResolution;
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
  Scene scene;
  ShadowVolume shadowVolume;
  ShadowVolumeRenderer shadowVolumeRenderer;
  SaveLoadManager saveLoadManager;

  //input
  KeyboardManager keyboard;
  MouseInputManager& mouseInput;

  //GUI and text
  TextManager textManager;

  //multithreading
  void setupThreads();
  std::unique_ptr<std::thread> waterAnimator;
  std::unique_ptr<std::thread> meshIndirectBufferUpdater;
  volatile bool modelsIndirectBufferPrepared = false, modelsIndirectBufferNeedUpdate = false;
  volatile bool waterKeyFrameReady = false, waterNeedNewKeyFrame = true;
  bool landIndirectBufferHasUpdated = false;
};

#endif // GAME_H
