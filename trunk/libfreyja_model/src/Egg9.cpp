/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : mongoose@users.sourceforge.net
 * Object  : Egg
 * License : GPL, also (C) 2000 Mongoose
 * Comments: This is the data model agent
 *
 * 
 *-- History -----------------------------------------------
 *
 * 2000-12-09:
 * Mongoose - Move to new data structures
 *
 * 2000-07-01:
 * Mongoose - Rebuilt backend
 *
 * 2000-05-09:
 * Mongoose - Almost complete rewrite
 *
 * 1999-07-31:
 * Mongoose - Created ( After prototyping in C for a week )
 ==========================================================================*/

#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "mendian.h"

#include "Egg.h"


void __print_unsigned_int(unsigned int u)
{
	printf("<%u>\n", u);
}

void __print_egg_vertex_t(egg_vertex_t *v)
{
	if (!v)
		return;

	printf("<%i, %d>\n", v->id, v->pos);
}

void __print_egg_texcoord_t(egg_texcoord_t *t)
{
	if (!t)
		return;

	printf("<%i, %d>\n", t->id, t->uvw);
}

void __print_egg_polygon_t(egg_polygon_t *p)
{
	if (!p)
		return;
	
	printf("<%i, \n", p->id);
	printf("List <vertex_id> ");
	p->vertex.print(__print_unsigned_int);
	printf("List <texcoord_id> ");
	p->texcoord.print(__print_unsigned_int);
	printf(", %i>\n", p->material);
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Egg::Egg()
{
	mDebugLevel = 0;

	mVertices.reserve(256);
	mTexCoords.reserve(256);

	mVertexVector.reserve(256);
	mVertexArray = mVertexVector.getVectorArray();
	mTexCoordVector.reserve(256);
	mTexCoordArray = mTexCoordVector.getVectorArray();
	mNormalVector.reserve(256);
	mNormalArray = mNormalVector.getVectorArray();
}


Egg::~Egg()
{
	erase();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

int Egg::checkFile(const char *filename)
{
	u_int32_t id, version;
	FILE *f;


	f = fopen(filename, "rb");

	if (!f)
	{
		printError("Couldn't read file");
		perror(filename);
		return -1;
	}

	eRead_Small_uint32(&id, f);
	eRead_Small_uint32(&version, f);

	/* NOTE: I sometimes have versions that only differ by version id
	 *       that are used for internal testing ( not public testing )
	 *       This let's me test stuff w/o screwing users everytime I
	 *       want to do a version rollover and be backwards compatible */
	if (id == EGG_FILE)
	{
		switch (version)
		{
		case EGG_8_12:
		case EGG_VERSION:
			return 0;
		}
	}
	
	return -2;
}


unsigned int Egg::getCount(egg_type_t type)
{
	unsigned int count = 0;


	switch (type)
	{
	case VERTEX:
		count = mVertices.size();
		break;
	case POLYGON:
		count = mPolygons.size();
		break;
	case TEXCOORD:
		count = mTexCoords.size();
		break;
	case GROUP:
		count = mGroups.size();
		break;
	case MESH:
		count = mMeshes.size();
		break;
	case BONE:
		count = mTags.size();
		break;
	case SKELETON:
		count = mBoneFrames.size();
		break;
	case ANIMATION:
		count = mAnimations.size();
		break;
	case METADATA:
		count = 0;
		printError("getCount> Not implementation for %c%c%c%c, %s:%d\n", 
					  ((char *)type)[0], ((char *)type)[1], ((char *)type)[2], ((char *)type)[3],
					  __FILE__, __LINE__);
		break;
	}

	return count;
}


unsigned int Egg::getNearest(egg_type_t type, vec3_t xyz)
{
	unsigned int best = 0;
	vec_t dist = 0.0;
	vec_t closest = 999999.0;


	switch (type)
	{
	case VERTEX:
		if (mVertices.empty())
			return 0;
     
		for (mVertices.start(); mVertices.forward(); mVertices.next())
		{
			egg_vertex_t *current = mVertices.current();
			
			if (!current)
				continue;
			
			dist = helDist3v(xyz, mVertexArray[current->pos]);
			
			if (dist < closest)
			{
				best = current->id;
				closest = dist;
			}
		}

		break;
	case TEXCOORD:
		if (mTexCoords.empty())
			return 0;
     
		for (mTexCoords.start(); mTexCoords.forward(); mTexCoords.next())
		{
			egg_texcoord_t *current = mTexCoords.current();
			Vector3d v;
			
			if (!current)
				continue;

			dist = helDist3v(xyz, mTexCoordArray[current->uvw]);
			
			if (dist < closest)
			{
				best = current->id;
				closest = dist;
			}
		}
		break;
	case BONE:
		if (mTags.empty())
			return 0;
     
		for (mTags.start(); mTags.forward(); mTags.next())
		{
			egg_bone_t *current = mTags.current();
			
			if (!current)
				continue;
			
			dist = helDist3v(xyz, current->position.mVec);
			
			if (dist < closest)
			{
				best = current->id;
				closest = dist;
			}
		}
		break;
	case SKELETON:
		printDebug(1, "getNearest> Undefined for BONEFRAME");
		break;
	case ANIMATION:
		printDebug(1, "getNearest> Undefined for ANIMATION");
		break;
	default:
		printError("getNearest> Not Implemented for %s, %s:%i", 
					  ((type == POLYGON) ? "POLYGON" :
						((type == MESH) ? "MESH" :
						 ((type == GROUP) ? "GROUP" : 
						  "UNKNOWN"))),
					  __FILE__, __LINE__);
	}

	return best;
}


int Egg::saveFile(const char *filename)
{
	FILE *f;
	u_int32_t u, version, id;
	egg_vertex_t *vertex;
	egg_texcoord_t *texcoord;
	egg_polygon_t *polygon;
	egg_group_t *group;


	f = fopen(filename, "wb");

	if (!f)
	{
		printError("saveFile> Couldn't save file.");
		perror(filename);
		return -1;
	}

	// Header ///////////////////////////////
	id = EGG_FILE;
	version = EGG_VERSION;
	eWrite_Small_uint32(&id, f); 
	eWrite_Small_uint32(&version, f); 

	u = getCount(VERTEX);
	eWrite_Small_uint32(&u, f);   
	u = getCount(TEXCOORD);
	eWrite_Small_uint32(&u, f);   
	u = getCount(POLYGON);
	eWrite_Small_uint32(&u, f);  
	u = getCount(METADATA);
	eWrite_Small_uint32(&u, f);
	u = getCount(GROUP);
	eWrite_Small_uint32(&u, f);
	u = getCount(MESH);
	eWrite_Small_uint32(&u, f);
	u = getCount(BONE);
	eWrite_Small_uint32(&u, f);    
	u = getCount(SKELETON);
	eWrite_Small_uint32(&u, f);
	u = getCount(ANIMATION);
	eWrite_Small_uint32(&u, f);
  
	////////////////////////////////////////

	for (mVertices.start(); mVertices.forward(); mVertices.next())
	{
		vertex = mVertices.current();
    
		if (!vertex)
			continue;
    
		u = vertex->id;
		eWrite_Small_uint32(&u, f);
		eWrite_Small_float(&mVertexArray[vertex->pos][1], f);
		eWrite_Small_float(&mVertexArray[vertex->pos][2], f);
		eWrite_Small_float(&mTexCoordArray[vertex->uvw][0], f);
		eWrite_Small_float(&mTexCoordArray[vertex->uvw][1], f);
		eWrite_Small_float(&mTexCoordArray[vertex->uvw][2], f);
		eWrite_Small_float(&mNormalArray[vertex->normal][0], f);
		eWrite_Small_float(&mNormalArray[vertex->normal][1], f);
		eWrite_Small_float(&mNormalArray[vertex->normal][2], f);

		// FIXME: Add weights
	}

	for (mTexCoords.start(); mTexCoords.forward(); mTexCoords.next())
	{
		texcoord = mTexCoords.current();
    
		if (!texcoord)
			continue;
    
		u = texcoord->id;
		eWrite_Small_uint32(&u, f);
		eWrite_Small_float(&mTexCoordArray[texcoord->uvw][0], f);
		eWrite_Small_float(&mTexCoordArray[texcoord->uvw][1], f);
		eWrite_Small_float(&mTexCoordArray[texcoord->uvw][2], f);
	} 

	// PolyMesh //////////////////////

	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		polygon = mPolygons.current();

		if (!polygon)
			continue;

		u = polygon->id;
		eWrite_Small_uint32(&u, f);
		eWrite_Small_int32(&polygon->material, f);

		u = polygon->vertex.size();
		eWrite_Small_uint32(&u, f);

		for (polygon->vertex.start(); polygon->vertex.forward(); polygon->vertex.next())
		{
			u = polygon->vertex.current();
			eWrite_Small_uint32(&u, f);
		}

		u = polygon->texcoord.size();
		eWrite_Small_uint32(&u, f);

		for (polygon->texcoord.start(); polygon->texcoord.forward(); polygon->texcoord.next())
		{
			u = polygon->texcoord.current();
			eWrite_Small_uint32(&u, f);
		}
	}

	////////////////////////////////////////

	for (mGroups.start(); mGroups.forward(); mGroups.next())
	{
		group = mGroups.current();
    
		if (!group)
			continue;
    
		u = group->id;
		eWrite_Small_uint32(&u, f);
    
		u = group->vertex.size();
		eWrite_Small_uint32(&u, f);
    
		for (group->vertex.start(); group->vertex.forward(); group->vertex.next())
		{
			u = group->vertex.current();
			eWrite_Small_uint32(&u, f);
		}
	}

   for (mMeshes.start(); mMeshes.forward(); mMeshes.next())
   {
		saveMesh(mMeshes.current(), f);
   }

   for (mTags.start(); mTags.forward(); mTags.next())
   {
		saveTag(mTags.current(), f);
   }

   for (mBoneFrames.start(); mBoneFrames.forward(); mBoneFrames.next())
   {
		saveBoneFrame(mBoneFrames.current(), f);
   }

   for (mAnimations.start(); mAnimations.forward(); mAnimations.next())
   {
		saveAnimation(mAnimations.current(), f);
   }

   fclose(f);

   return 0;
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Egg::setDebugLevel(unsigned int n)
{
	mDebugLevel = n;
}


void Egg::erase()
{
	for (mVertices.start(); mVertices.forward(); mVertices.next())
	{
		if (mVertices.current())
			(mVertices.current())->weights.erase();
	}

	mVertices.erase();
	mTexCoords.erase();
	mPolygons.erase();
	mGroups.erase();
	mMeshes.erase();
	mTags.erase();
	mBoneFrames.erase();
	mAnimations.erase();
}


void Egg::transform(egg_type_t type, egg_transform_t transform, vec3_t xyz)
{
	
}


bool Egg::combineVertices(unsigned int A, unsigned int B)
{		
	egg_vertex_t *a = getVertex(A);
	egg_vertex_t *b = getVertex(B);
	egg_polygon_t *polygon;
	egg_group_t *grp;
	unsigned int i, j;


	if (A == B || !a || !b)
		return false;
			
	for (i = b->ref.begin(); i < b->ref.end(); ++i)
	{
		polygon = getPolygon(b->ref[i]);
		
		if (polygon)
		{
			/* Replace B with A to match sorted array ids */
			polygon->vertex.Replace(B, A);
			a->ref.pushBack(b->ref[i]);
		}
	}
			
	/* Don't let VertexDel() see we refence any polygons */
	b->ref.clear();
			
	// Mongoose 2002.01.19, handle groups here
	// Remove all references to dead vertex from groups
	for (i = mGroups.begin(); i < mGroups.end(); ++i)
	{
		grp = mGroups[i];
		
		for (j = grp->vertex.begin(); 
			  grp->vertex.forward(); grp->vertex.next())
		{
			if (grp->vertex.current() == B)
			{
				grp->vertex.remove(grp->vertex.currentIndex());
			}
		}
	}
	
	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		if (isVertexInPolygon(B, mPolygons.current()))
		{
			(mPolygons.current())->vertex.Replace(B, A);
		}
	}
	
	mVertices.remove(B);
	delete b;

	return true;
}


void Egg::combineTexcoords(unsigned int A, unsigned int B)
{
		if (A != B)
		{
			egg_texcoord_t *a = getTexCoord(A);
			egg_texcoord_t *b = getTexCoord(B);
			egg_polygon_t *polygon;
			
			
			if (!a || !b)
				return;
			
			for (b->ref.start(); b->ref.forward(); b->ref.next())
			{
				polygon = getPolygon(b->ref.current());
				
				if (polygon)
				{
					// Must be replace B with A to match sorted list ids
					polygon->texcoord.Replace(B, A);
					
					a->ref.pushBack(b->ref.current());
				}
			}
			
			// Mongoose: Don't let delTexCoord see we refence any polygons
			b->ref.clear();
			
			remove(TEXCOORD, B);

}


void Egg::remove(egg_type_t type, unsigned int index)
{
	switch (type)
	{
	case VERTEX:
		delVertex(getVertex(index));
		break;
	case POLYGON:
		delPolygon(getPolygon(index));
		break;
	case TEXCOORD:
		delTexCoord(getTexCoord(index));
		break;
	case GROUP:
		delGroup(getGroup(index));
		break;
	case MESH:
		delMesh(getMesh(index));
		break;
	case BONE:
		delTag(getTag(index));
		break;
	case SKELETON:
		//delBoneFrame(getBoneFrame(index));
		//break;
	case ANIMATION:
		//delAnimation(getAnimation(index));
		//break;
	case METADATA:
		//delMetaData(getMetadata(index));
		//break;
	default:
		printDebug(0, "remove> Not Implemented for %d type %s:%d\n",
					  type, __FILE__, __LINE__);
		
	}
}


int Egg::loadFile(char *filename)
{
	Map<unsigned int, unsigned int> transV;
	Map<unsigned int, unsigned int> transT;
	Map<unsigned int, unsigned int> transP;
	Map<unsigned int, unsigned int> transG;
	Map<unsigned int, unsigned int> transVwST;
	Vector<unsigned int> vertexlist;
	Vector<unsigned int> texcoordlist;
	FILE *f;
	egg_group_t *group;
	egg_vertex_t *vertex;
	egg_mesh_t *mesh;
	egg_bone_t *tag;
	egg_animation_t *animation;
	egg_skeleton_t *boneframe;
	u_int32_t u, version, id, i, j, n;
	u_int32_t num_vertex, num_texcoord, num_poly, num_marker;
	u_int32_t num_grp, numMeshes, numTags, num_frame, num_aframe;
	float x, y, z, tu, tv, tw, nx, ny, nz;
	int material;
	unsigned int polygonId;


	f = fopen(filename, "rb");

	if (!f)
	{
		perror("Egg::Load> ");
		return -1;
	}

	// Header ////////////////////////////
	fread(&id, 4, 1, f);

	if (id != EGG_FILE)
	{
		printError("Load> invalid header 0x%x", id);
		return -2;
	}

	fread(&version, 4, 1, f);

	if (version != EGG_VERSION && 
		 version != EGG_8_12)
	{
		printError("Load> invalid or unsupported version 0x%x", version);
		return -3;
	}

	fread(&num_vertex, 4, 1, f);   
	fread(&num_texcoord, 4, 1, f);   
	fread(&num_poly, 4, 1, f);   
	fread(&num_marker, 4, 1, f);   

	fread(&num_grp, 4, 1, f);   
	fread(&numMeshes, 4, 1, f);   
	fread(&numTags, 4, 1, f);   
	fread(&num_frame, 4, 1, f);   
	fread(&num_aframe, 4, 1, f);

	////////////////////////////////////////

	for (i = 0; i < num_vertex; ++i)
	{
		fread(&id, 4, 1, f);
		fread(&x, sizeof(vec_t), 1, f);
		fread(&y, sizeof(vec_t), 1, f);
		fread(&z, sizeof(vec_t), 1, f);

		tu = tv = nx = ny = nz = 0.0;
		tw = 1.0;

		fread(&tu, sizeof(vec_t), 1, f);
		fread(&tv, sizeof(vec_t), 1, f);

		switch (version)
		{
		case EGG_VERSION:
			fread(&tw, sizeof(vec_t), 1, f);
			break;
		default:
			;
		}

		fread(&nx, sizeof(vec_t), 1, f);
		fread(&ny, sizeof(vec_t), 1, f);
		fread(&nz, sizeof(vec_t), 1, f);

		vertex = addVertex(x, y, z, nx, ny, nz, tu, tv, tw);
		transV.Add(id, vertex->id);
	}
  
	for (i = 0; i < num_texcoord; ++i)
	{
		fread(&id, 4, 1, f); 
		fread(&tu, sizeof(float), 1, f);
		fread(&tv, sizeof(float), 1, f);

		switch (version)
		{
		case EGG_8_12:
			tw = 1.0f;
			break;
		default:
			fread(&tw, sizeof(float), 1, f);
		}

		transT.Add(id, addTexCoord(tu, tv, tw));   
	}

	for (i = 0; i < num_poly; ++i)
	{
		fread(&id, 4, 1, f);
		fread(&material, 4, 1, f);

		// Vertex indeces, translated to new ids
		fread(&n, 4, 1, f);

		for (j = 0; j < n; ++j)
		{
			fread(&u, 4, 1, f);
			vertexlist.pushBack(transV[u]);
		}

		// TexCoord indices, translated to new ids
		fread(&n, 4, 1, f);

		for (j = 0; j < n; ++j)
		{
			fread(&u, 4, 1, f);
			texcoordlist.pushBack(transT[u]);
		}

		if (isDebugLevel(5))
		{
			printDebug("*** %i %i %i %i\n", 
						  i, id, vertexlist.size(), texcoordlist.size());
		}

		polygonId = addPolygon(vertexlist, texcoordlist, material);
		transP.Add(id, polygonId);
		vertexlist.clear();
		texcoordlist.clear();
	}

	// Mongoose 2002.07.05, Marker system removed
	for (i = 0; i < num_marker; ++i)
	{
		u_int32_t id, type, size;
		unsigned char *metadata = 0x0;


		fread(&id, 4, 1, f);
		fread(&type, 4, 1, f);
		fread(&size, 4, 1, f);

		if (size > 0)
		{
			metadata = new unsigned char[size];
			fread(metadata, size, 1, f);
		}

		addMetaData(id, type, size, metadata);
	} 

	////////////////////////////////////////


	for (i = 0; i < num_grp; ++i)
	{
		fread(&id, 4, 1, f);

		group = newGroup();
		addGroup(group);
		transG.Add(id, group->id);

		// vertices
		fread(&n, 4, 1, f);

		group->vertex.reserve(n);

		for (j = 0; j < n; ++j)
		{
			fread(&u, 4, 1, f);
			group->vertex.pushBack(transV[u]);
		}

		vertex = getVertex(group->vertex[0]);
    
		if (vertex)
		{
			group->center = mVertexArray[vertex->pos];
		}
		else
		{
			group->center.zero();
		}
	}

#ifdef DEBUG_HEAVY
	printf("vertex id");
	transV.Print(__print_unsigned_int);
	printf("texcoord id");
	transT.Print(__print_unsigned_int);
	printf("polygon id");
	transP.Print(__print_unsigned_int);
	printf("vertex group id");
	transG.Print(__print_unsigned_int);
#endif

	for (i = 0; i < numMeshes; ++i)
	{
		mesh = loadMesh(f);

		if (!mesh)
		{
			printf("EGG::Load> Error loading mesh[%i] resetting model\n", i);

			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			erase();
			fclose(f);

			return -10;
		}
	  
		addMesh(mesh);
	}

	mTags.reserve(numTags+8);

	for (i = 0; i < numTags; ++i)
	{
		tag = loadTag(f);

		if (!tag)
		{
			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			fclose(f);
			erase();
			return -20;
		}

		printf("-- Loading Tag #%i of %i\n", i, numTags);

		addTag(tag);
	}

	for (i = 0; i < num_frame; ++i)
	{
		boneframe = loadBoneFrame(f);

		if (!boneframe)
		{
			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			fclose(f);
			erase();
			return -30;
		}

		addBoneFrame(boneframe);
	}

	for (i = 0; i < num_aframe; ++i)
	{
		animation = loadAnimation(f);

		if (!animation)
		{
			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			fclose(f);
			erase();
			return -40;
		}

		addAnimation(animation);
	}

	fclose(f);
	return 0;
}


/// TexCoords //////////////////////////////////////////////////

egg_texcoord_t *Egg::getTexCoord(unsigned int id)
{
	if (mTexCoords.empty())
	{
		return 0x0;
	}

	return mTexCoords[id];
}


unsigned int Egg::addTexCoord(vec_t u, vec_t v, vec_t w)
{
	Vector3d vec;
	egg_texcoord_t *texcoord;


	vec = Vector3d(u, v, w);
	mVertexVector.pushBack(vec.mVec);

	texcoord = new egg_texcoord_t;
	texcoord->uvw = mVertexVector.size() - 1;

	mTexCoords.pushBack(texcoord);
	texcoord->id = mTexCoords.size() - 1;

	return texcoord->id;
}


void Egg::delTexCoord(egg_texcoord_t *t)
{
	if (t)
	{
		mTexCoords.remove(t->id);
		delete t;
	}
}




/// Vertices ////////////////////////////////////////////////

egg_vertex_t *Egg::getVertex(unsigned int id)
{
	if (mVertices.empty())
	{
		return 0x0;
	}

	return mVertices[id];
}





egg_vertex_t *Egg::addVertex(vec_t x, vec_t y, vec_t z)
{
	return addVertex(x, y, z, 0.0, 1.0, 0.0, 0.5, 0.5, 1.0);
}


egg_vertex_t *Egg::addVertex(vec_t x, vec_t y, vec_t z, 
									  vec_t nx, vec_t ny, vec_t nz,
									  vec_t u, vec_t v, vec_t w)

{
	egg_vertex_t *vertex;


	vertex = new egg_vertex_t;
	vertex->pos    = addPosition(x, y, z);
	vertex->normal = addNormal(nx, ny, nz);
	vertex->uvw    = addUVW(u, v, w);

	mVertices.pushBack(vertex);
	vertex->id = mVertices.size() - 1;

	return vertex;
}




void Egg::delVertex(egg_vertex_t *v)
{
	egg_group_t *grp;


	if (!v)
		return;

	// FIMXE: I don't think you can do this with the list
	//        I think it'll only get the first match and stop
	//        See hack in loop

	// Remove all polygons using this vertex
	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		if (isVertexInPolygon(v->id, mPolygons.current()))
		{
			delPolygon(mPolygons.current());
			mPolygons.start(); // FIXME: hack to ensure clean all from list
		}
	}

	// Remove all references to vertex from groups
	for (mGroups.start(); mGroups.forward(); mGroups.next())
	{
		grp = mGroups.current();

		for (grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
		{
			if (grp->vertex.current() == v->id)
			{
				grp->vertex.remove(grp->vertex.currentIndex());
			}
		}
	}

	mVertices.remove(v->id);

	delete v;
}

egg_polygon_t *Egg::getPolygon(unsigned int id)
{
	if (mPolygons.empty())
		return NULL;
  
	return mPolygons[id];
}

/* FIXME: Really need 2 polygon types (classes?) 
 * One with and one without external texcoords */
unsigned int Egg::addPolygon(Vector<unsigned int> &vertex,
									  Vector<unsigned int> &texcoord, 
									  int material)
{
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texcoord_t *tex;


	/* It's okay for texcoord list to be empty, it means there are no
	 * polymapped texcoords for this polygon */

	assert(vertex.empty());

	if (vertex.empty())
	{
		printError("PolygonAdd> Assertion failed, No vertices\n");
		return UINT_MAX;
	}

	polygon = new egg_polygon_t;
	mPolygons.pushBack(polygon);
	polygon->id = mPolygons.size() - 1;
	polygon->material = material;

	for (texcoord.start(); texcoord.forward(); texcoord.next())
	{
		tex = getTexCoord(texcoord.current());

		if (!tex)
		{
			printf("Invalid texcoord %u\n", texcoord.current());
			delete polygon;
			return UINT_MAX;
		}

		tex->ref.pushBack(polygon->id);
		polygon->texcoord.pushBack(tex->id);
	}

	for (vertex.start(); vertex.forward(); vertex.next())
	{
		vert = getVertex(vertex.current());

		if (!vert)
		{
			printf("Invalid vertex %u\n", vertex.current());
			delete polygon;
			return UINT_MAX;
		}

		vert->ref.pushBack(polygon->id);
		polygon->vertex.pushBack(vert->id);
	}

	return polygon->id;
}


void Egg::delPolygon(egg_polygon_t *polygon)
{
	egg_mesh_t *mesh;
	egg_texcoord_t *texcoord;


	if (!polygon)
		return;

	// Remove all references to polygon from meshes
	for (mMeshes.start(); mMeshes.forward(); mMeshes.next())
	{
		mesh = mMeshes.current();
		mesh->polygons.remove(polygon->id);
	}

	// Remove texcoords used by polygon
	for (polygon->texcoord.start(); 
		  polygon->texcoord.forward(); polygon->texcoord.next())
	{
		texcoord = getTexCoord(polygon->texcoord.current());

		if (!texcoord)
			continue;
    
		polygon->texcoord.remove(texcoord->id);
		texcoord->ref.remove(polygon->id);

		// Mongoose: account for shared texcoords!
		if (texcoord->ref.empty())
			delTexCoord(texcoord);
	}

	mPolygons.remove(polygon->id);  
	delete polygon;
}


void Egg::delPolygon(Vector<unsigned int> &list)
{
	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		if (isMatchForPolygon(list, mPolygons.current()))
		{
			delPolygon(mPolygons.current());
			return;
		}
	}
}


unsigned int Egg::selectPolygon(Vector<unsigned int> &list)
{
	egg_polygon_t *polygon;


	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		if (isMatchForPolygon(list, mPolygons.current()))
		{
			polygon = mPolygons.current();
			return polygon->id;
		}
	}

	return UINT_MAX;
}

