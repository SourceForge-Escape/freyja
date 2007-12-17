/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : TransformKeyframe
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: 
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Created, based on old keyframe template.
 ==========================================================================*/

#ifndef GUARD__FREYJA_VECKEYFRAME_H_
#define GUARD__FREYJA_VECKEYFRAME_H_

#include "Keyframe.h"


namespace freyja {

class TransformKeyframe : 
	public Keyframe
{
 public:

	TransformKeyFrame( freyja::AnimationTrack* track, vec_t time ) : 
		KeyFrame( track, time ), 
		mPosition(),
		mOrientation() 
	{}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual ~TransformKeyFrame() 
	{}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	vec_t GetData() 
	{ return mData; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetData(const vec_t v) 
	{ mData = v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual bool Serialize( TiXmlElement* container );
	virtual bool Unserialize( TiXmlElement* container );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods.
	 *
	 ------------------------------------------------------*/

protected:

	vec_t mData;                      /* Keyframe data */
};


inline
bool TransformKeyframe::Serialize( TiXmlElement* container )
{
	if (!container)
		return false;

	TiXmlElement* keyframe = new TiXmlElement("TransformKeyframe");
	keyframe->SetDoubleAttribute("time", mTime);
	keyframe->SetDoubleAttribute("x", mPosition.mX );
	keyframe->SetDoubleAttribute("y", mPosition.mY );
	keyframe->SetDoubleAttribute("z", mPosition.mZ );
	keyframe->SetDoubleAttribute("qx", mOrientation.mX );
	keyframe->SetDoubleAttribute("qy", mOrientation.mY );
	keyframe->SetDoubleAttribute("qz", mOrientation.mZ );
	keyframe->SetDoubleAttribute("qw", mOrientation.mW );

	if ( mMetaData.c_str() )
	{
		TiXmlElement* metadata = new TiXmlElement("metadata");
		TiXmlText* text = new TiXmlText( mMetaData.c_str() );
		metadata->LinkEndChild(text);
		keyframe->LinkEndChild(metadata);
	}

	container->LinkEndChild( keyframe );

	return true;
}


inline
bool TransformKeyframe::Unserialize( TiXmlElement* keyframe )
{
	if (!keyframe)
		return false;

	// FIXME: Name check.

	keyframe->QueryFloatAttribute("time", &mTime);
	keyframe->QueryFloatAttribute("x", &mPosition.mX );
	keyframe->QueryFloatAttribute("y", &mPosition.mY );
	keyframe->QueryFloatAttribute("z", &mPosition.mZ );
	keyframe->QueryFloatAttribute("qx", &mOrientation.mX );
	keyframe->QueryFloatAttribute("qy", &mOrientation.mY );
	keyframe->QueryFloatAttribute("qz", &mOrientation.mZ );
	keyframe->QueryFloatAttribute("qw", &mOrientation.mW );

	TiXmlElement *child = keyframe->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		if (s == "metadata")
		{
			mMetaData = child->GetText();
		}
	}

	return true; 
}


} // namespace freyja

#endif // GUARD__FREYJA_VECKEYFRAME_H_
