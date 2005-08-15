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

#include "FreyjaFileReader.h"
#include "FreyjaPluginABI.h"
#include "FreyjaFSM.h"

FreyjaFSM *FreyjaFSM::mFreyjaFSM = 0x0;

// FIXME Very temporary gobals until interface is done no code changes 
//       since this class is instanced as a gobal these should work fine
//       for now w/o threads until the new implementation is done
//       This is to remove exposure of Egg types in header
#include "Egg.h"

	Egg *mEgg;                          /* Pointer to the modeler backend  */

	egg_tag_t *mTag;                    /* Current tag/bolt-on and|or bone */

	egg_mesh_t *mMesh;                  /* Current mesh */

	egg_group_t *mGroup;                /* Current vertex grouping */

	egg_boneframe_t *mBoneFrame;        /* MeshTree animation frame */

	egg_animation_t *mAnimation;        /* MeshTree animation group */


FreyjaFSM::FreyjaFSM()
{
	extern Egg *gEgg;
	mEgg = gEgg;  // freyjaSpawn always allocates in proper order for this to work
	mTag = 0x0;
	mMesh = 0x0;
	mGroup = 0x0;
	mAnimation = 0x0;
	mBoneFrame = 0x0;
	mTextureId = 0;

	FreyjaFSM::mFreyjaFSM = this;
}


FreyjaFSM::~FreyjaFSM()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

uint32 FreyjaFSM::freyjaGetCount(freyja_object_t type)
{
	switch (type)
	{
	case FREYJA_MODEL:
		return 1; // NOTE: Old Egg backend has no concept of multiple models
		break;

	case FREYJA_VERTEX:
		return mEgg->getVertexCount();
		break;

	case FREYJA_TEXCOORD:
		return mEgg->getTexelCount();
		break;

	case FREYJA_MESH:
		return mEgg->getMeshCount();
		break;

	case FREYJA_VERTEX_GROUP:
		return mEgg->getGroupCount();
		break;

	case FREYJA_POLYGON:
		return mEgg->getPolygonCount();
		break;

	case FREYJA_BONE:
		return mEgg->getTagCount();
		break;

	case FREYJA_SKELETON:
		return mEgg->getBoneFrameCount();
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
		break;
	}

	return 0;
}


index_t FreyjaFSM::freyjaIterator(freyja_object_t type, index_t item)
{
	Vector<egg_vertex_t *> *vertex;
	Vector<egg_texel_t *> *texel;
	Vector<egg_mesh_t *> *mesh;
	Vector<egg_group_t *> *group;
	Vector<egg_polygon_t *> *polygon;
	Vector<egg_tag_t *> *tag;
	Vector<egg_boneframe_t *> *boneframe;
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
		tag = mEgg->TagList();
    
		if (!tag)
			return INDEX_INVALID;
 
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexBone = tag->begin();
			break;
		case FREYJA_NEXT:
			++mIndexBone;
			break;
		default:
			mIndexBone = item;
		}

		if (mIndexBone < tag->end())
		{
			mTag = (*tag)[mIndexBone];
			return mIndexBone;
		}
		break;


	case FREYJA_SKELETON:
		boneframe = mEgg->BoneFrameList();
    
		if (!boneframe)
			return INDEX_INVALID;
 
		switch (item)
		{
		case FREYJA_CURRENT:
			break;
		case FREYJA_RESET:
			mIndexSkeleton = boneframe->begin();
			break;
		case FREYJA_NEXT:
			++mIndexSkeleton;
			break;
		default:
			mIndexSkeleton = item;
		}

		if (mIndexSkeleton < boneframe->end())
		{
			return mIndexSkeleton;
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

	return INDEX_INVALID;
}


void FreyjaFSM::freyjaGetVertex(vec3_t xyz)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->VertexList();

	if (!vertex || mIndexVertex >= vertex->end() || mIndexVertex < 0)
		return;
 
	vert = (*vertex)[mIndexVertex];

	if (!vert)
		return;

	xyz[0] = vert->pos[0];
	xyz[1] = vert->pos[1];
	xyz[2] = vert->pos[2];
}


void FreyjaFSM::freyjaGetVertexTexCoord(vec2_t uv)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->VertexList();

	if (!vertex || mIndexVertex >= vertex->end())
		return;
 
	vert = (*vertex)[mIndexVertex];

	if (!vert)
		return;

	uv[0] = vert->uv[0];
	uv[1] = vert->uv[1];
}


index_t FreyjaFSM::freyjaGetBoneMeshIndex(uint32 element)
{
	if (mTag)
	{
		return mTag->mesh[element];
	}

	return INDEX_INVALID;
}