/// Groups ////////////////////////////////////////////////

egg_group_t *Egg::getGroup(unsigned int id)
{
	if (mGroups.empty())
		return NULL;

	return mGroups[id];
}

void Egg::addGroup(egg_group_t *group)
{
	if (!group)
		return;

	mGroups.pushBack(group);
	group->id = mGroups.size() - 1;
}


egg_group_t *Egg::newGroup()
{
	egg_group_t *group;


	group = new egg_group_t;
	group->id = UINT_MAX;
	group->center.zero();

	return group;
}



void Egg::delGroup(egg_group_t *group)
{
	if (!group)
		return;

	// FIXME: Maybe it would be nice to check and see if it's used
	//        by another group ( but groups may be obsoleted soon )
	for (group->vertex.start(); group->vertex.forward(); group->vertex.next())
	{
		remove(VERTEX, group->vertex.current());
	}

   mGroups.remove(group->id);
   delete group;
}




/// Meshes ////////////////////////////////////////////////

egg_mesh_t *Egg::newMesh()
{
	egg_mesh_t *mesh;


	mesh = new egg_mesh_t;
	return mesh;
}


void Egg::delMesh(egg_mesh_t *mesh)
{
	if (!mesh)
		return;

	// Unlink groups/frames ( Assumes sole ownder )
	for (mesh->group.start(); mesh->group.forward(); mesh->group.next())
	{
		delGroup(mesh->group.current());
	}

	for (mesh->polygon.start(); mesh->polygon.forward(); mesh->polygon.next())
	{
		delPolygon(mesh->polygon.current());
	}

	mMeshes.remove(mesh->id);  // unlink from list
	////////////////////////////////////////////////////////////
	// Private
	////////////////////////////////////////////////////////////

	delete mesh; // delete mesh
}


