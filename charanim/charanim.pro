TEMPLATE = subdirs

SUBDIRS += \
    render \
	anim

DISTFILES += \
	shaders/flat.frag \
	shaders/materials.frag \
	shaders/textures.frag \
	shaders/flat.vert \
	shaders/materials.vert \
    shaders/textures.vert \
    maps/map_0.txt \
    models/sphere.obj \
    models/sphere.mtl \
    models/sphere-wireframe.obj \
    models/sphere-wireframe.mtl
