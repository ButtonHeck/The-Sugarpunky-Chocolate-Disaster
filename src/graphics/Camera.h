#ifndef CAMERA_H
#define CAMERA_H
#include <vector>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/detail/func_geometric.hpp>
#include <glm/detail/func_trigonometric.hpp>
#include <glm/detail/func_common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "game/Settings.h"

enum MOVE_DIRECTION
{
  FORWARD,
  BACKWARD,
  RIGHT,
  LEFT,
  UP,
  DOWN
};

class Camera
{
public:

  Camera(glm::vec3 position);
  glm::mat4 getViewMatrix() const;
  void processMouseCursor(float xOffset, float yOffset);
  void processMouseScroll(float yOffset);
  void processKeyboardInput(float delta, MOVE_DIRECTION dir, std::vector<std::vector<float>>& hillsMap);
  void switchFPSmode();
  bool getFPSmode() const;
  float getZoom() const;
  glm::vec3 getPosition() const;
  glm::vec3 getDirection() const;
  void setPitch(float pitch);
  void setYaw(float yaw);
  void setPosition(float x, float y, float z);
  glm::vec3 getFront() const;
  glm::vec3 getRight() const;
  glm::vec3 getUp() const;
  float getPitch();
  float getYaw();
  void updateVectors();
  int getMapCoordX() const;
  int getMapCoordZ() const;
private:
  //options
  float zoom, moveSpeed, mouseSensitivity;
  bool FPSmode;
  //Euler angles
  float yaw, pitch;
  //direction vectors
  glm::vec3 Position, Front, Right, Up, WorldUp;
  void moveCameraFrontAxial(bool forward, float velocity);
};

#endif // CAMERA_H
