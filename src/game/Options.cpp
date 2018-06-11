#include "Options.h"

Options::Options()
{
  options.assign(
  {
    {RENDER_SHADOW_ON_TREES, true},
    {RENDER_TREE_MODELS, true},
    {ANIMATE_WATER, true},
    {RENDER_DEBUG_TEXT, true},
    {RECREATE_TERRAIN_REQUEST, false},
    {SAVE_REQUEST, false},
    {LOAD_REQUEST, false},
    {SHOW_CURSOR, false},
    {SHOW_BUILDABLE, false},
    {MODELS_FC, true},
    {HILLS_FC, true},
    {WATER_FC, true}
  });
}

bool Options::get(Option value)
{
  return options[value].second;
}

void Options::set(Option value, bool flag)
{
  options[value].second = flag;
}

void Options::switchOpt(Option value)
{
  options[value].second = !options[value].second;
}
