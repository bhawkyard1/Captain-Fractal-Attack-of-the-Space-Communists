TARGET = SDLOpenGL

OBJECTS_DIR = obj

INCLUDEPATH += $$PWD/include

SOURCES += $$PWD/main.cpp \
           $$PWD/src/*.cpp \
           $$PWD/src/ui/*.cpp

HEADERS += $$PWD/include/base.hpp \
           $$PWD/include/*.hpp \
           $$PWD/include/ui/*.hpp

QMAKE_CXXFLAGS += $$system(sdl2-config --cflags)
QMAKE_CXXFLAGS += -std=c++11

LIBS += $$system(sdl2-config --libs)
LIBS += -lSDL2_image -lSDL2_mixer -lSDL2_ttf

#include($(HOME)/NGL/UseNGL.pri)

OTHER_FILES += readme.md
#           += resources/fonts/pix.TTF
