TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/main.cpp \
    src/game/Game.cpp \
    src/game/SaveLoadManager.cpp \
    src/game/Options.cpp \
    src/graphics/Frustum.cpp \
    src/game/world/models/Mesh.cpp \
    src/game/world/models/Model.cpp \
    src/input/KeyboardManager.cpp \
    src/input/MouseInputManager.cpp \
    src/game/world/models/plants/GrassGenerator.cpp \
    src/game/world/models/plants/HillTreesGenerator.cpp \
    src/game/world/models/plants/LandPlantsGenerator.cpp \
    src/game/world/models/plants/PlantGenerator.cpp \
    src/game/world/terrain/Generator.cpp \
    src/util/BenchmarkTimer.cpp \
    src/game/world/models/ModelChunk.cpp \
    src/game/world/terrain/TerrainTile.cpp \
    src/graphics/ScreenResolution.cpp \
    src/graphics/Camera.cpp \
    src/game/world/Chunk.cpp \
    src/util/VRAM_Monitor.cpp \
    src/game/world/ambience/theSun/TheSun.cpp \
    src/util/Logger.cpp \
    src/game/world/terrain/hills/HillsGenerator.cpp \
    src/game/world/terrain/hills/HillsFacade.cpp \
    src/game/world/terrain/hills/HillsShader.cpp \
    src/game/world/terrain/hills/HillsRenderer.cpp \
    src/game/world/terrain/shore/ShoreGenerator.cpp \
    src/game/world/terrain/shore/ShoreFacade.cpp \
    src/game/world/terrain/shore/ShoreShader.cpp \
    src/game/world/terrain/shore/ShoreRenderer.cpp \
    src/game/world/terrain/buildable/BuildableGenerator.cpp \
    src/game/world/terrain/buildable/BuildableFacade.cpp \
    src/game/world/terrain/buildable/BuildableShader.cpp \
    src/game/world/terrain/buildable/BuildableRenderer.cpp \
    src/game/world/terrain/underwater/UnderwaterGenerator.cpp \
    src/game/world/terrain/underwater/UnderwaterFacade.cpp \
    src/game/world/terrain/underwater/UnderwaterShader.cpp \
    src/game/world/terrain/underwater/UnderwaterRenderer.cpp \
    src/game/world/terrain/land/LandGenerator.cpp \
    src/game/world/terrain/land/LandFacade.cpp \
    src/game/world/terrain/land/LandRenderer.cpp \
    src/game/world/terrain/land/LandShader.cpp \
    src/game/world/terrain/water/WaterGenerator.cpp \
    src/game/world/terrain/water/WaterFacade.cpp \
    src/game/world/terrain/water/WaterRenderer.cpp \
    src/game/world/terrain/water/WaterShader.cpp \
    src/game/world/terrain/land/LandChunk.cpp \
    src/game/world/ambience/skybox/Skybox.cpp \
    src/game/world/ambience/skybox/SkyboxFacade.cpp \
    src/game/world/ambience/skybox/SkyboxShader.cpp \
    src/game/world/ambience/skybox/SkyboxRenderer.cpp \
    src/game/world/ambience/theSun/TheSunFacade.cpp \
    src/game/world/ambience/theSun/TheSunShader.cpp \
    src/game/world/ambience/theSun/TheSunRenderer.cpp \
    src/game/world/models/plants/PlantsFacade.cpp \
    src/game/world/models/plants/PlantsShader.cpp \
    src/graphics/RendererStateManager.cpp \
    src/game/world/Scene.cpp \
    src/game/world/models/plants/GrassRenderer.cpp \
    src/game/world/models/plants/TreesRenderer.cpp \
    src/graphics/ShadowVolume.cpp \
    src/util/DirectoriesSettings.cpp \
    src/game/world/ambience/lensFlare/LensFlareElement.cpp \
    src/game/world/ambience/lensFlare/LensFlareFacade.cpp \
    src/game/world/ambience/lensFlare/LensFlareRenderer.cpp \
    src/graphics/openglObjects/Framebuffer.cpp \
    src/graphics/openglObjects/Query.cpp \
    src/graphics/openglObjects/BufferCollection.cpp \
    src/graphics/textures/BindlessTextureManager.cpp \
    src/graphics/textures/TextureLoader.cpp \
    src/graphics/textures/TextureManager.cpp \
    src/graphics/shaders/Shader.cpp \
    src/graphics/shaders/ShaderManager.cpp \
    src/graphics/framebuffers/DepthmapFramebuffer.cpp \
    src/graphics/framebuffers/ScreenFramebuffer.cpp \
    src/graphics/framebuffers/WaterReflectionFramebuffer.cpp \
    src/graphics/framebuffers/WaterRefractionFramebuffer.cpp \
    src/graphics/gui/FontLoader.cpp \
    src/graphics/gui/TextManager.cpp \
    src/graphics/gui/CoordinateSystemRenderer.cpp \
    src/graphics/gui/FrustumRenderer.cpp \
    src/graphics/gui/ShadowVolumeRenderer.cpp \
    src/game/world/models/hemisphere/HemisphereFacade.cpp \
    src/game/world/models/hemisphere/HemisphereShader.cpp \
    src/game/world/models/hemisphere/Hemisphere.cpp

