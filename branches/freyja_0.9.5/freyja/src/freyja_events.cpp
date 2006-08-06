/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/mongoose/
 * Email   : mongoose@icculus.org
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
 * 2001.06.26:
 * Mongoose - Created
 ==========================================================================*/

#define FREYJA_APP_PLUGINS

#ifdef FREYJA_APP_PLUGINS
#   include <freyja/FreyjaPluginABI.h>
#endif

#include <mgtk/mgtk_events.h>
#include <mgtk/ResourceEvent.h>
#include <freyja/FreyjaPlugin.h>
#include <freyja/FreyjaImage.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>

#include "FreyjaRender.h"
#include "FreyjaControl.h"
#include "freyja_events.h"


arg_list_t *freyja_rc_color(arg_list_t *args);


Resource gResource;
int gSkelTreeWidgetIndex;


void eRecentFiles(unsigned int value)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	FreyjaControl::mInstance->LoadModel(FreyjaControl::mInstance->GetRecentFilename(value));
}


//extern void mgtk_event_update_tree(unsigned int id, mgtk_tree_t *tree);

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
	const char *rootName = freyjaGetBoneNameString(rootIndex);
	uint32 rootSkelBID = freyjaGetBoneSkeletalBoneIndex(rootIndex);

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


void UpdateSkeletonUI_Callback(uint32 skelIndex)
{
	mgtk_tree_t *tree;

	tree = generateSkeletalUI(skelIndex, 
							  freyjaGetSkeletonRootIndex(skelIndex), 0x0);

	mgtk_event_update_tree(gSkelTreeWidgetIndex, tree);
}

int freyja_get_event_id_by_name(char *symbol)
{
	int id = -1;

	gResource.Lookup(symbol, &id);

	return id;
}


void setColor(vec4_t dest, vec4_t color)
{
	dest[0] = color[0];	
	dest[1] = color[1];	
	dest[2] = color[2];	
	dest[3] = color[3];
}


void eVertexExtrude()
{
	extern int freyjaVertexExtrude(int32 vertexIndex, vec_t midpointScale, vec3_t normal);
	//vec3_t n;

	//freyjaGetVertexNormalXYZ3fv(mModel->getCurrentVertex(), n);
	//freyjaVertexExtrude(mModel->getCurrentVertex(), 0.5f, n);
}

void eNoImplementation(ResourceEvent *e)
{
	freyja_print("!'%s' : No longer implemented or disabled.",
				(e && e->getName()) ? e->getName() : "Unknown event");
}


void freyja_plugin_generic(const char *symbol, void *something)
{
	// 1. look for symbol in the 'hack bind list'
	// 2. return what you found via the void pointer

	if (strncmp(symbol, "freyja_load_texture_buffer", 25) == 0)
	{
		something = (void *)freyja_load_texture_buffer;
	}

	something = 0x0;
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


void freyja_init_application_plugins(const char *dir)
{
#ifdef FREYJA_APP_PLUGINS
	FreyjaFileReader reader;
	char module_name[128];
	char module_symbol[128];
	void (*init)(void (*func)(const char*, void*));
	bool done = false;
	char *module_filename;
	void *handle;


	freyja_print("![Freyja application plugin system invoked]");

	if (!reader.openDirectory(dir))
	{
		freyja_print("!Couldn't access application plugin directory.");
		return;
	}

	while (!done && (module_filename = reader.getNextDirectoryListing()))
	{
		if (reader.isDirectory(module_filename))
			continue;

		// FIXME: Add check here for SO check

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

	reader.closeDirectory();

	freyja_print("![Freyja application plugin loader sleeps now]\n");

#else
	freyja_print("FreyjaAppPlugin: This build was compiled w/o plugin support");
#endif
}

void freyja_handle_resource_start();
void freyja_callback_get_image_data_rgb24(const char *filename, 
										unsigned char **image, 
										int *width, int *height);


void freyja_handle_application_window_close()
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	FreyjaControl::mInstance->event(eShutdown);
}


void freyja_handle_color(int id, float r, float g, float b, float a)
{
	vec4_t color;

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;


	/* Color event listener */
	if (ResourceEvent::listen(id - 10000 /*ePluginEventBase*/, color, 4))
		return; // true;

	switch (id)
	{
	case eColorMaterialAmbient: 
		freyjaMaterialAmbient(freyjaGetCurrentMaterial(), color);
		freyja_refresh_material_interface(); // FIXME HACK
		break;

	case eColorMaterialDiffuse:
		freyjaMaterialDiffuse(freyjaGetCurrentMaterial(), color);
		freyja_refresh_material_interface(); // FIXME HACK
		break;

	case eColorMaterialSpecular:
		freyjaMaterialSpecular(freyjaGetCurrentMaterial(), color);
		freyja_refresh_material_interface(); // FIXME HACK
		break;

	case eColorMaterialEmissive:
		freyjaMaterialEmissive(freyjaGetCurrentMaterial(), color);
		freyja_refresh_material_interface(); // FIXME HACK
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
		setColor(FreyjaRender::mColorBackground, color);
		freyja_event_set_color(eColorBackground, r, g, b, a);
		break;

	case eColorGrid:
		setColor(FreyjaRender::mColorGridSeperator, color);
		setColor(FreyjaRender::mColorGridLine, color);
		freyja_event_set_color(eColorGrid, r, g, b, a);
		break;

	case eColorVertex:
		setColor(FreyjaRender::mColorVertex, color);
		freyja_event_set_color(eColorVertex, r, g, b, a);
		break;

	case eColorVertexHighlight:
		setColor(FreyjaRender::mColorVertexHighlight, color);
		freyja_event_set_color(eColorVertexHighlight, r, g, b, a);
		break;

	case eColorMesh:
		setColor(FreyjaRender::mColorWireframe, color);
		freyja_event_set_color(eColorMesh, r, g, b, a);
		break;

	case eColorMeshHighlight:
		setColor(FreyjaRender::mColorWireframeHighlight, color);
		freyja_event_set_color(eColorMeshHighlight, r, g, b, a);
		break;

	default:
		return;
	}

	mgtk_event_gl_refresh();
}



///////////////////////////////////////////////////////////////////////
// MGtk wrappers
///////////////////////////////////////////////////////////////////////

#if defined (WIN32) || (MACOSX)
#else
void mgtk_print(char *format, ...)
{
	va_list args;

	va_start(args, format);
	freyja_print_args(format, &args);
	va_end(args);
}


void mgtk_callback_get_image_data_rgb24(const char *filename, 
										unsigned char **image, 
										int *width, int *height)
{
	freyja_callback_get_image_data_rgb24(filename, image, width, height);
}


void mgtk_update_tree(unsigned int id, mgtk_tree_t *tree)
{
	freyja_print("FIXME: mgtk_update_tree() not in libmgtk yet");
}


void mgtk_handle_application_window_close()
{
	freyja_handle_application_window_close();
}


void mgtk_handle_color(int id, float r, float g, float b, float a)
{
	freyja_handle_color(id, r, g, b, a);
}


void mgtk_handle_command(int command)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated, Event %i not handled", command);
	FreyjaControl::mInstance->event(command);
}


