/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dmvd.h
 * @ingroup MleParts
 *
 * This file defines the class for a 2D Movie Role.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2025 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
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

#if defined(MLE_REHEARSAL)
#include <sys/time.h>
#include <movie.h>

#elif defined(_WINDOWS)

#include "mle/dib.h"


class MleAviDIB : public MleDIB
{
  public:

    void setDIB ( BITMAPINFO * info, BYTE * data, BOOL own )
	{ bmInfo = info; bmData = data; ownData = own; }
};
#include <vfw.h> // Video for Windows
#endif /* MLE_REHEARSAL */


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

#if defined(MLE_REHEARSAL))

    // Get the appropriate frame of the movie media ready for rendering
    // by calling nextFrame().  This function is scheduled with the
    // scheduler in the PHASE_DELEGATE phase.
    static void		update(void *clientData);
#endif /* MLE_REHEARSAL */

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

#if defined(MLE_REHEARSAL)

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

#elif defined(_WINDOWS)

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
#endif /* MLE_REHEARSAL */
};


#endif /* __MLE_2DMVD_H_ */
