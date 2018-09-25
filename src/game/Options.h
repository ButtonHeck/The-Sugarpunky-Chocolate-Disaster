#ifndef OPTIONS_H
#define OPTIONS_H
#include <vector>

enum OPTION
{
  OPT_TREES_SHADOW_EMPHASIZE = 0,
  OPT_DRAW_TREES = 1,
  OPT_ANIMATE_WATER = 2,
  OPT_DRAW_DEBUG_TEXT = 3,
  OPT_RECREATE_TERRAIN_REQUEST = 4,
  OPT_SAVE_REQUEST = 5,
  OPT_LOAD_REQUEST = 6,
  OPT_SHOW_CURSOR = 7,
  OPT_DRAW_BUILDABLE = 8,
  OPT_MODELS_CULLING = 9,
  OPT_HILLS_CULLING = 10,
  OPT_WATER_CULLING = 11,
  OPT_USE_MULTISAMPLING = 12,
  OPT_USE_SHADOWS = 13,
  OPT_CREATE_SHADOW_MAP_REQUEST = 14,
  OPT_DRAW_FLAT_TERRAIN = 15,
  OPT_DRAW_WATER = 16,
  OPT_MODELS_FLAT_BLENDING = 17,
  OPT_MODELS_PHONG_SHADING = 18,
  OPT_POLYGON_LINE = 19
};

class Options
{
public:
  Options();
  bool get(OPTION value);
  void set(OPTION value, bool flag);
  void switchOpt(OPTION value);
private:
  std::vector<std::pair<OPTION, bool>> options;
};

#endif // OPTIONS_H
