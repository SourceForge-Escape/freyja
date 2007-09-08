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

#ifndef GUARD__FREYJA_KEYFRAME_H_
#define GUARD__FREYJA_KEYFRAME_H_

#include <math.h>
#include <hel/math.h>
#include <hel/Vec3.h>
#include <hel/Mat44.h>
#include <hel/Quat.h>
#include <hel/Ray.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>

#include "freyja.h"

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

using namespace mstl;
using hel::Vec3;

namespace freyja {

class KeyFrame
{
public:

	typedef enum {

		fNone = 0,
		fTx = 1,
		fTy = 2,
		fTz = 4,
		fRx = 8,
		fRy = 16,
		fSx = 32,
		fSy = 64,
		fSz = 128

	} Flags;


	KeyFrame() : mFlags(fNone), mTime(0.0f), mMetaData("") { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	KeyFrame(vec_t time) : mFlags(fNone), mTime(time), mMetaData("") { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~KeyFrame() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	const char *GetMetaData() { return mMetaData.c_str(); }
	void SetMetaData(const char *s) { mMetaData = s; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : MetaData property.
	 *
	 ------------------------------------------------------*/

	vec_t GetTime() { return mTime; }
	virtual void SetTime(vec_t time) { mTime = time; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Time property.
	 *
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileWriter &w) = 0;
	virtual bool Unserialize(SystemIO::FileReader &r) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Text serialization methods.
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetSerializedSize() = 0;
	virtual bool Serialize(SystemIO::TextFileWriter &w) = 0;
	virtual bool Unserialize(SystemIO::TextFileReader &r) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Binary serialization methods.
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	virtual bool Serialize(TiXmlElement *container) = 0;
	virtual bool Unserialize(TiXmlElement *container) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods.
	 *
	 ------------------------------------------------------*/
#endif

 protected:

	virtual bool SerializeBase(SystemIO::FileWriter &w);
	virtual bool UnserializeBase(SystemIO::FileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Text serialization methods for base attributes.
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetSerializedSizeBase();
	virtual bool SerializeBase(SystemIO::TextFileWriter &w);
	virtual bool UnserializeBase(SystemIO::TextFileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Binary serialization methods for base attributes.
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	virtual bool SerializeBase(TiXmlElement *container);
	virtual bool UnserializeBase(TiXmlElement *container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods for base attributes.
	 *
	 ------------------------------------------------------*/
#endif


	byte mFlags;       /* Used in obsolete code for interface use */

	vec_t mTime;       /* Time offset to this frame in sec */

	String mMetaData;  /* XML metadata for keyframe.  
						* Not to be confused with XML serialization. */
};


class VecKeyFrame : public KeyFrame
{
 public:

	VecKeyFrame() : KeyFrame(), mData(0.0f) {}

	VecKeyFrame(vec_t time) : KeyFrame(time), mData(0.0f) {}

	virtual ~VecKeyFrame() {}

	vec_t GetData() { return mData; }

	virtual void SetData(const vec_t v) { mData = v; }

	virtual uint32 GetSerializedSize() { return 4; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : 
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileWriter &w) 
	{ return w.WriteFloat32(mData); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Write data from this object to disk
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::TextFileWriter &w) 
	{
		w.Print(" %f", mTime);
		w.Print(" %u", mFlags);
		w.Print(" %f\n", mData);
		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Write data from this object to disk
	 ------------------------------------------------------*/

	virtual bool Unserialize(SystemIO::FileReader &r) 
	{ mData = r.ReadFloat32(); return true; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read data into this object from disk
	 ------------------------------------------------------*/

	virtual bool Unserialize(SystemIO::TextFileReader &r) 
	{ 
		mTime = r.ParseFloat();
		mFlags = r.ParseInteger();
		mData = r.ParseFloat();
		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read data into this object from disk
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	virtual bool Serialize(TiXmlElement *container)
	{
		if (!container)
			return false;

		TiXmlElement *key = new TiXmlElement("VecKeyFrame");
		key->SetAttribute("vertex", mFlags);
		key->SetDoubleAttribute("time", mTime);
		key->SetDoubleAttribute("v", mData);

		container->LinkEndChild(key);

		return true;
	}

	virtual bool Unserialize(TiXmlElement *container)
	{
		if (!container)
			return false;

		{
			int attr = 0;
			container->QueryIntAttribute("flags", &attr);
			mFlags = attr;
		}

		container->QueryFloatAttribute("time", &mTime);
		container->QueryFloatAttribute("v", &mData);

		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods.
	 *
	 ------------------------------------------------------*/
#endif

	vec_t mData;                      /* Keyframe data */
};


class Vec3KeyFrame : public KeyFrame
{
 public:

	Vec3KeyFrame() : KeyFrame(), mData(0.0f, 0.0f, 0.0f) {}

	Vec3KeyFrame(vec_t time) : KeyFrame(time), mData(0.0f, 0.0f, 0.0f) {}

	virtual ~Vec3KeyFrame() {}

	hel::Vec3 GetData() { return mData; }

	virtual void SetData(const hel::Vec3 &v) { mData = v; }

	virtual uint32 GetSerializedSize() { return 1+4+4*3; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : 
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileWriter &w) 
	{
		w.WriteByte(mFlags);
		w.WriteFloat32(mTime);
		w.WriteFloat32(mData.mVec[0]);
		w.WriteFloat32(mData.mVec[1]);
		w.WriteFloat32(mData.mVec[2]);
		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Write data from this object to disk
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::TextFileWriter &w) 
	{
		w.Print(" %f", mTime);
		w.Print(" %u", mFlags);
		w.Print(" %f %f %f\n", mData.mVec[0], mData.mVec[1], mData.mVec[2]);
		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Write data from this object to disk
	 ------------------------------------------------------*/

	virtual bool Unserialize(SystemIO::FileReader &r) 
	{ 
		mFlags = r.ReadByte();
		mTime = r.ReadFloat32();
		mData.mVec[0] = r.ReadFloat32();
		mData.mVec[1] = r.ReadFloat32();
		mData.mVec[2] = r.ReadFloat32(); 
		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read data into this object from disk
	 ------------------------------------------------------*/

	virtual bool Unserialize(SystemIO::TextFileReader &r) 
	{ 
		mTime = r.ParseFloat();
		mFlags = r.ParseInteger();
		mData.mVec[0] = r.ParseFloat();
		mData.mVec[1] = r.ParseFloat();
		mData.mVec[2] = r.ParseFloat();
		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read data into this object from disk
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	virtual bool Serialize(TiXmlElement *container)
	{
		if (!container)
			return false;

		TiXmlElement *key = new TiXmlElement("Vec3KeyFrame");
		key->SetAttribute("vertex", mFlags);
		key->SetDoubleAttribute("time", mTime);

		key->SetDoubleAttribute("x", mData.mVec[0]);
		key->SetDoubleAttribute("y", mData.mVec[1]);
		key->SetDoubleAttribute("z", mData.mVec[2]);

		container->LinkEndChild(key);

		return true;
	}

	virtual bool Unserialize(TiXmlElement *container)
	{
		if (!container)
			return false;

		{
			int attr = 0;
			container->QueryIntAttribute("flags", &attr);
			mFlags = attr;
		}

		container->QueryFloatAttribute("time", &mTime);
		container->QueryFloatAttribute("x", &mData.mVec[0]);
		container->QueryFloatAttribute("y", &mData.mVec[1]);
		container->QueryFloatAttribute("z", &mData.mVec[2]);

		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods.
	 *
	 ------------------------------------------------------*/
#endif

	hel::Vec3 mData;                      /* Keyframe data */
};


class Vec3x3KeyFrame : public KeyFrame
{
 public:

	Vec3x3KeyFrame() : KeyFrame() {}

	virtual ~Vec3x3KeyFrame() {}

	virtual uint32 GetSerializedSize() { return 1+4+4*3*3; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : 
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileWriter &w) 
	{
		w.WriteByte(mFlags);
		w.WriteFloat32(mTime);		
		for (uint32 i = 0; i < 3; ++i)
		{
			w.WriteFloat32(mData[i].mVec[0]);
			w.WriteFloat32(mData[i].mVec[1]);
			w.WriteFloat32(mData[i].mVec[2]);
		}
		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Write data from this object to disk
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::TextFileWriter &w) 
	{
		w.Print(" %f", mTime);
		w.Print(" %u", mFlags);
		for (uint32 i = 0; i < 3; ++i)
		{
			w.Print(" %f %f %f\n", mData[i].mVec[0], mData[i].mVec[1], mData[i].mVec[2]);
		}
		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Write data from this object to disk
	 ------------------------------------------------------*/

	virtual bool Unserialize(SystemIO::FileReader &r) 
	{ 
		mFlags = r.ReadByte();
		mTime = r.ReadFloat32();
		for (uint32 i = 0; i < 3; ++i)
		{
			mData[i].mVec[0] = r.ReadFloat32();
			mData[i].mVec[1] = r.ReadFloat32();
			mData[i].mVec[2] = r.ReadFloat32(); 
		}
		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read data into this object from disk
	 ------------------------------------------------------*/

	virtual bool Unserialize(SystemIO::TextFileReader &r) 
	{ 
		mTime = r.ParseFloat();
		mFlags = r.ParseInteger();
		for (uint32 i = 0; i < 3; ++i)
		{
			mData[i].mVec[0] = r.ParseFloat();
			mData[i].mVec[1] = r.ParseFloat();
			mData[i].mVec[2] = r.ParseFloat();
		}
		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read data into this object from disk
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	virtual bool Serialize(TiXmlElement *container)
	{
		if (!container)
			return false;

		TiXmlElement *key = new TiXmlElement("Vec3x3KeyFrame");
		key->SetAttribute("vertex", mFlags);
		key->SetDoubleAttribute("time", mTime);

		key->SetDoubleAttribute("tx", mData[0].mVec[0]);
		key->SetDoubleAttribute("ty", mData[0].mVec[1]);
		key->SetDoubleAttribute("tz", mData[0].mVec[2]);

		key->SetDoubleAttribute("rx", mData[1].mVec[0]);
		key->SetDoubleAttribute("ry", mData[1].mVec[1]);
		key->SetDoubleAttribute("rz", mData[1].mVec[2]);

		key->SetDoubleAttribute("sx", mData[2].mVec[0]);
		key->SetDoubleAttribute("sy", mData[2].mVec[1]);
		key->SetDoubleAttribute("sz", mData[2].mVec[2]);

		container->LinkEndChild(key);

		return true;
	}

	virtual bool Unserialize(TiXmlElement *container)
	{
		if (!container)
			return false;

		{
			int attr = 0;
			container->QueryIntAttribute("flags", &attr);
			mFlags = attr;
		}

		container->QueryFloatAttribute("time", &mTime);
		container->QueryFloatAttribute("tx", &mData[0].mVec[0]);
		container->QueryFloatAttribute("ty", &mData[0].mVec[1]);
		container->QueryFloatAttribute("tz", &mData[0].mVec[2]);

		container->QueryFloatAttribute("rx", &mData[1].mVec[0]);
		container->QueryFloatAttribute("ry", &mData[1].mVec[1]);
		container->QueryFloatAttribute("rz", &mData[1].mVec[2]);

		container->QueryFloatAttribute("sx", &mData[2].mVec[0]);
		container->QueryFloatAttribute("sy", &mData[2].mVec[1]);
		container->QueryFloatAttribute("sz", &mData[2].mVec[2]);

		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods.
	 *
	 ------------------------------------------------------*/
#endif


	hel::Vec3 mData[3];                    /* Keyframe data */
};


//////////////////////////////////////////////////////////////////////////
// Keyframe 0.9.6 API
//////////////////////////////////////////////////////////////////////////

template <typename T> 
class Keyframe
{
public:
	Keyframe(const T& data) 
		: mFlags(0x0), mTime(0.0f), mMetaData(""), mData(data) 
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	Keyframe(const T& data, const vec_t time) 
		: mFlags(0x0), mTime(time), mMetaData(""), mData(data) 
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~Keyframe() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	T& GetData() 
	{ return this->mData; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get (mutatible) Data property.
	 *
	 ------------------------------------------------------*/

	void SetData(const T& data) 
	{ this->mData = data; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Set Data property.
	 *
	 ------------------------------------------------------*/

	const char* GetMetaData() 
	{ return this->mMetaData.c_str(); }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Get MetaData property.
	 *
	 ------------------------------------------------------*/

	void SetMetaData(const char* s) 
	{ this->mMetaData = s; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Set MetaData property.
	 *
	 ------------------------------------------------------*/

	vec_t GetTime() 
	{ return this->mTime; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Get Time property.
	 *
	 ------------------------------------------------------*/

	virtual void SetTime(const vec_t time) 
	{ this->mTime = time; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Set Time property.
	 *
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileWriter& w);
	virtual bool Unserialize(SystemIO::FileReader& r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Text serialization methods.
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetSerializedSize();
	virtual bool Serialize(SystemIO::TextFileWriter& w);
	virtual bool Unserialize(SystemIO::TextFileReader& r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Binary serialization methods.
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	virtual bool Serialize(TiXmlElement* container);
	virtual bool Unserialize(TiXmlElement* container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods.
	 *
	 ------------------------------------------------------*/
#endif

 protected:

	virtual bool SerializeBase(SystemIO::FileWriter& w);
	virtual bool UnserializeBase(SystemIO::FileReader& r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Text serialization methods for base attributes.
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetSerializedSizeBase();
	virtual bool SerializeBase(SystemIO::TextFileWriter& w);
	virtual bool UnserializeBase(SystemIO::TextFileReader& r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Binary serialization methods for base attributes.
	 *
	 ------------------------------------------------------*/

#if TINYXML_FOUND
	virtual bool SerializeBase(TiXmlElement* container);
	virtual bool UnserializeBase(TiXmlElement* container);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : XML serialization methods for base attributes.
	 *
	 ------------------------------------------------------*/
#endif

	byte mFlags;       /* Used in obsolete code for interface use */

	vec_t mTime;       /* Time offset to this frame in sec */

	String mMetaData;  /* XML metadata for keyframe.  
						* Not to be confused with XML serialization. */

	T mData;           /* Aggregate of type data. */
};


////////////////////////////////////////////////////////////
// Generic methods.
////////////////////////////////////////////////////////////

template <typename T> 
uint32 Keyframe<T>::GetSerializedSizeBase()
{
	return 1 + 4 + 4 + this->mMetaData.length();
}


template <typename T> 
bool Keyframe<T>::SerializeBase(SystemIO::FileWriter &w)
{
	w.WriteByte(this->mFlags);
	w.WriteFloat32(this->mTime);

	w.WriteInt32U( this->mMetaData.length() );
	if ( this->mMetaData.length() )
	{
		w.WriteString( this->mMetaData.length(), this->mMetaData.c_str() );
	}
	
	return true;
}


template <typename T> 
bool Keyframe<T>::UnserializeBase(SystemIO::FileReader &r)
{
	this->mFlags = r.ReadByte();
	this->mTime = r.ReadFloat32();

	{
		uint32 len = r.ReadInt32U(); 
		char s[len+1];
		r.ReadString(len, s);
		s[len] = 0;
		this->mMetaData = s;
	}

	return true;
}


template <typename T> 
bool Keyframe<T>::SerializeBase(SystemIO::TextFileWriter &w)
{
	w.Print(" %u", this->mFlags);
	w.Print(" %f", this->mTime);
	w.Print(" \"%f\"", this->mMetaData.c_str() );

	return true;
}


template <typename T> 
bool Keyframe<T>::UnserializeBase(SystemIO::TextFileReader &r)
{
	this->mFlags = r.ParseInteger();
	this->mTime = r.ParseFloat();
	this->mMetaData = r.ParseStringLiteral();

	return true;
}


#if TINYXML_FOUND
template <typename T> 
bool Keyframe<T>::SerializeBase(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	keyframe->SetAttribute("flags", this->mFlags);
	keyframe->SetDoubleAttribute("time", this->mTime);

	if ( this->mMetaData.c_str() )
	{
		TiXmlElement *metadata = new TiXmlElement("metadata");
		TiXmlText *text = new TiXmlText( this->mMetaData.c_str() );
		//text->SetCDATA(true);
		metadata->LinkEndChild(text);
		keyframe->LinkEndChild(metadata);
	}

	return true;
}


template <typename T> 
bool Keyframe<T>::UnserializeBase(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	{ 
		int attr = 0;
		keyframe->QueryIntAttribute("flags", &attr);
		this->mFlags = attr;
	}

	keyframe->QueryFloatAttribute("time", &this->mTime);

	TiXmlElement *child = keyframe->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();
		
		if (s == "metadata")
		{
			this->mMetaData = child->GetText();
			break;
		}
	}

	return true;
}

#endif  // TINYXML_FOUND


////////////////////////////////////////////////////////////
// The implied class interface Specialization.
////////////////////////////////////////////////////////////

template<typename T>
uint32 Keyframe<T>::GetSerializedSize() 
{
	return this->mData.GetSerializedSize() + this->GetSerializedSizeBase(); 
}


template<typename T>
bool Keyframe<T>::Serialize(SystemIO::FileWriter &w) 
{
	this->SerializeBase(w);
	this->mData.Serialize(w);
	return true;
}


template<typename T>
bool Keyframe<T>::Unserialize(SystemIO::FileReader &r) 
{ 
	this->UnserializeBase(r);
	this->mData.Unserialize(r);
	return true;
}


template<typename T>
bool Keyframe<T>::Serialize(SystemIO::TextFileWriter &w) 
{
	this->SerializeBase(w);
	this->mData.Serialize(w);
	return true;
}


template<typename T>
bool Keyframe<T>::Unserialize(SystemIO::TextFileReader &r) 
{ 
	this->UnserializeBase(r);
	this->mData.Unserialize(r);
	return true;
}


#if TINYXML_FOUND
template<typename T>
bool Keyframe<T>::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *keyframe = new TiXmlElement( mData.GetXMLTag() );
	this->SerializeBase(keyframe);
	this->mData.Serialize(keyframe);
	container->LinkEndChild(keyframe);
	return true;
}


template<typename T>
bool Keyframe<T>::Unserialize(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	this->UnserializeBase(keyframe);
	this->mData.Unserialize(keyframe);
	return true;
}
#endif // TINYXML_FOUND


////////////////////////////////////////////////////////////
// The vec_t Specialization.
////////////////////////////////////////////////////////////

template<>
inline 
uint32 Keyframe<vec_t>::GetSerializedSize() 
{
	return 1 * 4 + GetSerializedSizeBase(); 
}


template<>
inline 
bool Keyframe<vec_t>::Serialize(SystemIO::FileWriter &w) 
{
	SerializeBase(w);
	w.WriteFloat32(mData); 
	return true;
}


template<>
inline 
bool Keyframe<vec_t>::Unserialize(SystemIO::FileReader &r) 
{ 
	UnserializeBase(r);
	mData = r.ReadFloat32();
	return true;
}


template<>
inline 
bool Keyframe<vec_t>::Serialize(SystemIO::TextFileWriter &w) 
{
	SerializeBase(w);
	w.Print(" %f\n", mData);
	return true;
}


template<>
inline 
bool Keyframe<vec_t>::Unserialize(SystemIO::TextFileReader &r) 
{ 
	UnserializeBase(r);
	mData = r.ParseFloat();
	return true; 
}


#if TINYXML_FOUND
template<>
inline 
bool Keyframe<vec_t>::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *keyframe = new TiXmlElement("VecKeyframe");
	SerializeBase(keyframe);
	keyframe->SetDoubleAttribute("v", mData);
	container->LinkEndChild(keyframe);
	return true;
}


template<>
inline 
bool Keyframe<vec_t>::Unserialize(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	UnserializeBase(keyframe);
	keyframe->QueryFloatAttribute("v", &mData);
	return true;
}
#endif // TINYXML_FOUND



////////////////////////////////////////////////////////////
// The hel::Vec3 Specialization.
////////////////////////////////////////////////////////////

template<>
inline 
uint32 Keyframe<hel::Vec3>::GetSerializedSize() 
{
	return 3 * 4 + GetSerializedSizeBase(); 
}


template<>
inline 
bool Keyframe<hel::Vec3>::Serialize(SystemIO::FileWriter &w) 
{
	SerializeBase(w);
	w.WriteFloat32(mData.mX); 
	w.WriteFloat32(mData.mY); 
	w.WriteFloat32(mData.mZ); 
	return true;
}


template<>
inline 
bool Keyframe<hel::Vec3>::Unserialize(SystemIO::FileReader &r) 
{ 
	UnserializeBase(r);
	mData.mX = r.ReadFloat32();
	mData.mY = r.ReadFloat32();
	mData.mZ = r.ReadFloat32();
	return true;
}


template<>
inline
bool Keyframe<hel::Vec3>::Serialize(SystemIO::TextFileWriter &w) 
{
	SerializeBase(w);
	w.Print(" %f %f %f\n", mData.mX, mData.mY, mData.mZ);
	return true;
}


template<>
inline 
bool Keyframe<hel::Vec3>::Unserialize(SystemIO::TextFileReader &r) 
{ 
	UnserializeBase(r);
	mData.mX = r.ParseFloat();
	mData.mY = r.ParseFloat();
	mData.mZ = r.ParseFloat();
	return true; 
}


#if TINYXML_FOUND
template<>
inline 
bool Keyframe<hel::Vec3>::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *keyframe = new TiXmlElement("Vec3Keyframe");
	SerializeBase(keyframe);
	keyframe->SetDoubleAttribute("x", mData.mX);
	keyframe->SetDoubleAttribute("y", mData.mY);
	keyframe->SetDoubleAttribute("z", mData.mZ);
	container->LinkEndChild(keyframe);
	return true;
}


template<>
inline 
bool Keyframe<hel::Vec3>::Unserialize(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	UnserializeBase(keyframe);
	keyframe->QueryFloatAttribute("x", &mData.mX);
	keyframe->QueryFloatAttribute("y", &mData.mY);
	keyframe->QueryFloatAttribute("z", &mData.mZ);
	return true;
}
#endif // TINYXML_FOUND


////////////////////////////////////////////////////////////
// The hel::Quat Specialization.
////////////////////////////////////////////////////////////

template<>
inline 
uint32 Keyframe<hel::Quat>::GetSerializedSize() 
{
	return 4 * 4 + GetSerializedSizeBase(); 
}


template<>
inline 
bool Keyframe<hel::Quat>::Serialize(SystemIO::FileWriter &w) 
{
	SerializeBase(w);
	w.WriteFloat32(mData.mX); 
	w.WriteFloat32(mData.mY); 
	w.WriteFloat32(mData.mZ); 
	w.WriteFloat32(mData.mW);
	return true;
}


template<>
inline 
bool Keyframe<hel::Quat>::Unserialize(SystemIO::FileReader &r) 
{ 
	UnserializeBase(r);
	mData.mX = r.ReadFloat32();
	mData.mY = r.ReadFloat32();
	mData.mZ = r.ReadFloat32();
	mData.mW = r.ReadFloat32();
	return true;
}


template<>
inline 
bool Keyframe<hel::Quat>::Serialize(SystemIO::TextFileWriter &w) 
{
	SerializeBase(w);
	w.Print(" %f %f %f %f\n", mData.mX, mData.mY, mData.mZ, mData.mW);
	return true;
}


template<>
inline 
bool Keyframe<hel::Quat>::Unserialize(SystemIO::TextFileReader &r) 
{ 
	UnserializeBase(r);
	mData.mX = r.ParseFloat();
	mData.mY = r.ParseFloat();
	mData.mZ = r.ParseFloat();
	mData.mW = r.ParseFloat();
	return true; 
}


#if TINYXML_FOUND
template<>
inline
bool Keyframe<hel::Quat>::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *keyframe = new TiXmlElement("QuatKeyFrame");
	SerializeBase(keyframe);
	keyframe->SetDoubleAttribute("x", mData.mX);
	keyframe->SetDoubleAttribute("y", mData.mY);
	keyframe->SetDoubleAttribute("z", mData.mZ);
	keyframe->SetDoubleAttribute("w", mData.mW);
	container->LinkEndChild(keyframe);

	return true;
}


template<>
inline 
bool Keyframe<hel::Quat>::Unserialize(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	UnserializeBase(keyframe);
	keyframe->QueryFloatAttribute("x", &mData.mX);
	keyframe->QueryFloatAttribute("y", &mData.mY);
	keyframe->QueryFloatAttribute("z", &mData.mZ);
	keyframe->QueryFloatAttribute("w", &mData.mW);
	return true; 
}
#endif // TINYXML_FOUND


////////////////////////////////////////////////////////////
// The hel::Ray Specialization.
////////////////////////////////////////////////////////////

template<>
inline 
uint32 Keyframe<hel::Ray>::GetSerializedSize() 
{
	return 6 * 4 + this->GetSerializedSizeBase(); 
}


template<>
inline 
bool Keyframe<hel::Ray>::Serialize(SystemIO::FileWriter &w) 
{
	SerializeBase(w);
	w.WriteFloat32(mData.GetOrigin().mX); 
	w.WriteFloat32(mData.GetOrigin().mY); 
	w.WriteFloat32(mData.GetOrigin().mZ); 
	w.WriteFloat32(mData.GetDir().mX); 
	w.WriteFloat32(mData.GetDir().mY); 
	w.WriteFloat32(mData.GetDir().mZ); 
	return true;
}


template<>
inline 
bool Keyframe<hel::Ray>::Unserialize(SystemIO::FileReader &r) 
{ 
	UnserializeBase(r);

	{
		vec_t x = r.ReadFloat32();
		vec_t y = r.ReadFloat32();
		vec_t z = r.ReadFloat32();
		mData.SetOrigin(x, y, z);
	}

	{
		vec_t i = r.ReadFloat32();
		vec_t j = r.ReadFloat32();
		vec_t k = r.ReadFloat32();
		mData.SetDir(i, j, k);
	}

	return true;
}


template<>
inline 
bool Keyframe<hel::Ray>::Serialize(SystemIO::TextFileWriter &w) 
{
	SerializeBase(w);
	w.Print(" %f %f %f ", mData.GetOrigin().mX, mData.GetOrigin().mY, mData.GetOrigin().mZ);
	w.Print(" %f %f %f\n", mData.GetDir().mX, mData.GetDir().mY, mData.GetDir().mZ);
	return true;
}


template<>
inline 
bool Keyframe<hel::Ray>::Unserialize(SystemIO::TextFileReader &r) 
{ 
	UnserializeBase(r);

	{
		vec_t x = r.ParseFloat();
		vec_t y = r.ParseFloat();
		vec_t z = r.ParseFloat();
		mData.SetOrigin(x, y, z);
	}

	{
		vec_t i = r.ParseFloat();
		vec_t j = r.ParseFloat();
		vec_t k = r.ParseFloat();
		mData.SetDir(i, j, k);
	}

	return true; 
}


#if TINYXML_FOUND
template<>
inline 
bool Keyframe<hel::Ray>::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *keyframe = new TiXmlElement("RayKeyFrame");
	SerializeBase(keyframe);
	keyframe->SetDoubleAttribute("x", mData.GetOrigin().mX);
	keyframe->SetDoubleAttribute("y", mData.GetOrigin().mY);
	keyframe->SetDoubleAttribute("z", mData.GetOrigin().mZ);
	keyframe->SetDoubleAttribute("i", mData.GetDir().mX);
	keyframe->SetDoubleAttribute("j", mData.GetDir().mY);
	keyframe->SetDoubleAttribute("k", mData.GetDir().mZ);

	container->LinkEndChild(keyframe);

	return true;
}


template<>
inline 
bool Keyframe<hel::Ray>::Unserialize(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	UnserializeBase(keyframe);

	{
		float x, y, z;
		keyframe->QueryFloatAttribute("x", &x);
		keyframe->QueryFloatAttribute("y", &y);
		keyframe->QueryFloatAttribute("z", &z);
		mData.SetOrigin(x, y, z);
	}

	{
		float i, j, k;
		keyframe->QueryFloatAttribute("i", &i);
		keyframe->QueryFloatAttribute("j", &j);
		keyframe->QueryFloatAttribute("k", &k);
		mData.SetDir(i, j, k);
	}

	return true; 
}
#endif // TINYXML_FOUND


////////////////////////////////////////////////////////////
// The Vector<vec_t> Specialization.
////////////////////////////////////////////////////////////

template<>
inline 
uint32 Keyframe< Vector<vec_t> >::GetSerializedSize() 
{
	return 4 + mData.end() * 4 + GetSerializedSizeBase(); 
}


template<> 
inline 
bool Keyframe< Vector<vec_t> >::Serialize(SystemIO::FileWriter &w) 
{
	SerializeBase(w);

	uint32 sz = mData.end();
	w.WriteInt32U(sz);

	for (uint32 i = 0; i < sz; ++i)
	{
		w.WriteFloat32( mData[i] );
	}

	return true;
}


template<>
inline
bool Keyframe< Vector<vec_t> >::Serialize(SystemIO::TextFileWriter &w) 
{
	SerializeBase(w);
	w.Print(" %u", mData.end());
	for (uint32 i = 0; i < mData.end(); ++i)
	{
		w.Print(" %f", mData[i]);
	}
	return true;
}


template<>
inline
bool Keyframe< Vector<vec_t> >::Unserialize(SystemIO::FileReader &r) 
{ 
	UnserializeBase(r);
	uint32 sz = r.ReadInt32U();
	mData.clear();
	mData.reserve(sz);

	for (uint32 i = 0; i < sz; ++i)
	{
		mData.push_back( r.ReadFloat32() );
	}

	return true; 
}


template<>
inline 
bool Keyframe< Vector<vec_t> >::Unserialize(SystemIO::TextFileReader &r) 
{ 
	UnserializeBase(r);
	uint32 sz = r.ParseInteger();
	mData.clear();
	mData.reserve(sz);

	for (uint32 i = 0; i < sz; ++i)
	{
		mData.push_back( r.ParseFloat() );
	}

	return true; 
}


#if TINYXML_FOUND
template<>
inline 
bool Keyframe< Vector<vec_t> >::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *key = new TiXmlElement("VertexAnimKeyFrame");
	SerializeBase(key);

	uint32 sz = mData.end();
	key->SetAttribute( "count", sz );

	for (uint32 i = 0; i < sz; )
	{
		TiXmlElement *v = new TiXmlElement("vertex");
		
		v->SetDoubleAttribute("x", mData[i++]);
		v->SetDoubleAttribute("y", mData[i++]);
		v->SetDoubleAttribute("z", mData[i++]);

		key->LinkEndChild(v);
	}

	container->LinkEndChild(key);

	return true;
}


template<> 
inline 
bool Keyframe< Vector<vec_t> >::Unserialize(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	UnserializeBase(keyframe);

	int count = 0;
	keyframe->QueryIntAttribute("count", &count);
	mData.clear();
	mData.reserve( count );

	TiXmlElement *child = keyframe->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();
	
		if (s == "vertex")
		{
			Vec3 v;
			child->QueryFloatAttribute("x", &v.mX);
			child->QueryFloatAttribute("y", &v.mY);
			child->QueryFloatAttribute("z", &v.mZ);

			mData.push_back(v.mX);
			mData.push_back(v.mY);
			mData.push_back(v.mZ);
		}
	}

	return true; 
}
#endif // TINYXML_FOUND


} // namespace freyja

#endif // GUARD__FREYJA_KEYFRAME_H_

