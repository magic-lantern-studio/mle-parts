TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    Mle2dImageActor \
    Mle3dCameraActor \
    MleEscapeActor

Mle2dImageActor.file = Mle2dImageActor/Mle2dImageActor.pro
Mle3dCameraActor.file = Mle3dCameraActor/Mle3dCameraActor.pro
MleEscapeActor.file = MleEscapeActor/MleEscapeActor.pro