HEADERS += \
    src/game/Game.h \
    src/game/SaveLoadManager.h \
    src/game/Options.h \
    src/graphics/Frustum.h \
    src/game/world/models/Mesh.h \
    src/game/world/models/Model.h \
    src/input/KeyboardManager.h \
    src/input/MouseInputManager.h \
    src/generator/PlantGenerator.h \
    src/game/world/models/plants/GrassGenerator.h \
    src/game/world/models/plants/HillTreesGenerator.h \
    src/game/world/models/plants/LandPlantsGenerator.h \
    src/game/world/models/plants/PlantGenerator.h \
    src/game/world/terrain/Generator.h \
    src/util/BenchmarkTimer.h \
    src/util/Timer.h \
    src/game/world/models/ModelChunk.h \
    src/game/world/terrain/TerrainTile.h \
    src/graphics/ScreenResolution.h \
    src/graphics/Camera.h \
    src/game/world/Chunk.h \
    src/util/VRAM_Monitor.h \
    src/game/world/ambience/theSun/TheSun.h \
    src/util/Logger.h \
    src/game/world/terrain/hills/HillsGenerator.h \
    src/game/world/terrain/hills/HillsFacade.h \
    src/game/world/terrain/hills/HillsShader.h \
    src/game/world/terrain/hills/HillsRenderer.h \
    src/game/world/terrain/shore/ShoreGenerator.h \
    src/game/world/terrain/shore/ShoreFacade.h \
    src/game/world/terrain/shore/ShoreShader.h \
    src/game/world/terrain/shore/ShoreRenderer.h \
    src/game/world/terrain/buildable/BuildableGenerator.h \
    src/game/world/terrain/buildable/BuildableFacade.h \
    src/game/world/terrain/buildable/BuildableShader.h \
    src/game/world/terrain/buildable/BuildableRenderer.h \
    src/game/world/terrain/underwater/UnderwaterGenerator.h \
    src/game/world/terrain/underwater/UnderwaterFacade.h \
    src/game/world/terrain/underwater/UnderwaterShader.h \
    src/game/world/terrain/underwater/UnderwaterRenderer.h \
    src/game/world/terrain/land/LandGenerator.h \
    src/game/world/terrain/land/LandFacade.h \
    src/game/world/terrain/land/LandRenderer.h \
    src/game/world/terrain/land/LandShader.h \
    src/game/world/terrain/water/WaterGenerator.h \
    src/game/world/terrain/water/WaterFacade.h \
    src/game/world/terrain/water/WaterRenderer.h \
    src/game/world/terrain/water/WaterShader.h \
    src/game/world/terrain/land/LandChunk.h \
    src/game/world/ambience/skybox/Skybox.h \
    src/game/world/ambience/skybox/SkyboxFacade.h \
    src/game/world/ambience/skybox/SkyboxShader.h \
    src/game/world/ambience/skybox/SkyboxRenderer.h \
    src/game/world/ambience/theSun/TheSunFacade.h \
    src/game/world/ambience/theSun/TheSunShader.h \
    src/game/world/ambience/theSun/TheSunRenderer.h \
    src/game/world/models/plants/PlantsFacade.h \
    src/game/world/models/plants/PlantsShader.h \
    src/graphics/RendererStateManager.h \
    src/util/typeAliases.h \
    src/game/world/Scene.h \
    src/game/world/models/plants/GrassRenderer.h \
    src/game/world/models/plants/TreesRenderer.h \
    src/game/OptionsUnits.h \
    src/graphics/ShadowVolume.h \
    src/graphics/GraphicsSettings.h \
    src/game/world/SceneSettings.h \
    src/util/DirectoriesSettings.h \
    src/game/world/ambience/lensFlare/LensFlareElement.h \
    src/game/world/ambience/lensFlare/LensFlareFacade.h \
    src/game/world/ambience/lensFlare/LensFlareRenderer.h \
    src/graphics/openglObjects/Framebuffer.h \
    src/graphics/openglObjects/Query.h \
    src/graphics/openglObjects/BufferCollection.h \
    src/graphics/textures/BindlessTextureManager.h \
    src/graphics/textures/TextureLoader.h \
    src/graphics/textures/TextureManager.h \
    src/graphics/textures/TextureUnits.h \
    src/graphics/shaders/Shader.h \
    src/graphics/shaders/ShaderManager.h \
    src/graphics/shaders/ShaderUnits.h \
    src/graphics/framebuffers/DepthmapFramebuffer.h \
    src/graphics/framebuffers/ScreenFramebuffer.h \
    src/graphics/framebuffers/WaterReflectionFramebuffer.h \
    src/graphics/framebuffers/WaterRefractionFramebuffer.h \
    src/graphics/gui/FontLoader.h \
    src/graphics/gui/TextManager.h \
    src/graphics/gui/CoordinateSystemRenderer.h \
    src/graphics/gui/FrustumRenderer.h \
    src/graphics/gui/ShadowVolumeRenderer.h \
    src/game/world/models/hemisphere/HemisphereFacade.h \
    src/game/world/models/hemisphere/HemisphereShader.h \
    src/game/world/models/hemisphere/Hemisphere.h

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lGL

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lGLEW

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lglfw

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -lIL

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/x86_64-linux-gnu/libIL.a

