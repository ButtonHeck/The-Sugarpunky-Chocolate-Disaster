#include "Camera"
#include "BenchmarkTimer"

#include <iomanip>

Camera::Camera(glm::vec3 position)
  :
    zoom(FOV),
    moveSpeed(MOVE_SPEED),
    mouseSensitivity(INITIAL_MOUSE_SENSITIVITY),
    useAcceleration(true),
    viewAccelerationSensitivity(INITIAL_VIEW_ACCELERATION_SENSITIVITY),
    moveAccelerationSensitivity(INITIAL_MOVE_ACCELERATION_SENSITIVITY),
    yaw(INITIAL_YAW_ANGLE_DEGREES),
    position(position)
{
  updateDirectionVectors();
}

Camera &Camera::operator=(const Camera &rhs)
{
  this->mouseSensitivity = rhs.mouseSensitivity;
  this->useAcceleration = rhs.useAcceleration;
  this->firstPersonShooterMode = rhs.firstPersonShooterMode;
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
  this->accumulateMoveHorizontal = rhs.accumulateMoveHorizontal;
  this->accumulateMoveVertical = rhs.accumulateMoveVertical;
  this->updateDirectionVectors();
  return *this;
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
      if (!accumulateMoveHorizontal)
        diminishMoveAcceleration(moveAccelerationSide);
      glm::vec3 move = right * velocity * moveAccelerationSide;
      position += move;
    }

  if (moveAccelerationY != 0.0f)
    {
      if (!accumulateMoveVertical)
        diminishMoveAcceleration(moveAccelerationY);
      glm::vec3 move = WORLD_UP * velocity * moveAccelerationY;
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
  if (pitch >= MAX_PITCH)
    pitch = MAX_PITCH;
  if (pitch <= MIN_PITCH)
    pitch = MIN_PITCH;

  viewAccelerationX *= useAcceleration ? VIEW_ACCELERATION_DAMPENING_FACTOR : 0;
  viewAccelerationY *= useAcceleration ? VIEW_ACCELERATION_DAMPENING_FACTOR : 0;

  updateDirectionVectors();
}

void Camera::processMouseScroll(float yOffset)
{
  mouseSensitivity += yOffset * viewAccelerationSensitivity;
  mouseSensitivity = glm::clamp(mouseSensitivity, MIN_MOUSE_SENSITIVITY, MAX_MOUSE_SENSITIVITY);
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
      accumulateMoveHorizontal = true;
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
      accumulateMoveHorizontal = true;
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
      accumulateMoveVertical = true;
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
      accumulateMoveVertical = true;
      if (!useAcceleration)
        moveAccelerationY = -1.0f;
      else
        {
          moveAccelerationY -= moveAccelerationSensitivity;
          moveAccelerationY = glm::max(moveAccelerationY, -1.0f);
        }
    }
}

void Camera::setYaw(float newYaw)
{
  yaw = newYaw;
  updateDirectionVectors();
}

void Camera::setPitch(float newPitch)
{
  pitch = newPitch;
  updateDirectionVectors();
}

void Camera::switchFPSmode()
{
  firstPersonShooterMode = !firstPersonShooterMode;
}

void Camera::switchAcceleration()
{
  useAcceleration = !useAcceleration;
}

void Camera::disableMoveAcceleration()
{
  accumulateMoveHorizontal = accumulateMoveFront = accumulateMoveVertical = false;
}

glm::mat4 Camera::getViewMatrix() const
{
  return glm::lookAt(position, position + front, WORLD_UP);
}

glm::mat3 Camera::getViewMatrixMat3() const
{
  return glm::mat3(getViewMatrix());
}

glm::mat4 Camera::getReflectionViewMatrix() const
{
  glm::vec3 positionReflected(position.x, -position.y, position.z);
  glm::vec3 lookDirectionReflected(position + front);
  lookDirectionReflected.y *= -1;
  return glm::lookAt(positionReflected, lookDirectionReflected, WORLD_UP);
}

float Camera::getZoom() const
{
  return zoom;
}

const glm::vec3 &Camera::getPosition() const
{
  return position;
}

const glm::vec3 &Camera::getDirection() const
{
  return front;
}

const glm::vec3 &Camera::getRight() const
{
  return right;
}

const glm::vec3 &Camera::getUp() const
{
  return up;
}

glm::vec2 Camera::getWorldCoordinates() const
{
  using glm::clamp;
  return glm::vec2(clamp((int)(WORLD_WIDTH + clamp(position.x, -HALF_WORLD_WIDTH_F, HALF_WORLD_WIDTH_F)) - HALF_WORLD_WIDTH, 0, WORLD_WIDTH - 1),
                   clamp((int)(WORLD_HEIGHT + clamp(position.z, -HALF_WORLD_HEIGHT_F, HALF_WORLD_HEIGHT_F)) - HALF_WORLD_HEIGHT, 0, WORLD_HEIGHT - 1));
}

glm::vec2 Camera::getViewAcceleration() const
{
  return useAcceleration ? glm::vec2(viewAccelerationX, viewAccelerationY) : glm::vec2(DEFAULT_VIEW_ACCELERATION);
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
  updateDirectionVectors();
}

void Camera::updateDirectionVectors()
{
  float x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  float y = std::sin(glm::radians(pitch));
  float z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
  glm::vec3 newFront(x,y,z);
  front = glm::normalize(newFront);
  right = glm::normalize(glm::cross(front, WORLD_UP));
  /* for "up" vector no need to normalize it explicitly
   * as the cross product of 2 orthogonal unit vectors (front and WORLD_UP) is already a unit length vector
   */
  up = glm::cross(right, front);
}

void Camera::diminishMoveAcceleration(float &directionAccelerationValue)
{
  if (!useAcceleration)
    {
      directionAccelerationValue = 0.0f;
      return;
    }
  else
    {
      directionAccelerationValue *= MOVE_ACCELERATION_DAMPENING_FACTOR;
      if (glm::abs(directionAccelerationValue) <= 0.01f)
        directionAccelerationValue = 0.0f;
    }
}

void Camera::moveCameraFrontAxial(float velocity)
{
  float moveDistance = velocity * moveAccelerationFront;
  if (!firstPersonShooterMode)
    position += front * moveDistance;
  else
    {
      position.x += front.x * moveDistance;
      position.z += front.z * moveDistance;
    }
}
