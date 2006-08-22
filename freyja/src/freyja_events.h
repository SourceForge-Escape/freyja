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

#ifndef GUARD__FREYJA_MONGOOSE_FREYJA_EVENT_H
#define GUARD__FREYJA_MONGOOSE_FREYJA_EVENT_H

#include <stdarg.h>
#include <mgtk/mgtk_keys.h>
#include <mstl/String.h>
#include <freyja/FreyjaPluginABI.h>


#define FREYJA_LOG_FILE "Freyja-dev.log"


typedef enum {

	// Special events
	eNone = 0,
	eShutdown = 1,

	// Event flow control modes ( majors )
	eMode,     /* These events change modes ( modify states ) */
	eEvent,    /* These events request handling ( perform actions ) */

	// Event ids ( minors )
	eNewFile,
	eOpenFile,
	eSaveFile,
	eSaveAsFile,
	eCloseFile,
	eSaveFileModel,
	eSaveAsFileModel,
	eOpenFileModel,
	eOpenFileTexture,
	eAppendFile,
	eRevertFile,
	eExportFile,
	eImportFile,
	ePluginMenu,

	eHelp,
	eInfo,
	eAbout,
	eDebugBackend,
	eFullscreen,
	eScreenShot,

	ePreferencesDialog,
	eAboutDialog,
	eHelpDialog,

	eMaterialMultiTex,
	eMaterialTex,

	ePolygonSize,
	eGenMeshHeight,
	eGenMeshCount,
	eGenMeshSegements,


	eGeneratePatchMesh,

	eMeshNew,
	eMeshDelete,
	eMeshMove,
	eMeshRotate,
	eMeshScale,
	eMeshTexcoordSpherical,
	eMeshTexcoordCylindrical,
	eMeshTesselate,
	eSetMaterialTexture,
	eTransformModel,
	eAnimationNext,
	eAnimationPrev,
	eAnimationSlider,
	eZoomIn,
	eZoomOut,
	eSelectLight,
	eOpenGLNormalize,
	eOpenGLBlend,
	eDebug,
	eGenerateCone,
	eMeshMirrorX,
	eMeshMirrorY,
	eMeshMirrorZ,
	eMeshSelect,

	ePolygonSplit,

	eCopyAppendMode,
	eSplitObject,

	eDelete,
	eSelect,
	eUnselect,
	eCopy,
	eCut,
	ePaste,
	eUndo,
	eRedo,
	eSelectAll,

	eAddObject,
	eMoveObject,
	eRotateObject,
	eScaleObject,

	eTransformSelectedVertices,
	eTransformGroup,
	eTransformScene,
	eTransformMesh,
	eTransformBone,
	eTransformFace,
	eTransformPoint,

	eRenderBbox,
	eCamera,
	
	eExtrude,

	eMeshGenerateNormals,

	eGenerateNormals,
	eGenerateCube,
	eGenerateTube,
	eGenerateCircle,
	eGeneratePlane,
	eGenerateCylinder,
	eGenerateSphere,

	eTexcoordCombine,

	eSetMeshTexture,
	eSetPolygonTexture,
	eMirrorUV_X,
	eMirrorUV_Y,
	eTranslateUV,
	eRotateUV,
	eScaleUV,
	eTmpUVMapOn,
	eTmpUVMapOff,
	eTextureSlotLoad,

	eViewports,

	eOpenTexture,
	eOpenTextureB,

	/* Widget interaction events */
	eZoom,
	eSelectMaterial,	
	eMove,
	eMove_X,
	eMove_Y,
	eMove_Z,
	eRotate,
	eRotate_X,
	eRotate_Y,
	eRotate_Z,
	eScale,
	eScale_X,
	eScale_Y,
	eScale_Z,
	eMoveBone_X,
	eMoveBone_Y,
	eMoveBone_Z,

	ePolyMapTexturePolygon,
	eSetFacesMaterial,

	eColorMaterialAmbient,
	eColorMaterialDiffuse,
	eColorMaterialSpecular,
	eColorMaterialEmissive,
	eColorLightAmbient,
	eColorLightDiffuse,
	eColorLightSpecular,
	eColorBackground,
	eColorGrid,
	eColorVertex,
	eColorVertexHighlight,
	eColorMesh,
	eColorMeshHighlight,

	eModelIterator,
	ePolygonIterator,
	eMeshIterator,
	eGroupIterator,
	eBoneIterator,

	eTransformMenu,
	eObjectMenu,

	eSkeletalDeform,

	/* Text events */
	eSetCurrentBoneName,
	eSetTextureNameA,
	eSetTextureNameB,
	eSetMaterialName,

	/* OLD EVENTS */
	FREYJA_MODE_MODEL_EDIT,
	FREYJA_MODE_MODEL_VIEW,
	FREYJA_MODE_TEXTURE_EDIT,
	FREYJA_MODE_ANIMATION,
	FREYJA_MODE_MODEL_VIEW2,
	FREYJA_MODE_PLANE_BACK,
	FREYJA_MODE_PLANE_RIGHT,
	FREYJA_MODE_PLANE_BOTTOM,
	FREYJA_MODE_PLANE_XY,
	FREYJA_MODE_PLANE_XZ,
	FREYJA_MODE_PLANE_YZ,
	FREYJA_MODE_PLANE_FREE,
	FREYJA_MODE_AXIS_X,
	FREYJA_MODE_AXIS_Y,
	FREYJA_MODE_AXIS_Z,

	FREYJA_MODE_RENDER_TEXTURE,
	FREYJA_MODE_RENDER_FACE,
	FREYJA_MODE_RENDER_LINELOOP,
	FREYJA_MODE_RENDER_WIREFRAME,
	FREYJA_MODE_RENDER_LIGHTING,
	FREYJA_MODE_RENDER_POINTS,
	FREYJA_MODE_RENDER_BLENDING,
	FREYJA_MODE_RENDER_MATERIAL,
	FREYJA_MODE_MATERIAL_EDIT,
	FREYJA_MODE_RENDER_NORMALS,
	FREYJA_MODE_RENDER_BONETAG,
	FREYJA_MODE_RENDER_GRID,
	eRenderPatch,

	FREYJA_MODE_LOAD_MODEL,
	FREYJA_MODE_SAVE_MODEL,
	FREYJA_MODE_LOAD_TEXTURE,
	FREYJA_MODE_SAVE_MATERIAL,
	FREYJA_MODE_LOAD_MATERIAL,
	FREYJA_MODE_LOAD_EMITTER,
	FREYJA_MODE_SAVE_EMITTER,
	FREYJA_MODE_LOAD_ANIMATION,
	FREYJA_MODE_SAVE_ANIMATION,
	FREYJA_MODE_LOAD_MAP,

	CMD_ANIMATION_NEXT,
	CMD_ANIMATION_PREV,
	CMD_ANIMATION_PLAY,
	CMD_ANIMATION_STOP,

	CMD_POLYGON_ADD,
	CMD_POLYGON_DELETE,
	CMD_POLYGON_SELECT,

	CMD_POINT_ADD,
	CMD_POINT_DELETE,
	CMD_POINT_COMBINE,

	CMD_BONE_PREV,
	CMD_BONE_NEXT,
	CMD_BONE_NEW,
	CMD_BONE_SELECT,
	CMD_BONE_ADD_MESH,
	CMD_BONE_DELETE_MESH,
	CMD_BONE_CONNECT,
	CMD_BONE_DISCONNECT,
	CMD_BONE_MOVE_PIVOT,


	CMD_MESH_ADD,
	CMD_MESH_MOVE_CENTER,

	CMD_MISC_ZOOM_IN,
	CMD_MISC_ZOOM_OUT,
	CMD_MISC_SIDES_P,
	CMD_MISC_SIDES_M,
	CMD_MISC_INFO,
	CMD_MISC_FPS,
	CMD_MISC_WIDGETSHOWN,
	CMD_MISC_BBOX_SELECT,
	CMD_MISC_GEN_TEXMAP_XY,
	CMD_MISC_VERTEX_UV,
	CMD_MISC_SCENE_ROTATE,

	eMeshFlipNormals,
	ePluginEventBase = 10000

} event_subject_id;


