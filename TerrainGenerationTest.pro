TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/game/main.cpp \
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
    src/graphics/FontManager.cpp \
    src/graphics/CoordinateSystemRenderer.cpp \
    src/graphics/Frustum.cpp \
    src/graphics/TextureManager.cpp \
    src/graphics/ShaderManager.cpp \
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
    src/chunk/TerrainChunk.cpp


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

HEADERS += \
    src/game/Game.h \
    src/game/Timer.h \
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
    src/graphics/FontManager.h \
    src/graphics/CoordinateSystemRenderer.h \
    src/graphics/Frustum.h \
    src/graphics/TextureManager.h \
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
    src/chunk/TerrainChunk.h

DISTFILES += \
    res/textures/hill.jpg \
    res/textures/hill2.jpg \
    res/textures/hill_specular.jpg \
    res/textures/water.png \
    res/textures/water_specular.png \
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
    res/textures/normalMap.jpg \
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
    res/shaders/water.gs \
    res/shaders/water_noFC.vs \
    res/shaders/water_noFC.fs \
    res/shaders/hills_noFC.vs \
    res/shaders/hills_noFC.fs \
    res/shaders/hills.gs \
    res/shaders/hdr.vs \
    res/shaders/hdr.fs \
    res/shaders/MS_toDefault.vs \
    res/shaders/MS_toDefault.fs \
    res/shaders/_depth.fs \
    res/shaders/flat_shadow.vs \
    res/shaders/hills_shadow.vs \
    res/shaders/model_shadow.vs \
    res/shaders/shore_shadow.vs \
    res/shaders/underwater_shadow.vs \
    res/shaders/water_shadow.vs \
    res/models/hillTree1/hillTree1.obj \
    res/models/hillTree2/hillTree2.obj \
    res/models/hillTree3/hillTree3.obj \
    res/models/hillTree4/hillTree4.obj \
    res/models/hillTree5/hillTree5.obj \
    res/models/hillTree6/hillTree6.obj \
    res/models/hillTree7/hillTree7.obj \
    res/models/tree1/tree1.obj \
    res/models/tree2/tree2.obj \
    res/models/tree3/tree3.obj \
    res/models/hillTree1/tree.jpg \
    res/models/hillTree2/tree.jpg \
    res/models/hillTree3/tree.jpg \
    res/models/hillTree4/tree.jpg \
    res/models/hillTree5/tree.jpg \
    res/models/hillTree6/tree.jpg \
    res/models/hillTree7/tree.jpg \
    res/models/tree1/leaves.jpg \
    res/models/tree1/tree.jpg \
    res/models/tree2/leaves.jpg \
    res/models/tree2/tree.jpg \
    res/models/tree3/leaves.jpg \
    res/models/tree3/tree.jpg \
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
    res/models/hillTree1/hillTree1.mtl \
    res/models/hillTree2/hillTree2.mtl \
    res/models/hillTree3/hillTree3.mtl \
    res/models/hillTree4/hillTree4.mtl \
    res/models/hillTree5/hillTree5.mtl \
    res/models/hillTree6/hillTree6.mtl \
    res/models/hillTree7/hillTree7.mtl \
    res/models/tree1/tree1.mtl \
    res/models/tree2/tree2.mtl \
    res/models/tree3/tree3.mtl

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lassimp

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lfreetype

INCLUDEPATH += $$PWD/../../../../../usr/local/include/freetype2
DEPENDPATH += $$PWD/../../../../../usr/local/include/freetype2

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libfreetype.a
