#ifndef MESHBUFFERUPDATETHREAD_H
#define MESHBUFFERUPDATETHREAD_H
#include "game/Thread.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "generator/PlantGenerator.h"
#include "graphics/Frustum.h"
#include "graphics/Camera.h"

class MeshBufferUpdateThread : public Thread
{
public:
  MeshBufferUpdateThread(GLFWwindow* window, Camera& camera, PlantGenerator* plantGenerator, Frustum& viewFrustum);
  ~MeshBufferUpdateThread() = default;
  void setDataReady(bool ready);
  void setDataNeed(bool need);
  bool waitFor() const;
private:
  void updateMeshIndirectBuffer();
  GLFWwindow* window;
  Camera& camera;
  PlantGenerator* plantGenerator;
  Frustum& frustum;
  volatile bool meshesIndirectDataReady = false, meshesIndirectDataNeed = false;
};

#endif // MESHBUFFERUPDATETHREAD_H
