#-----------------------------
#
# Project created by LordWhiro
#
#-----------------------------

QT       += core gui opengl

TARGET = Castle3D
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp \
    Window.cpp \
    OBJLoader.cpp \
    UpdateThread.cpp \
    Renderer.cpp \
    PrefabBase.cpp \
    Castle.cpp \
    Voxel.cpp \
    Prefab.cpp

HEADERS  += \
    Window.hpp \
    OBJLoader.hpp \
    UpdateThread.hpp \
    Renderer.hpp \
    PrefabBase.hpp \
    Castle.hpp \
    Voxel.hpp \
    Prefab.hpp \
    CastleRessources.hpp

OTHER_FILES += \
    programs/01_default/vertex.vsh \
    programs/01_default/fragment.fsh \
    programs/02_rooted/vertex.vsh \
    programs/02_rooted/fragment.fsh \
    programs/03_squared/vertex.vsh \
    programs/03_squared/fragment.fsh \
    testfrag.fsh


