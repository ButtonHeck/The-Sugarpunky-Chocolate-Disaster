#include "Options.h"

Options::Options()
{
  //all the "...REQUEST" variables should initially be false
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
    {WATER_FC, true},
    {MULTISAMPLE_ENABLE, true}
  });
}

bool Options::get(OPTION value)
{
  return options[value].second;
}

void Options::set(OPTION value, bool flag)
{
  options[value].second = flag;
}

void Options::switchOpt(OPTION value)
{
  options[value].second = !options[value].second;
}
