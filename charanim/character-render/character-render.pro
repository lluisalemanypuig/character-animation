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
	exists(/usr/local/include/cal3d) {
		LIBS += -lcal3d
	}
    else {
        LIBS += -L../../../cal3d/lib64/ -lcal3d
        INCLUDEPATH += -I ../../../cal3d/include
    }
}

# Files
SOURCES += \
    main.cpp \
    utils.cpp

HEADERS += \
    utils.hpp
