/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : EggPlugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the python plugin handler class
 *
 *
 *-- Test Defines -------------------------------------------
 *           
 * UNIT_TEST_EGGPLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2001.02.24:
 * Mongoose - Created, based on python test code
 *
 ==========================================================================*/

#include <sys/types.h>
#include <dirent.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include <hel/math.h>
#include <hel/Quaternion.h>
#include <hel/Vector3d.h>

#include "FreyjaFileReader.h"
#include "EggPlugin.h"

EggPlugin *EggPlugin::mEggPlugin = 0x0;


EggPlugin::EggPlugin(Egg *egg)
{
	mEgg = egg;
	mFlags = 0;
	mTag = 0x0;
	mMesh = 0x0;
	mGroup = 0x0;
	mAnimation = 0x0;
	mBoneFrame = 0x0;
	mTextureId = 0;

	EggPlugin::mEggPlugin = this;
}


EggPlugin::~EggPlugin()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

uint32 EggPlugin::freyjaGetCount(freyja_object_t type)
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

	case FREYJA_MESHTREE_FRAME:
		return mEgg->getAnimationCount();
		break;

	case FREYJA_TEXTURE:
		return freyjaGetTextureCount();
		break;

	case FREYJA_ANIMATION:
	case FREYJA_VERTEX_FRAME:		
	case FREYJA_MATERIAL:
	case FREYJA_MESHTREE_TAG:
		return 0;
		break;
	}

	return 0;
}


long EggPlugin::freyjaIterator(freyja_object_t type, int item)
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
		if (item < 1 && item > 0) // I have no idea why...
		{
			return 0;
		}
		break;

	case FREYJA_VERTEX:
		vertex = mEgg->VertexList();
    
		if (!vertex)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexVertex = vertex->begin();
			break;
		case FREYJA_LIST_NEXT:
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
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexTexCoord = texel->begin();
			break;
		case FREYJA_LIST_NEXT:
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
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexMesh = mesh->begin();
			break;
		case FREYJA_LIST_NEXT:
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
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexGroup = group->begin();
			break;
		case FREYJA_LIST_NEXT:
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
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexPolygon = polygon->begin();
			break;
		case FREYJA_LIST_NEXT:
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


	case FREYJA_MESHTREE_TAG:
	case FREYJA_BONE:
		tag = mEgg->TagList();
    
		if (!tag)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexBone = tag->begin();
			break;
		case FREYJA_LIST_NEXT:
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
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexSkeleton = boneframe->begin();
			break;
		case FREYJA_LIST_NEXT:
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
	case FREYJA_MESHTREE_FRAME:
		skelanim = mEgg->AnimationList();
    
		if (!skelanim)
			return FREYJA_PLUGIN_ERROR;
 
		switch (item)
		{
		case FREYJA_LIST_CURRENT:
			break;
		case FREYJA_LIST_RESET:
			mIndexSkeletonAnim = skelanim->begin();
			break;
		case FREYJA_LIST_NEXT:
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
	case FREYJA_ANIMATION:
		break;
	}

	return FREYJA_PLUGIN_ERROR;
}


void EggPlugin::freyjaGetVertex(vec3_t xyz)
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


void EggPlugin::freyjaGetVertexTexCoord(vec2_t uv)
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


