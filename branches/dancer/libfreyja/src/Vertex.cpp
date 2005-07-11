/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Vertex
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: Vertex class, split from dancer prototype Mesh class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.11:
 * Mongoose - Created
 ==========================================================================*/

#include "Vertex.h"

using namespace freyja;


uint32 Vertex::mType = 0x54524556;
uint32 Vertex::mVersion = 1;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Vertex::Vertex()
{
	uint32 i, count;


	flags = fNone;
	mesh = INDEX_INVALID;

	/* Setup gobal pool UID reference */
	mOldUID = INDEX_INVALID;
	mUID = count = mGobalPool.size();

	for (i = 0; i < count; ++i)
	{
		if (mGobalPool[i] == 0x0)
		{
			mUID = i;
			break;
		}	
	}

	mGobalPool.assign(mUID, this);
}


Vertex::~Vertex()
{
	/* Mark NULL in pool, marking free slot */
	mGobalPool.assign(mUID, 0x0);

	/* Notify Polygons about deleted vertex */
	freyjaPolygonVertexDeleteHandler(polygonRef, mUID);
	polygonRef.clear();

	/* Bones don't depend on vertices, so no notification needed */
	weights.erase();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

index_t Vertex::findVertexIndexByOldUID(index_t oldUID)
{
	uint32 i, count = getCount();
	Vertex *v;

	if (oldUID == INDEX_INVALID)
		return INDEX_INVALID;

	for (i = 0; i < count; ++i)
	{
		v = getVertex(i);

		if (v && v->mOldUID == oldUID)
		{
			return i;
		}
	}

	return INDEX_INVALID;
}


uint32 Vertex::getCount()
{
	return mGobalPool.size();
}


uint32 Vertex::getSerializedSize()
{
	return (4 +
		4 + 1 + 4 + 12 + 12 + 12 +
		4 + 8 * weights.size() + 
		4 + 4 * polygonRef.size());
}


uint32 Vertex::getChunkType()
{
	return mType;
}


Vertex *Vertex::getVertex(index_t uid)
{
	if (uid >= getCount() || uid == INDEX_INVALID)
		return 0x0;

	return mGobalPool[uid];
}


bool Vertex::serialize(FileWriter &w)
{
	uint32 i, count;


	/* Header */
	//w.writeInt32U(mType);  // Handled layer above here
	//w.writeInt32U(getSerializedSize());
	w.writeInt32U(mVersion);

	/* Data */
	w.writeInt32U(mUID);
	w.writeInt8U(flags);
	w.writeInt32U(mesh);
	w.writeFloat32(xyz[0]);
	w.writeFloat32(xyz[1]);
	w.writeFloat32(xyz[2]);
	w.writeFloat32(normal[0]);
	w.writeFloat32(normal[1]);
	w.writeFloat32(normal[2]);
	w.writeFloat32(uvw[0]);
	w.writeFloat32(uvw[1]);
	w.writeFloat32(uvw[2]);

	/* Weights data */
	count = weights.size();
	w.writeInt32U(count);

	for (i = 0; i < count; ++i)
	{
		if (weights[i])
		{
			w.writeInt32U(weights[i]->mBoneIndex);
			w.writeFloat32(weights[i]->mWeight);
		}
		else
		{
			w.writeInt32U(INDEX_INVALID);
			w.writeFloat32(0.0f);
		}
	}

	/* Polygon references data */
	count = polygonRef.size();
	w.writeInt32U(count);

	for (i = 0; i < count; ++i)
	{
		w.writeInt32U(polygonRef[i]);
	}

	return true;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Vertex::assignToMesh(index_t meshIndex)
{
	mesh = meshIndex;
}


bool Vertex::serialize(FileReader &r)
{
	uint32 i, count, version, idx;
	vec_t weight;


	/* Header */
	//type = r.readInt32U();  // Handled layer above here
	//size = r.readInt32U();
	version = r.readInt32U();

	if (version != mVersion)
		return false;

	/* Data */
	mOldUID = r.readInt32U(); // 'Old' uid
	flags = r.readInt8U();
	flags |= fOld_UIDs; // Flag that the UIDs used are old
	mesh = r.readInt32U(); // 'Old' mesh uid

	xyz[0] = r.readFloat32();
	xyz[1] = r.readFloat32();
	xyz[2] = r.readFloat32();

	normal[0] = r.readFloat32();
	normal[1] = r.readFloat32();
	normal[2] = r.readFloat32();

	uvw[0] = r.readFloat32();
	uvw[1] = r.readFloat32();
	uvw[2] = r.readFloat32();

	/* Weights */
	count = r.readInt32U();

	for (i = 0; i < count; ++i)
	{
		idx = r.readInt32U(); // 'Old' bone UID
		weight = r.readFloat32();

		if (idx != INDEX_INVALID)
		{
			weights.pushBack(new Weight(idx, weight));
		}
	}

	/* Polygon references */
	count = r.readInt32U();

	for (i = 0; i < count; ++i)
	{
		idx = r.readInt32U();

		if (idx != INDEX_INVALID)
		{		
			polygonRef.pushBack(idx); // 'Old' polygon UID
		}
	}

	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_VERTEX
int runVertexUnitTest(int argc, char *argv[])
{
	Vertex test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Vertex class test]\n");

	return runVertexUnitTest(argc, argv);
}
#endif
