#ifndef CAMERA_H
#define CAMERA_H
#include <fstream>
#include "util/Settings.h"
#include "util/typeAliases.h"
#include "util/BenchmarkTimer.h"

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
  void updateAcceleration(float xOffset, float yOffset);
  void processMouseCursor();
  void processMouseScroll(float yOffset);
  void processKeyboardInput(float delta, MOVE_DIRECTION dir, const map2D_f& hillsMap);
  void switchFPSmode();
  bool getFPSmode() const;
  float getZoom() const;
  glm::vec3 getPosition() const;
  glm::vec3 getDirection() const;
  void setYaw(float newYaw);
  void setPitch(float newPitch);
  glm::vec3 getRight() const;
  glm::vec3 getUp() const;
  void updateVectors();
  int getMapCoordX() const;
  int getMapCoordZ() const;
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);
private:
  //options
  float zoom, moveSpeed, mouseSensitivity;
  float accelerationX, accelerationY, accelerationSensitivity;
  bool FPSmode;
  const float CAMERA_WORLD_BORDER_OFFSET = 8.0f;
  const float CAMERA_WORLD_MIN_HEIGHT = 2.0f;
  const float CAMERA_WORLD_MIN_HEIGHT_ON_HILLS = CAMERA_WORLD_MIN_HEIGHT - HILLS_OFFSET_Y;
  const float CAMERA_WORLD_MAX_HEIGHT = 30.0f;
  //Euler angles
  float yaw, pitch;
  //direction vectors
  glm::vec3 position, front, right, up, worldUp;
  void moveCameraFrontAxial(bool forward, float velocity);
};

#endif // CAMERA_H
