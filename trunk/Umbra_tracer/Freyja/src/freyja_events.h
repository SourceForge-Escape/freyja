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


/* Mongoose 2004.10.28, 
 * These freyja_event_'s must be condensed into the eEvents
 * before the new command pattern event system can be started */
enum freyja_event_type
{
	EVENT_MAIN          =  0,
	EVENT_MISC          =  5
};




typedef enum {

	eNone = 0,
	eShutdown = 1,
	eNewFile,
	eOpenFile,
	eSaveFile,
	eCloseFile,
	eInfo,
	eDebug,
	eFullscreen,
	eScreenShot,

	eSetMaterialTexture,

	/* Dialogs */
	eAboutDialog,
	ePreferencesDialog,
	eHelpDialog,

	eMaterialMultiTex,
	eMaterialTex,

	eOpenFileTexture,

	eDelete,
	eSelect,
	eCopy,
	eCut,
	ePaste,

	eAddObject,
	eMoveObject,
	eRotateObject,
	eScaleObject,

	eTransformGroup,
	eTransformScene,
	eTransformMesh,
	eTransformMeshPivot,
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
	eGenerateCone,
	eGenerateTriStrip,
	eGenerateCylinder,
	eGenerateSphere,

	eSetMeshTexture,
	eTextureSlotLoad,

	eZoomIn,
	eZoomOut,
	eModelProperties,

	eAnimationNext,
	eAnimationPrev,
	eAnimationStop,
	eAnimationPlay,
	eAnimationSlider,

	eMeshNew,
	eMeshDelete,
	eMeshMove,
	eMeshSelect,
	eMeshRotate,
	eMeshScale,
	eMeshMirror,
	eMeshCenter,

	/* Text events */
	eSetCurrentBoneName,
	eSetTextureNameA,
	eSetTextureNameB,


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

	eSelectLight,

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

	/* Events to be rolled over */
	//CMD_MISC_FPS,
	//CMD_MISC_WIDGETSHOWN,
	CMD_MISC_SIDES_M,
	CMD_MISC_SIDES_P,
	CMD_MISC_TEXEL_COMBINE,
	CMD_MISC_BBOX_SELECT,
	CMD_MISC_GEN_TEXMAP_XY,
	CMD_MISC_VERTEX_UV,
	CMD_MISC_SCENE_ROTATE,

	CMD_POLYGON_ADD,
	CMD_POLYGON_DELETE,
	CMD_POLYGON_SELECT,

	CMD_POINT_ADD,
	CMD_POINT_DELETE,
	CMD_POINT_COMBINE,

	CMD_BONE_PREV,
	CMD_BONE_NEXT,
	CMD_BONE_NEW,
	CMD_BONE_ADD_MESH,
	CMD_BONE_DELETE_MESH,
	CMD_BONE_CONNECT,
	CMD_BONE_DISCONNECT,
	CMD_BONE_MOVE_PIVOT,

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

	
	eOpenGLNormalize,
	eOpenGLBlend,

	/* Misbehaved numbered events */
	eSetMaterialName = 799

} event_subject_id;


#define freyja_event_file_dialog mgtk_event_file_dialog
#define freyja_event_get_float mgtk_event_get_float
#define freyja_event_set_range mgtk_event_set_range

void freyja_event_fileselection_append_pattern(char *label, char *pattern);

void freyja_event_set_color(int colorId, float r, float g, float b, float a);

void freyja_event_gl_refresh();

void freyja_event_notify_observer1f(event_subject_id e, float value);

void freyja_application_window_move(int x, int y);

void freyja_event_start();       /* Starts up Freyja subsystems */
void freyja_event_exit();        /* Calls shutdown and exits GUI */
void freyja_event_shutdown();    /* Cleans up Freyja subsystems */

void freyja_event_info_dialog(char *icon, char *message);
void freyja_event_file_dialog(char *title);
void freyja_event_fullscreen();
void freyja_event_unfullscreen();

void freyja_event_file_dialog(char *filename);

float freyja_event_get_float(int event);
void freyja_event_set_float(int event, float value);
int freyja_event_set_range(int event, unsigned int value,
						   unsigned int min, unsigned int max);


char *freyja_rc_map(char *s);
void freyja_swap_buffers();
void freyja_set_main_window_title(char *title);

void freyja_print(char *format, ...);
void freyja_print_args(char *format, va_list *args); /* Only works for glibc! */

int freyja_create_confirm_dialog(char *dialog_icon,
								 char *information_message, 
								 char *question_message,
								 char *cancel_icon, char *cancel_text,
								 char *accept_icon, char *accept_text);

void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name);


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
