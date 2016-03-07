TARGET = SDLOpenGL

OBJECTS_DIR = obj

INCLUDEPATH += $$PWD/include

SOURCES += $$PWD/main.cpp \
           $$PWD/src/*.cpp \
           $$PWD/src/ui/*.cpp

HEADERS += $$PWD/include/base.hpp \
           $$PWD/include/*.hpp \
           $$PWD/include/ui/*.hpp

QMAKE_CXXFLAGS += -std=c++11

#include($(HOME)/NGL/UseNGL.pri)

OTHER_FILES += readme.md
#           += resources/fonts/pix.TTF

CONFIG += COMPILE_MODE_SDL

COMPILE_MODE_SDL {
    unix {
        QMAKE_CXXFLAGS += $$system(sdl2-config --cflags)
        LIBS += $$system(sdl2-config --libs)
        LIBS += -lSDL2_image -lSDL2_mixer -lSDL2_ttf
    }

    win32 {
        INCLUDEPATH += C:/SDL2/i686-w64-mingw32/include
        INCLUDEPATH += C:/SDL2/i686-w64-mingw32/include/SDL2
        LIBS += -L"C:/SDL2/i686-w64-mingw32/lib"
        LIBS += -lmingw32 -mwindows -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2
    }
}

COMPILE_MODE_NGL
{
    unix {
        include($(HOME)/NGL/UseNGL.pri)
    }
}
