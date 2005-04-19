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

#include <hel/ViewVolume.h>

#include "OpenGLFreyjaScene.h"
#include "freyja_events.h"


// FreyjaInterface

enum view_mode {                   /* View|Edit mode */     
	VIEWMODE_MODEL_EDIT       = 1,
	VIEWMODE_TEXTURE_EDIT     = 2,
	VIEWMODE_MAP_VIEW         = 3,
	VIEWMODE_MODEL_VIEW       = 4,
	VIEWMODE_MATERIAL_EDIT    = 5
};


class FreyjaRender
{
public:



	typedef enum {
		PLANE_XY = 0,
		PLANE_XZ = 1,
		PLANE_YZ = 2

	} plane_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaRender(OpenGLFreyjaScene *scene);
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


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int getCurrentPlane();

	int getCurrentTextureIndex();


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void setCurrentPlane(unsigned int plane);

	void setCurrentTextureIndex(int index);

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

	//void Flags(flags_t flag, int op);
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

	//unsigned int Flags();
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns control flags for model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.09: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	//void EditTexture(int n);
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
	
	void Rotate(int axis, float angle);
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

	// FIXME
	void setZoom(float zoom);
	float getZoom();
	

private:    

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

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

	
	//void DrawPolygon(egg_polygon_t &polygon);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders polygon
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	//void DrawMesh(egg_mesh_t &mesh);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders mesh
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	//void DrawModel(Egg *egg);
	/*------------------------------------------------------
	 * Pre  : Called from proper method
	 * Post : Renders model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	//void DrawBbox(egg_group_t *group);
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

	void drawLights();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Point lights rendered as spheres
	 *        Spot lights as cones
	 *        Directional lights as spheres 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.23:
	 * Mongoose - Created, test API method made offical
	 ------------------------------------------------------*/


	OpenGLFreyjaScene *mScene;                 /* Scene data model */

	unsigned int mRenderMode;                  /* Rendering mode */


	unsigned int mPlane;

	int mTextureIndex;

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

	float mZoom;                               /* Used to cache zoom */

	float _angles[3];                          /* Used to rotate the scene */
};

#endif
