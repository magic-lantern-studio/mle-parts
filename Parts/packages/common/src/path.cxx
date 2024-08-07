/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file path.h
 * @ingroup MleParts
 *
 * This file implements utility classes for path following.
 * This file includes:
 * <ol>
 * <li>the abstract base class for Path</li>
 * <li>an implementation of Path for multiple line segments</li>
 * <li>a package providing path following behavior for vehicles</li>
 * </ol>
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

// Include Magic Lantern header files.
#include "mle/mlMalloc.h"

#include "mle/path.h"
#include "mle/behavlib.h"


// ------------------------------------------------------------------------
// PathFollow steering behavior package
// ------------------------------------------------------------------------

PathFollow::PathFollow (Vehicle& vehicleRef, PursueEvade& peRef)
 : vehicle(vehicleRef),
   pe(peRef)
{
    predictionGain = DEFAULT_PATH_PREDICT;
}


// ------------------------------------------------------------------------
// This version is obsolete.  I'm leaving it in the file right now just for
// safety's sake.  The version below (with the PathDirection argument) used
// to be called steeringToFollowPath2 (note "2" at the end) but now it has
// replaced this older version and has inherited its name.


// void PathFollow::steeringToFollowPath (const Path *path,
// 				       MlVector3 *steering)
// {
//     // predict our future position based on current velocity
//     vehicle.linearPredictPosition (predictionGain, &futurePosition);

//     // map future position to "future position on path"
//     (*path).mapPointToPath (&futurePosition, &onPath);

//     // steer if the predicted future position outside the "path tube"
//     if (pointsWithinDistance ((*path).width/2, &onPath, &futurePosition))
// 	{
// 	    // fidling with what to do in the case where no steering is
// 	    // required to stay within the tube.  Either we are stopped
// 	    // inside the tube or we are already pointing in the right
// 	    // direction.

// // 	    // just coast
// // 	    (*steering) = MlVector3::zero;

// 	    // thrust straight ahead
// 	    (*steering) = vehicle.velocity;
// 	}
//     else
// 	// compute steering toward "future position on path"
// 	pe.steeringForStaticPursuit (&vehicle, &onPath, steering);
// }


// ------------------------------------------------------------------------
// Note that this is a slight generalization of the previous routine.  This
// one includes a pathDirection argument (which is one of PATH_FORWARD,
// PATH_EITHER, PATH_BACKWARD).  I added a new version rather than changing
// the older one for upward compatibility in calling sequences.  Eventually
// this might want to replace PathFollow::steeringToFollowPath


void PathFollow::steeringToFollowPath (const Path *path,
	const PathDirection pathDirection,
	MlVector3 *steering)
{
    // predict our future position based on current velocity
    vehicle.linearPredictPosition (predictionGain, &futurePosition);

    // determine a "future position on the path"
    if (pathDirection == PATH_EITHER)
	{
 	    (*path).mapPointToPath (&futurePosition, &onPath);
	}
    else
	{
	    MlScalar now = (*path).mapPointToPathLength (&(vehicle.position));
	    MlScalar speed = (vehicle.velocity).approximateLength ();
	    MlScalar dist = mlMul (predictionGain, vehicle.maxSpeed);
	    if (pathDirection == PATH_BACKWARD) dist = -dist;
	    (*path).mapPathLengthToPoint (now + dist, &onPath);
	}

    // steer if the predicted future position outside the "path tube"
    if (pointsWithinDistance ((*path).width/2, &onPath, &futurePosition))
	{
	    // fidling with what to do in the case where no steering is
	    // required to stay within the tube.  Either we are stopped
	    // inside the tube or we are already pointing in the right
	    // direction.

// 	    // just coast
// 	    (*steering) = MlVector3::zero;

	    // thrust straight ahead
	    (*steering) = vehicle.velocity;
	}
    else
	// compute steering toward "future position on path"
	pe.steeringForStaticPursuit (&onPath, steering);
}


// ------------------------------------------------------------------------
// class static variables


MlVector3 PathFollow::futurePosition;
MlVector3 PathFollow::onPath;


// ------------------------------------------------------------------------
// some geometric utilities                (move to some library sometime?)
// ------------------------------------------------------------------------


// Finds the location along a given line segment which is nearest to a
// given point.
//
// This is a simple implementation but perhaps not the fastest.  The
// square root in that call to normalize may actually not be required.
// But algorithms based on distances squared need to be concerned
// about fixed point overflow.


