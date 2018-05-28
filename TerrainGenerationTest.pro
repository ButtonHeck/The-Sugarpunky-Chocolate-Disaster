TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

SOURCES += main.cpp \
    Shader.cpp \
    Camera.cpp \
    TextureLoader.cpp \
    TerrainTile.cpp \
    InputController.cpp \
    MapGenerator.cpp \
    WaterMapGenerator.cpp \
    HillsMapGenerator.cpp \
    UnderwaterQuadMapGenerator.cpp \
    BaseMapGenerator.cpp \
    Skybox.cpp \
    Mesh.cpp \
    Model.cpp \
    TreeGenerator.cpp \
    FontManager.cpp \
    CoordinateSystemRenderer.cpp \
    SaveLoadManager.cpp \
    BuildableMapGenerator.cpp \
    ModelChunk.cpp


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
    Timer.h \
    Settings.h \
    Shader.h \
    Camera.h \
    TextureLoader.h \
    TerrainTile.h \
    InputController.h \
    MapGenerator.h \
    WaterMapGenerator.h \
    HillsMapGenerator.h \
    UnderwaterQuadMapGenerator.h \
    BaseMapGenerator.h \
    Skybox.h \
    Mesh.h \
    Model.h \
    TreeGenerator.h \
    FontManager.h \
    CoordinateSystemRenderer.h \
    SaveLoadManager.h \
    BuildableMapGenerator.h \
    ModelChunk.h

DISTFILES += \
    textures/base.jpg \
    textures/base2.jpg \
    textures/base_normal.jpg \
    textures/base2_x2.jpg \
    textures/base_x2.jpg \
    textures/underwater_sand.jpg \
    textures/sand.jpg \
    textures/sand2.jpg \
    textures/hill.jpg \
    textures/hill2.jpg \
    textures/hill_specular.jpg \
    textures/water.png \
    textures/water_specular.png \
    textures/underwater_sand.jpg \
    textures/cubemap/back.png \
    textures/cubemap/down.png \
    textures/cubemap/front.png \
    textures/cubemap/left.png \
    textures/cubemap/right.png \
    textures/cubemap/up.png \
    shaders/terrainVertex.vs \
    shaders/skybox.vs \
    shaders/skybox.fs \
    shaders/water.fs \
    shaders/hills.fs \
    shaders/sand.fs \
    shaders/underwater.fs \
    shaders/model.vs \
    shaders/model.fs \
    shaders/font.vs \
    shaders/font.fs \
    shaders/coordinateSystem.vs \
    shaders/coordinateSystem.gs \
    shaders/coordinateSystem.fs \
    shaders/buildableTiles.fs \
    shaders/buildableTiles.vs \
    shaders/selectedTile.vs \
    models/tree1/tree1.obj \
    models/tree1/leaves.jpg \
    models/tree1/tree.jpg \
    models/tree1/tree1.mtl \
    models/tree2/tree2.obj \
    models/tree2/leaves.jpg \
    models/tree2/tree.jpg \
    models/tree2/tree2.mtl \
    models/tree3/tree3.obj \
    models/tree3/leaves.jpg \
    models/tree3/tree.jpg \
    models/tree3/tree3.mtl \
    models/hillTree1/hillTree1.obj \
    models/hillTree1/leaves.jpg \
    models/hillTree1/tree.jpg \
    models/hillTree1/hillTree1.mtl \
    models/hillTree2/hillTree2.obj \
    models/hillTree2/leaves.jpg \
    models/hillTree2/tree.jpg \
    models/hillTree2/hillTree2.mtl \
    models/hillTree3/hillTree3.obj \
    models/hillTree3/leaves.jpg \
    models/hillTree3/tree.jpg \
    models/hillTree3/hillTree3.mtl \
    fonts/GentiumPlus-R.ttf \
    fonts/OCTAPOST_1.ttf \
    shaders/flat.vs \
    shaders/flat.fs \
    shaders/hills.vs \
    shaders/water.vs

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lassimp

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lfreetype

INCLUDEPATH += $$PWD/../../../../../usr/local/include/freetype2
DEPENDPATH += $$PWD/../../../../../usr/local/include/freetype2

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libfreetype.a