//////////////////////////////////////////////////////////////////////////////
// Freyja mgtk interface
//////////////////////////////////////////////////////////////////////////////

void freyja_append_eventid(char *symbol, int eventid);
/*------------------------------------------------------
 * Pre  : 
 * Post : Aliases another <symbol> for existing <eventid>
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

int freyja_append_item_to_menu(int event, const char *label, int item_event);
/*------------------------------------------------------
 * Pre  : 
 * Post : Appends menuitem <label>, <item_event> to menu <event>
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_application_window_move(int x, int y);
/*------------------------------------------------------
 * Pre  : 
 * Post : Moves application window without user input
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_callback_get_image_data_rgb24(const char *filename, 
										  unsigned char **image, 
										  int *width, int *height);
/*------------------------------------------------------
 * Pre  : 
 * Post : Callback retrieves image data from filename
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

int freyja_create_confirm_dialog(char *dialog_icon,
								 char *information_message, 
								 char *question_message,
								 char *cancel_icon, char *cancel_text,
								 char *accept_icon, char *accept_text);
/*------------------------------------------------------
 * Pre  : 
 * Post : Contructs a confirmation dialog
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

int freyja_event2i(int event, int cmd);
/*------------------------------------------------------
 * Pre  : <event> and <cmd> are an valid event pair
 * Post : Passes event to freyja master control
 *
 *        Returns -1 on unhandled event
 *
 *-- History ------------------------------------------
 *
 * 2002.01.19:
 *  Mongoose - Created
 ------------------------------------------------------*/

