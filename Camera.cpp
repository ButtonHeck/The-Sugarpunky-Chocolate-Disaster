#include "Camera.h"

Camera::Camera(glm::vec3 position)
  :
    zoom(40.0f),
    moveSpeed(30),
    mouseSensitivity(0.05f),
    FPSmode(false),
    yaw(-90.0f),
    pitch(0.0f),
    Position(position),
    Front(glm::vec3(0.0, -2.0, -1.0)),
    WorldUp(0.0, 1.0, 0.0)
{
  updateVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
  return glm::lookAt(Position, Position + Front, WorldUp);
}

void Camera::processMouseCursor(float xOffset, float yOffset)
{
  xOffset *= mouseSensitivity;
  yOffset *= mouseSensitivity;
  yaw -= xOffset;
  pitch -= yOffset;
  if (pitch >= 89.0f)
    pitch = 89.0f;
  if (pitch <= -89.0f)
    pitch = -89.0f;
  updateVectors();
}

void Camera::processMouseScroll(float yOffset)
{
  zoom -= yOffset;
  if (zoom >= 45.0f)
    zoom = 45.0f;
  if (zoom <= 1.0)
    zoom = 1.0f;
  updateVectors();
}

void Camera::processKeyboardInput(float delta, Movement_Direction dir)
{
  float velocity = delta * moveSpeed;
  if (dir == FORWARD)
    {
      Position += Front * velocity;
      if (FPSmode)
        Position.y -= Front.y * velocity;
    }
  if (dir == BACKWARD)
    {
      Position -= Front * velocity;
      if (FPSmode)
        Position.y += Front.y * velocity;
    }
  if (dir == RIGHT)
    Position += Right * velocity;
  if (dir == LEFT)
    Position -= Right * velocity;
  if (dir == UP)
    Position += WorldUp * velocity;
  if (dir == DOWN)
    Position -= WorldUp * velocity;
}

void Camera::setFPSmode(bool on)
{
  FPSmode = on;
}

float Camera::getZoom() const
{
  return zoom;
}

glm::vec3 Camera::getPosition() const
{
  return Position;
}

glm::vec3 Camera::getDirection() const
{
  return Front;
}

void Camera::updateVectors()
{
  float x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  float y = std::sin(glm::radians(pitch));
  float z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  glm::vec3 front(x,y,z);
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}
