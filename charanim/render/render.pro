TEMPLATE = lib
TARGET = render

CONFIG += c++11
CONFIG += staticlib
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_DEBUG += -DDEBUG
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

HEADERS += \
    obj_reader.hpp \
    box.hpp \
    include_gl.hpp \
    cameras/orthogonal.hpp \
    cameras/perspective.hpp \
    cameras/observer.hpp \
    geometry/rsphere.hpp \
    geometry/rtriangle.hpp \
    geometry/rplane.hpp \
    geometry/rrectangle.hpp \
    geometry/rgeometry.hpp \
    textures/stb_image.h \
    textures/std_image.hpp \
    textures/texture_loader.hpp \
    scene/viewer.hpp \
    shader/shader.hpp \
    shader/shader_helper.hpp \
    mesh/tri_mesh.hpp \
    mesh/rendered_tri_mesh.hpp \
    mesh/tri_mesh_utils.hpp

SOURCES += \
    obj_reader.cpp \
    box.cpp \
    geometry/rplane.cpp \
    geometry/rrectangle.cpp \
    geometry/rsphere.cpp \
    geometry/rtriangle.cpp \
    cameras/orthogonal.cpp \
    cameras/perspective.cpp \
    cameras/observer.cpp \
    geometry/rgeometry.cpp \
    textures/texture_loader.cpp \
    scene/viewer.cpp \
    shader/shader.cpp \
    shader/shader_helper.cpp \
    mesh/rendered_tri_mesh.cpp \
    mesh/tri_mesh.cpp \
    mesh/tri_mesh_utils.cpp

# for 'self' includes
INCLUDEPATH += ..

# physim library
INCLUDEPATH += ../../../physics-simulator/
DEPENDPATH += ../../../physics-simulator/

