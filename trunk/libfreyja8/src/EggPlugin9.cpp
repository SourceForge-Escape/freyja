/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : GooseEgg
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Plugin
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the python plugin handler class
 *
 * 
 *-- Test Defines -------------------------------------------
 *           
 * TEST_PLUGIN  Builds module test code as a console program
 *
 *-- History ------------------------------------------------ 
 *
 * 2001-02-24:
 * Mongoose - Created, based on python test code
 *
 ==========================================================================*/

#include <dlfcn.h> 
#include <sys/types.h>
#include <dirent.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <hel/math.h>
#include <hel/Quaternion.h>
#include <hel/Vector3d.h>

#include "EggFileReader.h"

#include "EggPlugin9.h"


EggPlugin *EggPlugin::mEggPlugin = 0x0;


void eggPrintMessage(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->eggPrintMessage(format, &args);
	va_end(args);
}


void eggPrintError(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	EggPlugin::mEggPlugin->eggPrintError(format, &args);
	va_end(args);
}


int eggGenerateUVFromPlaneXY(vec3_t xyz, vec_t *u, vec_t *v)
{
	vec_t s;


	if (!u || !v)
	{
		eggPrintMessage("eggGenerateUVFromXYZ> ERROR: Passed invalid data");
		return -1;
	}

	*u = (xyz[0] > 0) ? xyz[0] : -xyz[0];
	s = 0.0025;
  
	while (*u > 1.0)
	{
		*u *= s;
		s *= 0.01;
	}
  
	*v = (xyz[1] > 0) ? xyz[1] : -xyz[1];
	s = 0.0025;
  
	while (*v > 1.0)
	{
		*v *= s;
		s *= 0.01;
	}
  
	*u = 1.0 - *u;
	*v = 1.0 - *v;

	return 0;
}





Vector<unsigned int> *eggFindVerticesByBox(bbox_t bbox)
{
	Vector<unsigned int> *list;
	unsigned int i, count;
	vec3_t xyz;


	eggCriticalSection(EGG_WRITE_LOCK);

	count = eggGetNum(FREYJA_VERTEX); 

	if (count == PLUGIN_ERROR)
	{
		return 0x0;
	}

	list = new Vector<unsigned int>();

	/* Using egg iterator interface */
	eggIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (i = 0; i < count; ++i)
	{
		eggGetVertex3f(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(eggIterator(FREYJA_VERTEX, 
											   FREYJA_LIST_CURRENT));
				}
			}
		}

		eggIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}

	eggCriticalSection(EGG_WRITE_UNLOCK);
	
	return list;
}


Vector<unsigned int> *eggFindVerticesInBox(bbox_t bbox,
										   Vector<unsigned int> &vertices)
{
	Vector<unsigned int> *list;
	unsigned int i, count;
	vec3_t xyz;


	eggCriticalSection(EGG_WRITE_LOCK);

	list = new Vector<unsigned int>();

	/* Using egg iterator interface */
	for (i = 0; i < count; ++i)
	{
		if (PLUGIN_ERROR == eggIterator(FREYJA_VERTEX, vertices[i]))
			continue;

		eggGetVertex3f(xyz);
		
		if (xyz[0] >= bbox[0][0] && xyz[0] <= bbox[1][0])
		{
			if (xyz[1] >= bbox[0][1] && xyz[1] <= bbox[1][1])
			{
				if (xyz[0] >= bbox[0][2] && xyz[0] <= bbox[1][2])
				{
					list->pushBack(eggIterator(FREYJA_VERTEX, 
											   FREYJA_LIST_CURRENT));
				}
			}
		}
	}

	eggCriticalSection(EGG_WRITE_UNLOCK);
	
	return list;
}


void eggGenerateVertexNormals()
{
	Vector <Vector3d *> faceNormals;
	Vector3d a, b, c, aa, bb, normal;
	unsigned int i, j, v0, v1, v2, vertexCount, faceCount;


	eggCriticalSection(EGG_WRITE_LOCK);
	
	vertexCount = eggGetNum(FREYJA_VERTEX);
	faceCount = eggGetNum(FREYJA_POLYGON); 

	eggIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);

    for (i = 0; i < faceCount; ++i)
    {
		eggGetPolygon1u(FREYJA_VERTEX, 0, &v0);
		eggGetPolygon1u(FREYJA_VERTEX, 1, &v1);
		eggGetPolygon1u(FREYJA_VERTEX, 2, &v2);

		eggPrintMessage("<%d %d %d>\n", v0, v1, v2);
		eggIterator(FREYJA_VERTEX, v0);
		eggGetVertex3f(a.mVec);
		eggIterator(FREYJA_VERTEX, v1);
		eggGetVertex3f(b.mVec);
		eggIterator(FREYJA_VERTEX, v2);
		eggGetVertex3f(c.mVec);


		/* Compute 2 vectors from the triangle face */	
		//aa = b - a;
		//bb = b - c;
		
		/* Compute normal for the face, and store it */
		normal = Vector3d::cross(b - a, b - c);
		faceNormals.pushBack(new Vector3d(normal));

		eggIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
	}

	eggIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);

	/* Compute vertex normals */
    for (i = 0; i < vertexCount; ++i)
    {
		egg_vertex_t *vertex = EggPlugin::mEggPlugin->eggGetVertex(eggIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT));
		eggIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);

		if (!vertex)
		{
			eggPrintMessage("ERROR\n");
			continue;
		}

		normal.zero();

		eggPrintMessage("%d :: %d faces\n", vertex->id, vertex->ref.size());
		for (j = vertex->ref.begin(); j < vertex->ref.end(); ++j)
		{
			normal += *faceNormals[j];
		}

		normal.normalize();

		vertex->norm[0] = normal.mVec[0];
		vertex->norm[1] = normal.mVec[1];
		vertex->norm[2] = normal.mVec[2];

		eggPrintMessage("%d :: %f %f %f\n", vertex->id, 
			   normal.mVec[0],
			   normal.mVec[1],
			   normal.mVec[2]);
    }

	faceNormals.erase();
	
	eggCriticalSection(EGG_WRITE_UNLOCK);
}


