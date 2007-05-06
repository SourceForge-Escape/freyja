/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/mongoose/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: This is the common event system interface
 *           for freyja
 *
 *           It's independent of the widget set, etc
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.04.01:
 * Mongoose - All platforms use registered callbacks now, removed old code
 *
 * 2001.06.26:
 * Mongoose - Created
 ==========================================================================*/

#define FREYJA_APP_PLUGINS 1
#define DISABLED_EVENT 0

#include <string.h>

#define USING_FREYJA_CPP_ABI
#include <freyja/LegacyABI.h>  // FIXME: Still using legacy polymesh generator
#include <freyja/MeshABI.h>
#include <freyja/TextureABI.h>
#include <freyja/PluginABI.h>
#include <freyja/Plugin.h>
#include <freyja/LightABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/FreyjaImage.h>
#include <freyja/freyja.h>

#include <mstl/SystemIO.h>

#include "FreyjaRender.h"
#include "FreyjaControl.h"

#include <mgtk/mgtk_events.h>
#include <mgtk/mgtk_linker.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/ConfirmationDialog.h>

#include "freyja_events.h"

arg_list_t *freyja_rc_color(arg_list_t *args);

long PLUGIN_EVENT_COUNTER = ePluginEventBase;

using namespace freyja3d;


long freyja_get_new_plugin_eventid()
{
	return PLUGIN_EVENT_COUNTER++;
}