void mgtk_handle_command2i(int event, int command)
{
	freyja_event2i(event, command);
}


void mgtk_handle_event1u(int event, unsigned int value)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	if (!FreyjaControl::mInstance->event(event, value))
	{
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("  mgtk_handle_event1u spawned previous unhandled event %i:%i", eEvent, event);
	}
}


void mgtk_handle_event1f(int event, float value)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	if (!FreyjaControl::mInstance->event(event, value))
	{
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("   mgtk_handle_event1f spawned previous unhandled event %i:%i", eEvent, event);
	}
}


void mgtk_handle_file_dialog_selection(char *filename)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	FreyjaControl::mInstance->handleFilename(filename);
}


void mgtk_handle_gldisplay()
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	//if (FreyjaControl::mInstance)
		FreyjaControl::mInstance->Display();
}


void mgtk_handle_glresize(unsigned int width, unsigned int height)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	//if (FreyjaControl::mInstance)
		FreyjaControl::mInstance->HandleResize(width, height);
}


void mgtk_handle_key_press(int key, int mod)
{
	freyja_print("mgtk_handle_key_press(%d, %d) not handled", key, mod);
}


void mgtk_handle_resource_start()
{
	freyja_handle_resource_start();
}


void mgtk_handle_slider1u(int event, unsigned int value)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	FreyjaControl::mInstance->event(event, value);
}


void mgtk_handle_text(int event, char *text)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	//if (FreyjaControl::mInstance)
		FreyjaControl::mInstance->handleTextEvent(event, text);
}


void mgtk_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
	freyja_get_pixmap_filename(dest, size, icon_name);
}


char *mgtk_rc_map(char *filename_or_dirname)
{
	return freyja_rc_map(filename_or_dirname);
}


void freyja_handle_motion(int x, int y);
void mgtk_handle_motion(int x, int y)
{
	freyja_handle_motion(x, y);
}


void freyja_handle_mouse(int button, int state, int mod, int x, int y);
void mgtk_handle_mouse(int button, int state, int mod, int x, int y)
{
	freyja_handle_mouse(button, state, mod, x, y);
}

#endif


void mgtk_event_gldisplay()
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	//if (FreyjaControl::mInstance)
		FreyjaControl::mInstance->Display();
}


void mgtk_event_glresize(unsigned int width, unsigned int height)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	//if (FreyjaControl::mInstance)
		FreyjaControl::mInstance->HandleResize(width, height);
}


///////////////////////////////////////////////////////////////////////
// Freyja wrappers
///////////////////////////////////////////////////////////////////////

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
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	if (!FreyjaControl::mInstance->event(event, value))
	{
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("  mgtk_handle_event1u spawned previous unhandled event %i:%i", eEvent, event);
	}
}


void freyja_handle_event1f(int event, float value)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	if (!FreyjaControl::mInstance->event(event, value))
	{
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("   mgtk_handle_event1f spawned previous unhandled event %i:%i", eEvent, event);
	}
}


void freyja_handle_file_dialog_selection(char *filename)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	FreyjaControl::mInstance->handleFilename(filename);
}


void freyja_handle_gldisplay()
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl Singleton not allocated");
	FreyjaControl::mInstance->Display();
}


void freyja_handle_glresize(unsigned int width, unsigned int height)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl Singleton not allocated");
	FreyjaControl::mInstance->HandleResize(width, height);
}


void freyja_handle_slider1u(int event, unsigned int value)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	FreyjaControl::mInstance->event(event, value);
}


void freyja_handle_text(int event, char *text)
{
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	//if (FreyjaControl::mInstance)
		FreyjaControl::mInstance->handleTextEvent(event, text);
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
	ASSERT_MSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");
	FreyjaControl::mInstance->event(command);
}


void eModelGenerateNormals()
{
	freyjaModelGenerateVertexNormals(0); // only 1 model atm
}


