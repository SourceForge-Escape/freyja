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

#include <mgtk/mgtk_tree.h>
#include <mgtk/MGtkLua.h>

#include "FreyjaControl.h" // Only used for legacy EvBoneIteratorId use.


void freyja3d_scenegraph_update_skeleton_helper(mgtk_tree_t* subroot, index_t id)
{
	if (!subroot)
		return;

	for (uint32 i = 0, n = freyjaGetBoneChildCount(id); i < n; ++i)
	{
		index_t cid = freyjaGetBoneChild(id, i);
		mgtk_tree_t* child = mgtk_tree_add_new_child( subroot, freyjaGetBoneNameString(cid), cid );
		freyja3d_scenegraph_update_skeleton_helper(child, cid);
	}	
}


void freyja3d_scenegraph_update_metadata(mgtk_tree_t* scene)
{
	mgtk_tree_t* subroot = mgtk_tree_add_new_child( scene, "Metadata", 0 );
	
	for (uint32 i = 0, n = Metadata::GetObjectCount(); i < n; ++i)
	{
		Metadata* metadata = Metadata::GetObjectByUid( i );
			
		// This method will encounter any 'gaps' ( NULL pointers ) in the container.
		if ( metadata )
		{
			mgtk_tree_t* node = mgtk_tree_add_new_child( subroot, metadata->GetName(), i );
			mgtk_tree_add_new_child( node, metadata->GetType(), 0 );
			mgtk_tree_add_new_child( node, metadata->GetModel(), 0 );
			mgtk_tree_add_new_child( node, metadata->GetMaterial(), 0 );
		}
	}
}


void freyja3d_scenegraph_update_model(mgtk_tree_t* scene)
{
	mgtk_tree_t* model = mgtk_tree_add_new_child( scene, "Model", 0 );
	mgtk_tree_t* meshes = mgtk_tree_add_new_child( model, "Meshes", 0 );

	for (uint32 i = 0, n = freyjaGetMeshCount(); i < n; ++i)
	{
		if ( freyjaIsMeshAllocated(i) )
		{
			mgtk_tree_add_new_child( meshes, freyjaGetMeshNameString(i), i );
		}
	}
}


void freyja3d_scenegraph_update_material(mgtk_tree_t* scene)
{
	mgtk_tree_t* material = mgtk_tree_add_new_child( scene, "Material", 0 );

	for (uint32 i = 0, n = freyjaGetMaterialCount(); i < n; ++i)
	{
		mgtk_tree_add_new_child( material, freyjaGetMaterialName(i), i );
	}
}


void freyja3d_scenegraph_update_skeleton(mgtk_tree_t* scene)
{
	mgtk_tree_t* skeleton = mgtk_tree_add_new_child( scene, "Skeleton", 0 );
	
	for (uint32 i = 0, n = freyjaGetBoneCount(); i < n; ++i)
	{
		if ( freyjaIsBoneAllocated(i) )
		{
			if ( freyjaGetBoneParent(i) == INDEX_INVALID )
			{
				// This is either a root or subroot at the top level.
				mgtk_tree_t* root = mgtk_tree_add_new_child( skeleton, freyjaGetBoneNameString(i), i );
				freyja3d_scenegraph_update_skeleton_helper(root, i);
			}
		}
	}
}


void freyja3d_scenegraph_update()
{
	/* Generate treeview model. */
	mgtk_tree_t* scene = mgtk_tree_new("Scene", -1); // -1 is treemodel root.

	/* Metadata. */
	freyja3d_scenegraph_update_metadata( scene );

	/* Materials. */
	freyja3d_scenegraph_update_material( scene );

	/* Model. */
	freyja3d_scenegraph_update_model( scene );

	/* Skeleton. */
	freyja3d_scenegraph_update_skeleton( scene );

	/* Update treeview widget(s). */
	mgtk_tree_update_widget( "Scene", freyja3d::FreyjaControl::EvBoneIteratorId, scene ); // FIXME: EvSceneGraphId

	/* Free tree model. */
	mgtk_tree_delete( scene );
}


void freyja3d_scenegraph_init()
{
	freyja3d_scenegraph_update();
}

