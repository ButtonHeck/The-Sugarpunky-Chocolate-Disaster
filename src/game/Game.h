#ifndef GAME_H
#define GAME_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "src/game/Settings.h"
#include "src/game/Timer.h"
#include "src/game/Options.h"
#include "src/game/InputController.h"
#include "src/game/SaveLoadManager.h"
#include "src/graphics/Camera.h"
#include "src/graphics/Renderer.h"
#include "src/graphics/Frustum.h"
#include "src/graphics/ShaderManager.h"
#include "src/graphics/FontManager.h"
#include "src/graphics/CoordinateSystemRenderer.h"
#include "src/graphics/Skybox.h"
#include "src/graphics/TextureManager.h"
#include "src/generator/UnderwaterQuadMapGenerator.h"
#include "src/model/Model.h"

class Game
{
public:
  Game(GLFWwindow* window, glm::vec3& cursorDir, Camera& camera, Options& options, int width, int height, float aspect);
  ~Game();
  void setupVariables();
  void prepareTerrain();
  void prepareMS_FBO();
  void prepareScreenVAO();
  void drawFrameToScreenRectangle(bool enableHDR, bool enableMS);
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
  SaveLoadManager* saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator);
  TreeGenerator* treeGenerator;
  ShaderManager shaderManager;
  FontManager* fontManager;
  CoordinateSystemRenderer csRenderer = CoordinateSystemRenderer(&shaderManager.get(SHADER_CS));
  UnderwaterQuadMapGenerator underwaterQuadGenerator;
  Skybox skybox;
  TextureManager* textureManager;
  glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)scr_width / (float)scr_height, NEAR_PLANE, FAR_PLANE);
  GLuint screenVAO, screenVBO, multisampleFBO, screenFBO;
};

#endif // GAME_H