void freyja_event_exit();
/*------------------------------------------------------
 * Pre  : 
 * Post : Calls shutdown and exits GUI
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

#define freyja_event_file_dialog mgtk_event_file_dialog
void freyja_event_file_dialog(int event, char *s);
/*------------------------------------------------------
 * Pre  : 
 * Post : Spawns file dialog for <event> with title <s>
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_event_fullscreen();
/*------------------------------------------------------
 * Pre  : 
 * Post : Make application fullscreen
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

#define freyja_event_get_color mgtk_event_get_color
void freyja_event_get_color(int colorId,float &r, float &g, float &b, float &a);

void freyja_event_set_color(int colorId, float r, float g, float b, float a);

#define freyja_event_get_float mgtk_event_get_float
float freyja_event_get_float(int event);

void freyja_event_gl_refresh();
/*------------------------------------------------------
 * Pre  : 
 * Post : Display callback
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_event_info_dialog(char *icon, char *message);
/*------------------------------------------------------
 * Pre  : 
 * Post : Spawns info dialog with <message> and <icon>
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_event_notify_observer1f(event_subject_id e, float value);
/*------------------------------------------------------
 * Pre  : 
 * Post : This back propagates value changes to widgets
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_event_set_float(int event, float value);
/*------------------------------------------------------
 * Pre  : 
 * Post : Set float <value> for widget <event>
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

#define freyja_event_set_range mgtk_event_set_range
int freyja_event_set_range(int event, unsigned int value,
						   unsigned int min, unsigned int max);
/*------------------------------------------------------
 * Pre  : 
 * Post : Set range for a spinbutton
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_event_shutdown();
/*------------------------------------------------------
 * Pre  : 
 * Post : Cleans up Freyja subsystems
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_event_start();
/*------------------------------------------------------
 * Pre  : 
 * Post : Starts up Freyja subsystems
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_event_unfullscreen();
/*------------------------------------------------------
 * Pre  : 
 * Post : Restore application from fullscreen
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

int freyja_get_event_id_by_name(char *symbol);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets event Id from the symbol name
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

long freyja_get_new_plugin_eventid();
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets next UID for a plugin event
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets full path filename for icons
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_get_rc_path(char *s, long sz);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets user resource dir
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_get_rc_filename(char *s, const char *filename, long sz);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets user resource dir relative filename
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_get_share_path(char *s, long sz);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets system share dir
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_get_share_filename(char *s, const char *filename, long sz);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets system share dir relative filename
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_handle_color(int id, float r, float g, float b, float a);
/*------------------------------------------------------
 * Pre  : 
 * Post : mgtk callback handles color button value change
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_handle_resource_start();
/*------------------------------------------------------
 * Pre  : 
 * Post : mgtk callback inits the modeler application
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_install_user();
/*------------------------------------------------------
 * Pre  : 
 * Post : Copies files from system share dir to user dir
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

char freyja_is_user_installed();
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns non zero if it can find a user resource
 *        directory.
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_load_texture_buffer(byte *image, uint32 w, uint32 h, uint32 bpp);
/*------------------------------------------------------
 * Pre  : Arguments are componets of a valid pixel buffer
 * Post : Pixbuf is loaded as a texture or surface
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_plugin_generic(const char *symbol, void *something);
/*------------------------------------------------------
 * Pre  : <symbol> is the query 
 * Post : Returns <something> which is the thing symbol
 *         is bound to... mostly functions and data  ;)
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_print(char *format, ...);
/*------------------------------------------------------
 * Pre  : (<format>, ...) just like printf()  
 * Post : Prints to status bar, and will also print to
 *        console and log with '!' as first char in format
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_print_args(char *format, va_list *args);
/*------------------------------------------------------
 * Pre  : Just like freyja_print, but va_list is non-local
 * Post : Just like freyja_print
 *
 *        This only works for glibc!
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

String freyja_rc_map_string(const char *s);
/*------------------------------------------------------
 * Pre  : <s> is a valid filename or dir in resource dir
 * Post : Returns fullpath filename as string
 *
 *-- History ------------------------------------------
 *
 * 2006.08.01:
 * Mongoose - Created
 ------------------------------------------------------*/

