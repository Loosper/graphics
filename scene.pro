TEMPLATE = app
TARGET = scene
# /* INCLUDEPATH */

QT += widgets opengl gui
CONFIG += c++11
LIBS += -lGLU

SOURCES += main.cpp Scene.cpp
HEADERS += Scene.h
