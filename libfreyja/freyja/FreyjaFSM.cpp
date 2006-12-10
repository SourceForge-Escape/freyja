/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : FreyjaFSM
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the python plugin handler class
 *
 *
 *-- Test Defines -------------------------------------------
 *           
 * UNIT_TEST_FREYJAFSM  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 ==========================================================================*/

#define USING_FREYJA_CPP_ABI

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <hel/math.h>
#include <hel/Quaternion.h>
#include <hel/Vector3d.h>
#include "freyja.h"
#include "Mesh.h"
#include "BoneABI.h"
#include "SkeletonABI.h"
#include "FreyjaPluginABI.h"
#include "FreyjaFSM.h"

using namespace freyja;

index_t gFreyjaCurrentVertex = INDEX_INVALID;
extern index_t gFreyjaCurrentMesh;
extern index_t gFreyjaCurrentModel;


FreyjaFSM *FreyjaFSM::mInstance = 0x0;


FreyjaFSM::FreyjaFSM() :
	mStack(),
	mVertexList(),
	mTexCoordList(),
	mMesh(NULL),
	mTextureId(0),
	mGroupId(0),
	mIndexModel(0),
	mIndexVertex(INDEX_INVALID), 
	mIndexTexCoord(INDEX_INVALID),
	mIndexPolygon(INDEX_INVALID),
	mIndexGroup(INDEX_INVALID),
	mIndexMesh(INDEX_INVALID),
	mIndexBone(INDEX_INVALID),
	mIndexSkeleton(INDEX_INVALID),
	mIndexSkeletonAnim(INDEX_INVALID),
	mIndexSkelKeyframe(INDEX_INVALID)
{
}


FreyjaFSM::~FreyjaFSM()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

// FIXME: Move all these gobal iterators to OBJECT based FSMs
index_t freyjaGetCurrentModelIndex()
{
	return gFreyjaCurrentModel; 
}


index_t freyjaGetCurrentVertexIndex()
{
	return gFreyjaCurrentVertex;
}


void freyjaCurrentVertex(index_t vertexIndex)
{
	gFreyjaCurrentVertex = vertexIndex;
}


uint32 FreyjaFSM::freyjaGetCount(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_MODEL:
		return freyjaGetModelCount();
		break;

	case FREYJA_VERTEX:
		return freyjaGetMeshVertexCount(gFreyjaCurrentMesh);
		break;

	case FREYJA_TEXCOORD:
		return freyjaGetMeshTexCoordCount(gFreyjaCurrentMesh);
		break;

	case FREYJA_MESH:
		return freyjaGetModelMeshCount(gFreyjaCurrentModel);
		break;

	case FREYJA_POLYGON:
		return freyjaGetMeshPolygonCount(gFreyjaCurrentMesh);
		break;

	case FREYJA_BONE:
		return freyjaGetBoneCount();
		break;

	case FREYJA_SKELETON:
		return freyjaGetSkeletonCount();
		break;

	case FREYJA_TEXTURE:
		return freyjaGetTextureCount();
		break;

	case FREYJA_MATERIAL:
		return freyjaGetMaterialCount();
		break;
		
	default:
		MARK_MSGF("%s(%s) is not implemented in this branch.", 
				  __func__, freyjaObjectToString(type) );
	}

	return 0;
}


index_t FreyjaFSM::freyjaIterator(freyja_object_t type, index_t item)
{
	switch (type)
	{
	case FREYJA_POLYGON:
		{
			Mesh *m = freyjaGetMeshClass( mIndexMesh );
			uint32 count = m ? m->GetFaceCount() : 0;

			switch (item)
			{
			case FREYJA_CURRENT:
				break;
			case FREYJA_RESET:
				mIndexPolygon = 0;
				break;
			case FREYJA_NEXT:
				++mIndexPolygon;
				break;
			default:
				mIndexPolygon = item;
			}

			if (mIndexPolygon < count)
			{
				return mIndexPolygon;
			}
		}
		break;


	case FREYJA_BONE:
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexBone = 0;
			break;
		case FREYJA_NEXT:
			++mIndexBone;
			break;
		default:
			mIndexBone = item;
		}

		if (freyjaIsBoneAllocated(mIndexBone))
		{
			return mIndexBone;
		}
		break;

	default:	
		MARK_MSGF("%s(%s) is not implemented in this branch.", 
				  __func__, freyjaObjectToString(type) );
	}

	return INDEX_INVALID;
}