void freyja_handle_resource_init(Resource &r)
{
	////////////////////////////////////////////////////////////////////
	// New freyja events
	////////////////////////////////////////////////////////////////////

	ResourceEventCallbackUInt::add("eRecentFiles", &eRecentFiles);
	ResourceEventCallback::add("eGenerateNormals", &eModelGenerateNormals);
	ResourceEventCallback2::add("eAnimationStop", &eNoImplementation);
	ResourceEventCallback2::add("eAnimationPlay", &eNoImplementation);
	//ResourceEventCallback2::add("eUndo", &eNoImplementation);
	ResourceEventCallback2::add("eRedo", &eNoImplementation);
	ResourceEventCallback2::add("eSkeletalDeform", &eNoImplementation);

	FreyjaViewEventsAttach();
	FreyjaControlEventsAttach();


	////////////////////////////////////////////////////////////////////
	// Old style events
	////////////////////////////////////////////////////////////////////

	// Event types used for flow control of event ids
	r.RegisterInt("eMode", eMode);
	r.RegisterInt("eEvent", eEvent);


	// Event ids

	/* Mongoose 2002.01.12, 
	 * Bind script functions to C/C++ functions */
	r.RegisterFunction("color", freyja_rc_color);

	/* Mongoose 2002.01.21, 
	 * Bind some script vars to matching name in C/C++ */
	r.RegisterInt("eNone", eNone);

	r.RegisterInt("eCopyAppendMode", eCopyAppendMode);

	// Edit actions and modes
	r.RegisterInt("eUndo", eUndo);
	r.RegisterInt("eCut", eCut);
	r.RegisterInt("eCopy", eCopy);
	r.RegisterInt("ePaste", ePaste);
	r.RegisterInt("eDelete", eDelete);
	r.RegisterInt("eSelect", eSelect);
	r.RegisterInt("eUnselect", eUnselect);
	r.RegisterInt("eSelectAll", eSelectAll);


	r.RegisterInt("eSplitObject", eSplitObject);
	r.RegisterInt("eAppendFile", eAppendFile);
	r.RegisterInt("eRevertFile", eRevertFile);
	r.RegisterInt("eExportFile", eExportFile);
	r.RegisterInt("eImportFile", eImportFile);
	r.RegisterInt("eCloseFile", eCloseFile);
	r.RegisterInt("eOpenFileTexture", eOpenFileTexture);
	r.RegisterInt("eOpenFileModel", eOpenFileModel);

	r.RegisterInt("ePluginMenu", ePluginMenu);  /* MenuItem Widget attach */


	// dialogs
	r.RegisterInt("ePreferencesDialog", ePreferencesDialog);
	r.RegisterInt("eAboutDialog", eAboutDialog);
	r.RegisterInt("eHelpDialog", eHelpDialog);


	r.RegisterInt("eMaterialMultiTex", eMaterialMultiTex);
	r.RegisterInt("eMaterialTex", eMaterialTex);


	r.RegisterInt("eOpenFileModel", eOpenFileModel);
	r.RegisterInt("eSaveFileModel", eSaveFileModel);
	r.RegisterInt("eSaveAsFileModel", eSaveAsFileModel);

	r.RegisterInt("eScreenShot", eScreenShot);
	r.RegisterInt("eShutdown", eShutdown);
	r.RegisterInt("eNewFile", eNewFile);
	r.RegisterInt("eOpenFile", eOpenFile);
	r.RegisterInt("eSaveFile", eSaveFile);
	r.RegisterInt("eSaveAsFile", eSaveAsFile);
	r.RegisterInt("eInfo", eInfo);
	r.RegisterInt("eDebug", eDebug);
	r.RegisterInt("eFullscreen", eFullscreen);

	r.RegisterInt("eGenerateCube", eGenerateCube);
	r.RegisterInt("eGenerateTube", eGenerateTube);
	r.RegisterInt("eGenerateCircle", eGenerateCircle);
	r.RegisterInt("eGeneratePlane", eGeneratePlane);
	r.RegisterInt("eGenerateCylinder", eGenerateCylinder);
	r.RegisterInt("eGenerateSphere", eGenerateSphere);
	r.RegisterInt("eGenerateCone", eGenerateCone);

	r.RegisterInt("ePolygonSplit", ePolygonSplit);

	r.RegisterInt("eMeshNew", eMeshNew);
	r.RegisterInt("eMeshDelete", eMeshDelete);
	r.RegisterInt("eMeshSelect", eMeshSelect);
	r.RegisterInt("eMeshMirrorX", eMeshMirrorX);
	r.RegisterInt("eMeshMirrorY", eMeshMirrorY);
	r.RegisterInt("eMeshMirrorZ", eMeshMirrorZ);
	r.RegisterInt("eMeshTexcoordSpherical",eMeshTexcoordSpherical);
	r.RegisterInt("eMeshTexcoordCylindrical", eMeshTexcoordCylindrical);
	r.RegisterInt("eMeshTesselate", eMeshTesselate);

	r.RegisterInt("ePolygonSize", ePolygonSize);
	r.RegisterInt("eGenMeshHeight", eGenMeshHeight);
	r.RegisterInt("eGenMeshCount", eGenMeshCount);
	r.RegisterInt("eGenMeshSegements", eGenMeshSegements);
	r.RegisterInt("eRenderBbox", eRenderBbox);

	r.RegisterInt("eSetMaterialTexture", eSetMaterialTexture);
	r.RegisterInt("eTmpUVMapOff",eTmpUVMapOff);
	r.RegisterInt("eTmpUVMapOn",eTmpUVMapOn);
	r.RegisterInt("eMirrorUV_X", eMirrorUV_X);
	r.RegisterInt("eMirrorUV_Y", eMirrorUV_Y);
	r.RegisterInt("eTranslateUV", eTranslateUV);
	r.RegisterInt("eRotateUV", eRotateUV);
	r.RegisterInt("eScaleUV", eScaleUV);
	r.RegisterInt("eSetMeshTexture", eSetMeshTexture);
	r.RegisterInt("eSetPolygonTexture", eSetPolygonTexture);
	r.RegisterInt("eTextureSlotLoad", eTextureSlotLoad);


	r.RegisterInt("eMeshGenerateNormals", eMeshGenerateNormals);
	r.RegisterInt("eMeshFlipNormals", eMeshFlipNormals);
	r.RegisterInt("eObjectMenu", eObjectMenu);
	r.RegisterInt("eAddObject", eAddObject);
	r.RegisterInt("eMoveObject", eMoveObject);
	r.RegisterInt("eRotateObject", eRotateObject);
	r.RegisterInt("eScaleObject", eScaleObject);

	r.RegisterInt("eExtrude", eExtrude);

	r.RegisterInt("eTransformMenu", eTransformMenu);
	r.RegisterInt("eTransformSelectedVertices", eTransformSelectedVertices);
	r.RegisterInt("eTransformGroup", eTransformGroup);
	r.RegisterInt("eTransformScene", eTransformScene);
	r.RegisterInt("eTransformFace", eTransformFace);
	r.RegisterInt("eTransformMesh", eTransformMesh);
	r.RegisterInt("eTransformMeshPivot", eTransformMeshPivot);
	r.RegisterInt("eTransformBone", eTransformBone);
	r.RegisterInt("eTransformPoint", eTransformPoint);

	r.RegisterInt("eSetFacesMaterial", eSetFacesMaterial);

	r.RegisterInt("eAnimationNext", eAnimationNext);
	r.RegisterInt("eAnimationPrev", eAnimationPrev);
	r.RegisterInt("eAnimationSlider", eAnimationSlider);

	/* Widget events ( widgets hold data like spinbuttons, etc ) */
	r.RegisterInt("eScale", eScale);
	r.RegisterInt("eScale_X", eScale_X);
	r.RegisterInt("eScale_Y", eScale_Y);
	r.RegisterInt("eScale_Z", eScale_Z);
	r.RegisterInt("eMove", eMove);
	r.RegisterInt("eMove_X", eMove_X);
	r.RegisterInt("eMove_Y", eMove_Y);
	r.RegisterInt("eMove_Z", eMove_Z);
	r.RegisterInt("eRotate", eRotate);
	r.RegisterInt("eRotate_X", eRotate_X);
	r.RegisterInt("eRotate_Y", eRotate_Y);
	r.RegisterInt("eRotate_Z", eRotate_Z);
	r.RegisterInt("eZoom", eZoom);
	r.RegisterInt("eCamera", eCamera);
	r.RegisterInt("eSelectMaterial", eSelectMaterial);

	r.RegisterInt("eModelIterator", eModelIterator);
	r.RegisterInt("ePolygonIterator", ePolygonIterator);
	r.RegisterInt("eMeshIterator", eMeshIterator);
	r.RegisterInt("eGroupIterator", eGroupIterator);
	r.RegisterInt("eBoneIterator", eBoneIterator);

	r.RegisterInt("eTexcoordCombine", eTexcoordCombine);
	r.RegisterInt("CMD_MISC_GEN_TEXMAP_XY", CMD_MISC_GEN_TEXMAP_XY);

	r.RegisterInt("eSetTextureNameA", eSetTextureNameA);
	r.RegisterInt("eSetTextureNameB", eSetTextureNameB);
	r.RegisterInt("eSetMaterialName", eSetMaterialName);

	r.RegisterInt("eZoomIn", eZoomIn);
	r.RegisterInt("eZoomOut", eZoomOut);
	r.RegisterInt("eSelectLight", eSelectLight);

	r.RegisterInt("ePolyMapTexturePolygon", ePolyMapTexturePolygon);

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

	r.RegisterInt("eViewports", eViewports);

	r.RegisterInt("eVertexNew", CMD_POINT_ADD);
	//r.RegisterInt("eVertexMove", CMD_POINT_MOVE);
	r.RegisterInt("eVertexCombine", CMD_POINT_COMBINE);
	r.RegisterInt("eVertexDelete", CMD_POINT_DELETE);
	r.RegisterInt("ePolygonNew", CMD_POLYGON_ADD);	
	r.RegisterInt("ePolygonDelete", CMD_POLYGON_DELETE);	
	r.RegisterInt("ePolygonSelect", CMD_POLYGON_SELECT);	

	/* Bone edit */
	r.RegisterInt("eSetCurrentBoneName", eSetCurrentBoneName); // textbox
	r.RegisterInt("eBoneNew", CMD_BONE_NEW);
	r.RegisterInt("eBoneSelect", CMD_BONE_SELECT);
	r.RegisterInt("eBoneLinkChild", CMD_BONE_CONNECT);
	r.RegisterInt("eBoneUnLinkChild", CMD_BONE_DISCONNECT);
	r.RegisterInt("eBoneLinkMesh", CMD_BONE_ADD_MESH);
	r.RegisterInt("eBoneUnLinkMesh", CMD_BONE_DELETE_MESH);
	r.RegisterInt("eBoneMoveJoint", CMD_BONE_MOVE_PIVOT);


	r.RegisterInt("eSelectionByBox", CMD_MISC_BBOX_SELECT);
	r.RegisterInt("eTextureMapProjection", CMD_MISC_GEN_TEXMAP_XY);
	r.RegisterInt("eModeModel", FREYJA_MODE_MODEL_EDIT);
	r.RegisterInt("eModeMaterial", FREYJA_MODE_MATERIAL_EDIT);
	r.RegisterInt("eModeSkin", FREYJA_MODE_TEXTURE_EDIT);
	r.RegisterInt("eModeAnimation", FREYJA_MODE_MODEL_VIEW);

	r.RegisterInt("eGeneratePatchMesh", eGeneratePatchMesh);

	r.RegisterInt("eRenderPatch", eRenderPatch);
	r.RegisterInt("eRenderWireframe",FREYJA_MODE_RENDER_WIREFRAME);
	r.RegisterInt("eRenderFace", FREYJA_MODE_RENDER_FACE);
	r.RegisterInt("eRenderVertex", FREYJA_MODE_RENDER_POINTS);
	r.RegisterInt("eRenderNormals", FREYJA_MODE_RENDER_NORMALS);
	r.RegisterInt("eRenderTexture", FREYJA_MODE_RENDER_TEXTURE);
	r.RegisterInt("eRenderLighting", FREYJA_MODE_RENDER_LIGHTING);
	r.RegisterInt("eRenderMaterial", FREYJA_MODE_RENDER_MATERIAL);
	r.RegisterInt("eRenderSkeleton",FREYJA_MODE_RENDER_BONETAG);
	r.RegisterInt("eRenderGrid", FREYJA_MODE_RENDER_GRID);

	r.RegisterInt("eOpenGLNormalize", eOpenGLNormalize);
	r.RegisterInt("eOpenGLBlend", eOpenGLBlend);

	r.RegisterInt("FREYJA_MODE_PLANE_XY", FREYJA_MODE_PLANE_XY);
	r.RegisterInt("FREYJA_MODE_PLANE_YZ", FREYJA_MODE_PLANE_YZ);
	r.RegisterInt("FREYJA_MODE_PLANE_XZ", FREYJA_MODE_PLANE_XZ);
	r.RegisterInt("FREYJA_MODE_PLANE_FREE", FREYJA_MODE_PLANE_FREE);

	r.RegisterInt("FREYJA_MODE_AXIS_X", FREYJA_MODE_AXIS_X);
	r.RegisterInt("FREYJA_MODE_AXIS_Y", FREYJA_MODE_AXIS_Y);
	r.RegisterInt("FREYJA_MODE_AXIS_Z", FREYJA_MODE_AXIS_Z);

	//r.RegisterSymbol();

	/* Load and init plugins */
	char *dir = freyja_rc_map("plugins");	
	freyja_init_application_plugins(dir);
	delete [] dir;

	/* Hook plugins to resource */
	uint32 i, n = ResourceAppPluginTest::mPlugins.size();

	for (i = 0; i < n; ++i)
	{
		if (ResourceAppPluginTest::mPlugins[i] != 0x0)
		{
			ResourceAppPluginTest::mPlugins[i]->mEventsAttach();
		}
	}
}