////////////////////////////////////////////////////////////////////


unsigned int eggBegin(egg_plugin_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggBegin(type);

	return PLUGIN_ERROR;
}


unsigned int eggEnd()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggEnd();

	return PLUGIN_ERROR;
}


int eggTextureStore(EggTextureData *textureData)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTextureStore(textureData);

	return -1;
}


unsigned int eggTexCoordStore2f(float s, float t)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTexCoordStore2f(s, t);

	return PLUGIN_ERROR;
}


unsigned int eggVertexStore3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggVertexStore3f(x, y, z);

	return PLUGIN_ERROR;
}


void eggVertexWeightStore(unsigned int index, 
						  vec_t weight, unsigned int bone)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertexWeightStore(index, weight, bone);
}


void eggVertexUVStore2f(unsigned int vIndex, float u, float v)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertexUVStore2f(vIndex, u, v);
}


void eggVertexNormalStore3f(unsigned int vIndex, float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertexNormalStore3f(vIndex, x, y, z);
}


void eggVertex1i(unsigned int egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggVertex1i(egg_id);
}


void eggTexCoord1i(unsigned int egg_id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggTexCoord1i(egg_id);
}


void eggTexture1i(int id)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggTexture1i(id);  
}


unsigned int eggFlags()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggFlags();

	return PLUGIN_ERROR;
}


unsigned int eggGroupCenter3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGroupCenter(x, y, z);

	return PLUGIN_ERROR;
}


unsigned int eggTagName(char *name)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagName(name);

	return PLUGIN_ERROR;  
}

unsigned int eggTagPos3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagPos(x, y, z);

	return PLUGIN_ERROR;  
}


unsigned int eggTagFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagFlags(flags);

	return PLUGIN_ERROR;
}


unsigned int eggTagAddMesh1u(unsigned int mesh)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagAddMesh(mesh);

	return PLUGIN_ERROR;
}


unsigned int eggTagAddSlave1u(unsigned int tag)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagAddSlave(tag);

	return PLUGIN_ERROR;
}


unsigned int eggMeshTreeAddFrame3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggMeshTreeAddFrame(x, y, z);

	return PLUGIN_ERROR;
}


unsigned int eggTagRotate3f(float x, float y, float z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTagRotate(x, y, z);

	return PLUGIN_ERROR;
}


unsigned int eggTagRotateQuaternion4f(vec_t w, vec_t x, vec_t y, vec_t z)
{
	vec_t heading, bank, attitude;
	Quaternion q = Quaternion(w, x, y, z);

	q.getEulerAngles(&heading, &bank, &attitude);

	// For now just switch to eular ( ew ) and do I have this right?
	if (EggPlugin::mEggPlugin) // h b a; a b h
		return EggPlugin::mEggPlugin->eggTagRotate(attitude*57.295779513082323,
												   bank*57.295779513082323, 
												   heading*57.295779513082323);

	return PLUGIN_ERROR;
}


unsigned int eggMeshTreeTag1u(unsigned int tag)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggMeshTreeTag(tag);

	return PLUGIN_ERROR;
}

int eggGetTextureFilename(unsigned int index, char **filename)
{
	*filename = 0x0;

	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTextureFilename(index, 
															filename);
	return PLUGIN_ERROR;	
}

int eggGetTextureImage(unsigned int index, unsigned int *w, unsigned int *h, 
					   unsigned int *depth,  unsigned int *type,
					   unsigned char **image)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTextureImage(index, w, h, 
														 depth, type, image);
	return PLUGIN_ERROR;
}


unsigned int eggTextureStoreFilename(char *filename)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTextureStoreFilename(filename);

	return PLUGIN_ERROR;
}

unsigned int eggTextureStoreBuffer(unsigned char *image, unsigned int depth,
								   unsigned int width, unsigned int height,
								   egg_colormode_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggTextureStoreBuffer(image, depth,
															width, height,
															type);

	return PLUGIN_ERROR;
  
}


