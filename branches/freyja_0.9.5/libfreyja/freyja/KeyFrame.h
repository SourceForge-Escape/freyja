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


class ScaleKeyFrame : public Vec3KeyFrame
{
 public:

	ScaleKeyFrame() : Vec3KeyFrame() { mData = hel::Vec3(1.0f, 1.0f, 1.0f); }

	~ScaleKeyFrame() {}

	virtual void SetData(const hel::Vec3 &v) 
	{
		for (uint32 i = 0; i < 3; ++i)
		{
			// Scale = 0.0f undefined property, so filter it out
			if (v.mVec[i] > 0.0f)
				mData.mVec[i] = v.mVec[i]; 
		}
	}

	virtual void Scale(hel::Vec3 v) { SetData(v); }	

	virtual void Scale(vec3_t xyz) { SetData(hel::Vec3(xyz)); }	

	virtual void Scale(vec_t s) { SetData(hel::Vec3(s, s, s)); }

	virtual void ScaleX(vec_t x) { SetData(hel::Vec3(x, 1.0f, 1.0f)); }

	virtual void ScaleY(vec_t y) { SetData(hel::Vec3(1.0f, y, 1.0f)); }

	virtual void ScaleZ(vec_t z) { SetData(hel::Vec3(1.0f, 1.0f, z)); }
};


} // namespace freyja

#endif // GUARD__FREYJA_KEYFRAME_H_
