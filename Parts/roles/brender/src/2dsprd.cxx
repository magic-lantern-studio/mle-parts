/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dsprd.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 2D Sprite Role.
 *
 * @author Mark S. Millard
 * @date May 1, 2003
 */

// COPYRIGHT_BEGIN
//
//  Copyright (C) 2000-2007  Wizzer Works
//
//  Wizzer Works makes available all content in this file ("Content").
//  Unless otherwise indicated below, the Content is provided to you
//  under the terms and conditions of the Common Public License Version 1.0
//  ("CPL"). A copy of the CPL is available at
//
//      http://opensource.org/licenses/cpl1.0.php
//
//  For purposes of the CPL, "Program" will mean the Content.
//
//  For information concerning this Makefile, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

/************************** Begin common includes ***************************/
#include "mle/mlMalloc.h"
#include "mle/MleActor.h"
#include "mle/MleLoad.h"
#include "mle/filemref.h"
#include "mle/2dspra.h"
#include "mle/2dsprd.h"
/************************** End common  includes ****************************/



/************************** Begin SGI includes ******************************/
#if defined(MLE_REHEARSAL) || defined(__sgi)
#include <GL/gl.h>
#include <il/ilConfigure.h>
#include <il/ilFileImg.h>
#include <il/ilGenericImgFile.h>
#include "mle/sgibrstage.h"
#endif // MLE_REHEARSAL or __sgi
/************************** End SGI includes ********************************/



/************************** Begin WIN32 includes ****************************/
#if defined(WIN32)
#include "mle/mlFileio.h"
#include "mle/dib.h"
#include "mle/pcstage.h"
#include "mle/br2dset.h"
#endif // WIN32
/************************** End WIN32 includes ******************************/


// Local debug flag
// #define _MLE_SPRITE_DEBUG_ 1

#define MACHINE_POINTER_SIZE sizeof(void*)


MLE_ROLE_SOURCE(Mle2dSpriteRole, Mle2dRole);

Mle2dSpriteRole::Mle2dSpriteRole(MleActor *actor)
 : Mle2dRole(actor)
{
    position.setValue(ML_SCALAR(0), ML_SCALAR(0));
    visible = rescale = FALSE;
    currentPage = pageCount = 0;
    pageHeight = pageWidth = pageSize = 0;
    xscale = yscale = 1.0;
#if defined(MLE_REHEARSAL) || defined(__sgi)
    spriteData = NULL;
#endif // MLE_REHEARSAL or __sgi

    // For now, assume a transparent channel is used.
    // xxx Ideally, this information would be
    //     embedded in the sprite media. These two
    //     member variables are a reminder of this
    //     possibility.
    transparencyCheck = transparent = TRUE;
}


Mle2dSpriteRole::~Mle2dSpriteRole(void)
{
#if defined(MLE_REHEARSAL) || defined(__sgi)
    if (spriteData)
      mlFree(spriteData);
#endif // MLE_REHEARSAL or __sgi
}

void
Mle2dSpriteRole::setPosition(MleVector2 &p)
{
    position.setValue(p.getValue());

  // Update bounding box used by rehearsal player.
#if defined(MLE_REHEARSAL) || defined(__sgi)
    if(spriteData)
	{
		MlScalar min[2], max[2];
		min[0] = p[0];
		min[1] = p[1];
		max[0] = p[0] + mlLongToScalar(pageWidth) - ML_SCALAR_ONE;
		max[1] = p[1] + mlLongToScalar(pageHeight) - ML_SCALAR_ONE;
		setBounds(min, max);
    }
#endif // MLE_REHEARSAL or __sgi
}

void
Mle2dSpriteRole::setScale(MlScalar xs, MlScalar ys)
{
    xscale = mlScalarToFloat(xs);
    yscale = mlScalarToFloat(ys);

    // Cache scale state
    if ( (xscale == 1.0) && (yscale == 1.0) )
        rescale = FALSE;
    else
        rescale = TRUE;
}

void
Mle2dSpriteRole::setVisible(int show)
{
    visible = (MlBoolean) show;
}

int
Mle2dSpriteRole::getPageWidth(void)
{
    return pageWidth;
}

void
Mle2dSpriteRole::setPageWidth(int width)
{
    pageWidth = width;
}

int
Mle2dSpriteRole::getPageHeight(void)
{
    return pageHeight;
}