// FIXME: Rewrite this to be Skeleton based!!  Allow for bones outside of root
//        and include Skeleton in tree
//
//        Also have tree root be skeleton name, so you can do multiple skeletons
//        in the widget if needed later ala scene graphs
mgtk_tree_t *generateSkeletalUI(uint32 skelIndex, uint32 rootIndex, 
								mgtk_tree_t *tree)
{
	if (!freyjaIsBoneAllocated(rootIndex))
	{
		freyja_print("!generateSkeletalUI(): No Skeleton root given.\n");
		return 0x0;
	}

	uint32 rootChildCount = freyjaGetBoneChildCount(rootIndex);
	const char *rootName = freyjaGetBoneNameString(rootIndex);
	uint32 rootSkelBID = rootIndex;//freyjaGetBoneSkeletalBoneIndex(rootIndex);

	if (rootChildCount > freyjaGetBoneCount())
	{
		freyja_print("!generateSkeletalUI(): Invalid boneIndex.\n");
		return 0x0;
	}

	if (tree == 0x0)
	{
		tree = new mgtk_tree_t;
		snprintf(tree->label, 63, "root");	
		tree->label[63] = '\0';
		tree->parent = 0x0;
	}
	else
	{
		snprintf(tree->label, 63, "bone%03i", rootSkelBID);
		tree->label[63] = '\0';
	}

	if (rootName[0])
	{
		snprintf(tree->label, 63, "%s", rootName);
		tree->label[63] = '\0';
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

	for (uint32 count = 0, i = 0; i < rootChildCount; ++i)
	{
		uint32 boneChild = freyjaGetBoneChild(rootIndex, i);

		if (freyjaIsBoneAllocated(boneChild))
		{
			tree->children[count].parent = tree;
			generateSkeletalUI(skelIndex, boneChild, &tree->children[count++]);
		}
	}

	return (tree->parent) ? tree->parent : tree;
}


void UpdateSkeletonUI_Callback(uint32 skelIndex)
{
	mgtk_textentry_value_set(ResourceEvent::GetResourceIdBySymbol("eSkeletonName"),
							 freyjaGetSkeletonName(skelIndex));

	mgtk_tree_t *tree = NULL;

	if (freyjaIsBoneAllocated(freyjaGetSkeletonRootIndex(skelIndex)))
	{
		tree = generateSkeletalUI(skelIndex, 
								  freyjaGetSkeletonRootIndex(skelIndex), 0x0);
	}

	mgtk_event_update_tree(FreyjaControl::EvBoneIteratorId, tree);
}


// Get the basename of the full path name and strip ext '.so', '.dll', etc
void freyja_init_get_basename(const char *filename, char *basename, uint32 size)
{
	uint32 i, j, l = strlen(filename);
	char c;


	for (i = 0, j = 0; i < l; ++i)
	{
		c = filename[i];

		switch (c)
		{
		case '\\':
		case '/':
			j = 0;
			basename[0] = 0;
			break;
		
		default:
			if (j < size-1)
			{
				basename[j++] = c;
				basename[j] = 0;
			}
		}
	}

	for (; i > 0; --i)
	{
		c = basename[i];

		if (c == '.')
			break;
	}

	if (i > 0)
		basename[i] = 0;

	//freyjaPrintMessage("! %s", basename);
}


void freyja_plugin_generic(const char *symbol, void *something)
{
#if 0
	// NOTICE I don't actually let plugins query the Resource for safety ;)

	// 1. look for symbol in the 'hack bind list'
	// 2. return what you found via the void pointer

	if (strncmp(symbol, "freyja_load_texture_buffer", 25) == 0)
	{
		something = (void *)freyja_load_texture_buffer;
	}
#endif
	something = 0x0; // Stop the madness
}


void freyja_init_application_plugins(const char *dir)
{
#if FREYJA_APP_PLUGINS
	SystemIO::FileReader reader;
	char module_name[128];
	char module_symbol[128];
	void (*init)(void (*func)(const char*, void*));
	bool done = false;
	const char *module_filename;
	void *handle;


	freyja_print("![Freyja application plugin system invoked]");

	if (!reader.OpenDir(dir))
	{
		freyja_print("!Couldn't access application plugin directory.");
		return;
	}

	while (!done && (module_filename = reader.GetNextDirectoryListing()))
	{
		if (reader.IsDirectory(module_filename))
			continue;

		if (!SystemIO::CheckModuleExt(module_filename))
			continue;

		if (!(handle = freyjaModuleLoad(module_filename)))
		{
			continue; /* Try the next plugin, even after a bad module load */
		}
		else
		{
			freyja_init_get_basename(module_filename, module_name, 128);
			snprintf(module_symbol, 128, "freyja_%s_init", module_name);

			freyja_print("!Module '%s' opened.", module_filename);

			init = (void (*)(void (*)(const char*, void*)))freyjaModuleImportFunction(handle, module_symbol);

			if (init == NULL)  
			{
				freyjaModuleUnload(handle);
				continue;
			}

			/* Call plugin's init function */
	      	(*init)(freyja_plugin_generic);

			freyja_print("!Module '%s' linked.", module_filename);

			// Keep these plugins in memory...
			//freyjaModuleUnload(handle);
		}
	}

	reader.CloseDir();

	freyja_print("![Freyja application plugin loader sleeps now]\n");

#else
	freyja_print("FreyjaAppPlugin: This build was compiled w/o plugin support");
#endif
}


void freyja_handle_application_window_close()
{
	FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	// There is some bug with some Gtk+ builds that allow this function
	// to be called again before this exits.  
	// Also relates to the Cancel doesn't cancel bug.
	FreyjaControl::mInstance->Shutdown();
}


void freyja_handle_color(int id, float r, float g, float b, float a)
{
	vec4_t color = { r, g, b, a };

	/* Color event listener */
	if (ResourceEvent::listen(id - ePluginEventBase, color, 4))
		return;

	switch (id)
	{
	case eColorMaterialAmbient: 
		freyjaMaterialAmbient(freyjaGetCurrentMaterial(), color);
		MaterialEv::mHack->RefreshInterface();
		break;

	case eColorMaterialDiffuse:
		freyjaMaterialDiffuse(freyjaGetCurrentMaterial(), color);
		MaterialEv::mHack->RefreshInterface();
		break;

	case eColorMaterialSpecular:
		freyjaMaterialSpecular(freyjaGetCurrentMaterial(), color);
		MaterialEv::mHack->RefreshInterface();
		break;

	case eColorMaterialEmissive:
		freyjaMaterialEmissive(freyjaGetCurrentMaterial(), color);
		MaterialEv::mHack->RefreshInterface();
		break;

	case eColorLightAmbient: 
		freyjaLightAmbient(freyjaGetCurrentLight(), color);
		freyja_event_set_color(eColorLightAmbient, r, g, b, a);
		break;

	case eColorLightDiffuse:
		freyjaLightDiffuse(freyjaGetCurrentLight(), color);
		freyja_event_set_color(eColorLightDiffuse, r, g, b, a);
		break;

	case eColorLightSpecular:
		freyjaLightSpecular(freyjaGetCurrentLight(), color);
		freyja_event_set_color(eColorLightSpecular, r, g, b, a);
		break;

	case eColorBackground:
		memcpy(FreyjaRender::mColorBackground, color, sizeof(vec4_t));
		freyja_event_set_color(eColorBackground, r, g, b, a);
		break;

	case eColorGrid:
		memcpy(FreyjaRender::mColorGridSeperator, color, sizeof(vec4_t));
		memcpy(FreyjaRender::mColorGridLine, color, sizeof(vec4_t));
		freyja_event_set_color(eColorGrid, r, g, b, a);
		break;

	case eColorVertex:
		memcpy(FreyjaRender::mColorVertex, color, sizeof(vec4_t));
		freyja_event_set_color(eColorVertex, r, g, b, a);
		break;

	case eColorVertexHighlight:
		memcpy(FreyjaRender::mColorVertexHighlight, color, sizeof(vec4_t));
		freyja_event_set_color(eColorVertexHighlight, r, g, b, a);
		break;

	case eColorMesh:
		memcpy(FreyjaRender::mColorWireframe, color, sizeof(vec4_t));
		freyja_event_set_color(eColorMesh, r, g, b, a);
		break;

	case eColorMeshHighlight:
		memcpy(FreyjaRender::mColorWireframeHighlight, color, sizeof(vec4_t));
		freyja_event_set_color(eColorMeshHighlight, r, g, b, a);
		break;

	case eColorBone:
		memcpy(FreyjaRender::mColorBone, color, sizeof(vec4_t));
		freyja_event_set_color(eColorBone, r, g, b, a);
		break;

	case eColorBoneHighlight:
		memcpy(FreyjaRender::mColorBoneHighlight, color, sizeof(vec4_t));
		freyja_event_set_color(eColorBoneHighlight, r, g, b, a);
		break;

	default:
		return;
	}

	mgtk_event_gl_refresh();
}


///////////////////////////////////////////////////////////////////////
// Freyja wrappers
///////////////////////////////////////////////////////////////////////

const char *freyja_get_resource_path_callback()
{
	static String s;
	s = freyja_get_resource_path(); // in case it's changed somehow
	return s.c_str();
}


// FIXME remove duplicates
void freyja_handle_key_press(int key, int mod)
{
	freyja_print("mgtk_handle_key_press(%d, %d) not handled", key, mod);
}


void freyja_handle_command2i(int event, int command)
{
	freyja_event2i(event, command);
}


void freyja_handle_event1u(int event, unsigned int value)
{
	FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	// Removed legacy uint event codepath, kept old menu event fallback
	if (!ResourceEvent::listen(event - ePluginEventBase, value))
	{
		// Old style 2 ID menu events
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("!Event(%i, %i) dropped.", eEvent, event);
	}
}


void freyja_handle_event1f(int event, float value)
{
	FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	if (!FreyjaControl::mInstance->event(event, value))
	{
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("   mgtk_handle_event1f spawned previous unhandled event %i:%i", eEvent, event);
	}
}


void freyja_handle_gldisplay()
{
	FREYJA_ASSERTMSG(FreyjaRender::mInstance, "FreyjaRender Singleton not allocated");
	FreyjaRender::mInstance->Display();
}


void freyja_handle_glresize(unsigned int width, unsigned int height)
{
	FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaRender Singleton not allocated");
	FreyjaRender::mInstance->ResizeContext(width, height);
}


void freyja_handle_text_array(int event, unsigned int count, char **text)
{
	if (count != 2)
		return;

	if (!ResourceEvent::listen(event - ResourceEvent::eBaseEvent, 
							   text[0], text[1]))
	{
		// Not handled
	}	
}


void freyja_handle_text(int event, char *text)
{
	if (text == NULL || text[0] == 0)
		return;

	if (!ResourceEvent::listen(event - ePluginEventBase, text))
	{
		//if (!FreyjaControl::mInstance->handleTextEvent(event, text))
		freyja_print("%s(%i, '%s'): Unhandled event.", __func__, event, text);
	}
}


void freyja_callback_get_image_data_rgb24(const char *filename, 
										unsigned char **image, 
										int *width, int *height)
{
	FreyjaImage img;
	unsigned char *swap;

	*image = 0x0;
	*width = 0;
	*height = 0;

	if (!img.loadImage(filename))
	{
		img.setColorMode(FreyjaImage::RGB_24);
		img.scaleImage(256, 256);
		img.getImage(&swap);
		*image = swap;
		*width = img.getWidth();
		*height = img.getHeight();
	}
}


void freyja_handle_command(int command)
{
	FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	if (!ResourceEvent::listen(command - 10000 /*ePluginEventBase*/))
		freyja_print("!Event(%d): Unhandled event.", command);
}


void freyja_set_dialog_visible(const char *name)
{
	int e = ResourceEvent::GetResourceIdBySymbol((char*)name);
	mgtk_event_dialog_visible_set(e, 1);	
}


void polymap_update_question()
{
	if (mgtk::ExecuteConfirmationDialog("PolyMapDialog"))
	{
		Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());
		m->ConvertAllFacesToTexCoordPloymapping();
	}
}



////////////////////////////////////////////////////////////
// Non-object Event Handler Functions
////////////////////////////////////////////////////////////

void eNoImplementation(ResourceEvent *e)
{
	freyja_print("!'%s' : No longer implemented or disabled.",
				(e && e->getName()) ? e->getName() : "Unknown event");
}