long EggPlugin::freyjaGetPolygon(freyja_object_t type, int32 item, 
								 int32 *value)
{
	Vector<egg_polygon_t *> *polygon_lst;
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;



	polygon_lst = mEgg->PolygonList();
  
	if (!polygon_lst)
		return FREYJA_PLUGIN_ERROR;

	polygon = (*polygon_lst)[mIndexPolygon];

	if (!polygon)
		return FREYJA_PLUGIN_ERROR; 

	switch (type)
	{
	case FREYJA_VERTEX:
		vert = mEgg->getVertex(polygon->vertex[item]);
    
		if (!vert)
			return FREYJA_PLUGIN_ERROR;

		*value = vert->id;
		return vert->id;
		break;
	case FREYJA_TEXCOORD:
		tex = mEgg->getTexel(polygon->vertex[item]);
    
		if (!tex)
			return FREYJA_PLUGIN_ERROR;

		*value = tex->id;
		return tex->id;
		break;
	default:
		return FREYJA_PLUGIN_ERROR;
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaGetPolygon(freyja_object_t type, long item, float *value)
{
	Vector<egg_polygon_t *> *polygon_lst;
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;



	polygon_lst = mEgg->PolygonList();
  
	if (!polygon_lst)
		return FREYJA_PLUGIN_ERROR;

	polygon = (*polygon_lst)[mIndexPolygon];

	if (!polygon)
		return FREYJA_PLUGIN_ERROR; 

	switch (type)
	{
	case FREYJA_VERTEX:
		vert = mEgg->getVertex(polygon->vertex[item]);
    
		if (!vert)
			return FREYJA_PLUGIN_ERROR;

		value[0] = vert->pos[0];
		value[1] = vert->pos[1];
		value[2] = vert->pos[2];

		return vert->id;
		break;
	case FREYJA_TEXCOORD:
		tex = mEgg->getTexel(polygon->vertex[item]);
    
		if (!tex)
			return FREYJA_PLUGIN_ERROR;

		value[0] = tex->st[0];
		value[1] = tex->st[1];
    
		return tex->id;
		break;
	default:
		return FREYJA_PLUGIN_ERROR;
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaGetBoneMeshIndex(long element)
{
	if (mTag)
	{
		return mTag->mesh[element];
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaGetBoneMeshCount()
{
	if (mTag)
	{
		return mTag->mesh.size();
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaGetCurrent(freyja_object_t type)
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

	case FREYJA_MESHTREE_TAG:
	case FREYJA_BONE:
		if (mTag)
			return mTag->id;
		break;

	case FREYJA_SKELETON:
		if (mBoneFrame)
			return mBoneFrame->id;
		break;

	case FREYJA_MESHTREE_FRAME:
		if (mAnimation)
			return mAnimation->id;
		break;

	case FREYJA_MODEL:
	case FREYJA_MATERIAL:
	case FREYJA_TEXTURE:
	case FREYJA_ANIMATION:
		return 0;
		break;
	}

	return FREYJA_PLUGIN_ERROR;
}


long EggPlugin::freyjaGetBoneRotate(vec_t *x, vec_t *y, vec_t *z)
{
	if (mTag)
	{
		*x = mTag->rot[0];
		*y = mTag->rot[1];
		*z = mTag->rot[2];
   
		return 0;
	}

	return FREYJA_PLUGIN_ERROR;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void EggPlugin::freyjaBegin(freyja_object_t type)
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
			freyjaPrintError("EggPlugin::freyjaBegin> GROUP defined outside MESH.");
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

	case FREYJA_MESHTREE_FRAME:
		mStack.push(FREYJA_MESHTREE_FRAME);
		mAnimation = new egg_animation_t;
		mEgg->addAnimation(mAnimation);
		break;

	case FREYJA_MODEL:
		mStack.push(FREYJA_MODEL);
		break;

	default:
		freyjaPrintError("freyjaBegin(%i): Unknown type", type);
		mStack.push(type);
	}
}


void EggPlugin::freyjaEnd()
{
	unsigned int polygon;


	switch (mStack.pop())
	{
	case FREYJA_POLYGON:
		polygon = mEgg->addPolygon(mVertexList, mTexCoordList, mTextureId);

		if (polygon == UINT_MAX)
		{
			freyjaPrintMessage("EggPlugin::freyjaEnd> Polygon is invalid\n");
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


long EggPlugin::freyjaTexCoord2f(vec_t s, vec_t t)
{
	return mEgg->addTexel(s, t);
}


long EggPlugin::freyjaVertex3f(vec_t x, vec_t y, vec_t z)
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
#ifdef EGGPLUGIN_WARN_VERTEX_OUTSIDE_GROUP
			freyjaPrintError("freyjaVertex3f: WARNING Vertex[%i] outside GROUP!",
							 vert->id);
#endif
		}

		return vert->id;
	}
	else
		return FREYJA_PLUGIN_ERROR;
}


void EggPlugin::freyjaVertexWeight(long index, vec_t weight, long bone)
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

			if ((int)vWeight->bone == bone) // Alter weight
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
		freyjaPrintError("WARNING: Weight overflow not handled here %s:%d\n", 
					  __FILE__, __LINE__);

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


void EggPlugin::freyjaGetVertexNormal(vec3_t xyz)
{
	egg_vertex_t *vert;


	vert = mEgg->getVertex(mIndexVertex);

	if (!vert)
		return;

	xyz[0] = vert->norm[0];
	xyz[1] = vert->norm[1];
	xyz[2] = vert->norm[2];
}


void EggPlugin::freyjaPolygonVertex1i(long index)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mVertexList.pushBack(index);
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaVertex1i> Vertex defined outside POLYGON!\n");
	}
}


void EggPlugin::freyjaMeshFlags1u(unsigned int flags)
{
	if (mStack.peek() == FREYJA_MESH || !mMesh)
	{
		mMesh->flags = flags;
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaMeshFlags1u> Flag defined outside MESH!\n");
	}
}


void EggPlugin::freyjaPolygonTexCoord1i(long index)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTexCoordList.pushBack(index);
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaTexCoord1i> Texel defined outside POLYGON!\n");
	}
}


void EggPlugin::freyjaPolygonMaterial1i(long index)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTextureId = index;
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaTexture1i> Texture defined outside POLYGON!\n");
	}
}


