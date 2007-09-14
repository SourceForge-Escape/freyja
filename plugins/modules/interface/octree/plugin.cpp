/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : perlinnoise plugin
 * License : No use without permission (c) 2000-2005 Mongoose
 * Comments: This is the backend of the modeler
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.23:
 * Mongoose - Created, Based on freyja prototype
 ==========================================================================*/

#include <string.h>
#include <math.h>
#include <hel/math.h>
#include <mstl/SystemIO.h>
#include <freyja/PerlinNoise.h>
#include <freyja/FreyjaImage.h>
#include <freyja/PluginABI.h>
#include <freyja/TextureABI.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/QueryDialog.h>
#include <mgtk/mgtk_events.h>

#include "Octree.h"
#include "OctreeHelper.h"
#include "FreyjaOctreeHelper.h"

using namespace mstl;
using namespace mgtk;

Octree gOctree;
bool gOctreeVisible = false;

extern "C" {

	void freyja_octree_init(void (*func)(const char*, void*));

	void freyja_octree_event_attach();

	void freyja_octree_ui_attach();

	void freyja_octree_draw();
}


void freyja_octree_init(void (*func)(const char*, void*))
{
	ResourcePlugin *plugin = 
	new ResourcePlugin(freyja_octree_event_attach, freyja_octree_ui_attach);	

	plugin->mDrawCB = freyja_octree_draw;
}


void freyja_octree_clear()
{
	gOctree.Reset();
}


template<>
Octree::Node* mstl::list<Octree::Node*>::mDefault = NULL; 

void freyja_octree_rebuild()
{
	index_t meshIndex = 0;
	unsigned int count = 16;

	// Generated query dialog
	{
		QueryDialog d;
		d.mName = "Octree plugin"; 
		d.mDialogIcon = "gtk-question"; 
		d.mInformationMessage.Set("Octree plugin settings"); 
		d.mCancelIcon = "gtk-cancel"; 
		d.mCancelText = "Cancel"; 
		d.mAcceptIcon = "gtk-ok"; 
		d.mAcceptText = "Build"; 

		{
			QueryDialogValue<int> v("meshid", "Mesh Index:", 0);
			d.mInts.push_back(v);
		}
		
		{
			QueryDialogValue<int> v("count", "Faces per node:", 16);
			d.mInts.push_back(v);
		}

		if ( d.Execute() )
		{
			meshIndex = d.GetInt("meshid");
			count = d.GetInt("count");
		}
		else
		{
			return;  // The user wished to cancel the entire action.
		}
	}

	// Octree processing
	freyja::Mesh *mesh = freyja::Mesh::GetMesh( meshIndex ); 

	gOctree.Reset();

	if (mesh)
	{
		mgtk_print("Octree generation can take some time...");
		mesh->UpdateBoundingVolume();

		FreyjaOctreeHelper helper;
		helper.SetMesh( mesh );
		helper.SetMaxCount( count ); 
		gOctree.Generate( helper );

#if 1
		// DEBUG: Show all marked faces...
		mstl::list<Octree::Node*> queue;
		queue.push_back( &gOctree.GetRoot() );

		unsigned int count = 0;

		while ( queue.front() )
		{
			Octree::Node* node = queue.front();
			queue.pop_front();

			for (uint32 i = 0, n = node->mChildren.size(); i < n; ++i)
			{
				if ( node->mChildren[i] )
					queue.push_back( node->mChildren[i] );
			}

			for (uint32 i = 0, n = node->mFaces.size(); i < n; ++i)
			{
				mesh->SetFaceFlags( node->mFaces[i], freyja::Face::fSelected );
			}

			count += node->mFaces.size();
		}

		MGTK_ASSERTMSG(count == mesh->GetFaceCount(), "Some faces not mapped to octree!"); 

#endif

		gOctreeVisible = true;
		mgtk_print("Octree generated.");

#if 0
		// Build walkmesh.
		// Note that if the mesh isn't 'clean' this will still work, but
		// waste memory with unused 'gaps' in the array.
		{
			mstl::Vector<vec_t>& vertices = gOctree.GetVertices();
			vertices.clear();
			vec_t* array = mesh->GetVertexArray(); 
			uint32 count = mesh->GetVertexArrayCount();

			if ( !array )
				count = 0;

			if ( count )
				vertices.resize(count+1);

			for (uint32 i = 0; i < count; ++i)
			{
				vertices.push_back( array[i] );
			}
		}

		{
			mstl::Vector<uint16>& faces = gOctree.GetFaces(); 
			uint32 count = mesh->GetFaceCount();

			if ( count )
				faces.resize(count+1);

			for (uint32 i = 0; i < count; ++i)
			{
				Face* face = mesh->GetFace(i);

				if ( face )
				{
					uint32 j = 0;
					foreach ( face->mIndices, j )
					{
						faces.push_back( face->mIndices[j] );
					}
				}
			}
		}
#endif
	}
	else
	{
		gOctreeVisible = false;
		mgtk_print("Octree found no mesh to process.");
	}
}


void freyja_octree_visibility()
{
	gOctreeVisible = !gOctreeVisible;
}