// Accesors /////////////////////////////////////

unsigned int eggGetNum(egg_plugin_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetNum(type);

	return PLUGIN_ERROR;
}


unsigned int eggIterator(egg_plugin_t type, int item)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggIterator(type, item);

	return PLUGIN_ERROR;
}


unsigned int eggGetVertex3f(float *xyz)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetVertex(xyz);

	return PLUGIN_ERROR;
}


unsigned int eggGetVertexNormal3f(float *xyz)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetVertexNormal(xyz);

	return PLUGIN_ERROR;
}


unsigned int eggGetPolygon1u(egg_plugin_t type, int item, 
							 unsigned int *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetPolygon(type, item, value);

	return PLUGIN_ERROR;
}


unsigned int eggGetPolygon3f(egg_plugin_t type, int item, float *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetPolygon(type, item, value);

	return PLUGIN_ERROR;
}

unsigned int eggCriticalSection(egg_lock_t request)
{
	eggPrintMessage("eggCriticalSection> Not implemented, %s:%i\n", 
		   __FILE__, __LINE__);

	return PLUGIN_ERROR;
}

unsigned int eggGetBoneName(unsigned int index, unsigned int size, char *name)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->eggGetBone(index);
		
	if (bone)
	{
		strncpy(name, bone->name, size);

		return bone->id;
	}

	return PLUGIN_ERROR;
}


void eggSetBoneParent(int index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->eggGetBone(index);


	if (bone)
	{
		bone->parent = index;
	}	
}


int eggGetBoneParent(unsigned int index)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->eggGetBone(index);


	if (bone)
	{
		return bone->parent;
	}

	return -2;
}

unsigned int eggGetBoneRotationXYZW4fv(unsigned int index, vec4_t xyzw)
{
	egg_tag_t *bone = EggPlugin::mEggPlugin->eggGetBone(index);
	
	if (bone)
	{
		Quaternion q = Quaternion(bone->rot[0], bone->rot[1], bone->rot[2]);
		vec4_t wxyz;

		q.getQuaternion4fv(wxyz);
		
		xyzw[0] = wxyz[0];
		xyzw[1] = wxyz[1];
		xyzw[2] = wxyz[2];
		xyzw[3] = wxyz[3];

		return bone->id;
	}

	return PLUGIN_ERROR;
}

unsigned int eggGetBoneTranslation3fv(unsigned int index, vec3_t xyz)
{
	if (EggPlugin::mEggPlugin)
	{
		egg_tag_t *bone = EggPlugin::mEggPlugin->eggGetBone(index);

		xyz[0] = bone->center[0];
		xyz[1] = bone->center[1];
		xyz[2] = bone->center[2];

		return bone->id;
	}

	return PLUGIN_ERROR;
}


unsigned int eggGetTagMesh1u(unsigned int item, unsigned int *value)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTagMesh(item, value);

	return PLUGIN_ERROR;
}


unsigned int eggGetCurrent(egg_plugin_t type)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetCurrent(type);

	return PLUGIN_ERROR;
}


unsigned int eggGetTagMeshNum()
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTagMeshNum();

	return PLUGIN_ERROR;
}


unsigned int eggGetTagRotate3f(float *x, float *y, float *z)
{
	if (EggPlugin::mEggPlugin)
		return EggPlugin::mEggPlugin->eggGetTagRotate(x, y, z);

	return PLUGIN_ERROR;
}

void eggMeshFlags1u(unsigned int flags)
{
	if (EggPlugin::mEggPlugin)
		EggPlugin::mEggPlugin->eggMeshFlags1u(flags);	
}

////////////////////////////////////////////////////////////


EggPlugin::EggPlugin(Egg *egg, char *plugin_dir)
{
	/* Backend, data model */
	mFlags = 0;
	mEgg = egg;

	/* State machine's 3d model object pointers */
	mModel = 0x0;

	mVertexGroup = 0x0;

	mMesh = 0x0;
	mVertexFrame = 0x0;
	mUVMap = 0x0;
	mMaterial = 0x0;

	mSkeleton = 0x0;
	mBone = 0x0;

	mAnimation = 0x0;


	/* Plugin directory setup */
	if (plugin_dir && plugin_dir[0])
	{
		long len = strlen(plugin_dir);

		mPluginDir = new char[len + 2];
		strncpy(mPluginDir, plugin_dir, len);
		mPluginDir[len+1] = 0;
	}
	else
	{
		mPluginDir = new char[64];
		strcpy(mPluginDir, "/usr/share/freyja/plugins/");    
	}

	EggPlugin::mEggPlugin = this;
}


