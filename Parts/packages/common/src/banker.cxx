/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file banker.cxx
 * @ingroup MleParts
 *
 * This file implements a class for a vehicle capable of banking.
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2000-2024 Wizzer Works
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
//  For information concerning this source file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END


// Include system header files.
#include <stdio.h>


// Include Magic Lantern header files.
#include "mle/banker.h"
#include "mle/MleScheduler.h"
#include "mle/MleDirector.h"
#include "math/3dmath.h"


// ------------------------------------------------------------------------
//
// The MleSphericalSurface and MleToroidalSurface classes almost
// certainly should be in some other file.  Perhaps they should be in
// the "surface" title, since that is currently the only place they
// are used, and these are just intended to be sample implemetations.
//
// ------------------------------------------------------------------------


class MleSphericalSurface : public MleSurface
{
  public:

    MleSphericalSurface(void);

    MlScalar radius;
    MlVector3 position;

    // Given an arbitrary point in space, returns the nearest point
    // on the sphere and the surface normal at that point.
    virtual void mapPointToSurface (const MlVector3 *initialPoint,
				    MlVector3 *surfacePoint,
				    MlVector3 *surfaceNormal) const;
};


MleSphericalSurface::MleSphericalSurface (void)
{
    radius = ML_SCALAR (8);
    position = MlVector3::zero;
}



void MleSphericalSurface::mapPointToSurface (const MlVector3 *initialPoint,
 MlVector3 *surfacePoint, MlVector3 *surfaceNormal) const
{
    (*surfaceNormal) = (*initialPoint) - position;
    (*surfaceNormal).normalize();
    (*surfacePoint) = (*surfaceNormal) * radius;
}


// ------------------------------------------------------------------------
// toroidal surface class
//
// The torus is defined to be "lying" on the XZ plane.  The global Y axis
// is also the torus' major axis: through the middle of, and partallel to,
// the hole.  The torus is further described by two parameters, its major
// and minor radii.  Consider the intersection of the torus and a vertical
// (XY) plane: it is two circles.  The minor radius of the torus is the
// radius of either circle.  The distance from the global origin to the
// center of either circle is the major radius of the torus.  If the minor
// radius was zero, the torus would be a circle of the major radius.


class MleToroidalSurface : public MleSurface
{
  public:

    MleToroidalSurface(void);

    MlScalar majorRadius;
    MlScalar minorRadius;

    // Given an arbitrary point in space, returns the nearest point
    // on the toroidal surface and the normal to the surface at that
    // point.
    virtual void mapPointToSurface (const MlVector3 *initialPoint,
		MlVector3 *surfacePoint, MlVector3 *surfaceNormal) const;
};


MleToroidalSurface::MleToroidalSurface (void)
{
    majorRadius = ML_SCALAR (6);
    minorRadius = ML_SCALAR (3);
}



// Given an arbitrary point in space, return the nearest point on 
// the toroidal surface and the normal to the surface at that point.
void MleToroidalSurface::mapPointToSurface (const MlVector3 *initialPoint,
    MlVector3 *surfacePoint, MlVector3 *surfaceNormal) const
{

    // normal toward initial point in plane of torus
    MlVector3 majorNormal = *initialPoint;
    majorNormal[1] = ML_SCALAR_ZERO;
    if (majorNormal.isZero())
	    majorNormal[0] = ML_SCALAR_ONE;
    else
	    majorNormal.normalize();

    // this is the point nearest the initial point which is
    // along the circular "spline" of the torus.
    MlVector3 pointOnSpine = majorNormal * majorRadius;

    // offset from pointOnSpine, along line to initial point
    // with length equal to  minorRadius
    MlVector3 spineOffset = *initialPoint - pointOnSpine;
    (*surfaceNormal) = spineOffset;
    (*surfaceNormal).normalize();
    spineOffset = (*surfaceNormal) * minorRadius;

    (*surfacePoint) = pointOnSpine + spineOffset;
}


// ------------------------------------------------------------------------


Reorient::Reorient (SimpleMass& simplemass,MlBoolean schedFlag)
 : sm(simplemass)
{
    // insert the update function in the scheduler if requested
    if (schedFlag)
		g_theTitle->m_theScheduler->insertFunc(PHASE_ACTOR,
			(MleSchedulerFunc)update, this, this);
}


// ------------------------------------------------------------------------


Reorient::~Reorient()
{
    // remove any scheduled function
    g_theTitle->m_theScheduler->remove(this);
}


// ------------------------------------------------------------------------
// Given three mutually perpendicular vectors (representing up, right and
// forward) convert them into a value for the vehicle's orientation member.


