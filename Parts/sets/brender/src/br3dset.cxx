/** @defgroup MleParts Magic Lantern Parts */

/**
 * @file br3dset.cxx
 * @ingroup MleParts
 *
 * This file implements the class for a 3D Set targeting the BRender
 * platform.
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

// Include system header files.
#include <stdio.h>

#ifdef __sgi
#include <math.h>
#endif


// Include Magic Lantern header files.
#include "mle/mlAssert.h"

// Include Magic Lantern Math header files.
#include "math/scalarbr.h"
#include "math/vector.h"
#include "math/transfrm.h"

// Include BRender target header files.
#include "mle/br3dset.h"
#include "mle/br3drole.h"
#include "mle/brstage.h"

#ifdef MLE_REHEARSAL
#include "mle/stagefuncs.h"
#endif /* MLE_REHEARSAL */


MLE_SET_SOURCE(Mle3dSet,MleSet);


#ifdef MLE_REHEARSAL
void
Mle3dSet::initClass(void)
{
	mlRegisterSetClass(Mle3dSet,MleSet);

	// Set properties go here.
	mlRegisterForumMember(Mle3dSet, position, MlVector2);
	mlRegisterForumMember(Mle3dSet, size, MlVector2);
}
#endif /* MLE_REHEARSAL */


Mle3dSet::Mle3dSet(void)
{
	// Create the scene graph root.
	m_root = BrActorAllocate(BR_ACTOR_NONE,NULL);

	// XXX - create the camera - this should be done by a camera actor.
	m_camera = BrActorAllocate(BR_ACTOR_CAMERA,NULL);
	BrActorAdd(m_root,m_camera);
	setCameraAspect(ML_SCALAR(0));
	m_camera->t.type = BR_TRANSFORM_TRANSLATION;
	BrVector3Set(&m_camera->t.t.translate.t,
		BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(3.0));

    // XXX - create a light - this could be done by a light actor.
	br_actor *light = BrActorAllocate(BR_ACTOR_LIGHT,NULL);
	BrActorAdd(m_root,light);
	BrLightEnable(light);

	// XXX - rotate the bogus startup light a little higher.
	light->t.type = BR_TRANSFORM_MATRIX34;
	light->t.t.mat.m[0][0] = BR_SCALAR(1);
	light->t.t.mat.m[0][1] = BR_SCALAR(0);
	light->t.t.mat.m[0][2] = BR_SCALAR(0);
	light->t.t.mat.m[1][0] = BR_SCALAR(0);
	light->t.t.mat.m[1][1] = BR_SCALAR(0.707);
	light->t.t.mat.m[1][2] = BR_SCALAR(-0.707);
	light->t.t.mat.m[2][0] = BR_SCALAR(0);
	light->t.t.mat.m[2][1] = BR_SCALAR(0.707);
	light->t.t.mat.m[2][2] = BR_SCALAR(0.707);
	light->t.t.mat.m[3][0] = BR_SCALAR(0);
	light->t.t.mat.m[3][1] = BR_SCALAR(0);
	light->t.t.mat.m[3][2] = BR_SCALAR(0);

	// Add the Set to the Stage.
	BrStage *stage = BrStage::cast(MleStage::g_theStage);
	stage->addSet((void (*)(MleSet *))render,this);

#ifdef WIN32
	m_lastDirtyRectangle = 0;
#endif /* WIN32 */
}

void
Mle3dSet::attach(MleRole *p,MleRole *c)
{
	// Cast to a Mle3dRole
	MLE_ASSERT(c);
	Mle3dRole *child = Mle3dRole::cast(c);

	// If parent is non-NULL, attach role-to-role.
	if ( p )
	{
		// Cast parent to a Mle3dRole
		Mle3dRole *parent = Mle3dRole::cast(p);

		// Do the attachment.
		parent->addChild(child);
	}
	else
	{
		// Otherwise, attach the child to the Set.
		br_actor *childRoot = child->getRoot();
		MLE_ASSERT(childRoot);

		BrActorAdd(m_root,childRoot);
	}
}


int Mle3dSet::setCamera(MleActor *actor)
{
	int result=FALSE;

    br_actor* newCamera = NULL;
    if (actor && actor->getRole())
        newCamera=(br_actor*)(Mle3dRole::cast(actor->getRole())->getRoot());

    if (newCamera && (newCamera->type == BR_ACTOR_CAMERA))
    {
        m_camera = newCamera;
        result=TRUE;
    }

    return result;
}


