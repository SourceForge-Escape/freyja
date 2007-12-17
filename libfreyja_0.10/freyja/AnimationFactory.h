/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2005-2007 Mongoose
 *
 * Object  : AnimationFactory
 *
 * Comments: This provides both a Factory Method, serialization, and storage
 *           interface for Animation.
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_ANIMATIONFACTORY_H_
#define GUARD__FREYJA_ANIMATIONFACTORY_H_

#include "XMLSerializer.h"


namespace freyja {

class Animation;

class AnimationFactory : 
	public XMLSerializer
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	AnimationFactory( )
	{ }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Constructor. 
	 *
	 ------------------------------------------------------*/

	virtual ~AnimationFactory( )
	{ }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Animation methods
	////////////////////////////////////////////////////////////

	freyja::Animation* CreateAnimation( );
	freyja::Animation* CreateAnimation( const char* name, vec_t duration );
	/*------------------------------------------------------
	 * Pre  : The <name> must be unique, and <duration> must be positive.
	 *
	 * Post : Stores new animation if valid.
	 *        Returns a new animation, or NULL if name is not unique.
	 *
	 ------------------------------------------------------*/

	uint32 GetAnimationCount( ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Gets the number of animations stored.
	 *
	 ------------------------------------------------------*/

	freyja::Animation* GetAnimation( uint16 id ) const;
	freyja::Animation* GetAnimation( const char* name ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns requested animation or NULL.
	 *
	 ------------------------------------------------------*/

	bool HasAnimation( const char* name ) const;
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns true if Animation with name is found.
	 *
	 ------------------------------------------------------*/

	void RemoveAnimation( const char* name );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Animation is removed from storage.
	 *
	 ------------------------------------------------------*/

    virtual bool SerializeAnimation( const char* name, FREYJA_XMLSERIALIZER_NODE parent ) const;
	virtual bool UnserializeAnimation( FREYJA_XMLSERIALIZER_NODE node );	
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : XmlSerializer interface.
	 *        Unserialize will replace Animations with the same name.
	 *
	 ------------------------------------------------------*/

protected:
	
};


} // namespace freyja

#endif // GUARD__FREYJA_ANIMATIONFACTORY_H_
