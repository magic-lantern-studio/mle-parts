TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    Mle2dImageMediaref \
    Mle3dColorMapMediaref \
    Mle3dTextureMapMediaref \
    Mle3dModelMediaref

Mle2dImageMediaref.file = Mle2dImageMediaref/2dimagemref.pro
Mle3dColorMapMediaref.file = Mle3dColorMapMediaref/3dcolormapmref.pro
Mle3dTextureMapMediaref.file = Mle3dTextureMapMediaref/3dtexturemapmref.pro
Mle3dModelMediaref.file = Mle3dModelMediaref/3dmodelmref.pro
