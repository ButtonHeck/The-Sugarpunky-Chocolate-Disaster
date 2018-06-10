#ifndef MODELMANAGER_H
#define MODELMANAGER_H
#include <vector>
#include <string>

class ModelManager
{
public:
  ModelManager();
  std::vector<std::string>& getPlainTreesPathsList();
  std::vector<std::string>& getHillTreesPathsList();
private:
  std::vector<std::string> plainTreesList;
  std::vector<std::string> hillTreesList;
};

#endif // MODELMANAGER_H
