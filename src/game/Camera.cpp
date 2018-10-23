#include "game/Camera.h"

Camera::Camera(glm::vec3 position)
  :
    zoom(FOV),
    moveSpeed(8),
    mouseSensitivity(0.04f),
    FPSmode(false),
    yaw(-90.0f),
    pitch(0.0f),
    position(position),
    front(glm::vec3(0.0, 0.0, -1.0)),
    worldUp(0.0, 1.0, 0.0)
{
  updateVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
  return glm::lookAt(position, position + front, worldUp);
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
    position += right * velocity;
  else if (dir == LEFT)
    position -= right * velocity;
  if (dir == UP)
    position += worldUp * velocity;
  else if (dir == DOWN)
    position -= worldUp * velocity;

  if (position.x > HALF_WORLD_WIDTH - CAMERA_WORLD_BORDER_OFFSET)
    position.x = HALF_WORLD_WIDTH - CAMERA_WORLD_BORDER_OFFSET;
  else if (position.x < -HALF_WORLD_WIDTH + CAMERA_WORLD_BORDER_OFFSET)
    position.x = -HALF_WORLD_WIDTH + CAMERA_WORLD_BORDER_OFFSET;
  if (position.z > HALF_WORLD_HEIGHT - CAMERA_WORLD_BORDER_OFFSET)
    position.z = HALF_WORLD_HEIGHT - CAMERA_WORLD_BORDER_OFFSET;
  else if (position.z < -HALF_WORLD_HEIGHT + CAMERA_WORLD_BORDER_OFFSET)
    position.z = -HALF_WORLD_HEIGHT + CAMERA_WORLD_BORDER_OFFSET;

  if (position.y < CAMERA_WORLD_MIN_HEIGHT)
    position.y = CAMERA_WORLD_MIN_HEIGHT;
  if (position.y > CAMERA_WORLD_MAX_HEIGHT)
    position.y = CAMERA_WORLD_MAX_HEIGHT;

  if (hillsMap[position.z + HALF_WORLD_HEIGHT][position.x + HALF_WORLD_WIDTH] > 0 ||
      hillsMap[position.z + HALF_WORLD_HEIGHT + 1][position.x + HALF_WORLD_WIDTH] > 0 ||
      hillsMap[position.z + HALF_WORLD_HEIGHT][position.x + HALF_WORLD_WIDTH + 1] > 0 ||
      hillsMap[position.z + HALF_WORLD_HEIGHT + 1][position.x + HALF_WORLD_WIDTH + 1] > 0)
    {
      int x1 = std::floor(position.x + HALF_WORLD_WIDTH);
      int x2 = std::round(position.x + HALF_WORLD_WIDTH);
      int z1 = std::floor(position.z + HALF_WORLD_HEIGHT);
      int z2 = std::round(position.z + HALF_WORLD_HEIGHT);
      float xRatio = position.x + HALF_WORLD_WIDTH - x1;
      float zRatio = position.z + HALF_WORLD_HEIGHT - z1;
      float x1z1Height = hillsMap[z1][x1] + CAMERA_WORLD_MIN_HEIGHT_ON_HILLS;
      float x2z1Height = hillsMap[z1][x2] + CAMERA_WORLD_MIN_HEIGHT_ON_HILLS;
      float x1z2Height = hillsMap[z2][x1] + CAMERA_WORLD_MIN_HEIGHT_ON_HILLS;
      float x2z2Height = hillsMap[z2][x2] + CAMERA_WORLD_MIN_HEIGHT_ON_HILLS;
      position.y = glm::max(position.y,
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
  return position;
}

glm::vec3 Camera::getDirection() const
{
  return front;
}

void Camera::setYaw(float newYaw)
{
  this->yaw = newYaw;
  updateVectors();
}

void Camera::setPitch(float newPitch)
{
  pitch = newPitch;
  updateVectors();
}

glm::vec3 Camera::getRight() const
{
  return right;
}

glm::vec3 Camera::getUp() const
{
  return up;
}

void Camera::updateVectors()
{
  float x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  float y = std::sin(glm::radians(pitch));
  float z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  glm::vec3 newFront(x,y,z);
  front = glm::normalize(newFront);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

int Camera::getMapCoordX() const
{
  return glm::clamp((int)(WORLD_WIDTH + glm::clamp(position.x, -HALF_WORLD_WIDTH_F, HALF_WORLD_WIDTH_F)) - HALF_WORLD_WIDTH, 0, WORLD_WIDTH - 1);
}

int Camera::getMapCoordZ() const
{
  return glm::clamp((int)(WORLD_HEIGHT + glm::clamp(position.z, -HALF_WORLD_HEIGHT_F, HALF_WORLD_HEIGHT_F)) - HALF_WORLD_HEIGHT, 0, WORLD_HEIGHT - 1);
}

void Camera::serialize(std::ofstream &output)
{
  output << position.x << " ";
  output << position.y << " ";
  output << position.z << " ";
  output << pitch << " ";
  output << yaw << " ";
}

void Camera::deserialize(std::ifstream &input)
{
  input >> position.x >> position.y >> position.z >> pitch >> yaw;
  updateVectors();
}

void Camera::moveCameraFrontAxial(bool forward, float velocity)
{
  if (!FPSmode)
    position += forward ? front * velocity : -front * velocity;
  else
    {
      position.x += forward ? front.x * velocity : -front.x * velocity;
      position.z += forward ? front.z * velocity : -front.z * velocity;
    }
}
