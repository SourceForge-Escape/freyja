/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : GPL, Copyright 2001-2007 Terry 'Mongoose' Hendrix II
 * Comments: This is the scenegraph widget.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.11.14:
 * Mongoose - Created
 ==========================================================================*/

#include "config.h"

#include <string.h>

#include <freyja/freyja.h>
#include <freyja/BoneABI.h>
#include <freyja/LightABI.h>
#include <freyja/LuaABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/MeshABI.h>
#include <freyja/Metadata.h>

#include <mgtk/mgtk_lua.h>
#include <mgtk/mgtk_timeslider.h>
#include <mgtk/mgtk_tree.h>

#include "freyja_events.h"


void freyja3d_scenegraph_update_skeleton_helper(mgtk_tree_t* subroot, int event, index_t id)
{
	if (!subroot)
		return;

	for (uint32 i = 0, n = freyjaGetBoneChildCount(id); i < n; ++i)
	{
		index_t cid = freyjaGetBoneChild(id, i);
		mgtk_tree_t* child = mgtk_tree_add_new_child( subroot, freyjaGetBoneNameString(cid), event, cid );
		freyja3d_scenegraph_update_skeleton_helper(child, event, cid);
	}	
}


void freyja3d_scenegraph_update_metadata(mgtk_tree_t* scene)
{
	mgtk_tree_t* subroot = mgtk_tree_add_new_child( scene, "Metadata", -1, 0 );
	const int event = freyja3d_get_event_id( "eMetadataIterator" );

	for (uint32 i = 0, n = Metadata::GetObjectCount(); i < n; ++i)
	{
		Metadata* metadata = Metadata::GetObjectByUid( i );
			
		// This method will encounter any 'gaps' ( NULL pointers ) in the container.
		if ( metadata )
		{
			mgtk_tree_t* node = mgtk_tree_add_new_child( subroot, metadata->GetName(), event, i );
			mgtk_tree_add_new_child( node, metadata->GetType(), -1, 0 );
			mgtk_tree_add_new_child( node, metadata->GetModel(), -1, 0 );
			mgtk_tree_add_new_child( node, metadata->GetMaterial(), -1, 0 );
		}
	}
}


void freyja3d_scenegraph_update_model(mgtk_tree_t* scene)
{
	mgtk_tree_t* model = mgtk_tree_add_new_child( scene, "Model", -1, 0 );
	mgtk_tree_t* meshes = mgtk_tree_add_new_child( model, "Meshes", -1, 0 );
	const int event = freyja3d_get_event_id( "eMeshIterator" );

	for (uint32 i = 0, n = freyjaGetMeshCount(); i < n; ++i)
	{
		if ( freyjaIsMeshAllocated(i) )
		{
			mgtk_tree_add_new_child( meshes, freyjaGetMeshNameString(i), event, i );
		}
	}
}


void freyja3d_scenegraph_update_material(mgtk_tree_t* scene)
{
	mgtk_tree_t* material = mgtk_tree_add_new_child( scene, "Material", -1, 0 );
	const int event = freyja3d_get_event_id( "eMaterialIterator" );

	for (uint32 i = 0, n = freyjaGetMaterialCount(); i < n; ++i)
	{
		mgtk_tree_add_new_child( material, freyjaGetMaterialName(i), event, i );
	}
}


void freyja3d_scenegraph_update_skeleton(mgtk_tree_t* scene)
{
	mgtk_tree_t* skeleton = mgtk_tree_add_new_child( scene, "Skeleton", -1, 0 );
	const int event = freyja3d_get_event_id( "eBoneIterator" );

	for (uint32 i = 0, n = freyjaGetBoneCount(); i < n; ++i)
	{
		if ( freyjaIsBoneAllocated(i) )
		{
			if ( freyjaGetBoneParent(i) == INDEX_INVALID )
			{
				// This is either a root or subroot at the top level.
				mgtk_tree_t* root = mgtk_tree_add_new_child( skeleton, freyjaGetBoneNameString(i), event, i );
				freyja3d_scenegraph_update_skeleton_helper(root, event, i);
			}
		}
	}
}


