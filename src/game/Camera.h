#ifndef CAMERA_H
#define CAMERA_H
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include "util/SceneSettings.h"
#include "util/typeAliases.h"
#include "util/BenchmarkTimer.h"

constexpr float FOV = 60.0f;

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
  Camera& operator=(const Camera& rhs);
  glm::mat4 getViewMatrix() const;
  void updateViewAcceleration(float xOffset, float yOffset);
  void processMouseCursor();
  void processMouseScroll(float yOffset);
  void updateAccelerations(MOVE_DIRECTION dir);
  void move(float delta, const map2D_f& hillsMap);
  void switchFPSmode();
  void switchAcceleration();
  void disableMoveAcceleration();
  bool getFPSmode() const;
  float getZoom() const;
  glm::vec3 getPosition() const;
  glm::vec3 getDirection() const;
  void setYaw(float newYaw);
  void setPitch(float newPitch);
  glm::vec3 getRight() const;
  glm::vec3 getUp() const;
  int getMapCoordX() const;
  int getMapCoordZ() const;
  glm::vec2 getViewAcceleration() const;
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

private:
  void updateVectors();
  void diminishMoveAcceleration(float& accelerationDirectionValue);
  void moveCameraFrontAxial(float velocity);

  //options
  float zoom, moveSpeed, mouseSensitivity;
  bool useAcceleration;
  float viewAccelerationX, viewAccelerationY, viewAccelerationSensitivity;
  float moveAccelerationSide, moveAccelerationY, moveAccelerationFront;
  float moveAccelerationSensitivity;
  bool accumulateMoveSide = false, accumulateMoveFront = false, accumulateMoveY = false;
  bool FPSmode;
  const float CAMERA_WORLD_BORDER_OFFSET = 8.0f;
  const float CAMERA_WORLD_MIN_HEIGHT = 2.0f;
  const float CAMERA_WORLD_MIN_HEIGHT_ON_HILLS = CAMERA_WORLD_MIN_HEIGHT - HILLS_OFFSET_Y;
  const float CAMERA_WORLD_MAX_HEIGHT = 30.0f;
  //Euler angles
  float yaw, pitch;
  //direction vectors
  glm::vec3 position, front, right, up, worldUp;
};

#endif // CAMERA_H
