/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file Snapper.h
 * @ingroup MlParts
 *
 * This file defines a class that constrains a manipulator to "snap"
 * during manipulation to a given alignment constraint.
 */

 // COPYRIGHT_BEGIN
 //
 // The MIT License (MIT)
 //
 // Copyright (c) 2017-2025 Wizzer Works
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

#ifndef  __SNAPPER_H_
#define  __SNAPPER_H_

// Include Inventor header files.
#include <Inventor/SbLinear.h>
#include <Inventor/SbPList.h>

// Declare external classes.
class Snapper;
class SnapperManipInfo;
class SoDragger;
class SoFullPath;
class SoGetBoundingBoxAction;
class SoGetMatrixAction;
class SoPath;
class SoTransformManip;
class Source;
class Target;

/**
 * @brief Snapper is a class that constrains a manipulator to "snap"
 * during manipulation to a given alignment constraint.
 *
 * There are three coordinate spaces involved in snapping: world
 * space, object space (the space of the object being manipulated),
 * and dragger space (the space in which the manipulator is
 * defined). The dragger's transformation converts from object space
 * to dragger space. The cumulative transformation of the path from
 * the world root to the manipulator (but not including the manip's
 * transform) converts from dragger space to world space.
 */
class Snapper
{
  public:

    Snapper();
    ~Snapper();

    // Sets the target and source instances to use.
    void	setTarget(Target *t)		{ target = t; }
    void	setSource(Source *s)		{ source = s; }

    // Enables or disables snapping, or returns the current state.
    void	enable(SbBool flag);
    SbBool	isEnabled() const;

    // Enables or disables scale snapping, or returns the current state.
    void	enableScaleSnapping(SbBool flag);
    SbBool	isScaleSnappingEnabled() const;

    // Adds or removes a manipulator to constrain. The manipulator
    // must be derived from SoTransformManip and must be the last node
    // in the given path (when cast to an SoFullPath).
    void	addManip(SoPath *pathToManip);
    void	removeManip(SoTransformManip *manip);

    // If this flag is set to TRUE (the default), moving one manipulator
    // in the list causes the rest to be moved as well, relative to the
    // first.
    void	setGangFlag(SbBool flag)	{ gangManips = flag; }
    SbBool	getGangFlag() const		{ return gangManips; }

    // Sets/returns the rotation-snapping tolerance, represented as a
    // number of degrees. The default is 10 degrees.
    void	setRotationTolerance(float numDegrees);
    float	getRotationTolerance() const;

    // Sets/returns the translation-snapping tolerance, represented as
    // an fraction of the window size. The default is .02.
    void	setTranslationTolerance(float frac) { transTolerance = frac; }
    float	getTranslationTolerance() const	    { return transTolerance; }

    // Sets/returns the scale-snapping tolerance, represented as
    // an fraction of the size of each dimension of bounding box of
    // the manipulated object. The default is .1.
    void	setScaleTolerance(float frac)	{ scaleTolerance = frac;}
    float	getScaleTolerance() const	{ return scaleTolerance; }

    // Snaps the manipulator with the given index to meet and align
    // with the first target point and plane
    void	snapNow(int whichManip);

  //mvo 8/1/96: changed from private so we can subclass
  protected:
    // This enum indicates what type of motion a manipulator is
    // undergoing
    enum ManipMotion {
	SCALING,
	ROTATING,
	CENTERING,
	TRANSLATING,
	UNKNOWN,		// Can't figure it out
	FIRST_MOTION,		// Haven't started yet
	NOT_MOVING,		// Not moving the manip
    };

    // This saves the current state of snapping as a bunch of bits.
    enum SnapState {

	// When rotation is snapped:
	ROTATE_X	   = 0x00001,	// About x-axis
	ROTATE_Y	   = 0x00002,	// About y-axis
	ROTATE_Z	   = 0x00004,	// About z-axis
	ROTATE_ANY	   = 0x00007,	// Any rotation

	// When translation is snapped to a principal plane of the
	// bounding box:
	TRANSLATE_X_MIN    = 0x00008,	// Left side
	TRANSLATE_X_CENTER = 0x00010,	// Plane between left and right sides
	TRANSLATE_X_MAX    = 0x00020,	// Right side
	TRANSLATE_Y_MIN    = 0x00040,	// Bottom side
	TRANSLATE_Y_CENTER = 0x00080,	// Plane between bottom and top sides
	TRANSLATE_Y_MAX    = 0x00100,	// Top side
	TRANSLATE_Z_MIN    = 0x00200,	// Back side
	TRANSLATE_Z_CENTER = 0x00400,	// Plane between back and front sides
	TRANSLATE_Z_MAX    = 0x00800,	// Front side
	TRANSLATE_ANY      = 0x00ff8,	// Any translation

