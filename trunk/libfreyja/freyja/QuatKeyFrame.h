/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : KeyFrame
 * License : No use w/o permission (C) 2005-2006 Mongoose
 * Comments: KeyFrame class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_KEYFRAME - Builds KeyFrame class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.11.06:
 * Mongoose - New KISS implementation is so nice
 *
 * 2005.07.08:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_QUATKEYFRAME_H_
#define GUARD__FREYJA_QUATKEYFRAME_H_

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include "KeyFrame.h"
#include "Track.h"


namespace freyja {


class QuatKeyFrame : public KeyFrame
{
 public:

	QuatKeyFrame() : KeyFrame(), mQ() {}
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	QuatKeyFrame(vec_t time) : KeyFrame(time), mQ() {}
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~QuatKeyFrame() {}
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	Quat &Get() { return mQ; }
	virtual void Set(const Quat &q) { mQ = q; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Quaternion rotation property.
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetSerializedSize() { return 4*4+GetSerializedSizeBase(); }
	virtual bool Serialize(SystemIO::FileWriter &w);
	virtual bool Unserialize(SystemIO::FileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Binary serialization methods.
	 *
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::TextFileWriter &w);
	virtual bool Unserialize(SystemIO::TextFileReader &r) ;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Text serialization methods.
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	virtual bool Serialize(TiXmlElement *container);
	virtual bool Unserialize(TiXmlElement *keyframe);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods.
	 *
	 ------------------------------------------------------*/
#endif


 protected:

	Quat mQ;             /* Quaternion rotation. */
};


////////////////////////////////////////////////////////////
// Inline methods
////////////////////////////////////////////////////////////

inline
bool QuatKeyFrame::Serialize(SystemIO::FileWriter &w) 
{
	SerializeBase(w);
	w.WriteFloat32(mQ.mX); 
	w.WriteFloat32(mQ.mY); 
	w.WriteFloat32(mQ.mZ); 
	w.WriteFloat32(mQ.mW);
	return true;
}


inline
bool QuatKeyFrame::Unserialize(SystemIO::FileReader &r) 
{ 
	UnserializeBase(r);
	mTime = r.ReadFloat32();
	mFlags = r.();
	mQ.mX = r.ReadFloat32();
	mQ.mY = r.ReadFloat32();
	mQ.mZ = r.ReadFloat32();
	mQ.mW = r.ReadFloat32();
	return true;
}


inline
virtual bool QuatKeyFrame::Serialize(SystemIO::TextFileWriter &w) 
{
	SerializeBase(w);
	w.Print(" %f %f %f %f\n", mQ.mX, mQ.mY, mQ.mZ, mQ.mW);
	return true;
}


inline
virtual bool QuatKeyFrame::Unserialize(SystemIO::TextFileReader &r) 
{ 
	UnserializeBase(r);
	mQ.mX = r.ParseFloat();
	mQ.mY = r.ParseFloat();
	mQ.mZ = r.ParseFloat();
	mQ.mW = r.ParseFloat();
	return true; 
}


#if TINYXML_FOUND
inline
virtual bool QuatKeyFrame::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *keyframe = new TiXmlElement("QuatKeyFrame");
	SerializeBase(keyframe);
	keyframe->SetDoubleAttribute("x", mQ.mX);
	keyframe->SetDoubleAttribute("y", mQ.mY);
	keyframe->SetDoubleAttribute("z", mQ.mZ);
	keyframe->SetDoubleAttribute("w", mQ.mW);
	container->LinkEndChild(keyframe);

	return true;
}


inline
virtual bool QuatKeyFrame::Unserialize(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	UnserializeBase(keyframe);
	keyframe->QueryFloatAttribute("x", &mQ.mX);
	keyframe->QueryFloatAttribute("y", &mQ.mY);
	keyframe->QueryFloatAttribute("z", &mQ.mZ);
	keyframe->QueryFloatAttribute("w", &mQ.mW);
	return true; 
}
#endif


} // namespace freyja

#endif // GUARD__FREYJA_QUATKEYFRAME_H_
