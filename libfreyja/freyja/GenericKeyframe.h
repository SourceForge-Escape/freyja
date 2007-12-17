/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : KeyFrame
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: GenericKeyFrame class.
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

#ifndef GUARD__FREYJA_GENERICKEYFRAME_H_
#define GUARD__FREYJA_GENERICKEYFRAME_H_

#include <math.h>
#include <hel/math.h>
#include <mstl/String.h>

#include "Keyframe.h"
#include "freyja.h"


template <typename T> 
class GenericKeyframe : public XMLSerializerInterface
{
public:
	GenericKeyFrame( freyja::AnimationTrack* track, vec_t time, const char* metadata, const T& data) :
		mOwner( track ),
		mTime( time ),
		mMetaData( metadata ),
		mData( data )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	GenericKeyFrame( freyja::AnimationTrack* track, vec_t time, const char* metadata, const T& data) :
		mOwner( track ),
		mTime( time ),
		mMetaData( metadata ),
		mData( data )
	{ }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~GenericKeyframe()
	{ }
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

	freyja::AnimationTrack* mOwner;  /* Track containing this keyframe. */

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

#endif // GUARD__FREYJA_GENERICKEYFRAME_H_
