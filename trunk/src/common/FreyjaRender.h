/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaRender
 * License : No use w/o permission (C)2001 Mongoose
 * Comments: GL context rendering backend for Freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.07.28:
 * Mongoose - Update documentation and prune obsoloete methods
 *
 * 2000.08.25:
 * Mongoose - Created, based on GooseEgg view prototype
 *
 =================================================================*/

#ifndef GUARD__MONGOOSE_FREYJA_FREYJARENDER_H_
#define GUARD__MONGOOSE_FREYJA_FREYJARENDER_H_

#include "FreyjaModel.h"


enum view_mode {                   /* View|Edit mode */     
	VIEWMODE_MODEL_EDIT       = 1,
	VIEWMODE_TEXTURE_EDIT     = 2,
	VIEWMODE_MAP_VIEW         = 3,
	VIEWMODE_MODEL_VIEW       = 4,
	VIEWMODE_MATERIAL_EDIT    = 5
};

enum rotate_flags {
	X_F = 1,
	Y_F = 2,
	Z_F = 4
};


class FreyjaRender
{
public:

	typedef enum {                 /* Rendering modes */
		RENDER_WIREFRAME        = 1,
		RENDER_TEXTURE          = 2,
		RENDER_FACE             = 4,
		RENDER_EDIT_GRID        = 8,
		RENDER_LIGHTING         = 16,         /* Render with GL lighting */
		RENDER_PARTICLES        = 32,
		RENDER_BONES            = 64,
		RENDER_MATERIAL         = 128,
		RENDER_CAMERA           = 256,        /* Enable panning  */
		RENDER_BBOX             = 512,        /* Rendering bounding box */
		RENDER_NORMAL           = 1024, 
		//RENDER_RESERVED1        = 2048, 
		//RENDER_RESERVED2        = 4096, 
		RENDER_BBOX_SEL         = 8192,
		//RENDER_RESERVED3        = 16384,
		RENDER_POINTS           = 32768
	} flags_t;


	FreyjaRender();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FreyjaRender is constructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.02:
	 * Mongoose - No args
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaRender();
	/*------------------------------------------------------
	 * Pre  : FreyjaRender exists
	 * Post : FreyjaRender is deconstructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Register(FreyjaModel *model);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Hook into data model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.04.11:
	 * Mongoose - Use FreyjaModel instead of Egg directly
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Init(int width, int height, bool fast_card);
	/*------------------------------------------------------
	 * Pre  : Width and Height are the GL context dim 
    *        Fast_card is true if GL hw accel is present
	 *
	 * Post : Sets up GL parms for GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	void Reset();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Reset to inital state
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Flags(flags_t flag, int op);
	/*------------------------------------------------------
	 * Pre  : The Flag and Operator are valid
	 *        Op { 0 = clear, 1 = set }
	 *
	 * Post : Sets control flags for model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int Flags();
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns control flags for model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void EditTexture(int n);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set texture to render on a full window quad
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.10.07:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void RotateAmount(float n);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set rotate amount
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	void Rotate(int flags, float n);
	/*------------------------------------------------------
	 * Pre  : Flags are valid XYZ flags
	 * Post : Rotate on axis n degrees
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	float RotateAmount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return incremental rotate amount
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getRotation(vec3_t v);
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Return present rotation angles in Euler 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	void CustomColor(int flags, float r, float g, float b);
	/*------------------------------------------------------
	 * Pre  : Flags are vaild color flag(s) ( ORable ),
	 *        you can set more than one color at once
	 *
	 * Post : Set custom color(s) to given color
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void ViewMode(int mode);
	/*------------------------------------------------------
	 * Pre  : Mode are valid view_mode(s)
	 * Post : Sets viewing options
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int Width();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get GL context window width 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int Height();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get GL context window height
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Width(unsigned int i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set GL context window width 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Height(unsigned int i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set GL context window height
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Display();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders and rasters a frame to GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Reshape(unsigned int w, unsigned int h);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resizes GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void ScreenShot();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Generates a screenshot of just GL context
	 *        with an auto timestamped name
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.??.??:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	
private:    

	void DrawQuad(float x, float y, float w, float h);
	/*------------------------------------------------------
	 * Pre  : X, Y, W, H define a 2d quad
	 * Post : Draws a quad orthogonal to GL context
	 *        as a solid or with current texture
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	
	void DrawPolygon(egg_polygon_t &polygon);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawMesh(egg_mesh_t &mesh);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawModel(Egg *egg);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawBbox(egg_group_t *group);
	/*------------------------------------------------------
	 * Pre  : Group exists
	 * Post : Renders any group's bounding box
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.??.??:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawGrid(int w, int h, int size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders a grid orthogal to gl context
	 *        WxH at size spacing and adjusts for scroll
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawWindow(int plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders 3d model/particles/etc editor
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawMaterialEditWindow();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders material editing sphere
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void DrawTextureEditWindow(unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renders texel editing quad
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	
	void Rotate(float n, int axis);
	/*------------------------------------------------------
	 * Pre  : Axis is 0 = x, 1 = y, 2 = z
	 * Post : Rotate on axis n degrees
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void BindTexture(unsigned int texture);
	/*------------------------------------------------------
	 * Pre  : texture is valid texture id
	 * Post : Binds GL texture 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.28:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	// START HACKS for new features / roll overs
	void drawLights();
	// END HACKS

	FreyjaModel *_model;                       /* Backend agent */
	
	unsigned int mRenderMode;                  /* Rendering mode */

	unsigned int _texture;                     /* Currently bound texture */

	unsigned int _width;                       /* Width of 3d window */

	unsigned int _height;                      /* Height of 3d window */

	bool _fast_card;                           /* Using a fast GL card? */

	bool _init;                                /* OpenGL context started? */

	int _view_mode;                            /* View mode */

	float _aspect_ratio;                       /* Cached aspect ratio */

	float _rotate_amount;                      /* Degrees to rotate per
																 call to rotate */

	float _scroll[3];                          /* Used to cache scroll */

	float _zoom;                               /* Used to cache zoom */

	float _angles[3];                          /* Used to rotate the scene */

	float _edit_polygon_color[3];              /* Custom color support */
	float _edit_line_color[3];
	float _edit_line_highlight_color[3];
	float _edit_vertex_color[3];
	float _edit_vertex_highlight_color[3];
	float _edit_bbox_color[3];
	float _edit_bg_color[3];
	float _edit_grid_color[3];
	float _edit_grid_x_axis_color[3];
	float _edit_grid_y_axis_color[3];
	float _edit_grid_z_axis_color[3];
	float _edit_grid_8_color[3];
	float _edit_text_color[3];
	float _view_bg_color[3];
	float _view_text_color[3];
	float _buffer_vertex_color[3];
	float _texture_edit_line_color[3];
	
	float _bone_line_width;                    /* Custom artifact size */
	float _bone_point_size;
	float _default_point_size;
	float _default_line_width;
	float _vertex_point_size;
};

#endif