void freyja3d_scenegraph_update_camera(mgtk_tree_t* scene)
{
	mgtk_tree_t* camera = mgtk_tree_add_new_child( scene, "Cameras", -1, 0 );
	const int event = freyja3d_get_event_id( "eCameraIterator" );
	mstl::String s;

	for (uint32 i = 0, n = freyjaGetLightCount(); i < n; ++i)
	{
		s.Set("camera%i", i);
		mgtk_tree_add_new_child( camera, s.c_str(), event, i );
	}
}


void freyja3d_scenegraph_update_light(mgtk_tree_t* scene)
{
	mgtk_tree_t* light = mgtk_tree_add_new_child( scene, "Lights", -1, 0 );
	const int event = freyja3d_get_event_id( "eLightIterator" );
	mstl::String s;

	for (uint32 i = 0, n = freyjaGetLightCount(); i < n; ++i)
	{
		s.Set("light%i", i);
		mgtk_tree_add_new_child( light, s.c_str(), event, i );
	}
}


void freyja3d_scenegraph_update()
{
	/* Generate treeview model. */
	mgtk_tree_t* scene = mgtk_tree_new("Scene", -1, -1); // id = -1 is treemodel root.

	/* Cameras. */
	freyja3d_scenegraph_update_camera( scene );

	/* Lights. */
	freyja3d_scenegraph_update_light( scene );

	/* Metadata. */
	freyja3d_scenegraph_update_metadata( scene );

	/* Materials. */
	freyja3d_scenegraph_update_material( scene );

	/* Model. */
	freyja3d_scenegraph_update_model( scene );

	/* Skeleton. */
	freyja3d_scenegraph_update_skeleton( scene );

	/* Update treeview widget(s) with a default event. */
	int event = freyja3d_get_event_id( "eBoneIterator" );
	mgtk_tree_update_widget( "Scene", event, scene );

	/* Free tree model. */
	mgtk_tree_delete( scene );

	
	/* FIXME: Hack to update custom time slider after import for testing. */
	{
		int event = freyja3d_get_event_id( "eAnimationSlider" );
		uint32 track_id = 0;//FreyjaControl::GetInstance()->GetSelectedAnimation();

		mgtk_time_slider_reset_markers( event );

		unsigned int end = 150;

		for ( uint32 bone_id = 0, bone_count = freyjaGetBoneCount(); bone_id < bone_count; ++bone_id )
		{
			/* Gather up a list of keyframe times. */
			Bone* bone = Bone::GetBone( bone_id );
			BoneTrack& track = bone->GetTrack( track_id );

			if ( track.GetRotKeyframeCount() > end )
				end = track.GetRotKeyframeCount();

			if ( track.GetLocKeyframeCount() > end )
				end = track.GetLocKeyframeCount();
		}

		mgtk_time_slider_set_range( event, 0, end );
		//mgtk_time_slider_set_range( event, 100, 200 );

		for ( uint32 bone_id = 0, bone_count = freyjaGetBoneCount(); bone_id < bone_count; ++bone_id )
		{
			/* Gather up a list of keyframe times. */
			Bone* bone = Bone::GetBone( bone_id );
			BoneTrack& track = bone->GetTrack( track_id );
			vec_t rate_inverse = 1.0f / track.GetRate();

			for ( uint32 i = 0; i < track.GetRotKeyframeCount(); ++i )
			{
				//vec_t time = track.GetRotKeyframeTime( i );
				//vec_t time = (vec_t)i * rate_inverse;
				mgtk_time_slider_add_marker( event, i );
			}

			for ( uint32 i = 0; i < track.GetLocKeyframeCount(); ++i )
			{
				//vec_t time = track.GetLocKeyframeTime( i );
				//vec_t time = (vec_t)i * rate_inverse;
				mgtk_time_slider_add_marker( event, i );
			}
		}
	}
}


void freyja3d_scenegraph_init()
{
	freyja3d_scenegraph_update();
}


void freyja3d_scenegraph_update_metadata()
{
	// FIXME: For now we just wrap due to current gtk+ binding implementation limits.
	//        Later optimize this to only update metadata branch.
	freyja3d_scenegraph_update();
}