void freyja_handle_resource_start()
{
	/* Mongoose 2002.02.02, This is the backend entry
	 *   for some damn reason it's started by the fucking
	 *   widget layer 'woo hoo'
	 *
	 *   Also needs no parms really
	 *
	 *   TODO, FIXME: Rewrite all code to focus on this file
	 *                as main entry and start interface from
	 *                here, most likey using resource for
	 *                total gui control */

	freyja_print("@Freyja started...");

	freyja_handle_resource_init(gResource);

	/* User install of icons, samples, configs, etc */
	if (!freyja_is_user_installed())
		freyja_install_user();

	FreyjaControl *startInstance = new FreyjaControl(&gResource);

	// This is done when FreyjaControl is allocated now
	/* Build the user interface from lisp, and load user preferences */
	//FreyjaControl::mInstance->loadResource();

	/* FreyjaAppPlugin prototype testing... */
	uint32 i, n = ResourceAppPluginTest::mPlugins.size();

	for (i = 0; i < n; ++i)
	{
		if (ResourceAppPluginTest::mPlugins[i] != 0x0)
		{
			ResourceAppPluginTest::mPlugins[i]->mGUIAttach();
		}
	}

	/* Setup material interface */
	freyja_refresh_material_interface();

	/* Setup editor modes and drop-down menus */
	mgtk_option_menu_value_set(eTransformMenu, 1);
	mgtk_option_menu_value_set(eObjectMenu, 0);
	freyja_event2i(eEvent, FREYJA_MODE_MODEL_EDIT);
	FreyjaControl::mInstance->event(eTransformMesh);
	FreyjaControl::mInstance->event(eSelect);

	freyja_set_main_window_title(BUILD_NAME);
	mgtk_event_gl_refresh();

	freyja_print("Welcome to Freyja %s-%s, %s", VERSION, BUILD_ID, __DATE__);

	/* Mongoose 2002.02.23, Hook for exit() calls */
	atexit(freyja_event_shutdown);
}

