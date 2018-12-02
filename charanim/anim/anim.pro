TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    charanim.hpp \
    utils.hpp \
    vec_helper.hpp

SOURCES += main.cpp \
    utils.cpp \
    charanim_variables.cpp \
    charanim_render.cpp \
    charanim_init.cpp

# self-includes
INCLUDEPATH += ../

# render (model, obj reader, ...)
LIBS += -L../render/ -lrender
PRE_TARGETDEPS += ../render/librender.a
INCLUDEPATH += ..
DEPENDPATH += ..

# physim library
CONFIG(debug, debug|release) {
    LIBS += -L../../../physics-simulator/physim-debug/ -lphysim
    PRE_TARGETDEPS += ../../../physics-simulator/physim-debug/libphysim.a
}
CONFIG(release, debug|release) {
    LIBS += -L../../../physics-simulator/physim-release/ -lphysim
    PRE_TARGETDEPS += ../../../physics-simulator/physim-release/libphysim.a
}
INCLUDEPATH += ../../../physics-simulator/
DEPENDPATH += ../../../physics-simulator/

# OpenGL
LIBS += -lglut -lGLU -lGLEW
unix {
    exists(/usr/lib/nvidia-304/) {
        LIBS += -L/usr/lib/nvidia-304/ -lGL
    }
    else {
        LIBS += -lGL
    }
}

# cal3d
unix {
    exists(/usr/lib/) {
		LIBS += -lcal3d
	}
	else {
		LIBS += -L../../../cal3d/lib64/libcal3d.la
		INCLUDEPATH += -I ../../../cal3d/include
	}
}
