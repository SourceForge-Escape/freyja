/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: Them bones - I think I'm gonna make a big 'o pile of them bones
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.26:
 * Mongoose - Refactored into new plugin event system framework from old 
 *            freyja modeler code base.
 ==========================================================================*/

#include <string.h>
#include <math.h>
#include <hel/math.h>
#include <freyja-0.10/freyja.h> 
#include <freyja-0.10/SkeletonABI.h> 
#include <freyja-0.10/BoneABI.h> 
#include <freyja-0.10/Bone.h> 
#include <freyja-0.10/Skeleton.h> 
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>

using namespace freyja;

index_t gCurrentBone = INDEX_INVALID;
index_t gCurrentSkeleton = INDEX_INVALID;
byte gDontUpdateBoneName = 0;
extern int gSkelTreeWidgetIndex;

#include "FreyjaModel.h" // BUILT_WITH_APP
#include "FreyjaControl.h" // BUILT_WITH_APP
extern FreyjaModel *gFreyjaModel; // BUILT_WITH_APP
extern FreyjaControl *gFreyjaControl; // BUILT_WITH_APP


void SkeletalEventsAttach();
void SkeletalGUIAttach();

void eBoneTranslateX(vec_t x);
void eBoneTranslateY(vec_t y);
void eBoneTranslateZ(vec_t z);
void eBoneRotateX(vec_t x);
void eBoneRotateY(vec_t y);
void eBoneRotateZ(vec_t z);
void eBoneCreationEditorStateTransition();
void eBoneSelectionEditorStateTransition();
void eBoneConnectEditorStateTransition();
void eBoneDisconnectEditorStateTransition();
void eBoneIterator(unsigned int id);
void eBoneSelectByMouse(mgtk_mouse_event_t &e);
void eBoneConnectByMouse(mgtk_mouse_event_t &e);
void eBoneCreateAtPositionByMouse(mgtk_mouse_event_t &e);
void eBoneDisconnectByMouse(mgtk_mouse_event_t &e);
void eSetCurrentBoneName(char *text);
void updateSkeletonUI(index_t skelIndex);
mgtk_tree_t *generateSkeletalUI(uint32 skelIndex, uint32 rootIndex, 
								mgtk_tree_t *tree);

ResourceAppPluginTest Skeletal(SkeletalEventsAttach, SkeletalGUIAttach);


void SkeletalGUIAttach()
{

}


void SkeletalEventsAttach()
{
	ResourceEventCallbackVec::add("eBoneTranslateX", &eBoneTranslateX);
	ResourceEventCallbackVec::add("eBoneTranslateY", &eBoneTranslateY);
	ResourceEventCallbackVec::add("eBoneTranslateZ", &eBoneTranslateZ);
	ResourceEventCallbackVec::add("eBoneRotateX", &eBoneRotateX);
	ResourceEventCallbackVec::add("eBoneRotateY", &eBoneRotateY);
	ResourceEventCallbackVec::add("eBoneRotateZ", &eBoneRotateZ);

	ResourceEventCallback::add("eBoneCreationEditorStateTransition",
								eBoneCreationEditorStateTransition);
	ResourceEventCallback::add("eBoneSelectionEditorStateTransition",
								eBoneSelectionEditorStateTransition);
	ResourceEventCallback::add("eBoneConnectEditorStateTransition",
								eBoneConnectEditorStateTransition);
	ResourceEventCallback::add("eBoneDisconnectEditorStateTransition",
								eBoneDisconnectEditorStateTransition);

	ResourceEventCallbackUInt::add("eBoneIterator", &eBoneIterator);

	ResourceEventCallbackString::add("eSetCurrentBoneName", 
									&eSetCurrentBoneName);

	ResourceEventCallbackMouse::add("eBoneSelectByMouse",
									&eBoneSelectByMouse);
	ResourceEventCallbackMouse::add("eBoneConnectByMouse",
									&eBoneConnectByMouse);
	ResourceEventCallbackMouse::add("eBoneCreateAtPositionByMouse",
									&eBoneCreateAtPositionByMouse);
	ResourceEventCallbackMouse::add("eBoneDisconnectByMouse",
									&eBoneDisconnectByMouse);
}


void eBoneTranslateX(vec_t x)
{
	vec3_t xyz;

	freyjaGetBoneTranslation3fv(gCurrentBone, xyz);
	xyz[0] = x;
	freyjaBoneTranslate3fv(gCurrentBone, xyz);
	mgtk_event_gl_refresh();
}


void eBoneTranslateY(vec_t y)
{
	vec3_t xyz;

	freyjaGetBoneTranslation3fv(gCurrentBone, xyz);

	freyjaPrintMessage("! %i. %f, %f, %f -> %f, %f, %f",
					gCurrentBone, xyz[0], xyz[1], xyz[2], xyz[0], y, xyz[2]);

	xyz[1] = y;
	freyjaBoneTranslate3fv(gCurrentBone, xyz);
	mgtk_event_gl_refresh();

}


