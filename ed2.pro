TARGET = SDLOpenGL

OBJECTS_DIR = obj

INCLUDEPATH += $$PWD/include

SOURCES += $$PWD/main.cpp \
           $$PWD/src/base.cpp \
           $$PWD/src/enemy.cpp \
           $$PWD/src/laser.cpp \
           $$PWD/src/missile.cpp \
           $$PWD/src/pfx.cpp \
           $$PWD/src/player.cpp \
           $$PWD/src/squad.cpp \
           $$PWD/src/ship.cpp \
           $$PWD/src/ship_presets.cpp \
           $$PWD/src/stardust.cpp \
           $$PWD/src/stardust_sprite.cpp \
           $$PWD/src/sim_time.cpp \
           $$PWD/src/universe.cpp

HEADERS += $$PWD/include/base.hpp \
           $$PWD/include/enemy.hpp \
           $$PWD/include/laser.hpp \
           $$PWD/include/missile.hpp \
           $$PWD/include/pfx.hpp \
           $$PWD/include/player.hpp \
           $$PWD/include/squad.hpp \
           $$PWD/include/ship.hpp \
           $$PWD/include/stardust.hpp \
           $$PWD/include/stardust_sprite.hpp \
           $$PWD/include/universe.hpp \
           $$PWD/include/file.hpp \
           $$PWD/include/sim_time.hpp \
           $$PWD/include/common.hpp \
           $$PWD/include/user_input.hpp \
           $$PWD/include/common.hpp \
           $$PWD/include/ui.hpp \
           $$PWD/include/ui_classes.hpp \
           $$PWD/include/weapons.hpp

QMAKE_CXXFLAGS += $$system(sdl2-config --cflags)
QMAKE_CXXFLAGS += -std=c++11

LIBS += $$system(sdl2-config --libs)
LIBS += -lSDL2_image -lSDL2_mixer -lSDL2_ttf

#include($(HOME)/NGL/UseNGL.pri)

OTHER_FILES += readme.md
#            += resources/fonts/pix.TTF
