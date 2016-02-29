#############################################################################
# Makefile for building: SDLOpenGL
# Generated by qmake (3.0) (Qt 5.5.0)
# Project:  ed2.pro
# Template: app
# Command: /opt/qt/5.5/gcc_64/bin/qmake -spec linux-clang CONFIG+=debug -o Makefile ed2.pro
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = clang
CXX           = clang++
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -pipe -I/usr/local/include/SDL2 -D_REENTRANT -std=c++11 -g -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I. -Iinclude -I/opt/qt/5.5/gcc_64/include -I/opt/qt/5.5/gcc_64/include/QtGui -I/opt/qt/5.5/gcc_64/include/QtCore -I. -I/opt/qt/5.5/gcc_64/mkspecs/linux-clang
QMAKE         = /opt/qt/5.5/gcc_64/bin/qmake
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
DISTNAME      = SDLOpenGL1.0.0
DISTDIR = /home/i7620560/Documents/programming/CA1/projects/Elite_Dangerous_v2/obj/SDLOpenGL1.0.0
LINK          = clang++
LFLAGS        = -ccc-gcc-name g++ -Wl,-rpath,/opt/qt/5.5/gcc_64 -Wl,-rpath,/opt/qt/5.5/gcc_64/lib
LIBS          = $(SUBLIBS) -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -L/opt/qt/5.5/gcc_64/lib -lQt5Gui -L/usr/lib64 -lQt5Core -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
SED           = sed
STRIP         = strip

####### Output directory

OBJECTS_DIR   = obj/

####### Files

SOURCES       = main.cpp \
		src/base.cpp \
		src/common.cpp \
		src/enemy.cpp \
		src/faction.cpp \
		src/laser.cpp \
		src/missile.cpp \
		src/pfx.cpp \
		src/player.cpp \
		src/ship.cpp \
		src/squad.cpp \
		src/stardust.cpp \
		src/stardust_sprite.cpp \
		src/util.cpp \
		src/vectors.cpp 
OBJECTS       = obj/main.o \
		obj/base.o \
		obj/common.o \
		obj/enemy.o \
		obj/faction.o \
		obj/laser.o \
		obj/missile.o \
		obj/pfx.o \
		obj/player.o \
		obj/ship.o \
		obj/squad.o \
		obj/stardust.o \
		obj/stardust_sprite.o \
		obj/util.o \
		obj/vectors.o
DIST          = /opt/qt/5.5/gcc_64/mkspecs/features/spec_pre.prf \
		/opt/qt/5.5/gcc_64/mkspecs/common/unix.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/linux.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/sanitize.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/gcc-base.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/gcc-base-unix.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/clang.conf \
		/opt/qt/5.5/gcc_64/mkspecs/qconfig.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dcore.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dcore_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dinput.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dinput_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquick.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquick_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquickrenderer.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquickrenderer_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3drenderer.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3drenderer_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_bluetooth.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_bluetooth_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_bootstrap_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_clucene_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_concurrent.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_concurrent_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_core.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_core_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_dbus.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_dbus_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_declarative.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_declarative_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_designer.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_designer_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_designercomponents_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_enginio.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_enginio_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_gui.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_gui_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_help.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_help_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_location.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_location_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimedia.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimedia_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimediawidgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimediawidgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_network.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_network_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_nfc.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_nfc_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_opengl.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_opengl_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_openglextensions.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_openglextensions_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_platformsupport_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_positioning.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_positioning_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_printsupport.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_printsupport_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qml.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qml_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qmldevtools_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qmltest.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qmltest_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quick.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quick_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quickparticles_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quickwidgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quickwidgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_script.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_script_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_scripttools.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_scripttools_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sensors.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sensors_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_serialport.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_serialport_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sql.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sql_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_svg.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_svg_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_testlib.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_testlib_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_uiplugin.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_uitools.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_uitools_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webchannel.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webchannel_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webengine.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webengine_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginecore.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginecore_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginewidgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginewidgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkit.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkit_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkitwidgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkitwidgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_websockets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_websockets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webview_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_widgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_widgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_x11extras.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_x11extras_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xml.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xml_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xmlpatterns.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xmlpatterns_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/features/qt_functions.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/qt_config.prf \
		/opt/qt/5.5/gcc_64/mkspecs/linux-clang/qmake.conf \
		/opt/qt/5.5/gcc_64/mkspecs/features/spec_post.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/exclusive_builds.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/default_pre.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/resolve_config.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/default_post.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/warn_on.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/qt.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/resources.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/moc.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/unix/opengl.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/unix/thread.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/testcase_targets.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/exceptions.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/yacc.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/lex.prf \
		ed2.pro include/base.hpp \
		include/base.hpp \
		include/common.hpp \
		include/enemy.hpp \
		include/faction.hpp \
		include/file.hpp \
		include/laser.hpp \
		include/missile.hpp \
		include/pfx.hpp \
		include/player.hpp \
		include/renderer.hpp \
		include/rendering.hpp \
		include/sfx.hpp \
		include/shapes.hpp \
		include/ship.hpp \
		include/ship_presets.hpp \
		include/sim_time.hpp \
		include/squad.hpp \
		include/stardust.hpp \
		include/stardust_sprite.hpp \
		include/ui.hpp \
		include/ui_classes.hpp \
		include/universe.hpp \
		include/user_input.hpp \
		include/util.hpp \
		include/vectors.hpp \
		include/weapons.hpp main.cpp \
		src/base.cpp \
		src/common.cpp \
		src/enemy.cpp \
		src/faction.cpp \
		src/laser.cpp \
		src/missile.cpp \
		src/pfx.cpp \
		src/player.cpp \
		src/ship.cpp \
		src/squad.cpp \
		src/stardust.cpp \
		src/stardust_sprite.cpp \
		src/util.cpp \
		src/vectors.cpp