void eBoneTranslateZ(vec_t z)
{
	vec3_t xyz;

	freyjaGetBoneTranslation3fv(gCurrentBone, xyz);
	xyz[2] = z;
	freyjaBoneTranslate3fv(gCurrentBone, xyz);
	mgtk_event_gl_refresh();
}


void eBoneRotateX(vec_t x)
{
	vec3_t xyz;

	freyjaGetBoneRotation3fv(gCurrentBone, xyz);
	mgtk_print("! eBoneRotateX> %f %f %f -> %f %f %f",
				xyz[0],xyz[1],xyz[2], x,xyz[1],xyz[2]);
	xyz[0] = HEL_DEG_TO_RAD(x);
	freyjaBoneRotateEuler3fv(gCurrentBone, xyz);
	mgtk_event_gl_refresh();
}


void eBoneRotateY(vec_t y)
{
	vec3_t xyz;

	freyjaGetBoneRotation3fv(gCurrentBone, xyz);
	xyz[1] = HEL_DEG_TO_RAD(y);
	freyjaBoneRotateEuler3fv(gCurrentBone, xyz);
	mgtk_event_gl_refresh();
}


void eBoneRotateZ(vec_t z)
{
	vec3_t xyz;

	freyjaGetBoneRotation3fv(gCurrentBone, xyz);
	xyz[2] = HEL_DEG_TO_RAD(z);
	freyjaBoneRotateEuler3fv(gCurrentBone, xyz);
	mgtk_event_gl_refresh();
}


/* This remaps an event mapping in the editor, therefore it alters 
 * the 'editing mode' */
void eBoneCreationEditorStateTransition()
{
	int id = resourceGetEventId1s("eSelectionEditorStateTransition");

	if (ResourceEvent::listen(id - 10000, eBoneCreateAtPositionByMouse))
	{
		mgtk_print("EditorStateTransition> Create bone at position...");
	}
	else
	{
		mgtk_print("Unable to set EditorStateTransition. %s:%i", __FILE__, __LINE__);
	}
}


void eBoneSelectionEditorStateTransition()
{
	int id = resourceGetEventId1s("eSelectionEditorStateTransition");

	if (ResourceEvent::listen(id - 10000, eBoneSelectByMouse))
	{
		//mTransformMode = FreyjaModel::TransformBone;
		//mEventMode = modeSelect;
		mgtk_print("EditorStateTransition> Select bone...");
	}
	else
	{
		mgtk_print("Unable to set EditorStateTransition. %s:%i", __FILE__, __LINE__);
	}
}


void eBoneConnectEditorStateTransition()
{
	int id = resourceGetEventId1s("eSelectionEditorStateTransition");

	if (ResourceEvent::listen(id - 10000, eBoneConnectByMouse))
	{
		mgtk_print("Select a bone to connect to current bone...");
		//mTransformMode = FreyjaModel::TransformBone;
		//mEventMode = BONE_CONNECT_MODE;
	}
	else
	{
		mgtk_print("Unable to set EditorStateTransition. %s:%i", __FILE__, __LINE__);
	}
}


void eBoneDisconnectEditorStateTransition()
{
	int id = resourceGetEventId1s("eSelectionEditorStateTransition");

	if (ResourceEvent::listen(id - 10000, eBoneDisconnectByMouse))
	{
		mgtk_print("Select bone to break from current");
		//mTransformMode = FreyjaModel::TransformBone;
		//mEventMode = BONE_DISCONNECT_MODE;
	}
	else
	{
		mgtk_print("Unable to set EditorStateTransition. %s:%i", __FILE__, __LINE__);
	}
}


void eBoneSelectByMouse(mgtk_mouse_event_t &e)
{	
	vec_t x = e.x, y = e.y, z;

	gFreyjaControl->getWorldFromScreen(&x, &y, &z);
	gFreyjaModel->selectBone(x, y); // needs old plane projection picker
	gCurrentBone = gFreyjaModel->getCurrentBone();
}


void eBoneConnectByMouse(mgtk_mouse_event_t &e)
{	
	index_t parent = gCurrentBone;
	vec_t x = e.x, y = e.y, z;

	gFreyjaControl->getWorldFromScreen(&x, &y, &z);
	gFreyjaModel->selectBone(x, y); // needs old plane projection picker
	freyjaBoneAddChild(parent, gFreyjaModel->getCurrentBone());
	gFreyjaModel->setCurrentBone(parent);
	updateSkeletonUI(gCurrentSkeleton);
}


void eBoneCreateAtPositionByMouse(mgtk_mouse_event_t &e)
{
	index_t bone;
	vec_t x = e.x, y = e.y, z;

	gFreyjaControl->getWorldFromScreen(&x, &y, &z);
	bone = freyjaBoneCreate(gCurrentSkeleton);
	freyjaBoneTranslate3f(bone, x, y, z);

	mgtk_print("New bone[%i] created.", bone);
	updateSkeletonUI(gCurrentSkeleton);
}


