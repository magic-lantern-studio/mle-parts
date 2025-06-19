/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file 2dmvd.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 2D Movie Role.
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


#if defined(MLE_REHEARSAL)
#include <string.h>
#include <GL/gl.h>
#endif /* MLE_REHEARSAL */

#include "mle/2dmva.h"
#include "mle/2dmvd.h"
#include "mle/MleLoad.h"
#include "mle/filemref.h"

#if defined(MLE_REHEARSAL)
#include "mle/titleenv.h"
#include "mle/sgibrstage.h"

#elif defined(_WINDOWS)

#include "mle/pcstage.h"
#include "mle/br2dfrm.h"
#endif /* MLE_REHEARSAL */


#if defined(_WINDOWS)
// Forward declaration.
static int printf(const char *format, ... );
static void CALLBACK cb(UINT, UINT, DWORD dwUser, DWORD, DWORD);
#endif /* _WINDOWS */


MLE_ROLE_SOURCE(Mle2dMvRole, Mle2dRole);


Mle2dMvRole::Mle2dMvRole(MleActor *actor)
 : Mle2dRole(actor)
{
    screenPosition.setValue(ML_SCALAR(0), ML_SCALAR(0));
    displayState = FALSE;
    fileName = NULL;
    sound = NULL;

#if defined(MLE_REHEARSAL)
    imageBuffer = NULL;
    audioBuffer = NULL;
    movieId = 0;

    firstTime = TRUE;
    firstFrame = TRUE;
    currentFrame = 0;
    prevFrame = -1;
    currentDeltaTime = 0;

    theTitle->scheduler->insertFunc(PHASE_ROLE, update, this, this);

#elif defined(_WINDOWS)

    filePtr     = NULL;
    videoStream = NULL;
    audioStream = NULL;
    getFramePtr = NULL;
    timerEvent  = NULL;
    resolution  = 0;
#endif /* MLE_REHEARSAL */
}


Mle2dMvRole::~Mle2dMvRole(void)
{
    if (fileName)
        delete [] fileName;

    close();
}


void
Mle2dMvRole::screenLocation(MlVector2 &pos)
{
    screenPosition.setValue(pos.getValue());

#if defined(MLE_REHEARSAL)

    // Update its bounding box.
    MlScalar width, height, min[2], max[2];

    if(movieId)
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
#endif /* MLE_REHEARSAL */
}


void
Mle2dMvRole::load(MlMediaRef mv)
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

#if defined(MLE_REHEARSAL)
            firstTime = TRUE;
#endif /* MLE_REHEARSAL */

            delete fileMediaRef;
            return;
        }
        else
            delete [] fileName;
    }
    fileName = new char[strlen(newFileName) + 1];
    strcpy(fileName, newFileName);

#if defined(MLE_REHEARSAL)
    if (initMvFile())
    {
        firstTime = TRUE;

        // Get it ready to display the first frame.
        Mle2dMvRole::update(this);
    }

#elif defined(_WINDOWS)
    initAviFile();
#endif /* MLE_REHEARSAL */

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
#if defined(MLE_REHEARSAL)
    drawMvFrame();

#elif defined(_WINDOWS)

    drawAviFrame();
#endif /* MLE_REHEARSAL */
}


#if defined(MLE_REHEARSAL)
void
Mle2dMvRole::update(void *clientData)
{
    Mle2dMvRole *role = (Mle2dMvRole *) clientData;

    ML_ASSERT(role);

    // Advances to the next frame, and skip frame if necessary.
    role->nextFrame();
}
#endif /* MLE_REHEARSAL */


void
Mle2dMvRole::nextFrame(void)
{
#if defined(MLE_REHEARSAL)
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
            if (firstFrame)
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
                if (currentFrame >= imageLength)
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

#elif defined(_WINDOWS)

    dataPtr = AVIStreamGetFrame ( getFramePtr, streamPos );
    bitmapInfo = (BITMAPINFO *) dataPtr;
    h = (BITMAPINFOHEADER *) &(bitmapInfo->bmiHeader);
    bitmap = ((BYTE *)bitmapInfo) + h->biSize
                        + ( h->biClrUsed * sizeof(RGBQUAD) );
    if ( streamPos == 0 )
    {
        if ( sound )
            sound->play ();
    }
    streamPos = AVIStreamNextSample ( videoStream, streamPos );
    if ( streamPos < 0 )
        streamPos = 0;
#endif /* MLE_REHEARSAL */
}


void
Mle2dMvRole::close()
{
#if defined(MLE_REHEARSAL)

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

#elif defined(_WINDOWS)

    if ( timerEvent )
    {
        timeKillEvent ( timerEvent );
        timerEvent = NULL;
    }
    if ( resolution )
    {
        timeEndPeriod ( resolution );
        resolution = 0;
    }
    if ( sound )
    {
        delete sound;
        sound = NULL;
    }
    if ( getFramePtr )
    {
        AVIStreamGetFrameClose ( getFramePtr );
        getFramePtr = NULL;
    }
    if ( videoStream )
    {
        AVIStreamRelease ( videoStream );
        videoStream = NULL;
    }
    if ( audioStream )
    {
        AVIStreamRelease ( audioStream );
        audioStream = NULL;
    }
    if ( filePtr )
    {
        AVIFileRelease ( filePtr );
        filePtr = NULL;
    }
    dib.setDIB ( NULL, NULL, FALSE );

#endif /* MLE_REHEARSAL */
}


