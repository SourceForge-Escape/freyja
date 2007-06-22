/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Face
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the freyja::Face class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.12.09:
 * Mongoose - Created, split from Mesh.h
 ==========================================================================*/

#ifndef GUARD__FREYJA_FACE_H_
#define GUARD__FREYJA_FACE_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include "freyja.h"
#include "Track.h"

using hel::Vec3;


namespace freyja {

class Face
{
public:

	typedef enum {
		fNone        =  0,
		fHighlighted =  1,
		fMaterial    =  2,
		fSelected    =  4,
		fHidden      =  8,
		fRayHit      = 16,
		fPolyMappedTexCoords = 32,
		fPolyMappedNormals   = 64,
		fAlpha               = 128
	} Flags;


	Face() :
		mFlags(fNone),
		mSmoothingGroup(0),
		mColor(0),
		//mVisible(0x0),
		mMaterial(0), // Always have a valid material Id
		mNormal(0.0f, 1.0f, 0.0f),
		mIndices(),
		mTexCoordIndices(),
		mNormalsIndices(),
		mNeighbours()
	{
	}

	~Face()
	{
	}


	static const char *GetChunkType() { return "Face"; }

	static uint32 GetChunkVersion() { return 9500; /* 0.9.5-00 */ }


	void AppendNormal(index_t idx)
	{
		mFlags |= fPolyMappedNormals;
		mNormalsIndices.pushBack(idx);
	}


	void AppendTexCoord(index_t idx)
	{
		mFlags |= fPolyMappedTexCoords;
		mTexCoordIndices.pushBack(idx);
	}


	void AppendVertex(index_t idx)
	{
		// FIXME: Now you should append this Face's id to the vertex's polyref
		// FIXME: Might want to cache edge map too
		mIndices.pushBack(idx);
	}

	index_t FindCorrespondingTexCoord(index_t vertexIndex)
	{
		for (uint32 i = 0, n = mIndices.size(); i < n; ++i)
		{
			if (mIndices[i] == vertexIndex)
			{
				return mTexCoordIndices[i];
			}
		}
		
		return INDEX_INVALID;
	}


	void WeldTexCoords(index_t replace, index_t texcoord)
	{
		for (uint32 i = 0, n = mTexCoordIndices.size(); i < n; ++i)
		{
			if (mTexCoordIndices[i] == replace)
			{
				mTexCoordIndices[i] = texcoord;
				break;
			}
		}
	}


	void WeldVertices(index_t replace, index_t vertex)
	{
		for (uint32 i = 0, n = mIndices.size(); i < n; ++i)
		{
			if (mIndices[i] == replace)
			{
				mIndices[i] = vertex;
				break;
			}
		}
	}


	void PurgePolyMappedTexCoords()
	{
		mFlags &= ~fPolyMappedTexCoords;
		mTexCoordIndices.clear();
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Disables polymapped texCoords for this face 
	 *
	 ------------------------------------------------------*/

	void ClearSmoothingGroup() { mSmoothingGroup = 0; }
	void SetSmoothingGroup(uint32 g) { mSmoothingGroup = g; }
	/*------------------------------------------------------
	 * Pre  : -1 < <g> < 33 ( Relates to power of 2, bitmap ) 
	 *        Groups:  1-24,    Normal smoothing groups
	 *                25-32,    Reserved for special use
	 *
	 *        NOTE Groups/Colors may later use crease flags in other
	 *             'geometery types'.
	 *
	 * Post : 
	 *
	 ------------------------------------------------------*/

	size_t SerializedSize() { return 0; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::FileWriter &w) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileWriter &w) 
	{ 
		w.Print("\t\tface %u %u %u %u\n", 
				mFlags, mSmoothingGroup, mColor, mMaterial);

		uint32 i;
		w.Print("\t\t\tfv %u ", mIndices.size()); 
		foreach(mIndices, i)
		{
			w.Print("%u ", mIndices[i]);
		}
		w.Print("\n");

		w.Print("\t\t\tft %u ", mTexCoordIndices.size());
		foreach(mTexCoordIndices, i)
		{
			w.Print("%u ", mTexCoordIndices[i]);
		}
		w.Print("\n");

		w.Print("\t\t\tfn %u ", mNormalsIndices.size());
		foreach(mNormalsIndices, i)
		{
			w.Print("%u ", mNormalsIndices[i]);
		}
		w.Print("\n");

		return true;
	}

