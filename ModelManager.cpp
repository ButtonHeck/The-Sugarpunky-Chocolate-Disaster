#include "ModelManager.h"

ModelManager::ModelManager()
{
  plainTreesList = {"/models/tree1/tree1.obj",
                    "/models/tree2/tree2.obj",
                    "/models/tree3/tree3.obj"};
  hillTreesList = {"/models/hillTree1/hillTree1.obj",
                   "/models/hillTree2/hillTree2.obj",
                   "/models/hillTree3/hillTree3.obj"};
}

std::vector<std::string> &ModelManager::getPlainTreesPathsList()
{
  return plainTreesList;
}

std::vector<std::string> &ModelManager::getHillTreesPathsList()
{
  return hillTreesList;
}
