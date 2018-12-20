TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    charanim.hpp \
    vec_helper.hpp \
    terrain/terrain.hpp \
    definitions.hpp \
    terrain/regular_grid.hpp \
    terrain/ray_rasterize.hpp \
    terrain/ray_rasterize_4_way.hpp \
    utils/utils.hpp \
    utils/indexed_minheap.hpp \
    utils/indexed_minheap.cpp

SOURCES += main.cpp \
    charanim_variables.cpp \
    charanim_render.cpp \
    sim_00.cpp \
    terrain/terrain.cpp \
    terrain/regular_grid.cpp \
    terrain/ray_rasterize.cpp \
    terrain/ray_rasterize_4_way.cpp \
    sim_01.cpp \
    charanim_init.cpp \
    utils/utils.cpp

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
	exists(/usr/lib/cal3d) {
		LIBS += -lcal3d
	}
	else {
		LIBS += -L../../../cal3d/lib64/libcal3d.la
		INCLUDEPATH += -I ../../../cal3d/include
	}
}
