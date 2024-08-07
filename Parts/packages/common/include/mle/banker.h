/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file banker.h
 * @ingroup MleParts
 *
 * This file defines a class for a vehicle capable of banking.
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

#ifndef __MLE_BANKER_H_
#define __MLE_BANKER_H_


// Include Magic Lantern header files.
#include "mle/mlTypes.h"
#include "mle/MleScheduler.h"
#include "mle/simpmass.h"


// base surface class (maybe this should be in a new surface.h)

/**
 * @brief Base class used to define surfaces for Vehicles to follow.
 *
 * @see ReorientOnSurface, Vehicle
 */
class MleSurface
{
  public:

    // This implements the basic functionality of a MleSurface:
    // mapping from an arbitary point in space to the "nearest"
    // point on the surface and the surface normal at that point.
    virtual void mapPointToSurface (const MlVector3 *initialPoint,
				    MlVector3 *surfacePoint,
				    MlVector3 *surfaceNormal) const =0;
};



/**
 * @brief Reorient base class for packages which adjust
 * the orientation of a Vehicle.
 *
 * A Vehicle defines a local coordinate space based on its velocity.
 * Packages based on the Reorient class adjust the orientation (and/or
 * position) of this local coordinate space to simulate various classes
 * of Vehicle.
 * 
 * @brief Vehicle, ReorientBank, ReorientOnSurface, ReorientUpright
 */
class Reorient
{
  public:

    // Constructor: initializes reference to SimpleMass and optionally
    // schedules the vehicle's update function.
    Reorient (SimpleMass& sm, MlBoolean schedFlag = TRUE);

    // Destructor: removes scheduler items associated with this instance.
    ~Reorient();
    
    // A reference to the mass associated with this Reorient instance.
    SimpleMass& sm;
    
    // Sets the SimpleMass's orientation from three mutually
    // perpendicular basis vectors.
    void setOrientationFromBasisVectors (MlVector3 *up,
					 MlVector3 *right,
					 MlVector3 *forward);

    // Adjusts the associated Vehicle's position and orientation, in a
    // manner determined by a specific Reorient class.
    virtual void adjustVehicle (void) =0;

    // Per frame update function (optionally scheduled by contructor).
    static void update (Reorient *reorient);
};


/**
 * @brief Reorient Bank package to adjust Vehicle orientation like
 * a banking aircraft.
 *
 * A Vehicle defines a local coordinate space based on its velocity.
 * The ReorientBank package adjusts the Vehicle's "roll" (rotation
 * about its local "forward" axis) to mimic an aircraft's banking.  Two
 * tuning parameters can be used to adjust the relative contribution of
 * turning acceleration (which rolls the vehicle so "up" points to the
 * center of path curvature) and the contribution of gravitational
 * acceleration (which rolls the vehicle so "up" points toward global
 * "up").
 * 
 * @see Vehicle, Reorient, ReorientBank, ReorientOnSurface, ReorientUpright
 */
class ReorientBank : public Reorient
{
  public:

    // Constructor: initializes reference to SimpleMass and optionally
    // schedules the vehicle's update function.
    ReorientBank (SimpleMass& sm, MlBoolean schedFlag = TRUE);

    // weighting for contribution due to turning acceleration.
    MlScalar bankFactorAccel;

    // weighting for contribution due to gravity.
    MlScalar bankFactorGrav;

    // Adjusts the vehicle's "roll" (rotation about its local forward
    // axis) to mimic an aircraft's banking.
    void adjustVehicle (void);
};


/**
 * @package Reorient On Surface package to adjust Vehicle orientation
 * as if sliding over a surface.
 *
 * A Vehicle defines a local coordinate space based on its velocity.
 * The ReorientOnSurface package adjusts the Vehicle's position and
 * orientation so that it follows an MleSurface.  This can be used to
 * create the impression of a Vehicle driving, skimming, or sliding
 * over an arbitrary surface.
 * 
 * @see Vehicle, Reorient, ReorientBank, ReorientOnSurface, ReorientUpright
 */
class ReorientOnSurface : public Reorient
{
  public:

    // Constructor: initializes reference to SimpleMass and optionally
    // schedules the vehicle's update function.
    ReorientOnSurface (SimpleMass& sm, MlBoolean schedFlag = TRUE);

    // For surface following: current surface, initially NULL
    MleSurface *surface;

    // Adjusts the SimpleMass's position, orientation, and velocity
    // in order to constrain it to motion on the current surface.
    void orientOnSurface (void);

    // Adjusts the SimpleMass's position, orientation, and velocity
    // in order to constrain it to motion on the current surface.
    void adjustVehicle (void);
};


/**
 * @brief Reorient Upright package to force Vehicle orientation to be "upright".
 *
 * A Vehicle defines a local coordinate space based on its velocity.
 * The ReorientUpright package adjusts the Vehicle's orientation so
 * that it remains "upright".  That is, that the vehicle's "local up"
 * axis remains aligned with the "global up" axis.  This was originally
 * used to combine Vehicle behaviors with articulated human figure
 * animation.
 * 
 * @see Vehicle, Reorient, ReorientBank, ReorientOnSurface, ReorientUpright
 */
class ReorientUpright : public Reorient
{
  public:

    // Constructor: initializes reference to SimpleMass and optionally
    // schedules the vehicle's update function.
    ReorientUpright (SimpleMass& sm, MlBoolean schedFlag = TRUE);

    // Align the vehicle's "local up" axis with the "global up" axis.
    void adjustVehicle (void);
};


#endif
