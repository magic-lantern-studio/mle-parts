/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file MleIv2dMovieRole.cxx
 * @ingroup MleParts
 *
 * This file implements the 2D Movie Role targeting the Inventor platform.
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

#include <string.h>
#include <GL/gl.h>

#include "mle/2dmva.h"
#include "mle/2dmvd.h"
#include "mle/MleLoad.h"
#include "mle/filemref.h"

#include "mle/titleenv.h"
#include "mle/sched.h"
#include "mle/sgiivstage.h"


MLE_ROLE_SOURCE(Mle2dMvRole, Mle2dRole);


Mle2dMvRole::Mle2dMvRole(MleActor *actor)
 : Mle2dRole(actor)
{
    screenPosition.setValue(ML_SCALAR(0), ML_SCALAR(0));
    displayState = FALSE;
    fileName = NULL;
    sound = NULL;

    imageBuffer = NULL;
    audioBuffer = NULL;
    movieId = 0;

    firstTime = TRUE;
    firstFrame = TRUE;
    currentFrame = 0;
    prevFrame = -1;
    currentDeltaTime = 0;

    theTitle->scheduler->insertFunc(PHASE_DELEGATE, update, this, this);
}


Mle2dMvRole::~Mle2dMvRole(void)
{
    if (fileName)
		delete [] fileName;

    close();
}


void
Mle2dMvRole::screenLocation(MleVector2 &pos)
{
    screenPosition.setValue(pos.getValue());

    // Update its bounding box.
    MlScalar width, height, min[2], max[2];

    if (movieId)
	{
		min[0] = pos[0];
		min[1] = pos[1];
		max[0] = pos[0] + mlLongToScalar(imageWidth) - ML_SCALAR_ONE;
		max[1] = pos[1] + mlLongToScalar(imageHeight) - ML_SCALAR_ONE;
    }
    else
	{
		// This is to handle the case where media is not loaded in correctly.
		getBounds(min, max);
			width = max[0] - min[0] + ML_SCALAR_ONE;
			height = max[1] - min[1] + ML_SCALAR_ONE;
		min[0] = pos[0];
		min[1] = pos[1];
		max[0] = pos[0] + width - ML_SCALAR_ONE;
		max[1] = pos[1] + height - ML_SCALAR_ONE;
    }
    setBounds(min, max);
}


void
Mle2dMvRole::load(MediaRef mv)
{
    char *newFileName;
    MlScalar min[2], max[2];
    filemref *fileMediaRef;

    fileMediaRef = (filemref *) mlLoadMediaRef(mv, NULL);
    // Give it a default bounding box of 64 by 64 if it is a bogus
    // media ref.
    if (fileMediaRef)
	{
		newFileName = (char *) fileMediaRef->read();
		if (! newFileName)
		{
			screenPosition.getValue(min[0], min[1]);
			max[0] = min[0] + ML_SCALAR(63);
			max[1] = min[1] + ML_SCALAR(63);
			setBounds(min, max);
			delete fileMediaRef;
			return;
		}
    }
    else
	{
		screenPosition.getValue(min[0], min[1]);
		max[0] = min[0] + ML_SCALAR(63);
		max[1] = min[1] + ML_SCALAR(63);
		setBounds(min, max);
		return;
    }

    // Return if it is the same movie as the current one; otherwise,
    // update the fileName.
    if (fileName)
	{
		if (strcmp(fileName, newFileName) == 0)
		{

			firstTime = TRUE;

			delete fileMediaRef;
			return;
		}
		else
			delete [] fileName;
    }
    fileName = new char[strlen(newFileName) + 1];
    strcpy(fileName, newFileName);

    if (initMvFile())
	{
		firstTime = TRUE;
		// Get it ready to display the first frame.
		Mle2dMvRole::update(this);
    }

    // Delete the file media ref.
    // XXX might want to store in some media ref registry for latter use.
    delete fileMediaRef;
}


void
Mle2dMvRole::display(int state)
{
    displayState = state;
}


void
Mle2dMvRole::draw(void *)
{
    drawMvFrame();
}