int Mle3dSet :: setCameraTransform(MlTransform *t)
{
    m_camera->t.type = BR_TRANSFORM_MATRIX34;

    // Set the BRender matrix, and
    // convert to a MAgic LAntner transform.
    for (int i = 0; i < 4; i++)
    {
	    for (int j = 0; j < 3; j++)
	        m_camera->t.t.mat.m[i][j] = mlScalarToBRScalar((*t)[i][j]);
    }

    // Any input is allowable, so return success.
    return 0;
}


void Mle3dSet :: getCameraTransform(MlTransform *t)
{
    // Turn the camera transform into a matrix.
    br_matrix34 m;
    BrTransformToMatrix34(&m,&m_camera->t);

    // Convert to a Magic Lantern transform.
    for (int i = 0; i < 4; i++)
    {
	for (int j = 0; j < 3; j++)
	    (*t)[i][j] = mlBRScalarToScalar(m.m[i][j]);
    }
}


int
Mle3dSet::setCameraPosition(const MlVector3 *position)
{
	// If the current transform is the identity type, change to translation.
	if ( m_camera->t.type == BR_TRANSFORM_IDENTITY )
		m_camera->t.type = BR_TRANSFORM_TRANSLATION;
	
	// The following is a minor hack, but saves code.
	//   BRender has many different flavors of transforms, but
	//   they all (except identity) save the translation in the
	//   last three words.  This means we can pick any style
	//   as long as it isn't identity and things just work.
	m_camera->t.t.translate.t.v[0] = mlScalarToBRScalar((*position)[0]);
	m_camera->t.t.translate.t.v[1] = mlScalarToBRScalar((*position)[1]);
	m_camera->t.t.translate.t.v[2] = mlScalarToBRScalar((*position)[2]);

	return 0;
}


void
Mle3dSet::getCameraPosition(MlVector3 *position)
{
	// If the current transform is identity, the position is the origin
	if ( m_camera->t.type == BR_TRANSFORM_IDENTITY )
		position->setValue(ML_SCALAR(0),ML_SCALAR(0),ML_SCALAR(0));
	else
		position->setValue(
			mlBRScalarToScalar(m_camera->t.t.translate.t.v[0]),
			mlBRScalarToScalar(m_camera->t.t.translate.t.v[1]),
			mlBRScalarToScalar(m_camera->t.t.translate.t.v[2]));
}

	
int
Mle3dSet::setCameraOrientation(const MlRotation *orientation)
{
	// Preserve the current translation.
	//    This is a minor hack, but saves code.  BRender has many
	//    different flavors of transforms, but they all (except
	//    identity) save the translation in the last three words.
	//    This means translation will be preserved if we change
	//    from any translation style except identity.
	if ( m_camera->t.type == BR_TRANSFORM_IDENTITY )
	{
		m_camera->t.t.mat.m[3][0] = BR_SCALAR(0);
		m_camera->t.t.mat.m[3][1] = BR_SCALAR(0);
		m_camera->t.t.mat.m[3][2] = BR_SCALAR(0);
	}

	m_camera->t.type = BR_TRANSFORM_MATRIX34;

	// Now set the orientation by quaternion.
	MlTransform t;
	orientation->getValue(t);

	// Set the BRender matrix.
	m_camera->t.t.mat.m[0][0] = mlScalarToBRScalar(t[0][0]);
	m_camera->t.t.mat.m[0][1] = mlScalarToBRScalar(t[0][1]);
	m_camera->t.t.mat.m[0][2] = mlScalarToBRScalar(t[0][2]);
	m_camera->t.t.mat.m[1][0] = mlScalarToBRScalar(t[1][0]);
	m_camera->t.t.mat.m[1][1] = mlScalarToBRScalar(t[1][1]);
	m_camera->t.t.mat.m[1][2] = mlScalarToBRScalar(t[1][2]);
	m_camera->t.t.mat.m[2][0] = mlScalarToBRScalar(t[2][0]);
	m_camera->t.t.mat.m[2][1] = mlScalarToBRScalar(t[2][1]);
	m_camera->t.t.mat.m[2][2] = mlScalarToBRScalar(t[2][2]);

	// Any input is allowable, so return success.
	return 0;
}

