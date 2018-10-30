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
    src/graphics/Shader.cpp \
    src/graphics/TextureLoader.cpp \
    src/graphics/Renderer.cpp \
    src/graphics/CoordinateSystemRenderer.cpp \
    src/graphics/Frustum.cpp \
    src/graphics/TextureManager.cpp \
    src/graphics/ShaderManager.cpp \
    src/model/Mesh.cpp \
    src/model/Model.cpp \
    src/model/IndirectBufferToken.cpp \
    src/input/KeyboardManager.cpp \
    src/input/MouseInputManager.cpp \
    src/graphics/FontLoader.cpp \
    src/game/world/plants/GrassGenerator.cpp \
    src/game/world/plants/HillTreesGenerator.cpp \
    src/game/world/plants/LandPlantsGenerator.cpp \
    src/game/world/plants/PlantGenerator.cpp \
    src/game/world/terrain/Generator.cpp \
    src/game/world/PlantGeneratorFacade.cpp \
    src/game/world/WorldGeneratorFacade.cpp \
    src/util/BenchmarkTimer.cpp \
    src/util/Settings.cpp \
    src/game/world/plants/ModelChunk.cpp \
    src/game/world/terrain/TerrainTile.cpp \
    src/graphics/DepthmapBuffer.cpp \
    src/graphics/ScreenBuffer.cpp \
    src/graphics/ScreenResolution.cpp \
    src/game/Camera.cpp \
    src/graphics/TextManager.cpp \
    src/game/world/Chunk.cpp \
    src/util/VRAM_Monitor.cpp \
    src/graphics/OpenglBuffer.cpp \
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
    src/game/world/ambience/theSun/TheSunRenderer.cpp

HEADERS += \
    src/game/Game.h \
    src/game/SaveLoadManager.h \
    src/game/Options.h \
    src/graphics/TextureUnits.h \
    src/graphics/Shader.h \
    src/graphics/ShaderManager.h \
    src/graphics/TextureLoader.h \
    src/graphics/Renderer.h \
    src/graphics/CoordinateSystemRenderer.h \
    src/graphics/Frustum.h \
    src/graphics/TextureManager.h \
    src/model/Mesh.h \
    src/model/Model.h \
    src/model/IndirectBufferToken.h \
    src/input/KeyboardManager.h \
    src/input/MouseInputManager.h \
    src/generator/PlantGenerator.h \
    src/graphics/FontLoader.h \
    src/game/world/plants/GrassGenerator.h \
    src/game/world/plants/HillTreesGenerator.h \
    src/game/world/plants/LandPlantsGenerator.h \
    src/game/world/plants/PlantGenerator.h \
    src/game/world/terrain/Generator.h \
    src/game/world/PlantGeneratorFacade.h \
    src/game/world/WorldGeneratorFacade.h \
    src/util/BenchmarkTimer.h \
    src/util/Settings.h \
    src/util/Timer.h \
    src/game/world/plants/ModelChunk.h \
    src/game/world/terrain/TerrainTile.h \
    src/graphics/DepthmapBuffer.h \
    src/graphics/ScreenBuffer.h \
    src/graphics/ScreenResolution.h \
    src/game/Camera.h \
    src/graphics/TextManager.h \
    src/game/world/Chunk.h \
    src/util/VRAM_Monitor.h \
    src/graphics/OpenglBuffer.h \
    src/game/world/ambience/theSun/TheSun.h \
    src/util/Logger.h \
    src/game/world/terrain/hills/HillsGenerator.h \
    src/game/world/terrain/hills/HillsFacade.h \
    src/game/world/terrain/hills/HillsShader.h \
    src/game/world/terrain/hills/HillsRenderer.h \
    src/graphics/ShaderUnits.h \
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
    src/game/world/ambience/theSun/TheSunRenderer.h

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
    res/textures/hill_specular.jpg \
    res/textures/cubemap/back.png \
    res/textures/cubemap/down.png \
    res/textures/cubemap/front.png \
    res/textures/cubemap/left.png \
    res/textures/cubemap/right.png \
    res/textures/cubemap/up.png \
    res/textures/shore.jpg \
    res/textures/shore2.jpg \
    res/textures/flat.jpg \
    res/textures/flat2.jpg \
    res/textures/flat2_x2.jpg \
    res/textures/flat_x2.jpg \
    res/shaders/skybox.vs \
    res/shaders/skybox.fs \
    res/shaders/water.fs \
    res/shaders/hills.fs \
    res/shaders/underwater.fs \
    res/shaders/model.vs \
    res/shaders/model.fs \
    res/shaders/font.vs \
    res/shaders/font.fs \
    res/shaders/coordinateSystem.vs \
    res/shaders/coordinateSystem.gs \
    res/shaders/coordinateSystem.fs \
    res/shaders/buildableTiles.fs \
    res/shaders/buildableTiles.vs \
    res/shaders/selectedTile.vs \
    res/fonts/GentiumPlus-R.ttf \
    res/fonts/OCTAPOST_1.ttf \
    res/fonts/Laconic_Bold.otf \
    res/shaders/hills.vs \
    res/shaders/water.vs \
    res/shaders/underwater.vs \
    res/shaders/shore.fs \
    res/shaders/shore.vs \
    res/shaders/selectedTile.fs \
    res/saves/testSave.txt \
    res/shaders/MS_toDefault.vs \
    res/shaders/model_shadow.vs \
    res/shaders/terrain_shadow.vs \
    res/shaders/MS_toDefault_hdr.fs \
    res/perAppLog.txt \
    res/perFrameLog.txt \
    res/shaders/waterFC.gs \
    res/shaders/waterFC.vs \
    res/shaders/hillsFC.vs \
    res/shaders/hillsFC.gs \
    res/shaders/_FC.fs \
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
    res/shaders/modelPhong.fs \
    res/shaders/modelPhong.vs \
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
    res/shaders/land.vs \
    res/shaders/land.fs \
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
    res/shaders/theSun.vs \
    res/shaders/theSun.fs

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lassimp

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -lpthread-2.23
