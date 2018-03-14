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
    Skybox.cpp


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
    Skybox.h

DISTFILES += \
    scene.vs \
    shaders/scene.fs \
    shaders/scene.vs \
    textures/grass.jpg \
    textures/grass_test.jpg \
    textures/grassHill_test.jpg \
    textures/grassHill.jpg \
    textures/sand.jpg \
    textures/water.jpg \
    textures/water.png \
    textures/water2.png \
    textures/water2.jpg \
    textures/underwater_sand.jpg \
    textures/sand2.jpg \
    textures/grass2.jpg \
    textures/grass3.jpg \
    textures/grass4.jpg \
    textures/cubemap1/back.png \
    textures/cubemap1/down.png \
    textures/cubemap1/front.png \
    textures/cubemap1/left.png \
    textures/cubemap1/right.png \
    textures/cubemap1/up.png \
    shaders/skybox.vs \
    shaders/skybox.fs