void
Mle3dSet::getCameraOrientation(MlRotation *orientation)
{
	// Turn the camera transform into a matrix.
	br_matrix34 m;
	BrTransformToMatrix34(&m,&m_camera->t);

	// XXX - This is a good example of the kind of code we need to get rid of.

	// Convert to a Magic Lanter transform.
	MlTransform t;
	t.makeIdentity();
	t[0][0] = mlBRScalarToScalar(m.m[0][0]);
	t[0][1] = mlBRScalarToScalar(m.m[0][1]);
	t[0][2] = mlBRScalarToScalar(m.m[0][2]);
	t[1][0] = mlBRScalarToScalar(m.m[1][0]);
	t[1][1] = mlBRScalarToScalar(m.m[1][1]);
	t[1][2] = mlBRScalarToScalar(m.m[1][2]);
	t[2][0] = mlBRScalarToScalar(m.m[2][0]);
	t[2][1] = mlBRScalarToScalar(m.m[2][1]);
	t[2][2] = mlBRScalarToScalar(m.m[2][2]);

	// Set the orientation.
	orientation->setValue(t);
}


#ifdef WIN32

// I would be interested in finding out if the ViewBoundsCallback and the
// curForum variable could be made into private static data members of the
// class - not important, but wraps things up a bit neater than using
// friend functions.

static void BR_CALLBACK ViewBoundsCallback(
        br_actor *actor,
        br_model *model,
        br_material *material,
	    void *render_data,
        br_uint_8 style,
        br_matrix4 *model_to_screen,
        br_int_32 *bounds);


// Keep a copy of the current Set for use by the bounds callback.
static Mle3dSet* g_curSet;

#endif /* WIN32 */


int
Mle3dSet::setCameraNearClipping(MlScalar nearPlane)
{
	// XXX - BRender has a bug with calculating the near clipping
	//   plane.  If the hither and yon planes are desired to
	//   be at nearPlane and farPlane, then you have to tell
	//   BRender to put the near clipping plane at
        //   (farPlane*nearPlane)/(2*farPlane - nearPlane)

	MlScalar farPlane = mlBRScalarToScalar(
		((br_camera *)m_camera->type_data)->yon_z);
	((br_camera *)m_camera->type_data)->hither_z = mlScalarToBRScalar(
		mlMulDiv(farPlane,nearPlane,farPlane + farPlane - nearPlane));
	
	return 0;	// This means success.
}


MlScalar
Mle3dSet::getCameraNearClipping(void)
{
	// XXX - BRender has a bug with calculating perspective
	//   transformations.  You get the wrong near clipping
	//   plane.  If BRender thinks the planes are at near
	//   and far, the actual near is
    //   2*farPlane*nearPlane/(farPlane + nearPlane)
	MlScalar nearPlane = mlBRScalarToScalar(
		((br_camera *)m_camera->type_data)->hither_z);
	MlScalar farPlane = mlBRScalarToScalar(
		((br_camera *)m_camera->type_data)->yon_z);
	
	return mlMulDiv(nearPlane + nearPlane,farPlane,farPlane + nearPlane);
}


int
Mle3dSet::setCameraFarClipping(MlScalar farPlane)
{
	((br_camera *)m_camera->type_data)->yon_z = mlScalarToBRScalar(farPlane);

	return 0;	// This means success.
}

MlScalar
Mle3dSet::getCameraFarClipping(void)
{
	return mlBRScalarToScalar(
		((br_camera *)m_camera->type_data)->yon_z);
}

int
Mle3dSet::setCameraAspect(MlScalar aspect)
{
	// Special case zero to mean set with the stage aspect ratio.
	if ( aspect == ML_SCALAR_ZERO )
	{
		// Get the stage size.
		int width,height;
#ifdef MLE_REHEARSAL
		MleStage::g_theStage->getSize(&width,&height);
#else /* MLE_REHEARSAL */
		// XXX quick fix for build.
		width = 640;
		height = 480;
#endif /* MLE_REHEARSAL */

		// oOerwrite the aspect.
		aspect = mlDiv(mlLongToScalar(width),mlLongToScalar(height));
	}

	((br_camera *)m_camera->type_data)->aspect = mlScalarToBRScalar(aspect);

	return 0;	// This means success.
}

