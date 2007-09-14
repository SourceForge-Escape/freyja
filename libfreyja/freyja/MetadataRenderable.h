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

#ifndef GUARD__LIBFREYJA_METADATARENDERABLE_H_
#define GUARD__LIBFREYJA_METADATARENDERABLE_H_

#include <hel/math.h>
#include <hel/Vec3.h>

#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>

#include "freyja.h"


namespace freyja {

class MetadataRenderable
{
public:
	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	MetadataRenderable();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of MetadataRenderable
	 *
	 ------------------------------------------------------*/

	~MetadataRenderable();
	/*------------------------------------------------------
	 * Pre  : MetadataRenderable object is allocated
	 * Post : Deconstructs an object of MetadataRenderable
	 *
	 ------------------------------------------------------*/

	static MetadataRenderable* ImportToCache(const char* filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Caches renderable ( if not already cached ),
	 *        and returns pointer to cache instance.
	 *
	 ------------------------------------------------------*/

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

	uint32 mRefCount;

	mstl::String mModel;            /* Model filename / renderable instance key. */

	mstl::Vector<vec_t> mVertices;  /* Geometry buffers for rendering the icon. */

	mstl::Vector<vec_t> mTexcoords;

	mstl::Vector<vec_t> mNormals;

	mstl::Vector<uint16> mIndices;  /* Triangle indices for rendering. */
};


////////////////////////////////////////////////////////////
// Inline Methods
////////////////////////////////////////////////////////////

inline
MetadataRenderable::MetadataRenderable()  :
		mRefCount(0),
		mModel(),
		mVertices(),
		mNormals(),
		mIndices()
{ }


inline
MetadataRenderable::~MetadataRenderable()
{ }


inline
MetadataRenderable* MetadataRenderable::ImportToCache(const char* filename)
{
#warning "FIXME: Implement this function for metadata renderables."
	return NULL;
}

} // freyja

#endif // GUARD__LIBFREYJA_METADATARENDERABLE_H_
