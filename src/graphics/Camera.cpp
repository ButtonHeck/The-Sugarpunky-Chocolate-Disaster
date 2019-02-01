#include "graphics/Camera.h"

Camera::Camera(glm::vec3 position)
  :
    zoom(FOV),
    moveSpeed(8),
    mouseSensitivity(0.015f),
    useAcceleration(true),
    viewAccelerationSensitivity(0.001f),
    moveAccelerationSensitivity(0.05f),
    FPSmode(false),
    yaw(-90.0f),
    pitch(0.0f),
    position(position),
    front(glm::vec3(0.0, 0.0, -1.0)),
    worldUp(0.0, 1.0, 0.0)
{
  updateVectors();
}

Camera &Camera::operator=(const Camera &rhs)
{
  this->mouseSensitivity = rhs.mouseSensitivity;
  this->useAcceleration = rhs.useAcceleration;
  this->FPSmode = rhs.FPSmode;
  this->yaw = rhs.yaw;
  this->pitch = rhs.pitch;
  this->position = rhs.position;
  this->front = rhs.front;
  this->viewAccelerationX = rhs.viewAccelerationX;
  this->viewAccelerationY = rhs.viewAccelerationY;
  this->viewAccelerationSensitivity = rhs.viewAccelerationSensitivity;
  this->moveAccelerationSide = rhs.moveAccelerationSide;
  this->moveAccelerationFront = rhs.moveAccelerationFront;
  this->moveAccelerationY = rhs.moveAccelerationY;
  this->accumulateMoveFront = rhs.accumulateMoveFront;
  this->accumulateMoveSide = rhs.accumulateMoveSide;
  this->accumulateMoveY = rhs.accumulateMoveY;
  this->updateVectors();
  return *this;
}

glm::mat4 Camera::getViewMatrix() const
{
  return glm::lookAt(position, position + front, worldUp);
}

glm::mat4 Camera::getReflectionViewMatrix() const
{
  glm::vec3 positionReflected(position.x, -position.y, position.z);
  glm::vec3 lookDirectionReflected(position + front);
  lookDirectionReflected.y *= -1;
  return glm::lookAt(positionReflected, lookDirectionReflected, worldUp);
}

void Camera::updateViewAcceleration(float xOffset, float yOffset)
{
  viewAccelerationX += xOffset * mouseSensitivity;
  viewAccelerationY += yOffset * mouseSensitivity;
}

void Camera::processMouseCursor()
{
  BENCHMARK("Camera: processMouse", true);
  yaw -= viewAccelerationX;
  pitch -= viewAccelerationY;
  if (pitch >= 89.9f)
    pitch = 89.9f;
  if (pitch <= -65.0f)
    pitch = -65.0f;

  viewAccelerationX *= useAcceleration ? 0.85f : 0;
  viewAccelerationY *= useAcceleration ? 0.85f : 0;

  updateVectors();
}

void Camera::processMouseScroll(float yOffset)
{
  mouseSensitivity += yOffset * viewAccelerationSensitivity;
  mouseSensitivity = glm::clamp(mouseSensitivity, 0.002f, 0.02f);
}

void Camera::updateAccelerations(MOVE_DIRECTION dir)
{
  if (dir == FORWARD)
    {
      accumulateMoveFront = true;
      if (!useAcceleration)
        moveAccelerationFront = 1.0f;
      else
        {
          moveAccelerationFront += moveAccelerationSensitivity;
          moveAccelerationFront = glm::min(moveAccelerationFront, 1.0f);
        }
    }
  else if (dir == BACKWARD)
    {
      accumulateMoveFront = true;
      if (!useAcceleration)
        moveAccelerationFront = -1.0f;
      else
        {
          moveAccelerationFront -= moveAccelerationSensitivity;
          moveAccelerationFront = glm::max(moveAccelerationFront, -1.0f);
        }
    }

  if (dir == RIGHT)
    {
      accumulateMoveSide = true;
      if (!useAcceleration)
        moveAccelerationSide = 1.0f;
      else
        {
          moveAccelerationSide += moveAccelerationSensitivity;
          moveAccelerationSide = glm::min(moveAccelerationSide, 1.0f);
        }
    }
  else if (dir == LEFT)
    {
      accumulateMoveSide = true;
      if (!useAcceleration)
        moveAccelerationSide = -1.0f;
      else
        {
          moveAccelerationSide -= moveAccelerationSensitivity;
          moveAccelerationSide = glm::max(moveAccelerationSide, -1.0f);
        }
    }

  if (dir == UP)
    {
      accumulateMoveY = true;
      if (!useAcceleration)
        moveAccelerationY = 1.0f;
      else
        {
          moveAccelerationY += moveAccelerationSensitivity;
          moveAccelerationY = glm::min(moveAccelerationY, 1.0f);
        }
    }
  else if (dir == DOWN)
    {
      accumulateMoveY = true;
      if (!useAcceleration)
        moveAccelerationY = -1.0f;
      else
        {
          moveAccelerationY -= moveAccelerationSensitivity;
          moveAccelerationY = glm::max(moveAccelerationY, -1.0f);
        }
    }
}

