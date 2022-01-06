TEMPLATE = app
TARGET = scene
# /* INCLUDEPATH */

QT += widgets opengl gui
CONFIG += c++11
LIBS += -lGLU

SOURCES += main.cpp Scene.cpp Drawer.cpp Rocket.cpp SolarSystem.cpp materials.cpp
HEADERS += Scene.h Drawer.h Rocket.h Material.h SolarSystem.h