egg_mesh_t *Egg::getMesh(unsigned int id)
{
	if (mMeshes.empty())
		return NULL;

	return mMeshes[id];
}


void Egg::addMesh(egg_mesh_t *mesh)
{
	if (!mesh)
		return;

	mMeshes.pushBack(mesh);
	mesh->id = mMeshes.size() - 1;
}


int Egg::saveMesh(egg_mesh_t *mesh, FILE *f)
{
	u_int32_t u;


	if (!mesh)
		return -1;
  
	// Check point
	u = EGG_MESH_CHUNK_START;
	eWrite_Small_uint32(&u, f);

	// Vertex Group indices
	u = mesh->group.size();
	eWrite_Small_uint32(&u, f);

	for (mesh->group.start(); mesh->group.forward(); mesh->group.next())
	{
		u = mesh->group.current();
		eWrite_Small_uint32(&u, f);
	}

	// Check point
	u = 0xbdbdbdbd;
	eWrite_Small_uint32(&u, f);

	// Polygon indices
	u = mesh->polygon.size();
	eWrite_Small_uint32(&u, f);

	for (mesh->polygon.start(); mesh->polygon.forward(); mesh->polygon.next())
	{
		u = mesh->polygon.current();
		eWrite_Small_uint32(&u, f);
	}

	// Check point
	u = EGG_MESH_CHUNK_END;
	eWrite_Small_uint32(&u, f);

	return 0;
}


egg_mesh_t *Egg::loadMesh(FILE *f)
{
	egg_mesh_t *mesh;
	unsigned int u, i, n;


	mesh = newMesh();
	//MeshAdd(mesh);

	// Check point
	fread(&u, 4, 1, f);

	if (u != EGG_MESH_CHUNK_START)
	{
		printError("MeshLoad> start checkpoint [ERROR]\n");
	}

	// Vertex Group indices
	fread(&n, 4, 1, f);

	for (i = 0; i < n; ++i)
	{
		fread(&u, 4, 1, f);
		mesh->group.pushBack(u);
	}

	// Check point
	fread(&u, 4, 1, f);

	if (u != 0xbdbdbdbd)
	{
		printError("MeshLoad> internal checkpoint [ERROR]\n");
	}

	// Polygon indices
	fread(&n, 4, 1, f);

#ifdef DEBUG_EGG_LOAD
	printDebug(5, "MeshLoad> Loading %i polygons\n", n);
#endif

	for (i = 0; i < n; ++i)
	{
		fread(&u, 4, 1, f);
		mesh->polygon.pushBack(u);

#ifdef DEBUG_EGG_LOAD
		printDebug(5, "MeshLoad> Adding polygon[%i] = %i\n", i, u);
#endif
	}

	// Check point
	fread(&u, 4, 1, f);
  
	if (u != EGG_MESH_CHUNK_END)
	{
		printError("MeshLoad> end checkpoint [ERROR]\n");
	}

	return mesh;
}

/// Tags //////////////////////////////////////////////////

egg_bone_t *Egg::getTag(unsigned int id)
{
	if (mTags.empty())
		return NULL;

	return mTags[id];
}

//FIXME: Only size is handled, like the rest of this file I/O code
//       it needs ordering code in parts, and etc - but I have no
//       big endian machines at home
int Egg::saveTag(egg_bone_t *tag, FILE *f)
{
	int32_t li;
	u_int32_t lu;  


	if (!tag)
		return -1;

	// Check point
	lu = EGG_BTAG_CHUNK_START;
	eWrite_Small_uint32(&lu, f);

	li = tag->id;
	eWrite_Small_int32(&li, f);

	lu = tag->slave.size();
	eWrite_Small_uint32(&lu, f);

	for (tag->slave.start(); tag->slave.forward(); tag->slave.next())
	{
		lu = tag->slave.current();
		eWrite_Small_uint32(&lu, f);
	}

	lu = tag->mesh.size();
	eWrite_Small_uint32(&lu, f);

	for (tag->mesh.start(); tag->mesh.forward(); tag->mesh.next())
	{
		lu = tag->mesh.current();
		eWrite_Small_uint32(&lu, f);
	}

	fwrite(&tag->flag, 1, 1, f);

	eWrite_Small_float(&tag->position.mVec[0], f);
	eWrite_Small_float(&tag->position.mVec[1], f);
	eWrite_Small_float(&tag->position.mVec[2], f);

	eWrite_Small_float(&tag->rotation.mVec[0], f);
	eWrite_Small_float(&tag->rotation.mVec[1], f);
	eWrite_Small_float(&tag->rotation.mVec[2], f);

	// tag's 'transform matrix' isn't stored to disk

	// Check point
	lu = EGG_BTAG_CHUNK_END;
	eWrite_Small_uint32(&lu, f);

	return 0;
}


egg_bone_t *Egg::loadTag(FILE *f)
{
	egg_bone_t *tag;
	int32_t li;
	u_int32_t lu;  
	int i, n;


	tag = new egg_bone_t;

	// Check point
	fread(&lu, 4, 1, f);

	if (lu != EGG_BTAG_CHUNK_START)
	{
		printError("TagLoad> start checkpoint [ERROR]\n");
	}

	fread(&li, 4, 1, f);
	tag->id = li;

	fread(&lu, 4, 1, f);
	n = lu;

	for (i = 0; i < n; i++)
	{
		fread(&lu, 4, 1, f);
		tag->slave.pushBack(lu);
	}

	fread(&lu, 4, 1, f);
	n = lu;

	for (i = 0; i < n; i++)
	{
		fread(&lu, 4, 1, f);
		tag->mesh.pushBack(lu);
	}

	fread(&tag->flag, 1, 1, f);

	fread(&tag->position.mVec[0], sizeof(vec_t), 1, f);
	fread(&tag->position.mVec[1], sizeof(vec_t), 1, f);
	fread(&tag->position.mVec[2], sizeof(vec_t), 1, f);

	fread(&tag->rotation.mVec[0], sizeof(vec_t), 1, f);
	fread(&tag->rotation.mVec[1], sizeof(vec_t), 1, f);
	fread(&tag->rotation.mVec[2], sizeof(vec_t), 1, f);

	// tag's 'transform matrix' isn't stored on disk

	// Check point
	fread(&lu, 4, 1, f);  

	if (lu != EGG_BTAG_CHUNK_END)
	{
		printError("TagLoad> end checkpoint [ERROR]\n");
	}

	return tag; 
}


void Egg::addTag(egg_bone_t *tag)
{
	if (tag)
	{
		mTags.pushBack(tag);
		tag->id = mTags.size() - 1;
	}
}


egg_bone_t *Egg::addTag(vec_t x, vec_t y, vec_t z, char flag)
{
	egg_bone_t *tag;


	tag = new egg_bone_t;
	tag->flag = flag; 
	tag->parent = 0;
	tag->position.mVec[0] = x;
	tag->position.mVec[1] = y;
	tag->position.mVec[2] = z;
	tag->rotation.mVec[0] = 0;
	tag->rotation.mVec[1] = 0;
	tag->rotation.mVec[2] = 0;
	tag->matrix.setIdentity();

	addTag(tag);

	return tag;
}




void Egg::delTag(egg_bone_t *tag)
{
	if (!tag)
		return;

	mTags.remove(tag->id);

	delete tag;
}


void Egg::rotateTagAboutOrigin(unsigned int tag, vec_t rx, vec_t ry, vec_t rz)
{
   Matrix transform;
   Vector3d rotation;
   egg_bone_t *etag;
   egg_mesh_t *mesh;
   egg_group_t *grp;
   egg_vertex_t *vert;
   unsigned int count;


   etag = getTag(tag);

   if (!etag)
		return;

	rotation = Vector3d(helDegToRad(rx), helDegToRad(ry), helDegToRad(rz));

	transform.setIdentity();
   transform.translate(etag->position.mVec);

   for (etag->mesh.start(); etag->mesh.forward(); etag->mesh.next())
   {
		mesh = getMesh(etag->mesh.current());

		if (!mesh)
			continue;

		for (mesh->group.start(); mesh->group.forward(); mesh->group.next())
		{   
			grp = getGroup(mesh->group.current());

			if (!grp)
				continue;

			for (grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
			{
				vert = getVertex(grp->vertex.current());

				if (!vert)
					continue;
			 
				vert->pos[0] -= etag->position.mVec[0];
				vert->pos[1] -= etag->position.mVec[1];
				vert->pos[2] -= etag->position.mVec[2];
			 
				transform.multiply3v(vert->pos, vert->pos);
			 
				vert->pos[0] += etag->position.mVec[0];
				vert->pos[1] += etag->position.mVec[1];
				vert->pos[2] += etag->position.mVec[2];
			 
				if (count == 0)
				{
					grp->bboxMin = vert->pos;
					grp->bboxMax = vert->pos;
				 
					count++;
				}
				else
				{
					resizeBoundingBox(grp, vert->pos);
				}
			}
		}
   }
	
   for (etag->slave.start(); etag->slave.forward(); etag->slave.next())
   {
		rotateTagAboutPoint(etag->slave.current(), etag->position.mVec, 
								  rx, ry, rz);
   }
}


void Egg::rotateTagAboutPoint(unsigned int tag, vec3_t p,
										vec_t rx, vec_t ry, vec_t rz)
{
   Matrix transform;
   egg_bone_t *etag;
   egg_mesh_t *mesh;
   egg_group_t *grp;
   egg_vertex_t *vert;
   unsigned int count;
   vec_t xr = helDegToRad(rx);
   vec_t yr = helDegToRad(ry);
   vec_t zr = helDegToRad(rz);


   etag = getTag(tag);

   if (!etag)
		return;

   transform.setIdentity();
   transform.rotate(xr, yr, zr);   

   for (etag->mesh.start(); etag->mesh.forward(); etag->mesh.next())
   {
		mesh = getMesh(etag->mesh.current());
	  
		if (!mesh)
			continue;
	  
		for (mesh->group.start(); mesh->group.forward(); mesh->group.next())
		{   
			grp = getGroup(mesh->group.current());
		  
			if (!grp)
				continue;

			for (grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
			{
				vert = getVertex(grp->vertex.current());
			  
				if (!vert)
					continue;
			  
				vert->pos[0] -= p[0];
				vert->pos[1] -= p[1];
				vert->pos[2] -= p[2];
			  
				transform.multiply3v(vert->pos, vert->pos);
			  
				vert->pos[0] += p[0];
				vert->pos[1] += p[1];
				vert->pos[2] += p[2];
			  
				if (count == 0)
				{
					grp->bboxMin = vert->pos;
					grp->bboxMax = vert->pos;
				  
					count++;
				}
				else
				{
					resizeBoundingBox(grp, vert->pos);
				}
			}
		}
   }

   etag->position -= p;
   etag->position = transform * etag->position;
   etag->position += p;
     
   for (etag->slave.start(); etag->slave.forward(); etag->slave.next())
   {
		rotateTagAboutPoint(etag->slave.current(), p, rx, ry, rz);
   }
}


void Egg::disconnectTag(unsigned int master, unsigned int slave)
{
	egg_bone_t *tagA;
	egg_bone_t *tagB;


	tagA = getTag(master);
	tagB = getTag(slave);

	if (!tagA || !tagB)
		return;

	tagA->slave.remove(slave);
}


void Egg::connectTag(unsigned int master, unsigned int slave)
{
	egg_bone_t *tagA;
	egg_bone_t *tagB;


	tagA = getTag(master);
	tagB = getTag(slave);

	if (!tagA || !tagB)
		return;

	tagA->slave.pushBack(slave);
}


void Egg::addTagMesh(egg_bone_t *tag, unsigned int mesh)
{
	if (!tag || !getMesh(mesh))
		return;

	tag->mesh.pushBack(mesh);
}


void Egg::delTagMesh(egg_bone_t *tag, unsigned int mesh)
{
	if (!tag || !getMesh(mesh))
		return;

	tag->mesh.remove(mesh);
}


/// BoneFrames ////////////////////////////////////////////////


void Egg::addBoneFrame(egg_skeleton_t *boneframe)
{
	mBoneFrames.pushBack(boneframe);
	boneframe->id = mBoneFrames.size() - 1;
}


egg_skeleton_t *Egg::getBoneFrame(unsigned int id)
{
	if (mBoneFrames.empty())
		return NULL;

	return mBoneFrames[id];
}


int Egg::saveBoneFrame(egg_skeleton_t *boneframe, FILE *f)
{
	int32_t li;
	u_int32_t lu;  


	if (!boneframe)
		return -1;

	// Check point
	lu = EGG_TFRM_CHUNK_START;
	eWrite_Small_uint32(&lu, f);

	li = boneframe->id;
	eWrite_Small_int32(&li, f);

	lu = boneframe->tag.size();
	eWrite_Small_uint32(&lu, f);

	for (boneframe->tag.start(); boneframe->tag.forward(); boneframe->tag.next())
	{
		lu = boneframe->tag.current();
		eWrite_Small_uint32(&lu, f);
	}

	eWrite_Small_float(&boneframe->center.mVec[0], f);
	eWrite_Small_float(&boneframe->center.mVec[1], f);
	eWrite_Small_float(&boneframe->center.mVec[2], f);

	// Check point
	lu = EGG_TFRM_CHUNK_END;
	eWrite_Small_uint32(&lu, f);

	return 0;
}


egg_skeleton_t *Egg::loadBoneFrame(FILE *f)
{
	egg_skeleton_t *boneframe;
	int32_t li;
	u_int32_t lu;  
	int i, n;


	boneframe = new egg_skeleton_t;
  
	// Check point
	fread(&lu, 4, 1, f);  

	if (lu != EGG_TFRM_CHUNK_START)
	{
		printError("BoneFrameLoad> start checkpoint [ERROR]\n");
	}

	fread(&li, 4, 1, f);
	boneframe->id = li;

	fread(&lu, 4, 1, f);
	n = lu;

	for (i = 0; i < n; i++)
	{
		fread(&lu, 4, 1, f);
		boneframe->tag.pushBack(lu);
	}

	fread(&boneframe->center.mVec[0], sizeof(vec_t), 1, f);
	fread(&boneframe->center.mVec[1], sizeof(vec_t), 1, f);
	fread(&boneframe->center.mVec[2], sizeof(vec_t), 1, f);

	// Check point
	fread(&lu, 4, 1, f);

	if (lu != EGG_TFRM_CHUNK_END)
	{
		printError("BoneFrameLoad> end checkpoint [ERROR]\n");
	}

	return boneframe;
}


unsigned int Egg::addBoneFrame(vec_t x, vec_t y, vec_t z)
{
	egg_skeleton_t *boneframe = new egg_skeleton_t;


	mBoneFrames.pushBack(boneframe);
	boneframe->id = mBoneFrames.size() - 1;
	boneframe->center.mVec[0] = x;
	boneframe->center.mVec[1] = y;
	boneframe->center.mVec[2] = z;

	mBoneFrames.pushBack(boneframe);

	return (boneframe->id);
}

// Animations///////////////////////////////

void Egg::addAnimation(egg_animation_t *a)
{
	if (a)
	{
		mAnimations.pushBack(a);
		a->id = mAnimations.size() - 1;
	}
}


egg_animation_t *Egg::getAnimation(unsigned int frame)
{
	return mAnimations[frame];
}


int Egg::saveAnimation(egg_animation_t *a, FILE *f)
{
	int32_t li;
	u_int32_t lu;  


	if (!a)
		return -1;

	// Check point
	lu = EGG_AFRM_CHUNK_START;
	eWrite_Small_uint32(&lu, f);

	li = a->id;
	eWrite_Small_int32(&li, f);

	lu = a->frame.size();
	eWrite_Small_uint32(&lu, f);

	for (a->frame.start(); a->frame.forward(); a->frame.next())
	{
		lu = a->frame.current();
		eWrite_Small_uint32(&lu, f);
	}

	// Check point
	lu = EGG_AFRM_CHUNK_END;
	eWrite_Small_uint32(&lu, f);

	return 0;
}


egg_animation_t *Egg::loadAnimation(FILE *f)
{
	egg_animation_t *a;
	int32_t li;
	u_int32_t lu;  
	int i, n;


	a = new egg_animation_t;
  
	// Check point
	fread(&lu, 4, 1, f);  

	if (lu != EGG_AFRM_CHUNK_START)
	{
		printError("AnimationLoad> start checkpoint [ERROR]\n");
	}

	fread(&li, 4, 1, f);
	a->id = li;

	fread(&lu, 4, 1, f);
	n = lu;

	for (i = 0; i < n; i++)
	{
		fread(&lu, 4, 1, f);
		a->frame.pushBack(lu);
	}

	// Check point
	fread(&lu, 4, 1, f);  

	if (lu != EGG_AFRM_CHUNK_END)
	{
		printError("AnimationLoad> end checkpoint [ERROR]\n");
	}

	return a;
}


////////////////////////////////////////////////////////////
// Protected methods                                        
////////////////////////////////////////////////////////////

Vector <egg_texcoord_t *> *Egg::getTexCoordList()
{
	return &mTexCoords;
}

Vector<egg_group_t *> *Egg::getGroupList() 
{
	return &mGroups;
}

Vector <egg_vertex_t *> *Egg::getVertexList() 
{
	return &mVertices;
}

Vector<egg_polygon_t *> *Egg::getPolygonList() 
{
	return &mPolygons;
}

Vector<egg_mesh_t *> *Egg::getMeshList()
{
	return &mMeshes;
}

Vector<egg_bone_t *> *Egg::getTagList()
{
	return &mTags;
}

Vector <egg_skeleton_t *> *Egg::getBoneFrameList()
{
	return &mBoneFrames;
}



////////////////////////////////////////////////////////////
// Private methods                                        
////////////////////////////////////////////////////////////


bool Egg::isMatchForPolygon(Vector<unsigned int> &list, egg_polygon_t *polygon)
{
	if (!polygon)
		return false;

	if (list.size() != polygon->vertex.size())
		return false;

	for (list.start(); list.forward(); list.next())
	{
		if (!isVertexInPolygon(list.current(), polygon))
			return false;
	}

	return true;
}


bool Egg::isVertexInPolygon(unsigned int vertex, egg_polygon_t *polygon)
{
	if (!polygon)
		return false;

	if (polygon->vertex.SearchIndex(vertex) != UINT_MAX)
		return true;

	return false;
}


void Egg::resizeBoundingBox(egg_group_t *grp, vec3_t p)
{
	if (!grp || grp->vertex.empty())
		return;
	
	if (grp->vertex.size() == 1)
	{
		grp->bboxMin = p;
		grp->bboxMax = p;
		return;
	}

	if (p[0] < grp->bboxMin.mVec[0]) 
		grp->bboxMin.mVec[0] = p[0];
	else if (p[0] > grp->bboxMax.mVec[0])
		grp->bboxMax.mVec[0] = p[0];

	if (p[1] < grp->bboxMin.mVec[1]) 
		grp->bboxMin.mVec[1] = p[1];
	else if (p[1] > grp->bboxMax.mVec[1])
		grp->bboxMax.mVec[1] = p[1];

	if (p[2] < grp->bboxMin.mVec[2]) 
		grp->bboxMin.mVec[2] = p[2];
	else if (p[2] > grp->bboxMax.mVec[2])
		grp->bboxMax.mVec[2] = p[2];
}


void Egg::resizeBoundingBox(egg_group_t *grp)
{
	bool init = false;
	egg_vertex_t *vertex;


	if (!grp || grp->vertex.empty())
		return;

	for (grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
	{
		vertex = mVertices.current();
    
		if (!vertex)
			continue;

		if (!init)
		{	
			grp->bboxMin = vertex->pos;
			grp->bboxMax = vertex->pos;

			init = true;
		}
		else
		{
			if (vertex->pos[0] < grp->bboxMin.mVec[0]) 
				grp->bboxMin.mVec[0] = vertex->pos[0];
			else if (vertex->pos[0] > grp->bboxMax.mVec[0])
				grp->bboxMax.mVec[0] = vertex->pos[0];
	
			if (vertex->pos[1] < grp->bboxMin.mVec[1]) 
				grp->bboxMin.mVec[1] = vertex->pos[1];
			else if (vertex->pos[1] > grp->bboxMax.mVec[1])
				grp->bboxMax.mVec[1] = vertex->pos[1];
      
			if (vertex->pos[2] < grp->bboxMin.mVec[2]) 
				grp->bboxMin.mVec[2] = vertex->pos[2];
			else if (vertex->pos[2] > grp->bboxMax.mVec[2])
				grp->bboxMax.mVec[2] = vertex->pos[2];
		}
	}
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

bool Egg::isDebugLevel(unsigned int level)
{
	return (mDebugLevel >= level);
}


void Egg::print(char *s, ...)
{
	va_list args;

	va_start(args, s);
	fprintf(stdout, "Egg::");
	vfprintf(stdout, s, args);
	fprintf(stdout, "\n");
	va_end(args);
}


void Egg::printError(char *s, ...)
{
	va_list args;
	
	va_start(args, s);
	fprintf(stderr, "Egg::");
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test Code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_EGG
int main(int argc, char *argv[])
{
	Egg egg;


	printf("[Egg class test]\n");
	egg.setDebugLevel(4); // 5 for a lot of crap

	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
			if (!egg.loadFile(argv[2]))
				printf("main: Load reports success.\n");
		}
		else if (strcmp(argv[1], "save") == 0)
		{
			// FIXME: Add code here to generate a small egg

			if (!egg.saveFile(argv[2]))
				printf("main: Save reports success.\n");
		}
		else if (strcmp(argv[1], "test") == 0 && argc > 3)
		{
			if (!egg.loadFile(argv[2]))
				printf("main: Load reports success.\n");
			if (!egg.saveFile(argv[3]))
				printf("main: Save reports success.\n");
			if (!egg.loadFile(argv[3]))
				printf("main: Load reports success.\n");
		}
		else
		{
			printf("\n\n%s [save | load | test] filename.egg [testout.egg]\n", 
					 argv[0]);
		}
	}
	else
	{
		printf("\n\n%s [save | load | test] filename.egg [testout.egg]\n", 
				 argv[0]);
	}

	return 0;
}
#endif
