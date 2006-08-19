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

// FIXME: Move all these gobal iterators to FSMs or the FSM singleton
index_t freyjaGetCurrentModelIndex()
{
	return 0; // Egg has a single model data structure
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

	case FREYJA_VERTEX_GROUP:
		MARK_MSGF("FREYJA_VERTEX_GROUP Obsolete");
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

	case FREYJA_SKEL_ANIMATION:
		return freyjaGetAnimationCount();
		break;

	case FREYJA_SKEL_KEYFRAME:
		return freyjaGetAnimationFrameCount(mIndexSkeletonAnim);
		break;

	case FREYJA_MATERIAL:
		return freyjaGetMaterialCount();
		break;

	case FREYJA_VERTEX_FRAME:
		MARK_MSGF("FREYJA_VERTEX_FRAME Obsolete");
		break;
		
	default:
		MARK_MSGF("Invalid state %i", type);
	}

	return 0;
}


index_t FreyjaFSM::freyjaIterator(freyja_object_t type, index_t item)
{
#ifdef USING_EGG
	Vector<egg_vertex_t *> *vertex;
	Vector<egg_texel_t *> *texel;
	Vector<egg_mesh_t *> *mesh;
	Vector<egg_group_t *> *group;
	Vector<egg_polygon_t *> *polygon;
	Vector<egg_animation_t *> *skelanim;

	switch (type)
	{
	case FREYJA_MODEL:
		return 0;  // This was disabled
		break;

	case FREYJA_VERTEX:
		vertex = mEgg->VertexList();
    
		if (!vertex)
			return INDEX_INVALID;
 
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexVertex = vertex->begin();
			break;
		case FREYJA_NEXT:
			++mIndexVertex;
			break;
		default:
			mIndexVertex = item;
		}

		if (mIndexVertex < vertex->end())
		{
			return mIndexVertex;
		}
		break;

	case FREYJA_TEXCOORD:
		texel = mEgg->TexelList();
    
		if (!texel)
			return INDEX_INVALID;
 
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexTexCoord = texel->begin();
			break;
		case FREYJA_NEXT:
			++mIndexTexCoord;
			break;
		default:
			mIndexTexCoord = item;
		}

		if (mIndexTexCoord < texel->end())
		{
			return mIndexTexCoord;
		}
		break;

	case FREYJA_MESH:
		mesh = mEgg->MeshList();
    
		if (!mesh)
			return INDEX_INVALID;
 
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexMesh = mesh->begin();
			break;
		case FREYJA_NEXT:
			++mIndexMesh;
			break;
		default:
			mIndexMesh = item;
		}

		if (mIndexMesh < mesh->end())
		{
			return mIndexMesh;
		}
		break;

	case FREYJA_VERTEX_FRAME:
	case FREYJA_VERTEX_GROUP:
		group = mEgg->GroupList();
    
		if (!group)
			return INDEX_INVALID;
 
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexGroup = group->begin();
			break;
		case FREYJA_NEXT:
			++mIndexGroup;
			break;
		default:
			mIndexGroup = item;
		}

		if (mIndexGroup < group->end())
		{
			return mIndexGroup;
		}
		break;

	case FREYJA_POLYGON:
		polygon = mEgg->PolygonList();
    
		if (!polygon)
			return INDEX_INVALID;
 
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexPolygon = polygon->begin();
			break;
		case FREYJA_NEXT:
			++mIndexPolygon;
			break;
		default:
			mIndexPolygon = item;
		}

		if (mIndexPolygon < polygon->end())
		{
			return mIndexPolygon;
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


	case FREYJA_SKELETON:
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexSkeleton = 0;
			break;
		case FREYJA_NEXT:
			++mIndexSkeleton;
			break;
		default:
			mIndexSkeleton = item;
		}

		if (freyjaIsBoneAllocated(mIndexBone))
		{
			return mIndexBone;
		}

		break;

	case FREYJA_SKEL_KEYFRAME:
		skelanim = mEgg->AnimationList();
    
		if (!skelanim)
			return INDEX_INVALID;
 
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexSkeletonAnim = skelanim->begin();
			break;
		case FREYJA_NEXT:
			++mIndexSkeletonAnim;
			break;
		default:
			mIndexSkeletonAnim = item;
		}

		if (mIndexSkeletonAnim < skelanim->end())
		{
			return mIndexSkeletonAnim;
		}
		break;

	case FREYJA_MATERIAL:
	case FREYJA_TEXTURE:
	case FREYJA_SKEL_ANIMATION:
		break;
	}
#else
	switch (type)
	{
	case FREYJA_POLYGON:
		{
			Mesh *m = freyjaModelGetMeshClass( mIndexModel, mIndexMesh );
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
		MARK_MSGF("'type = %s' Not implemented in this branch", freyjaGetObjectName(type) );
	}
#endif

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
		mIndexMesh = gFreyjaCurrentMesh = freyjaModelCreateMesh(mIndexModel);
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

		polygon = freyjaModelMeshPolygonCreate(mIndexModel, mIndexMesh);
		
		for (i = 0, count = mVertexList.size(); i < count; ++i)
		{
			vertex = mVertexList[i];
			freyjaModelMeshPolygonAddVertex1i(mIndexModel, 
											  mIndexMesh, polygon, vertex);

			if ( i < mTexCoordList.size() )
			{
				vertex = mTexCoordList[i];
				freyjaModelMeshPolygonAddTexCoord1i(mIndexModel, 
													mIndexMesh, polygon, vertex);
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
	return freyjaMeshVertexCreate3f(mIndexMesh, mIndexGroup, x, y, z);
}


void FreyjaFSM::freyjaGetVertexNormal(vec3_t nxyz)
{
	freyjaGetVertexNormalXYZ3fv(mIndexVertex, nxyz);
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