void nearestPointOnSegment (const MlVector3* testPoint,
	const MlVector3* endPoint0,
	const MlVector3* endPoint1, 
	MlVector3* result)
{
    // get unit vector along segment, and length
    MlVector3 unitAlong = (*endPoint1) - (*endPoint0);
    MlScalar segmentLength = unitAlong.normalize ();

    // vector from endpoint0 to test point
    MlVector3 toTest = (*testPoint) - (*endPoint0);

    // project onto segment
    MlScalar projection = unitAlong.dot (toTest);

    // use endpoints if <0 or > length
    if (projection < ML_SCALAR_ZERO)
	{
	    // first endpoint is nearest
	    *result = *endPoint0;
	}
    else
	{
	    if (projection > segmentLength)
		{
		    // second endpoint is nearest
		    *result = *endPoint1;
		}
	    else
		{
		    // projected point on segment is nearest
//		    *result = projection * unitAlong;
		    *result = *endPoint0 + (projection * unitAlong);
		}
	}
}


// ------------------------------------------------------------------------
// Finds the distance from a given line segment to a given point.


// not being used right now, replaced by nearestPointOnSegmentAndDistance
// below.


MlScalar distanceFromPointToSegment (const MlVector3* testPoint,
	const MlVector3* endPoint0,
	const MlVector3* endPoint1)
{
    MlVector3 nearest;
    nearestPointOnSegment (testPoint, endPoint0, endPoint1, &nearest);
    return ((*testPoint) - nearest).length();
}


// ------------------------------------------------------------------------
// Finds the distance from a given line segment to a given point, and also
// returns the point on the segment from whihc the distance was measured.


MlScalar distanceAndNearestPointOnSegment (const MlVector3* testPoint,
	const MlVector3* endPoint0,
	const MlVector3* endPoint1, 
	MlVector3* nearest)
{
    nearestPointOnSegment (testPoint, endPoint0, endPoint1, nearest);
    return ((*testPoint) - (*nearest)).length();
}


// ------------------------------------------------------------------------
// poly-line path
// ------------------------------------------------------------------------


PolyLinePath::PolyLinePath (int vertexCount,
	MlVector3 *vertexArray,
	MlScalar pathWidth,
	MlBoolean pathClosed)
{
    vertices = vertexCount;
//    vertex = vertexArray;
    width = pathWidth;
    closed = pathClosed;

    // malloc the vertex array so it can be dynamically lengthened
    vertex = (MlVector3 *)mlMalloc(vertices*sizeof(MlVector3));
    for ( int i = 0; i < vertices; i++ )
	vertex[i] = vertexArray[i];
}

PolyLinePath::PolyLinePath(void)
{
    vertices = 0;
    vertex = NULL;
    width = ML_SCALAR_ONE;
    closed = FALSE;
}

PolyLinePath::~PolyLinePath()
{
    // deallocate the vertex array
    mlFree(vertex);
}

int
PolyLinePath::setVertex(int index,const MlVector3& v)
{
	// if index is negative, add a new vertex
	if ( index < 0 )
		index = vertices;
	
	// see if we need to do reallocation
	if ( index >= vertices )
	{
		vertices = index + 1;
		vertex = (MlVector3 *)
			mlRealloc(vertex,vertices*sizeof(MlVector3));
	}
	
	// copy the vertex value
	vertex[index] = v;

	// return the index
	return index;
}

void
PolyLinePath::setWidth(MlScalar w)
{
	width = w;
}

void
PolyLinePath::setClosed(MlBoolean c)
{
	closed = c;
}

// ------------------------------------------------------------------------
// Maps a given point in space to the nearest point on the path.
//
// Loop over all path segments, find the distance to each, minimize this
// value, return the cooresponding nearest point on path.


void PolyLinePath::mapPointToPath (const MlVector3 *point,
	MlVector3 *onPath) const
{
    MlVector3 nearest;
    MlScalar minDistance = INFINITY;
    MlVector3 bgnV, endV;
 

    // loop over all path segments
    for (int i = 0; (i<(vertices-1) || (closed && i<vertices)); i++)
	{
	  if (i==vertices-1) // for closed path's last segment
	    {
	      bgnV = vertex[i];
	      endV = vertex[0];
	    }
	  else
	    {
	      bgnV = vertex[i];
	      endV = vertex[i+1];
	    }
	  // get distance to segment and nearest point on segment
	  MlScalar d = distanceAndNearestPointOnSegment (point,
							 &bgnV,
							 &endV,
							 &nearest);

	  // if smallest distance seen so far, save the nearest point
	  if (d < minDistance)
	    {
	      minDistance = d;
	      *onPath = nearest;
	    }
	}
}


