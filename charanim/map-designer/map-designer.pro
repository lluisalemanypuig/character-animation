TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    utils.cpp

HEADERS += \
    utils.hpp

# self-includes
INCLUDEPATH += ../

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
