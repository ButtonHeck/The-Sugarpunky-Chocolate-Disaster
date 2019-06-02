#include "DirectoriesSettings.h"

const std::string getResourcesDirectory()
{
  char cwd_buffer[FILENAME_MAX];
  getcwd(cwd_buffer, FILENAME_MAX);
  std::string cwd(cwd_buffer);
  return cwd.substr(0, cwd.find_last_of("/"));
}