void
Mle2dMvRole::update(void *clientData)
{
    Mle2dMvRole *role = (Mle2dMvRole *) clientData;

    ML_ASSERT(role);

    // Advances to the next frame, and skip frame if necessary.
    role->nextFrame();
}


void
Mle2dMvRole::nextFrame(void)
{
    long deltaTime;
    DMstatus status;

    // Don't update if media is not loaded in.
    if (! movieId)
		return;

    if (firstTime)
	{
		// The first time it comes up is a special case.  We don't want to
		// play the audio but just retreive the first frame of video and
		// put it in the image buffer ready for display.
		firstTime = FALSE;

		status = mvReadFrames(imageTrack, currentFrame, 1, imageFrameSize,
			imageBuffer);
		if (status != DM_SUCCESS)
			fprintf(stderr, "Mle2dMvRole: mvReadFrames error.\n");
    }
    else
	{
		if (displayState)
		{
			if(firstFrame)
			{
				// If it is the first frame, start playing the audio and
				// start the timer.  Note the first frame of video data is
				// in the imageBuffer already and the video is actually
				// played in the stage phase by the draw() routine.
				if (audioBuffer)
					sound->play();

				gettimeofday(&startTime);
				firstFrame = FALSE;
			}
			else
			{
				// Save the previous frame count and calculate the current
				// frame.  This will drop frames if needed.
				prevFrame = currentFrame;
				gettimeofday(&currentTime);
				if (startTime.tv_sec == currentTime.tv_sec)
					deltaTime = currentTime.tv_usec - startTime.tv_usec;
				else if (currentTime.tv_sec > startTime.tv_sec)
				{
					deltaTime = (currentTime.tv_sec - startTime.tv_sec) *
						1000000 + (currentTime.tv_usec -
						startTime.tv_usec);
				}
				startTime.tv_sec = currentTime.tv_sec;
				startTime.tv_usec = currentTime.tv_usec;

				currentFrame = currentFrame + (currentDeltaTime + deltaTime) /
						   imageFrameTime;
				currentDeltaTime = (currentDeltaTime + deltaTime) %
						   imageFrameTime;
				// Go back to the beginning if needed.
				if(currentFrame >= imageLength)
				{
					currentFrame = 0;
					firstFrame = TRUE;
				}

				// Retreive the appropriate frame of video data.  Don't need
				// to retreive a frame if it is still within the time for
				// the same frame.
				if (currentFrame != prevFrame)
				{
					status = mvReadFrames(imageTrack, currentFrame, 1,
							  imageFrameSize, imageBuffer);
					if (status != DM_SUCCESS)
						fprintf(stderr, "Mle2dMvRole: mvReadFrames error.\n");
				}
			}
		}
    }
}


void
Mle2dMvRole::close()
{
    // Remove the scheduled update() function.
    theTitle->scheduler->remove(this);

    if (sound)
		delete sound;
    if (imageBuffer)
		delete [] imageBuffer;
    if (audioBuffer)
		delete [] audioBuffer;
    if (movieId)
		mvClose(movieId);
}


