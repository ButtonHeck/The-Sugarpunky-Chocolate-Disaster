TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

INCLUDEPATH += $$PWD/src

SOURCES += \
    src/game/main.cpp \
    src/game/Game.cpp \
    src/game/Settings.cpp \
    src/game/InputController.cpp \
    src/game/SaveLoadManager.cpp \
    src/game/Options.cpp \
    src/graphics/Shader.cpp \
    src/graphics/Camera.cpp \
    src/graphics/TextureLoader.cpp \
    src/graphics/Skybox.cpp \
    src/graphics/Renderer.cpp \
    src/graphics/CoordinateSystemRenderer.cpp \
    src/graphics/Frustum.cpp \
    src/graphics/TextureManager.cpp \
    src/graphics/ShaderManager.cpp \
    src/graphics/FontManager.cpp \
    src/generator/MapGenerator.cpp \
    src/generator/WaterMapGenerator.cpp \
    src/generator/HillsMapGenerator.cpp \
    src/generator/UnderwaterQuadMapGenerator.cpp \
    src/generator/BaseMapGenerator.cpp \
    src/generator/TreeGenerator.cpp \
    src/generator/BuildableMapGenerator.cpp \
    src/model/Mesh.cpp \
    src/model/Model.cpp \
    src/model/TerrainTile.cpp \
    src/chunk/ModelChunk.cpp \
    src/chunk/Chunk.cpp \
    src/chunk/TerrainChunk.cpp \
    src/timer/BenchmarkTimer.cpp \
    src/model/IndirectBufferToken.cpp

HEADERS += \
    src/game/Game.h \
    src/game/Settings.h \
    src/game/InputController.h \
    src/game/SaveLoadManager.h \
    src/game/Options.h \
    src/graphics/Shader.h \
    src/graphics/ShaderManager.h \
    src/graphics/Camera.h \
    src/graphics/TextureLoader.h \
    src/graphics/Renderer.h \
    src/graphics/Skybox.h \
    src/graphics/CoordinateSystemRenderer.h \
    src/graphics/Frustum.h \
    src/graphics/TextureManager.h \
    src/graphics/FontManager.h \
    src/generator/MapGenerator.h \
    src/generator/WaterMapGenerator.h \
    src/generator/HillsMapGenerator.h \
    src/generator/UnderwaterQuadMapGenerator.h \
    src/generator/BaseMapGenerator.h \
    src/generator/TreeGenerator.h \
    src/generator/BuildableMapGenerator.h \
    src/model/TerrainTile.h \
    src/model/Mesh.h \
    src/model/Model.h \
    src/chunk/ModelChunk.h \
    src/chunk/Chunk.h \
    src/chunk/TerrainChunk.h \
    src/timer/BenchmarkTimer.h \
    src/timer/Timer.h \
    src/model/IndirectBufferToken.h

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
    res/shaders/flat.vs \
    res/shaders/flat.fs \
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
    res/models/tree8/foliage_spec.png

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lassimp

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lfreetype

INCLUDEPATH += $$PWD/../../../../../usr/local/include/freetype2
DEPENDPATH += $$PWD/../../../../../usr/local/include/freetype2

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libfreetype.a

unix:!macx: LIBS += -lpthread-2.23
