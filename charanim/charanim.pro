TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    render/cameras/observer.hpp \
    render/cameras/orthogonal.hpp \
    render/cameras/perspective.hpp \
    render/shader/shader_helper.hpp \
    render/shader/shader.hpp \
    render/box.hpp \
    render/include_gl.hpp \
    charanim.hpp \
    utils.hpp \
    render/viewer.hpp

SOURCES += main.cpp \
    render/cameras/observer.cpp \
    render/cameras/orthogonal.cpp \
    render/cameras/perspective.cpp \
    render/shader/shader_helper.cpp \
    render/shader/shader.cpp \
    render/box.cpp \
    utils.cpp \
    charanim_variables.cpp \
    charanim_render.cpp \
    render/viewer.cpp \
    charanim_init.cpp

# self-includes
INCLUDEPATH += ../

# physim library
CONFIG(debug, debug|release) {
    LIBS += -L../../physics-simulator/physim-debug/ -lphysim
    PRE_TARGETDEPS += ../../physics-simulator/physim-debug/libphysim.a
}
CONFIG(release, debug|release) {
    LIBS += -L../../physics-simulator/physim-release/ -lphysim
    PRE_TARGETDEPS += ../../physics-simulator/physim-release/libphysim.a
}
INCLUDEPATH += ../../physics-simulator/
DEPENDPATH += ../../physics-simulator/

# cal3d
unix {
    exists(/usr/lib/cal3d/) {
        LIBS += -lcal3d
    }
    else {
        LIBS += -L../../cal3d/lib64 -lcal3d
        INCLUDEPATH += -I ../../cal3d/include
    }
}

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