void eMeshUnselectFaces()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if ( m )
	{
		for (uint32 i = 0, n = m->GetFaceCount(); i < n; ++i)
		{
			m->ClearFaceFlags(i, Vertex::fSelected);
		}

		freyja_print("Reset selected flag on all faces in mesh.");
	}
}


void eMeshUnselectVertices()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if ( m )
	{
		for (uint32 i = 0, n = m->GetVertexCount(); i < n; ++i)
		{
			m->ClearVertexFlags(i, Vertex::fSelected);
		}

		freyja_print("Reset selected flag on all vertices in mesh.");
	}
}


void eSetSelectedViewport(unsigned int value)
{
	FreyjaControl::mInstance->SetSelectedViewport(value);
}


void eSelectedFacesGenerateNormals()
{
	Mesh *m = freyjaGetMeshClass(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		m->SelectedFacesGenerateVertexNormals();
	}
}

void eSelectedFacesFlipNormals()
{
	Mesh *m = freyjaGetMeshClass(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		m->SelectedFacesFlipVertexNormals();
	}
}


void eSelectedFacesDelete()
{
	Mesh *m = freyjaGetMeshClass(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		m->DeleteSelectedFaces();
	}
}


void eSmoothingGroupsDialog()
{
	uint32 id = ResourceEvent::GetResourceIdBySymbol("eSmoothingGroupsDialog");
	mgtk_event_dialog_visible_set(id, 1);	
}


void eSmooth(unsigned int group, unsigned int value)
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		freyja_print("Faces in group (%i) %s.", 
					 group, value ? "selected" : "unselected");

		if (value)
		{
			FreyjaControl::mInstance->mGroupBitmap |= (1<<group);
			m->SetGroupsFaceSelected(FreyjaControl::mInstance->mGroupBitmap);
		}
		else
		{
			m->ClearGroupsFaceSelected(FreyjaControl::mInstance->mGroupBitmap);
			FreyjaControl::mInstance->mGroupBitmap ^= (1<<group);
		}

		freyja_event_gl_refresh();
	}
}


void eGroupClear()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		uint32 group = FreyjaControl::mInstance->mGroupBitmap;

		if (group > 24)
		{
			freyja_print("Make sure only one group is toggled while assigning");
			return;
		}

		freyja_print("Selected faces removed from smoothing group (%i).",group);
		m->SelectedFacesMarkSmoothingGroup(group, false);
		freyja_event_gl_refresh();
	}
}


void eSetSelectedFacesAlpha()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		freyja_print("Selected faces alpha flag enabled.");
		m->SetFlagForSelectedFaces(Face::fAlpha);
		FreyjaControl::mInstance->Dirty();
	}
	else
	{
		freyja_print("Alpha flag can not be set. No selected faces.");
	}
}


void eClearSelectedFacesAlpha()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		freyja_print("Selected faces alpha flag enabled.");
		m->ClearFlagForSelectedFaces(Face::fAlpha);
		FreyjaControl::mInstance->Dirty();
	}
	else
	{
		freyja_print("Alpha flag can not be cleared. No selected faces.");
	}
}


void eGroupAssign()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		uint32 group = FreyjaControl::mInstance->mGroupBitmap;

		if (group > 24)
		{
			freyja_print("Make sure only one group is toggled while assigning");
			return;
		}

		freyja_print("Selected faces assigned to smoothing group (%i).", group);
		m->SelectedFacesMarkSmoothingGroup(group, true);

		// Go ahead and update the vertex normals here automatically for now
		//m->GroupedFacesGenerateVertexNormals(group);
		freyja_event_gl_refresh();
	}
}


vec_t gWeight = 1.0f;
void eWeight(vec_t w)
{
	gWeight = w;
	freyja_print("Weight set to %f", gWeight);
}

void eAssignWeight()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		index_t bone = FreyjaControl::mInstance->GetSelectedBone();
		m->SetWeightSelectedVertices(bone, gWeight);
		freyja_print("Selected vertices set to bone %i weighting to %f.", bone, gWeight);
	}
}

void eClearWeight()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		index_t bone = FreyjaControl::mInstance->GetSelectedBone();
		m->RemoveWeightSelectedVertices(bone);
		freyja_print("Selected vertices removing bone %i weighting...", bone);
	}
}


void eBoneRefreshBindPose()
{
	index_t bone = FreyjaControl::mInstance->GetSelectedBone();
	Bone *b = Bone::GetBone( bone );
	if (b) b->UpdateBindPose();
}


void eMirrorMeshX()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(-1.0f, 1.0f, 1.0f));
		freyja_print("Mirroring mesh over X...");
		m->TransformVertices(mat);
	}
}

void eMirrorMeshY()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, -1.0f, 1.0f));
		freyja_print("Mirroring mesh over Y...");
		m->TransformVertices(mat);
	}
}


void eMirrorMeshZ()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, 1.0f, -1.0f));
		freyja_print("Mirroring mesh over Z...");
		m->TransformVertices(mat);
	}
}


void eMirrorFacesX()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(-1.0f, 1.0f, 1.0f));
		freyja_print("Mirroring selected faces over X...");
		//m->TransformFacesWithFlag(Face::fSelected, mat);
		m->TransformVertices(mat);
	}
}


void eMirrorFacesY()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, -1.0f, 1.0f));
		freyja_print("Mirroring selected faces over Y...");
		m->TransformFacesWithFlag(Face::fSelected, mat);
		m->TransformVertices(mat);
	}
}


void eMirrorFacesZ()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		hel::Mat44 mat;
		mat.Scale(hel::Vec3(1.0f, 1.0f, -1.0f));
		freyja_print("Mirroring selected faces over Z...");
		m->TransformFacesWithFlag(Face::fSelected, mat);
		m->TransformVertices(mat);
	}
}


vec_t gSnapWeldVertsDist = 0.001f;
void eSnapWeldVertsDist(vec_t d)
{
	gSnapWeldVertsDist = d;
}


void eSnapWeldVerts()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		m->WeldVerticesByDistance(gSnapWeldVertsDist);
	}
}

void eCleanupVertices()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());

	if (m)
	{
		m->VertexCleanup();
	}
}



void eGenerateCone()
{
	hel::Vec3 v;
	freyjaGenerateConeMesh(v.mVec, 
						   FreyjaControl::mInstance->GetGenMeshHeight(),
						   FreyjaControl::mInstance->GetGenMeshCount());
	FreyjaControl::mInstance->Dirty();
}


void eGenerateCylinder()
{
	hel::Vec3 v;
	freyjaGenerateCylinderMesh(v.mVec, 
							   FreyjaControl::mInstance->GetGenMeshHeight(), 
							   FreyjaControl::mInstance->GetGenMeshCount(),
							   FreyjaControl::mInstance->GetGenMeshSegements());
	FreyjaControl::mInstance->Dirty();
}