MlScalar
Mle3dSet::getCameraAspect(void)
{
	return mlBRScalarToScalar(
		((br_camera *)m_camera->type_data)->aspect);
}

void
Mle3dSet::render(MleSet *s)
{
    Mle3dSet *set = Mle3dSet::cast(s);

    MLE_ASSERT(set->m_root);
    MLE_ASSERT(set->m_camera);

    // get the stage.
    BrStage *stage = BrStage::cast(MleStage::g_theStage);

#ifdef ASDF
    // XXX quick fix for aspect.
    int width,height;
    stage->getSize(&width,&height);
    ((br_camera *)set->m_camera->type_data)->aspect =
        BR_DIV(BrIntToScalar(width),BrIntToScalar(height));
#endif /* ASDF */
	
#ifdef WIN32
    // Set clear bounds based on the union dirty rectangle's
    // This was computed as the render_bounds of the previous frame.
    set->m_clearBounds = set->m_renderBounds;
 
    // Reset dirty rectangle list.
    set->m_lastDirtyRectangle = 0;
    set->m_renderBounds.min_x = INT_MAX;
    set->m_renderBounds.min_y = INT_MAX;
    set->m_renderBounds.max_x = INT_MIN;
    set->m_renderBounds.max_y = INT_MIN;
 
    // Hook dirty rectangle callback, and render view.
    g_curSet = set; // Use static to pass to ViewBoundsCallback.
    br_renderbounds_cbfn* old_cb =
        BrZbSetRenderBoundsCallback(ViewBoundsCallback);

#endif /* WIN32 */

    // Do the BRender rendering.
    //   Don't do the buffer exchange here.  There may be several Sets
    //   run on this Stage, so the Stage will take care of the double
    //   buffering.
    BrZbSceneRender(set->m_root,set->m_camera,
        stage->getPixelBuffer(),stage->getDepthBuffer());

#ifdef WIN32
    BrZbSetRenderBoundsCallback(old_cb);

    // Update the Set with the latest regions that need to be blitted.
    stage->dirtyRegion(&(set->m_renderBounds));
    stage->dirtyRegion(&(set->m_clearBounds));
#endif /* WIN32 */
}


// Picking.
//   This interface is defined for rehearsal only.
#ifdef MLE_REHEARSAL

// Declare static variable to hold pick information.
static br_actor *pickActor;
static br_scalar pickDistance;

#if 0
static int BR_CALLBACK pickCB(br_actor *actor,br_model *,
	br_vector3 *,br_vector3 *,br_scalar near,br_scalar,void *)
#endif /* 0 */
static int BR_CALLBACK pickCB(
		br_actor *actor,
		br_model *,
		br_material *,
		br_vector3 *, br_vector3 *,
		br_scalar near, br_scalar ,
		void *)
{
    // If we are beyond the currently picked object then just return
    //	if ( pickActor && pickDistance <= near )
    //		return 0;

	while ( actor )
	{
		if ( MleRole::pickRegistry.find(actor) )
		    break;

		actor = actor->parent;
	}

	if ( actor )
	{
		pickActor = actor;
		pickDistance = near;

		// XXX - replace this line and uncomment the first two lines
		//   to use bounding box proximity for priority
		return 1;
	}

	return 0;
}

MleRole *
Mle3dSet::pickRole(int x, int y)
{
	MLE_ASSERT(m_root);
	MLE_ASSERT(m_camera);

	// Get the stage.
	BrStage *stage = BrStage::cast(MleStage::g_theStage);

	// Correct screen coordinates.
	//   BRender always thinks the window size is 640x480, which is
	//   the startup size for the SGI stage.  The window can change
	//   size without BRender knowing it.  We need to fix up the screen
	//   coordinates to map into 640x480.
	int width,height;
	stage->getSize(&width,&height);
	int px = (x - width/2)*640/width;
	int py = (y - height/2)*480/height;

	// Call BRender to do the pick.
	pickActor = NULL;
	BrScenePick2D(m_root,m_camera,stage->getFrontBuffer(),
		px,py,pickCB,this);
	
	if ( pickActor )
		return (MleRole *)MleRole::pickRegistry.find(pickActor);

	return NULL;
}

#endif /* MLE_REHEARSAL picking */


#ifdef MLE_REHEARSAL

