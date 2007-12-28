/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Vertex
 * License : No use w/o permission (C) 2004-2006 Mongoose
 * Comments: This is the freyja::Vertex class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.12.09:
 * Mongoose - Created, split from Mesh.h
 ==========================================================================*/

#include <hel/math.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include "Vertex.h"
#include "freyja.h"

using namespace mstl;
using namespace freyja;

#if 0

Vertex::Vertex(index_t vertex, index_t texcoord, index_t normal) :
	mFlags(fNone),
	mVertexIndex(vertex),
	mTexCoordIndex(texcoord),    
	mNormalIndex(normal),
	mMaterial(INDEX_INVALID),
	mFaceRefs(),
	mTmpRefs()
{
}


Vertex::Vertex() :
	mFlags(fNone),
	mVertexIndex(INDEX_INVALID),
	mTexCoordIndex(INDEX_INVALID),    
	mNormalIndex(INDEX_INVALID),
	mMaterial(INDEX_INVALID),
	mFaceRefs(),
	mTmpRefs()
{
}


Vertex::Vertex(const Vertex &v) :
	mFlags(v.mFlags),
	mVertexIndex(v.mVertexIndex),
	mTexCoordIndex(v.mTexCoordIndex),    
	mNormalIndex(v.mNormalIndex),
	mMaterial(v.mMaterial),
	mFaceRefs(v.mFaceRefs),
	mTmpRefs(v.mTmpRefs)
{
}


Vertex::~Vertex()
{
}


size_t Vertex::SerializedSize() 
{
	return ( 1 + 4 * 6); 
}


bool Vertex::Serialize(SystemIO::FileWriter &w) 
{ 
	freyja_file_chunk_t chunk;

	chunk.type = FREYJA_CHUNK_VERTEX;
	chunk.size = SerializedSize();
	chunk.flags = 0x0;
	chunk.version = 10;

	w.WriteInt8U(mFlags);
	w.WriteLong(mVertexIndex);
	w.WriteLong(mTexCoordIndex);
	w.WriteLong(mNormalIndex);
	w.WriteLong(mMaterial);

	// No use in storing face references to disk!

	return true; 
}


bool Vertex::Serialize(SystemIO::TextFileWriter &w) 
{ 
	w.Print("\t\tv %u %u %u %u %u\n", 
			mFlags, mVertexIndex, mTexCoordIndex, mNormalIndex, mMaterial);
	return true;
}


bool Vertex::Serialize(SystemIO::TextFileReader &r) 
{ 
	const char *symbol = r.ParseSymbol();
	if (symbol[0] != 'v')
		return false;

	mFlags = r.ParseInteger();
	mVertexIndex = r.ParseInteger();
	mTexCoordIndex = r.ParseInteger();
	mNormalIndex = r.ParseInteger();
	mMaterial = r.ParseInteger();

	return true;
}


#if TINYXML_FOUND

bool Vertex::Serialize(TiXmlElement *container)
{
	if (!container)
		return false;	
		
	TiXmlElement *vertex = new TiXmlElement("vertex");

	vertex->SetAttribute("flags", mFlags);
	vertex->SetAttribute("vertex", mVertexIndex);
	vertex->SetAttribute("texcoord", mTexCoordIndex);
	vertex->SetAttribute("normal", mNormalIndex);
	vertex->SetAttribute("material", mMaterial);

#if 0   // This is redundant for disk saves
	uint32 i;
	foreach (mFaceRefs, i)
	{
		TiXmlElement *element = new TiXmlElement("face-ref");
		element->SetAttribute("id", i);
		element->SetAttribute("index", mFaceRefs[i]);
		vertex->LinkEndChild(element);
	}
#endif

	container->LinkEndChild(vertex);
	return true;
}


bool Vertex::Unserialize(TiXmlElement *container)
{
	if (!container)
		return false;

	TiXmlElement *vertex = container;//->FirstChildElement("vertex");

	if (!vertex)
		return false;

	int attr;
	vertex->QueryIntAttribute("flags", &attr);
	mFlags = attr < 0 ? INDEX_INVALID : attr;

	vertex->QueryIntAttribute("vertex", &attr);
	mVertexIndex = attr < 0 ? INDEX_INVALID : attr;

	vertex->QueryIntAttribute("texcoord", &attr);
	mTexCoordIndex = attr < 0 ? INDEX_INVALID : attr;

	vertex->QueryIntAttribute("normal", &attr);
	mNormalIndex = attr < 0 ? INDEX_INVALID : attr;

	vertex->QueryIntAttribute("material", &attr);
	mMaterial = attr < 0 ? INDEX_INVALID : attr;

	TiXmlElement *child = vertex->FirstChildElement();
	for( ; child; child = child->NextSiblingElement() )
	{
		String s = child->Value();

		attr = -1;
		child->QueryIntAttribute("index", &attr);
		unsigned int idx = attr < 0 ? INDEX_INVALID : attr;

		if (s == "face-ref")
		{
			mFaceRefs.push_back(idx);
		}
	}

	return true;
}

#endif

	
void Vertex::Meld(Vertex &v)
{
	// We don't want transform operations on this anymore, so
	// we make it unselectable and hidden.
	mFlags = fHidden | fMuted;
	mVertexIndex = v.mVertexIndex;
	mTexCoordIndex = v.mTexCoordIndex;
	mNormalIndex = v.mNormalIndex;
	mMaterial = v.mMaterial;
}
	
#endif // 0
