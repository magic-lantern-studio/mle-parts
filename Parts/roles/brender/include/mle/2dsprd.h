/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dsprd.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Sprite Role.
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

#ifndef __MLE_2DSPRD_H_
#define __MLE_2DSPRD_H_


/********************** Begin common includes ******************************/
#include "mle/mlTypes.h"
#include "math/vector.h"
#include "mle/2drole.h"
/********************** End common includes ********************************/


/********************** Begin SGI includes *********************************/
#if defined(MLE_REHEARSAL) || defined(__sgi)
#include <il/ilABGRImg.h>
#include <il/ilFileImg.h>
#endif // MLE_REHEARSAL or __sgi
/********************** End SGI includes ***********************************/


/********************** Begin Win32 includes *******************************/
#if defined(WIN32)
#include "mle/dib.h"
#endif // WIN32
/********************** End Win32 includes *********************************/

// Declare external classes.
class MleActor;


/**
 * @brief A 2D Sprite Role.
 *
 * This role implements the platform-dependent code for Mle2dSpriteActor
 * for the SGI (Irix 5.3/6.2) and Windows95 (Win32 and GDK) platforms.
 * This file is ifdef'ed for each of these platforms. (See MleSpriteActor
 * for more information about the functionality of Mle2dSpriteRole.
 * 
 * @see Mle2dSpriteActor.
 */
class Mle2dSpriteRole : public Mle2dRole 
{

    MLE_ROLE_HEADER(Mle2dSpriteRole);

  public:

	Mle2dSpriteRole(MleActor *actor);

    virtual		~Mle2dSpriteRole(void);

    // Loads sprite data from disk. Sprite media is a set
    // of one or more image files. On the SGI platform, the
    // images should be rgba files, while on the PC the images should
    // be DIB files. There should be on image file per sprite page.
    // All images should be of the same size.
    MlBoolean 		load(MediaRef img);

    // Draws sprite. The 2d Set's render() function calls draw()
    // each scheduler cycle during the PHASE_SET phase.
    virtual void	draw(void *data);

    // Sets sprite screen position. The sprite's position is
    // the lower-left corner of the sprite image rectangle.
    // The 2d coordinate space's origin is the lower-left corner
    // of the window.
    virtual void        setPosition(MleVector2 &pos);

    // Set sprite page scale. The default sprite scale is 1.0.
    // Increasing the scale increases the area of the sprite page.
    // Decreasing the scale decreases the area of the sprite page.
    virtual void        setScale(MleScalar xs, MleScalar ys);

    // Shows or hides the sprite on the screen.
    virtual void        setVisible(int show);

    // Gets width of the current sprite page.
    // Currently, all pages must be of the same width.
    virtual int         getPageWidth(void);

    // Gets width of the current sprite page.
    // Currently, all pages must be of the same width.
    virtual void        setPageWidth(int width);

    // Gets the height of the current sprite page.
    // Currently, all pages must be of the same height.
    virtual int         getPageHeight(void);

    // Gets the height of the current sprite page.
    // Currently, all pages must be of the same height.
    virtual void        setPageHeight(int height);

    // Sets the sprite's current page number. This sets the page
    // that will be rendered by the draw() function. Pages are
    // counted using 0-origin. Therefore, this number must not be 
    // greater than the number of sprite pages minus one.
    virtual void        setCurrentPage(int number);

    // Returns the number of pages in sprite image.
    virtual int         getPageCount(void);

    // Sets the number of pages in sprite image.
    virtual void        setPageCount(int count);

  protected:

    MleVector2          position;
    int                 currentPage;        // Current sprite page number
                                            //   (first page is page 0)
    int                 pageCount;          // Number of pages in sprite
    int                 pageWidth;          // sprite page width
    int                 pageHeight;         // sprite page height
    int                 pageSize;           // cached size of sprite page
                                            //   (in bytes)
    MlBoolean           visible;            // TRUE means sprite is shown on
                                            //   screen (unless clipped) --
                                            //   initially FALSE
    float               xscale;             // Sprite x scale (scale of
                                            //   currently displayed page)
    float               yscale;             // Sprite y scale (scale of
                                            //   currently displayed page)
    MlBoolean           rescale;            // Should sprite be scaled?
                                            //   (0 = no, 1 = yes)
    MlBoolean           transparencyCheck;  // If TRUE, then we should check
                                            // whether transparency is required.
    MlBoolean           transparent;        // Is it a transparent sprite?
                                            // Of interest only if transparencyCheck is TRUE.

  private:

#if defined(WIN32)
    MleDIB**             dib;                // DIB objects
#endif // WIN32
#if defined(MLE_REHEARSAL) || defined(__sgi)
    unsigned char**     spriteData;         // Array of pointers to images
                                            // One image per sprite page
#endif /* MLE_REHEARSAL or __sgi */
};


#endif  /* __MLE_2DSPRD_H_ */