char *freyja_rc_map(char *s);
/*------------------------------------------------------
 * Pre  : <s> is a valid filename or dir in resource dir
 * Post : Returns fullpath filename as C string or NULL
 *
 *        Call delete [] on this string once you're done
 *
 *        mgtk indirectly queries this to setup things
 *        such as user icons, etc
 *
 *-- History ------------------------------------------
 *
 * 2002.01.12: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_refresh_material_interface();
/*------------------------------------------------------
 * Pre  : 
 * Post : Syncs material interface to backend values
 *
 *-- History ------------------------------------------
 *
 * 2002.02.12:
 * Mongoose - Created
 ------------------------------------------------------*/

int freyja_remove_all_items_to_menu(int event);
/*------------------------------------------------------
 * Pre  : <event> is the Id of the menu subject 
 * Post : Removes all submenus and menuitems from menu
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_set_main_window_title(char *title);
/*------------------------------------------------------
 * Pre  : 
 * Post : <title> is set as the new window title
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_swap_buffers();
/*------------------------------------------------------
 * Pre  : 
 * Post : Called at the end of display, but does nothing
 *        you'd expect in mgtk builds
 *
 *-- History ------------------------------------------
 *
 * Unknown:
 * Mongoose - Created
 ------------------------------------------------------*/


//////////////////////////////////////////////////////////////////////////////
// Skeleton tree mgtk interface hackery 
//////////////////////////////////////////////////////////////////////////////

typedef struct callback_bone_s 
{
	int id;
	char name[64];
	unsigned int numChildren;
	callback_bone_s *parent;
	callback_bone_s *children;

} callback_bone_t;

void callback_update_skeleton(callback_bone_t *bone);

void UpdateSkeletonUI_Callback(uint32 skelIndex);


#endif
