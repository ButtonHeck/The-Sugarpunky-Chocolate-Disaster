TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

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
    FontManager.cpp


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
    FontManager.h

DISTFILES += \
    scene.vs \
    textures/sand.jpg \
    textures/underwater_sand.jpg \
    textures/cubemap1/back.png \
    textures/cubemap1/down.png \
    textures/cubemap1/front.png \
    textures/cubemap1/left.png \
    textures/cubemap1/right.png \
    textures/cubemap1/up.png \
    shaders/skybox.vs \
    shaders/skybox.fs \
    textures/cubemap1scaled/back.png \
    textures/cubemap1scaled/down.png \
    textures/cubemap1scaled/front.png \
    textures/cubemap1scaled/left.png \
    textures/cubemap1scaled/right.png \
    textures/cubemap1scaled/up.png \
    shaders/water.fs \
    shaders/hills.fs \
    textures/hill.jpg \
    textures/hill_specular.jpg \
    textures/hill_test.jpg \
    shaders/sand.fs \
    shaders/base.fs \
    shaders/underwater.fs \
    textures/cubemap1fx/back.png \
    textures/cubemap1fx/down.png \
    textures/cubemap1fx/front.png \
    textures/cubemap1fx/left.png \
    textures/cubemap1fx/right.png \
    textures/cubemap1fx/up.png \
    textures/base.jpg \
    textures/base_normal.jpg \
    textures/base_specular.jpg \
    textures/base_test.jpg \
    textures/water.jpg \
    textures/water.png \
    textures/water_specular.png \
    textures/base2.jpg \
    textures/hill2.jpg \
    textures/sand2.jpg \
    shaders/model.vs \
    shaders/model.fs \
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
    shaders/terrainVertex.vs \
    fonts/GentiumPlus-R.ttf \
    shaders/font.vs \
    shaders/font.fs \
    fonts/OCTAPOST_1.ttf

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lassimp

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

unix:!macx: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lfreetype

INCLUDEPATH += $$PWD/../../../../../usr/local/include/freetype2
DEPENDPATH += $$PWD/../../../../../usr/local/include/freetype2

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libfreetype.a
