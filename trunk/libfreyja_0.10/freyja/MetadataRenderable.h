/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Metadata
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: The user given metadata will be made into an XML structure.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.04.29:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__LIBFREYJA_RENDERABLE_H_
#define GUARD__LIBFREYJA_RENDERABLE_H_

#include <hel/math.h>
#include <hel/Vec3.h>

#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>

#include "freyja.h"


namespace freyja {

class Renderable
{
public:
	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Renderable();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of MetadataRenderable
	 *
	 ------------------------------------------------------*/

	~Renderable();
	/*------------------------------------------------------
	 * Pre  : MetadataRenderable object is allocated
	 * Post : Deconstructs an object of MetadataRenderable
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Cache API methods.
	////////////////////////////////////////////////////////////

	static Renderable* FindInCache(const char* key);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to cached instance or NULL if DNE.
	 *
	 ------------------------------------------------------*/

	static Renderable* ImportToCache(const char* filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Caches renderable ( if not already cached ),
	 *        and returns pointer to cache instance.
	 *
	 ------------------------------------------------------*/

	uint32 mRefCount;

	mstl::String mModel;            /* Model filename / renderable instance key. */

	vec_t* mVertices;  /* Geometry buffers for rendering the icon. */
	vec_t* mTexcoords;
	vec_t* mNormals;

	uint16* mIndices;  /* Triangle indices for rendering. */
	uint16 mFaceCount;


protected:

	bool LoadModel(const char* filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 1. Loads mesh using libfreyja plugin system if not already loaded.
	 *        2. Converts to simple MetadataRenderable geometry.
	 *        3. Deletes libfreyja editable model, since this is just a visual cue.
	 *
	 *        Returns false on failure.
	 *
	 ------------------------------------------------------*/

	static Renderable* mMRU;

	static mstl::Vector<Renderable*> mGobalPool;
};


////////////////////////////////////////////////////////////
// Inline Methods
////////////////////////////////////////////////////////////

inline
Renderable::Renderable()  :
		mRefCount(0),
		mModel(),
		mVertices(NULL),
		mTexcoords(NULL),
		mNormals(NULL),
		mIndices(NULL),
		mFaceCount(0)
{ }


inline
Renderable::~Renderable()
{
	if (mVertices) delete [] mVertices;
	if (mTexcoords) delete [] mTexcoords;
	if (mNormals) delete [] mNormals;
	if (mIndices) delete [] mIndices; 
}


} // freyja

#endif // GUARD__LIBFREYJA_RENDERABLE_H_