struct IntersectCBData
{
    br_vector3 vec;
    int found;
};


static int BR_CALLBACK intersectCB(br_actor *actor,
				   br_model *,
				   br_material *,
				   br_vector3 *pos,
				   br_vector3 *dir,
				   br_scalar near,
				   br_scalar /*far*/,
				   void *data)
{
    IntersectCBData *callerData = (IntersectCBData *) data;
    callerData->found = 1;

    // XXX - it seems BRender is handing us y and z swapped in pos and dir
    // vectors! So, let's swap y and z... --mvo 1/24/96
    br_scalar s;
    s = pos->v[1];
    pos->v[1] = pos->v[2];
    pos->v[2] = s;
    s = dir->v[1];
    dir->v[1] = dir->v[2];
    dir->v[2] = s;

    // XXX - and let's invert Z's sign -- thanks, boys!
    pos->v[2] = -pos->v[2];
    dir->v[2] = -dir->v[2];

#if 0
#define mlBRScalarToFloat(n) mlScalarToFloat(mlBRScalarToScalar(n))

printf("BR intersectCB: pos: %f, %f, %f\n",
 mlBRScalarToFloat(pos->v[0]), 
 mlBRScalarToFloat(pos->v[1]), 
 mlBRScalarToFloat(pos->v[2]));

printf("BR intersectCB: dir: %f, %f, %f\n",
 mlBRScalarToFloat(dir->v[0]), 
 mlBRScalarToFloat(dir->v[1]), 
 mlBRScalarToFloat(dir->v[2]));

printf("BR intersectCB: near %f\n", 
 mlBRScalarToFloat(near));
#endif /* 0 */

    // Calculate position of intersection in actor space.
    br_vector3 distance;
    BrVector3Scale(&distance, dir, near);
    BrVector3Accumulate(pos, &distance);

#if 0
printf("BR intersectCB: distance: %f, %f, %f\n",
 mlBRScalarToFloat(distance.v[0]), 
 mlBRScalarToFloat(distance.v[1]), 
 mlBRScalarToFloat(distance.v[2]));

printf("BR intersectCB: pos now: %f, %f, %f\n",
 mlBRScalarToFloat(pos->v[0]), 
 mlBRScalarToFloat(pos->v[1]), 
 mlBRScalarToFloat(pos->v[2]));
#endif /* 0 */

    // Convert position to world coords by adding actor position.
    // ...first find root node
    br_actor *parentActor = actor;
    while (parentActor->parent != NULL)
	    parentActor = parentActor->parent;
	
    // Calculate xform from actor to root.
    br_matrix34 mat;
    BrActorToActorMatrix34(&mat, actor, parentActor);

#if 0
printf("BR intersectCB: actor->world translation: %f, %f, %f\n",
 mlBRScalarToFloat(mat.m[3][0]), 
 mlBRScalarToFloat(mat.m[3][1]), 
 mlBRScalarToFloat(mat.m[3][2]));
#endif

    // Take the translation and add to position.
    BrVector3Accumulate(pos, (br_vector3 *) mat.m[3]);

#if 0
printf("BR intersectCB: final pos: %f, %f, %f\n",
 mlBRScalarToFloat(pos->v[0]), 
 mlBRScalarToFloat(pos->v[1]), 
 mlBRScalarToFloat(pos->v[2]));
#endif

    // Copy into caller's vector.
    BrVector3Copy(&callerData->vec, pos);

    return 1;	// This signals completion of picking.
}

// Similar to pickRole(), but instead of returning a Role,
// it returns the point of intersection of a ray cast into the screen
// and geometry in the scene, from a given screen x,y position.

void 
Mle3dSet::intersectScreenCoordinates(int x, int y, FwScalar* coord)
{
    MLE_ASSERT(m_root);
    MLE_ASSERT(m_camera);

    // Get the stage.
    BrStage *stage = BrStage::cast(Stage::g_theStage);

    // Correct screen coordinates.
    //   BRender always thinks the window size is 640x480, which is
    //   the startup size for the SGI stage.  The window can change
    //   size without BRender knowing it.  We need to fix up the screen
    //   coordinates to map into 640x480.
    int width,height;
    stage->getSize(&width,&height);
    int px = (x - width/2)*640/width;
    int py = (y - height/2)*480/height;

    // Call BRender to do the intersection - it will fill in our
    // caller's coord value.
    IntersectCBData data;
    data.found = FALSE;
    BrScenePick2D(root, camera, stage->getFrontBuffer(),
	    px, py, intersectCB, &data);
    if (data.found)
    {
		// Set the point of intersection.
		coord[0] = mlBRScalarToScalar(data.vec.v[0]);
		coord[1] = fmlRScalarToScalar(data.vec.v[1]);
		coord[2] = fmlRScalarToScalar(data.vec.v[2]);
	}
    else
    {
		coord[0] = coord[1] = coord[2] = ML_SCALAR_ZERO;
    }
}

