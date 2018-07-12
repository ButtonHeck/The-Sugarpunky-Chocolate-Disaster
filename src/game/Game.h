#ifndef GAME_H
#define GAME_H
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "game/Settings.h"
#include "game/Timer.h"
#include "game/Options.h"
#include "game/InputController.h"
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

class Game
{
public:
  Game(GLFWwindow* window, glm::vec3& cursorDir, Camera& camera, Options& options, int width, int height, float aspect);
  ~Game();
  void setupVariables();
  void prepareTerrain();
  void prepareMS_FBO();
  void prepareDepthMapFBO();
  void prepareScreenVAO();
  void drawFrameToScreenRectangle(bool enableMS);
  void drawFrameObjects(glm::mat4& projectionView);
  void drawFrameObjectsDepthmap();
  void loop();
private:
  int scr_width;
  int scr_height;
  float aspect_ratio;
  GLFWwindow* window;
  glm::vec3& cursorToViewportDirection;
  Timer timer;
  Camera& camera;
  Renderer renderer = Renderer(camera);
  Frustum viewFrustum;
  Options& options;
  InputController input;
  TextureLoader textureLoader;
  WaterMapGenerator* waterMapGenerator = new WaterMapGenerator();
  HillsMapGenerator* hillMapGenerator = new HillsMapGenerator(waterMapGenerator->getMap());
  BaseMapGenerator* baseMapGenerator = new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap());
  BuildableMapGenerator* buildableMapGenerator = new BuildableMapGenerator(baseMapGenerator->getMap(), hillMapGenerator->getMap());
  SaveLoadManager* saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator, camera);
  TreeGenerator* treeGenerator;
  ShaderManager shaderManager;
  FontManager* fontManager;
  CoordinateSystemRenderer csRenderer = CoordinateSystemRenderer(&shaderManager.get(SHADER_CS));
  UnderwaterQuadMapGenerator underwaterQuadGenerator;
  Skybox skybox;
  TextureManager* textureManager;
  glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)scr_width / (float)scr_height, NEAR_PLANE, FAR_PLANE);
  GLuint screenVAO, screenVBO, multisampleFBO, screenFBO, depthMapFBO;
  unsigned int frameCounter = 0;
};

#endif // GAME_H