unsigned int EggPlugin::freyjaFlags()
{
	return mFlags;
}


void EggPlugin::freyjaFlags(unsigned int set)
{
	mFlags = set;
}


void EggPlugin::freyjaGroupCenter(vec_t x, vec_t y, vec_t z)
{
	if (mStack.peek() == FREYJA_VERTEX_GROUP)
	{
		if (!mGroup)
		{
			freyjaPrintError("EggPlugin::freyjaGroupCenter> GROUP isn't allocated!\n");
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
		freyjaPrintError("EggPlugin::freyjaGroupCenter> Center defined outside GROUP!\n");
		return;
	}
}


void EggPlugin::freyjaBoneName(char *name)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("EggPlugin::freyjaBonePos> BONEMTAG isn't allocated!\n");
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
		freyjaPrintError("EggPlugin::freyjaBonePos> Pos defined outside BONEMTAG!\n");
		return;;
	}
}


void EggPlugin::freyjaBonePos(float x, float y, float z)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("EggPlugin::freyjaBonePos> BONEMTAG isn't allocated!\n");
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
		freyjaPrintError("EggPlugin::freyjaBonePos> Pos defined outside BONEMTAG!\n");
		return;
	}
}


void EggPlugin::freyjaBoneFlags(unsigned int flags)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("EggPlugin::freyjaBoneFlags> BONEMTAG isn't allocated!\n");
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
		freyjaPrintError("EggPlugin::freyjaBoneFlags> Flag defined outside BONEMTAG!\n");
		return;
	}
}


void EggPlugin::freyjaBoneAddMesh(long mesh)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError( 
					"EggPlugin::freyjaBoneAddMesh> BONEMTAG isn't allocated!\n");
			return;
		}
		else 
		{
			mTag->mesh.pushBack(mesh);
		}
	}
	else if (mTag) // HACK
	{
		mTag->mesh.pushBack(mesh);
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaBoneAddMesh> Mesh defined outside BONEMTAG!\n");
		return;
	}
}


void EggPlugin::freyjaBoneAddChild(long tag)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			freyjaPrintError("EggPlugin::freyjaBoneAddSlave> BONEMTAG isn't allocated!\n");
		}
		else 
		{
			egg_tag_t *child =  mEgg->getTag(tag);
			
			if (child)
				child->parent = mTag->id;

			// If it fails here it's got to be picked up in SKELETON 
			mTag->slave.pushBack(tag);
		}
	}
	else
	{
		freyjaPrintError("EggPlugin::freyjaBoneAddSlave> Slave defined outside BONEMTAG!\n");
	}
}


void EggPlugin::freyjaMeshTreeAddFrame(vec_t x, vec_t y, vec_t z)
{
	if (mAnimation)
	{
		mBoneFrame = mEgg->BoneFrame(mEgg->BoneFrameAdd(x, y, z));    
		mAnimation->frame.pushBack(mBoneFrame->id);
	}
}


void EggPlugin::freyjaBoneRotate(vec_t x, vec_t y, vec_t z)
{
	if (mTag)
	{
		mTag->rot[0] = x;
		mTag->rot[1] = y;
		mTag->rot[2] = z;
	}
}


void EggPlugin::freyjaMeshTreeFrameAddBone(long tag)
{
	if (mBoneFrame)
	{
		mBoneFrame->tag.pushBack(tag);
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

#ifdef UNIT_TEST_EGGPLUGIN

int runEggPluginUnitTest(int argc, char *argv[])
{
	EggPlugin ep;

	return 0;
}


int main(int argv, char *argc[])
{
 	printf("[EggPlugin class test]\n");

	return runEggPluginUnitTest(argc, argv); 
  
	return 0;
}

#endif
