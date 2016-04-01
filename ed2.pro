TARGET = "Captain Fractal: Attack of the Space Communists"

OBJECTS_DIR = obj

QT += gui opengl core

isEqual(QT_MAJOR_VERSION, 5) {
        cache()
        DEFINES += QT5BUILD
}

win32 {
    INCLUDEPATH += C:\SDL2\i686-w64-mingw32\include\SDL2
    INCLUDEPATH += C:\SDL2\i686-w64-mingw32\include
    LIBS += -L"C:/SDL2/lib/x64/" -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer
    #LIBS += -L"C:\SDL2\i686-w64-mingw32\lib" -mwindows -lmingw32 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2main -lSDL2
    LIBS += -L"C:/NGL/lib/NGL.lib" -lNGL
}

INCLUDEPATH += $$PWD/include

MOC_DIR = moc

SOURCES += $$PWD/main.cpp \
           $$PWD/src/*.cpp \
           $$PWD/src/ui/*.cpp

HEADERS += $$PWD/include/base.hpp \
           $$PWD/include/*.hpp \
           $$PWD/include/ui/*.hpp

CONFIG += console

!equals(PWD, $${OUT_PWD}){
        copydata.commands = echo "creating destination dirs" ;
        # now make a dir
        copydata.commands += mkdir -p $$OUT_PWD/shaders ;
        copydata.commands += echo "copying files" ;
        # then copy the files
        copydata.commands += $(COPY_DIR) $$PWD/shaders/* $$OUT_PWD/shaders/ ;
        # now make sure the first target is built before copy
        first.depends = $(first) copydata
        export(first.depends)
        export(copydata.commands)
        # now add it as an extra target
        QMAKE_EXTRA_TARGETS += first copydata
}

win32: {
    PRE_TARGETDEPS += C:/NGL/lib/NGL.lib
    INCLUDEPATH += -I c:/boost
    INCLUDEPATH += $$(HOMEDRIVE)/NGL/include/
    DEFINES += GL42
    DEFINES += WIN32
    DEFINES += _WIN32
    DEFINES += _USE_MATH_DEFINES
    LIBS += -LC:/NGL/lib/ -lNGL
    DEFINES += NO_DLL
}

OTHER_FILES += readme.md \
            += ./shaders/*.glsl

CONFIG += console

unix {
    QMAKE_CXXFLAGS += $$system(sdl2-config --cflags)
    LIBS += $$system(sdl2-config --libs)
    LIBS += -lSDL2_image -lSDL2_mixer -lSDL2_ttf

}

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        message("including $HOME/NGL")
        include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
        message("Using custom NGL location")
        include($(NGLDIR)/UseNGL.pri)
}