MlBoolean
Mle2dMvRole::initMvFile(void)
{
    DMstatus status;
    DMparams *imageTrackParam, *audioTrackParam;
    MlScalar min[2], max[2];


    // Check for valid movie file.
    if (! mvIsMovieFile(fileName))
	{
		fprintf(stderr, "Mle2dMvRole: cannot open movie file %s.\n",
			fileName);
		return FALSE;
    }

    // Free the resources occupy by the current movie.
    if(movieId)
	{
		mvClose(movieId);
		movieId = 0;
    }
    if (imageBuffer)
	{
		delete [] imageBuffer;
		imageBuffer = NULL;
    }
    if (audioBuffer)
	{
		delete [] audioBuffer;
		audioBuffer = NULL;
    }
    if (sound)
	{
		delete sound;
		sound = NULL;
    }

    // Open new movie file.
    if (mvOpenFile(fileName, O_RDONLY, &movieId) != DM_SUCCESS)
	{
		fprintf(stderr, "Mle2dMvRole: cannot open movie file %s.\n", 
                fileName);
		return FALSE;
    }

    // Get image track, image width and height, frameSize in bytes, total
    // number of frames.
    if (mvFindTrackByMedium(movieId, DM_IMAGE, &imageTrack) != DM_SUCCESS)
	{
		fprintf(stderr, "Mle2dMvRole: cannot find image track.\n"); 
		mvClose(movieId);
		movieId = 0;
		return FALSE;
    }
    imageRate = mvGetImageRate(imageTrack);
    imageWidth = mvGetImageWidth(imageTrack);
    imageHeight = mvGetImageHeight(imageTrack);
    imageTrackParam = mvGetParams(imageTrack);
    imageFrameSize = dmImageFrameSize(imageTrackParam);
    imageLength = mvGetTrackLength(imageTrack);
    imageBuffer = new char[imageFrameSize];

    // Determine the duration per frame in usec/frame.
    imageFrameTime = (int) (1000000.0 / imageRate);

    // Update its bounding box.
    min[0] = screenPosition[0];
    min[1] = screenPosition[1];
    max[0] = screenPosition[0] + mlLongToScalar(imageWidth) - ML_SCALAR_ONE;
    max[1] = screenPosition[1] + mlLongToScalar(imageHeight) - ML_SCALAR_ONE;
    setBounds(min, max);

    // Get audio track, number of bits per sample, number samples per
    // second, number of channels, sample format, total number of frames.
    if (mvFindTrackByMedium(movieId, DM_AUDIO, &audioTrack) != DM_SUCCESS)
	{
		fprintf(stderr, "Mle2dMvRole: cannot find audio track.\n"); 
		return FALSE;
    }
    audioWidth = mvGetAudioWidth(audioTrack);
    audioRate = mvGetAudioRate(audioTrack);
    audioChannels = mvGetAudioChannels(audioTrack);
    audioFormat = mvGetAudioFormat(audioTrack);
    audioLength = mvGetTrackLength(audioTrack);

    //
    // Get the number of bytes per frame (one sample per channel) and
    // allocate a buffer large enough to contain all the audio samples.
    //
    audioTrackParam = mvGetParams(audioTrack);
    audioFrameSize = dmAudioFrameSize(audioTrackParam);
    audioBuffSize = audioFrameSize * audioLength;
    audioBuffer = new char[audioBuffSize];

    status = mvReadFrames(audioTrack, 0, audioLength, audioBuffSize,
			  audioBuffer);
    if (status == DM_SUCCESS)
	{
		sound = new MleSampleSound;

		// Open and set up the sound object.
		sound->open();
		sound->setup(audioFormat, audioWidth, audioChannels, audioLength,
				 audioRate );
		sound->setBuffer((char *) audioBuffer);

		return TRUE;
    }
    else
	{
		fprintf(stderr, "Mle2dMvRole: audio track read error.\n");

		// Encountered error, remove audio buffer.
		if (audioBuffer)
		{
			delete [] audioBuffer;
			audioBuffer = NULL;
		}

		return FALSE;
    }
}


void
Mle2dMvRole::drawMvFrame(void)
{
    int stageWidth, stageHeight;
    MlScalar x, y;

    // Don't draw if media is not loaded in.
    if(! movieId)
		return;

    if (displayState)
	{
        screenPosition.getValue(x, y);

        MleSGIIvStage::theStage->getSize(&stageWidth, &stageHeight);

        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT |
            GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT |
            GL_TEXTURE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);

        glDisable(GL_DEPTH_TEST);
		glDisable(GL_ALPHA_TEST);
        glDisable(GL_TEXTURE_1D);
        glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0.0, stageWidth - 1.0, 0.0, stageHeight - 1.0, 0.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glRasterPos3i(mlScalarToLong(x), mlScalarToLong(y), 0);
        glDrawPixels(imageWidth, imageHeight, GL_ABGR_EXT, GL_UNSIGNED_BYTE,
		     imageBuffer);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glPopAttrib();
    }
}