#endif	/* MLE_REHEARSAL intersect */

#ifdef MLE_REHEARSAL

static void calculateBounds(br_actor *actor,br_scalar *pmin,br_scalar *pmax);

void
Mle3dSet::getBounds(MlScalar pmin[3],MlScalar pmax[3])
{
	// Declare local min and max vectors.
	//   These are minimum and maximum values for x, y, and z.
	static br_scalar min[3] = {BR_SCALAR_MAX,BR_SCALAR_MAX,BR_SCALAR_MAX};
	static br_scalar max[3] = {-BR_SCALAR_MAX,-BR_SCALAR_MAX,-BR_SCALAR_MAX};

	// Initialize to bogus values.
    //	min[0] = min[1] = min[2] = BR_SCALAR_MAX;
    //	max[0] = max[1] = max[2] = -BR_SCALAR_MAX;

	calculateBounds(m_root,min,max);
	
	pmin[0] = mlBRScalarToScalar(min[0]);
	pmin[1] = mlBRScalarToScalar(min[1]);
	pmin[2] = mlBRScalarToScalar(min[2]);
	pmax[0] = mlBRScalarToScalar(max[0]);
	pmax[1] = mlBRScalarToScalar(max[1]);
	pmax[2] = mlBRScalarToScalar(max[2]);
}

void 
Mle3dSet::projectScreenCoordinates(int x,int y,MlScalar* coord)
{
	// Fet the stage width and height.
	int width,height;
	MleStage::g_theStage->getSize(&width,&height);

	// Get normalized device coords.
	float xs = (float)(2*x - width)/(float)width;
	float ys = (float)(height - 2*y)/(float)height;

	// Fix x position with aspect.
	xs *= BrScalarToFloat(((br_camera *)m_camera->type_data)->aspect);

	// Get the camera field of view.
	float fov = BrScalarToFloat(
			BrAngleToRadian(
			    ((br_camera *)m_camera->type_data)->field_of_view));

	// Determine a depth.
	// XXX - BRender has a bug with calculating perspective
	//   transformations.  You get the wrong near clipping
	//   plane.  If BRender thinks the planes are at near
	//   and far, the actual near is 2*far*near/(far + near)
	float near =
	    BrScalarToFloat(((br_camera *)m_camera->type_data)->hither_z);
	float far =
	    BrScalarToFloat(((br_camera *)m_camera->type_data)->yon_z);
	float depth = 0.5*(2.0f*near*far/(near + far) + far);

	// XXX
	//   Choose some constant distance from the camera to place things at.
	//   If this is outside the current bounds, place it at the near clip.
	//   This is kind of a hack.
	depth = 20.0f;
	if ( depth < 2*far*near/(far + near) ) depth = 2*far*near/(far + near);
	if ( depth > far ) depth = far;

	// Get a multiplier for the screen coordinates.
	float t = ftan(0.5*fov)*depth;

	// Get the point.
	br_vector3 v;
	v.v[0] = BrFloatToScalar(xs*t);
	v.v[1] = BrFloatToScalar(ys*t);
	v.v[2] = BrFloatToScalar(-depth);

	// Apply camera transform.
	br_vector3 u;
	br_matrix34 m;
	BrTransformToMatrix34(&m,&camera->t);
	BrMatrix34ApplyP(&u,&v,&m);

	// Write destination.
	coord[0] = mlBRScalarToScalar(u.v[0]);
	coord[1] = mlBRScalarToScalar(u.v[1]);
	coord[2] = mlBRScalarToScalar(u.v[2]);
}