void FreyjaFSM::freyjaGetVertex(vec3_t xyz)
{
	freyjaGetVertexXYZ3fv(mIndexVertex, xyz);
}


void FreyjaFSM::freyjaGetVertexTexCoord(vec2_t uv)
{
	freyjaGetVertexTexcoord2fv(mIndexVertex, uv);
}


index_t FreyjaFSM::freyjaGetCurrent(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_VERTEX:
		return mIndexVertex;
		break;

	case FREYJA_TEXCOORD:
		return mIndexTexCoord;
		break;

	case FREYJA_MESH:
#ifdef USING_EGG
		if (mMesh)
			return mMesh->id;
#else
		return mIndexMesh;
#endif
		break;

	case FREYJA_VERTEX_FRAME:
	case FREYJA_VERTEX_GROUP:
#ifdef USING_EGG
		if (mGroup)
			return mGroup->id;
#else
		return mIndexGroup;
#endif
		break;

	case FREYJA_POLYGON:
		return mIndexPolygon;
		break;

	case FREYJA_BONE:
		return mIndexBone;
		break;

	case FREYJA_SKELETON:
		return mIndexSkeleton;
		break;

	case FREYJA_SKEL_ANIMATION:
	case FREYJA_SKEL_KEYFRAME:
	case FREYJA_MODEL:
	case FREYJA_MATERIAL:
	case FREYJA_TEXTURE:
		return 0;
		break;
	}

	return INDEX_INVALID;
}


void FreyjaFSM::getBoneRotation(vec_t *x, vec_t *y, vec_t *z)
{
	vec3_t xyz;
	freyjaGetBoneRotationEuler3fv(mIndexBone, xyz);
	*x = xyz[0];
	*y = xyz[1];
	*z = xyz[2];
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaFSM::freyjaBegin(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_MESH:
		mStack.push(FREYJA_MESH);
		mIndexMesh = gFreyjaCurrentMesh = freyjaMeshCreate();
		break;

	case FREYJA_VERTEX_FRAME:
	case FREYJA_VERTEX_GROUP:
		mStack.push(FREYJA_VERTEX_GROUP);

#ifdef USING_EGG
		if (!mMesh)
		{
			freyjaPrintError("FreyjaFSM::freyjaBegin> GROUP defined outside MESH.");
		}
		else
		{
			mGroup = mEgg->newGroup();
			mGroup->flags = 0x0;
			mEgg->addGroup(mGroup);
			mMesh->group.pushBack(mGroup->id); 
			mGroup->mesh = mMesh->id;

			if (type == FREYJA_VERTEX_FRAME)
			{
				mGroup->flags = 0xBADA55;
			}
		}
#endif
		break;

	case FREYJA_POLYGON:
		mGroupId = 0;
		mStack.push(FREYJA_POLYGON);
		mVertexList.clear();
		mTexCoordList.clear();

#ifdef USING_EGG
		if (!mMesh)
		{
			//freyjaPrintError("freyjaBegin: WARNING, POLYGON outside MESH");
		}
#endif
		break;

	case FREYJA_BONE:
		mStack.push(FREYJA_BONE);
		mIndexBone = freyjaBoneCreate(mIndexSkeleton);
		freyjaSkeletonAddBone(mIndexSkeleton, mIndexBone);
		break;

	case FREYJA_SKELETON:
		mStack.push(FREYJA_SKELETON);
		mIndexSkeleton = freyjaSkeletonCreate();
		break;

	case FREYJA_MODEL:
		mStack.push(FREYJA_MODEL);
		break;

	case FREYJA_SKEL_ANIMATION:
	case FREYJA_SKEL_KEYFRAME:
	default:
		freyjaPrintError("freyjaBegin(%i): Unknown type", type);
		mStack.push(type);
	}
}


void FreyjaFSM::freyjaEnd()
{
	index_t polygon = INDEX_INVALID, vertex = INDEX_INVALID;
	uint32 i, count;


	switch (mStack.pop())
	{
	case FREYJA_POLYGON:

		polygon = freyjaMeshPolygonCreate(mIndexMesh);
		
		freyjaMeshPolygonGroup1u(mIndexMesh, polygon, mGroupId);

		for (i = 0, count = mVertexList.size(); i < count; ++i)
		{
			vertex = mVertexList[i];
			freyjaMeshPolygonAddVertex1i(mIndexMesh, polygon, vertex);

			if ( i < mTexCoordList.size() )
			{
				vertex = mTexCoordList[i];
				freyjaMeshPolygonAddTexCoord1i(mIndexMesh, polygon, vertex);
			}
		}

		mVertexList.clear();
		mTexCoordList.clear();
		break;

	case FREYJA_SKELETON:
		freyjaSkeletonUpdateBones(mIndexSkeleton);
		break;

	default:
		;
	}
}