void Camera::move(float delta, const map2D_f &hillsMap)
{
  BENCHMARK("Camera: move", true);
  float velocity = delta * moveSpeed;

  if (moveAccelerationFront != 0.0f)
    {
      if (!accumulateMoveFront)
        diminishMoveAcceleration(moveAccelerationFront);
      moveCameraFrontAxial(velocity);
    }

  if (moveAccelerationSide != 0.0f)
    {
      if (!accumulateMoveSide)
        diminishMoveAcceleration(moveAccelerationSide);
      glm::vec3 move = right * velocity * moveAccelerationSide;
      position += move;
    }

  if (moveAccelerationY != 0.0f)
    {
      if (!accumulateMoveY)
        diminishMoveAcceleration(moveAccelerationY);
      glm::vec3 move = worldUp * velocity * moveAccelerationY;
      position += move;
    }

  position.x = glm::clamp(position.x,
                          -HALF_WORLD_WIDTH + CAMERA_WORLD_BORDER_OFFSET,
                          HALF_WORLD_WIDTH - CAMERA_WORLD_BORDER_OFFSET);
  position.y = glm::clamp(position.y, CAMERA_WORLD_MIN_HEIGHT, CAMERA_WORLD_MAX_HEIGHT);
  position.z = glm::clamp(position.z,
                          -HALF_WORLD_HEIGHT + CAMERA_WORLD_BORDER_OFFSET,
                          HALF_WORLD_HEIGHT - CAMERA_WORLD_BORDER_OFFSET);

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

void Camera::switchAcceleration()
{
  useAcceleration = !useAcceleration;
}

void Camera::disableMoveAcceleration()
{
  accumulateMoveSide = accumulateMoveFront = accumulateMoveY = false;
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

void Camera::diminishMoveAcceleration(float &accelerationDirectionValue)
{
  if (!useAcceleration)
    {
      accelerationDirectionValue = 0.0f;
      return;
    }
  else
    {
      accelerationDirectionValue *= 0.92f;
      if (glm::abs(accelerationDirectionValue) <= 0.01f)
        accelerationDirectionValue = 0.0f;
    }
}

int Camera::getMapCoordX() const
{
  return glm::clamp((int)(WORLD_WIDTH + glm::clamp(position.x, -HALF_WORLD_WIDTH_F, HALF_WORLD_WIDTH_F)) - HALF_WORLD_WIDTH, 0, WORLD_WIDTH - 1);
}

int Camera::getMapCoordZ() const
{
  return glm::clamp((int)(WORLD_HEIGHT + glm::clamp(position.z, -HALF_WORLD_HEIGHT_F, HALF_WORLD_HEIGHT_F)) - HALF_WORLD_HEIGHT, 0, WORLD_HEIGHT - 1);
}

glm::vec2 Camera::getViewAcceleration() const
{
  if (useAcceleration)
    return glm::vec2(viewAccelerationX, viewAccelerationY);
  else
    return glm::vec2(20.0f);
}

void Camera::serialize(std::ofstream &output)
{
  output << std::setprecision(5);
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

void Camera::moveCameraFrontAxial(float velocity)
{
  float moveDistance = velocity * moveAccelerationFront;
  if (!FPSmode)
    {
      position += front * moveDistance;
    }
  else
    {
      position.x += front.x * moveDistance;
      position.z += front.z * moveDistance;
    }
}