void eGenerateTube()
{
	const char *dialog = "GenerateTubeDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int count = mgtk_get_query_dialog_int(dialog, "count");
		int segments = mgtk_get_query_dialog_int(dialog, "segments");
		vec_t height = mgtk_get_query_dialog_float(dialog, "height");
		vec_t radius = mgtk_get_query_dialog_float(dialog, "radius");

		FREYJA_ASSERTMSG(0, "c = %i, s = %i, r = %f, h = %f",
						 count, segments, radius, height);

		hel::Vec3 v;
		freyjaGenerateTubeMesh(v.mVec, height, count, segments);
		FreyjaControl::mInstance->Dirty();		
	}
}


void eGenerateSphere()
{
	hel::Vec3 v;
	freyjaGenerateSphereMesh(v.mVec, 
							 FreyjaControl::mInstance->GetGenMeshHeight(), 
							 FreyjaControl::mInstance->GetGenMeshCount(),
							 FreyjaControl::mInstance->GetGenMeshCount());
	FreyjaControl::mInstance->Dirty();
}


void eGenerateCube()
{
	float size = mgtk_create_query_dialog_float("gtk-dialog-question",
												"Size?",
												FreyjaControl::mInstance->GetGenMeshHeight(), 
												0.5, 64, 
												1, 3);
	hel::Vec3 v(size * -0.5f, 0.0f, size * -0.5f);
	index_t mesh = freyjaMeshCreateCube(v.mVec, size);
	FreyjaControl::mInstance->SetSelectedMesh(mesh);
	FreyjaControl::mInstance->Dirty();
}


void eGeneratePlane()
{
	const char *dialog = "GenerateSheetDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int rows = mgtk_get_query_dialog_int(dialog, "rows");
		int cols = mgtk_get_query_dialog_int(dialog, "cols");
		vec_t size = mgtk_get_query_dialog_float(dialog, "size");

		//FREYJA_ASSERTMSG(0, "%i %i %f", rows, cols, size);

		size *= 4;
		hel::Vec3 v(size * -0.5f, 0.3f, size * -0.5f);
		freyjaMeshCreateSheet(v.mVec, size, rows, cols);
		FreyjaControl::mInstance->Dirty();		
	}
}


void eGenerateCircle()
{
	const char *dialog = "GenerateCircleDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int count = mgtk_get_query_dialog_int(dialog, "count");
		vec_t r = mgtk_get_query_dialog_float(dialog, "radius");
		hel::Vec3 v(0.0f, 0.2f, 0.0f);

		freyjaMeshCreateCircle(v.mVec, r, count);
		FreyjaControl::mInstance->Dirty();		
	}
}


void eGenerateRing()
{
	const char *dialog = "GenerateRingDialog";
	int ok = mgtk_execute_query_dialog(dialog);

	if (ok)
	{
		int rings = mgtk_get_query_dialog_int(dialog, "rings");
		int count = mgtk_get_query_dialog_int(dialog, "count");
		vec_t r = mgtk_get_query_dialog_float(dialog, "radius");
		hel::Vec3 v(0.0f, 0.2f, 0.0f);

		freyjaMeshCreateRing(v.mVec, r, count, rings+1);
		FreyjaControl::mInstance->Dirty();		
	}
}


void eMeshTesselate()
{
	freyjaMeshTesselateTriangles(FreyjaControl::mInstance->GetSelectedMesh());
	FreyjaControl::mInstance->Dirty();
}


void eMeshTexcoordSpherical()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());
	if (m)
	{
		m->UVMapSelectedFaces_Spherical();
		polymap_update_question();
		FreyjaControl::mInstance->Dirty();
	}
}


void eMeshTexcoordCylindrical()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());
	if (m)
	{
		m->UVMapSelectedFaces_Cylindrical();
		polymap_update_question();
		FreyjaControl::mInstance->Dirty();
	}
}


void eMeshTexcoordPlaneProj()
{
	Mesh *m = Mesh::GetMesh(FreyjaControl::mInstance->GetSelectedMesh());
	if (m)
	{
		m->UVMapSelectedFaces_Plane();
		polymap_update_question();
		FreyjaControl::mInstance->Dirty();
	}
}


void eMeshGenerateNormals()
{
	freyjaMeshGenerateVertexNormals(FreyjaControl::mInstance->GetSelectedMesh());
	freyja_event_gl_refresh();
	FreyjaControl::mInstance->Dirty();
}

// This should be moved back into control with a -W/Int prefix
void eMoveObject()
{
	FreyjaControl::mInstance->eMoveObject(1);
}

void eRotateObject()
{
	FreyjaControl::mInstance->eRotateObject(1);
}	

void eScaleObject()
{
	FreyjaControl::mInstance->eScaleObject(1);
}	

void eSelect()
{
	FreyjaControl::mInstance->eSelect(1);
}

void eUnselect()
{
	FreyjaControl::mInstance->eUnselect(1);
}		


void eBoneSelect()
{
	FreyjaControl::mInstance->SetObjectMode(FreyjaControl::tBone);
	FreyjaControl::mInstance->SetActionMode(FreyjaControl::aSelect);
	freyja_print("Select bone...");
}


void eBoneNew()
{
	FreyjaControl::mInstance->SetObjectMode(FreyjaControl::tBone);
	FreyjaControl::mInstance->CreateObject();
	//freyja_print("Select new child bone placement directly...");
	//mEventMode = BONE_ADD_MODE;
	FreyjaControl::mInstance->Dirty();
}


void eExtrude()
{
	hel::Vec3 v = FreyjaRender::mTestRay.mDir;
	v *= -8.0f;
	freyjaMeshPolygonExtrudeQuad1f(FreyjaControl::mInstance->GetSelectedMesh(), 
								   FreyjaControl::mInstance->GetSelectedFace(),
								   v.mVec);
#if 0
	// Currently we don't support this 
	// ( it marks all the new face vertices as selected )
	if (freyjaGetPolygonVertexCount(GetSelectedFace()))
	{
		long polygonIndex = GetSelectedFace();
		long i, idx, count = freyjaGetPolygonVertexCount(polygonIndex);
		Vector<unsigned int> list = GetVertexSelectionList();
		
		list.clear();
		
		for (i = 0; i < count; ++i)
		{
			idx = freyjaGetPolygonVertexIndex(polygonIndex, i);
			list.pushBack(idx);
		}
	}
#endif

	FreyjaControl::mInstance->Dirty();
}

void eMeshFlipNormals()
{
	freyjaMeshNormalFlip(FreyjaControl::mInstance->GetSelectedMesh());
	freyja_print("Flipping normals for mesh[%i]", 
				 FreyjaControl::mInstance->GetSelectedMesh());
	FreyjaControl::mInstance->Dirty();
}


void eHelpDialog()
{
	freyja_set_dialog_visible("eHelpDialog");
}

void ePreferencesDialog()
{
	freyja_set_dialog_visible("ePreferencesDialog");
}