static void calculateBounds(br_actor *actor,br_scalar *pmin,br_scalar *pmax)
{
    MLE_ASSERT(actor);

    // Declare local min and max vectors.
    //   These are minimum and maximum values for x, y, and z.
    br_scalar min[3];
    br_scalar max[3];

    // Initialize to bogus values.
    min[0] = min[1] = min[2] = BR_SCALAR_MAX;
    max[0] = max[1] = max[2] = -BR_SCALAR_MAX;

    // traverse
    br_actor *child = actor->children;
    while ( child )
    {
        calculateBounds(child,min,max);
        child = child->next;
    }

    // Look at the local bounds if this is a MODEL actor.
    if ( actor->type == BR_ACTOR_MODEL )
    {
		if ( actor->model )
		{
			// Check minimum.
			if ( actor->model->bounds.min.v[0] < min[0] )
				min[0] = actor->model->bounds.min.v[0];
			if ( actor->model->bounds.min.v[1] < min[1] )
				min[1] = actor->model->bounds.min.v[1];
			if ( actor->model->bounds.min.v[2] < min[2] )
				min[2] = actor->model->bounds.min.v[2];

			// Check maximum.
			if ( actor->model->bounds.max.v[0] > max[0] )
				max[0] = actor->model->bounds.max.v[0];
			if ( actor->model->bounds.max.v[1] > max[1] )
				max[1] = actor->model->bounds.max.v[1];
			if ( actor->model->bounds.max.v[2] > max[2] )
				max[2] = actor->model->bounds.max.v[2];
		}
		else
		{
			// It's the default model, the bi-unit cube.
			if ( BR_SCALAR(-1) < min[0] )
				min[0] = BR_SCALAR(-1);
			if ( BR_SCALAR(-1) < min[1] )
				min[1] = BR_SCALAR(-1);
			if ( BR_SCALAR(-1) < min[2] )
				min[2] = BR_SCALAR(-1);
			if ( BR_SCALAR(1) > max[0] )
				max[0] = BR_SCALAR(1);
			if ( BR_SCALAR(1) > max[1] )
				max[1] = BR_SCALAR(1);
			if ( BR_SCALAR(1) > max[2] )
				max[2] = BR_SCALAR(1);
		}
    }
    // XXX - can add else if clauses for other types, like lights.

    // If there isn't anything here, don't go any further
    if ( max[0] < min[0] )
        return;

    // now transform all points of the bounding box
    br_matrix34 mat;
    int identity = (actor->t.type == BR_TRANSFORM_IDENTITY);

    if ( !identity )
        BrTransformToMatrix34(&mat,&actor->t);

    br_vector3 src,dst;

#define XFORM(X,Y,Z) \
    if ( identity ) { \
        dst.v[0] = X; dst.v[1] = Y; dst.v[2] = Z; \
    } \
    else { \
        src.v[0] = X; src.v[1] = Y; src.v[2] = Z; \
        BrMatrix34ApplyP(&dst,&src,&mat); \
    } \
    if ( dst.v[0] < pmin[0] ) pmin[0] = dst.v[0]; \
    if ( dst.v[1] < pmin[1] ) pmin[1] = dst.v[1]; \
    if ( dst.v[2] < pmin[2] ) pmin[2] = dst.v[2]; \
    if ( dst.v[0] > pmax[0] ) pmax[0] = dst.v[0]; \
    if ( dst.v[1] > pmax[1] ) pmax[1] = dst.v[1]; \
    if ( dst.v[2] > pmax[2] ) pmax[2] = dst.v[2]

    XFORM(min[0],min[1],min[2]);
    XFORM(max[0],min[1],min[2]);
    XFORM(min[0],max[1],min[2]);
    XFORM(max[0],max[1],min[2]);
    XFORM(min[0],min[1],max[2]);
    XFORM(max[0],min[1],max[2]);
    XFORM(min[0],max[1],max[2]);
    XFORM(max[0],max[1],max[2]);
}


//
// Sending Set functional info to the tools.
//

const char** Mle3dSet::getFunctions()
{
    static char *funcs[] = 
    {
		NULL
    };
    
    return funcs;
}

const char** Mle3dSet::getFunctionAttributes(char* functionName)
{
    printf("ERROR Mle3dSet(br)::getFunctionAttributes: unknown func name '%s'\n", 
	    functionName);
    return NULL;
}

