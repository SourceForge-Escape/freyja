/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : VecKeyframe
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

class VecKeyframe : 
	public Keyframe
{
 public:

	VecKeyFrame( freyja::AnimationTrack* track, vec_t time ) : 
		KeyFrame( track, time ), 
		mData(0.0f) 
	{}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual ~VecKeyFrame() 
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
bool VecKeyframe::Serialize( TiXmlElement* container )
{
	if (!container)
		return false;

	TiXmlElement* child = new TiXmlElement("VecKeyframe");
	child->SetDoubleAttribute("time", mTime);
	child->SetDoubleAttribute("data", mData);

	if ( mMetaData.c_str() )
	{
		TiXmlElement* metadata = new TiXmlElement("metadata");
		TiXmlText* text = new TiXmlText( mMetaData.c_str() );
		metadata->LinkEndChild(text);
		child->LinkEndChild(metadata);
	}

	container->LinkEndChild( child );

	return true;
}


inline
bool VecKeyframe::Unserialize( TiXmlElement* container )
{
	if (!container)
		return false;

	// FIXME: Name check.

	container->QueryFloatAttribute("time", &mTime);
	container->QueryFloatAttribute("data", &mData);

	TiXmlElement *child = container->FirstChildElement();
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