long PLUGIN_EVENT_COUNTER = ePluginEventBase;

long freyja_get_new_plugin_eventid()
{
	return PLUGIN_EVENT_COUNTER++;
}


void freyja_append_eventid(char *symbol, int eventid)
{
	gResource.RegisterInt(symbol, eventid);
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
	len = strlen("C:/freyja/");

	if (sz < len)
		return;
	
	snprintf(s, len, "C:/freyja/");
#endif
}


void freyja_get_share_path(char *s, long sz)
{
	if (sz < 64)
		return;

#ifdef unix
	snprintf(s, strlen("/usr/share/freyja/"), "/usr/share/freyja/");
#else
	snprintf(s, strlen("C:/freyja/"), "C:/freyja/");
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
	len = strlen("C:/freyja/") + strlen(filename);

	if (sz < len)
		return;
	
	snprintf(s, len, "C:/freyja/%s", filename);
#endif
}


int freyja_append_item_to_menu(int event, const char *label, int item_event)
{
	return mgtk_append_item_to_menu(event, label, item_event);
}


int freyja_remove_all_items_to_menu(int event)
{
	return mgtk_remove_all_items_to_menu(event);
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
	len = strlen("C:/freyja/") + strlen(filename);

	if (sz < len)
		return;
	
	snprintf(s, len, "C:/freyja/%s", filename);
#endif
}