void freyja_octree_import()
{
	char *path = mgtk_rc_map("/");
	char *filename =
	mgtk_filechooser_blocking("freyja - Import Octree...", path, 0,
							  "Octree object (*.xml)", "*.xml");

	if (path) 
	{
		delete [] path;
	}

	if ( filename )
	{
		mgtk_print("! Importing: '%s'\n", filename);
		gOctree.Unserialize( filename );
	}

	mgtk_filechooser_blocking_free(filename);
}


void freyja_octree_export()
{
	char *path = mgtk_rc_map("/");
	char *filename =
	mgtk_filechooser_blocking("freyja - Export Octree...", path, 1,
							  "Octree object (*.xml)", "*.xml");

	if (path) 
	{
		delete [] path;
	}

	if ( filename )
	{
		mgtk_print("! Exporting: '%s'\n", filename);
		gOctree.Serialize( filename );
	}

	mgtk_filechooser_blocking_free(filename);
}


void freyja_octree_nop()
{
}


void freyja_octree_event_attach()
{
	ResourceEventCallback::add("eOctreeMenu", &freyja_octree_nop);
	ResourceEventCallback::add("eOctreeRebuild", &freyja_octree_rebuild);
	ResourceEventCallback::add("eOctreeVisibility", &freyja_octree_visibility);
	ResourceEventCallback::add("eOctreeImport", &freyja_octree_import);
	ResourceEventCallback::add("eOctreeExport", &freyja_octree_export);
}


void freyja_octree_ui_attach()
{
	// Setup octree plugin menu.
	int mainMenuId = Resource::mInstance->getIntByName("ePluginMenu");
	int menuId = Resource::mInstance->getIntByName("eOctreeMenu");
	mgtk_append_menu_to_menu(mainMenuId, "Octree", menuId);

	// Append menu items.
	{
		int id = Resource::mInstance->getIntByName("eOctreeRebuild");
		mgtk_append_item_to_menu(menuId, "Rebuild", id);
	}

	{
		int id = Resource::mInstance->getIntByName("eOctreeVisibility");
		mgtk_append_item_to_menu(menuId, "Show/Hide", id);
	}

	{
		int id = Resource::mInstance->getIntByName("eOctreeImport");
		mgtk_append_item_to_menu(menuId, "Open...", id);
	}

	{
		int id = Resource::mInstance->getIntByName("eOctreeExport");
		mgtk_append_item_to_menu(menuId, "Save As...", id);
	}
}


void freyja_octree_draw_node(Octree::Node &node)
{
	for (int32 i = 0, n = node.mChildren.size(); i < n; ++i)
	{
		if ( node.mChildren[i] )
		{
			freyja_octree_draw_node( *(node.mChildren[i]) );
		}
	}

	hel::Vec3 &min = node.mMin, &max = node.mMax;

	mgtk_draw_line6f(max.mX, max.mY, max.mZ,
					 max.mX, min.mY, max.mZ);
	mgtk_draw_line6f(max.mX, max.mY, max.mZ,
					 min.mX, max.mY, max.mZ);
	mgtk_draw_line6f(max.mX, max.mY, max.mZ,
					 max.mX, max.mY, min.mZ);

	mgtk_draw_line6f(min.mX, max.mY, max.mZ,
					 min.mX, max.mY, min.mZ);
	mgtk_draw_line6f(max.mX, min.mY, max.mZ,
					 max.mX, min.mY, min.mZ);
	mgtk_draw_line6f(max.mX, min.mY, max.mZ,
					 min.mX, min.mY, max.mZ);

	mgtk_draw_line6f(max.mX, max.mY, min.mZ,
					 max.mX, min.mY, min.mZ);
	mgtk_draw_line6f(max.mX, max.mY, min.mZ,
					 min.mX, max.mY, min.mZ);
	mgtk_draw_line6f(min.mX, max.mY, max.mZ,
					 min.mX, min.mY, max.mZ);

	mgtk_draw_line6f(min.mX, min.mY, min.mZ,
					 min.mX, max.mY, min.mZ);
	mgtk_draw_line6f(min.mX, min.mY, min.mZ,
					 max.mX, min.mY, min.mZ);
	mgtk_draw_line6f(min.mX, min.mY, min.mZ,
					 min.mX, min.mY, max.mZ);
}


void freyja_octree_draw_faux_control_points(Octree::Node &node)
{
	for (int32 i = 0, n = node.mChildren.size(); i < n; ++i)
	{
		if ( node.mChildren[i] )
		{
			freyja_octree_draw_faux_control_points( *(node.mChildren[i]) );
		}
	}

	hel::Vec3 center = (node.mMin + node.mMax) * 0.5f;
	mgtk_draw_point3f(center.mX, center.mY, center.mZ);
}


void freyja_octree_draw()
{
	// FIXME: You need some kind of token or setting to control this draw.
	if (!gOctreeVisible)
		return;

	// Draw octree nodes
	Octree::Node &root = gOctree.GetRoot();

	// Yellow Pen
	mgtk_draw_color3f(1.0f, 1.0f, 0.0f);
	freyja_octree_draw_node(root);

	// Red Pen
	mgtk_draw_point_size1f(5.0f);
	mgtk_draw_color3f(1.0f, 0.0f, 0.0f);
	freyja_octree_draw_faux_control_points(root);
}




