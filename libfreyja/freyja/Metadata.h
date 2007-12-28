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

	virtual const char* GetType() const
	{ return "Metadata"; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer tag type.
	 *
	 ------------------------------------------------------*/
 
	virtual uint32 GetVersion() const
	{ return 0; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer tag version.
	 *
	 ------------------------------------------------------*/

	bool Serialize( XMLSerializerNode metadata ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XmlSerializer implementation.
	 *
	 ------------------------------------------------------*/

	bool Unserialize( XMLSerializerNode metadata );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XmlSerializer implementation.
	 *
	 ------------------------------------------------------*/

	freyja::Material* GetMaterial() const
	{ return mMaterial; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

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
Metadata::Metadata()
{ }


inline
Metadata::~Metadata()
{ }


} // freyja

#endif // GUARD__FREYJA_METADATA_H_