char freyja_is_user_installed()
{
	FreyjaFileReader r;
	char path[128];

	freyja_get_rc_path(path, 128);

#ifdef WIN32
	return 1;
#endif

	return (r.doesFileExist(path));
}


void freyja_install_user()
{
	FreyjaFileReader r;
	FreyjaFileWriter w;
	char rc[512];
	char share[512];
	char *filename;


	/* Copy top level rc files */
	freyja_get_rc_path(rc, 512);
	freyja_get_share_path(share, 512);
	FreyjaFileWriter::createDirectory(rc);

	if (r.openDirectory(share))
	{
		while ((filename = r.getNextDirectoryListing()))
		{
			if (r.isDirectory(filename))
				continue;

			w.copyFileToPath(filename, rc);
		}
	}


	/* Copy icon files */
	freyja_get_rc_filename(rc, "icons/", 512);
	freyja_get_share_filename(share, "icons/", 512);
	FreyjaFileWriter::createDirectory(rc);

	if (r.openDirectory(share))
	{
		while ((filename = r.getNextDirectoryListing()))
		{
			if (r.isDirectory(filename))
				continue;

			w.copyFileToPath(filename, rc);
		}
	}


	/* Copy material files */
	freyja_get_rc_filename(rc, "materials/", 512);
	freyja_get_share_filename(share, "materials/", 512);
	FreyjaFileWriter::createDirectory(rc);

	if (r.openDirectory(share))
	{
		while ((filename = r.getNextDirectoryListing()))
		{
			if (r.isDirectory(filename))
				continue;

			w.copyFileToPath(filename, rc);
		}
	}


	/* Copy palettes files */
	freyja_get_rc_filename(rc, "palettes/", 512);
	freyja_get_share_filename(share, "palettes/", 512);
	FreyjaFileWriter::createDirectory(rc);

	if (r.openDirectory(share))
	{
		while ((filename = r.getNextDirectoryListing()))
		{
			if (r.isDirectory(filename))
				continue;

			w.copyFileToPath(filename, rc);
		}
	}


	/* Misc */
	freyja_get_rc_filename(rc, "models/", 512);
	FreyjaFileWriter::createDirectory(rc);

	freyja_get_rc_filename(rc, "animations/", 512);
	FreyjaFileWriter::createDirectory(rc);

	freyja_get_rc_filename(rc, "textures/", 512);
	FreyjaFileWriter::createDirectory(rc);

	//freyja_get_rc_filename(rc, "particles/", 512);
	//FreyjaFileWriter::createDirectory(rc);

	/* Copy plugins */
	freyja_get_rc_filename(rc, "plugins/", 512);
	freyja_get_share_filename(share, "plugins/", 512);
	FreyjaFileWriter::createDirectory(rc);

	if (r.openDirectory(share))
	{
		while ((filename = r.getNextDirectoryListing()))
		{
			if (r.isDirectory(filename))
				continue;

			w.copyFileToPath(filename, rc);
		}
	}
}


void freyja_event_file_dialog(char *s)
{
	static bool on = 0;


	mgtk_event_file_dialog(s);

	if (!on)
	{
		extern void mgtk_add_menu_item(char *text, long event);
		long i, count = freyjaGetPluginCount();

		//mgtk_add_menu_item("All Files (*.*)", 9000);
		mgtk_event_fileselection_append_pattern("All Files (*.*)", "*.*");

		for (i = 0; i < count; ++i)
		{
			FreyjaPluginDesc *plugin = freyjaGetPluginClassByIndex(i);
			
			if (plugin && plugin->mImportFlags)
				//mgtk_add_menu_item(plugin->mDescription, 9001+i);
				mgtk_event_fileselection_append_pattern(plugin->mDescription,
														plugin->mExtention);
		}

		on = 1;
	}
}


void freyja_application_window_move(int x, int y)
{
	mgtk_application_window_move(x, y);
}


void freyja_event_fullscreen()
{
	mgtk_application_window_fullscreen();
}


void freyja_event_unfullscreen()
{
	mgtk_application_window_unfullscreen();
}


void freyja_event_exit()
{
	mgtk_event_shutdown();
}

void freyja_event_gl_refresh()
{
	mgtk_event_gl_refresh();
}


void freyja_event_fileselection_append_pattern(char *label, char *pattern)
{
	mgtk_event_fileselection_append_pattern(label, pattern);
}


void freyja_event_info_dialog(char *icon, char *message)
{
	mgtk_create_info_dialog(icon, message);
}


void freyja_handle_motion(int x, int y)
{
	if (FreyjaControl::mInstance)
	{
		FreyjaControl::mInstance->motionEvent(x, y);
	}
}