QMAKE_TARGET  = SDLOpenGL
DESTDIR       = #avoid trailing-slash linebreak
TARGET        = SDLOpenGL


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: ed2.pro /opt/qt/5.5/gcc_64/mkspecs/linux-clang/qmake.conf /opt/qt/5.5/gcc_64/mkspecs/features/spec_pre.prf \
		/opt/qt/5.5/gcc_64/mkspecs/common/unix.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/linux.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/sanitize.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/gcc-base.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/gcc-base-unix.conf \
		/opt/qt/5.5/gcc_64/mkspecs/common/clang.conf \
		/opt/qt/5.5/gcc_64/mkspecs/qconfig.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dcore.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dcore_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dinput.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dinput_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquick.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquick_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquickrenderer.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquickrenderer_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3drenderer.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3drenderer_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_bluetooth.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_bluetooth_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_bootstrap_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_clucene_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_concurrent.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_concurrent_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_core.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_core_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_dbus.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_dbus_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_declarative.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_declarative_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_designer.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_designer_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_designercomponents_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_enginio.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_enginio_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_gui.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_gui_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_help.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_help_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_location.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_location_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimedia.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimedia_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimediawidgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimediawidgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_network.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_network_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_nfc.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_nfc_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_opengl.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_opengl_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_openglextensions.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_openglextensions_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_platformsupport_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_positioning.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_positioning_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_printsupport.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_printsupport_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qml.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qml_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qmldevtools_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qmltest.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qmltest_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quick.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quick_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quickparticles_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quickwidgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quickwidgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_script.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_script_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_scripttools.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_scripttools_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sensors.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sensors_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_serialport.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_serialport_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sql.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sql_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_svg.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_svg_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_testlib.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_testlib_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_uiplugin.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_uitools.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_uitools_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webchannel.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webchannel_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webengine.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webengine_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginecore.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginecore_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginewidgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginewidgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkit.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkit_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkitwidgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkitwidgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_websockets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_websockets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webview_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_widgets.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_widgets_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_x11extras.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_x11extras_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xml.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xml_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xmlpatterns.pri \
		/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xmlpatterns_private.pri \
		/opt/qt/5.5/gcc_64/mkspecs/features/qt_functions.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/qt_config.prf \
		/opt/qt/5.5/gcc_64/mkspecs/linux-clang/qmake.conf \
		/opt/qt/5.5/gcc_64/mkspecs/features/spec_post.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/exclusive_builds.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/default_pre.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/resolve_config.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/default_post.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/warn_on.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/qt.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/resources.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/moc.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/unix/opengl.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/unix/thread.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/testcase_targets.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/exceptions.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/yacc.prf \
		/opt/qt/5.5/gcc_64/mkspecs/features/lex.prf \
		ed2.pro \
		/opt/qt/5.5/gcc_64/lib/libQt5Gui.prl \
		/opt/qt/5.5/gcc_64/lib/libQt5Core.prl
	$(QMAKE) -spec linux-clang CONFIG+=debug -o Makefile ed2.pro
