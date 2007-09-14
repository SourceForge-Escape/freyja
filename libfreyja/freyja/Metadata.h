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
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_METADATA - Builds MetaData class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.04.29:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__LIBFREYJA_METADATA_H_
#define GUARD__LIBFREYJA_METADATA_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <hel/Quat.h>

#include <mstl/String.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include "MetadataRenderable.h"
#include "freyja.h"


namespace freyja {

class Metadata
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Metadata();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Metadata
	 *
	 ------------------------------------------------------*/

	~Metadata();
	/*------------------------------------------------------
	 * Pre  : Metadata object is allocated
	 * Post : Deconstructs an object of Metadata
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public 
	////////////////////////////////////////////////////////////

	bool AddToPool();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Places this object in gobal pool if not there.
	 *        Returns false on failure.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetPos() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns object position in world space.
	 *
	 ------------------------------------------------------*/

	const MetadataRenderable* GetRenderable() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns object's geometric representation.
	 *
	 ------------------------------------------------------*/

	const hel::Quat& GetRot() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns object oreintation in world space.
	 *
	 ------------------------------------------------------*/

	const hel::Vec3& GetScale() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns object scale.
	 *
	 ------------------------------------------------------*/

	index_t GetUid();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns UID.
	 *
	 ------------------------------------------------------*/

	bool RemoveFromPool();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Removes this object in gobal pool if there.
	 *        Returns false on failure.
	 *
	 ------------------------------------------------------*/

	void SetPos(const hel::Vec3& pos);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets object position in world space.
	 *
	 ------------------------------------------------------*/

	void SetRot(const hel::Quat& q);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets object orientation.
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND

	bool Serialize(TiXmlElement* metadata);
	bool Unserialize(TiXmlElement* metadata);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML object serialization methods.
	 *        Returns false on failure.
	 *
	 ------------------------------------------------------*/

#endif // TINYXML_FOUND

	bool Unserialize(const char* filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML object serialization from given file.
	 *        Returns false on failure.
	 *
	 ------------------------------------------------------*/

	const char* GetName() { return mName.c_str(); }
	void SetName(const char* name) { mName = name; }

	const char* GetType() { return mType.c_str(); }
	void SetType(const char* type) { mType = type; }

	const char* GetMetadata() { return mMetadata.c_str(); }
	void SetMetadata(const char* metadata) { mMetadata = metadata; }

	const char* GetModel() { return mRenderable ? mRenderable->mModel.c_str() : NULL; }
	void SetModel(const char* model);

	const char* GetMaterial() { return mMaterial.c_str(); }
	void SetMaterial(const char* material) { mMaterial = material; }


	////////////////////////////////////////////////////////////
	// Static Methods
	////////////////////////////////////////////////////////////

	static Metadata *GetObjectByUid(index_t uid);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns object from gobal pool or NULL.
	 *
	 ------------------------------------------------------*/

	static uint32 GetObjectCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns object count from gobal pool.
	 *
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Block copy const and operator= and their defaults usage.
	////////////////////////////////////////////////////////////

	Metadata(const Metadata& metadata);

	Metadata& operator =(const Metadata &metadata);


	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	index_t mUid;                  /* Unique MetaData object identifier. */

	uint32 mFlags;                 /* Option flags. */

	mstl::String mName;            /* User given object name. */

	mstl::String mType;            /* User given object type. */

	mstl::String mMetadata;        /* User given XML metadata. */

	MetadataRenderable* mRenderable;  /* Used as a geometric symbol. */

	mstl::String mMaterial;        /* Optional material for Renderable. */

	index_t mMaterialIndex;

	hel::Vec3 mPos;                /* Common 3d properties for metadata objects. */

	hel::Quat mRot;

	hel::Vec3 mScale;
};



////////////////////////////////////////////////////////////
// Inline Methods
////////////////////////////////////////////////////////////

inline
Metadata::Metadata() :
		mUid(INDEX_INVALID),
		mFlags(0),
		mName(),
		mType(),
		mMetadata(),
		mRenderable(NULL),
		mMaterial(),
		mMaterialIndex(0),
		mPos(),
		mRot(),
		mScale()
{ }


inline
Metadata::~Metadata()
{ }


inline
const hel::Vec3& Metadata::GetPos() const
{
	return mPos;
}


inline
const MetadataRenderable* Metadata::GetRenderable() const
{
	return mRenderable;
}


inline
const hel::Quat& Metadata::GetRot() const
{
	return mRot;
}


inline
const hel::Vec3& Metadata::GetScale() const
{
	return mScale;
}


inline
index_t Metadata::GetUid()
{
	return mUid;
}


inline
void Metadata::SetPos(const hel::Vec3& pos)
{
	mPos = pos;
}


inline
void Metadata::SetRot(const hel::Quat& q)
{
	mRot = q;
}


} // freyja

#endif // GUARD__LIBFREYJA_METADATA_H_