void eAboutDialog()
{
	freyja_set_dialog_visible("eAboutDialog");
}


void eAnimationNext()
{
	FreyjaControl::mInstance->SetSelectedAnimation(FreyjaControl::mInstance->GetSelectedAnimation() + 1);
	freyja_print("Animation Track[%i].", 
				 FreyjaControl::mInstance->GetSelectedAnimation());
}

void eAnimationPrev()
{
	if (FreyjaControl::mInstance->GetSelectedAnimation())
		FreyjaControl::mInstance->SetSelectedAnimation(FreyjaControl::mInstance->GetSelectedAnimation() - 1);
	else
		FreyjaControl::mInstance->SetSelectedAnimation(0);
	
	freyja_print("Animation Track[%i].", 
				 FreyjaControl::mInstance->GetSelectedAnimation());
}


void eLightPos(uint32 i, vec_t value)
{
	uint32 light = FreyjaControl::mInstance->GetSelectedLight();
	vec3_t pos;
	freyjaGetLightPosition4v(light, pos);
	pos[i] = value;
	freyjaLightPosition4v(light, pos);
	FreyjaControl::mInstance->Dirty();	
}


void eLightPosX(vec_t value)
{
	eLightPos(0, value);
}


void eLightPosY(vec_t value)
{
	eLightPos(1, value);
}


void eLightPosZ(vec_t value)
{
	eLightPos(2, value);
}


void ePolygonSize(uint32 value)
{
	FreyjaControl::mInstance->SetFaceEdgeCount(value);
	freyja_print("Polygons creation using %i sides", 
				 FreyjaControl::mInstance->GetFaceEdgeCount());
}


void eTestTextView()
{
	mgtk_create_query_dialog_text("gtk-dialog-question", 
								  "Knock knock?", eNone, "Who's there?");
}


void eTestAssertMsgBox()
{
	void *ptr = NULL;

	FREYJA_ASSERTMSG(ptr != NULL, 
					 "Dummy NULL pointer assertion test.\n\tptr = %p", 
					 ptr);
}


