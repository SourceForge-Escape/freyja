/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : SDL/GLUT template
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: This is the common event system interface
 *           for freyja
 *
 *           It's independent of the widget set, etc
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
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


typedef enum {

	eNone = 0,
	eShutdown = 1,
	eNewFile,
	eOpenFile,
	eSaveFile,
	eSaveAsFile,
	eCloseFile,
	eOpenFileTexture,
	eAppendFile,
	eRevertFile,
	eExportFile,
	eImportFile,

	eHelp,
	eInfo,
	eAbout,
	eDebugEgg,
	eFullscreen,
	eScreenShot,

	ePreferencesDialog,
	eAboutDialog,
	eHelpDialog,

	eMaterialMultiTex,
	eMaterialTex,

	ePolygonSize,

	eMeshNew,
	eMeshDelete,
	eMeshMove,
	eMeshRotate,
	eMeshScale,
	eSetMaterialTexture,
	eTransformMeshPivot,
	eAnimationNext,
	eAnimationPrev,
	eAnimationStop,
	eAnimationPlay,
	eAnimationSlider,
	eZoomIn,
	eZoomOut,
	eSelectLight,
	eOpenGLNormalize,
	eOpenGLBlend,
	eDebug,
	eGenerateCone,
	eMeshMirror,
	eMeshSelect,

	eCopyAppendMode,
	eSplitObject,

	eDelete,
	eSelect,
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

	eTransformGroup,
	eTransformScene,
	eTransformMesh,
	eTransformBone,
	eTransformPoint,

	ePointJoint,
	eSphereJoint,
	eAxisJoint,
	eLineBone,
	ePolyMeshBone,
	eRenderBbox,
	eCamera,

	eGenerateNormals,
	eGenerateCube,
	eGenerateTriStrip,
	eGenerateCylinder,
	eGenerateSphere,

	eSetMeshTexture,
	eTextureSlotLoad,

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
	eMeshIterator,
	eGroupIterator,
	eBoneIterator,

	/* Text events */
	eSetCurrentBoneName,
	eSetTextureNameA,
	eSetTextureNameB,

	/* OLD EVENTS */
	FREYJA_MODE_MODEL_EDIT,
	FREYJA_MODE_MODEL_VIEW,
	FREYJA_MODE_TEXTURE_EDIT,
	FREYJA_MODE_ANIMATION,
	FREYJA_MODE_MODEL_VIEW2,
	FREYJA_MODE_PLANE_XY,
	FREYJA_MODE_PLANE_XZ,
	FREYJA_MODE_PLANE_YZ,

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
	CMD_MISC_TEXEL_COMBINE,
	CMD_MISC_BBOX_SELECT,
	CMD_MISC_GEN_TEXMAP_XY,
	CMD_MISC_VERTEX_UV,
	CMD_MISC_SCENE_ROTATE,

	EVENT_MISC,


	/* Misbehaved text events */
	eSetMaterialName = 799

} event_subject_id;

#define freyja_event_file_dialog mgtk_event_file_dialog
#define freyja_event_get_float mgtk_event_get_float
#define freyja_event_set_range mgtk_event_set_range

void freyja_event_start();       /* Starts up Freyja subsystems */
void freyja_event_exit();        /* Calls shutdown and exits GUI */
void freyja_event_shutdown();    /* Cleans up Freyja subsystems */

void freyja_event_notify_observer1f(event_subject_id id, float r);

void freyja_event_info_dialog(char *icon, char *message);
void freyja_event_file_dialog(char *title);
void freyja_event_fullscreen();
void freyja_event_unfullscreen();

void freyja_event_file_dialog_notify(char *filename);

float freyja_event_get_float(int event);
void freyja_event_set_float(int event, float value);
int freyja_event_set_range(int event, unsigned int value,
						   unsigned int min, unsigned int max);

void freyja_event_notify_view_log(const char *message);

void freyja_event_key_press(int key, int mod);
void freyja_event_new_key_cmd(int key, int event, int cmd);
char *freyja_rc_map(char *s);

char *freyja_rc_map(char *s);
/*------------------------------------------------------
 * Pre  : s is a valid filename or path/filename 
 *        under resouce dir
 *
 * Post : Returns full path filename as string or 
 *        NULL on error
 *
 *-- History ------------------------------------------
 *
 * 2002.01.12: 
 * Mongoose - Created
 ------------------------------------------------------*/

void freyja_set_main_window_title(char *title);

void freyja_swap_buffers();

//////////////////////////////////////////////////////////////////////////////
// External, old API /////////////////////////////////////////////////////////
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

	void refresh_material_interface();
	/*------------------------------------------------------
	 * Pre  : This listener is implmented by interface
	 * Post : Interface syncs interface to backend values
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.12:
	 * Mongoose - Created
	 ------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////////////


#define EVENT_FREYJA         -1000
#define EVENT_REDO_LAST      -5
#define CMD_FREYJA_EXIT      0
#define CMD_FREYJA_GL_INIT   1
#define CMD_NULL             0

//////////////////////////////////////////////////////////





void freyja_handle_color(int id, float r, float g, float b, float a);

int freyja_create_confirm_dialog(char *dialog_icon,
								 char *information_message, 
								 char *question_message,
								 char *cancel_icon, char *cancel_text,
								 char *accept_icon, char *accept_text);

void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name);

void freyja_event_fileselection_append_pattern(char *label, char *pattern);

void freyja_event_set_color(int colorId, float r, float g, float b, float a);

void freyja_event_gl_refresh();

void freyja_event_notify_observer1f(event_subject_id e, float value);

void freyja_application_window_move(int x, int y);

void freyja_event_start();       /* Starts up Freyja subsystems */
void freyja_event_exit();        /* Calls shutdown and exits GUI */
void freyja_event_shutdown();    /* Cleans up Freyja subsystems */

void freyja_print(char *format, ...);
void freyja_print_args(char *format, va_list *args); // only works for glibc!

int freyja_event2i(int event, int cmd);
/*------------------------------------------------------
 * Pre  : Event and Cmd are valid event pair
 * Post : Passes event to freyja control
 *
 *        Returns -1 on unhandled event
 *
 *-- History ------------------------------------------
 *
 * 2002.01.19:
 *  Mongoose - Created
 ------------------------------------------------------*/

void freyja_refresh_material_interface();
/*------------------------------------------------------
 * Pre  : This listener is implmented by interface
 * Post : Interface syncs interface to backend values
 *
 *-- History ------------------------------------------
 *
 * 2002.02.12:
 * Mongoose - Created
 ------------------------------------------------------*/
#endif
