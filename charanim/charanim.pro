TEMPLATE = subdirs

SUBDIRS += \
    render \
	anim \
    character-render

DISTFILES += \
	shaders/flat.frag \
	shaders/materials.frag \
	shaders/textures.frag \
	shaders/flat.vert \
	shaders/materials.vert \
    shaders/textures.vert \
    shaders/character.vert \
    shaders/character.frag \
    maps/map_0.txt \
    models/sphere.obj \
    models/sphere.mtl \
    models/sphere-wireframe.obj \
    models/sphere-wireframe.mtl