	// When scaling is snapped:
	SCALE_X		   = 0x01000,	// Snapped X dimension
	SCALE_Y		   = 0x02000,	// Snapped Y dimension
	SCALE_Z		   = 0x04000,	// Snapped Z dimension
	SCALE_ANY	   = 0x07000,	// Any scale
    };

    SbPList		infos;			// List of info instances
    Target		*target;		// Snapping target
    Source		*source;		// Snapping source (if enabled)
    SbVec3f		targetPoint;		// World-space target location
    SbVec3f		targetNormal;		// World-space target normal
    SbVec3f		targetLengths;		// In 3 world-space dimensions
    SbBool		gangManips;		// Whether manip affects others
    float		rotTolerance;		// Rotation tolerance in 0-1
    float		transTolerance;		// Translation tolerance
    float		scaleTolerance;		// Scale tolerance
    int			snapState;		// State of snapping
    ManipMotion	motionType;		// Motion type
    SbBool		moveInDimension[3];	// Motion in a dimension?
    SbMatrix	origMatrixInv;		// Inverse of dragger's
						// motion matrix on mouse down

    // These save the starting translation, scale and center values so that we
    // can compare motion changes in each dimension
    SbVec3f		startTranslation, startScaleFactor, startCenter;

    // These are the minimum, center, and maximum points of the
    // object-space bounding box of the object being manipulated.
    // Since the box is aligned with the coordinate system planes, we
    // can use the individual coordinates separately.
    SbVec3f		boxPoints[3];	// Min, center, max (in that order)

    // These store the object-space point and normal of the Source
    // (when it is enabled).
    SbVec3f		objectSourcePoint, objectSourceNormal;

    // These store the dragger-space edge vector of the Source and the
    // length of the dragger-space edge of the Target. They are used
    // for scale snapping.
    SbVec3f		draggerSourceEdge;
    float		draggerTargetLength;

    SoGetBoundingBoxAction *bboxAction;
    SoGetMatrixAction	   *matrixAction;

    // Computes the object-space normal and center point of the side of
    // the dragger box to use for snap-now.
    void	computeSnapSide(SnapperManipInfo *info,
				SbVec3f &boxNormal, SbVec3f &boxCenter);

    // Rotates the given manipulator so that it aligns with the first
    // target plane normal
    void	rotateSnapNow(SnapperManipInfo *info,
			      const SbVec3f &startNormal);

    // Translates the given manipulator to center the correct face on
    // the first target point
    void	translateSnapNow(SnapperManipInfo *info,
				 const SbVec3f &startPoint);

    // These do the real work of interactive snapping
    void	initSnap(SnapperManipInfo *info);
    void	snap(SnapperManipInfo *info);
    void	finishSnap(SnapperManipInfo *info);

    // Initializes member variables when snapping is enabled
    void	initSnappingStuff(SnapperManipInfo *info,
				  const SbMatrix &worldToDraggerP,
				  const SbMatrix &worldToDraggerN,
				  const SbMatrix &draggerToWorldP);

    // Tests if rotation is close to the given object space normal. If so,
    // it snaps the rotation and returns TRUE.
    SbBool	snapRotation(SnapperManipInfo *info, const SbVec3f &normal);

    // Tests if translation is close to the plane perpendicular to the
    // given axis dimension (0 = X, 1 = Y, 2 = Z). If so, it snaps the
    // translation and returns TRUE. It also returns min, center, or
    // max as 0, 1, 2 in "which".
    SbBool	snapTranslation(SnapperManipInfo *info,
				int dimension, int &which);

    // Tests if center is close to the plane perpendicular to the
    // given axis dimension (0 = X, 1 = Y, 2 = Z). If so, it snaps the
    // center and returns TRUE. It also returns min, center, or
    // max as 0, 1, 2 in "which".
    SbBool	snapCenter(SnapperManipInfo *info,
				int dimension, int &which);

    // Tests if scaling in the given dimension is close to any target
    // scale length. If so, it snaps the scaling and returns TRUE.
    SbBool	snapScale(SnapperManipInfo *info, int dimension,
			  SbBool isUniform);

    // Transforms all ganged manips and the source by the given info's
    // motion matrix
    void	transform(SnapperManipInfo *info);

    // This computes the type of motion the manipulator is undergoing,
    // and the valid directions
    void	getManipMotion(SnapperManipInfo *info);

    // Computes the cumulative matrix from the changed fields
    void	computeNewMatrix(SnapperManipInfo *info,
				 SbMatrix &matrix, SbBool isJustTranslation);

    // This computes the center of transformation from the given info
    SbVec3f	getTransformCenter(SnapperManipInfo *info);

    friend class SnapperManipInfo;
};

//////////////////////////////////////////////////////////////////////
//
// Magic Lantern subclass of Snapper
//

class MleSnapper : public Snapper
{
  public:
    // Gets number of manips in snapper's manip list.
    int getNumManips() { return infos.getLength(); }
};

#endif /* __SNAPPER_H_ */
