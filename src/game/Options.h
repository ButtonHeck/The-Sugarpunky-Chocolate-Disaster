#ifndef OPTIONS_H
#define OPTIONS_H
#include <vector>

enum Option
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
  WATER_FC = 11
};

class Options
{
public:
  Options();
  bool get(Option value);
  void set(Option value, bool flag);
  void switchOpt(Option value);
private:
  std::vector<std::pair<Option, bool>> options;
};

#endif // OPTIONS_H
