/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dmvd.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Movie Role.
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

#ifndef __MLE_2DMVD_H_
#define __MLE_2DMVD_H_

// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/2drole.h"
#include "mle/smpsound.h"

#if defined(MLE_REHEARSAL) || defined(__sgi)
#include <sys/time.h>
#include <movie.h>

#elif defined(WIN32)

#include "mle/dib.h"


class MleAviDIB : public MleDIB
{
  public:

    void setDIB ( BITMAPINFO * info, BYTE * data, BOOL own )
	{ bmInfo = info; bmData = data; ownData = own; }
};
#include <vfw.h> // Video for Windows
#endif /* MLE_REHEARSAL or __sgi */


class MleActor;


class Mle2dMvRole : public Mle2dRole 
{

    MLE_ROLE_HEADER(Mle2dMvRole);

  public:

	Mle2dMvRole(MleActor *actor);

    virtual		~Mle2dMvRole(void);

    // Update the position where the movie is drawn.  The position is
    // in screen coordinate and lower left is the movie origin.
    void		screenLocation(MlVector2 &pos);

    // This will read in a movie.  It can also be used to read in a different
    // movie during runtime.
    void		load(MlMediaRef mv);

    // The movie can be toggle on (with TRUE) or off (with FALSE).
    void		display(int state);

    // The render() function of the 2D Set calls this function to update
    // this role every cycle.  This function is not scheduled with the
    // scheduler.
    virtual void	draw(void *data);

#if defined(MLE_REHEARSAL) || defined(__sgi)

    // Get the appropriate frame of the movie media ready for rendering
    // by calling nextFrame().  This function is scheduled with the
    // scheduler in the PHASE_DELEGATE phase.
    static void		update(void *clientData);
#endif /* MLE_REHEARSAL or __sgi */

    // Retreive the appropriate movie media at the time.
    void		nextFrame(void);

  protected:

    // Use for freeing all the current resources.
    void		close();

	/* Movie origin (lower left) in screen coordinate. */
    MlVector2  screenPosition;
	/* TRUE for on, FALSE for off */
    int         displayState;
    char		*fileName;
    void		*imageBuffer, *audioBuffer;

  private:

    MleSampleSound	*sound;

#if defined(MLE_REHEARSAL) || defined(__sgi)

    MlBoolean	initMvFile(void);
    void		drawMvFrame(void);

    MVid		movieId, imageTrack, audioTrack;
    double		imageRate;
    int			imageWidth, imageHeight;
    int			imageFrameSize;
    MVframe		imageLength;
    int			imageFrameTime;

    int			audioWidth;
    double		audioRate;
    int			audioChannels;
    DMaudioformat	audioFormat;
    MVframe		audioLength;
    int			audioFrameSize;
    int			audioBuffSize;

    MlBoolean		firstTime, firstFrame;
    int			currentFrame, prevFrame;
    long		currentDeltaTime;
    struct timeval	startTime, currentTime;

#elif defined(WIN32)

    MlBoolean		initAviFile(void);
    void		drawAviFrame(void);

    MleAviDIB		dib;		// DIB object

    AVIFILEINFO		fileInfo;
    PAVIFILE		filePtr;
    AVISTREAMINFO	videoInfo;
    AVISTREAMINFO	audioInfo;
    PAVISTREAM		videoStream;
    PAVISTREAM		audioStream;
    PGETFRAME		getFramePtr;
    LONG		streamPos;
    LPVOID		dataPtr;
    BITMAPINFO *	bitmapInfo;
    BITMAPINFOHEADER *	h;
    BYTE *		bitmap;
    UINT		timerEvent;
    UINT		resolution;
    UINT		period;
    UINT		frameRate;

    DWORD		audioTotalSamples;
    DWORD		audioBytesPerSample;
    DWORD		audioSamplesPerSecond;
    DWORD		audioBufferSize;
#endif /* MLE_REHEARSAL or __sgi */
};


#endif /* __MLE_2DMVD_H_ */