mstl::String gTestBoneMetadata("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
void eTestBoneMetadataText(char *text)
{
	gTestBoneMetadata = text;
}


void eTestBoneMetadata()
{
	uint32 e = ResourceEvent::GetResourceIdBySymbol("eTestBoneMetadataText");

	mgtk_create_query_dialog_text("gtk-dialog-question", 
								  "Bone metadata test.", 
								  e, gTestBoneMetadata.c_str());
}


void FreyjaMiscEventsAttach()
{
	ResourceEventCallback::add("eTestAssertMsgBox", &eTestAssertMsgBox);
	ResourceEventCallback::add("eTestTextView", &eTestTextView);
	ResourceEventCallbackString::add("eTestBoneMetadataText", &eTestBoneMetadataText);

	ResourceEventCallback::add("eBoneRefreshBindPose", &eBoneRefreshBindPose);
	ResourceEventCallback::add("eBoneMetaData", &eTestBoneMetadata);
	ResourceEventCallback::add("eBoneNew", &eBoneNew);
	ResourceEventCallback::add("eBoneSelect", &eBoneSelect);

	ResourceEventCallback::add("eMoveObject", &eMoveObject);
	ResourceEventCallback::add("eRotateObject", &eRotateObject);
	ResourceEventCallback::add("eScaleObject", &eScaleObject);
	ResourceEventCallback::add("eSelect", &eSelect);
	ResourceEventCallback::add("eUnselect", &eUnselect);

	ResourceEventCallback::add("eGenerateRing", &eGenerateRing);
	ResourceEventCallback::add("eGenerateCircle", &eGenerateCircle);
	ResourceEventCallback::add("eGeneratePlane", &eGeneratePlane);
	ResourceEventCallback::add("eGenerateCube", &eGenerateCube);
	ResourceEventCallback::add("eGenerateSphere", &eGenerateSphere);
	ResourceEventCallback::add("eGenerateTube", &eGenerateTube);
	ResourceEventCallback::add("eGenerateCylinder", &eGenerateCylinder);
	ResourceEventCallback::add("eGenerateCone", &eGenerateCone);

	ResourceEventCallbackVec::add("eSnapWeldVertsDist", &eSnapWeldVertsDist);

	ResourceEventCallback::add("eSetSelectedFacesAlpha", &eSetSelectedFacesAlpha);
	ResourceEventCallback::add("eClearSelectedFacesAlpha", &eClearSelectedFacesAlpha);

	ResourceEventCallback::add("eAssignWeight", &eAssignWeight);
	ResourceEventCallback::add("eClearWeight", &eClearWeight);
	ResourceEventCallbackVec::add("eWeight", &eWeight);

	ResourceEventCallback::add("eMirrorFacesX", &eMirrorFacesX);
	ResourceEventCallback::add("eMirrorFacesY", &eMirrorFacesY);
	ResourceEventCallback::add("eMirrorFacesZ", &eMirrorFacesZ);

	ResourceEventCallback::add("eMeshTesselate", &eMeshTesselate);
	ResourceEventCallback::add("eMeshTexcoordSpherical", &eMeshTexcoordSpherical);
	ResourceEventCallback::add("eMeshTexcoordCylindrical", &eMeshTexcoordCylindrical);
	ResourceEventCallback::add("eMeshGenerateNormals", &eMeshGenerateNormals);
	ResourceEventCallback::add("eMeshTexcoordPlaneProj", &eMeshTexcoordPlaneProj);
	ResourceEventCallback::add("eMirrorMeshX", &eMirrorMeshX);
	ResourceEventCallback::add("eMirrorMeshY", &eMirrorMeshY);
	ResourceEventCallback::add("eMirrorMeshZ", &eMirrorMeshZ);

	ResourceEventCallback::add("eCleanupVertices", eCleanupVertices);
	ResourceEventCallback::add("eSnapWeldVerts", &eSnapWeldVerts);

	ResourceEventCallback::add("eGroupClear", &eGroupClear);
	ResourceEventCallback::add("eGroupAssign", &eGroupAssign);

	ResourceEventCallback::add("eSmoothingGroupsDialog", eSmoothingGroupsDialog);
	ResourceEventCallbackUInt2::add("eSmooth", &eSmooth);

	ResourceEventCallback::add("eSelectedFacesFlipNormals", &eSelectedFacesFlipNormals);
	ResourceEventCallback::add("eSelectedFacesGenerateNormals", &eSelectedFacesGenerateNormals);
	ResourceEventCallback::add("eSelectedFacesDelete", &eSelectedFacesDelete);
	ResourceEventCallback::add("eMeshUnselectFaces", &eMeshUnselectFaces);
	ResourceEventCallback::add("eMeshUnselectVertices", &eMeshUnselectVertices);

	ResourceEventCallbackUInt::add("eSetSelectedViewport", &eSetSelectedViewport);
	ResourceEventCallbackUInt::add("ePolygonSize", &ePolygonSize);

	// FIXME, These light events don't appear to trigger
	ResourceEventCallbackVec::add("eLightPosX", &eLightPosX);
	ResourceEventCallbackVec::add("eLightPosY", &eLightPosY);
	ResourceEventCallbackVec::add("eLightPosZ", &eLightPosZ);

	ResourceEventCallback::add("eAnimationNext", &eAnimationNext);
	ResourceEventCallback::add("eAnimationPrev", &eAnimationPrev);

	ResourceEventCallback::add("eMeshFlipNormals", &eMeshFlipNormals);
	ResourceEventCallback::add("eHelpDialog", &eHelpDialog);
	ResourceEventCallback::add("ePreferencesDialog", &ePreferencesDialog);
	ResourceEventCallback::add("eAboutDialog", &eAboutDialog);

	ResourceEventCallback::add("eExtrude", &eExtrude);
}


///////////////////////////////////////////////////////////////////////


void freyja_handle_resource_init(Resource &r)
{
	////////////////////////////////////////////////////////////////////
	// New freyja events
	////////////////////////////////////////////////////////////////////

	// Not implemented or removed misc events
	//ResourceEventCallback2::add("eTextureSlotLoadToggleB", &eNoImplementation);
	ResourceEventCallback2::add("eBezierPolygonPatch", &eNoImplementation);
	ResourceEventCallback2::add("eSelectAll", &eNoImplementation);
	ResourceEventCallback2::add("eShaderSlotLoad", &eNoImplementation);
	ResourceEventCallback2::add("eOpenFileTextureB", &eNoImplementation);
	ResourceEventCallback2::add("eCollapseFace", &eNoImplementation);
	ResourceEventCallback2::add("eSetMaterialTextureB", &eNoImplementation);
	ResourceEventCallback2::add("eEnableMaterialFragment", &eNoImplementation);
	ResourceEventCallback2::add("eUVPickRadius", &eNoImplementation);
	ResourceEventCallback2::add("eVertexPickRadius", &eNoImplementation);
	ResourceEventCallback2::add("eAnimationStop", &eNoImplementation);
	ResourceEventCallback2::add("eAnimationPlay", &eNoImplementation);
	ResourceEventCallback2::add("eBoneLinkChild", &eNoImplementation);
	ResourceEventCallback2::add("eBoneUnLinkChild", &eNoImplementation);
	ResourceEventCallback2::add("eAppendFile", &eNoImplementation);
	ResourceEventCallback2::add("eBoneDelete", &eNoImplementation);
	ResourceEventCallback2::add("eVertexNew", &eNoImplementation);
	ResourceEventCallback2::add("eVertexDelete", &eNoImplementation);
	ResourceEventCallback2::add("ePolygonNew", &eNoImplementation);
	ResourceEventCallback2::add("ePolygonDelete", &eNoImplementation);
	ResourceEventCallback2::add("ePolygonSelect", &eNoImplementation);
	ResourceEventCallback2::add("eRenderShadow", &eNoImplementation);
	ResourceEventCallback2::add("ePolyMapTexturePolygon", &eNoImplementation);
	ResourceEventCallback2::add("eUVMapCreate", &eNoImplementation);
	ResourceEventCallback2::add("eUVMapDelete", &eNoImplementation);
	ResourceEventCallback2::add("eMirrorUV_X", &eNoImplementation);
	ResourceEventCallback2::add("eMirrorUV_Y", &eNoImplementation);
	ResourceEventCallback2::add("eTranslateUV", &eNoImplementation);
	ResourceEventCallback2::add("eRotateUV", &eNoImplementation);
	ResourceEventCallback2::add("eScaleUV", &eNoImplementation);
 
	// Class listeners
	FreyjaControl::AttachMethodListeners();
	FreyjaRender::AttachMethodListeners();

	// Non-class listeners
	FreyjaMiscEventsAttach();

	/* Mongoose 2002.01.12, 
	 * Bind script functions to C/C++ functions */
	r.RegisterFunction("color", freyja_rc_color);


	////////////////////////////////////////////////////////////////////
	// Old style events
	////////////////////////////////////////////////////////////////////

	/* Mongoose 2002.01.21, 
	 * Bind some script vars to matching name in C/C++ */

	// Event types used for flow control of event ids
	r.RegisterInt("eMode", eMode);
	r.RegisterInt("eEvent", eEvent);
	r.RegisterInt("eNop", eNop);
	r.RegisterInt("eNone", eNone);

	// Menus
	r.RegisterInt("ePluginMenu", ePluginMenu);
	r.RegisterInt("eBlendDestMenu", eBlendDestMenu);
	r.RegisterInt("eBlendSrcMenu", eBlendSrcMenu);
	r.RegisterInt("eObjectMenu", eObjectMenu);
	r.RegisterInt("eViewportModeMenu", eViewportModeMenu);
	r.RegisterInt("eTransformMenu", eTransformMenu);

	// Widget events ( widgets hold data like spinbuttons, etc )
	r.RegisterInt("eMove_X", eMove_X);
	r.RegisterInt("eMove_Y", eMove_Y);
	r.RegisterInt("eMove_Z", eMove_Z);
	r.RegisterInt("eRotate_X", eRotate_X);
	r.RegisterInt("eRotate_Y", eRotate_Y);
	r.RegisterInt("eRotate_Z", eRotate_Z);
	r.RegisterInt("eScale_X", eScale_X);
	r.RegisterInt("eScale_Y", eScale_Y);
	r.RegisterInt("eScale_Z", eScale_Z);

	// Colors
	r.RegisterInt("eColorMaterialAmbient", eColorMaterialAmbient);
	r.RegisterInt("eColorMaterialDiffuse", eColorMaterialDiffuse);
	r.RegisterInt("eColorMaterialSpecular", eColorMaterialSpecular);
	r.RegisterInt("eColorMaterialEmissive", eColorMaterialEmissive);
	r.RegisterInt("eColorLightAmbient", eColorLightAmbient);
	r.RegisterInt("eColorLightDiffuse", eColorLightDiffuse);
	r.RegisterInt("eColorLightSpecular", eColorLightSpecular);
	r.RegisterInt("eColorBackground", eColorBackground);
	r.RegisterInt("eColorGrid", eColorGrid);
	r.RegisterInt("eColorMesh", eColorMesh);
	r.RegisterInt("eColorVertex", eColorVertex);
	r.RegisterInt("eColorVertexHighlight", eColorVertexHighlight);
	r.RegisterInt("eColorMeshHighlight", eColorMeshHighlight);
	r.RegisterInt("eColorBone", eColorBone);
	r.RegisterInt("eColorBoneHighlight", eColorBoneHighlight);

	/* Load and init plugins */
	String dir = freyja_rc_map_string("plugins");	
	freyja_init_application_plugins(dir.GetCString());

	/* Hook plugins to resource */
	uint32 i, n = mgtk::ResourcePlugin::mPlugins.size();

	for (i = 0; i < n; ++i)
	{
		if (mgtk::ResourcePlugin::mPlugins[i] != 0x0)
		{
			mgtk::ResourcePlugin::mPlugins[i]->mEventsAttach();
		}
	}
}


void freyja_handle_resource_start()
{
	/* Mongoose 2002.02.02,  
	 * This is the backend entry called by the mgtk widget layer.
	 */

	/* Mongoose 2007.04.07,
	 * Here all the resources are setup, and the main interface starts. 
	 */

	freyja_print("!@Freyja started...");

	freyja_handle_resource_init(FreyjaControl::GetInstance()->GetResource());

	/* User install of icons, samples, configs, etc */
	if (!freyja_is_user_installed())
		freyja_install_user();

	/* Build the user interface from lisp, and load user preferences */
	FreyjaControl::mInstance->Init();

	/* FreyjaAppPlugin prototype testing... */
	uint32 i, n = mgtk::ResourcePlugin::mPlugins.size();

	for (i = 0; i < n; ++i)
	{
		if (mgtk::ResourcePlugin::mPlugins[i] != 0x0)
		{
			mgtk::ResourcePlugin::mPlugins[i]->mGUIAttach();
		}
	}

	/* Setup material interface */
	MaterialEv::mHack->RefreshInterface();

	/* Setup editor modes and drop-down menus */
	mgtk_option_menu_value_set(eViewportModeMenu, 0);
	FreyjaControl::mInstance->eModeModel();

	mgtk_option_menu_value_set(eTransformMenu, 1);
	FreyjaControl::mInstance->SetObjectMode(FreyjaControl::tMesh);

	mgtk_option_menu_value_set(eObjectMenu, 0);
	FreyjaControl::mInstance->SetActionMode(FreyjaControl::aSelect);

	/* Set init window title, log welcome, and refresh OpenGL context */
	freyja_set_main_window_title(BUILD_NAME);
	freyja_print("Welcome to Freyja %s-%s, %s", VERSION, BUILD_ID, __DATE__);
	mgtk_event_gl_refresh();

	/* Mongoose 2002.02.23, Hook for exit() calls */
	atexit(freyja_event_shutdown);
}


void freyja_append_eventid(char *symbol, int eventid)
{
	FreyjaControl::mInstance->GetResource().RegisterInt(symbol, eventid);
}


void freyja_get_rc_path(char *s, long sz)
{
	long len;


	s[0] = 0;

#ifdef unix
	char *env;

	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
		return;
	}

	len = strlen(env) + strlen("/.freyja/");

	if (sz < len)
		return;

	snprintf(s, len, "%s/.freyja/", env);
#else
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);

	len = strlen(cwd);

	if (sz < len)
		return;
	
	snprintf(s, len, cwd);
