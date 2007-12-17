/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : KeyFrame
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: KeyFrame class. Base for: Transform, VertexMorph, Pose, etc
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.12.11:
 * Mongoose - Reduced and refined API more. 
 *
 * 2006.11.06:
 * Mongoose - New KISS implementation is so nice.
 *
 * 2005.07.08:
 * Mongoose - Created.
 ==========================================================================*/

#ifndef GUARD__FREYJA_KEYFRAME_H_
#define GUARD__FREYJA_KEYFRAME_H_

#include <math.h>
#include <hel/math.h>
#include <mstl/String.h>

#include "freyja.h"

namespace freyja {

class Keyframe : 
		public XMLSerializer
{
public:

	KeyFrame( freyja::AnimationTrack* track, vec_t time, const char* metadata ) :
		mOwner( track ),
		mTime( time ),
		mMetaData( metadata )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	KeyFrame( freyja::AnimationTrack* track, vec_t time ) :
		mOwner( track ),
		mTime( time ), 
		mMetaData( "" ) 
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~KeyFrame() 
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	virtual Keyframe* Duplicate( freyja::AnimationTrack* owner ) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Duplicates this keyframe in another track ( <owner> ).
	 *
	 ------------------------------------------------------*/

	const char* GetMetaData() const { return mMetaData.c_str(); }
	void SetMetaData( const char* s ) { mMetaData = s; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : MetaData property.
	 *
	 ------------------------------------------------------*/

	const vec_t GetTime() const { return mTime; }
	void SetTime( const vec_t time ) { mTime = time; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Time property.
	 *
	 ------------------------------------------------------*/

	virtual bool Serialize( FREYJA_XMLSERIALIZER_NODE parent ) = 0;
	virtual bool Unserialize( FREYJA_XMLSERIALIZER_NODE node ) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods.
	 *
	 ------------------------------------------------------*/

 protected:

	freyja::AnimationTrack* mOwner;  /* Track containing this keyframe. */

	vec_t mTime;                     /* Time offset to this frame in seconds. */

	mstl::String mMetaData;          /* String for storing XML metadata. */
};


} // namespace freyja

#endif // GUARD__FREYJA_KEYFRAME_H_
