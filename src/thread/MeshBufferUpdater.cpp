#include "MeshBufferUpdater.h"

MeshBufferUpdater::MeshBufferUpdater(GLFWwindow *window, Camera &camera, PlantGenerator *plantGenerator, Frustum &viewFrustum)
  :
    Thread(),
    window(window),
    camera(camera),
    plantGenerator(plantGenerator),
    frustum(viewFrustum)
{
  thread = new std::thread(&MeshBufferUpdater::updateMeshIndirectBuffer, this);
}

void MeshBufferUpdater::setDataReady(bool ready)
{
  ready = ready;
}

void MeshBufferUpdater::setDataNeed(bool need)
{
  newDataNeed = need;
}

bool MeshBufferUpdater::waitFor() const
{
  return (!ready && newDataNeed);
}

void MeshBufferUpdater::updateMeshIndirectBuffer()
{
  auto& plainPlants = plantGenerator->getPlainPlants();
  auto& hillTrees = plantGenerator->getHillTrees();
  auto& plainChunks = plantGenerator->getPlainPlantsModelChunks();
  auto& hillChunks = plantGenerator->getHillTreeModelChunks();
  while(!glfwWindowShouldClose(window))
    {
      if (newDataNeed)
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
          ready = true;
          newDataNeed = false;
        }
      std::this_thread::yield();
    }
}
