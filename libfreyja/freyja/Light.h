/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaLight
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the light data model class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJALIGHT - Builds FreyjaLight class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, based on Freyja's Light class from 2002.01.27
 ==========================================================================*/

#ifndef GUARD__FREYJA_LIGHT_H_
#define GUARD__FREYJA_LIGHT_H_

#include "Node.h"


namespace freyja {

class Light :
		public Node
{
 public:

	typedef enum {
		ePoint = 1,
		eDirectional,
		eSpot

	} light_type_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Light( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaLight
	 *
	 *-- History ------------------------------------------
	 *

	 * 2004.10.22: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Light();
	/*------------------------------------------------------
	 * Pre  : FreyjaLight object is allocated
	 * Post : Deconstructs an object of FreyjaLight
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


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
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	unsigned int mOptions;    /* Option bitflags */

	light_type_t mType;       /* Type of light */

	vec4_t mAmbient;          /* Ambient color. */

	vec4_t mDiffuse;          /* Diffuse color. */

	vec4_t mSpecular;         /* Specular color. */

	vec4_t mDir;	          /* Direction for directional or spot light use */

	vec_t mCutOff;            /* Cut off angle for spot light use */

	vec_t mExponent;          /* Exponent for spot light use */

	vec_t mAttenuation;       /* Attenuation factor */
};


inline
const char* Light::GetType() const
{ return "Light"; }

 
inline
uint32 Light::GetVersion() const
{ return 0; }


inline
freyja::Node* Light::Duplicate() const
{ return new Light(*this); }


inline
freyja::Material* Light::GetMaterial() const
{ return NULL; }


inline
bool Light::Serialize( XMLSerializerNode parent ) const
{
	return false;
}

inline
bool Light::Unserialize( XMLSerializerNode node )
{
	return false;
}

inline
mstl::String Light::GetInfo() const
{
	return mstl::String( "Light" );
}

inline
const hel::Quat& Light::GetWorldOrientation() const
{
	return mOrientation;
}

inline
const hel::Vec3& Light::GetWorldPosition() const
{
	return mPosition;
}

inline
void Light::DuplicateChildren( freyja::Node* parent, bool recurse )
{
}


} // namespace freyja

#endif  // GUARD__FREYJA_LIGHT_H_
