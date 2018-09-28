#include "MeshBufferUpdateThread.h"

MeshBufferUpdateThread::MeshBufferUpdateThread(GLFWwindow *window, Camera &camera, PlantGenerator *plantGenerator, Frustum &viewFrustum)
  :
    Thread(),
    window(window),
    camera(camera),
    plantGenerator(plantGenerator),
    frustum(viewFrustum)
{
  thread = new std::thread(&MeshBufferUpdateThread::updateMeshIndirectBuffer, this);
}

void MeshBufferUpdateThread::setDataReady(bool ready)
{
  meshesIndirectDataReady = ready;
}

void MeshBufferUpdateThread::setDataNeed(bool need)
{
  meshesIndirectDataNeed = need;
}

bool MeshBufferUpdateThread::waitFor() const
{
  return (!meshesIndirectDataReady && meshesIndirectDataNeed);
}

void MeshBufferUpdateThread::updateMeshIndirectBuffer()
{
  auto& plainPlants = plantGenerator->getPlainPlants();
  auto& hillTrees = plantGenerator->getHillTrees();
  auto& plainChunks = plantGenerator->getPlainPlantsModelChunks();
  auto& hillChunks = plantGenerator->getHillTreeModelChunks();
  while(!glfwWindowShouldClose(window))
    {
      if (meshesIndirectDataNeed)
        {
          BENCHMARK("(ST)Model: update meshes DIBs data", true);
          float cameraOnMapX = glm::clamp(camera.getPosition().x, -(float)HALF_WORLD_WIDTH, (float)HALF_WORLD_WIDTH);
          float cameraOnMapZ = glm::clamp(camera.getPosition().z, -(float)HALF_WORLD_HEIGHT, (float)HALF_WORLD_HEIGHT);
          glm::vec2 cameraPositionXZ = glm::vec2(cameraOnMapX, cameraOnMapZ);
          for (unsigned int i = 0; i < plainPlants.size(); i++)
            {
              Model& model = plainPlants[i];
              model.prepareMeshesIndirectData(plainChunks, i, cameraPositionXZ, frustum);
            }
          for (unsigned int i = 0; i < hillTrees.size(); i++)
            {
              Model& model = hillTrees[i];
              model.prepareMeshesIndirectData(hillChunks, i, cameraPositionXZ, frustum);
            }
          meshesIndirectDataReady = true;
          meshesIndirectDataNeed = false;
        }
      std::this_thread::yield();
    }
}