#if defined(MLE_REHEARSAL)
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
    if (movieId)
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
    if(mvOpenFile(fileName, O_RDONLY, &movieId) != DM_SUCCESS)
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

    if (imageLength == 0)
    {
        fprintf(stderr, "Mle2dMvRole: track length is zero.\n"); 
        mvClose(movieId);
        movieId = 0;
        return FALSE;
    }

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

    // Get the number of bytes per frame (one sample per channel) and
    // allocate a buffer large enough to contain all the audio samples.
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
    if (! movieId)
        return;

    if (displayState)
    {
        screenPosition.getValue(x, y);

#if defined(MLE_REHEARSAL)
        MleSGIBrStage::theStage->getSize(&stageWidth, &stageHeight);
#else
        stageWidth = 640;
        stageHeight = 480;
#endif /* MLE_REHEARSAL */

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
#endif /* MLE_REHEARSAL */


#if defined(_WINDOWS)
MlBoolean
Mle2dMvRole::initAviFile(void)
{
    HRESULT hr;

    // Open the given AVI file for reading.
    hr = AVIFileOpen ( &filePtr, fileName, OF_READ, NULL );
    printf ( "hr = %x filePtr = %x\n", hr, filePtr );
    if ( hr != 0 )
    {
        return FALSE;
    }

    // Get the AVI file information.
    AVIFileInfo ( filePtr, &fileInfo, sizeof(fileInfo) );
    int width = fileInfo.dwWidth;
    int height = fileInfo.dwHeight;
    printf ( "width = %d height = %d\n", width, height );

    // Get the video stream.
    hr = AVIFileGetStream ( filePtr, &videoStream, streamtypeVIDEO, 0 );
    printf ( "hr = %x videoStream = %x\n", hr, videoStream );
    if ( hr != 0 )
    {
        close ();
        return FALSE;
    }

    // Get the video stream information.
    AVIStreamInfo ( videoStream, &videoInfo, sizeof(videoInfo) );
    frameRate = videoInfo.dwRate / videoInfo.dwScale;
    printf ( "frameRate = %d\n", frameRate );

    period = 1000 / frameRate; // milliseconds
    resolution = period;
    timeBeginPeriod ( resolution );

    // Get the frame pointer for the video stream.
    getFramePtr = AVIStreamGetFrameOpen ( videoStream, NULL );
    printf ( "getFramePtr = %x\n", getFramePtr );
    if ( getFramePtr == NULL )
    {
        close ();
        return FALSE;
    }

    // Get the first video frame.
    streamPos = 0;
    dataPtr = AVIStreamGetFrame ( getFramePtr, streamPos );
    printf ( "dataPtr = %x\n", dataPtr );
    if ( dataPtr == NULL )
    {
        close ();
        return FALSE;
    }

    // Update the video stream position.
    streamPos = AVIStreamNextSample ( videoStream, streamPos );

    // Set up pointers to the returned DIB.
    bitmapInfo = (BITMAPINFO *) dataPtr;
    h = (BITMAPINFOHEADER *) &(bitmapInfo->bmiHeader);
    bitmap = ((BYTE *)bitmapInfo) + h->biSize
            + ( h->biClrUsed * sizeof(RGBQUAD) );

    // Make sure the DIB has a color table.
    printf ( "biClrUsed %d\n", h->biClrUsed );
    if ( h->biClrUsed != 256 )
    {
        close ();
        return FALSE;
    }

    // Set up a palette for the DIB.
    PALETTEENTRY palEntries[256];
    for ( int i = 0; i < 256; i++ )
    {
        palEntries[i].peRed = bitmapInfo->bmiColors[i].rgbRed;
        palEntries[i].peGreen = bitmapInfo->bmiColors[i].rgbGreen;
        palEntries[i].peBlue = bitmapInfo->bmiColors[i].rgbBlue;
        palEntries[i].peFlags = PC_NOCOLLAPSE;
    }

    // Map the palette here to that of the set's palette.
    MlePaletteType *dibPalette;
    dibPalette = ((Mle2dSet *)set)->getPalette();
    dibPalette->setEntries ( 10, 236, &palEntries[10] );

    // Set the system palette.
    MlePCBRenderStage * pcStage;
    pcStage = (MlePCBRenderStage *)MleStage::theStage;
    pcStage->setPalette ( dibPalette );

    // Get the audio stream.
    hr = AVIFileGetStream ( filePtr, &audioStream, streamtypeAUDIO, 0 );
    printf ( "hr = %x audioStream = %x\n", hr, audioStream );
    if ( hr != 0 )
    {
        return TRUE;        // no audio is okay
    }

    // Get the audio stream information.
    AVIStreamInfo ( audioStream, &audioInfo, sizeof(audioInfo) );

    audioTotalSamples = audioInfo.dwLength;
    audioBytesPerSample = audioInfo.dwSampleSize;
    audioSamplesPerSecond = audioInfo.dwRate / audioInfo.dwScale;
    audioBufferSize = audioTotalSamples * audioBytesPerSample;

    printf ( "TotalSamples %d BytesPerSample %d SamplesPerSecond %d\n",
    audioTotalSamples, audioBytesPerSample, audioSamplesPerSecond );

    // Create a sample sound object.
    sound = new MleSampleSound;
    sound->open ();

    // Read the entire audio stream into a Direct Sound buffer.
    // XXX Need to handle streams that are too large for memory.
    hr = sound->setup ( audioTotalSamples, audioBytesPerSample,
                audioSamplesPerSecond );
    printf ( "hr = %x BufferSize = %d\n", hr, audioBufferSize );
    if ( hr != 0 )
    {
        delete sound;
        sound = NULL;
        return TRUE;
    }

    void * ptr1, * ptr2;
    int bytes1, bytes2;

    // Lock the audio buffer (assuming Direct Sound).
    hr = sound->lock ( 0, audioBufferSize, &ptr1, &bytes1, &ptr2, &bytes2, 0 );
    printf ( "hr = %x ptr1 = %x bytes1 = %d\n", hr, ptr1, bytes1 );
    if ( hr != 0 )
    {
        delete sound;
        sound = NULL;
        return TRUE;
    }

    // Read the audio stream.
    LONG bytesRead = 0, samplesRead = 0;
    hr = AVIStreamRead ( audioStream, audioInfo.dwStart,
            audioTotalSamples, ptr1, audioBufferSize,
            &bytesRead, &samplesRead );

    printf ( "hr = %x bytesRead = %d samplesRead = %d\n",
                hr, bytesRead, samplesRead );
    if ( hr != 0 )
    {
        delete sound;
        sound = NULL;
    }

    // Unlock the audio buffer.
    sound->unlock ( ptr1, bytes1, ptr2, bytes2 );

    return TRUE;
}


void
Mle2dMvRole::drawAviFrame(void)
{
    MlScalar xPos, yPos, min[2], max[2];
    MleDIB *destDib;
    int xs, ys, xd, yd;
    int srcWidth,srcHeight,destWidth,destHeight;

    if ( displayState && videoStream )
    {
        // Start a periodic timer at the video frame rate.
        if ( ! timerEvent )
        {
            timerEvent = timeSetEvent ( period, resolution, cb, (DWORD)this,
                            TIME_PERIODIC );
            if ( sound )
            sound->play ();
        }

        dib.setDIB ( bitmapInfo, bitmap, FALSE );

        screenPosition.getValue(xPos,yPos);
        xd = mlScalarToLong(xPos);
        yd = mlScalarToLong(yPos);

        destDib = ((Mle2dSet *)set)->getDib();
        srcWidth = dib.getWidth();
        srcHeight = dib.getHeight();
        destWidth = destDib->getWidth();
        destHeight = destDib->getHeight();

        // Check for trivial clipping
        if ((xd >= destWidth) || (yd >= destHeight))
            return;
        if ((destWidth <= 0) || (destHeight <= 0))
            return;
        if ((srcWidth <= 0) || (srcHeight <= 0))
            return;

        xs = ys = 0;

        if ((xd + srcWidth) > destWidth)
            srcWidth = destWidth - xd;

        if ((yd + srcHeight) > destHeight)
        {
            int origHeight = srcHeight;
            srcHeight = destHeight - yd;
            ys = origHeight - srcHeight;
        }

        // On Windows, the origin is the upper, left-hand corner;
        // therefore, we have to compensate for the vertical offset
        yd = destHeight - yd - srcHeight;

        // Copy pixel data into rendering buffer
        dib.copyBits(destDib,xd,yd,srcWidth,srcHeight,xs,ys);

        // Set extent of role for dirty rectangle calculation
        min[0] = mlLongToScalar(xd);
        min[1] = mlLongToScalar(yd);
        max[0] = mlLongToScalar(xd + srcWidth - 1);
        max[1] = mlLongToScalar(yd + srcHeight - 1);
        setBounds(min, max);
    }
}


static void CALLBACK cb ( UINT, UINT, DWORD dwUser, DWORD, DWORD )
{
    Mle2dMvRole * role = (Mle2dMvRole *) dwUser;

    role->nextFrame ();
}


static int printf ( const char * format, ... )
{
    char pbuf[100];
    va_list ap;
    va_start ( ap, format );
    _vsnprintf ( pbuf, sizeof(pbuf)-1, format, ap );
    va_end ( ap );
    OutputDebugString ( pbuf );
    return 0;
}
#endif /* _WINDOWS */
