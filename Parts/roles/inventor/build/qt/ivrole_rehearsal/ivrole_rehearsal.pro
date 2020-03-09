TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = \
    ivroles \
    MleIv2dImgRole \
    MleIv2dRole \
    MleIv3dRole

ivroles.file = ivroles/ivroles.pro
MleIv2dImgRole.file = MleIv2dImgRole/MleIv2dImgRole.pro
MleIv2dRole.file = MleIv2dRole/MleIv2dRole.pro
MleIv3dRole.file = MleIv3dRole/MleIv3dRole.pro