index_t FreyjaFSM::freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z)
{
	vec3_t v = { x, y, z };
	return freyjaMeshVertexCreate3fv(mIndexMesh, v);
}


void FreyjaFSM::freyjaGetVertexNormal(vec3_t nxyz)
{
	freyjaGetVertexNormalXYZ3fv(mIndexVertex, nxyz);
}


void FreyjaFSM::freyjaPolygonGroup1u(uint32 group)
{

	if (mStack.peek() == FREYJA_POLYGON)
	{
		mGroupId = group;
	}
	else
	{
		freyjaPrintError("%s> Texture defined outside POLYGON!\n", __func__);
	}
}


void FreyjaFSM::freyjaPolygonAddVertex1i(index_t vertexIndex)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mVertexList.pushBack(vertexIndex);
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaVertex1i> Vertex defined outside POLYGON!\n");
	}
}


void FreyjaFSM::freyjaMeshFlags1u(uint32 flags)
{
	if (mStack.peek() == FREYJA_MESH)
	{
#ifdef USING_EGG
		mMesh->flags = flags;
#else
		BUG_ME("Not Implemented");
#endif
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaMeshFlags1u> Flag defined outside MESH!\n");
	}
}


void FreyjaFSM::freyjaPolygonAddTexCoord1i(index_t texcoordIndex)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTexCoordList.pushBack(texcoordIndex);
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaTexCoord1i> Texel defined outside POLYGON!\n");
	}
}


void FreyjaFSM::freyjaPolygonMaterial1i(index_t materialIndex)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTextureId = materialIndex;
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaTexture1i> Texture defined outside POLYGON!\n");
	}
}


void FreyjaFSM::freyjaGroupCenter(vec_t x, vec_t y, vec_t z)
{
	if (mStack.peek() == FREYJA_VERTEX_GROUP)
	{
#ifdef USING_EGG
		if (!mGroup)
		{
			freyjaPrintError("FreyjaFSM::freyjaGroupCenter> GROUP isn't allocated!\n");
			return;
		}
		else 
		{
			mGroup->center[0] = x;
			mGroup->center[1] = y;
			mGroup->center[2] = z;
		}
#endif
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaGroupCenter> Center defined outside GROUP!\n");
		return;
	}
}


void FreyjaFSM::boneName(const char *name)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		freyjaBoneName(mIndexBone, name);
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaBonePos> Pos defined outside BONEMTAG!\n");
		return;;
	}
}


void FreyjaFSM::freyjaBonePosition(vec_t x, vec_t y, vec_t z)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!freyjaIsBoneAllocated(mIndexBone))
		{
			freyjaPrintError("FreyjaFSM::freyjaBonePos> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			freyjaBoneTranslate3f(mIndexBone, x, y, z);
		}
	}
	else if (freyjaIsBoneAllocated(mIndexBone)) // HACK
	{
		freyjaBoneTranslate3f(mIndexBone, x, y, z);
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaBonePos> Pos defined outside BONEMTAG!\n");
		return;
	}
}


void FreyjaFSM::boneFlags(unsigned int flags)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!freyjaIsBoneAllocated(mIndexBone))
		{
			freyjaPrintError("FreyjaFSM::freyjaBoneFlags> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			freyjaBoneFlags(mIndexBone, flags);
		}
	}
	else if (freyjaIsBoneAllocated(mIndexBone)) // HACK
	{
		freyjaBoneFlags(mIndexBone, flags);
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaBoneFlags> Flag defined outside BONEMTAG!\n");
		return;
	}
}


void FreyjaFSM::boneAddChild(index_t boneIndex)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		freyjaBoneAddChild(mIndexBone, boneIndex);
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaBoneAddSlave> Slave defined outside BONEMTAG!\n");
	}
}


void FreyjaFSM::freyjaBoneRotate(vec_t x, vec_t y, vec_t z)
{
	freyjaBoneRotateEuler3f(mIndexBone, x, y, z);
}


