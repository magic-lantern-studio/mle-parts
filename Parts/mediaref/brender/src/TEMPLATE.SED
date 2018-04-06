#!/bin/sh

## Sample of how to generate the .cxx and .h files for a web-based
## subclass of an existing media ref.

f=3dcomawm
sed \
    -e "s@TEMPLATE_CLASS_HEADER@<mle/3dcomawm.h>@g" \
    -e "s@TEMPLATE_IFDEF@__MLE_3DCOMAWM_H_@g" \
    -e "s@TEMPLATE_CLASS@Mle3dColorMapWebMediaRef@g" \
    -e "s@TEMPLATE_PARENT_HEADER@<mle/3dcomapm.h>@g" \
    -e "s@TEMPLATE_PARENT@Mle3dColorMapMediaRef@g" \
    -e "s@TEMPLATE_TITLE@3D COLORMAP WEB-capable @g" \
    TEMPLATE.CXX > ${f}.cxx
sed \
    -e "s@TEMPLATE_CLASS_HEADER@<mle/3dcomawm.h>@g" \
    -e "s@TEMPLATE_IFDEF@__MLE_3DCOMAWM_H_@g" \
    -e "s@TEMPLATE_CLASS@Mle3dColorMapWebMediaRef@g" \
    -e "s@TEMPLATE_PARENT_HEADER@<mle/3dcomapm.h>@g" \
    -e "s@TEMPLATE_PARENT@Mle3dColorMapMediaRef@g" \
    -e "s@TEMPLATE_TITLE@3D COLORMAP WEB-capable @g" \
    mle/TEMPLATE.H > mle/${f}.h

