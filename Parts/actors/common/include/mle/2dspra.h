/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dspra.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Sprite Actor.
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

#ifndef __MLE_2DSPRA_H_
#define __MLE_2DSPRA_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "math/vector.h"
#include "mle/MleActor.h"


/**
 *
 * @brief A 2D screen-blitting sprite actor.
 *
 * This is a 2D screen-blitting sprite actor that allows you to display
 * a scalable, rectangular sprite at a given position in the
 * screen using screen coordinates. The sprite can be sequenced
 * through one or more pages. A sprite consists of one or more
 * pages of the same size.
 * 
 * On the SGI platform, a sprite page is an rgba file. In the PC
 * platform, a sprite page is a dib file. Conversion between rgb
 * and dib files is provided through the rgbtodib converter command.
 * 
 * Sprites can have a transparent color. On the SGI, the transparent
 * color is supplied by the bitmap in the fourth (i.e., alpha) channel
 * of the image. On the PC, the transparent color is specified via
 * an option to the rgbtodib converter.
 * 
 * Sprite actors are typically added to the 2D Set implemented by the
 * Mle2dSet class. This set supports back-to-front relationships among
 * sprites (and other 2D actors, like Mle2dImgActor) within the set.
 *  
 * CURRENT LIMITATIONS
 * 
 * Sprite scaling is currently supported only on the SGI platform.
 * 
 * @see rgbtodib, Mle2dSet, and Mle2dImgActor.
 */

class Mle2dSpriteActor : public MleActor 
{

    MLE_ACTOR_HEADER(Mle2dSpriteActor);

  public:

    // Constructor
    Mle2dSpriteActor(void);

    // Destructor
    virtual ~Mle2dSpriteActor(void);

    // Initializes the sprite actor
    // This is called after the actor and
    // delegate have been created and their
    // state has been initialized from disk.
    virtual void        init(void);

#if defined(MLE_REHEARSAL)
    // Called when a property of the actor has been
    // changed by the FireWalker actor editor or by
    // some other authoring tool.
    virtual void	resolveEdit(const char *property);
#endif  /* MLE_REHEARSAL */

    // Set the sprite's position on the screen using
    // screen pixel coordinates. The sprite position is
    // the lower-left corner of the sprite image page.
    virtual void        setPosition(MlVector2 &position);

    // Gets the size of a sprite page in memory
    // All sprite pages are of the same size.
    virtual int         getPageWidth(void);

    // Gets the height of a sprite page in memory. 
    virtual int         getPageHeight(void);

    // Gets the x-coordinate scale of the sprite.
    // The sprite can be scaled on the screen.
    // A scale of (1.0, 1.0) renders the sprite
    // using the original page width and height.
    // A number < 1.0 shrinks the sprite.
    virtual MlScalar    getXScale(void);

    // Gets the y-coordinate scale of the sprite.
    virtual MlScalar    getYScale(void);

#if defined(MLE_REHEARSAL) || defined(__sgi)
    // Sets the x and y-coordinate scale of the sprite.
    // The scale can be changed at runtime on SGI platforms.
    // Changing the sprite's scale will slow display performance.
    virtual void        setScale(MlScalar deltaw, MlScalar deltah);
#endif // MLE_REHEARSAL or __sgi

    // Gets the sprite's current page number (0, 1, ..., N).
    // The sprite consists of pages: each page
    // is an image. Each page is numbered from 0.
    // The highest page number corresponds to the
    // maximum number of images in the sprite minus one.
    virtual int         getCurrentPage(void);

    // Sets the sprite's current page number (0, 1, ..., N).
    // This number must not exceed the sprite's number of
    // pages minus one.
    virtual void        setCurrentPage(int number);

    // Gets the number of pages in the sprite.
    virtual int         getPageCount(void);

    // Loads the sprite media from disk. load() is called
    // by MleSpriteActor's init() function. It loads the
    // sprite data. The sprite's  pageCount, pageWidth and pageHeight 
    // must be set/ with the expected sprite information
    // before load() is called. Returns TRUE if load was successful.
    virtual MlBoolean   load(MlMediaRef spriteData);

    // Gets the sprite's visibility.
    // Returns whether its invisible (FALSE) or visible (TRUE).
    virtual int         getVisible(void);

    // Make sprite invisible (FALSE) or visible (TRUE)
    // The sprite's visibility can be turned on and off
    virtual void        setVisible(int visible);

    // Pushes full sprite actor state to delegate.
    // This is an option for using the sprite actor (e.g., at
    // init and update time). Often, it will be enough and faster to
    // call the appropriate setX function (e.g., setScale)
    // at the right phase instead of pushing all of the
    // actor's state to the delegate.
    virtual void        push(void);

    // Update sprite during PHASE_ACTOR.
    // This is a simple sample script for working with the
    // sprite, but not what users might need for
    // a specific title.
    static  void        update(Mle2dSpriteActor* sprite);

  private:

    void                checkFrameInterval(void);

  public:

    // <b>spriteData</b> is the sprite image sequence media reference.
    MlMediaRef		    spriteData;      

    // <b>position</b> is the sprite position on the window. The
    // position is the position of the lower-left corner of the sprite
    // page in the window's coordinate system.
    MlVector2           position;


    // <b>currentPage</b> is the sprite's current page number.
    int                 currentPage;         

    // <b>visible</b> indicates whether the sprite
    // is visible or invisible on the screen.
    int                 visible;      

    // <b>xscale</b> is the x scale of the sprite.
    MlScalar            xscale;      

    // <b>yscale</b> is the y scale of the sprite.
    MlScalar            yscale;    

    // <b>pageCount</b> is the number of pages in the sprite.
    // This must be set in the workprint or in the actor's code
    // before the sprite's load() function is called.
    int                 pageCount; 

    // <b>pageWidth</b> is the current sprite page width.
    // The initial page width value must be set in the workprint
    // or in the actor's code before the sprite image data
    // is loaded with load().
    int                 pageWidth;

    // <b>pageHeight</b> is the current sprite page height.
    // The initial page height value must be set in the workprint
    // or in the actor's code before the sprite image data
    // is loaded with load().
    int                 pageHeight;

    // <b>frameInterval</b> is the intervals between
    // rendering cycles (i.e., frame) in which the
    // sprite image should be re-rendered.
    //   0: Don't update the sprite at all
    //   n: Render the sprite every n frames
    //      The higher n, the slower the update.
    int                 frameInterval;
};

#endif	/* __MLE_2DSPRA_H_ */



