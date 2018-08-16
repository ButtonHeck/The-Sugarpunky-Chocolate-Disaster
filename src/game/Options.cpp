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
    {HILLS_FC, false},
    {WATER_FC, false},
    {MULTISAMPLE_ENABLE, true},
    {SHADOW_ENABLE, true},
    {CREATE_SHADOW_MAP_REQUEST, true},
    {RENDER_FLAT_TERRAIN, true},
    {RENDER_WATER, true},
    {OCCLUSION_CULLING, false}
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
