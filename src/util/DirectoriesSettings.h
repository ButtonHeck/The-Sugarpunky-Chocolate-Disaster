#ifndef DIRECTORIESSETTINGS
#define DIRECTORIESSETTINGS
#include <string>
#include <direct.h>

const std::string getResourcesDirectory();
const std::string RES_DIR = getResourcesDirectory() + "/res/";
const std::string SHADER_DIR = RES_DIR + "shaders/";
const std::string TEXTURES_DIR = RES_DIR + "textures/";
const std::string MODELS_DIR = RES_DIR + "models/";
const std::string FONT_DIR = RES_DIR + "fonts/";
const std::string SAVES_DIR = RES_DIR + "saves/";

#endif // DIRECTORIESSETTINGS