EggPlugin::~EggPlugin()
{
	mTextures.erase();
	mTextureFiles.erase();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

unsigned int EggPlugin::eggGetNum(egg_plugin_t type)
{
	switch (type)
	{
	case FREYJA_VERTEX:
		return mEgg->getVertexCount();
		break;
	case FREYJA_TEXCOORD:
		return mEgg->getTexelCount();
		break;
	case FREYJA_MESH:
		return mEgg->getMeshCount();
		break;
	case FREYJA_GROUP:
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
	case FREYJA_SKEL_ANIM:
		return mEgg->getAnimationCount();
		break;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggIterator(egg_plugin_t type, int item)
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
	case FREYJA_VERTEX:
		vertex = mEgg->VertexList();
    
		if (!vertex)
			return PLUGIN_ERROR;
 
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
			return PLUGIN_ERROR;
 
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
			return PLUGIN_ERROR;
 
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
	case FREYJA_GROUP:
		group = mEgg->GroupList();
    
		if (!group)
			return PLUGIN_ERROR;
 
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
			return PLUGIN_ERROR;
 
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
	case FREYJA_BONE:
		tag = mEgg->TagList();
    
		if (!tag)
			return PLUGIN_ERROR;
 
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
			return mIndexBone;
		}
		break;
	case FREYJA_SKELETON:
		boneframe = mEgg->BoneFrameList();
    
		if (!boneframe)
			return PLUGIN_ERROR;
 
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
	case FREYJA_SKEL_ANIM:
		skelanim = mEgg->AnimationList();
    
		if (!skelanim)
			return PLUGIN_ERROR;
 
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
		//	case FREYJA_MESHTREE_ANIM:
		//		printf("EggPlugin::eggIterator> %s Not implemented, %s:%i\n", 
		//			   "FREYJAMANIMATION_ANIM", __FILE__, __LINE__);
		//		break;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggGetVertex(float *xyz)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->VertexList();

	if (!vertex || mIndexVertex >= vertex->end())
		return PLUGIN_ERROR;
 
	vert = (*vertex)[mIndexVertex];

	if (!vert)
		return PLUGIN_ERROR;

	xyz[0] = vert->pos[0];
	xyz[1] = vert->pos[1];
	xyz[2] = vert->pos[2];

	return vert->id;
}


unsigned int EggPlugin::eggGetVertexNormal(float *xyz)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *vert;


	vertex = mEgg->VertexList();

	if (!vertex || mIndexVertex >= vertex->end())
		return PLUGIN_ERROR;
 
	vert = (*vertex)[mIndexVertex];

	if (!vert)
		return PLUGIN_ERROR;

	xyz[0] = vert->norm[0];
	xyz[1] = vert->norm[1];
	xyz[2] = vert->norm[2];

	return vert->id;
}


unsigned int EggPlugin::eggGetPolygon(egg_plugin_t type, int item, 
									  unsigned int *value)
{
	Vector<egg_polygon_t *> *polygon_lst;
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;



	polygon_lst = mEgg->PolygonList();
  
	if (!polygon_lst)
		return PLUGIN_ERROR;

	polygon = (*polygon_lst)[mIndexPolygon];

	if (!polygon)
		return PLUGIN_ERROR; 

	switch (type)
	{
	case FREYJA_VERTEX:
		vert = mEgg->getVertex(polygon->vertex[item]);
    
		if (!vert)
			return PLUGIN_ERROR;

		*value = vert->id;
		return vert->id;
		break;
	case FREYJA_TEXCOORD:
		tex = mEgg->getTexel(polygon->vertex[item]);
    
		if (!tex)
			return PLUGIN_ERROR;

		*value = tex->id;
		return tex->id;
		break;
	default:
		return PLUGIN_ERROR;
	}

	return PLUGIN_ERROR;
}


egg_tag_t *EggPlugin::eggGetBone(unsigned int index)
{
	Vector<egg_tag_t *> *bones;
	egg_tag_t *bone;
	
	bones = mEgg->TagList();

	if (!bones || bones->empty() || index > bones->size()-1)
		return 0x0;

	bone = (*bones)[index];

	return bone;
}


egg_vertex_t *EggPlugin::eggGetVertex(unsigned int index)
{
	Vector<egg_vertex_t *> *vertex;
	egg_vertex_t *v;
	
	vertex = mEgg->VertexList();

	if (!vertex || vertex->empty() || index > vertex->size()-1)
		return 0x0;

	v = (*vertex)[index];

	return v;
}