void
Mle2dSpriteRole::setPageHeight(int height)
{
    pageHeight = height;
}

void
Mle2dSpriteRole::setCurrentPage(int number)
{
    currentPage = number;
}

int
Mle2dSpriteRole::getPageCount(void)
{
    return pageCount;
}

void
Mle2dSpriteRole::setPageCount(int count)
{
    pageCount = count;
}

MlBoolean
Mle2dSpriteRole::load(MediaRef sprite)
{
    filemref *fileMediaRef;
    char* spritename = NULL;

    // Load sprite name from media reference
    fileMediaRef = (filemref *) mlLoadMediaRef(sprite, NULL);
    spritename = (char *) fileMediaRef->read();
    if (spritename == NULL)
        return FALSE;

/************ Begin SGI Implementation ************************/
#if defined(MLE_REHEARSAL) || defined(__sgi)
    ilFileImg *spriteImage = NULL;
    ilABGRImg *img = NULL;
    int chanCount, imgWidth, imgHeight;

    // Allocate a string for each sprite image file name
    char *filename = (char *) mlMalloc(strlen(spritename) + 8);

    // Load sprite pages
    for (int page = 0 ; page < pageCount ; page++)
	{
		// Generate name for next sprite file
		sprintf(filename, "%s%d", spritename, page+1);
#if defined(_MLE_SPRITE_DEBUG_)
		printf("sprite name: %s   image #%d's filename: %s\n",
			spritename, page, filename);
#endif // _MLE_SPRITE_DEBUG_

		// Open next sprite file
		spriteImage = ilOpenImgFile(filename, "r");
		if (!spriteImage)
			return FALSE;

		// Convert to raw pixels for real-time drawing
		img = new ilABGRImg(spriteImage);
		if (!img)
			return FALSE;
		img->setCoordSpace(ilLowerLeftOrigin);
		imgWidth = img->getWidth();
		if (imgWidth != pageWidth)
			return FALSE;
		imgHeight = img->getHeight();
		if (imgHeight != pageHeight)
			return FALSE;
		chanCount = img->getNumChans();
		pageSize = imgWidth * imgHeight * chanCount;

    
		// Allocate sprite page buffers if necessary
		if (spriteData == NULL)
		{
		    int index;
            spriteData = (unsigned char **) mlMalloc(MACHINE_POINTER_SIZE * pageCount);
            if (spriteData == NULL)
				return FALSE;
			memset((void *)spriteData, 0, (MACHINE_POINTER_SIZE * pageCount));
			for (index = 0 ; index < pageCount ; index++)
			{
				spriteData[index] = (unsigned char *) mlMalloc(pageSize);
				if (spriteData[index] == NULL)
					return FALSE;
			}
		}

		// Load next sprite page image media
		img->getTile(0, 0, pageWidth, pageHeight, spriteData[page]);
    
		// Clean up
		delete img;
		delete spriteImage;
	}

    // Update sprite's bounding box.
    MlScalar min[2], max[2];
    min[0] = position[0];
    min[1] = position[1];
    max[0] = position[0] + mlLongToScalar(pageWidth)  - ML_SCALAR_ONE;
    max[1] = position[1] + mlLongToScalar(pageHeight) - ML_SCALAR_ONE;
    setBounds(min, max);

    return TRUE;
#endif // MLE_REHEARSAL or __sgi
/************ End SGI Implementation ************************/


/************ Begin Win32 Implementation ********************/
#if defined(WIN32)
    FILE *fp;
    BOOL loadedDibImage = FALSE;
    int index;

    MlePaletteType *dibPalette;
  
    // Allocate and initialize a dib object per sprite page
    dib = (MleDIB **) mlMalloc(MACHINE_POINTER_SIZE * pageCount);
    if (dib == NULL)
		return FALSE;
    memset((void *) dib, 0, (MACHINE_POINTER_SIZE * pageCount));
    for (index = 0 ; index < pageCount ; index++)
	{
        dib[index] = (MleDIB *) new MleDIB;
		if (dib[index] == NULL)
			return FALSE;
	}

	// Allocate a string for each sprite image file name
	char *filename = (char *) mlMalloc(strlen(spritename) + 8);

	// Load dib files (each file is a sprite page)
	for (int page = 0 ; page < pageCount ; page++)
	{
		// Generate next sprite image filename
		sprintf(filename, "%s%d.dib", spritename, page+1);

		fp = mlFOpen(filename,"rb");
		loadedDibImage = dib[page]->load(fp);
		mlFClose(fp);
		if (loadedDibImage == FALSE)
		    break;

		// Map the palette here to that of the set's palette.
		dibPalette = ((Mle2dSet *)set)->getPalette();
		dib[page]->mapColorsToPalette(dibPalette->getPaletteHandle());
	}

    return loadedDibImage;
  
#endif // WIN32
/************ End Win32 Implementation **********************/
}


