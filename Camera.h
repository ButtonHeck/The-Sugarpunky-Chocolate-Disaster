#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Movement_Direction
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
  void processKeyboardInput(float delta, Movement_Direction dir);
  void setFPSmode(bool on);
  float getZoom() const;
  glm::vec3 getPosition() const;
  glm::vec3 getDirection() const;
  void setPitch(float pitch);
  void setYaw(float yaw);
  glm::vec3 getFront() const;
  glm::vec3 getRight() const;
  glm::vec3 getUp() const;
  float getPitch();
  float getYaw();
private:
  void updateVectors();
  //options
  float zoom, moveSpeed, mouseSensitivity;
  bool FPSmode;
  //Euler angles
  float yaw, pitch;
  //direction vectors
  glm::vec3 Position, Front, Right, Up, WorldUp;
};

#endif // CAMERA_H