void FreyjaFSM::skeletonAddBone(index_t boneIndex)
{
	freyjaSkeletonAddBone(mIndexSkeleton, boneIndex);
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

#ifdef UNIT_TEST_FreyjaFSM

int runFreyjaFSMUnitTest(int argc, char *argv[])
{
	FreyjaFSM ep;

	return 0;
}


int main(int argv, char *argc[])
{
 	printf("[FreyjaFSM class test]\n");

	return runFreyjaFSMUnitTest(argc, argv); 
  
	return 0;
}

#endif



////////////////////////////////////////////////////////////
// ABI
////////////////////////////////////////////////////////////

void freyjaBegin(freyja_object_t type)
{
	FreyjaFSM::GetInstance()->freyjaBegin(type);
}


void freyjaEnd()
{
	FreyjaFSM::GetInstance()->freyjaEnd();
}


uint32 freyjaGetCount(freyja_object_t type)
{
	return FreyjaFSM::GetInstance()->freyjaGetCount(type);
}


index_t freyjaIterator(freyja_object_t type, index_t item)
{
	return FreyjaFSM::GetInstance()->freyjaIterator(type, item);
}


index_t freyjaGetCurrent(freyja_object_t type)
{
	return FreyjaFSM::GetInstance()->freyjaGetCurrent(type);
}


void freyjaMeshFlags1u(uint32 flags)
{
	FreyjaFSM::GetInstance()->freyjaMeshFlags1u(flags);	
}


void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z)
{
	FreyjaFSM::GetInstance()->freyjaGroupCenter(x, y, z);
}


void freyjaPolygonVertex1i(index_t id)
{
	FreyjaFSM::GetInstance()->freyjaPolygonAddVertex1i(id);
}


void freyjaPolygonTexCoord1i(index_t id)
{
	FreyjaFSM::GetInstance()->freyjaPolygonAddTexCoord1i(id);
}


void freyjaPolygonGroup1u(uint32 id)
{
	FreyjaFSM::GetInstance()->freyjaPolygonGroup1u(id);  
}


void freyjaPolygonMaterial1i(index_t id)
{
	FreyjaFSM::GetInstance()->freyjaPolygonMaterial1i(id);  
}


#ifdef OBSOLETE_FSM_EXPORTABLE
void freyjaSkeletonAddBone(index_t boneIndex)
{
	FreyjaFSM::GetInstance()->skeletonAddBone(boneIndex);
}
#endif


index_t freyjaVertexCreate3fv(vec3_t xyz)
{
	return FreyjaFSM::GetInstance()->freyjaVertexCreate3f(xyz[0], xyz[1], xyz[2]);
}


index_t freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z)
{
	return FreyjaFSM::GetInstance()->freyjaVertexCreate3f(x, y, z);
}


void freyjaGetVertex3fv(vec3_t xyz)
{
	FreyjaFSM::GetInstance()->freyjaGetVertex(xyz);
}


void freyjaGetVertexTexCoord2fv(vec2_t uv)
{
	FreyjaFSM::GetInstance()->freyjaGetVertexTexCoord(uv);
}


void freyjaGetVertexNormal3fv(vec3_t nxyz)
{
	FreyjaFSM::GetInstance()->freyjaGetVertexNormal(nxyz);
}


//////////////////////////////////////////////////////////////////
// 0.9.3 Compatibility API Wrappers
//////////////////////////////////////////////////////////////////

#ifdef USING_EGG
#   include "Egg.h" // Left over from 9.1 API, but 9.5 it's gone
Egg *freyja__getEggBackend();
#endif


// OBSOLETE
uint32 freyjaGetAnimationCount()
{
       return 0;
}


// OBSOLETE
uint32 freyjaGetAnimationFrameCount(index_t animationIndex)
{
       return 0;
}

char freyjaIsTexCoordAllocated(index_t texcoordIndex)
{
#ifdef USING_EGG
	return (freyja__getEggBackend()->getTexel(texcoordIndex) != 0x0);
#else
	BUG_ME("Not Implemented");
	return 1;
#endif
}


char freyjaIsPolygonAllocated(index_t polygonIndex)
{
#ifdef USING_EGG
       if (freyja__getEggBackend() &&
               freyja__getEggBackend()->getPolygon(polygonIndex))
               return 1;

       return 0;
#else
	BUG_ME("Not Implemented");
	return 1;
#endif
}

