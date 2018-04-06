/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file path.h
 * @ingroup MleParts
 *
 * This file defines a class for path follower vehicles.
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

#ifndef __MLE_PATHFOLL_H_
#define __MLE_PATHFOLL_H_

// Include Magic Lantern header files.
#include "mle/pe.h"
#include "mle/vehicle.h"
#include "math/vector.h"


// base Path class

class Path
{
  public:

    // The path is a tube of this diameter 
    MlScalar width;
    
    // This is the basic functionality of a path: given an arbitrary
    // point in space, project it to the "nearest" point on the path.
    virtual void mapPointToPath (const MlVector3 *point,
 		MlVector3 *onPath) const =0;
    
    // Like mapPointToPath, but returns result in terms of "path length
    // coordinate" (distance along the path)
	virtual MlScalar mapPointToPathLength (const MlVector3 *point) const =0;
    
    // Inverse of mapPointToPathLength, converts from "path length
    // coordinate" to a point on the path.
	virtual void mapPathLengthToPoint (MlScalar pathLength,
		MlVector3 *point) const =0;
    
    // Returns the path's total length
    virtual MlScalar length (void) const =0;
    
    // return the path start and end points
    virtual void getStart(MlVector3& p) const = 0;
    virtual void getEnd(MlVector3& p) const = 0;

};

// poly-line path

class PolyLinePath : public Path
{
  public:

    PolyLinePath (int vertexCount, MlVector3 *vertexArray,
		  MlScalar pathWidth, MlBoolean pathClosed);
    PolyLinePath(void);
    ~PolyLinePath();
    
    // Is path open (with a beginning and an end), or a closed loop? 
    // Closed means first point gets automotically connected to last.
    // Should this be in the base class?
    MlBoolean closed;
    
    // Number of vertices in path definition.
    int vertices;
    
    // Array of vertex locations
    MlVector3 *vertex;
    
    // set an arbitrary vertex in the path
    int setVertex(int index,const MlVector3& v);
    
    // set path parameters
    void setWidth(MlScalar width);
    void setClosed(MlBoolean closed);
    
    virtual void mapPointToPath (const MlVector3 *point,
		MlVector3 *onPath) const;
    
    virtual MlScalar mapPointToPathLength (const MlVector3 *point) const;
    
    virtual void mapPathLengthToPoint (MlScalar pathLength,
		MlVector3 *point) const;
    
    virtual MlScalar length (void) const;
    
    virtual void getStart(MlVector3& p) const;
    virtual void getEnd(MlVector3& p) const;
};


/**
 * @brief PathFollow
 *
 * This package provides an actor with the ability to follow (to fly
 * along) a predefined path.
 *
 * This package provides an actor with the ability to follow (to fly
 * along) a predefined path.  In addition to this package the actor
 * must contain a Vehicle package and a PursueEvade package.  The path
 * to be followed is defined by an instance of a class derived from
 * Path, such as PolyLinePath.
 */
class PathFollow
{
  public:

    // Constructor: requires references to sibling packages Vehicle
    // and PursueEvade of parent actor.
    PathFollow (Vehicle& vehicleRef, PursueEvade& peRef);

    // Path following can be done without regard to path direction
    // (PATH_EITHER), or can explictly go in the forward or backward
    // directions.
    enum PathDirection {PATH_FORWARD=1, PATH_EITHER=0, PATH_BACKWARD=-1};

    // reference member for Vehicle package
    Vehicle& vehicle;
    
    // reference member for PursueEvade package
    PursueEvade& pe;
    
    // Defines the relation between velocity and distance for prediction.
    MlScalar predictionGain;
    
    // Computes a steering vector given a path and a path direction.
    void steeringToFollowPath (const Path *path,
			       const PathDirection pathDirection,
			       MlVector3 *steering);
    
    // class storage of data for annotation use
    static MlVector3 futurePosition;
    static MlVector3 onPath;

  private:
};


#define DEFAULT_PATH_PREDICT ML_SCALAR (20)


#define mlRegisterPathFollowPackageMembers(actor,package)          \
{ mlRegisterActorMember (actor, package.predictionGain, MlScalar); }


#endif /* __MLE_PATHFOLL_H_ */