#endif
}


void freyja_get_share_path(char *s, long sz)
{
	if (sz < 64)
		return;

#ifdef unix
	snprintf(s, strlen("/usr/share/freyja/"), "/usr/share/freyja/");
#else
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);
	snprintf(s, strlen(cwd), cwd);
#endif
}


void freyja_get_rc_filename(char *s, const char *filename, long sz)
{
	long len;


	s[0] = 0;

#ifdef unix
	char *env;

	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
		return;
	}

	len = strlen(env) + strlen("/.freyja/") + strlen(filename);

	if (sz < len)
		return;

	snprintf(s, len, "%s/.freyja/%s", env, filename);
#else
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);

	len = strlen(cwd) + strlen(filename);

	if (sz < len)
		return;
	
	snprintf(s, len, "%s/%s", cwd, filename);
#endif
}


void freyja_get_share_filename(char *s, const char *filename, long sz)
{
	long len;


	s[0] = 0;

#ifdef unix
	len = strlen("/usr/share/freyja/") + strlen(filename);

	if (sz < len)
		return;

	snprintf(s, len, "/usr/share/freyja/%s", filename);
#else
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);

	len = strlen(cwd) + strlen(filename);

	if (sz < len)
		return;
	
	snprintf(s, len, "%s/%s", cwd, filename);
#endif
}


char freyja_is_user_installed()
{
	SystemIO::FileReader r;
	char path[128];

	freyja_get_rc_path(path, 128);

#ifdef WIN32
	return 1;
#endif

	return (r.DoesFileExist(path));
}


void freyja_install_user()
{
	SystemIO::FileReader r;
	SystemIO::FileWriter w;
	char rc[512];
	char share[512];
	const char *filename;


	/* Copy top level rc files */
	freyja_get_rc_path(rc, 512);
	freyja_get_share_path(share, 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Copy icon files */
	freyja_get_rc_filename(rc, "icons/", 512);
	freyja_get_share_filename(share, "icons/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Copy material files */
	freyja_get_rc_filename(rc, "materials/", 512);
	freyja_get_share_filename(share, "materials/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Copy palettes files */
	freyja_get_rc_filename(rc, "palettes/", 512);
	freyja_get_share_filename(share, "palettes/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Misc */
	freyja_get_rc_filename(rc, "models/", 512);
	SystemIO::File::CreateDir(rc);

	freyja_get_rc_filename(rc, "animations/", 512);
	SystemIO::File::CreateDir(rc);

	freyja_get_rc_filename(rc, "textures/", 512);
	SystemIO::File::CreateDir(rc);

	//freyja_get_rc_filename(rc, "particles/", 512);
	//SystemIO::File::CreateDir(rc);

	/* Copy plugins */
	freyja_get_rc_filename(rc, "plugins/", 512);
	freyja_get_share_filename(share, "plugins/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue; // Might want to recurse copy plugins later.

			SystemIO::CopyFileToPath(filename, rc);
		}
	}
}


void freyja_handle_motion(int x, int y)
{
	if (FreyjaControl::mInstance)
	{
		FreyjaControl::mInstance->MotionEvent(x, y);
	}
}


void freyja_handle_mouse(int button, int state, int mod, int x, int y)
{
	if (FreyjaControl::mInstance)
	{
		FreyjaControl::mInstance->MouseEvent(button, state, mod, x, y);
	}
}


int freyja_event2i(int event, int cmd)
{
	if (FreyjaControl::mInstance && event != eNop)
	{
		if (!FreyjaControl::mInstance->handleEvent(event, cmd))
		{
			freyja_print("  freyja_event2i passed previous unhandled event %i:%i",
						event, cmd);

			return -1;
		}
	}

	return 0;
}


void freyja_print(char *format, ...)
{
	va_list args;

	va_start(args, format);
	ControlPrinter::PrintArgs(format, &args);
	va_end(args);
}


void freyja_event_shutdown()
{
	if (FreyjaControl::mInstance)
	{
		delete FreyjaControl::mInstance;
	}

	freyja_print("!Thanks for using %s", PROGRAM_NAME);
	freyja_print("!   Build date: %s @ %s", __DATE__, __TIME__);
	freyja_print("!   Build host: %s", BUILD_HOST);
	freyja_print("!   Email addr: %s", EMAIL_ADDRESS);
	freyja_print("!   Web site  : %s", PROJECT_URL);

	ControlPrinter::StopLogging();
}


String freyja_get_resource_path()
{
	mstl::String s;

#if defined unix || MACOSX
	char *env = getenv("HOME");
	
	if (!env || !env[0])
	{
		MSTL_MSG("ERROR: Bad HOME envronment\n");
		s = "/usr/share/freyja";
	}
	else
	{
		s = env;
	}
	
	s += "/.freyja/";
#else
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);
	
	s = cwd;
	s += "/";
#endif

	return s;
}


String freyja_rc_map_string(const char *filename)
{
	String s = freyja_get_resource_path();
	s += filename;
	return s;
}


char *freyja_rc_map(char *basename)
{
	String s = freyja_get_resource_path();
	s += basename;
	return mstl::String::Strdup(s.c_str());;
}


arg_list_t *freyja_rc_color(arg_list_t *args)
{
	arg_list_t *sym, *r, *g, *b, *a;


	symbol_enforce_type(&sym, INT);
	symbol_enforce_type(&r, FLOAT);
	symbol_enforce_type(&g, FLOAT);
	symbol_enforce_type(&b, FLOAT);
	symbol_enforce_type(&a, FLOAT);

	// Call event func here - simulated with printf in tests
	if (sym && r && g && b && a)
	{
#ifdef DEBUG_RESOURCE_COLOR
		freyja_print("extern \"C\" { color(%s, %f, %f, %f); }\n",
					 get_int(sym),//get_string(sym),
					 get_float(r), get_float(g), get_float(b));
#endif

		mgtk_handle_color(get_int(sym),
						  get_float(r), get_float(g), get_float(b),
						  get_float(a));
	}
	else
	{
		printf("color> Failed to extract strict typed data from script\n");
	}

	delete_arg(&sym);
	delete_arg(&r);
	delete_arg(&g);
	delete_arg(&b);
	delete_arg(&a);

	return NULL;
}


void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
	if (size < 1)
		return;

#if defined unix || MACOSX
	snprintf(dest, size, "%s/.freyja/icons/%s",
			 (char *)getenv("HOME"), icon_name);
#else
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);
	snprintf(dest, size, "%s/icons/%s", cwd, icon_name);
#endif

	dest[size-1] = 0;
}