void eBoneDisconnectByMouse(mgtk_mouse_event_t &e)
{	
	index_t parent = gCurrentBone;
	vec_t x = e.x, y = e.y, z;

	gFreyjaControl->getWorldFromScreen(&x, &y, &z);
	gFreyjaModel->selectBone(x, y); // needs old plane projection picker
	freyjaBoneRemoveChild(parent, gFreyjaModel->getCurrentBone());
	gFreyjaModel->setCurrentBone(parent);
	updateSkeletonUI(gCurrentSkeleton);
}


void eBoneIterator(unsigned int id)
{
	char dupname[64];
	vec3_t xyz;
	int e = resourceGetEventId1s("eBoneIterator");


	/* Limits widget's valid range of values based on backend allocation
	 * while at the same time checks for validity */
	if (!freyja_event_set_range(e, id, 0, Bone::getCount()))
	{
		gFreyjaModel->setCurrentBone(id);

		if (id == gFreyjaModel->getCurrentBone() &&
			freyjaIsBoneAllocated(id))
		{
			gCurrentBone = id;
			mgtk_print("Selected Bone[%i] for editing.", id);

			/* Mongoose 2002.08.31,
			 * Update spin buttons dependent on this iterator */
			freyjaGetBoneRotation3fv(id, xyz);
			e = resourceGetEventId1s("eBoneRotateX"); // cache these to gobals or statics for a speed boost
			mgtk_spinbutton_value_set(e, HEL_RAD_TO_DEG(xyz[0]));
			e = resourceGetEventId1s("eBoneRotateY");
			mgtk_spinbutton_value_set(e, HEL_RAD_TO_DEG(xyz[1]));
			e = resourceGetEventId1s("eBoneRotateZ");
			mgtk_spinbutton_value_set(e, HEL_RAD_TO_DEG(xyz[2]));
				
			freyjaGetBoneTranslation3fv(id, xyz);
			e = resourceGetEventId1s("eBoneTranslateX");
			mgtk_spinbutton_value_set(e, xyz[0]);
			e = resourceGetEventId1s("eBoneTranslateY");
			mgtk_spinbutton_value_set(e, xyz[1]);
			e = resourceGetEventId1s("eBoneTranslateZ");
			mgtk_spinbutton_value_set(e, xyz[2]);

			/* Update any bone name listeners, 
			 * 1. Dup string to avoid evil widgets that want to mutate it
			 * 2. Diable event hook up in case of event loop */
			strncpy(dupname, freyjaGetBoneName1s(id), 64);
			gDontUpdateBoneName = 1;
			e = resourceGetEventId1s("eSetCurrentBoneName");
			mgtk_textentry_value_set(e, dupname);
			gDontUpdateBoneName = 0;

			mgtk_event_gl_refresh();
		}
	}
}


void eSetCurrentBoneName(char *text)
{
	if (!gDontUpdateBoneName)
	{
		freyjaBoneName(gFreyjaModel->getCurrentBone(), text);
		updateSkeletonUI(gCurrentSkeleton);
	}
}


void updateSkeletonUI(index_t skelIndex)
{
	mgtk_tree_t *tree;

	tree = generateSkeletalUI(skelIndex, 
							  freyjaGetSkeletonRootIndex(skelIndex), 0x0);

	mgtk_event_update_tree(gSkelTreeWidgetIndex, tree);
}


mgtk_tree_t *generateSkeletalUI(uint32 skelIndex, uint32 rootIndex, 
								mgtk_tree_t *tree)
{
	uint32 i, boneChild, count;


	if (!freyjaIsBoneAllocated(rootIndex))
	{
		freyja_print("!generateSkeletalUI> ERROR: NULL skeletal bone!\n");
		return 0x0;
	}

	uint32 rootChildCount = freyjaGetBoneChildCount(rootIndex);
	const char *rootName = freyjaGetBoneName1s(rootIndex);
	uint32 rootSkelBID = freyjaGetSkeletonBoneIndex(skelIndex, rootIndex);

	if (tree == 0x0)
	{
		tree = new mgtk_tree_t;
		snprintf(tree->label, 64, "root");	
		tree->parent = 0x0;
	}
	else
	{
		snprintf(tree->label, 64, "bone%03i", rootSkelBID);
	}

	if (rootName[0])
	{
		snprintf(tree->label, 64, "%s", rootName);
	}

	tree->id = rootIndex;
	tree->numChildren = rootChildCount;
	tree->children = 0x0;

#ifdef DEBUG_BONE_LOAD
	printf("-- %s : %i/%i children\n",  
		   tree->label, tree->numChildren, rootChildCount);
#endif

	if (tree->numChildren == 0)
		return tree->parent;

	tree->children = new mgtk_tree_t[tree->numChildren+1];

	for (count = 0, i = 0; i < rootChildCount; ++i)
	{
		boneChild = freyjaGetBoneChild(rootIndex, i);

		if (freyjaIsBoneAllocated(boneChild))
		{
			tree->children[count].parent = tree;
			generateSkeletalUI(skelIndex, boneChild, &tree->children[count++]);
		}
	}

	return (tree->parent) ? tree->parent : tree;
}






