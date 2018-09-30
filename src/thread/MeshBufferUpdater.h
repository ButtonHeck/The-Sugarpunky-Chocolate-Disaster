#ifndef MESHBUFFERUPDATER_H
#define MESHBUFFERUPDATER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "thread/Thread.h"
#include "generator/PlantGenerator.h"
#include "graphics/Frustum.h"
#include "graphics/Camera.h"

class MeshBufferUpdater : public Thread
{
public:
  MeshBufferUpdater(GLFWwindow* window, Camera& camera, PlantGenerator* plantGenerator, Frustum& viewFrustum);
  ~MeshBufferUpdater() = default;
  void setDataReady(bool ready);
  void setDataNeed(bool need);
  bool waitFor() const;
private:
  void updateMeshIndirectBuffer();
  GLFWwindow* window;
  Camera& camera;
  PlantGenerator* plantGenerator;
  Frustum& frustum;
  volatile bool ready = false, newDataNeed = false;
};

#endif // MESHBUFFERUPDATER_H