DISTFILES += \
    res/textures/hill.jpg \
    res/textures/hill2.jpg \
    res/textures/cubemap/back.png \
    res/textures/cubemap/down.png \
    res/textures/cubemap/front.png \
    res/textures/cubemap/left.png \
    res/textures/cubemap/right.png \
    res/textures/cubemap/up.png \
    res/textures/shore.jpg \
    res/textures/shore2.jpg \
    res/textures/flat2_x2.jpg \
    res/textures/flat_x2.jpg \
    res/shaders/skybox/skybox.vs \
    res/shaders/skybox/skybox.fs \
    res/shaders/modelGouraud/model.vs \
    res/shaders/modelGouraud/model.fs \
    res/shaders/font/font.vs \
    res/shaders/font/font.fs \
    res/shaders/coordinateSystem/coordinateSystem.vs \
    res/shaders/coordinateSystem/coordinateSystem.gs \
    res/shaders/coordinateSystem/coordinateSystem.fs \
    res/shaders/buildable/buildableTiles.fs \
    res/shaders/buildable/buildableTiles.vs \
    res/shaders/selected/selectedTile.vs \
    res/shaders/selected/selectedTile.fs \
    res/fonts/GentiumPlus-R.ttf \
    res/fonts/OCTAPOST_1.ttf \
    res/fonts/Laconic_Bold.otf \
    res/shaders/hills/hills.vs \
    res/shaders/hills/hills.fs \
    res/shaders/water/water.vs \
    res/shaders/water/water.fs \
    res/shaders/underwater/underwater.vs \
    res/shaders/underwater/underwater.fs \
    res/shaders/shore/shore.vs \
    res/shaders/shore/shore.fs \
    res/saves/testSave.txt \
    res/shaders/screen/MS_toDefault.vs \
    res/shaders/screen/MS_toDefault_hdr.fs \
    res/shaders/shadow/model_shadow.vs \
    res/shaders/shadow/terrain_shadow.vs \
    res/perAppLog.txt \
    res/perFrameLog.txt \
    res/shaders/waterFC/waterFC.gs \
    res/shaders/waterFC/waterFC.vs \
    res/shaders/hillsFC/hillsFC.vs \
    res/shaders/hillsFC/hillsFC.gs \
    res/fonts/font.png \
    res/fonts/font.fnt \
    res/models/hillTree1/tree.jpg \
    res/models/hillTree2/tree.jpg \
    res/models/hillTree3/tree.jpg \
    res/models/hillTree4/tree.jpg \
    res/models/hillTree5/tree.jpg \
    res/models/hillTree6/tree.jpg \
    res/models/hillTree7/tree.jpg \
    res/models/tree1/tree.jpg \
    res/models/tree1_2/tree.jpg \
    res/models/tree2/tree.jpg \
    res/models/tree2_2/tree.jpg \
    res/models/tree4/tree.jpg \
    res/models/tree5/tree.jpg \
    res/models/tree5_2/tree.jpg \
    res/models/tree6/tree.jpg \
    res/models/tree6_2/tree.jpg \
    res/models/tree7/tree.jpg \
    res/models/tree8/tree.jpg \
    res/models/hillTree1/foliage.png \
    res/models/hillTree2/foliage1.png \
    res/models/hillTree2/foliage2.png \
    res/models/hillTree2/foliage3.png \
    res/models/hillTree2/foliage4.png \
    res/models/hillTree3/foliage.png \
    res/models/hillTree4/foliage1.png \
    res/models/hillTree4/foliage2.png \
    res/models/hillTree4/foliage3.png \
    res/models/hillTree4/foliage4.png \
    res/models/hillTree5/cookie-foliage.png \
    res/models/hillTree5/foliage.png \
    res/models/hillTree6/cookie-foliage.png \
    res/models/hillTree6/foliage.png \
    res/models/hillTree7/foliage.png \
    res/models/tree1/foliage5.png \
    res/models/tree1_2/foliage5.png \
    res/models/tree2/foliage1.png \
    res/models/tree2_2/foliage1.png \
    res/models/tree3/foliage-red.png \
    res/models/tree3/foliage1.png \
    res/models/tree3/nut.png \
    res/models/tree3/stem.png \
    res/models/tree3_2/foliage-red.png \
    res/models/tree3_2/foliage1.png \
    res/models/tree3_2/nut.png \
    res/models/tree3_2/stem.png \
    res/models/tree4/cream.png \
    res/models/tree4/dough.png \
    res/models/tree5/foliage5.png \
    res/models/tree5_2/foliage1.png \
    res/models/tree6/foliage.png \
    res/models/tree6_2/foliage.png \
    res/models/tree7/foliage.png \
    res/models/tree8/foliage.png \
    res/models/tree8/foliage2.png \
    res/models/hillTree1/hillTree1.mtl \
    res/models/hillTree2/hillTree2.mtl \
    res/models/hillTree3/hillTree3.mtl \
    res/models/hillTree4/hillTree4.mtl \
    res/models/hillTree5/hillTree5.mtl \
    res/models/hillTree6/hillTree6.mtl \
    res/models/hillTree7/hillTree7.mtl \
    res/models/tree1/tree1.mtl \
    res/models/tree1_2/tree1_2.mtl \
    res/models/tree2/tree2.mtl \
    res/models/tree2_2/tree2_2.mtl \
    res/models/tree3/tree3.mtl \
    res/models/tree3_2/tree3_2.mtl \
    res/models/tree4/tree4.mtl \
    res/models/tree5/tree5.mtl \
    res/models/tree5_2/tree5_2.mtl \
    res/models/tree6/tree6.mtl \
    res/models/tree6_2/tree6_2.mtl \
    res/models/tree7/tree7.mtl \
    res/models/tree8/tree8.mtl \
    res/models/hillTree1/hillTree1.obj \
    res/models/hillTree2/hillTree2.obj \
    res/models/hillTree3/hillTree3.obj \
    res/models/hillTree4/hillTree4.obj \
    res/models/hillTree5/hillTree5.obj \
    res/models/hillTree6/hillTree6.obj \
    res/models/hillTree7/hillTree7.obj \
    res/models/tree1/tree1.obj \
    res/models/tree1_2/tree1_2.obj \
    res/models/tree2/tree2.obj \
    res/models/tree2_2/tree2_2.obj \
    res/models/tree3/tree3.obj \
    res/models/tree3_2/tree3_2.obj \
    res/models/tree4/tree4.obj \
    res/models/tree5/tree5.obj \
    res/models/tree5_2/tree5_2.obj \
    res/models/tree6/tree6.obj \
    res/models/tree6_2/tree6_2.obj \
    res/models/tree7/tree7.obj \
    res/models/tree8/tree8.obj \
    res/textures/terrainNormal.jpg \
    res/textures/diffuseMixMap.jpg \
    res/textures/waterNormal.jpg \
    res/textures/waterSpecular.jpg \
    res/models/grass1/grass1.obj \
    res/models/grass1/grass.png \
    res/models/grass1/grass2.png \
    res/models/grass1/grass1.mtl \
    res/models/grass2/grass.png \
    res/models/grass2/grass2.png \
    res/models/grass3/grass.png \
    res/models/grass3/grass2.png \
    res/models/grass4/grass.png \
    res/models/grass4/grass2.png \
    res/models/grass5/grass.png \
    res/models/grass5/grass2.png \
    res/models/grass5/rock.png \
    res/models/grass6/grass.png \
    res/models/grass6/grass2.png \
    res/models/grass6/rock.png \
    res/models/grass2/grass2.mtl \
    res/models/grass3/grass3.mtl \
    res/models/grass4/grass4.mtl \
    res/models/grass5/grass5.mtl \
    res/models/grass6/grass6.mtl \
    res/models/grass2/grass2.obj \
    res/models/grass3/grass3.obj \
    res/models/grass4/grass4.obj \
    res/models/grass5/grass5.obj \
    res/models/grass6/grass6.obj \
    res/shaders/modelPhong/modelPhong.fs \
    res/shaders/modelPhong/modelPhong.vs \
    res/models/grass5/rock_spec.png \
    res/models/grass6/rock_spec.png \
    res/models/hillTree1/tree_spec.jpg \
    res/models/hillTree2/tree_spec.jpg \
    res/models/hillTree3/tree_spec.jpg \
    res/models/hillTree4/tree_spec.jpg \
    res/models/hillTree5/tree_spec.jpg \
    res/models/hillTree6/tree_spec.jpg \
    res/models/hillTree7/tree_spec.jpg \
    res/models/tree1/tree_spec.jpg \
    res/models/tree1_2/tree_spec.jpg \
    res/models/tree2/tree_spec.jpg \
    res/models/tree2_2/tree_spec.jpg \
    res/models/tree4/tree_spec.jpg \
    res/models/tree5/tree_spec.jpg \
    res/models/tree5_2/tree_spec.jpg \
    res/models/tree6/tree_spec.jpg \
    res/models/tree6_2/tree_spec.jpg \
    res/models/tree7/tree_spec.jpg \
    res/models/tree8/tree_spec.jpg \
    res/models/hillTree1/foliage_spec.png \
    res/models/hillTree2/foliage_spec.png \
    res/models/hillTree3/foliage_spec.png \
    res/models/hillTree4/foliage_spec.png \
    res/models/hillTree5/cookie-foliage_spec.png \
    res/models/hillTree5/foliage_spec.png \
    res/models/hillTree6/cookie-foliage_spec.png \
    res/models/hillTree6/foliage_spec.png \
    res/models/hillTree7/foliage_spec.png \
    res/models/tree1/foliage5_spec.png \
    res/models/tree1_2/foliage5_spec.png \
    res/models/tree2/foliage1_spec.png \
    res/models/tree2_2/foliage1_spec.png \
    res/models/tree3/foliage1_spec.png \
    res/models/tree3_2/foliage1_spec.png \
    res/models/tree4/cream_spec.png \
    res/models/tree4/dough_spec.png \
    res/models/tree5/foliage5_spec.png \
    res/models/tree5_2/foliage5_spec.png \
    res/models/tree6/foliage_spec.png \
    res/models/tree6_2/foliage_spec.png \
    res/models/tree7/foliage_spec.png \
    res/models/tree8/foliage_spec.png \
    res/textures/underwater.jpg \
    res/shaders/land/land.vs \
    res/shaders/land/land.fs \
    res/textures/cubemapFar/back.png \
    res/textures/cubemapFar/down.png \
    res/textures/cubemapFar/front.png \
    res/textures/cubemapFar/left.png \
    res/textures/cubemapFar/right.png \
    res/textures/cubemapFar/up.png \
    res/textures/cubemapSky/back.png \
    res/textures/cubemapSky/down.png \
    res/textures/cubemapSky/front.png \
    res/textures/cubemapSky/left.png \
    res/textures/cubemapSky/right.png \
    res/textures/cubemapSky/up.png \
    res/textures/theSun.png \
    res/shaders/theSun/theSun.vs \
    res/shaders/theSun/theSun.fs \
    res/shaders/normals/shore_normals.vs \
    res/shaders/normals/hills_normals.vs \
    res/shaders/normals/water_normals.vs \
    res/shaders/normals/_normals.gs \
    res/shaders/normals/_normals.fs \
    res/textures/land.jpg \
    res/textures/land2.jpg \
    res/shaders/include/shadowSampling.ifs \
    res/shaders/include/desaturationFunc.ifs \
    res/shaders/include/shadingVariables.ifs \
    res/shaders/include/modelGrassAnimationAndBlending.ivs \
    res/shaders/frustum/frustum.vs \
    res/shaders/frustum/frustum.fs \
    res/shaders/shadow/shadow.gs \
    res/models/tree1LP/tree.jpg \
    res/models/tree1LP/tree_spec.jpg \
    res/models/tree1LP/foliage5.png \
    res/models/tree1LP/foliage5_spec.png \
    res/models/tree1LP/tree1LP.mtl \
    res/models/tree1_2LP/tree1_2LP.obj \
    res/models/tree1_2LP/tree.jpg \
    res/models/tree1_2LP/tree_spec.jpg \
    res/models/tree1_2LP/foliage5.png \
    res/models/tree1_2LP/foliage5_spec.png \
    res/models/tree1_2LP/tree1_2LP.mtl \
    res/models/tree1LP/tree1LP.obj \
    res/models/tree2LP/tree2LP.obj \
    res/models/tree2LP/tree.jpg \
    res/models/tree2LP/tree_spec.jpg \
    res/models/tree2LP/foliage1.png \
    res/models/tree2LP/foliage1_spec.png \
    res/models/tree2LP/tree2LP.mtl \
    res/models/tree2_2LP/tree2_2LP.obj \
    res/models/tree2_2LP/tree.jpg \
    res/models/tree2_2LP/tree_spec.jpg \
    res/models/tree2_2LP/foliage1.png \
    res/models/tree2_2LP/foliage1_spec.png \
    res/models/tree2_2LP/tree2_2LP.mtl \
    res/models/tree3LP/tree3LP.obj \
    res/models/tree3LP/foliage-red.png \
    res/models/tree3LP/foliage1.png \
    res/models/tree3LP/foliage1_spec.png \
    res/models/tree3LP/nut.png \
    res/models/tree3LP/stem.png \
    res/models/tree3LP/tree3LP.mtl \
    res/models/tree3_2LP/tree3_2LP.obj \
    res/models/tree3_2LP/foliage-red.png \
    res/models/tree3_2LP/foliage1.png \
    res/models/tree3_2LP/foliage1_spec.png \
    res/models/tree3_2LP/nut.png \
    res/models/tree3_2LP/tree3_2LP.mtl \
    res/models/tree4LP/tree4LP.obj \
    res/models/tree4LP/tree.jpg \
    res/models/tree4LP/tree_spec.jpg \
    res/models/tree4LP/cream.png \
    res/models/tree4LP/cream_spec.png \
    res/models/tree4LP/dough.png \
    res/models/tree4LP/dough_spec.png \
    res/models/tree4LP/tree4LP.mtl \
    res/models/tree5LP/tree5LP.obj \
    res/models/tree5LP/tree.jpg \
    res/models/tree5LP/tree_spec.jpg \
    res/models/tree5LP/foliage5.png \
    res/models/tree5LP/foliage5_spec.png \
    res/models/tree5LP/tree5LP.mtl \
    res/models/tree5_2LP/tree5_2LP.obj \
    res/models/tree5_2LP/tree.jpg \
    res/models/tree5_2LP/tree_spec.jpg \
    res/models/tree5_2LP/foliage5.png \
    res/models/tree5_2LP/foliage5_spec.png \
    res/models/tree5_2LP/tree5_2LP.mtl \
    res/models/tree6_2LP/tree6_2LP.obj \
    res/models/tree6LP/tree6LP.obj \
    res/models/tree7LP/tree7LP.obj \
    res/models/tree8LP/tree8LP.obj \
    res/models/tree6_2LP/tree.jpg \
    res/models/tree6_2LP/tree_spec.jpg \
    res/models/tree6LP/tree.jpg \
    res/models/tree6LP/tree_spec.jpg \
    res/models/tree7LP/tree.jpg \
    res/models/tree7LP/tree_spec.jpg \
    res/models/tree8LP/tree.jpg \
    res/models/tree8LP/tree_spec.jpg \
    res/models/tree6_2LP/foliage.png \
    res/models/tree6_2LP/foliage_spec.png \
    res/models/tree6LP/foliage.png \
    res/models/tree6LP/foliage_spec.png \
    res/models/tree7LP/foliage.png \
    res/models/tree7LP/foliage_spec.png \
    res/models/tree8LP/foliage.png \
    res/models/tree8LP/foliage2.png \
    res/models/tree8LP/foliage2_spec.png \
    res/models/tree8LP/foliage_spec.png \
    res/models/tree6_2LP/tree6_2LP.mtl \
    res/models/tree6LP/tree6LP.mtl \
    res/models/tree7LP/tree7LP.mtl \
    res/models/tree8LP/tree8LP.mtl \
    res/models/hillTree1LP/hillTree1LP.obj \
    res/models/hillTree2LP/hillTree2LP.obj \
    res/models/hillTree3LP/hillTree3LP.obj \
    res/models/hillTree4LP/hillTree4LP.obj \
    res/models/hillTree5LP/hillTree5LP.obj \
    res/models/hillTree6LP/hillTree6LP.obj \
    res/models/hillTree7LP/hillTree7LP.obj \
    res/models/hillTree1LP/tree.jpg \
    res/models/hillTree1LP/tree_spec.jpg \
    res/models/hillTree2LP/tree.jpg \
    res/models/hillTree2LP/tree_spec.jpg \
    res/models/hillTree3LP/tree.jpg \
    res/models/hillTree3LP/tree_spec.jpg \
    res/models/hillTree4LP/tree.jpg \
    res/models/hillTree4LP/tree_spec.jpg \
    res/models/hillTree5LP/tree.jpg \
    res/models/hillTree5LP/tree_spec.jpg \
    res/models/hillTree6LP/tree.jpg \
    res/models/hillTree6LP/tree_spec.jpg \
    res/models/hillTree7LP/tree.jpg \
    res/models/hillTree7LP/tree_spec.jpg \
    res/models/hillTree1LP/foliage.png \
    res/models/hillTree1LP/foliage_spec.png \
    res/models/hillTree2LP/foliage1.png \
    res/models/hillTree2LP/foliage2.png \
    res/models/hillTree2LP/foliage3.png \
    res/models/hillTree2LP/foliage4.png \
    res/models/hillTree2LP/foliage_spec.png \
    res/models/hillTree3LP/foliage.png \
    res/models/hillTree3LP/foliage_spec.png \
    res/models/hillTree4LP/foliage1.png \
    res/models/hillTree4LP/foliage2.png \
    res/models/hillTree4LP/foliage3.png \
    res/models/hillTree4LP/foliage4.png \
    res/models/hillTree4LP/foliage_spec.png \
    res/models/hillTree5LP/cookie-foliage.png \
    res/models/hillTree5LP/cookie-foliage_spec.png \
    res/models/hillTree5LP/foliage.png \
    res/models/hillTree5LP/foliage_spec.png \
    res/models/hillTree6LP/cookie-foliage.png \
    res/models/hillTree6LP/cookie-foliage_spec.png \
    res/models/hillTree6LP/foliage.png \
    res/models/hillTree6LP/foliage_spec.png \
    res/models/hillTree7LP/foliage.png \
    res/models/hillTree7LP/foliage_spec.png \
    res/models/hillTree1LP/hillTree1LP.mtl \
    res/models/hillTree2LP/hillTree2LP.mtl \
    res/models/hillTree3LP/hillTree3LP.mtl \
    res/models/hillTree4LP/hillTree4LP.mtl \
    res/models/hillTree5LP/hillTree5LP.mtl \
    res/models/hillTree6LP/hillTree6LP.mtl \
    res/models/hillTree7LP/hillTree7LP.mtl \
    res/models/grass1LP/grass1LP.obj \
    res/models/grass2LP/grass2LP.obj \
    res/models/grass3LP/grass3LP.obj \
    res/models/grass4LP/grass4LP.obj \
    res/models/grass5LP/grass5LP.obj \
    res/models/grass6LP/grass6LP.obj \
    res/models/grass1LP/grass.png \
    res/models/grass1LP/grass2.png \
    res/models/grass2LP/grass.png \
    res/models/grass2LP/grass2.png \
    res/models/grass3LP/grass.png \
    res/models/grass3LP/grass2.png \
    res/models/grass4LP/grass.png \
    res/models/grass4LP/grass2.png \
    res/models/grass5LP/grass.png \
    res/models/grass5LP/grass2.png \
    res/models/grass5LP/rock.png \
    res/models/grass5LP/rock_spec.png \
    res/models/grass6LP/grass.png \
    res/models/grass6LP/grass2.png \
    res/models/grass6LP/rock.png \
    res/models/grass6LP/rock_spec.png \
    res/models/grass1LP/grass1LP.mtl \
    res/models/grass2LP/grass2LP.mtl \
    res/models/grass3LP/grass3LP.mtl \
    res/models/grass4LP/grass4LP.mtl \
    res/models/grass5LP/grass5LP.mtl \
    res/models/grass6LP/grass6LP.mtl \
    res/textures/cubemapFarNormal/back.png \
    res/textures/cubemapFarNormal/down.png \
    res/textures/cubemapFarNormal/front.png \
    res/textures/cubemapFarNormal/left.png \
    res/textures/cubemapFarNormal/right.png \
    res/textures/cubemapFarNormal/up.png \
    res/textures/cubemapNormal/back.png \
    res/textures/cubemapNormal/down.png \
    res/textures/cubemapNormal/front.png \
    res/textures/cubemapNormal/left.png \
    res/textures/cubemapNormal/right.png \
    res/textures/cubemapNormal/up.png \
    res/textures/vignette.png \
    res/shaders/lensFlare/lensFlare.vs \
    res/shaders/lensFlare/lensFlare.fs \
    res/textures/lensFlares/flare1.png \
    res/textures/lensFlares/flare10.png \
    res/textures/lensFlares/flare11.png \
    res/textures/lensFlares/flare12.png \
    res/textures/lensFlares/flare13.png \
    res/textures/lensFlares/flare2.png \
    res/textures/lensFlares/flare3.png \
    res/textures/lensFlares/flare4.png \
    res/textures/lensFlares/flare5.png \
    res/textures/lensFlares/flare6.png \
    res/textures/lensFlares/flare7.png \
    res/textures/lensFlares/flare8.png \
    res/textures/lensFlares/flare9.png \
    res/textures/vignetteHoneycomb.png \
    res/textures/vignetteInterlacing.png \
    res/textures/waterDUDV.png \
    res/models/hillTree10cob/hillTree10cob.obj \
    res/models/hillTree8cob/hillTree8cob.obj \
    res/models/hillTree9cob/hillTree9cob.obj \
    res/models/hillTree10cob/foliage-red-spec.png \
    res/models/hillTree10cob/nut.png \
    res/models/hillTree8cob/foliage-red-spec.png \
    res/models/hillTree8cob/foliage-red.png \
    res/models/hillTree8cob/foliage-red2.png \
    res/models/hillTree9cob/foliage-red-spec.png \
    res/models/hillTree9cob/foliage-red.png \
    res/models/hillTree9cob/foliage-red2.png \
    res/models/hillTree10cob/hillTree10cob.mtl \
    res/models/hillTree8cob/hillTree8cob.mtl \
    res/models/hillTree9cob/hillTree9cob.mtl \
    res/textures/hill_specular.png \
    res/textures/hemisphereDiffuse.png \
    res/models/hemisphere/hemisphere.mtl \
    res/models/hemisphere/hemisphere.obj \
    res/shaders/hemisphere/hemisphere.vs \
    res/shaders/hemisphere/hemisphere.fs \
    res/models/sphere/sphere.obj \
    res/models/sphere/sphere.mtl \
    res/textures/sky.png

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lassimp

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -lpthread-2.23
