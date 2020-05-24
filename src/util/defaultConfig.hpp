#pragma once
#include <string_view>

constexpr std::string_view SUGARPUNK_DEFAULT_CONFIG = 
R"(# visual quality settings
[GRAPHICS]
# default = 0.3
near_plane<f>=0.3
# default = 500.0
far_plane<f>=500.0
# default = true
hdr<b>=true
# applied only if hdr is true, default = 2.2
hdr_exposure<f>=2.2
# applied only if multisampling is enabled, default = 4
multisamples<i>=4
# default = 2.0
anisotropy<f>=2.0
# applied only if DOF is enabled, default = 0.17
dof_distance_linear<f>=0.17
# default = 4096
depthmap_texture_width<i>=4096
# default = 4096
depthmap_texture_height<i>=4096
# default = 400
frame_water_reflection_width<i>=400
# default = 300
frame_water_reflection_height<i>=300
# default = equals to frame_water_reflection_width
frame_water_refraction_width<i>=400
# default - equals to frame_water_reflection_height
frame_water_refraction_height<i>=300
# default = 20.0
shadow_distance_layer1<f>=20.0
# default = 60.0
shadow_distance_layer2<f>=60.0

# settings applied to scene configuration and terrain generating algorithms
# IMPORTANT: changing some of these values may lead to visual discrepancies, so make sure you understand what you do
[SCENE]
# you better not touch this one (or at least do not set this value >=-1.0), default = -1.0
water_level<f>=-1.0
# minimal width of the river, default = 5
river_width_base<i>=5
# bizarre river generation mode, default = false
river_generation_bizarre_mode<b>=false
# height of the underwater tile, default = -4.0
underwater_level<f>=-4.0
# speed of the stars skysphere move, default = 0.045
planet_move_speed<f>=0.045
# this one is unfortunately poorly named - the higher this value the more sparsely plants distributed on a map, default = 8
plants_distribution_freq<i>=8
# this one supposed to make shore more smooth, but in practice it only makes the river wider, so keep it 5 by default
shore_smooth_cycles<i>=5

# camera settings that supposed to be not strictly constant, but should not be changed during game loop
[CAMERA]
# default = 2.0
min_height<f>=2.0
# default = 30.0
max_height<f>=30.0
# default = 60.0
fov<f>=60.0
# default = 8.0
move_speed<f>=8.0
# sensitivity is clamped in range [0.002; 0.02], default = 0.015
initial_mouse_sensitivity<f>=0.015
# the more this value is - the more inertly the camera view vector moves, default = 0.85
view_acceleration_dampening_factor<f>=0.85
# the more this value is - the more inertly camera itself moves, default = 0.92
move_acceleration_dampening_factor<f>=0.92
# defines how much mouse wheel changes view acceleration, default = 0.001
initial_view_acceleration_sensitivity<f>=0.001
# defines how 'fast' camera starts to move, default = 0.05
initial_move_acceleration_sensitivity<f>=0.05

# settings for grass generating process
[GRASS]
# default = 0.189
min_scale<f>=0.189
# default = 0.224
max_scale<f>=0.224

# settings for hill models generating process
[HILL_TREES]
# default = 0.38
min_scale_trees<f>=0.38
# default = 0.51
max_scale_trees<f>=0.51
# rocks size are predefined by the trees scale factor multiplied by this value, default = 0.75
rocks_scale_multiplier<f>=0.75
# defines how much offset (min) might be applied to the position vector, default = -0.6
min_position_offset<f>=-0.6
# defines how much offset (max) might be applied to the position vector, default = 0.6
max_position_offset<f>=0.6
# defines how much rotation offset (min) might be applied to the rotation part of the transform matrix, default = -0.05
min_rotation_offset<f>=-0.05
# defines how much rotation offset (max) might be applied to the rotation part of the transform matrix, default = 0.05
max_rotation_offset<f>=0.05
# defines maximum slope of a hill tile (difference between highest and lowest points of the tile)
# above which a tile should not contain any 'tree' model, default = 1.0
max_surface_slope_for_trees<f>=1.0
# defines maximum slope of a hill tile (difference between highest and lowest points of the tile)
# above which a tile should not contain any 'rock' model, default = 2.6
max_surface_slope_for_rocks<f>=2.6

# settings for land trees generating process
[LAND_TREES]
# default = 0.27
min_scale<f>=0.27
# default = 0.32
max_scale<f>=0.32
# defines how much offset (min) might be applied to the position vector, default = -0.25
min_position_offset<f>=-0.25
# defines how much offset (max) might be applied to the position vector, default = 0.25
max_position_offset<f>=0.25

# general settings for plants generator chunks management process
[PLANT_GENERATOR]
# number of chunks nearby camera that contain full-poly models, default = 8
loading_distance_chunks<i>=8
# number of chunks nearby camera that contain low-poly models(excluding number of chunks for high-poly ones), default = 32
loading_distance_chunks_lowpoly<i>=32
# number of chunks nearby camera that contain shadow casters data, default = 16
loading_distance_chunks_shadow<i>=16

# settings for hills generating process
[HILLS_GENERATOR]
# number of cycles of kernel hill tiles fattening process (dense type), default = 12
dense_cycles<i>=12
# number of cycles of kernel hill tiles fattening process (thin type), default = 6
thin_cycles<i>=6

# shader settings that are technically uniforms, but set only once during game initialization stage
[SHADERS]
# value of the ambient color impact to the terrain (day), default = 0.08
u_ambient_day_terrain<f>=0.08
# value of the ambient color impact to the plants models (day), default = 0.16
u_ambient_day_plants<f>=0.16
# value of the ambient color impact to the terrain (night), default = 0.03
u_ambient_night_terrain<f>=0.03
# value of the ambient color impact to the plants models (night), default = equals to u_ambient_night_terrain
u_ambient_night_plants<f>=0.03
# bias values for different geometry types (both terrain and models)
# default = 3.0
hills_bias<f>=3.0
# default = 1.0
shore_bias<f>=1.0
# default = 5.0
land_bias<f>=5.0
# default = 4.0
water_bias<f>=4.0
# default = 8.0
models_bias<f>=8.0)";
