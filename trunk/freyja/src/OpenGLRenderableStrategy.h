/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : OpenGLRenderableStrategy
 * License : No use w/o permission (c) 2007 Mongoose
 * Comments: OpenGL renderer implementation for freyja::Renderable.
 *
 *-- History ------------------------------------------------- 
 *
 * 2007.12.31:
 * Mongoose - Created.
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_OPENGLRENDERABLESTRATEGY__H_
#define GUARD__FREYJA3D_OPENGLRENDERABLESTRATEGY__H_

#include <freyja/Material.h>
#include <freyja/RenderableStrategy.h>

namespace freyja3d {

class OpenGLRenderableStrategy :
		public freyja::RenderableStrategy
{
public:

	OpenGLRenderableStrategy()
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual ~OpenGLRenderableStrategy()
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void ApplyMaterial( freyja::Material* mat );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void DrawIcon( const char* name  );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void DrawTriangles(  );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
};


} /* End namespace freyja3d */


#endif // GUARD__FREYJA3D_OPENGLRENDERABLESTRATEGY__H_