uint32 FreyjaFSM::freyjaGetBoneMeshCount()
{
	if (mTag)
	{
		return mTag->mesh.size();
	}

	return INDEX_INVALID;
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
		if (mMesh)
			return mMesh->id;
		break;

	case FREYJA_VERTEX_FRAME:
	case FREYJA_VERTEX_GROUP:
		if (mGroup)
			return mGroup->id;
		break;

	case FREYJA_POLYGON:
		return mIndexPolygon;
		break;

	case FREYJA_BONE:
		if (mTag)
			return mTag->id;
		break;

	case FREYJA_SKELETON:
		if (mBoneFrame)
			return mBoneFrame->id;
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


void FreyjaFSM::freyjaGetBoneRotate(vec_t *x, vec_t *y, vec_t *z)
{
	if (mTag)
	{
		*x = mTag->rot[0];
		*y = mTag->rot[1];
		*z = mTag->rot[2];
	}
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
		mMesh = mEgg->newMesh();
		mEgg->addMesh(mMesh);
		break;

	case FREYJA_VERTEX_FRAME:
	case FREYJA_VERTEX_GROUP:
		mStack.push(FREYJA_VERTEX_GROUP);

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
		break;

	case FREYJA_POLYGON:
		mStack.push(FREYJA_POLYGON);
		mVertexList.clear();
		mTexCoordList.clear();

		if (!mMesh)
		{
			//freyjaPrintError("freyjaBegin: WARNING, POLYGON outside MESH");
		}
		break;

	case FREYJA_BONE:
		mStack.push(FREYJA_BONE);
		mTag = mEgg->addTag(0.0, 0.0, 0.0, 0x00);
		mTag->name[0] = 0;
		mTag->parent = -1;
		break;

	case FREYJA_SKELETON:
		mStack.push(FREYJA_SKELETON);
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
	unsigned int polygon;


	switch (mStack.pop())
	{
	case FREYJA_POLYGON:
		polygon = mEgg->addPolygon(mVertexList, mTexCoordList, mTextureId);

		if (polygon == UINT_MAX)
		{
			freyjaPrintMessage("FreyjaFSM::freyjaEnd> Polygon is invalid\n");
		}

		if (mMesh)
		{
			mMesh->polygon.pushBack(polygon);
			mMesh->r_polygon.pushBack(mEgg->getPolygon(polygon));
		}
		else
		{
			freyjaPrintError("freyjaEnd: WARNING, FREYJA_POLYGON outside FREYJA_MESH");
		}
		break;

	case FREYJA_SKELETON:
		mEgg->updateBones();
		break;

	default:
		;
	}
}


index_t FreyjaFSM::freyjaTexCoordCreate2f(vec_t s, vec_t t)
{
	return mEgg->addTexel(s, t);
}


index_t FreyjaFSM::freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z)
{
	egg_vertex_t *vert;


	vert = mEgg->addVertex(x, y, z);

	if (vert)
	{
		if (mMesh)
			mMesh->vertices.pushBack(vert->id);

		if (mGroup)
		{
			mGroup->vertex.pushBack(vert->id);
		}
		else
		{
#ifdef FreyjaFSM_WARN_VERTEX_OUTSIDE_GROUP
			freyjaPrintError("freyjaVertex3f: WARNING Vertex[%i] outside GROUP!",
							 vert->id);
#endif
		}

		return vert->id;
	}
	else
		return INDEX_INVALID;
}


void FreyjaFSM::freyjaVertexWeight(index_t index, vec_t weight, index_t bone)
{
	egg_vertex_t *vert = mEgg->getVertex(index);
	egg_weight_t *vWeight;
	int emptySlot = -1;
	vec_t total = 0.0f;
	unsigned int i;


	if (!vert)
		return;

	for (i = vert->weights.begin(); i < vert->weights.end(); ++i)
	{
		vWeight = vert->weights[i];

		if (vWeight)
		{
			if (weight <= 0.0) // Remove weight
			{
				delete vWeight;
				vert->weights.assign(i, 0x0);
				return;
			}

			if (vWeight->bone == bone) // Alter weight
			{
				vWeight->weight = weight;
				return;
			}

			total = vWeight->weight;
		}
		else
		{
			emptySlot = i;
		}
	}

	if (weight <= 0.0) // Don't add dead weight ( remove requested )
		return;

	if (total + weight > 1.0)  // Just give a warning for now
		freyjaPrintError("WARNING: Weight overflow %.3f + %.3f > 1.0 not handled here %s:%d\n", 
					 total, weight, __FILE__, __LINE__);

	vWeight = new egg_weight_t;
	vWeight->weight = weight;
	vWeight->bone = bone;

	if (emptySlot > -1)
	{
		vert->weights.assign(emptySlot, vWeight);
	}
	else
	{
		vert->weights.pushBack(vWeight);
	}

	return;
}


void FreyjaFSM::freyjaGetVertexNormal(vec3_t xyz)
{
	egg_vertex_t *vert;


	vert = mEgg->getVertex(mIndexVertex);

	if (!vert)
		return;

	xyz[0] = vert->norm[0];
	xyz[1] = vert->norm[1];
	xyz[2] = vert->norm[2];
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
	if (mStack.peek() == FREYJA_MESH || !mMesh)
	{
		mMesh->flags = flags;
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
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaGroupCenter> Center defined outside GROUP!\n");
		return;
	}
}


