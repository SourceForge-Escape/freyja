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

#ifndef GUARD__FREYJA_VERTEXANIMKEYFRAME_H_
#define GUARD__FREYJA_VERTEXANIMKEYFRAME_H_

#if TINYXML_FOUND
#   include <tinyxml/tinyxml.h>
#endif

#include "KeyFrame.h"
#include "Track.h"


namespace freyja {

class VertexAnimKeyFrame : public KeyFrame
{
public:

	VertexAnimKeyFrame() : KeyFrame(), mVertices() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/
	
	~VertexAnimKeyFrame() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	void ArrayResize(const uint32 sz) { mVertices.resize(sz*3); } 
	/*------------------------------------------------------
	 * Pre  : <sz> is the number of *vertices to store in the array.
	 * Post : Alter the float array size.
	 *
	 ------------------------------------------------------*/

	uint32 GetVertexCount() { return mVertices.size()/3; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the number of vertices in the array.
	 *
	 ------------------------------------------------------*/
 
	vec_t *GetVertexArray() { return mVertices.get_array(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to the float array.
	 *
	 ------------------------------------------------------*/

	hel::Vec3 GetPos(const uint32 i);
	void SetPos(const uint32 i, const hel::Vec3 &pos);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vertex element's pos as roperty.
	 *
	 ------------------------------------------------------*/

	virtual uint32 GetSerializedSize() { return 4+4*mVertices.end(); }
	virtual bool Serialize(SystemIO::FileWriter &w);
	virtual bool Unserialize(SystemIO::FileReader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Binary serialization methods.
	 *
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::TextFileWriter &w);
	virtual bool Unserialize(SystemIO::TextFileReader &r);
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

	Vector<vec_t> mVertices;
};


class VertexAnimTrack : public Track
{
public:
	VertexAnimTrack() : Track() { mName = "VertexAnim"; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/
	
	~VertexAnimTrack() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	virtual KeyFrame *NewTrackKeyFrame(vec_t time) 
	{ return new VertexAnimKeyFrame(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Creates a new VertexAnimKeyFrame at <time>.
	 *
	 ------------------------------------------------------*/

	virtual VertexAnimKeyFrame *GetKeyframe(index_t idx) 
	{ return (VertexAnimKeyFrame *)Track::GetKeyframe(idx); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns keyframe by index <idx>, or NULL.
	 *
	 ------------------------------------------------------*/
};


inline 
hel::Vec3 VertexAnimKeyFrame::GetPos(const uint32 i)
{
	hel::Vec3 pos(0,0,0);

	if (i < mVertices.end())
	{
		vec_t *array = mVertices.get_array();
		uint32 idx = i * 3;
		pos.mX = array[idx  ];
		pos.mY = array[idx+1];
		pos.mZ = array[idx+2];
	}

	return pos;
}


inline 
void VertexAnimKeyFrame::SetPos(const uint32 i, const hel::Vec3 &pos)
{
	if (i < mVertices.end())
	{
		vec_t *array = mVertices.get_array();
		uint32 idx = i * 3;
		array[idx  ] = pos.mX;
		array[idx+1] = pos.mY;
		array[idx+2] = pos.mZ;
	}
}


inline 
bool VertexAnimKeyFrame::Serialize(SystemIO::FileWriter &w) 
{
	uint32 sz = mVertices.end();
	w.WriteInt32U(sz);

	for (uint32 i = 0; i < sz; ++i)
	{
		w.WriteFloat32(mVertices[i]);
	}

	return true;
}


inline 
bool VertexAnimKeyFrame::Serialize(SystemIO::TextFileWriter &w) 
{
	w.Print(" %f", mTime);
	w.Print(" %u", mFlags);
	w.Print(" %u", mVertices.end());
	for (uint32 i = 0; i < mVertices.end(); ++i)
	{
		w.Print(" %f", mVertices[i]);
	}
	return true;
}


inline 
bool VertexAnimKeyFrame::Unserialize(SystemIO::FileReader &r) 
{ 
	mVertices.clear();

	uint32 sz = r.ReadInt32U();

	for (uint32 i = 0; i < sz; ++i)
	{
		mVertices.pushBack(r.ReadFloat32());
	}

	return true; 
}


inline 
bool VertexAnimKeyFrame::Unserialize(SystemIO::TextFileReader &r) 
{ 
	mTime = r.ParseFloat();
	mFlags = r.ParseInteger();
	uint32 sz = r.ParseInteger();

	for (uint32 i = 0; i < sz; ++i)
	{
		mVertices.push_back(r.ParseFloat());
	}

	return true; 
}


inline 
bool VertexAnimKeyFrame::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *key = new TiXmlElement("VertexAnimKeyFrame");
	SerializeBase(key);

	uint32 sz = mVertices.end();
	key->SetAttribute( "count", mVertices.end() );

	for (uint32 i = 0; i < sz; )
	{
		TiXmlElement *v = new TiXmlElement("vertex");
		
		v->SetDoubleAttribute("x", mVertices[i++]);
		v->SetDoubleAttribute("y", mVertices[i++]);
		v->SetDoubleAttribute("z", mVertices[i++]);

		key->LinkEndChild(v);
	}

	container->LinkEndChild(key);

	return true;
}


inline 
bool VertexAnimKeyFrame::Unserialize(TiXmlElement *keyframe)
{
	if (!keyframe)
		return false;

	UnserializeBase(keyframe);

	int count = 0;
	keyframe->QueryIntAttribute("count", &count);

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

			mVertices.push_back(v.mX);
			mVertices.push_back(v.mY);
			mVertices.push_back(v.mZ);
		}
	}

	return true; 
}



} // namespace freyja

#endif // GUARD__FREYJA_VERTEXANIMKEYFRAME_H_