void freyja_handle_mouse(int button, int state, int mod, int x, int y)
{
	if (FreyjaControl::mInstance)
	{
		FreyjaControl::mInstance->mouseEvent(button, state, mod, x, y);
	}
}


void freyja_event_set_color(int colorId, float r, float g, float b, float a)
{
	mgtk_event_set_color(colorId, r, g, b, a);
}


int freyja_event2i(int event, int cmd)
{
	if (FreyjaControl::mInstance)
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


FILE *get_log_file()
{
	static FILE *f = 0x0;
	char *s;

	if (!f)
	{
#ifdef LOG_CHECK_AND_WRITE
		unsigned int i;
		char filename[128];

		for (i = 0; i < 256; ++i)
		{
			snprintf(filename, 128, "/tmp/Freyja%i.log", i);

			f = fopen(filename, "rb");

			if (!f)
				break;
		}

		f = fopen(filename, "w");
#else
		s = freyja_rc_map("Freyja-chimera.log");
		f = fopen(s, "w");
#endif
	}

	return f;
}


void freyja_swap_buffers()
{
	mgtk_event_swap_gl_buffers();
}


void close_log_file()
{
	if (get_log_file())
		fclose(get_log_file());
}


void freyja_print_args(char *format, va_list *args)
{
	FILE *f = get_log_file();
	char buffer[1024];
	unsigned int l;


	// Strip message of an trailing carrage return 
	//  and print to stdout and the status bar
	vsnprintf(buffer, 1023, format, *args);
	buffer[1023] = 0;

	l = strlen(buffer);
  
	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

#ifdef DEBUG_EVENT_PRINT
	fprintf(stdout, "DEBUG> %s\n", buffer);
#endif

	if (buffer[0] == '!')
	{
		buffer[0] = ' ';
		fprintf(stderr, "%s\n", buffer);
	}

	mgtk_event_notify_statusbar(buffer);

	if (f)
	{
		fprintf(f, "> ");
		fprintf( f, buffer );
		fprintf(f, "\n");
		fflush(f);
	}
}


void freyja_print(char *format, ...)
{
	va_list args;

	va_start(args, format);
	freyja_print_args(format, &args);
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

	close_log_file();
}


String freyja_rc_map_string(const char *s)
{
	char *cs = freyja_rc_map((char *)s);
	String str(cs);

	if (cs) 
		delete [] cs;

	return str;
}


char *freyja_rc_map(char *s)
{
	char *rc = NULL;
	char *path = "freyja";
	unsigned int len;
	
	
	if (!s || !s[0])
	{
		return NULL;
	}

	len = strlen(s) + strlen(path);
  
#if defined unix || MACOSX
	char *env;

	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
		return NULL;
	}

	len += strlen(env) + 8;

   rc = new char[len + 1];
   snprintf(rc, len, "%s/.%s/%s", env, path, s);
#else
	len += 8;

   rc = new char[len + 1];
   snprintf(rc, len, "C:/%s/%s", path, s);
#endif
	
	return rc;
}


void freyja_event_notify_observer1f(event_subject_id e, float value)
{
	mgtk_event_notify_observer1f(e, value);
}


void freyja_event_set_float(int event, float value)
{
	mgtk_spinbutton_value_set(event, value);
}


int freyja_create_confirm_dialog(char *dialog_icon,
								 char *information_message, 
								 char *question_message,
								 char *cancel_icon, char *cancel_text,
								 char *accept_icon, char *accept_text)
{
	return mgtk_create_confirm_dialog(dialog_icon,
									  information_message, 
									  question_message,
									  cancel_icon, cancel_text,
									  accept_icon, accept_text);
}


void freyja_event_file_dialog_notify(char *filename)
{
	if (FreyjaControl::mInstance)
		FreyjaControl::mInstance->handleFilename(filename);
}


void freyja_set_main_window_title(char *title)
{
	mgtk_application_window_title(title);
}