unsigned int EggPlugin::eggGetPolygon(egg_plugin_t type, int item, 
									  float *value)
{
	Vector<egg_polygon_t *> *polygon_lst;
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;



	polygon_lst = mEgg->PolygonList();
  
	if (!polygon_lst)
		return PLUGIN_ERROR;

	polygon = (*polygon_lst)[mIndexPolygon];

	if (!polygon)
		return PLUGIN_ERROR; 

	switch (type)
	{
	case FREYJA_VERTEX:
		vert = mEgg->getVertex(polygon->vertex[item]);
    
		if (!vert)
			return PLUGIN_ERROR;

		value[0] = vert->pos[0];
		value[1] = vert->pos[1];
		value[2] = vert->pos[2];

		return vert->id;
		break;
	case FREYJA_TEXCOORD:
		tex = mEgg->getTexel(polygon->vertex[item]);
    
		if (!tex)
			return PLUGIN_ERROR;

		value[0] = tex->st[0];
		value[1] = tex->st[1];
    
		return tex->id;
		break;
	default:
		return PLUGIN_ERROR;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggGetTagMesh(unsigned int item, unsigned int *value)
{
	if (mTag)
	{
		*value = mTag->mesh[item];
		return 0;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggGetTagMeshNum()
{
	if (mTag)
	{
		return mTag->mesh.size();
	}

	return PLUGIN_ERROR;
}


// FIXME: Not implmented fully
unsigned int EggPlugin::eggGetCurrent(egg_plugin_t type)
{
	switch (type)
	{
	case FREYJA_VERTEX:
		break;

	case FREYJA_TEXCOORD:
		break;

	case FREYJA_MESH:
		if (mMesh)
			return mMesh->id;
		break;

	case FREYJA_GROUP:
		if (mGroup)
			return mGroup->id;
		break;

	case FREYJA_POLYGON:
		break;

	case FREYJA_BONE:
		if (mTag)
			return mTag->id;
		break;

	case FREYJA_SKELETON:
		if (mBoneFrame)
			return mBoneFrame->id;
		break;

	case FREYJA_SKEL_ANIM:
		if (mAnimation)
			return mAnimation->id;
		break;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggGetTagRotate(float *x, float *y, float *z)
{
	if (mTag)
	{
		*x = mTag->rot[0];
		*y = mTag->rot[1];
		*z = mTag->rot[2];
   
		return 0;
	}

	return PLUGIN_ERROR;
}

	
void EggPlugin::eggPrintError(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	eggPrintError(format, &args);
	va_end(args);
}


void EggPlugin::eggPrintError(char *format, va_list *args)
{
	vfprintf(stderr, format, *args);
}


void EggPlugin::eggPrintMessage(char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	eggPrintMessage(format, &args);
	va_end(args);
}


void EggPlugin::eggPrintMessage(char *format, va_list *args)
{
	vfprintf(stdout, format, *args);
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void EggPlugin::importPlugins(char *directory)
{
#ifdef INTERACTIVE_PLUGINS
	EggFileReader reader;
	int (*import)(char *filename);
	int (*check)(char *filename);
	bool done = false;
	char *module_filename;
	void *handle;
	char *error;
	unsigned char *image = 0x0;
	unsigned int width = 0, height = 0;
	char type = 0;


	if (!reader.doesFileExist(filename))
	{
		print("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	print("[EggImage plugin system invoked]");

	for (i = mPluginDirectorys.begin(); i < mPluginDirectorys.end(); ++i)
	{
		if (!reader.openDirectory(mPluginDir))
		{
			eggPrintError("Couldn't access image plugin directory");
			continue;
		}

		while (!done && (module_filename = reader.getNextDirectoryListing()))
		{
			if (reader.isDirectory(module_filename))
				continue;

			if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
			{
				printError("In module '%s'.", module_filename);
				
				if ((error = dlerror()) != NULL)  
				{
					printError("%s", error);
				}

				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				print("Module '%s' opened.", module_filename);
				
				import = (int (*)(char *filename))dlsym(handle, "import_model");

				if ((error = dlerror()) != NULL)  
				{
					printError("%s", error);
					dlclose(handle);
					continue;
				}
				
				done = !(*import)((char*)filename);
				
				if ((error = dlerror()) != NULL) 
				{
					printError("%s", error);
					dlclose(handle);
					continue;
				}
				
				dlclose(handle);
			}
		}

		reader.closeDirectory();

		if (done)
			break;
	}

	print("[EggPlugin module loader sleeps now]\n");
#else
	print("EggPlugin: This build was compiled w/o plugin support");
#endif

	return -1;
}


int EggPlugin::importModel(const char *filename)
{
#ifdef EGGIMAGE_PLUGINS
	EggFileReader reader;
	int (*import)(char *filename);
	int (*check)(char *filename);
	bool done = false;
	char *module_filename;
	void *handle;
	char *error;
	unsigned char *image = 0x0;
	unsigned int width = 0, height = 0;
	char type = 0;


	if (!reader.doesFileExist(filename))
	{
		print("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	print("[EggImage plugin system invoked]");

	for (i = mPluginDirectorys.begin(); i < mPluginDirectorys.end(); ++i)
	{
		if (!reader.openDirectory(mPluginDir))
		{
			eggPrintError("Couldn't access image plugin directory");
			continue;
		}

		while (!done && (module_filename = reader.getNextDirectoryListing()))
		{
			if (reader.isDirectory(module_filename))
				continue;

			if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
			{
				printError("In module '%s'.", module_filename);
				
				if ((error = dlerror()) != NULL)  
				{
					printError("%s", error);
				}

				continue; /* Try the next plugin, after a bad module load */
			}
			else
			{
				print("Module '%s' opened.", module_filename);
				
				import = (int (*)(char *filename))dlsym(handle, "import_model");

				if ((error = dlerror()) != NULL)  
				{
					printError("%s", error);
					dlclose(handle);
					continue;
				}
				
				done = !(*import)((char*)filename);
				
				if ((error = dlerror()) != NULL) 
				{
					printError("%s", error);
					dlclose(handle);
					continue;
				}
				
				dlclose(handle);
			}
		}

		reader.closeDirectory();

		if (done)
			break;
	}

	print("[EggPlugin module loader sleeps now]\n");
#else
	print("EggPlugin: This build was compiled w/o plugin support");
#endif

	return -1;
}


int EggPlugin::importModel(char *filename)
{
#ifdef MODEL_PLUGINS
	bool loaded = false;
	char module_filename[128];
	char module_check[128];
	char module_import[128];
	char *name;
	int (*import)(char *filename);
	int (*check)(char *filename);
	void *handle;
	char *error;
	unsigned int i;


	if (!mEgg->checkFile(filename))
	{
		if (!mEgg->loadFile(filename))
			return 0;
		else
			return -2;
	}

	eggPrintMessage("[EggPlugin module loader invoked]\n");

	for (i = mModules.begin(); i < mModules.end(); ++i)
	{
		name = mModules[i];

		sprintf(module_filename, "%slibfreyja_loader-%s.so", mPluginDir, name);
		sprintf(module_import, "freyja_model__%s_import", name);
		sprintf(module_check, "freyja_model__%s_check", name);

		if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
		{
			eggPrintError("\tERROR: In module '%s'.\n", module_filename);

			if ((error = dlerror()) != NULL)  
			{
				eggPrintError("\tERROR: %s\n", error);
				//return -5;
			}

			//return -1;
			continue;
		}
		else
		{
			eggPrintMessage("\tModule '%s' opened.\n", module_filename);
    
			check = (int (*)(char *filename))dlsym(handle, module_check);

			if ((error = dlerror()) != NULL)  
			{
				eggPrintError("\tERROR: %s\n", error);
				//return -5;
				dlclose(handle);
				continue;
			}

			import = (int (*)(char * filename))dlsym(handle, module_import);

			if ((error = dlerror()) != NULL)  
			{
				eggPrintError("\tERROR: %s\n", error);
				//return -5;
				dlclose(handle);
				continue;
			}

			loaded = (loaded || (!(*check)(filename) && !(*import)(filename)));

			if ((error = dlerror()) != NULL) 
			{
				//return -10;
				dlclose(handle);
				continue;
			}

			dlclose(handle);
		}
	}

	eggPrintMessage("[EggPlugin module loader sleeps now]\n");

	if (loaded)
		return 0; // sucess
#else
	if (!mEgg->Check(filename))
	{
		if (!mEgg->Load(filename))
			return 0;
		else
			return -2;
	}
#endif
	return -1;
}


int EggPlugin::exportModel(char *filename, char *type)
{
#ifdef MODEL_PLUGINS
	bool saved = false;
	char module_filename[128];
	char module_export[128];
	char *name;
	int (*export_mdl)(char *filename);
	void *handle;
	char *error;


	if (!type || !filename)
		return -100;

	if (strcmp(type, "egg") == 0)
		return mEgg->saveFile(filename);

	eggPrintMessage("[EggPlugin module loader invoked]\n");

	name = type;

	sprintf(module_filename, "%slibfreyja_loader-%s.so", mPluginDir, name);
	sprintf(module_export, "freyja_model__%s_export", name);

	if (!(handle = dlopen(module_filename, RTLD_NOW)))
	{
		eggPrintError("\tERROR: In module '%s'.\n", module_filename);

		if ((error = dlerror()) != NULL)  
		{
			eggPrintError("\tERROR: %s\n", error);
		}
	}
	else
	{
		eggPrintMessage("\tModule '%s' opened.\n", module_filename);
    
		export_mdl = (int (*)(char * filename))dlsym(handle, module_export);

		if ((error = dlerror()) != NULL)  
		{
			eggPrintError("\tERROR: %s\n", error);
			dlclose(handle);
		}

		saved = (!(*export_mdl)(filename));

		if ((error = dlerror()) != NULL) 
		{
			dlclose(handle);
		}

		dlclose(handle);
	}

	eggPrintMessage("[EggPlugin module loader sleeps now]\n");

	if (saved)
		return 0; // sucess
#else
	if (!type || !filename)
		return -100;

	if (strcmp(type, "egg") == 0)
		return mEgg->saveFile(filename);
#endif
	return -1;
}

unsigned int EggPlugin::eggBegin(egg_plugin_t type)
{
	switch (type)
	{
	case FREYJA_MESH:
		mStack.push(FREYJA_MESH);
		mMesh = mEgg->newMesh();
		mEgg->addMesh(mMesh);
		break;
	case FREYJA_GROUP:
		mStack.push(FREYJA_GROUP);

		if (!mMesh)
		{
			eggPrintError("EggPlugin::eggBegin> GROUP defined outside MESH.");
		}
		else
		{
			mGroup = mEgg->newGroup();
			mEgg->addGroup(mGroup);
			mMesh->group.pushBack(mGroup->id); 
		}
		break;
	case FREYJA_POLYGON:
		mStack.push(FREYJA_POLYGON);
		mVertexList.clear();
		mTexCoordList.clear();

		if (!mMesh)
		{
			eggPrintError("EggPlugin::eggEnd> Polygon defined outside MESH!");
		}
		break;
	case FREYJA_BONE:
		mStack.push(FREYJA_BONE);
		mTag = mEgg->addTag(0.0, 0.0, 0.0, 0x00);
		mTag->name[0] = 0;
		break;
	case FREYJA_SKELETON:
		mStack.push(FREYJA_SKELETON);
		break;
	case FREYJA_SKEL_ANIM:
		mStack.push(FREYJA_SKEL_ANIM);
		mAnimation = new egg_animation_t;
		mEgg->addAnimation(mAnimation);
	default:
		;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggEnd()
{
	unsigned int polygon;


	switch (mStack.pop())
	{
	case FREYJA_POLYGON:
		polygon = mEgg->addPolygon(mVertexList, mTexCoordList, mTextureId);

		if (polygon == UINT_MAX)
		{
			eggPrintMessage("EggPlugin::eggEnd> Polygon is invalid\n");
			return PLUGIN_ERROR;
		}

		if (mMesh)
		{
			mMesh->polygon.pushBack(polygon);
			mMesh->r_polygon.pushBack(mEgg->getPolygon(polygon));
		}
		else
		{
			eggPrintError("EggPlugin::eggEnd> Polygon defined outside MESH!");
		}
		break;
	default:
		;
	}

	return PLUGIN_ERROR;
}


int EggPlugin::eggTextureStore(EggTextureData *textureData)
{
	if (textureData)
	{
		mTextures.pushBack(textureData);
		return mTextures.size() - 1;
	}

	return -1;
}


unsigned int EggPlugin::eggTextureStoreFilename(const char *filename)
{
	unsigned int i;


	if (!filename || !filename[0])
	{
		return 0;
	}

	if (!mTextureFiles.empty())
	{
		for (i = mTextureFiles.begin(); i < mTextureFiles.end(); ++i)
		{
			if (!strcmp(mTextureFiles[i], filename))
				return i;
		}
	}

	mTextureFiles.pushBack(strdup(filename));
	return mTextureFiles.size() - 1;
}

int EggPlugin::eggGetTextureFilename(unsigned int index, char **filename)
{
	*filename = 0x0;


	if (mTextureFiles.empty() || index > mTextureFiles.size()-1)
		return -1;

	*filename = mTextureFiles[index];

	return 0;
}


int EggPlugin::eggGetTextureImage(unsigned int index, 
								  unsigned int *w, unsigned int *h, 
								  unsigned int *depth,
								  unsigned int *type,
								  unsigned char **image)
{
	EggTextureData *t;


	if (mTextures.empty() || index > mTextures.size()-1)
		return -1;

	t = mTextures[index];

	// Init
	*image = 0x0;
	*depth = 0x0;
	*type = 0x0;
	*w = 0x0;
	*h = 0x0;

	if (!t)
		return -1;

	if (t->mImage && t->mWidth && t->mHeight && t->mBitDepth)
	{
		*image = t->mImage;
		*depth = t->mBitDepth;
		*type = t->mType;
		*w = t->mWidth;
		*h = t->mHeight;

		return 0;
	}
  
	return -1;
}


unsigned int EggPlugin::eggTexCoordStore2f(float s, float t)
{
	return mEgg->addTexel(s, t);
}


unsigned int EggPlugin::eggVertexStore3f(float x, float y, float z)
{
	egg_vertex_t *vert;


	vert = mEgg->addVertex(x, y, z);

	if (vert)
	{
		if (mGroup)
		{
			mGroup->vertex.pushBack(vert->id);
		}
		else
		{
			eggPrintError("EggPlugin::eggVertexStore3f> Vertex outside GROUP!");
		}

		return vert->id;
	}
	else
		return PLUGIN_ERROR;
}


void EggPlugin::eggVertexWeightStore(unsigned int index, 
									 vec_t weight, unsigned int bone)
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
		eggPrintError("WARNING: Weight overflow not handled here %s:%d\n", 
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


void EggPlugin::eggVertexUVStore2f(unsigned int vIndex, 
								   float u, float v)
{
	egg_vertex_t *vert;

  
	vert = mEgg->getVertex(vIndex);

	if (!vert)
		return;

	vert->uv[0] = u;
	vert->uv[1] = v;
}


void EggPlugin::eggVertexNormalStore3f(unsigned int vIndex, 
									   float x, float y, float z)
{
	egg_vertex_t *vert;

  
  
	vert = mEgg->getVertex(vIndex);

	if (!vert)
		return;

	vert->norm[0] = x;
	vert->norm[1] = y;
	vert->norm[2] = z;
}


void EggPlugin::eggVertex1i(unsigned int egg_id)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mVertexList.pushBack(egg_id);
	}
	else
	{
		eggPrintError("EggPlugin::eggVertex1i> Vertex defined outside POLYGON!\n");
	}
}


void EggPlugin::eggMeshFlags1u(unsigned int flags)
{
	if (mStack.peek() == FREYJA_MESH || !mMesh)
	{
		mMesh->flags = flags;
	}
	else
	{
		eggPrintError("EggPlugin::eggMeshFlags1u> Flag defined outside MESH!\n");
	}
}


void EggPlugin::eggTexCoord1i(unsigned int egg_id)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTexCoordList.pushBack(egg_id);
	}
	else
	{
		eggPrintError("EggPlugin::eggTexCoord1i> Texel defined outside POLYGON!\n");
	}
}


void EggPlugin::eggMaterial1i(int id)
{
	if (mStack.peek() == FREYJA_POLYGON)
	{
		mTextureId = id;
	}
	else
	{
		eggPrintError("EggPlugin::eggTexture1i> Texture defined outside POLYGON!\n");
	}
}


unsigned int EggPlugin::eggFlags()
{
	return mFlags;
}


void EggPlugin::eggFlags(unsigned int set)
{
	mFlags = set;
}


unsigned int EggPlugin::eggGroupCenter(float x, float y, float z)
{
	if (mStack.peek() == FREYJA_GROUP)
	{
		if (!mGroup)
		{
			eggPrintError("EggPlugin::eggGroupCenter> GROUP isn't allocated!\n");
			return PLUGIN_ERROR;
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
		eggPrintError("EggPlugin::eggGroupCenter> Center defined outside GROUP!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagName(char *name)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			eggPrintError("EggPlugin::eggTagPos> BONEMTAG isn't allocated!\n");
			return PLUGIN_ERROR;
		}
		else 
		{
			strncpy(mTag->name, name, 64);
			mTag->name[63] = 0;
		}
	}
	else
	{
		eggPrintError("EggPlugin::eggTagPos> Pos defined outside BONEMTAG!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagPos(float x, float y, float z)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			eggPrintError("EggPlugin::eggTagPos> BONEMTAG isn't allocated!\n");
			return PLUGIN_ERROR;
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
		eggPrintError("EggPlugin::eggTagPos> Pos defined outside BONEMTAG!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagFlags(unsigned int flags)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			eggPrintError("EggPlugin::eggTagFlags> BONEMTAG isn't allocated!\n");
			return PLUGIN_ERROR;
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
		eggPrintError("EggPlugin::eggTagFlags> Flag defined outside BONEMTAG!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagAddMesh(unsigned int mesh)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			eggPrintError( 
					"EggPlugin::eggTagAddMesh> BONEMTAG isn't allocated!\n");
			return PLUGIN_ERROR;
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
		eggPrintError("EggPlugin::eggTagAddMesh> Mesh defined outside BONEMTAG!\n");
		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggTagAddSlave(unsigned int tag)
{
	if (mStack.peek() == FREYJA_BONE)
	{
		if (!mTag)
		{
			eggPrintError("EggPlugin::eggTagAddSlave> BONEMTAG isn't allocated!\n");
      
			return PLUGIN_ERROR;
		}
		else 
		{
			//egg_tag_t *child =  mEgg->getTag(tag);
			//
			//			if (child)
			//	child->parent = eggGetCurrent(FREYJA_BONE);

			mTag->slave.pushBack(tag);
		}
	}
	else
	{
		eggPrintError("EggPlugin::eggTagAddSlave> Slave defined outside BONEMTAG!\n");

		return PLUGIN_ERROR;
	}

	return 0;
}


unsigned int EggPlugin::eggMeshTreeAddFrame(float x, float y, float z)
{
	if (mAnimation)
	{
		mBoneFrame = mEgg->BoneFrame(mEgg->BoneFrameAdd(x, y, z));    
		mAnimation->frame.pushBack(mBoneFrame->id);
   
		return 0;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggTagRotate(float x, float y, float z)
{
	if (mTag)
	{
		mTag->rot[0] = x;
		mTag->rot[1] = y;
		mTag->rot[2] = z;
   
		return 0;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggMeshTreeTag(unsigned int tag)
{
	if (mBoneFrame)
	{
		mBoneFrame->tag.pushBack(tag);
		return 0;
	}

	return PLUGIN_ERROR;
}


unsigned int EggPlugin::eggTextureStoreBuffer(unsigned char *image, 
											  unsigned int depth,
											  unsigned int width, 
											  unsigned int height,
											  egg_colormode_t type)
{
	EggTextureData *t;
	unsigned int size;


	if (!image || !depth || !width || !height)
		return PLUGIN_ERROR;

	size = width*height*depth;

	t = new EggTextureData();
 
	t->mWidth = width;
	t->mHeight = height;
	t->mType = type;
	t->mBitDepth = depth * 8;
	t->mImage = new unsigned char[size];
  
	memcpy(t->mImage, image, size);

	return eggTextureStore(t);
}


void EggPlugin::addModule(char *name)
{
	if (name && name[0])
		mModules.pushBack(name);
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
