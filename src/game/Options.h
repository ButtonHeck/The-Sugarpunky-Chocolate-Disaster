#ifndef OPTIONS_H
#define OPTIONS_H
#include <vector>

enum OPTION
{
  RENDER_SHADOW_ON_TREES = 0,
  RENDER_TREE_MODELS = 1,
  ANIMATE_WATER = 2,
  RENDER_DEBUG_TEXT = 3,
  RECREATE_TERRAIN_REQUEST = 4,
  SAVE_REQUEST = 5,
  LOAD_REQUEST = 6,
  SHOW_CURSOR = 7,
  SHOW_BUILDABLE = 8,
  MODELS_FC = 9,
  HILLS_FC = 10,
  WATER_FC = 11,
  MULTISAMPLE_ENABLE = 12,
  SHADOW_ENABLE = 13,
  CREATE_SHADOW_MAP_REQUEST = 14,
  RENDER_FLAT_TERRAIN = 15,
  RENDER_WATER = 16,
  OCCLUSION_CULLING = 17
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
