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

#include <hel/Matrix.h>

#include "Vertex.h"
#include "VertexABI.h"

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


index_t Vertex::getUID()
{
	return mUID;
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


index_t Vertex::combine(index_t vertexIndexA, index_t vertexIndexB)
{
	freyjaPrintMessage("Vertex::combine(...) Not Implemented %s:%i",
					   __FILE__, __LINE__);

	return vertexIndexA;
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
// C ABI
////////////////////////////////////////////////////////////

index_t gFreyjaCurrentVertex = INDEX_INVALID;

index_t freyjaGetCurrentVertex()
{
	return gFreyjaCurrentVertex;
}


void freyjaCurrentVertex(index_t vertexIndex)
{
	if (freyjaIsVertexAllocated(vertexIndex))
		gFreyjaCurrentVertex = vertexIndex;
}


/* C Accessors */

byte freyjaIsVertexAllocated(index_t vertexIndex)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		return 1;
	}

	return 0;
}


void freyjaGetVertexNormal3fv(index_t vertexIndex, vec3_t xyz)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		xyz[0] = v->normal[0];
		xyz[1] = v->normal[1];
		xyz[2] = v->normal[2];
	}
}


index_t freyjaGetVertexPolygonRefIndex(index_t vertexIndex, uint32 element)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v && element < v->polygonRef.size())
	{
		return v->polygonRef[element];
	}

	return INDEX_INVALID;
}


uint32 freyjaGetVertexPolygonRefCount(index_t vertexIndex)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		return v->polygonRef.size();
	}

	return 0;
}


void freyjaGetVertexPosition3fv(index_t vertexIndex, vec3_t xyz)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		xyz[0] = v->xyz[0];
		xyz[1] = v->xyz[1];
		xyz[2] = v->xyz[2];
	}
}


void freyjaGetVertexTexCoord2fv(index_t vertexIndex, vec2_t uv)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		uv[0] = v->uvw[0];
		uv[1] = v->uvw[1];
		//uvw[2] = v->uvw[2];
	}
}


void freyjaGetVertexFrame(index_t vertexIndex, uint32 element,
						  index_t *frameIndex, vec3_t xyz)
{
// 	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);


// 	if (vertex)
// 	{
// 		if (element > -1 && element < (int)vertex->frames.end())
// 		{
// 			vec_t *v = *(vertex->frames[element]);

// 			if (element < (int)vertex->frameId.end())
// 				*frameIndex = vertex->frameId[element];

// 			xyz[0] = v[0];
// 			xyz[1] = v[1];
// 			xyz[2] = v[2];

// 			return 0;
// 		}

// 		/* Invalid indices return orignal position, so you don't get holes */
// 		xyz[0] = vertex->pos[0];
// 		xyz[1] = vertex->pos[1];
// 		xyz[2] = vertex->pos[2];

// 		return 0;
// 	}

// 	return -1;
}


uint32 freyjaGetVertexFrameCount(int32 vertexIndex)
{
// 	egg_vertex_t *vertex = EggPlugin::mEggPlugin->getVertex(vertexIndex);
//	
// 	if (vertex)
// 	{
// 		return vertex->frames.end();
// 	}

 	return 0;
}


void freyjaGetVertexWeight(index_t vertexIndex, uint32 element,
						   index_t *bone, vec_t *weight)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v && element < v->weights.size() && v->weights[element])
	{
		*bone = v->weights[element]->mBoneIndex;
		*weight = v->weights[element]->mWeight;
	}
}


uint32 freyjaGetVertexWeightCount(index_t vertexIndex)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		return v->weights.size();
	}

	return 0;
}


byte freyjaGetVertexFlags(index_t vertexIndex)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		return v->flags;
	}

	return 0;
}


/* C Mutators */

index_t freyjaVertexCreate3fv(const vec3_t xyz)
{
	Vertex *v = new Vertex();

	v->xyz[0] = xyz[0];
	v->xyz[1] = xyz[1];
	v->xyz[2] = xyz[2];

	return v->getUID();
}


index_t freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z)
{
	Vertex *v = new Vertex();

	v->xyz[0] = x;
	v->xyz[1] = y;
	v->xyz[2] = z;

	return v->getUID();
}


void freyjaVertexDelete(index_t vertexIndex)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		delete v;
	}
}


index_t freyjaVertexCombine(index_t vertexIndexA, index_t vertexIndexB)
{
	return Vertex::combine(vertexIndexA, vertexIndexB);
}


void freyjaVertexAddWeight(index_t vertexIndex, vec_t weight, index_t bone)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		freyjaPrintMessage("freyjaVertexAddWeight Not Implemented %s:%i",
						   __FILE__, __LINE__);
	}
}


void freyjaVertexTexCoord2fv(index_t vertexIndex, const vec2_t uv)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		v->uvw[0] = uv[0];
		v->uvw[1] = uv[1];
		v->uvw[2] = 0.0f;
	}
}


void freyjaVertexTexCoord2f(index_t vertexIndex, vec_t u, vec_t v)
{
	Vertex *vertex = Vertex::getVertex(vertexIndex);

	if (vertex)
	{
		vertex->uvw[0] = u;
		vertex->uvw[1] = v;
		vertex->uvw[2] = 0.0f;
	}
}


void freyjaVertexNormalFlip(index_t vertexIndex)
{
	Vector3d n;

	if (freyjaIsVertexAllocated(vertexIndex))
	{
		freyjaGetVertexNormal3fv(vertexIndex, n.mVec);
		n = -n;
		n.normalize();
		freyjaVertexNormal3fv(vertexIndex, n.mVec);
	}
}


void freyjaVertexNormal3fv(index_t vertexIndex, const vec3_t xyz)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		v->normal[0] = xyz[0];
		v->normal[1] = xyz[1];
		v->normal[2] = xyz[2];
	}
}


void freyjaVertexNormal3f(index_t vertexIndex, vec_t x, vec_t y, vec_t z)
{
	Vertex *v = Vertex::getVertex(vertexIndex);

	if (v)
	{
		v->normal[0] = x;
		v->normal[1] = y;
		v->normal[2] = z;
	}
}


void freyjaVertexListTransform(Vector<index_t> &list,
								freyja_transform_action_t action, 
								vec_t x, vec_t y, vec_t z)
{
	Matrix m, inverse, normalTransform;
	vec3_t xyz;
	index_t vertexIndex;
	uint32 i, count;


	if (list.empty())
		return;

	m.setIdentity();

	switch (action)
	{
	case fTranslate:
		m.translate(x, y, z);
		break;

	case fRotate:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.rotate(x, y, z);
		break;

	case fScale:
		m.scale(x, y, z);
		break;

	case fScaleAboutPoint:   // FIXME
		m.scale(x, y, z);
		break;

	case fRotateAboutPoint:  // FIXME
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.rotate(x, y, z);
		break;
	}

	m.getInvert(inverse.mMatrix);
	inverse.getTransposeMatrix(normalTransform.mMatrix);

	for (i = list.begin(), count = list.end(); i < count; ++i)
	{
		vertexIndex = list[i];

		freyjaGetVertexPosition3fv(vertexIndex, xyz);
		m.multiply3v(xyz, xyz);
		freyjaVertexPosition3fv(vertexIndex, xyz);

		freyjaGetVertexNormal3fv(vertexIndex, xyz);
		normalTransform.multiply3v(xyz, xyz);
		freyjaVertexNormal3fv(vertexIndex, xyz);
	}
}


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
