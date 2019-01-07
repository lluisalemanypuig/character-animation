TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

# self-includes
INCLUDEPATH += ../

# OpenMP
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

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
    exists(/usr/include/cal3d) {
        LIBS += -lcal3d
    }
    else {
        LIBS += -L../../../cal3d/lib64/ -lcal3d
        INCLUDEPATH += -I ../../../cal3d/include
    }
}

# Files
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
    utils/indexed_minheap.cpp \
    sim_1xx.hpp

SOURCES += main.cpp \
    charanim_variables.cpp \
    charanim_render.cpp \
    terrain/terrain.cpp \
    terrain/regular_grid.cpp \
    terrain/ray_rasterize.cpp \
    terrain/ray_rasterize_4_way.cpp \
    charanim_init.cpp \
    utils/utils.cpp \
    sim_000.cpp \
    sim_100.cpp \
    sim_101.cpp \
    sim_102.cpp \
    sim_103.cpp \
    sim_200.cpp \
    sim_104.cpp \
    sim_1xx.cpp
