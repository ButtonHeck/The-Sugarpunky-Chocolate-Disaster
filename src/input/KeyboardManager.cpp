#include "input/KeyboardManager.h"

KeyboardManager::KeyboardManager(GLFWwindow *window, Camera &camera, Options &options)
  :
    window(window),
    camera(camera),
    options(options)
{}

void KeyboardManager::processInput(float delta, const map2D_f &hillsMap)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  processKey(GLFW_KEY_F1, [&](){camera.switchFPSmode();});
  processKey(GLFW_KEY_F2, OPT_MODELS_SHADOW_EMPHASIZE);
  processKey(GLFW_KEY_F3, OPT_ANIMATE_WATER);
  processKey(GLFW_KEY_F4, OPT_DRAW_TREES);
  processKey(GLFW_KEY_F5, OPT_DRAW_DEBUG_TEXT);
  processKey(GLFW_KEY_F6, OPT_DRAW_BUILDABLE);
  processKey(GLFW_KEY_F7, OPT_MODELS_PHONG_SHADING);
  processKey(GLFW_KEY_F9, [&]()
  {
      options.set(OPT_RECREATE_TERRAIN_REQUEST, true);
      options.set(OPT_CREATE_SHADOW_MAP_REQUEST, true);
    });
  processKey(GLFW_KEY_F10, [&](){options.set(OPT_SAVE_REQUEST, true);});
  processKey(GLFW_KEY_F11, [&]()
  {
      options.set(OPT_LOAD_REQUEST, true);
      options.set(OPT_CREATE_SHADOW_MAP_REQUEST, true);
    });
  processKey(GLFW_KEY_T, OPT_HILLS_CULLING);
  processKey(GLFW_KEY_Y, OPT_WATER_CULLING);
  processKey(GLFW_KEY_M, [&]()
  {
      options.toggle(OPT_USE_MULTISAMPLING);
      if (options.get(OPT_USE_MULTISAMPLING))
        glEnable(GL_MULTISAMPLE);
      else
        glDisable(GL_MULTISAMPLE);
    });
  processKey(GLFW_KEY_Q, OPT_USE_SHADOWS);
  processKey(GLFW_KEY_F, OPT_DRAW_LAND);
  processKey(GLFW_KEY_G, OPT_DRAW_WATER);
  processKey(GLFW_KEY_B, OPT_MODELS_FLAT_BLENDING);
  processKey(GLFW_KEY_L, OPT_POLYGON_LINE);

  processKey(GLFW_KEY_KP_8, [&]()
  {
      camera.setYaw(-90.0f);
    });
  processKey(GLFW_KEY_KP_6, [&]()
  {
      camera.setYaw(0.0f);
    });
  processKey(GLFW_KEY_KP_2, [&]()
  {
      camera.setYaw(90.0f);
    });
  processKey(GLFW_KEY_KP_4, [&]()
  {
      camera.setYaw(180.0f);
    });
  processKey(GLFW_KEY_KP_5, [&]()
  {
      camera.setPitch(-90.0f);
    });

  //process camera
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.processKeyboardInput(delta, FORWARD, hillsMap);
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboardInput(delta, BACKWARD, hillsMap);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboardInput(delta, LEFT, hillsMap);
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboardInput(delta, RIGHT, hillsMap);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    camera.processKeyboardInput(delta, DOWN, hillsMap);
  else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.processKeyboardInput(delta, UP, hillsMap);
}

void KeyboardManager::processKey(int keyCode, OPTION option)
{
  if (glfwGetKey(window, keyCode) == GLFW_PRESS)
    {
      if (!keysPressed[keyCode])
        {
          options.toggle(option);
          keysPressed[keyCode] = true;
        }
    }
  if (glfwGetKey(window, keyCode) == GLFW_RELEASE)
    keysPressed[keyCode] = false;
}

void KeyboardManager::processKey(int keyCode, std::function<void ()> function)
{
  if (glfwGetKey(window, keyCode) == GLFW_PRESS)
    {
      if (!keysPressed[keyCode])
        {
          function();
          keysPressed[keyCode] = true;
        }
    }
  if (glfwGetKey(window, keyCode) == GLFW_RELEASE)
    keysPressed[keyCode] = false;
}
