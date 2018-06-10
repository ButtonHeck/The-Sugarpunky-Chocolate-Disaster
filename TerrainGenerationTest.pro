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
    ModelChunk.cpp \
    Settings.cpp \
    Chunk.cpp \
    BaseTerrainChunk.cpp \
    CellChunk.cpp \
    Renderer.cpp \
    ModelManager.cpp


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
    ModelChunk.h \
    Chunk.h \
    BaseTerrainChunk.h \
    CellChunk.h \
    Renderer.h \
    ModelManager.h

DISTFILES += \
    res/textures/underwater_sand.jpg \
    res/textures/hill.jpg \
    res/textures/hill2.jpg \
    res/textures/hill_specular.jpg \
    res/textures/water.png \
    res/textures/water_specular.png \
    res/textures/underwater_sand.jpg \
    res/textures/cubemap/back.png \
    res/textures/cubemap/down.png \
    res/textures/cubemap/front.png \
    res/textures/cubemap/left.png \
    res/textures/cubemap/right.png \
    res/textures/cubemap/up.png \
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
    res/models/tree1/tree1.obj \
    res/models/tree1/leaves.jpg \
    res/models/tree1/tree.jpg \
    res/models/tree1/tree1.mtl \
    res/models/tree2/tree2.obj \
    res/models/tree2/leaves.jpg \
    res/models/tree2/tree.jpg \
    res/models/tree2/tree2.mtl \
    res/models/tree3/tree3.obj \
    res/models/tree3/leaves.jpg \
    res/models/tree3/tree.jpg \
    res/models/tree3/tree3.mtl \
    res/models/hillTree1/hillTree1.obj \
    res/models/hillTree1/leaves.jpg \
    res/models/hillTree1/tree.jpg \
    res/models/hillTree1/hillTree1.mtl \
    res/models/hillTree2/hillTree2.obj \
    res/models/hillTree2/leaves.jpg \
    res/models/hillTree2/tree.jpg \
    res/models/hillTree2/hillTree2.mtl \
    res/models/hillTree3/hillTree3.obj \
    res/models/hillTree3/leaves.jpg \
    res/models/hillTree3/tree.jpg \
    res/models/hillTree3/hillTree3.mtl \
    res/fonts/GentiumPlus-R.ttf \
    res/fonts/OCTAPOST_1.ttf \
    res/shaders/flat.vs \
    res/shaders/flat.fs \
    res/shaders/hills.vs \
    res/shaders/water.vs \
    res/shaders/underwater.vs \
    res/shaders/shore.fs \
    res/shaders/shore.vs \
    res/textures/shore.jpg \
    res/textures/shore2.jpg \
    res/textures/flat.jpg \
    res/textures/flat2.jpg \
    res/textures/flat2_x2.jpg \
    res/textures/flat_x2.jpg \
    res/textures/normalMap.jpg \
    res/shaders/selectedTile.fs

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lassimp

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lfreetype

INCLUDEPATH += $$PWD/../../../../../usr/local/include/freetype2
DEPENDPATH += $$PWD/../../../../../usr/local/include/freetype2

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libfreetype.a