void freyja_refresh_material_interface()
{
	vec4_t ambient, diffuse, specular, emissive;
	uint32 src, dest, flags;
	vec_t shininess;
	uint32 mIndex = freyjaGetCurrentMaterial();


	freyjaGetMaterialAmbient(mIndex, ambient);
	freyjaGetMaterialDiffuse(mIndex, diffuse);
	freyjaGetMaterialSpecular(mIndex, specular);
	freyjaGetMaterialEmissive(mIndex, emissive);
	shininess = freyjaGetMaterialShininess(mIndex);
	src = freyjaGetMaterialBlendSource(mIndex);
	dest = freyjaGetMaterialBlendDestination(mIndex);
	flags = freyjaGetMaterialFlags(mIndex);

	freyja_event_set_color(eColorMaterialAmbient, 
						   ambient[0], ambient[1], ambient[2], ambient[3]);
	freyja_event_set_color(eColorMaterialDiffuse, 
						   diffuse[0], diffuse[1], diffuse[2], diffuse[3]);
	freyja_event_set_color(eColorMaterialSpecular, 
						   specular[0], specular[1], specular[2], specular[3]);
	freyja_event_set_color(eColorMaterialEmissive, 
						   emissive[0], emissive[1], emissive[2], emissive[3]);

	mgtk_spinbutton_value_set(700, ambient[0]);	
	mgtk_spinbutton_value_set(701, ambient[1]);
	mgtk_spinbutton_value_set(702, ambient[2]);
	mgtk_spinbutton_value_set(703, ambient[3]);
	mgtk_spinbutton_value_set(704, diffuse[0]);
	mgtk_spinbutton_value_set(705, diffuse[1]);
	mgtk_spinbutton_value_set(706, diffuse[2]);
	mgtk_spinbutton_value_set(707, diffuse[3]);
	mgtk_spinbutton_value_set(708, specular[0]);
	mgtk_spinbutton_value_set(709, specular[1]);
	mgtk_spinbutton_value_set(710, specular[2]);
	mgtk_spinbutton_value_set(711, specular[3]);
	mgtk_spinbutton_value_set(712, emissive[0]);
	mgtk_spinbutton_value_set(713, emissive[1]);
	mgtk_spinbutton_value_set(714, emissive[2]);
	mgtk_spinbutton_value_set(715, emissive[3]);
	mgtk_spinbutton_value_set(716, shininess);
	mgtk_option_menu_value_set(780, src);
	mgtk_option_menu_value_set(781, dest);

	if (flags & fFreyjaMaterial_Blending)
	{
		mgtk_togglebutton_value_set(790, true);
	}
	else
	{
		mgtk_togglebutton_value_set(790, false);
	}

mgtk_togglebutton_value_set(eMaterialTex, false);
	if (flags & fFreyjaMaterial_Texture)
	{
		mgtk_togglebutton_value_set(eMaterialTex, true);
	}
	else
	{
		mgtk_togglebutton_value_set(eMaterialTex, false);
	}

	mgtk_textentry_value_set(799, freyjaGetMaterialName(mIndex));
	mgtk_spinbutton_value_set(eSetMaterialTexture,
                              freyjaGetMaterialTexture(mIndex));	
	mgtk_textentry_value_set(eSetTextureNameA,
                             freyjaGetMaterialTextureName(mIndex));

#ifdef UMBRA_LIGHTS_NOT_BACKPORTED
	freyja_event_set_color(eColorLightAmbient, 
						   OpenGLFreyjaScene::mMaterialLight.mAmbient[0],
						   OpenGLFreyjaScene::mMaterialLight.mAmbient[1],
						   OpenGLFreyjaScene::mMaterialLight.mAmbient[2],
						   OpenGLFreyjaScene::mMaterialLight.mAmbient[3]);
	freyja_event_set_color(eColorLightDiffuse, 
						   OpenGLFreyjaScene::mMaterialLight.mDiffuse[0],
						   OpenGLFreyjaScene::mMaterialLight.mDiffuse[1],
						   OpenGLFreyjaScene::mMaterialLight.mDiffuse[2],
						   OpenGLFreyjaScene::mMaterialLight.mDiffuse[3]);
	freyja_event_set_color(eColorLightSpecular,
						   OpenGLFreyjaScene::mMaterialLight.mSpecular[0],
						   OpenGLFreyjaScene::mMaterialLight.mSpecular[1],
						   OpenGLFreyjaScene::mMaterialLight.mSpecular[2],
						   OpenGLFreyjaScene::mMaterialLight.mSpecular[3]);
#endif
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
	snprintf(dest, size, "C:/freyja/icons/%s", icon_name);
#endif

	dest[size-1] = 0;
}


int main(int argc, char *argv[])
{
// Link up mgtk DLL stubs to these implementations
#if defined (WIN32) || (MACOSX)
	mgtk_win32_import("win32_mgtk_callback_get_image_data_rgb24", (void*)freyja_callback_get_image_data_rgb24);
	mgtk_win32_import("win32_mgtk_handle_color", (void*)freyja_handle_color);
	mgtk_win32_import("win32_mgtk_handle_application_window_close", (void*)freyja_handle_application_window_close);
	mgtk_win32_import("win32_mgtk_handle_command", (void*)freyja_handle_command);
	mgtk_win32_import("win32_mgtk_handle_command2i", (void*)freyja_handle_command2i);
	mgtk_win32_import("win32_mgtk_handle_event1u", (void*)freyja_handle_event1u);
	mgtk_win32_import("win32_mgtk_handle_event1f", (void*)freyja_handle_event1f);
	mgtk_win32_import("win32_mgtk_handle_file_dialog_selection", (void*)freyja_handle_file_dialog_selection);
	mgtk_win32_import("win32_mgtk_handle_gldisplay", (void*)freyja_handle_gldisplay);
	mgtk_win32_import("win32_mgtk_handle_glresize", (void*)freyja_handle_glresize);
	mgtk_win32_import("win32_mgtk_handle_key_press", (void*)freyja_handle_key_press);
	mgtk_win32_import("win32_mgtk_handle_motion", (void*)freyja_handle_motion);
	mgtk_win32_import("win32_mgtk_handle_mouse", (void*)freyja_handle_mouse);
	mgtk_win32_import("win32_mgtk_handle_resource_start", (void*)freyja_handle_resource_start);
	mgtk_win32_import("win32_mgtk_handle_slider1u", (void*)freyja_handle_slider1u);
	mgtk_win32_import("win32_mgtk_handle_text", (void*)freyja_handle_text);
	mgtk_win32_import("win32_mgtk_print", (void*)freyja_print);
	mgtk_win32_import("win32_mgtk_get_pixmap_filename", (void*)freyja_get_pixmap_filename);
	mgtk_win32_import("win32_mgtk_rc_map", (void*)freyja_rc_map);
#endif

	/* Hookup resource to event system */
	ResourceEvent::setResource(&gResource);

	mgtk_init(argc, argv);

	/* Mongoose 2002.02.23, 
	 * Load file passed by command line args */
	if (argc > 1)
	{
		mgtk_event_fileselection_set_dir(argv[1]);
		mgtk_handle_file_dialog_selection(argv[1]);
	}

	mgtk_start();

	return 0;
}





