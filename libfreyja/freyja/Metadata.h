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
 * 2007.12.26:
 * Mongoose - Metadata becomes a non-abstract Node in 10.0 design.
 *
 * 2006.04.29:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_METADATA_H_
#define GUARD__FREYJA_METADATA_H_

#include "Node.h"
#include "freyja.h"


namespace freyja {

class Metadata :
		public Node
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Metadata( const char* name );
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

	const char* GetMetadataType() const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetMetadataType(const char* type);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char* GetModel() const
	{ return NULL; } // FIXME
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetModel(const char* model);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public interfaces.
	////////////////////////////////////////////////////////////

	FREYJA_NODE_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Node implementation.
	 *
	 ------------------------------------------------------*/

	FREYJA_XMLSERIALIZER_INTERFACE
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer implementation.
	 *
	 ------------------------------------------------------*/

	FREYJA_RENDERABLE_INTERFACE
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Renderable implementation.
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

	freyja::Material* mMaterial;

	mstl::String mMetadataType;
};



////////////////////////////////////////////////////////////
// Inline Methods
////////////////////////////////////////////////////////////

inline
Metadata::Metadata( const char* name ) :
	Node( name )
{ }


inline
Metadata::~Metadata()
{ }

inline
const char* Metadata::GetType() const
{ return "Metadata"; }

 
inline
uint32 Metadata::GetVersion() const
{ return 0; }


inline
freyja::Material* Metadata::GetMaterial() const
{ return mMaterial; }


inline
mstl::String Metadata::GetInfo() const
{
	return mstl::String( "Metadata" );
}

inline
const hel::Quat& Metadata::GetWorldOrientation() const
{
	return mOrientation;
}

inline
const hel::Vec3& Metadata::GetWorldPosition() const
{
	return mPosition;
}

inline
freyja::Node* Metadata::Duplicate( ) const
{
	return NULL;
}

inline
void Metadata::DuplicateChildren( freyja::Node* parent, bool recurse )
{
}

} // freyja

#endif // GUARD__FREYJA_METADATA_H_
