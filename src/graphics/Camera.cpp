#include "graphics/Camera.h"

Camera::Camera(glm::vec3 position)
  :
    zoom(FOV),
    moveSpeed(8),
    mouseSensitivity(0.04f),
    FPSmode(false),
    yaw(-90.0f),
    pitch(0.0f),
    Position(position),
    Front(glm::vec3(0.0, 0.0, -1.0)),
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
  if (pitch >= 89.9f)
    pitch = 89.9f;
  if (pitch <= -65.0f)
    pitch = -65.0f;
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

void Camera::processKeyboardInput(float delta, MOVE_DIRECTION dir, std::vector<std::vector<float>>& hillsMap)
{
  float velocity = delta * moveSpeed;

  if (dir == FORWARD)
    moveCameraFrontAxial(true, velocity);
  else if (dir == BACKWARD)
    moveCameraFrontAxial(false, velocity);

  if (dir == RIGHT)
    Position += Right * velocity;
  else if (dir == LEFT)
    Position -= Right * velocity;
  if (dir == UP)
    Position += WorldUp * velocity;
  else if (dir == DOWN)
    Position -= WorldUp * velocity;

  const float WORLD_CAMERA_BORDER_OFFSET = 8.0f;
  if (Position.x > HALF_WORLD_WIDTH - WORLD_CAMERA_BORDER_OFFSET)
    Position.x = HALF_WORLD_WIDTH - WORLD_CAMERA_BORDER_OFFSET;
  else if (Position.x < -HALF_WORLD_WIDTH + WORLD_CAMERA_BORDER_OFFSET)
    Position.x = -HALF_WORLD_WIDTH + WORLD_CAMERA_BORDER_OFFSET;
  if (Position.z > HALF_WORLD_HEIGHT - WORLD_CAMERA_BORDER_OFFSET)
    Position.z = HALF_WORLD_HEIGHT - WORLD_CAMERA_BORDER_OFFSET;
  else if (Position.z < -HALF_WORLD_HEIGHT + WORLD_CAMERA_BORDER_OFFSET)
    Position.z = -HALF_WORLD_HEIGHT + WORLD_CAMERA_BORDER_OFFSET;

  const float WORLD_CAMERA_MIN_HEIGHT = 2.0f;
  const float WORLD_CAMERA_MIN_HEIGHT_ON_HILLS = WORLD_CAMERA_MIN_HEIGHT - HILLS_OFFSET_Y;
  if (Position.y < WORLD_CAMERA_MIN_HEIGHT)
    Position.y = WORLD_CAMERA_MIN_HEIGHT;
  if (hillsMap[Position.z + HALF_WORLD_HEIGHT][Position.x + HALF_WORLD_WIDTH] > 0 ||
      hillsMap[Position.z + HALF_WORLD_HEIGHT + 1][Position.x + HALF_WORLD_WIDTH] > 0 ||
      hillsMap[Position.z + HALF_WORLD_HEIGHT][Position.x + HALF_WORLD_WIDTH + 1] > 0 ||
      hillsMap[Position.z + HALF_WORLD_HEIGHT + 1][Position.x + HALF_WORLD_WIDTH + 1] > 0)
    {
      int x1 = std::floor(Position.x + HALF_WORLD_WIDTH);
      int x2 = std::round(Position.x + HALF_WORLD_WIDTH);
      int z1 = std::floor(Position.z + HALF_WORLD_HEIGHT);
      int z2 = std::round(Position.z + HALF_WORLD_HEIGHT);
      float xRatio = Position.x + HALF_WORLD_WIDTH - x1;
      float zRatio = Position.z + HALF_WORLD_HEIGHT - z1;
      float x1z1Height = hillsMap[z1][x1] + WORLD_CAMERA_MIN_HEIGHT_ON_HILLS;
      float x2z1Height = hillsMap[z1][x2] + WORLD_CAMERA_MIN_HEIGHT_ON_HILLS;
      float x1z2Height = hillsMap[z2][x1] + WORLD_CAMERA_MIN_HEIGHT_ON_HILLS;
      float x2z2Height = hillsMap[z2][x2] + WORLD_CAMERA_MIN_HEIGHT_ON_HILLS;
      Position.y = glm::max(Position.y,
                            glm::mix(glm::mix(x1z1Height, x1z2Height, zRatio),
                                     glm::mix(x2z1Height, x2z2Height, zRatio),
                                     xRatio));
    }
}

void Camera::switchFPSmode()
{
  FPSmode = !FPSmode;
}

bool Camera::getFPSmode() const
{
  return FPSmode;
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

void Camera::setPitch(float pitch)
{
  this->pitch = pitch;
}

void Camera::setYaw(float yaw)
{
  this->yaw = yaw;
}

void Camera::setPosition(float x, float y, float z)
{
  Position = glm::vec3(x, y, z);
}

glm::vec3 Camera::getFront() const
{
  return Front;
}

glm::vec3 Camera::getRight() const
{
  return Right;
}

glm::vec3 Camera::getUp() const
{
  return Up;
}

float Camera::getPitch()
{
  return pitch;
}

float Camera::getYaw()
{
  return yaw;
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

int Camera::getMapCoordX() const
{
  return glm::clamp((int)(WORLD_WIDTH + glm::clamp(Position.x, -(float)HALF_WORLD_WIDTH, (float)HALF_WORLD_WIDTH)) - HALF_WORLD_WIDTH, 0, WORLD_WIDTH - 1);
}

int Camera::getMapCoordZ() const
{
  return glm::clamp((int)(WORLD_HEIGHT + glm::clamp(Position.z, -(float)HALF_WORLD_HEIGHT, (float)HALF_WORLD_HEIGHT)) - HALF_WORLD_HEIGHT, 0, WORLD_HEIGHT - 1);
}

void Camera::moveCameraFrontAxial(bool forward, float velocity)
{
  if (!FPSmode)
    Position += forward ? Front * velocity : -Front * velocity;
  else
    {
      Position.x += forward ? Front.x * velocity : -Front.x * velocity;
      Position.z += forward ? Front.z * velocity : -Front.z * velocity;
    }
}