// Global render mode.
void 
Mle3dSet::setRenderMode(char* renderMode)
{
    br_uint_8 style;
    if (!strcmp(renderMode, RENDER_AS_IS))
    {
		style = BR_RSTYLE_DEFAULT;
    }
    else if (!strcmp(renderMode, RENDER_EDGES))
    {
		style = BR_RSTYLE_EDGES;
    }
    else if (!strcmp(renderMode, RENDER_POINTS))
    {
		style = BR_RSTYLE_POINTS;
    }
    else if (!strcmp(renderMode, RENDER_FULL))
    {
		style = BR_RSTYLE_FACES;
    }
    else if (!strcmp(renderMode, RENDER_BBOX_POINTS))
    {
		style = BR_RSTYLE_BOUNDING_POINTS;
    }
    else if (!strcmp(renderMode, RENDER_BBOX))
    {
		style = BR_RSTYLE_BOUNDING_EDGES;
    }
    else if (!strcmp(renderMode, RENDER_BBOX_FACES))
    {
	s	tyle = BR_RSTYLE_BOUNDING_FACES;
    }
    else
    {
		printf("ERROR Mle3dSet (br) setRenderMode: unknown render mode '%s'\n", 
			renderMode);
 		style = BR_RSTYLE_DEFAULT;
    }

    root->render_style = style;
}

	char* 
	Mle3dSet::getRenderMode()
	{
	printf("Mle3dSet (br): getRenderMode not implemented\n");
		return(NULL);
	}

#endif	/* MLE_REHEARSAL */


#ifdef WIN32
// Callback function to get dirty rectangles back from renderer.  Keeps
// track of the dirty rectangles as well as their union.
static void BR_CALLBACK ViewBoundsCallback(
    br_actor *     /* actor */,
    br_model *     /* model */,
    br_material *  /* material */,
	void *         /* render_data */,
    br_uint_8      /* style */,
    br_matrix4 *   /* model_to_screen */,
    br_int_32 *bounds)
{
    brwin_dirty_rect *dp;

    // Accumulate total bounding rectangle.
    if (bounds[BR_BOUNDS_MIN_X] < g_curSet->m_renderBounds.min_x)
		g_curSet->m_renderBounds.min_x = bounds[BR_BOUNDS_MIN_X];

    if (bounds[BR_BOUNDS_MIN_Y] < g_curSet->m_renderBounds.min_y)
        g_curSet->m_renderBounds.min_y = bounds[BR_BOUNDS_MIN_Y];

    if (bounds[BR_BOUNDS_MAX_X] > g_curSet->m_renderBounds.max_x)
        g_curSet->m_renderBounds.max_x = bounds[BR_BOUNDS_MAX_X];

    if (bounds[BR_BOUNDS_MAX_Y] > g_curSet->m_renderBounds.max_y)
        g_curSet->m_renderBounds.max_y = bounds[BR_BOUNDS_MAX_Y];

    // If list of rectangles is full, merge current into last
    if (g_curSet->m_lastDirtyRectangle >= MAX_DIRTY_RECTANGLES)
	{
		dp = g_curSet->m_dirtyRectangles + g_curSet->m_lastDirtyRectangle-1;

		if (bounds[BR_BOUNDS_MIN_X] < dp->min_x)
			dp->min_x = bounds[BR_BOUNDS_MIN_X];
		if( bounds[BR_BOUNDS_MIN_Y] < dp->min_y)
			dp->min_y = bounds[BR_BOUNDS_MIN_Y];

		if (bounds[BR_BOUNDS_MAX_X] > dp->max_x)
			dp->max_x = bounds[BR_BOUNDS_MAX_X];

		if (bounds[BR_BOUNDS_MAX_Y] > dp->max_y)
			dp->max_y = bounds[BR_BOUNDS_MAX_Y];

		return;
	}

    // Add this rectangle to list
    dp = g_curSet->m_dirtyRectangles + g_curSet->m_lastDirtyRectangle;
 
    dp->min_x = bounds[BR_BOUNDS_MIN_X];
    dp->min_y = bounds[BR_BOUNDS_MIN_Y];
    dp->max_x = bounds[BR_BOUNDS_MAX_X];
    dp->max_y = bounds[BR_BOUNDS_MAX_Y];
 
    g_curSet->m_lastDirtyRectangle++;
}

#endif /* WIN32 */