// ------------------------------------------------------------------------
// This is a prototype implementation.  The length calculations should be
// cached at path creation time.
//
// This routime is almost identical to PolyLinePath::mapPointToPath, it
// would be nice for thme to share some common substrate.


MlScalar PolyLinePath::mapPointToPathLength (const MlVector3 *point) const
{
    MlVector3 nearest;
    MlScalar minDistance = INFINITY;
    MlScalar pathLength = ML_SCALAR_ZERO;
    MlScalar lengthOfPreviousSegments = ML_SCALAR_ZERO;
    MlVector3 bgnV, endV;

    // loop over all path segments
    for (int i = 0; (i < (vertices-1) || (closed && i<vertices)); i++)
	{
	  if (i==vertices-1)
	    {
	      bgnV = vertex[i];
	      endV = vertex[0];
	    }
	  else
	    {
	      bgnV = vertex[i];
	      endV = vertex[i+1];
	    }

	  // get distance to segment and nearest point on segment
	  MlScalar d = distanceAndNearestPointOnSegment (point,
							 &bgnV,
							 &endV,
							 &nearest);

	    // if smallest distance seen so far, save the path length
	  if (d < minDistance)
	    {
	      minDistance = d;
	      // sum of the distance alogn this segment and the
	      // length of all earlier segments.
	      pathLength = (lengthOfPreviousSegments +
			    (bgnV - nearest).length());
	    }

	    // add this segment's length into the accumulator.
	    lengthOfPreviousSegments += (bgnV - endV).length();
	}

  	
    return pathLength;
}


// ------------------------------------------------------------------------
// This is a prototype implementation.  The length calculations should be
// cached at path creation time.


void PolyLinePath::mapPathLengthToPoint (MlScalar pathLength,
	MlVector3 *point) const
{
    if ( vertices == 0 )
	return;

    // handle out-of-bounds path lengths
    if (closed)
	{
	   if (pathLength<ML_SCALAR_ZERO || pathLength >= this->length())
	     {*point = vertex[0];          return;}
	}
    else
	{
	  if (pathLength < ML_SCALAR_ZERO)
	    {*point = vertex[0];          return;}
	  if (pathLength >= this->length())
	    {*point = vertex[vertices-1]; return;}
	}

    // loop over all path segments
    MlScalar startLength = ML_SCALAR_ZERO;
    for (int i = 0; (i<(vertices-1) || (closed && i<vertices)); i++)
	{
	    MlVector3 segment;

	    if (i == vertices-1) // last segment of closed path
	      segment = vertex[0] - vertex[i];
	    else
	      segment = vertex[i+1] - vertex[i];

	    MlScalar thisLength = segment.length();
	    MlScalar endLength = startLength + thisLength;
	    if (pathLength < endLength)
		{
		    MlScalar dist = pathLength - startLength;
		    MlScalar ratio = mlDiv (dist, thisLength);
		    *point = vertex[i] + (ratio * segment);
		    return;
		}
	    startLength = endLength;
	}
}


// ------------------------------------------------------------------------
// This is a prototype implementation.  The calculations should be cached
// at path creation time.


MlScalar PolyLinePath::length (void) const
{
    MlScalar lengthOfPreviousSegments = ML_SCALAR_ZERO;

    
    // loop over all path segments summing lengths
    for (int i = 0; i < (vertices-1); i++)
	lengthOfPreviousSegments += (vertex[i] - vertex[i+1]).length();

    // if a closed path, add the last segment length connecting the last
    // and the first vertex.
    if (closed)
      lengthOfPreviousSegments += (vertex[vertices-1] - vertex[0]).length();

    return lengthOfPreviousSegments;
}

void
PolyLinePath::getStart(MlVector3& p) const
{
	if ( vertices == 0 )
		p.setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(0));
	else
		p = vertex[0];
}

void
PolyLinePath::getEnd(MlVector3& p) const
{
	if ( vertices == 0 )
		p.setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(0));
	else
		p = vertex[vertices - 1];
}