	bool Serialize(SystemIO::TextFileReader &r) 
	{ 
		const char *symbol = r.ParseSymbol();
		if (strncmp(symbol, "face", 4))
			return false;

		mFlags = r.ParseInteger();
		mSmoothingGroup = r.ParseInteger();
		mColor = r.ParseInteger();
		mMaterial = r.ParseInteger();

		r.ParseSymbol(); // "fv"
		for (uint32 i = 0, n = r.ParseInteger(); i < n; ++i)
		{
			mIndices.push_back(r.ParseInteger());
		}

		r.ParseSymbol(); // "ft"
		for (uint32 i = 0, n = r.ParseInteger(); i < n; ++i)
		{
			mTexCoordIndices.push_back(r.ParseInteger());
		}

		r.ParseSymbol(); // "fn"
		for (uint32 i = 0, n = r.ParseInteger(); i < n; ++i)
		{
			mNormalsIndices.push_back(r.ParseInteger());
		}

		return true;
	}


#if TINYXML_FOUND
	bool Serialize(TiXmlElement *container)
	{	
		if (!container)
			return false;	
		
		TiXmlElement *face = new TiXmlElement("face");

		face->SetAttribute("flags", mFlags);
		face->SetAttribute("group", mSmoothingGroup);
		face->SetAttribute("color", mColor);
		face->SetAttribute("material", mMaterial);

		uint32 i;
		foreach (mIndices, i)
		{
			TiXmlElement *element = new TiXmlElement("vertex");
			element->SetAttribute("id", i);
			element->SetAttribute("index", mIndices[i]);
			face->LinkEndChild(element);
		}

		foreach (mTexCoordIndices, i)
		{
			TiXmlElement *element = new TiXmlElement("texcoord");
			element->SetAttribute("id", i);
			element->SetAttribute("index", mTexCoordIndices[i]);
			face->LinkEndChild(element);
		}

		foreach (mNormalsIndices, i)
		{
			TiXmlElement *element = new TiXmlElement("normal");
			element->SetAttribute("id", i);
			element->SetAttribute("index", mNormalsIndices[i]);
			face->LinkEndChild(element);
		}

		container->LinkEndChild(face);
		return true;
	}

	bool Unserialize(TiXmlElement *container)
	{
		if (!container)
			return false;

		TiXmlElement *face = container;//->FirstChildElement("face");

		if (!face)
			return false;

		int attr;
		face->QueryIntAttribute("flags", &attr);
		mFlags = attr < 0 ? INDEX_INVALID : attr;

		face->QueryIntAttribute("group", &attr);
		mSmoothingGroup = attr < 0 ? INDEX_INVALID : attr;

		face->QueryIntAttribute("color", &attr);
		mColor = attr < 0 ? INDEX_INVALID : attr;

		face->QueryIntAttribute("material", &attr);
		mMaterial = attr < 0 ? INDEX_INVALID : attr;

		TiXmlElement *child = face->FirstChildElement();
		for( ; child; child = child->NextSiblingElement() )
		{
			String s = child->Value();
			attr = -1;

			child->QueryIntAttribute("index", &attr);
			unsigned int idx = attr < 0 ? INDEX_INVALID : attr;

			// FIXME: should check id's in future, in case of hand edited files.
			if (s == "vertex")
			{
				mIndices.push_back(idx);
			}
			else if (s == "texcoord")
			{
				mTexCoordIndices.push_back(idx);
			}
			else if (s == "normal")
			{
				mNormalsIndices.push_back(idx);
			}
		}

		return true;
	}
#endif


	byte mFlags;                      /* Options flags */

	byte mSmoothingGroup;             /* Group bit index */

	byte mColor;                      /* Reserved use for future algorithms */

	//byte mVisible;                    /* Light visible 2^(light) bitflag */

	index_t mMaterial;                /* Material index */

	Vec3 mNormal;

	mstl::Vector<index_t> mIndices;         /* Indices to the Vertex *class */

	mstl::Vector<index_t> mTexCoordIndices; /* Only used with fPolyMappedTexCoords */

	mstl::Vector<index_t> mNormalsIndices;  /* Only used with fPolyMappedNormals */

	mstl::Vector<index_t> mNeighbours;      /* Faces that share an edge with this. */
};


} // End namespace freyja


#endif // GUARD__FREYJA_FACE_H_