void
Mle2dSpriteRole::draw(void *)
{
#if defined(MLE_REHEARSAL) || defined(__sgi)
    int windowWidth, windowHeight;

    if(visible && (spriteData != NULL))
	{
        // Get window width and height
#if defined(MLE_REHEARSAL)
        // xxx this should now work in __sgi stage
        MleSGIBrStage::theStage->getSize(&windowWidth, &windowHeight);
#endif // MLE_REHEASAL
        windowWidth = 640;
        windowHeight = 480;

       // Save graphics context
        glPushAttrib(GL_COLOR_BUFFER_BIT |
		   GL_CURRENT_BIT |
		   GL_DEPTH_BUFFER_BIT |
		   GL_ENABLE_BIT |
		   GL_TEXTURE_BIT |
		   GL_VIEWPORT_BIT |
		   GL_PIXEL_MODE_BIT |
		   GL_TRANSFORM_BIT);

        // Set up graphics context for fast 2D draw
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_TEXTURE_1D);
        glDisable(GL_TEXTURE_2D);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, windowWidth - 1.0, 0.0, windowHeight - 1.0, 0.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // Draw sprite's current page
        if ( (transparencyCheck && transparent) || !transparencyCheck )
		{
	        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	        glEnable(GL_BLEND);
		}
        MlScalar x ,y;
        position.getValue(x, y);

        glRasterPos3i(mlScalarToLong(x), mlScalarToLong(y), 0);

        // Rescale if necessary
        if (rescale == TRUE)
	        glPixelZoom((GLfloat) xscale, (GLfloat) yscale);

        glDrawPixels(pageWidth, pageHeight, GL_ABGR_EXT, GL_UNSIGNED_BYTE,
		   (unsigned char *) spriteData[currentPage]);
      
        // Restore draw context
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glPopAttrib();
    }
#endif // MLE_REHEARSAL or __sgi


#if defined(WIN32)
    // Declare local variables.
    MlScalar xPos, yPos, min[2], max[2];
    MleDIB *destDib;
    int x,y;
    int srcWidth,srcHeight,destWidth,destHeight;

    if (visible)
	{
        position.getValue(xPos,yPos);
        x = mlScalarToLong(xPos);
        y = mlScalarToLong(yPos);

        destDib = ((Mle2dSet *)set)->getDib();
        srcWidth = dib[currentPage]->getWidth();
        srcHeight = dib[currentPage]->getHeight();
        destWidth = destDib->getWidth();
        destHeight = destDib->getHeight();

	    // For Magic Lantern 2d actors, the origin is lower-left
        // corner of window (ahem!)
        y -= srcHeight;

        // Check for trivial clipping.
        if ((x >= destWidth) || (y >= destHeight))
            return;

        if ((x + srcWidth) >= destWidth)
            srcWidth = destWidth - x;
        if ((y + srcHeight) >= destHeight)
            srcHeight = destHeight - y;

        // Copy pixel data into rendering buffer
        dib[currentPage]->copyBits(destDib,x,y,srcWidth,srcHeight,0,0);

        // Set extent of role for dirty rectangle calculation
        min[0] = mlLongToScalar(x);
        min[1] = mlLongToScalar(y);
        max[0] = mlLongToScalar(x + srcWidth - 1);
        max[1] = mlLongToScalar(y + srcHeight - 1);
        setBounds(min, max);
    }
#endif // WIN32

}

#if defined(MLE_REHEARSAL) || defined(__sgi)
extern "C" {

/*
 * The following gl stub functions are needed due to the fact that
 * we are using IL version 2.4 which is based on the Iris gl library.
 * This will prevent IL activate the Iris gl and keep our current
 * executable based on OpenGL.
 */
void
foreground(void)
{
}


long
gversion(char[12])
{
    return 0;
}

}
#endif // MLE_REHEARSAL or __sgi
