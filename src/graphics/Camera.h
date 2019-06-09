#pragma once

#include "TypeAliases"
#include "SceneSettings"

#include <fstream>
#include <glm/gtc/matrix_transform.hpp>

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

  //mutators and setters
  void move(float delta, const map2D_f& hillsMap);
  void updateViewAcceleration(float xOffset, float yOffset);
  void processMouseCursor();
  void processMouseScroll(float yOffset);
  void updateAccelerations(MOVE_DIRECTION dir);
  void setYaw(float newYaw);
  void setPitch(float newPitch);
  void switchFPSmode();
  void switchAcceleration();
  void disableMoveAcceleration();

  //getters
  glm::mat4 getViewMatrix() const;
  glm::mat3 getViewMatrixMat3() const;
  glm::mat4 getReflectionViewMatrix() const;
  float getZoom() const;
  const glm::vec3& getPosition() const;
  const glm::vec3& getDirection() const;
  const glm::vec3& getRight() const;
  const glm::vec3& getUp() const;
  glm::vec2 getWorldCoordinates() const;
  glm::vec2 getViewAcceleration() const;

  //file saving/loading stuff
  void serialize(std::ofstream& output);
  void deserialize(std::ifstream& input);

private:
  const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
  const float FOV = 60.0f;
  const float MOVE_SPEED = 8.0f;
  const float INITIAL_MOUSE_SENSITIVITY = 0.015f;
  const float INITIAL_VIEW_ACCELERATION_SENSITIVITY = 0.001f;
  const float VIEW_ACCELERATION_DAMPENING_FACTOR = 0.85f;
  const float DEFAULT_VIEW_ACCELERATION = 20.0f;
  const float INITIAL_MOVE_ACCELERATION_SENSITIVITY = 0.05f;
  const float MOVE_ACCELERATION_DAMPENING_FACTOR = 0.92f;
  const float INITIAL_YAW_ANGLE_DEGREES = -90.0f;
  const float MAX_PITCH = 89.9f;
  const float MIN_PITCH = -65.0f;
  const float MAX_MOUSE_SENSITIVITY = 0.02f;
  const float MIN_MOUSE_SENSITIVITY = 0.002f;
  const float CAMERA_WORLD_BORDER_OFFSET = 8.0f;
  const float CAMERA_WORLD_MIN_HEIGHT = 2.0f;
  const float CAMERA_WORLD_MIN_HEIGHT_ON_HILLS = CAMERA_WORLD_MIN_HEIGHT - HILLS_OFFSET_Y;
  const float CAMERA_WORLD_MAX_HEIGHT = 30.0f;

  void updateDirectionVectors();
  void diminishMoveAcceleration(float& directionAccelerationValue);
  void moveCameraFrontAxial(float velocity);

  //options
  float zoom, moveSpeed, mouseSensitivity;
  bool useAcceleration;
  float viewAccelerationX, viewAccelerationY, viewAccelerationSensitivity;
  float moveAccelerationSide, moveAccelerationY, moveAccelerationFront;
  float moveAccelerationSensitivity;
  bool accumulateMoveHorizontal = false, accumulateMoveFront = false, accumulateMoveVertical = false;
  bool firstPersonShooterMode;

  //Euler angles
  float yaw, pitch;

  //direction vectors
  glm::vec3 position, front, right, up;
};