void Reorient::setOrientationFromBasisVectors (MlVector3 *up,
	MlVector3 *right, MlVector3 *forward)
{
    MlScalar z = ML_SCALAR_ZERO;
    MlTransform m0 ((*right)[0],   (*right)[1],   (*right)[2],
		    (*up)[0],      (*up)[1],      (*up)[2],
		    (*forward)[0], (*forward)[1], (*forward)[2],
		    z,             z,             z);
	
    MlTransform m1 (sm.right[0],   sm.up[0],      sm.forward[0], 
		    sm.right[1],   sm.up[1],      sm.forward[1], 
		    sm.right[2],   sm.up[2],      sm.forward[2], 
		    z,             z,             z);

    // If m0 == 0, then pretend m0==I and let m1 = m1*m0 == m1.
    // Else do the multiply.
    if (! m0.isZero()) sm.orientation.setValue(m1*m0);
}


// ------------------------------------------------------------------------


void Reorient::update (Reorient *r)
{
    r->adjustVehicle ();
}


// ------------------------------------------------------------------------


ReorientBank::ReorientBank (SimpleMass& simplemass,MlBoolean schedFlag)
    : Reorient (simplemass, schedFlag)
{
    // init the per-OrientMode parameters:
    bankFactorAccel = ML_SCALAR (0.0006f);
    bankFactorGrav  = ML_SCALAR (0.03f);
}

// ------------------------------------------------------------------------


void ReorientBank::adjustVehicle (void)
{
    // get the new up 
    // (bankFactorGrav and bankFactorAccel are magic weighting
    // factors for balencing gravity and applied forces)
    MlVector3 up;
    sm.globalizeVector(sm.up,up);
    up += (bankFactorGrav*sm.up + bankFactorAccel*sm.acceleration);

    // get the new forward
    MlVector3 forward = sm.velocity;
    forward.normalize();

    // No velocity means no change in banking.
    if (forward.isZero()) return;

    // generate the right vector
    MlVector3 right = forward.cross(up);
    right.normalize();

    // regenerate up
    up = right.cross(forward);

    setOrientationFromBasisVectors(&up, &right, &forward);
}


// ------------------------------------------------------------------------


ReorientOnSurface::ReorientOnSurface (SimpleMass& simplemass,
				      MlBoolean schedFlag)
    : Reorient (simplemass, schedFlag)
{
    // no default surface
    surface = NULL;
}


// ------------------------------------------------------------------------
//
// change name now that the package name is more descriptive?
// mapVehicleToSurface
// constrainVehicleToSurface
// constrainToSurface
//
// Probably should not do that "new MleSphericalSurface".  
// Maybe the "surface == NULL" case should be a no-op?


void ReorientOnSurface::orientOnSurface (void)
{
    // allocate a default surface if needed
    if (surface == NULL) surface = new MleSphericalSurface;

    // map the vehicle's current position to a point on the
    // surface and the normal to the surface at that point.
    MlVector3 surfacePoint;
    MlVector3 surfaceNormal;
    surface->mapPointToSurface (&sm.position,
				&surfacePoint,
				&surfaceNormal);

    // constrain position to be on surface
    sm.position = surfacePoint;

    // constrain velocity to be tangent to surface
    MlScalar normalSpeed = sm.velocity.dot(surfaceNormal);
    MlVector3 normalVelocity = normalSpeed * (surfaceNormal);
    sm.velocity -= normalVelocity;

    // constrain orientation so "up" is normal to surface
    MlVector3 forward = sm.velocity;      // new forward direction
    if (forward.isZero())
	    sm.globalizeVector (sm.forward, forward);
    else
	    forward.normalize();
    MlVector3 up = surfaceNormal;         // new up direction
    MlVector3 right = forward.cross(up);  // new right direction
    setOrientationFromBasisVectors(&up, &right, &forward);
}


// ------------------------------------------------------------------------
//
// maybe this should be merged with orientOnSurface
//
// Probably should not do that "new MleSphericalSurface".  
// Maybe the "surface == NULL" case should be a no-op?


void ReorientOnSurface::adjustVehicle (void)
{
    // allocate a default surface if needed
    if (surface == NULL) surface = new MleToroidalSurface;

    // orient vehicle to the surface at the given point
    orientOnSurface ();
}


// ------------------------------------------------------------------------


ReorientUpright::ReorientUpright (SimpleMass& simplemass,
	MlBoolean schedFlag)
 : Reorient (simplemass, schedFlag)
{
}


// ------------------------------------------------------------------------


void ReorientUpright::adjustVehicle (void)
{
    // new up is global up
    MlVector3 up = sm.up;

    // get the new forward
    MlVector3 forward = sm.velocity;
    forward.normalize();

    // No velocity means no change in banking.
    if (forward.isZero()) return;

    // generate the right vector
    MlVector3 right = forward.cross(up);
    right.normalize();

    // regenerate forward
    forward = up.cross(right);

    setOrientationFromBasisVectors(&up, &right, &forward);
}