/opt/qt/5.5/gcc_64/mkspecs/features/spec_pre.prf:
/opt/qt/5.5/gcc_64/mkspecs/common/unix.conf:
/opt/qt/5.5/gcc_64/mkspecs/common/linux.conf:
/opt/qt/5.5/gcc_64/mkspecs/common/sanitize.conf:
/opt/qt/5.5/gcc_64/mkspecs/common/gcc-base.conf:
/opt/qt/5.5/gcc_64/mkspecs/common/gcc-base-unix.conf:
/opt/qt/5.5/gcc_64/mkspecs/common/clang.conf:
/opt/qt/5.5/gcc_64/mkspecs/qconfig.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dcore.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dcore_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dinput.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dinput_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquick.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquick_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquickrenderer.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3dquickrenderer_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3drenderer.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_3drenderer_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_bluetooth.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_bluetooth_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_bootstrap_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_clucene_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_concurrent.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_concurrent_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_core.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_core_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_dbus.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_dbus_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_declarative.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_declarative_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_designer.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_designer_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_designercomponents_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_enginio.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_enginio_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_gui.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_gui_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_help.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_help_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_location.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_location_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimedia.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimedia_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimediawidgets.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_multimediawidgets_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_network.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_network_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_nfc.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_nfc_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_opengl.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_opengl_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_openglextensions.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_openglextensions_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_platformsupport_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_positioning.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_positioning_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_printsupport.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_printsupport_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qml.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qml_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qmldevtools_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qmltest.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qmltest_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quick.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quick_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quickparticles_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quickwidgets.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_quickwidgets_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_script.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_script_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_scripttools.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_scripttools_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sensors.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sensors_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_serialport.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_serialport_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sql.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_sql_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_svg.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_svg_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_testlib.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_testlib_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_uiplugin.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_uitools.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_uitools_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webchannel.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webchannel_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webengine.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webengine_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginecore.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginecore_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginewidgets.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webenginewidgets_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkit.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkit_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkitwidgets.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webkitwidgets_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_websockets.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_websockets_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_webview_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_widgets.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_widgets_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_x11extras.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_x11extras_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xcb_qpa_lib_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xml.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xml_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xmlpatterns.pri:
/opt/qt/5.5/gcc_64/mkspecs/modules/qt_lib_xmlpatterns_private.pri:
/opt/qt/5.5/gcc_64/mkspecs/features/qt_functions.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/qt_config.prf:
/opt/qt/5.5/gcc_64/mkspecs/linux-clang/qmake.conf:
/opt/qt/5.5/gcc_64/mkspecs/features/spec_post.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/exclusive_builds.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/default_pre.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/resolve_config.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/default_post.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/warn_on.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/qt.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/resources.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/moc.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/unix/opengl.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/unix/thread.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/testcase_targets.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/exceptions.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/yacc.prf:
/opt/qt/5.5/gcc_64/mkspecs/features/lex.prf:
ed2.pro:
/opt/qt/5.5/gcc_64/lib/libQt5Gui.prl:
/opt/qt/5.5/gcc_64/lib/libQt5Core.prl:
qmake: FORCE
	@$(QMAKE) -spec linux-clang CONFIG+=debug -o Makefile ed2.pro

qmake_all: FORCE


all: Makefile $(TARGET)

dist: distdir FORCE
	(cd `dirname $(DISTDIR)` && $(TAR) $(DISTNAME).tar $(DISTNAME) && $(COMPRESS) $(DISTNAME).tar) && $(MOVE) `dirname $(DISTDIR)`/$(DISTNAME).tar.gz . && $(DEL_FILE) -r $(DISTDIR)