int FreyjaAssertCallbackHandler(const char *file, unsigned int line, 
								const char *function,
								const char *expression, const char *msg)
{
	mstl::String s;
	s.Set("Assert encountered:\n %s:%i %s()\n '%s'\n %s", 
		  file, line, function, expression, msg);

	ControlPrinter::mLog.Print("[ASSERT] %s\n", s.c_str());

	// FIXME: Make a confirmation dialog as a locale template for this,
	//        and fill in the 'info' string here every time you call it.
	ConfirmationDialog q("FreyjaAssertCallbackHandlerDialog", 
						 "gtk-dialog-error", //"gtk-dialog-warning"
						 s.c_str(), 
						 "\nWould you like to continue with errors anyway?",
						 "gtk-quit", "_Exit", "gtk-ok", "_Continue");

	int action = q.Execute();

	ControlPrinter::mLog.Print("[ASSERT] User %s\n", 
							   action ? "continued" : "aborted");

	return action;
}


int FreyjaDebugInfoCallbackHandler(const char *file, unsigned int line, 
								   const char *function,
								   const char *expression, const char *msg)
{
	mstl::String s;
	s.Set("Called from:\n %s:%i %s()\n %s", 
		  file, line, function, msg); // Removed expression 

	ControlPrinter::mLog.Print("[INFO] %s\n", s.c_str());

	ConfirmationDialog q("FreyjaDebugInfoCallbackHandlerDialog", 
						 "gtk-dialog-info",//"gtk-dialog-warning",
						 s.c_str(), 
						 "",
						 "", "", "gtk-close", "_Close");

	int action = q.Execute();

	ControlPrinter::mLog.Print("[INFO] User %s\n", 
							   action ? "ok" : "cancel");

	return action;
}


int main(int argc, char *argv[])
{
	/* 'Link' up mgtk library stubs to these implementations */
	mgtk_link_import("mgtk_callback_get_image_data_rgb24", 
					 (void*)freyja_callback_get_image_data_rgb24);
	mgtk_link_import("mgtk_handle_color", (void*)freyja_handle_color);
	mgtk_link_import("mgtk_handle_application_window_close", 
					 (void*)freyja_handle_application_window_close);
	mgtk_link_import("mgtk_handle_command", (void*)freyja_handle_command);
	mgtk_link_import("mgtk_handle_command2i", (void*)freyja_handle_command2i);
	mgtk_link_import("mgtk_handle_event1u", (void*)freyja_handle_event1u);
	mgtk_link_import("mgtk_handle_event1f", (void*)freyja_handle_event1f);
	mgtk_link_import("mgtk_handle_gldisplay", (void*)freyja_handle_gldisplay);
	mgtk_link_import("mgtk_handle_glresize", (void*)freyja_handle_glresize);
	mgtk_link_import("mgtk_handle_key_press", (void*)freyja_handle_key_press);
	mgtk_link_import("mgtk_handle_motion", (void*)freyja_handle_motion);
	mgtk_link_import("mgtk_handle_mouse", (void*)freyja_handle_mouse);
	mgtk_link_import("mgtk_handle_resource_start", 
					 (void*)freyja_handle_resource_start);
	mgtk_link_import("mgtk_handle_text_array", (void*)freyja_handle_text_array);
	mgtk_link_import("mgtk_handle_text", (void*)freyja_handle_text);
	mgtk_link_import("mgtk_print", (void*)freyja_print);
	mgtk_link_import("mgtk_get_pixmap_filename", 
					 (void*)freyja_get_pixmap_filename);
	mgtk_link_import("mgtk_rc_map", (void*)freyja_rc_map);
	mgtk_link_import("mgtk_get_resource_path", 
					 (void*)freyja_get_resource_path_callback);

	/* Enabled logging */
	mstl::String s = freyja_rc_map_string(FREYJA_LOG_FILE);
	ControlPrinter::StartLogging(s.c_str());

	/* Hookup assert handlers, note freyja assert is also used by this layer */
	freyjaAssertHandler(FreyjaAssertCallbackHandler);
	mgtk_assert_handler(FreyjaAssertCallbackHandler);
	freyjaDebugInfoHandler(FreyjaDebugInfoCallbackHandler);

	/* Hookup resource to event system */
	ResourceEvent::setResource(&FreyjaControl::GetInstance()->GetResource());

	mgtk_init(argc, argv);

	/* Load file passed by command line args, CLI parser isn't really needed. */
	if (argc > 1 && argv[1][0] != '-')
	{
		FreyjaControl::mInstance->LoadModel(argv[1]);
	}

	mgtk_start();

	return 0;
}