void FreyjaFSM::freyjaBoneName(const char *name)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("FreyjaFSM::freyjaBonePos> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			strncpy(mTag->name, name, 64);
			mTag->name[63] = 0;
		}
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
		if (!mTag)
		{
			freyjaPrintError("FreyjaFSM::freyjaBonePos> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			mTag->center[0] = x;
			mTag->center[1] = y;
			mTag->center[2] = z;
		}
	}
	else if (mTag) // HACK
	{
		mTag->center[0] = x;
		mTag->center[1] = y;
		mTag->center[2] = z;
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaBonePos> Pos defined outside BONEMTAG!\n");
		return;
	}
}


void FreyjaFSM::freyjaBoneFlags(unsigned int flags)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("FreyjaFSM::freyjaBoneFlags> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			mTag->flag = flags;
		}
	}
	else if (mTag) // HACK
	{
		mTag->flag = flags;
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaBoneFlags> Flag defined outside BONEMTAG!\n");
		return;
	}
}


void FreyjaFSM::freyjaBoneAddChild(index_t boneIndex)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("FreyjaFSM::freyjaBoneAddSlave> BONEMTAG isn't allocated!\n");
		}
		else 
		{
			egg_tag_t *child =  mEgg->getTag(boneIndex);
			
			if (child)
				child->parent = mTag->id;

			// If it fails here it's got to be picked up in SKELETON 
			mTag->slave.pushBack(boneIndex);
		}
	}
	else
	{
		freyjaPrintError("FreyjaFSM::freyjaBoneAddSlave> Slave defined outside BONEMTAG!\n");
	}
}


void FreyjaFSM::freyjaBoneRotate(vec_t x, vec_t y, vec_t z)
{
	if (mTag)
	{
		mTag->rot[0] = x;
		mTag->rot[1] = y;
		mTag->rot[2] = z;
	}
}


void FreyjaFSM::freyjaSkeletonAddBone(index_t boneIndex)
{
	if (mBoneFrame)
	{
		mBoneFrame->tag.pushBack(boneIndex);
	}
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
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaBegin(type);
}


void freyjaEnd()
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaEnd();
}


uint32 freyjaGetCount(freyja_object_t type)
{
	if (FreyjaFSM::mFreyjaFSM)
		return FreyjaFSM::mFreyjaFSM->freyjaGetCount(type);

	return 0;
}


index_t freyjaIterator(freyja_object_t type, index_t item)
{
	if (FreyjaFSM::mFreyjaFSM)
		return FreyjaFSM::mFreyjaFSM->freyjaIterator(type, item);

	return INDEX_INVALID;
}


index_t freyjaGetCurrent(freyja_object_t type)
{
	if (FreyjaFSM::mFreyjaFSM)
		return FreyjaFSM::mFreyjaFSM->freyjaGetCurrent(type);

	return INDEX_INVALID;
}


index_t freyjaTexCoordCreate2f(vec_t u, vec_t v)
{
	if (FreyjaFSM::mFreyjaFSM)
		return FreyjaFSM::mFreyjaFSM->freyjaTexCoordCreate2f(u, v);

	return INDEX_INVALID;
}

void freyjaMeshFlags1u(uint32 flags)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaMeshFlags1u(flags);	
}


void freyjaGroupCenter3f(vec_t x, vec_t y, vec_t z)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaGroupCenter(x, y, z);
}


void freyjaPolygonVertex1i(index_t egg_id)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaPolygonAddVertex1i(egg_id);
}


void freyjaPolygonTexCoord1i(index_t egg_id)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaPolygonAddTexCoord1i(egg_id);
}

void freyjaPolygonMaterial1i(index_t id)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaPolygonMaterial1i(id);  
}


void freyjaSkeletonAddBone(index_t boneIndex)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaSkeletonAddBone(boneIndex);
}


index_t freyjaVertexCreate3fv(vec3_t xyz)
{
	if (FreyjaFSM::mFreyjaFSM)
		return FreyjaFSM::mFreyjaFSM->freyjaVertexCreate3f(xyz[0], xyz[1], xyz[2]);

	return INDEX_INVALID;
}


index_t freyjaVertexCreate3f(vec_t x, vec_t y, vec_t z)
{
	if (FreyjaFSM::mFreyjaFSM)
		return FreyjaFSM::mFreyjaFSM->freyjaVertexCreate3f(x, y, z);

	return INDEX_INVALID;
}


void freyjaVertexWeight(index_t index, vec_t weight, index_t bone)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaVertexWeight(index, weight, bone);
}


void freyjaGetVertex3fv(vec3_t xyz)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaGetVertex(xyz);
}


void freyjaGetVertexTexCoord2fv(vec2_t uv)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaGetVertexTexCoord(uv);
}


void freyjaGetVertexNormal3fv(vec3_t nxyz)
{
	if (FreyjaFSM::mFreyjaFSM)
		FreyjaFSM::mFreyjaFSM->freyjaGetVertexNormal(nxyz);
}