distdir: FORCE
	@test -d $(DISTDIR) || mkdir -p $(DISTDIR)
	$(COPY_FILE) --parents $(DIST) $(DISTDIR)/
	$(COPY_FILE) --parents include/base.hpp include/base.hpp include/common.hpp include/enemy.hpp include/faction.hpp include/file.hpp include/laser.hpp include/missile.hpp include/pfx.hpp include/player.hpp include/renderer.hpp include/rendering.hpp include/sfx.hpp include/shapes.hpp include/ship.hpp include/ship_presets.hpp include/sim_time.hpp include/squad.hpp include/stardust.hpp include/stardust_sprite.hpp include/ui.hpp include/ui_classes.hpp include/universe.hpp include/user_input.hpp include/util.hpp include/vectors.hpp include/weapons.hpp $(DISTDIR)/
	$(COPY_FILE) --parents main.cpp src/base.cpp src/common.cpp src/enemy.cpp src/faction.cpp src/laser.cpp src/missile.cpp src/pfx.cpp src/player.cpp src/ship.cpp src/squad.cpp src/stardust.cpp src/stardust_sprite.cpp src/util.cpp src/vectors.cpp $(DISTDIR)/


clean: compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


distclean: clean 
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


####### Sub-libraries

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

obj/main.o: main.cpp src/sim_time.cpp \
		include/sim_time.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/common.hpp \
		include/sfx.hpp \
		src/universe.cpp \
		include/universe.hpp \
		include/weapons.hpp \
		include/stardust.hpp \
		include/base.hpp \
		include/stardust_sprite.hpp \
		include/ship.hpp \
		src/ship_presets.cpp \
		include/ship_presets.hpp \
		include/enemy.hpp \
		include/laser.hpp \
		include/player.hpp \
		include/pfx.hpp \
		include/missile.hpp \
		include/ui_classes.hpp \
		include/ui.hpp \
		include/file.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/main.o main.cpp

obj/base.o: src/base.cpp include/base.hpp \
		include/vectors.hpp \
		include/common.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/base.o src/base.cpp

obj/common.o: src/common.cpp include/common.hpp \
		include/vectors.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/common.o src/common.cpp

obj/enemy.o: src/enemy.cpp include/enemy.hpp \
		include/ship.hpp \
		include/common.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/base.hpp \
		src/ship_presets.cpp \
		include/ship_presets.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/enemy.o src/enemy.cpp

obj/faction.o: src/faction.cpp include/faction.hpp \
		include/squad.hpp \
		include/enemy.hpp \
		include/ship.hpp \
		include/common.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/base.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/faction.o src/faction.cpp

obj/laser.o: src/laser.cpp include/laser.hpp \
		include/base.hpp \
		include/vectors.hpp \
		include/enemy.hpp \
		include/ship.hpp \
		include/common.hpp \
		include/util.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/laser.o src/laser.cpp

obj/missile.o: src/missile.cpp include/missile.hpp \
		include/ship.hpp \
		include/common.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/base.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/missile.o src/missile.cpp

obj/pfx.o: src/pfx.cpp include/pfx.hpp \
		include/base.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/common.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/pfx.o src/pfx.cpp

obj/player.o: src/player.cpp include/player.hpp \
		include/ship.hpp \
		include/common.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/base.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/player.o src/player.cpp

obj/ship.o: src/ship.cpp include/ship.hpp \
		include/common.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/base.hpp \
		include/weapons.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/ship.o src/ship.cpp

obj/squad.o: src/squad.cpp include/squad.hpp \
		include/enemy.hpp \
		include/ship.hpp \
		include/common.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/base.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/squad.o src/squad.cpp

obj/stardust.o: src/stardust.cpp include/stardust.hpp \
		include/base.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/common.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/stardust.o src/stardust.cpp

obj/stardust_sprite.o: src/stardust_sprite.cpp include/stardust_sprite.hpp \
		include/stardust.hpp \
		include/base.hpp \
		include/vectors.hpp \
		include/util.hpp \
		include/common.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/stardust_sprite.o src/stardust_sprite.cpp

obj/util.o: src/util.cpp include/util.hpp \
		include/vectors.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/util.o src/util.cpp

obj/vectors.o: src/vectors.cpp include/vectors.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/vectors.o src/vectors.cpp

####### Install

install:  FORCE

uninstall:  FORCE

FORCE:

