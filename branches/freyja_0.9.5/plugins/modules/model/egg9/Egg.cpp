/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
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
#include <math.h>

#include <hel/Vec3.h>
#include <hel/Mat44.h>
#include <mstl/SystemIO.h>

#include "Egg.h"

using namespace mstl;
using namespace hel;


void __print_unsigned_int(unsigned int u)
{
	printf("<%u>\n", u);
}

void __print_egg_vertex_t(egg_vertex_t *v)
{
	if (!v)
		return;

	printf("<%i, %f, %f, %f>\n", v->id, v->pos[0], v->pos[1], v->pos[2]);
}

void __print_egg_texel_t(egg_texel_t *t)
{
	if (!t)
		return;

	printf("<%i, %f, %f>\n", t->id, t->st[0], t->st[1]);
}

void __print_egg_polygon_t(egg_polygon_t *p)
{
	if (!p)
		return;
	
	printf("<%i, \n", p->id);
	printf("List <vertex_id> ");
	p->vertex.print(__print_unsigned_int);
	printf("List <texel_id> ");
	p->texel.print(__print_unsigned_int);
	printf(", %i>\n", p->shader);
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Egg::Egg()
{
	mDebugLevel = 0;
	mPrinter = 0x0;
	mVertices.reserve(256);
	mTexels.reserve(256);
	mAnimations.reserve(32);
}


Egg::~Egg()
{
	clear();
}


/* Mongoose 2004.03.27, 
 * This is here as a reminder until egg switches gears 
 * to support these features better HERE IN THIS CLASS */
#ifdef ONLY_YOU_CAN_HELP_STOP_EXTERNAL_HACKS
	void UpdateRenderList(int mesh, int frame);
	/*--------------------------------------------
	 * Created  : 2000-10-10 by Mongoose
	 * Modified : 
	 *
	 * Pre      : mesh is valid mesh id 
	 *            frame is valid frame id
	 *
	 * Post     : Backend constructs a new rendering
	 *            list for mesh[frame]
	 --------------------------------------------*/


void FreyjaModel::UpdateRenderList(int msh, int frame)
{
	Vector <unsigned int> vlist;
	egg_polygon_t *poly;
	egg_group_t *grp;
	egg_group_t *current;
	egg_mesh_t *mesh;
	unsigned int i, j, key, vert;
	
	
	if (_current_frame_cached == frame)
		return;
	
	_current_frame_cached = frame;
	
	mesh = _egg->getMesh(msh);
	grp = _egg->getGroup(frame);
	current = _egg->getGroup(_current_group);

	if (!mesh || !grp || !current)
	{
		event_print("FreyjaModel::UpdateRenderList> No mesh and group to update\n");
		return;
	}
    
	printf("FreyjaModel::UpdateRenderList> Experimental render list caching...\n");
	
	/****************************************************
	 * Mongoose: 
	 * The idea is to purge the old vertex pointers,
	 * then update the pointers using the vertex indices
	 * this is how 'vertex morph framing' is done ATM
	 ***************************************************/
	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{
		poly = _egg->getPolygon(i);

		if (!poly)
			continue;

		vlist.clear();

		for (j = poly->vertex.begin(); j < poly->vertex.end(); ++j)
		{
			key = current->vertex[j];
			vert = grp->vertex[key];
			vlist.pushBack(vert);

			//printf("key = %u, vert = %u\n", key, vert);
		}
    
		poly->vertex.clear();

		for (j = vlist.begin(); j < vlist.end(); ++j)
			poly->vertex.pushBack(j);

		//printf("-\n");
	}
}

#endif


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Egg::isDebugLevel(unsigned int level)
{
	return (mDebugLevel >= level);
}


void Egg::setPrinter(FreyjaPrinter *printer)
{
	mPrinter = printer;
}


void Egg::print(char *s, ...)
{
	va_list args;

	va_start(args, s);
	if (mPrinter)
	{
		mPrinter->messageArgs(s, &args);
	}
	else
	{
		fprintf(stdout, "Egg::");
		vfprintf(stdout, s, args);
		fprintf(stdout, "\n");
	}
	va_end(args);
}


int Egg::checkFile(const char *filename)
{
	unsigned long id, version;
	SystemIO::FileReader r;


	if (!r.Open(filename))
	{
		perror(filename);
		return -1;
	}

	id = r.ReadLong();
	version = r.ReadLong();

	if (id == EGG_FILE)
	{
		// NOTE: I sometimes have versions that only differ by version id
		//       that are used for internal testing ( not public testing )
		//       This let's me test stuff w/o screwing users everytime I
		//       want to do a version rollover and be backwards compatible
		switch (version)
		{
		case EGG_8_09:
		case EGG_8_10:
		case EGG_VERSION:
			return 0;
		}
	}

	r.Close();
	
	return -2;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Egg::setDebugLevel(unsigned int n)
{
	mDebugLevel = n;
}


void Egg::clear()
{
	unsigned int i;


	for (i = mVertices.begin(); i < mVertices.end(); ++i)
	{
		if (mVertices[i])
		{
			delete mVertices[i];
		}
	}  
	
	mVertices.clear();
	
	for (i = mTexels.begin(); i < mTexels.end(); ++i)
	{
		if (mTexels[i])
		{
			delete mTexels[i];
		}
	}  
	
	mTexels.clear();
	
	for (i = mPolygons.begin(); i < mPolygons.end(); ++i)
	{
		if (mPolygons[i])
			delete mPolygons[i];
	}

	mPolygons.clear();
	
	for (i = mGroups.begin(); i < mGroups.end(); ++i)
	{
		if (mGroups[i])
			delete mGroups[i];
	}
	
	mGroups.clear();
	
	for (i = mMeshes.begin(); i < mMeshes.end(); ++i)
	{
		if (mMeshes[i])
			delete mMeshes[i];
	}
	
	mMeshes.clear();
	
	for (i = mTags.begin(); i < mTags.end(); ++i)
	{
		if (mTags[i])
			delete mTags[i];
	}
	
	mTags.clear();
	
	for (i = mBoneFrames.begin(); i < mBoneFrames.end(); ++i)
	{
		if (mBoneFrames[i])
			delete mBoneFrames[i];
	}
	
	mBoneFrames.clear();
	
	for (i = mAnimations.begin(); i < mAnimations.end(); ++i)
	{
		if (mAnimations[i])
			delete mAnimations[i];
	}
	
	mAnimations.clear();
}

#if 0
int Egg::saveFile(const char *filename)
{
	FILE *f;
	unsigned long u, version, id;
	egg_vertex_t *vertex;
	egg_texel_t *texel;
	egg_polygon_t *polygon;
	egg_group_t *group;
	unsigned int i, j;
	unsigned int meshCount, polygonCount, groupCount, texelCount, vertexCount;


	f = fopen(filename, "wb");

	if (!f)
	{
		perror("Egg::saveFile> ");
		return -1;
	}

 	for (polygonCount = 0, i = mPolygons.begin(); i < mPolygons.end(); ++i)
	{
		if (mPolygons[i])
			++polygonCount;
	}

 	for (groupCount = 0, i = mGroups.begin(); i < mGroups.end(); ++i)
	{
		if (mGroups[i])
			++groupCount;
	}

 	for (meshCount = 0, i = mMeshes.begin(); i < mMeshes.end(); ++i)
	{
		if (mMeshes[i])
			++meshCount;
	}

	for (vertexCount = 0, i = mVertices.begin(); i < mVertices.end(); ++i)
	{
		if (mVertices[i])
			++vertexCount;
	}

	for (texelCount = 0, i = mTexels.begin(); i < mTexels.end(); ++i)
	{
		if (mTexels[i])
			++texelCount;
	}


	// Semi-smart trimming
	if (!meshCount)
	{
		polygonCount = 0; // can't have faces w/o meshes
		groupCount = 0;   // let's force vertex frames/groups to dep on mesh
	}

	if (!polygonCount && !groupCount)
	{
		vertexCount = 0; // can't have vertices w/o faces OR vertex frames/groups
	}

	if (!polygonCount)
	{
		texelCount = 0; // can't have texcoords w/o faces
	}

	print("Egg file save header:");
	// Header ///////////////////////////////
	id = EGG_FILE;
	version = EGG_VERSION;
	fwrite(&id = r.ReadLongU();
	fwrite(&version = r.ReadLongU();


	u = vertexCount;
	print("%i vertices", u);
	//u = getVertexCount();
	fwrite(&u = r.ReadLongU();   


	u = texelCount;
	print("%i texels", u);
	//u = getTexelCount();
	fwrite(&u = r.ReadLongU(); 


	u = polygonCount;
	print("%i polygons", u);
	//u = getPolygonCount();
	fwrite(&u = r.ReadLongU();  


	u = 0;  // Mongoose 2002.07.05, Marker system removed
	fwrite(&u = r.ReadLongU();


	u = groupCount;
	print("%i groups", u);
	//u = getGroupCount();
	fwrite(&u = r.ReadLongU();


	u = meshCount;
	print("%i meshes", u);
	//u = getMeshCount();
	fwrite(&u = r.ReadLongU();


 	for (u = 0, i = mTags.begin(); i < mTags.end(); ++i)
	{
		if (mTags[i])
			++u;
	}
	print("%i tags", u);
	//u = getTagCount();
	fwrite(&u = r.ReadLongU();
    
	
 	for (u = 0, i = mBoneFrames.begin(); i < mBoneFrames.end(); ++i)
	{
		if (mBoneFrames[i])
			++u;
	}
	print("%i boneframes", u);
	//u = getBoneFrameCount();
	fwrite(&u = r.ReadLongU();

	
 	for (u = 0, i = mAnimations.begin(); i < mAnimations.end(); ++i)
	{
		if (mAnimations[i])
			++u;
	}
	print("%i animations", u);
	//u = getAnimationCount();
	fwrite(&u = r.ReadLongU();
  
	////////////////////////////////////////

	for (i = mVertices.begin(); i < mVertices.end(); ++i)
	{
		vertex = mVertices[i];
    
		if (!vertex || !vertexCount)
			continue;
    
		u = vertex->id;
		fwrite(&u = r.ReadLongU();
		fwrite(&vertex->pos[0] = r.ReadFloat32();
		fwrite(&vertex->pos[1] = r.ReadFloat32();
		fwrite(&vertex->pos[2] = r.ReadFloat32();
		fwrite(&vertex->uv[0] = r.ReadFloat32();
		fwrite(&vertex->uv[1] = r.ReadFloat32();
		fwrite(&vertex->norm[0] = r.ReadFloat32();
		fwrite(&vertex->norm[1] = r.ReadFloat32();
		fwrite(&vertex->norm[2] = r.ReadFloat32();
	}

	for (i = mTexels.begin(); i < mTexels.end(); ++i)
	{
		texel = mTexels[i];
    
		if (!texel || !texelCount)
			continue;
    
		u = texel->id;
		fwrite(&u = r.ReadLongU();
		fwrite(&texel->st[0] = r.ReadFloat32();
		fwrite(&texel->st[1] = r.ReadFloat32();
	} 

	// PolyMesh //////////////////////

	for (i = mPolygons.begin(); i < mPolygons.end(); ++i)
	{
		polygon = mPolygons[i];

		if (!polygon || !polygonCount)
			continue;

		//print("polygon %i %p\n", i, polygon);

		u = polygon->id;
		fwrite(&u = r.ReadLongU();
		fwrite(&polygon->shader = r.ReadInt32();

		u = polygon->vertex.size();
		fwrite(&u = r.ReadLongU();

		for (j = polygon->vertex.begin(); j < polygon->vertex.end(); ++j)
		{
			u = polygon->vertex[j];
			fwrite(&u = r.ReadLongU();
		}

		u = polygon->texel.size();
		fwrite(&u = r.ReadLongU();

		for (j = polygon->texel.begin(); j < polygon->texel.end(); ++j)
		{
			u = polygon->texel[j];
			fwrite(&u = r.ReadLongU();
		}
	}

	////////////////////////////////////////

	for (i = mGroups.begin(); i < mGroups.end(); ++i)
	{
		group = mGroups[i];
    
		if (!group || !polygonCount)
			continue;
    
		u = group->id;
		fwrite(&u = r.ReadLongU();
    
		u = group->vertex.size();
		fwrite(&u = r.ReadLongU();
    
		for (j = group->vertex.begin(); j < group->vertex.end(); ++j)
		{
			u = group->vertex[j];
			fwrite(&u = r.ReadLongU();
		}
	}

   for (i = mMeshes.begin(); i < mMeshes.end(); ++i)
   {
		MeshSave(mMeshes[i], f);
   }

   for (i = mTags.begin(); i < mTags.end(); ++i)
   {
		saveTag(mTags[i], f);
   }

   for (i = mBoneFrames.begin(); i < mBoneFrames.end(); ++i)
   {
		BoneFrameSave(mBoneFrames[i], f);
   }

   for (i = mAnimations.begin(); i < mAnimations.end(); ++i)
   {
		AnimationSave(mAnimations[i], f);
   }

   r.Close();

   return 0;
}
#endif

bool Egg::Load(const char *filename)
{
	SystemIO::FileReader r;

	if (!r.Open(filename))
	{
		perror(filename);
		return false;
	}


	long si;
	unsigned long u, version, id, i, j, n;
	float dummy;
	unsigned long num_vertex, num_texel, num_poly, num_marker;
	unsigned long num_grp, numMeshes, numTags, num_frame, num_aframe;
	Map<unsigned int, unsigned int> transV;
	Map<unsigned int, unsigned int> transT;
	Map<unsigned int, unsigned int> transP;
	Map<unsigned int, unsigned int> transG;
	Map<unsigned int, unsigned int> transVwST;
	Vector<unsigned int> vertexlist;
	Vector<unsigned int> texellist;
	float x, y, z, s, t, nx, ny, nz;
	egg_group_t *group;
	egg_vertex_t *vertex;
	egg_mesh_t *mesh;
	egg_tag_t *tag;
	egg_animation_t *animation;
	egg_boneframe_t *boneframe;
	int shader, old_texel;
	unsigned int polygonId;


	// Header ////////////////////////////
	id = r.ReadLongU();

	if (id != EGG_FILE)
	{
		printError("Load> invalid header 0x%x", id);
		return false;
	}

	version = r.ReadLongU();

	if (version != EGG_VERSION && 
		version != EGG_8_10 &&
		version != EGG_8_09)
	{
		//printError("Load> invalid or unsupported version 0x%x", version);
		return false;
	}

	num_vertex = r.ReadLongU();
	num_texel = r.ReadLongU();
	num_poly = r.ReadLongU();
	num_marker = r.ReadLongU();

	num_grp = r.ReadLongU();
	numMeshes = r.ReadLongU();
	numTags = r.ReadLongU();
	num_frame = r.ReadLongU();
	num_aframe = r.ReadLongU();

	////////////////////////////////////////

	for (i = 0; i < num_vertex; ++i)
	{
		id = r.ReadLongU();
		x = r.ReadFloat32();
		y = r.ReadFloat32();
		z = r.ReadFloat32();

		s = t = nx = ny = nz = 0.0;

		switch (version)
		{
		case EGG_VERSION:
			s = r.ReadFloat32();
			t = r.ReadFloat32();

			nx = r.ReadFloat32();
			ny = r.ReadFloat32();
			nz = r.ReadFloat32();
			break;
		case EGG_8_10:
			si = r.ReadLongU();
			old_texel = si;

			// Mongoose 2002.03.02, This normal index was never 
			//   implemented in a public release - so don't use
			si = r.ReadLongU();
			break;
		}

		vertex = addVertex(x, y, z, nx, ny, nz, s, t);
		transV.Add(id, vertex->id);

		switch (version)
		{
		case EGG_8_10:
			transVwST.Add(old_texel, id);
			break;
		}
	}
  
	for (i = 0; i < num_texel; ++i)
	{
		id = r.ReadLongU(); 
		s = r.ReadFloat32();
		t = r.ReadFloat32();

		switch (version)
		{
		case EGG_8_10:
			vertex = getVertex(transVwST[old_texel]);

			if (vertex)
			{
				vertex->uv[0] = s;
				vertex->uv[1] = t;
			}
			break;
		}

		transT.Add(id, addTexel(s, t));   
	}

	for (i = 0; i < num_poly; ++i)
	{
		id = r.ReadLongU();
		shader = r.ReadLongU();

		// Vertex indeces, translated to new ids
		n = r.ReadLongU();

		for (j = 0; j < n; ++j)
		{
			u = r.ReadLongU();
			vertexlist.pushBack(transV[u]);
		}

		// Texel indices, translated to new ids
		n = r.ReadLongU();

		for (j = 0; j < n; ++j)
		{
			u = r.ReadLongU();
			texellist.pushBack(transT[u]);
		}

		if (isDebugLevel(5))
		{
			print("*** %i %i %i %i\n", 
					i, id, vertexlist.size(), texellist.size());
		}

		polygonId = addPolygon(vertexlist, texellist, shader);
		transP.Add(id, polygonId);
		vertexlist.clear();
		texellist.clear();
	}

	// Mongoose 2002.07.05, Marker system removed
	for (i = 0; i < num_marker; ++i)
	{
		u = r.ReadLongU(); // id
		u = r.ReadLongU(); // type

		// pos
		dummy = r.ReadFloat32();
		dummy = r.ReadFloat32();
		dummy = r.ReadFloat32();

		// sizeof data
		u = r.ReadLongU();

		if (u)
		{
			unsigned char *marker_data = new unsigned char[u];

			// data
			r.ReadBuffer(u, marker_data);

			delete [] marker_data;
		}
	} 

	////////////////////////////////////////


	for (i = 0; i < num_grp; ++i)
	{
		id = r.ReadLongU();

		group = newGroup();
		addGroup(group);
		transG.Add(id, group->id);

		// vertices
		n = r.ReadLongU();

		group->vertex.reserve(n);

		for (j = 0; j < n; ++j)
		{
			u = r.ReadLongU();
			group->vertex.pushBack(transV[u]);
		}

		vertex = getVertex(group->vertex[0]);
    
		if (vertex)
		{
			group->center[0] = vertex->pos[0];
			group->center[1] = vertex->pos[1];
			group->center[2] = vertex->pos[2];
		}
		else
		{
			group->center[0] = 0.0;
			group->center[1] = 0.0;
			group->center[2] = 0.0;
		}
	}

#ifdef DEBUG_HEAVY
	printf("vertex id");
	transV.Print(__print_unsigned_int);
	printf("texel id");
	transT.Print(__print_unsigned_int);
	printf("polygon id");
	transP.Print(__print_unsigned_int);
	printf("vertex group id");
	transG.Print(__print_unsigned_int);
#endif

	for (i = 0; i < numMeshes; ++i)
	{
		mesh = MeshLoad(r);

		if (!mesh)
		{
			printf("EGG::Load> Error loading mesh[%lu] resetting model\n", i);

			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			clear();
			r.Close();

			return -10;
		}
	  
		addMesh(mesh);
	}

	mTags.reserve(numTags+8);

	for (i = 0; i < numTags; ++i)
	{
		tag = loadTag(r);

		if (!tag)
		{
			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			r.Close();
			clear();
			return -20;
		}

		if (isDebugLevel(2))
		{
			print("-- Loading Tag #%i of %i\n", i, numTags);
		}

		addTag(tag);
	}

	for (i = mTags.begin(); i < mTags.end(); ++i)
	{
		if (mTags[i])
		{
			mTags[i]->parent = -1;
		}
	}

	for (i = mTags.begin(); i < mTags.end(); ++i)
	{
		if (mTags[i])
		{
			for (j = mTags[i]->slave.begin(); j < mTags[i]->slave.end(); ++j)
			{	
				n = mTags[i]->slave[j];

				if (mTags[n])
				{
					mTags[n]->parent = mTags[i]->id;
				}
			}
		}
	}

	for (i = 0; i < num_frame; ++i)
	{
		boneframe = BoneFrameLoad(r);

		if (!boneframe)
		{
			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			r.Close();
			clear();
			return -30;
		}

		BoneFrameAdd(boneframe);
	}

	for (i = 0; i < num_aframe; ++i)
	{
		animation = AnimationLoad(r);

		if (!animation)
		{
			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			r.Close();
			clear();
			return -40;
		}

		addAnimation(animation);
	}

	r.Close();
	return true;
}


/// Texels //////////////////////////////////////////////////

unsigned int Egg::getTexelCount()
{
	return mTexels.size();
}


egg_texel_t *Egg::getTexel(unsigned int id)
{
	if (mTexels.empty() || id >= mTexels.size())
	{
		return 0x0;
	}

	return mTexels[id];
}


unsigned int Egg::addTexel(float s, float t)
{
	egg_texel_t *texel;


	texel = new egg_texel_t;
	texel->st[0] = s;
	texel->st[1] = t;

	mTexels.pushBack(texel);
	texel->id = mTexels.size() - 1;

	return texel->id;
}


void Egg::delTexel(unsigned int id)
{
	delTexel(getTexel(id));
}


void Egg::delTexel(egg_texel_t *t)
{
	if (t)
	{
		mTexels.remove(t->id);
		delete t;
	}
}


void Egg::combineTexels(unsigned int A, unsigned int B)
{
	egg_texel_t *a = getTexel(A);
	egg_texel_t *b = getTexel(B);
	egg_polygon_t *polygon;
	unsigned int i;


	if (!a || !b)
		return;

	for (i = b->ref.begin(); i < b->ref.end(); ++i)
	{
		polygon = getPolygon(b->ref[i]);

		if (polygon)
		{
			// Must be replace B with A to match sorted list ids
			polygon->texel.Replace(B, A);

			a->ref.pushBack(b->ref[i]);
		}
	}

	// Mongoose: Don't let TexelDel see we refence any polygons
	b->ref.clear();

	delTexel(B);
}

/// Vertices ////////////////////////////////////////////////


unsigned int Egg::getVertexCount()
{
	return mVertices.size();
}


egg_vertex_t *Egg::getVertex(unsigned int id)
{
	if (mVertices.empty() || id >= mVertices.size())
	{
		return 0x0;
	}

	return mVertices[id];
}


void Egg::combineVertices(unsigned int A, unsigned int B)
{
	egg_vertex_t *a = getVertex(A);
	egg_vertex_t *b = getVertex(B);
	egg_polygon_t *polygon;
	egg_group_t *grp;
	unsigned int i, j;


	if (!a || !b)
		return;

	for (i = b->ref.begin(); i < b->ref.end(); ++i)
	{
		polygon = getPolygon(b->ref[i]);

		if (polygon)
		{
			// Must be replace B with A to match sorted list ids
			polygon->vertex.Replace(B, A);
			a->ref.pushBack(b->ref[i]);
		}
	}

	// Mongoose: Don't let VertexDel see we refence any polygons
	b->ref.clear();

	// Mongoose 2002.01.19, handle groups here
	// Remove all references to dead vertex from groups
	for (i = mGroups.begin(); i < mGroups.end(); ++i)
	{
		grp = mGroups[i];
		
		for (j = grp->vertex.begin(); j < grp->vertex.end(); ++j)
		{
			if (grp->vertex[j] == B)
			{
				grp->vertex.remove(j);
			}
		}
	}

	for (i = mPolygons.begin(); i < mPolygons.end(); ++i)
	{
		if (VertexInPolygon(B, mPolygons[i]))
		{
			(mPolygons[i])->vertex.Replace(B, A);
		}
	}

	mVertices.remove(B);
	delete b;
}


egg_vertex_t *Egg::addVertex(vec_t x, vec_t y, vec_t z)
{
	return addVertex(x, y, z, 0.0, 1.0, 0.0, 0.5, 0.5);
}


egg_vertex_t *Egg::addVertex(vec_t x, vec_t y, vec_t z, 
									  vec_t nx, vec_t ny, vec_t nz,
									  vec_t u, vec_t v)

{
	egg_vertex_t *vertex;


	vertex = new egg_vertex_t;
	vertex->pos[0] = x;
	vertex->pos[1] = y;
	vertex->pos[2] = z;
	vertex->norm[0] = nx;
	vertex->norm[1] = ny;
	vertex->norm[2] = nz;
	vertex->uv[0] = u;
	vertex->uv[1] = v;

	mVertices.pushBack(vertex);
	vertex->id = mVertices.size() - 1;

	return vertex;
}


void Egg::delVertex(unsigned int id)
{
	delVertex(getVertex(id));
}


void Egg::delVertex(egg_vertex_t *v)
{
	Vector<unsigned int> tmp;
	egg_group_t *grp;
	unsigned int i, j, index;


	if (!v)
		return;

	// FIMXE: I don't think you can do this with the list
	//        I think it'll only get the first match and stop
	//        See hack in loop

	// Remove all polygons using this vertex
	for (i = mPolygons.begin(); i < mPolygons.end(); ++i)
	{
		if (VertexInPolygon(v->id, mPolygons[i]))
		{
			delPolygon(mPolygons[i]);
			i = mPolygons.begin(); // FIXME: hack to ensure clean all from list
		}
	}

	// Remove all references to vertex from groups
	for (i = mGroups.begin(); i < mGroups.end(); ++i)
	{
		grp = mGroups[i];

		if (!grp)
			continue;

		// Data model and render list update
		tmp.clear();
		tmp.copy(grp->vertex);
		grp->vertex.clear();   /* You can't erase non allocated types */

		for (j = tmp.begin(); j < tmp.end(); ++j)
		{
			if (tmp[j] != v->id && getVertex(tmp[j]))
			{
				grp->vertex.pushBack(tmp[j]);
			}
		}
	}

	index = v->id;
	delete v;
	mVertices.assign(index, 0x0);
}


unsigned int Egg::getPolygonCount()
{
	return mPolygons.size();
}


egg_polygon_t *Egg::getPolygon(unsigned int id)
{
	if (mPolygons.empty() || id >= mPolygons.end()) // was >
		return 0x0;
  
	return mPolygons[id];
}


unsigned int Egg::addPolygon(Vector<unsigned int> &vertex,
									  Vector<unsigned int> &texel, 
									  int shader)
{
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;
	unsigned int i;


	if (vertex.empty())
	{
		printError("PolygonAdd> Assertion failed, No vertices\n");
		return UINT_MAX;
	}

	//if (texel.empty())
	//{
	//	printError("PolygonAdd> WARNING: No polymapped texcoords\n");
	//}

	polygon = new egg_polygon_t;
	mPolygons.pushBack(polygon);
	polygon->id = mPolygons.size() - 1;
	polygon->shader = shader;
	polygon->flags = 0;

	for (i = texel.begin(); i < texel.end(); ++i)
	{
		tex = getTexel(texel[i]);

		if (!tex)
		{
			printf("Invalid texel %u\n", texel[i]);
			delete polygon;
			return UINT_MAX;
		}

		tex->ref.pushBack(polygon->id);
		polygon->texel.pushBack(tex->id);
		polygon->r_texel.pushBack(tex);
	}

	for (i = vertex.begin(); i < vertex.end(); ++i)
	{
		vert = getVertex(vertex[i]);

		if (!vert)
		{
			printf("Invalid vertex %u\n", vertex[i]);
			delete polygon;
			return UINT_MAX;
		}

		vert->ref.pushBack(polygon->id);
		polygon->vertex.pushBack(vert->id);
		polygon->r_vertex.pushBack(vert);
	}

	return polygon->id;
}


Vector<egg_polygon_t *> &Egg::getPolygonVector()
{
	return mPolygons;
}


void Egg::delPolygon(unsigned int polygon)
{
	egg_polygon_t *poly = getPolygon(polygon);

	delPolygon(poly);
}


void Egg::delPolygon(egg_polygon_t *polygon)
{
	egg_mesh_t *mesh;
	egg_texel_t *texel;
	unsigned int i, j;
	Vector<unsigned int> tmp;


	if (!polygon)
		return;

	// Remove all references to polygon from meshes
	for (i = mMeshes.begin(); i < mMeshes.end(); ++i)
	{
		mesh = mMeshes[i];

		if (!mesh)
			continue;

		// Data model and render list update
		tmp.clear();
		tmp.copy(mesh->polygon);
		mesh->r_polygon.clear(); /* Don't erase here b/c these pointers are
										  * allocated and controlled outside the mesh */
		mesh->polygon.clear();   /* You can't erase non allocated types */

		for (j = tmp.begin(); j < tmp.end(); ++j)
		{
			if (tmp[j] != polygon->id && getPolygon(tmp[j]))
			{
				mesh->polygon.pushBack(tmp[j]);
				mesh->r_polygon.pushBack(getPolygon(tmp[j]));
			}
		}
	}

	// Remove texels used by polygon
	for (i = polygon->texel.begin(); i < polygon->texel.end(); ++i)
	{
		texel = getTexel(polygon->texel[i]);

		if (!texel)
			continue;
    
		polygon->texel.remove(texel->id);
		texel->ref.remove(polygon->id);

		// Mongoose: account for shared texels!
		if (texel->ref.empty())
			delTexel(texel);
	}

	unsigned int index = polygon->id;
	delete polygon;
	mPolygons.remove(index);  
}


void Egg::delPolygon(Vector<unsigned int> *list)
{
	unsigned int i;

	for (i = mPolygons.begin(); i < mPolygons.end(); ++i)
	{
		if (PolygonMatch(list, mPolygons[i]))
		{
			delPolygon(mPolygons[i]);
			return;
		}
	}
}


unsigned int Egg::selectPolygon(Vector<unsigned int> *list)
{
	egg_polygon_t *polygon;
	unsigned int i;


	for (i = mPolygons.begin(); i < mPolygons.end(); ++i)
	{
		if (PolygonMatch(list, mPolygons[i]))
		{
			polygon = mPolygons[i];
			return polygon->id;
		}
	}

	return UINT_MAX;
}

/// Groups ////////////////////////////////////////////////


unsigned int Egg::getGroupCount()
{
	return mGroups.size();
}


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
	group->center[0] = group->center[1] = group->center[2] = 0.0;

	return group;
}


void Egg::delGroup(unsigned int id)
{
	egg_group_t *group = getGroup(id);

	delGroup(group);
}


void Egg::delGroup(egg_group_t *group)
{
	unsigned int i;

	if (!group)
		return;

	// FIXME: Maybe it would be nice to check and see if it's used
	//        by another group ( but groups may be obsoleted soon )
	for (i = group->vertex.begin(); i < group->vertex.end(); ++i)
	{
		if (isDebugLevel(5))
			print("delGroup> FIXME: Needs vertex reference counters %s:%d", __FILE__, __LINE__);
		// delVertex(group->vertex.current());
	}

   mGroups.remove(group->id);
   delete group;
}


/// Utils /////////////////////////////////////////


void Egg::GroupTransform(unsigned int group, enum egg_transform type, 
								 vec_t x, vec_t y, vec_t z)
{
	egg_group_t *grp = getGroup(group);


	Transform(grp, type, x, y, z);
}


void Egg::GroupMirror(unsigned int group, bool x, bool y, bool z)
{
	egg_group_t *grp;
	egg_vertex_t *v;
	unsigned int i;

  
	grp = getGroup(group);

	if (!grp)
		return;

	for (i = grp->vertex.begin(); i < grp->vertex.end(); ++i)
	{
		v = getVertex(grp->vertex[i]);

		if (y)
			v->pos[0] = -v->pos[0];

		if (x)
			v->pos[1] = -v->pos[1];      

		if (z)
			v->pos[2] = -v->pos[2];      
	}
}


egg_group_t *Egg::GroupUnion(unsigned int groupA, unsigned int groupB)
{
	//FIXME  
	printError("GroupUnion> Not implemented %s:%i\n",
				  __FILE__, __LINE__);

	return NULL;
}


egg_group_t *Egg::GroupIntersection(unsigned int groupA, unsigned int groupB)
{
	//FIXME  
	printError("GroupIntersection> Not implemented %s:%i\n",
				  __FILE__, __LINE__);

	return NULL;
}


egg_mesh_t *Egg::MeshCopy(egg_mesh_t *mesh, Vector<unsigned int> *vertices)
{
	Map <unsigned int, unsigned int> trans;
	Vector <unsigned int> vertex_list;
	Vector <unsigned int> texel_list;
	egg_mesh_t *msh;
	egg_group_t *grp;
	egg_vertex_t *vert;
	egg_vertex_t *vert2;
	egg_polygon_t *poly;
	unsigned int poly2;
	egg_texel_t *texel;
	bool partial = false;
	unsigned int i, j;


	if (!mesh || !vertices)
	{
		return NULL;
	}

	// Make a new mesh with a vertex group
	msh = newMesh();
	addMesh(msh);
	grp = newGroup();
	addGroup(grp);
	msh->group.pushBack(grp->id);

	// Copy vertices, add to group|frame, and make a translation table
	for (i = vertices->begin(); i < vertices->end(); ++i)
	{
		vert = getVertex((*vertices)[i]);
		
		if (!vert)
			continue;

		vert2 = addVertex(vert->pos[0], vert->pos[1], vert->pos[2]);

		if (!vert2)
			continue;

		grp->vertex.pushBack(vert2->id); // Add(vert2->id);
		trans.Add(vert->id, vert2->id);
	} 

	// Copy polygons, add to mesh, and use the translation table from above
	for (i = mesh->polygon.begin(); mesh->polygon.end(); ++i)
	{
		poly = getPolygon(mesh->polygon[i]);
		
		if (!poly)
			continue;

		vertex_list.clear();
		texel_list.clear();

		partial = false;

		// Weed out parital polygons, since we may only have some of the vertices
		for (j = poly->vertex.begin(); j < poly->vertex.end(); ++j)
		{
			if (trans[poly->vertex[j]] == UINT_MAX)
			{
				partial = true;
				break;
			}
			else
			{
				vertex_list.pushBack(trans[poly->vertex[j]]);
			}
		}
		
		if (partial)
			continue;

		for (j = poly->texel.begin(); j < poly->texel.end(); ++j)
		{
			texel = getTexel(poly->texel[j]);

			if (!texel)
				continue;

			texel_list.pushBack(addTexel(texel->st[0], texel->st[1]));
		}
		
		poly2 = addPolygon(vertex_list, texel_list, poly->shader);
		
		if (poly2 != UINT_MAX)
			continue;

		msh->polygon.pushBack(poly2);
	}

	return msh;
}


/// Meshes ////////////////////////////////////////////////

unsigned int Egg::getMeshCount()
{
	return mMeshes.size();
}


egg_mesh_t *Egg::newMesh()
{
	egg_mesh_t *mesh;


	mesh = new egg_mesh_t;
	return mesh;
}


void Egg::delMesh(unsigned int mesh)
{
	delMesh(getMesh(mesh));
}


void Egg::delMesh(egg_mesh_t *mesh)
{
	unsigned int i;

	if (!mesh)
		return;

	// Unlink groups/frames ( Assumes sole owner )
	for (i = mesh->group.begin(); i < mesh->group.end(); ++i)
	{
		delGroup(mesh->group[i]);
	}

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{
		delPolygon(mesh->polygon[i]);
	}

	unsigned int index = mesh->id;
	delete mesh; // delete mesh
	mMeshes.assign(index, 0x0);  // unlink from list
	//mMeshes.remove(index);

#ifdef EGG_REORDER_MESH_ON_DELETE
	Vector<egg_mesh_t *> tmp;


	for (i = mMeshes.begin(); i < i = mMeshes.end(); ++i)
	{
		if (mMeshes[i])
			tmp.pushBack(mMeshes[i]);
	}

	mMeshes.copy(tmp);
#endif
}


egg_mesh_t *Egg::getMesh(unsigned int id)
{
	if (mMeshes.empty() || id >= mMeshes.end()) // was >
		return 0x0;

	return mMeshes[id];
}


void Egg::addMesh(egg_mesh_t *mesh)
{
	if (!mesh)
		return;

	mMeshes.pushBack(mesh);
	mesh->id = mMeshes.size() - 1;
}

#if 0
int Egg::MeshSave(egg_mesh_t *mesh, FILE *f)
{
	unsigned long u;
	unsigned int i;


	if (!mesh)
		return -1;
  
	// Check point
	u = EGG_MESH_CHUNK_START;
	fwrite(&u = r.ReadLongU();

	// Vertex Group indices
	u = mesh->group.size();
	fwrite(&u = r.ReadLongU();

	for (i = mesh->group.begin(); i < mesh->group.end(); ++i)
	{
		u = mesh->group[i];
		fwrite(&u = r.ReadLongU();
	}

	// Check point
	u = 0xbdbdbdbd;
	fwrite(&u = r.ReadLongU();

	// Polygon indices
	u = mesh->polygon.size();
	fwrite(&u = r.ReadLongU();

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{
		u = mesh->polygon[i];
		fwrite(&u = r.ReadLongU();
	}

	// Check point
	u = EGG_MESH_CHUNK_END;
	fwrite(&u = r.ReadLongU();

	return 0;
}
#endif


egg_mesh_t *Egg::MeshLoad(SystemIO::FileReader &r)
{
	egg_mesh_t *mesh;
	unsigned int u, i, n;


	mesh = newMesh();
	//MeshAdd(mesh);

	// Check point
	u = r.ReadLongU();

	if (u != EGG_MESH_CHUNK_START)
	{
		printError("MeshLoad> start checkpoint [ERROR]\n");
	}

	// Vertex Group indices
	n = r.ReadLongU();

	for (i = 0; i < n; ++i)
	{
		u = r.ReadLongU();
		mesh->group.pushBack(u);
	}

	// Check point
	u = r.ReadLongU();

	if (u != 0xbdbdbdbd)
	{
		printError("MeshLoad> internal checkpoint [ERROR]\n");
	}

	// Polygon indices
	n = r.ReadLongU();
	//printDebug(5, "MeshLoad> Loading %i polygons\n", n);

	for (i = 0; i < n; ++i)
	{
		u = r.ReadLongU();
		mesh->polygon.pushBack(u);
		//printDebug(5, "MeshLoad> Adding polygon[%i] = %i\n", i, u);
	}

	// Check point
	u = r.ReadLongU();
  
	if (u != EGG_MESH_CHUNK_END)
	{
		printError("MeshLoad> end checkpoint [ERROR]\n");
	}

	return mesh;
}


/// Tags //////////////////////////////////////////////////

void Egg::updateBones()
{
	unsigned int i, j, n;


	for (i = mTags.begin(); i < mTags.end(); ++i)
	{
		if (mTags[i])
		{
			mTags[i]->parent = -1;
		}
	}

	for (i = mTags.begin(); i < mTags.end(); ++i)
	{
		if (mTags[i])
		{
			for (j = mTags[i]->slave.begin(); j < mTags[i]->slave.end(); ++j)
			{	
				n = mTags[i]->slave[j];

				if (mTags[n])
				{
					mTags[n]->parent = mTags[i]->id;
				}
			}
		}
	}
}

unsigned int Egg::getTagCount()
{
	return mTags.size();
}


egg_tag_t *Egg::getTag(unsigned int id)
{
	if (mTags.empty() || id >= mTags.size())
		return NULL;

	return mTags[id];
}

// NOTE: Just replacing this OLD ASS code with mstl file i/o would add endian
//       support
#if 0
//FIXME: Only size is handled, like the rest of this file I/O code
//       it needs ordering code in parts, and etc - but I have no
//       big endian machines at home
int Egg::saveTag(egg_tag_t *tag, SystemIO::FileWriter &w)
{
	long li;
	unsigned long lu;  
	unsigned int i;


	if (!tag)
		return -1;

	// Check point
	lu = EGG_BTAG_CHUNK_START;
	fwrite(&lu = r.ReadLongU();

	/* Extention to 8.12 format!!! */
	lu = 0x454D414E;
	fwrite(&lu = r.ReadLongU();
	fwrite(tag->name, 64, 1, f);

	li = tag->id;
	fwrite(&li = r.ReadLongU();

	lu = tag->slave.size();
	fwrite(&lu = r.ReadLongU();

	for (i = tag->slave.begin(); i < tag->slave.end(); ++i)
	{
		lu = tag->slave[i];
		fwrite(&lu = r.ReadLongU();
	}

	lu = tag->mesh.size();
	fwrite(&lu = r.ReadLongU();

	for (i = tag->mesh.begin(); i < tag->mesh.end(); ++i)
	{
		lu = tag->mesh[i];
		fwrite(&lu = r.ReadLongU();
	}

	fwrite(&tag->flag, 1, 1, f);

	fwrite(&tag->center[0] = r.ReadFloat32();
	fwrite(&tag->center[1] = r.ReadFloat32();
	fwrite(&tag->center[2] = r.ReadFloat32();

	// tag's 'transform matrix' isn't used yet, so ignore it for now  =)

	fwrite(&tag->rot[0] = r.ReadFloat32();
	fwrite(&tag->rot[1] = r.ReadFloat32();
	fwrite(&tag->rot[2] = r.ReadFloat32();

	// Check point
	lu = EGG_BTAG_CHUNK_END;
	fwrite(&lu = r.ReadLongU();

	return 0;
}
#endif

egg_tag_t *Egg::loadTag(SystemIO::FileReader &r)
{
	egg_tag_t *tag;
	long li;
	unsigned long lu;  
	int i, n;


	tag = new egg_tag_t;

	// Check point
	lu = r.ReadLongU();

	if (lu != EGG_BTAG_CHUNK_START)
	{
		printError("TagLoad> start checkpoint [ERROR]\n");
	}

	li = r.ReadLongU();
	tag->id = li;

	/* Extention to 8.12 format!!! */
	if (li == 0x454D414E)
	{
		r.ReadString(64, tag->name);

		li = r.ReadLongU();
		tag->id = li;
	}

	lu = r.ReadLongU();
	n = lu;

	for (i = 0; i < n; i++)
	{
		lu = r.ReadLongU();
		tag->slave.pushBack(lu);
	}

	lu = r.ReadLongU();
	n = lu;

	for (i = 0; i < n; i++)
	{
		lu = r.ReadLongU();
		tag->mesh.pushBack(lu);
	}

	tag->flag = r.ReadByte();

	tag->center[0] = r.ReadFloat32();
	tag->center[1] = r.ReadFloat32();
	tag->center[2] = r.ReadFloat32();

	// tag's 'transform matrix' isn't used yet, so ignore it for now  =)

	tag->rot[0] = r.ReadFloat32();
	tag->rot[1] = r.ReadFloat32();
	tag->rot[2] = r.ReadFloat32();

	// Check point
	lu = r.ReadLongU();  

	if (lu != EGG_BTAG_CHUNK_END)
	{
		printError("TagLoad> end checkpoint [ERROR]\n");
	}

	return tag; 
}


void Egg::addTag(egg_tag_t *tag)
{
	if (tag)
	{
		mTags.pushBack(tag);
		tag->id = mTags.size() - 1;
	}
}


egg_tag_t *Egg::addTag(vec_t x, vec_t y, vec_t z, char flag)
{
	egg_tag_t *tag = new egg_tag_t;  
	tag->center[0] = x;
	tag->center[1] = y;
	tag->center[2] = z;
	tag->flag = flag;
	tag->parent = -1;
	Mat44 m;//	m.setIdentity();
	m.GetMatrix(tag->transform);

	addTag(tag);

	return tag;
}


void Egg::delTag(unsigned int id)
{
	delTag(getTag(id));
}


void Egg::delTag(egg_tag_t *tag)
{
	if (!tag)
		return;

	mTags.remove(tag->id);

	delete tag;
}


void Egg::TagRotateAbout(unsigned int tag, vec_t rx, vec_t ry, vec_t rz)
{
   egg_tag_t *etag;
   float x, y, z;
   Mat44 transform, inverse, normalTransform;
   egg_mesh_t *mesh;
   egg_group_t *grp;
   egg_vertex_t *vert;
   unsigned int count;
   vec_t xr = helDegToRad(rx);
   vec_t yr = helDegToRad(ry);
   vec_t zr = helDegToRad(rz);
	unsigned int i, j, k;
	vec_t norm;


   etag = getTag(tag);

   if (!etag)
		return;

   x = etag->center[0];
   y = etag->center[1];
   z = etag->center[2];

   transform.SetIdentity();
   transform.Rotate(xr, yr, zr);
	/* Since we only do rotation here we can use vertex transform
	   for normals too */
	//transform.getInvert(inverse.mMatrix);
	//inverse.getTransposeMatrix(normalTransform.mMatrix);
   normalTransform.SetMatrix(transform.mMatrix);

   for (i = etag->mesh.begin(); i < etag->mesh.end(); ++i)
   {
		mesh = getMesh(etag->mesh[i]);

		if (!mesh)
			continue;

		for (j = mesh->group.begin(); j < mesh->group.end(); ++j)
		{   
			grp = getGroup(mesh->group[j]);

			if (!grp)
				continue;

			for (k = grp->vertex.begin(); k < grp->vertex.end(); ++k)
			{
				vert = getVertex(grp->vertex[k]);

				if (!vert)
					continue;
			 
				vert->pos[0] -= etag->center[0];
				vert->pos[1] -= etag->center[1];
				vert->pos[2] -= etag->center[2];

				transform.Multiply3fv(vert->pos, vert->pos);

				vert->pos[0] += etag->center[0];
				vert->pos[1] += etag->center[1];
				vert->pos[2] += etag->center[2];

				normalTransform.Multiply3fv(vert->norm);
				norm = sqrt(vert->norm[0]*vert->norm[0] + vert->norm[1]*vert->norm[1] + vert->norm[2]*vert->norm[2]);
			 	vert->norm[0] /= norm;
			 	vert->norm[1] /= norm;
			 	vert->norm[2] /= norm;

				if (count == 0)
				{
					grp->bbox_min[0] = vert->pos[0];
					grp->bbox_min[1] = vert->pos[1];
					grp->bbox_min[2] = vert->pos[2];
				 
					grp->bbox_max[0] = vert->pos[0];
					grp->bbox_max[1] = vert->pos[1];
					grp->bbox_max[2] = vert->pos[2];
				 
					count++;
				}
				else
					resizeBoundingBox(grp, vert->pos);
			}
		}
   }
	
   for (i = etag->slave.begin(); i < etag->slave.end(); ++i)
   {
		TagRotateAboutPoint(etag->slave[i], etag->center, rx, ry, rz);
   }
}


void Egg::TagRotateAboutPoint(unsigned int tag, vec3_t p,
										vec_t rx, vec_t ry, vec_t rz)
{
   egg_tag_t *etag;
   Mat44 m;
   egg_mesh_t *mesh;
   egg_group_t *grp;
   egg_vertex_t *vert;
   unsigned int count;
   vec_t xr = helDegToRad(rx);
   vec_t yr = helDegToRad(ry);
   vec_t zr = helDegToRad(rz);
	unsigned int i, j, k;


   etag = getTag(tag);

   if (!etag)
		return;

   //m.setIdentity();
   m.Rotate(xr, yr, zr);   

   for (i = etag->mesh.begin(); i < etag->mesh.end(); ++i)
   {
		mesh = getMesh(etag->mesh[i]);
	  
		if (!mesh)
			continue;
	  
		for (j = mesh->group.begin(); j < mesh->group.end(); ++j)
		{   
			grp = getGroup(mesh->group[j]);
		  
			if (!grp)
				continue;

			for (k = grp->vertex.begin(); k < grp->vertex.end(); ++k)
			{
				vert = getVertex(grp->vertex[k]);
			  
				if (!vert)
					continue;
			  
				vert->pos[0] -= p[0];
				vert->pos[1] -= p[1];
				vert->pos[2] -= p[2];
			  
				m.Multiply3fv(vert->pos);
			  
				vert->pos[0] += p[0];
				vert->pos[1] += p[1];
				vert->pos[2] += p[2];
			  
				if (count == 0)
				{
					grp->bbox_min[0] = vert->pos[0];
					grp->bbox_min[1] = vert->pos[1];
					grp->bbox_min[2] = vert->pos[2];
				  
					grp->bbox_max[0] = vert->pos[0];
					grp->bbox_max[1] = vert->pos[1];
					grp->bbox_max[2] = vert->pos[2];
				  
					count++;
				}
				else
				{
					resizeBoundingBox(grp, vert->pos);
				}
			}
		}
   }

   etag->center[0] -= p[0];
   etag->center[1] -= p[1];
   etag->center[2] -= p[2];
     
   m.Multiply3fv(etag->center);
     
   etag->center[0] += p[0];
   etag->center[1] += p[1];
   etag->center[2] += p[2];  
     
   for (i = etag->slave.begin(); i < etag->slave.end(); ++i)
   {
		TagRotateAboutPoint(etag->slave[i], p, rx, ry, rz);
   }
}


void Egg::TagDisconnect(unsigned int master, unsigned int slave)
{
	egg_tag_t *tagA;
	egg_tag_t *tagB;


	tagA = getTag(master);
	tagB = getTag(slave);

	if (!tagA || !tagB)
		return;

	/* Mongoose 2004.12.08, 
	 * Mark parent as null (-1 index) */
	tagB->parent = -1;
	tagA->slave.remove(slave);
}


void Egg::connectTag(unsigned int master, unsigned int slave)
{
	egg_tag_t *tagA;
	egg_tag_t *tagB;


	tagA = getTag(master);
	tagB = getTag(slave);

	if (!tagA || !tagB)
		return;

	tagB->parent = master;
	tagA->slave.pushBack(slave);
}


void Egg::TagAddMesh(egg_tag_t *tag, unsigned int mesh)
{
	if (!tag || !getMesh(mesh))
		return;

	tag->mesh.pushBack(mesh);
}


void Egg::TagDelMesh(egg_tag_t *tag, unsigned int mesh)
{
	if (!tag || !getMesh(mesh))
		return;

	tag->mesh.remove(mesh);
}


/// BoneFrames ////////////////////////////////////////////////


unsigned int Egg::getBoneFrameCount()
{
	return mBoneFrames.size();
}


void Egg::BoneFrameAdd(egg_boneframe_t *boneframe)
{
	mBoneFrames.pushBack(boneframe);
	boneframe->id = mBoneFrames.size() - 1;
}


egg_boneframe_t *Egg::BoneFrame(unsigned int id)
{
	if (mBoneFrames.empty())
		return NULL;

	return mBoneFrames[id];
}


#if 0
int Egg::BoneFrameSave(egg_boneframe_t *boneframe, FILE *f)
{
	long li;
	unsigned long lu;  
	unsigned int i;


	if (!boneframe)
		return -1;

	// Check point
	lu = EGG_TFRM_CHUNK_START;
	fwrite(&lu = r.ReadLongU();

	li = boneframe->id;
	fwrite(&li = r.ReadLongU();

	lu = boneframe->tag.size();
	fwrite(&lu = r.ReadLongU();

	for (i = boneframe->tag.begin(); i < boneframe->tag.end(); ++i)
	{
		lu = boneframe->tag[i];
		fwrite(&lu = r.ReadLongU();
	}

	fwrite(&boneframe->center[0] = r.ReadFloat32();
	fwrite(&boneframe->center[1] = r.ReadFloat32();
	fwrite(&boneframe->center[2] = r.ReadFloat32();

	// Check point
	lu = EGG_TFRM_CHUNK_END;
	fwrite(&lu = r.ReadLongU();

	return 0;
}
#endif


egg_boneframe_t *Egg::BoneFrameLoad(SystemIO::FileReader &r)
{
	egg_boneframe_t *boneframe;
	long li;
	unsigned long lu;  
	int i, n;


	boneframe = new egg_boneframe_t;
  
	// Check point
	lu = r.ReadLongU();  

	if (lu != EGG_TFRM_CHUNK_START)
	{
		printError("BoneFrameLoad> start checkpoint [ERROR]\n");
	}

	li = r.ReadLongU();
	boneframe->id = li;

	lu = r.ReadLongU();
	n = lu;

	for (i = 0; i < n; i++)
	{
		lu = r.ReadLongU();
		boneframe->tag.pushBack(lu);
	}

	boneframe->center[0] = r.ReadFloat32();
	boneframe->center[1] = r.ReadFloat32();
	boneframe->center[2] = r.ReadFloat32();

	// Check point
	lu = r.ReadLongU();

	if (lu != EGG_TFRM_CHUNK_END)
	{
		printError("BoneFrameLoad> end checkpoint [ERROR]\n");
	}

	return boneframe;
}


unsigned int Egg::BoneFrameAdd(vec_t x, vec_t y, vec_t z)
{
	egg_boneframe_t *boneframe = new egg_boneframe_t;


	mBoneFrames.pushBack(boneframe);
	boneframe->id = mBoneFrames.size() - 1;
	boneframe->center[0] = x;
	boneframe->center[1] = y;
	boneframe->center[2] = z;

	mBoneFrames.pushBack(boneframe);

	return (boneframe->id);
}

// Animations///////////////////////////////

unsigned int Egg::getAnimationCount()
{
	return mAnimations.size();
}

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


#if 0
int Egg::AnimationSave(egg_animation_t *a, FILE *f)
{
	long li;
	unsigned long lu;  
	unsigned int i;

	if (!a)
		return -1;

	// Check point
	lu = EGG_AFRM_CHUNK_START;
	fwrite(&lu = r.ReadLongU();

	li = a->id;
	fwrite(&li = r.ReadLongU();

	lu = a->frame.size();
	fwrite(&lu = r.ReadLongU();

	for (i = a->frame.begin(); i < a->frame.end(); ++i)
	{
		lu = a->frame[i];
		fwrite(&lu = r.ReadLongU();
	}

	// Check point
	lu = EGG_AFRM_CHUNK_END;
	fwrite(&lu = r.ReadLongU();

	return 0;
}
#endif


egg_animation_t *Egg::AnimationLoad(SystemIO::FileReader &r)
{
	egg_animation_t *a;
	long li;
	unsigned long lu;  
	int i, n;


	a = new egg_animation_t;
  
	// Check point
	lu = r.ReadLongU();  

	if (lu != EGG_AFRM_CHUNK_START)
	{
		printError("AnimationLoad> start checkpoint [ERROR]\n");
	}

	li = r.ReadLongU();
	a->id = li;

	lu = r.ReadLongU();
	n = lu;

	for (i = 0; i < n; i++)
	{
		lu = r.ReadLongU();
		a->frame.pushBack(lu);
	}

	// Check point
	lu = r.ReadLongU();  

	if (lu != EGG_AFRM_CHUNK_END)
	{
		printError("AnimationLoad> end checkpoint [ERROR]\n");
	}

	return a;
}

/////////////////////////////////////////////////////////////////


void Egg::Transform(Vector<egg_vertex_t *> *list, enum egg_transform type,
						  vec_t x, vec_t y, vec_t z)
{
	egg_vertex_t *vert;
	Mat44 m, inverse, normalTransform;
	unsigned int i;
	vec_t norm;

	if (!list)
		return;

	//m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.Scale(x, y, z);
		break;
	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.Rotate(x, y, z);
		break;
	case TRANSLATE:
		m.Translate(x, y, z);
		break;
	default:
		return;
	}

	m.GetInverse(inverse);
	inverse.GetTranspose(normalTransform); // WTF?  oh well, bug for bug
	normalTransform.SetMatrix(inverse.mMatrix);

	for (i = list->begin(); i < list->end(); ++i)
	{
		vert = (*list)[i];

		if (!vert)
			continue;

		m.Multiply3fv(vert->pos);
		normalTransform.Multiply3fv(vert->norm);
		norm = sqrt(vert->norm[0]*vert->norm[0] + vert->norm[1]*vert->norm[1] + vert->norm[2]*vert->norm[2]);
	 	vert->norm[0] /= norm;
	 	vert->norm[1] /= norm;
	 	vert->norm[2] /= norm;
	}
}


void Egg::Transform(egg_tag_t *etag, enum egg_transform type, 
						  vec_t x, vec_t y, vec_t z)
{
	Mat44 m;


	if (!etag)
		return;

	//m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.Scale(x, y, z);
		break;


	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.Rotate(x, y, z);

		// FIXME
		printError("Egg::Transform> ( Tag ) Not fully implemented %s:%i\n", 
					  __FILE__, __LINE__);
		/* to avoid extra rotation in scene
		etag->rot[0] += x;
		etag->rot[1] += y;
		etag->rot[2] += z;

		if (etag->rot[0] > 360.0f)
			etag->rot[0] -= 360.0f;

		if (etag->rot[1] > 360.0f)
			etag->rot[1] -= 360.0f;

		if (etag->rot[2] > 360.0f)
			etag->rot[2] -= 360.0f;
		*/
		break;


	case TRANSLATE:
		m.Translate(x, y, z);
		break;


	default:
		return;
	}

	m.Multiply3fv(etag->center, etag->center);
}


void Egg::Transform(egg_group_t *grp, enum egg_transform type, 
						  vec_t x, vec_t y, vec_t z)
{
	egg_vertex_t *vert;
	Mat44 m, inverse, normalTransform;
	unsigned int i, count;
	vec_t norm;


	if (!grp)
		return;

	//m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.Scale(x, y, z);
		break;
	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.Rotate(x, y, z);
		break;
	case TRANSLATE:
		m.Translate(x, y, z);
		break;
	default:
		return;
	}

	m.GetInverse(inverse);
	inverse.GetTranspose(normalTransform); // WTF?  oh well, bug for bug
	normalTransform.SetMatrix(inverse.mMatrix);

	m.Multiply3fv(grp->center, grp->center);
  
	for (count = 0, i = grp->vertex.begin(); i < grp->vertex.end(); ++i)
	{
		vert = getVertex(grp->vertex[i]);

		if (!grp)
			continue;

		m.Multiply3fv(vert->pos);
		normalTransform.Multiply3fv(vert->norm);
		norm = sqrt(vert->norm[0]*vert->norm[0] + vert->norm[1]*vert->norm[1] + vert->norm[2]*vert->norm[2]);
	 	vert->norm[0] /= norm;
	 	vert->norm[1] /= norm;
	 	vert->norm[2] /= norm;

		if (count == 0)
		{
			grp->bbox_min[0] = vert->pos[0];
			grp->bbox_min[1] = vert->pos[1];
			grp->bbox_min[2] = vert->pos[2];
	
			grp->bbox_max[0] = vert->pos[0];
			grp->bbox_max[1] = vert->pos[1];
			grp->bbox_max[2] = vert->pos[2];
	
			count++;
		}
		else
			resizeBoundingBox(grp, vert->pos);
	}
}


void Egg::Transform(egg_mesh_t *mesh, enum egg_transform type, 
						  vec_t x, vec_t y, vec_t z)
{
	Mat44 m, inverse, normalTransform;
	egg_group_t *grp;
	egg_vertex_t *vert;
	unsigned int i, j, count;
	vec_t norm;

	if (!mesh)
		return;

	//m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.Scale(x, y, z);
		break;
	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);

		m.Rotate(x, y, z);
		break;
	case ROTATE_ABOUT_CENTER:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);

		m.Rotate(x, y, z);

		//grp = Group(mesh->group.Current());
		//x = grp->center[0];
		//y = grp->center[1];
		//z = grp->center[2];
		break;
	case TRANSLATE:
		m.Translate(x, y, z);
		break;
	default:
		return;
	}

	m.GetInverse(inverse);
	inverse.GetTranspose(normalTransform);  // WTF?  oh well, bug for bug
	normalTransform.SetMatrix(inverse.mMatrix);

	for (i = mesh->group.begin(); i < mesh->group.end(); ++i)
	{
		grp = getGroup(mesh->group[i]);

		if (!grp)
			continue;

		switch (type)
		{
		case ROTATE_ABOUT_CENTER:
			break;
// 		case TRANSLATE:
// 			// FIXME: matrix translate broken?
// 			grp->center[0] += x;
// 			grp->center[1] += y;
// 			grp->center[2] += z;
// 			break;
		default:
			m.Multiply3fv(grp->center);
		}
    
		count = 0;

		for (j = grp->vertex.begin(); j < grp->vertex.end(); ++j)
		{
			vert = getVertex(grp->vertex[j]);

			if (!grp)
				continue;

			normalTransform.Multiply3fv(vert->norm, vert->norm);
			norm = sqrt(vert->norm[0]*vert->norm[0] + vert->norm[1]*vert->norm[1] + vert->norm[2]*vert->norm[2]);
		 	vert->norm[0] /= norm;
		 	vert->norm[1] /= norm;
		 	vert->norm[2] /= norm;

			switch (type)
			{
			case ROTATE_ABOUT_CENTER:
				vert->pos[0] -= grp->center[0];
				vert->pos[1] -= grp->center[1];
				vert->pos[2] -= grp->center[2];
	
				m.Multiply3fv(vert->pos, vert->pos);
	
				vert->pos[0] += grp->center[0];
				vert->pos[1] += grp->center[1];
				vert->pos[2] += grp->center[2];
				break;
// 			case TRANSLATE:
// 				// FIXME: matrix translate broken?
// 				vert->pos[0] += x;
// 				vert->pos[1] += y;
// 				vert->pos[2] += z;
// 				break;
			default:
				m.Multiply3fv(vert->pos, vert->pos);
			}

			if (count == 0)
			{
				grp->bbox_min[0] = vert->pos[0];
				grp->bbox_min[1] = vert->pos[1];
				grp->bbox_min[2] = vert->pos[2];
	
				grp->bbox_max[0] = vert->pos[0];
				grp->bbox_max[1] = vert->pos[1];
				grp->bbox_max[2] = vert->pos[2];
	
				count++;
			}
			else
			{
				resizeBoundingBox(grp, vert->pos);
			}
		}
	}
}


void Egg::Transform(enum egg_transform type, vec_t x, vec_t y, vec_t z)
{
	Mat44 m, inverse, normalTransform;
	egg_tag_t *tag;
	egg_mesh_t *mesh;
	egg_group_t *grp;
	egg_vertex_t *vert;
	unsigned int i, j, k, count;
	vec_t norm;

	//m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.Scale(x, y, z);
		break;
	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);

		m.Rotate(x, y, z);
		break;
	case TRANSLATE:
		m.Translate(x, y, z);
		break;
	default:
		return;
	}

	for (i = mTags.begin(); i < mTags.end(); ++i)
	{
		tag = mTags[i];

		if (!tag)
			continue;  

		m.Multiply3fv(tag->center, tag->center);
	}

	m.GetInverse(inverse);
	inverse.GetTranspose(normalTransform); // bug for bug
	normalTransform.SetMatrix(inverse.mMatrix);

	for (i = mMeshes.begin(); i < mMeshes.end(); ++i)
	{
		mesh = mMeshes[i];

		if (!mesh)
			continue;

		for (j = mesh->group.begin(); j < mesh->group.end(); ++j)
		{
			grp = getGroup(mesh->group[j]);

			if (!grp)
				continue;

			m.Multiply3fv(grp->center, grp->center);

			count = 0;

			for (k = grp->vertex.begin(); k < grp->vertex.end(); ++k)
			{
				vert = getVertex(grp->vertex[k]);

				if (!vert)
					continue;

				m.Multiply3fv(vert->pos, vert->pos);
				normalTransform.Multiply3fv(vert->norm, vert->norm);
				norm = sqrt(vert->norm[0]*vert->norm[0] + vert->norm[1]*vert->norm[1] + vert->norm[2]*vert->norm[2]);
			 	vert->norm[0] /= norm;
			 	vert->norm[1] /= norm;
			 	vert->norm[2] /= norm;

				if (count == 0)
				{
					grp->bbox_min[0] = vert->pos[0];
					grp->bbox_min[1] = vert->pos[1];
					grp->bbox_min[2] = vert->pos[2];
	  
					grp->bbox_max[0] = vert->pos[0];
					grp->bbox_max[1] = vert->pos[1];
					grp->bbox_max[2] = vert->pos[2];
	
					count++;
				}
				else
				{
					resizeBoundingBox(grp, vert->pos);
				}
			}
		}
	}
}


////////////////////////////////////////////////////////////
// Protected methods                                        
////////////////////////////////////////////////////////////


Vector <egg_texel_t *> *Egg::TexelList()
{
	return &mTexels;
}

Vector<egg_group_t *> *Egg::GroupList() 
{
	return &mGroups;
}

Vector <egg_vertex_t *> *Egg::VertexList() 
{
	return &mVertices;
}

Vector<egg_polygon_t *> *Egg::PolygonList() 
{
	return &mPolygons;
}

Vector<egg_mesh_t *> *Egg::MeshList()
{
	return &mMeshes;
}

Vector<egg_tag_t *> *Egg::TagList()
{
	return &mTags;
}

Vector <egg_boneframe_t *> *Egg::BoneFrameList()
{
	return &mBoneFrames;
}

Vector<egg_animation_t *> *Egg::AnimationList()
{
	return &mAnimations;
}


////////////////////////////////////////////////////////////
// Private methods                                        
////////////////////////////////////////////////////////////


bool Egg::PolygonMatch(Vector<unsigned int> *list, egg_polygon_t *polygon)
{
	unsigned int i;


	if (!list || !polygon)
		return false;

	if (list->size() != polygon->vertex.size())
		return false;

	for (i = list->begin(); i < list->end(); ++i)
	{
		if (!VertexInPolygon((*list)[i], polygon))
			return false;
	}

	return true;
}


bool Egg::VertexInPolygon(unsigned int vertex, egg_polygon_t *polygon)
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
		grp->bbox_min[0] = p[0];
		grp->bbox_max[0] = p[0];

		grp->bbox_min[1] = p[1];
		grp->bbox_max[1] = p[1];

		grp->bbox_min[2] = p[2];
		grp->bbox_max[2] = p[2];
		return;
	}

	if (p[0] < grp->bbox_min[0]) 
		grp->bbox_min[0] = p[0];
	else if (p[0] > grp->bbox_max[0])
		grp->bbox_max[0] = p[0];

	if (p[1] < grp->bbox_min[1]) 
		grp->bbox_min[1] = p[1];
	else if (p[1] > grp->bbox_max[1])
		grp->bbox_max[1] = p[1];

	if (p[2] < grp->bbox_min[2]) 
		grp->bbox_min[2] = p[2];
	else if (p[2] > grp->bbox_max[2])
		grp->bbox_max[2] = p[2];
}


void Egg::resizeBoundingBox(egg_group_t *grp)
{
	bool init = false;
	egg_vertex_t *vertex;
	unsigned int i;


	if (!grp || grp->vertex.empty())
		return;

	for (i = grp->vertex.begin(); i < grp->vertex.end(); ++i)
	{
		vertex = mVertices[i];
    
		if (!vertex)
			continue;

		if (!init)
		{	
			grp->bbox_min[0] = vertex->pos[0];
			grp->bbox_max[0] = vertex->pos[0];
			grp->bbox_min[1] = vertex->pos[1];
			grp->bbox_max[1] = vertex->pos[1];      
			grp->bbox_min[2] = vertex->pos[2];
			grp->bbox_max[2] = vertex->pos[2];      

			init = true;
		}
		else
		{
			if (vertex->pos[0] < grp->bbox_min[0]) 
				grp->bbox_min[0] = vertex->pos[0];
			else if (vertex->pos[0] > grp->bbox_max[0])
				grp->bbox_max[0] = vertex->pos[0];
	
			if (vertex->pos[1] < grp->bbox_min[1]) 
				grp->bbox_min[1] = vertex->pos[1];
			else if (vertex->pos[1] > grp->bbox_max[1])
				grp->bbox_max[1] = vertex->pos[1];
      
			if (vertex->pos[2] < grp->bbox_min[2]) 
				grp->bbox_min[2] = vertex->pos[2];
			else if (vertex->pos[2] > grp->bbox_max[2])
				grp->bbox_max[2] = vertex->pos[2];
		}
	}
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

void Egg::printError(char *s, ...)
{
	va_list args;
	
	va_start(args, s);
	if (mPrinter)
	{
		mPrinter->errorArgs(s, &args);
	}
	else
	{
		fprintf(stderr, "Egg::");
		vfprintf(stderr, s, args);
		fprintf(stderr, "\n");
	}
	va_end(args);
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test Code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_EGG

#include "EggPlugin.h"

void import_driver(Egg *egg, char *filename)
{
	char plugindir[512];

	printf("[Egg import_driver]\n");

	snprintf(plugindir, 512, "%s/.freyja/plugins/", getenv("HOME"));
	EggPlugin test(egg, plugindir);

	test.importModel(filename);
}


// Random calls really
void driver(Egg *egg)
{
	unsigned int j, i;


	printf("[Egg driver]\n");

	EggPlugin(egg, "~/.freyja/plugins/");

	for (i = 0; i < 20; ++i)
	{
		printf("Making sphere %d\n", i);
		eggGenerateSphere(20, 20, 30);
		printf("Deleting sphere %d\n", i);
		egg->delMesh(i);
	}

	eggGenerateVertexNormals();

	printf("==========================================\n");

	for (i = 0; i < 20; ++i)
	{
		printf("Making sphere %d\n", i);
		eggGenerateSphere(20, 20, 30);
	}

	printf("==========================================\n");

	for (j = 0; j < 45; ++j)
	{
		printf("Deleting sphere %d\n", j);
		egg->delMesh(j);
	}

	eggGenerateVertexNormals();

	printf("==========================================\n");

	for (i = 0; i < 20; ++i)
	{
		printf("Making sphere %d\n", i);
		eggGenerateSphere(20, 20, 30);

		for (j = 0; j < 45; ++j)
		{
			printf("Deleting sphere %d\n", j);
			egg->delMesh(j);
		}
	}

	eggGenerateVertexNormals();
}


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
			{
				printf("main: Load reports success.\n");
			}
			else
			{
			}

			return 0;
		}
		else if (strcmp(argv[1], "save") == 0)
		{
			// FIXME: Add code here to generate a small egg

			if (!egg.saveFile(argv[2]))
				printf("main: Save reports success.\n");
			return 0;
		}
		else if (strcmp(argv[1], "iotest") == 0 && argc > 3)
		{
			driver(&egg);

			if (!egg.loadFile(argv[2]))
				printf("main: Load reports success.\n");
			if (!egg.saveFile(argv[3]))
				printf("main: Save reports success.\n");
			if (!egg.loadFile(argv[3]))
				printf("main: Load reports success.\n");
			
			return 0;
		}
		else if (!strcmp(argv[1], "import"))
		{
			import_driver(&egg, argv[2]);
		}
	}
	else if (!strcmp(argv[1], "test"))
	{
		driver(&egg);
	}
	
	printf("\n\n%s [save | load | test | iotest] filename.egg [iotestout.egg]\n", 
			 argv[0]);

	return 0;
}
#endif





////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS

#include <mstl/Vector.h>
#include <mstl/Map.h>
#include <mstl/List.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/MeshABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/freyja.h>


extern "C" {

	int freyja_model__eggv7_check(char *filename);
	int freyja_model__eggv7_import(char *filename);

	int freyja_model__eggv9_check(char *filename);
	int freyja_model__eggv9_import(char *filename);
	int freyja_model__eggv9_export(char *filename);

	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginName1s("egg9");
	freyjaPluginDescription1s("EGG 7,8,9 (*.egg)");
	freyjaPluginAddExtention1s("*.egg");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH |
						 FREYJA_PLUGIN_SKELETON |
						 FREYJA_PLUGIN_VERTEX_MORPHING);
}


int import_model(char *filename)
{
	if (!freyja_model__eggv7_check(filename))
		return freyja_model__eggv7_import(filename);

	if (!freyja_model__eggv9_check(filename))
		return freyja_model__eggv9_import(filename);

	return -1;
}


int freyja_model__eggv7_check(char *filename)
{
	SystemIO::FileReader r;
	char header[32];

	if (!r.Open(filename))
	{
		perror(filename);
		return -1;
	}

	r.ReadString(32, header);      
	header[9] = 0;

	r.Close();

	if (strcmp(header, "Egg v7B") == 0)
		return 0;

	return -2;
}


int freyja_model__eggv9_check(char *filename)
{
	SystemIO::FileReader r;
	long magic, version;


	if (!r.Open(filename))
	{
		perror(filename);
		return -1;
	}

	r.SetByteOrder(SystemIO::File::LITTLE);
	

	// Header ////////////////////////////
	magic = r.ReadLong();

	if (magic != EGG_FILE)
	{
		//freyjaPrintError("Load> invalid header 0x%x", magic);
		return -2;
	}

	version= r.ReadLong();

	if (version != EGG_VERSION && 
		version != EGG_8_10 &&
		version != EGG_8_09)
	{
		//freyjaPrintError("Load> invalid or unsupported version 0x%x", version);
		return -3;
	}

	r.Close();

	return 0;
}

int freyja_model__eggv9_import(char *filename)
{
	Egg egg;

	if (!egg.Load(filename))
		return -1;

	
	freyjaBegin(FREYJA_MODEL);

	uint32 m, v, f, i;
	foreach (egg.GetMeshes(), m)
	{
		egg_mesh_t *mesh = egg.GetMeshes()[m];

		if (!mesh)
			continue;

		freyjaBegin(FREYJA_MESH);

		/* Vertices with index mappings */
		Vector<uint32> vertices;
		Map<long, uint32> verticesMap;

		foreach (mesh->polygon, f)
		{
			egg_polygon_t *polygon = egg.GetPolygons()[f];

			foreach (polygon->vertex, v)
			{
				uint32 vv = polygon->vertex[v];

				if (vertices.SearchIndex(vv) == vertices.GetErrorIndex())
				{
					verticesMap.Add(vv, vertices.size());
					vertices.pushBack(vv);
	
					egg_vertex_t *vertex = egg.GetVertices()[vv];

					i = freyjaVertexCreate3fv(vertex->pos);
					freyjaVertexNormal3fv(i, vertex->norm);
					freyjaVertexTexCoord2fv(i, vertex->uv);
				}
			}
		}

		foreach (mesh->polygon, f)
		{
			egg_polygon_t *polygon = egg.GetPolygons()[f];

			freyjaBegin(FREYJA_POLYGON);

			foreach (polygon->vertex, v)
			{
				uint32 vv = verticesMap[polygon->vertex[v]];
				freyjaPolygonVertex1i(vv);
			}

			// FIXME: not supporting polymapped
			foreach (polygon->texel, v)
			{
				//uint32 vv = polygon->texel[v];
				//freyjaPolygonTexCoord1i(vv);
			}

			freyjaPolygonMaterial1i(polygon->shader);

			freyjaEnd(); // FREYJA_POLYGON
		}

		freyjaEnd(); // FREYJA_MESH
	}

	freyjaEnd(); // FREYJA_MODEL


	return 0;
}


int freyja_model__eggv7_import(char *filename)
{
	SystemIO::FileReader r;

	int id, size, texture, material, type, slaves;
	int animation_count, mesh_count, bone_count, free_poly_count;
	int frame_count, marker_count, polygon_count, vertex_count;
	int i, ii, tmp, j, m;
	char header[32];
	bool bad_poly;
	float center[3];
	float pos[3];
	float st[2];
	List<unsigned int> trans;
	List<unsigned int> actual;
	Map<unsigned int, unsigned int> transM;
	Map<unsigned int, unsigned int> transT;
	unsigned int vertex, vt;
	long index;


	if (!r.Open(filename))
	{
		perror(filename);
		return -1;
	}

	r.ReadString(8, header);

	if (!(strcmp(header, "Egg v7B") == 0))
	{
		printf("Load> ERROR %s isn't Egg v7B.\n", filename);
		r.Close();
		return -2;
	}
  
	mesh_count = r.ReadInt32();    
	free_poly_count = r.ReadInt32();    
	bone_count = r.ReadInt32();
	animation_count = r.ReadInt32();

	freyjaBegin(FREYJA_MODEL);

	for (m = 0; m < mesh_count; m++)
	{
		r.ReadString(8, header);

		if (!(strcmp(header, "Egg m7D") == 0))
		{
			printf("LoadV7> Mesh isn't a valid 'Egg m7D' mesh\n");
			return -3;
		}

		freyjaBegin(FREYJA_MESH);
		transM.Add(m, freyjaGetCurrent(FREYJA_MESH));

		frame_count = r.ReadInt32();    
		marker_count = r.ReadInt32();
		polygon_count = r.ReadInt32();

		printf("LoadV7> %i frames, %i markers\n", frame_count, marker_count);

		vt = 0;

		for (i = 0; i < frame_count; i++)
		{
			center[0] = r.ReadFloat32();
			center[1] = r.ReadFloat32();
			center[2] = r.ReadFloat32();

			tmp = r.ReadInt32();
			vertex_count = r.ReadInt32();
    
			printf("LoadV7> group[%i] { %i vertices }\n", i, vertex_count);

			// Start a new vertex group
			freyjaBegin(FREYJA_VERTEX_GROUP);
			freyjaGroupCenter3f(center[0], center[1], center[2]);

			// ALL frames (groups) have same number of vertices in V7
			for (ii = 0; ii < vertex_count; ii++)
			{
				pos[0] = r.ReadFloat32();
				pos[1] = r.ReadFloat32();
				pos[2] = r.ReadFloat32();
				id = r.ReadInt32();
	
				// Store vertices in group
				vertex = freyjaVertexCreate3f(pos[0], pos[1], pos[2]);
	
				// Mongoose: Here I track the loaded id and actual id vs the ii id
				//           You fear it, I'm sure... this let's us map on 1:n 
				//           objects ( why we don't use Map here )
				trans.Add(id);
				actual.Add(vertex);

				//printf("LoadV7> trans[%i] = %i\n", id, vertex);
			}

			// End FREYJA_GROUP
			freyjaEnd();
		}
    
		for (ii = 0; ii < polygon_count; ii++)
		{      
			// Start a new polygon
			freyjaBegin(FREYJA_POLYGON);
      
			size = r.ReadInt32();
			texture = r.ReadInt32();
			material = r.ReadInt32();

			bad_poly = false;
       
			for (j = 0; j < size; j++)
			{
				id = r.ReadInt32();
				st[0] = r.ReadFloat32();
				st[1] = r.ReadFloat32();

				// Mongoose: Get actual id based on loaded id and packed id 
				//printf("trans[%i] = ", id);
				id = actual[trans.SearchKey(id)];
				//printf("%i\n", id);

				freyjaPolygonVertex1i(id);  
				freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));
			}
       
			if (!bad_poly)
			{
				freyjaPolygonMaterial1i(texture);
	 
				// End FREYJA_POLYGON
				freyjaEnd();
			}
		}

		// Mongoose: Flush the id translation tables per "frame"
		trans.Clear();
		actual.Clear();

		for (i = 0; i < marker_count; i++)
		{
			id = r.ReadInt32();
			type = r.ReadByte();
			pos[0] = r.ReadFloat32();
			pos[1] = r.ReadFloat32();
			pos[2] = r.ReadFloat32();
       
			// FIXME: No support for markers, but never used as far as I rememeber
			printf("eggv7_import> Eggv7 Metadata not exported from plugin\n");
		}

		// End FREYJA_MESH
		freyjaEnd();
	}

	for (j = 0; j < bone_count; j++)
	{
		id = r.ReadInt32();

		// Note: No longer allow plugin to control backend
		//       and the backend supports more than one model,
		//       so swaping m and mesh_count works around that
		//       by seeing how many meshes where loaded from above
		mesh_count = r.ReadInt32();

		if (mesh_count > m)
			return -5;

		slaves = r.ReadInt32();
		pos[0] = r.ReadFloat32();
		pos[1] = r.ReadFloat32();
		pos[2] = r.ReadFloat32();

		printf("tag[%i] {\n", id);
		printf(" center ( %f %f %f )\n", pos[0], pos[1], pos[2]);

		// Start bone tag/bolton
		freyjaBegin(FREYJA_BONE);
		index = freyjaGetCurrent(FREYJA_BONE);
		transT.Add(id, index);
		freyjaBoneTranslate3fv(index, pos);
		freyjaBoneFlags(index, 0x00);

		printf(" %i meshes:\n   ", mesh_count);

		for (i = 0; i < mesh_count; i++)
		{
			id = r.ReadInt32();
			printf(" %i", id);

			// FIXME: 0.9.5 obsolete ABI call
			//freyjaBoneAddMesh(index, transM[id]);
		}

		printf("\n %i slaves:\n   ", slaves);

		for (i = 0; i < slaves; i++)
		{
			id = r.ReadInt32();
			printf(" %i", id);

			freyjaBoneAddChild(index, transT[id]);
		}

		printf("\n}\n");

		// End FREYJA_BONE_TAG
		freyjaEnd();
	}

	freyjaEnd(); // FREYJA_MODEL

	r.Close();

	return 0;
}
#endif

















