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

#include <hel/Vector3d.h>
#include <hel/Matrix.h>


#include "Egg.h"


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


void Egg::print(char *s, ...)
{
	va_list args;

	va_start(args, s);
	fprintf(stdout, "Egg::");
	vfprintf(stdout, s, args);
	fprintf(stdout, "\n");
	va_end(args);
}


int Egg::checkFile(char *filename)
{
	unsigned long id, version;
	FILE *f;


	f = fopen(filename, "rb");

	if (!f)
	{
		perror("Egg::CheckFile> ");
		return -1;
	}

	fread(&id, 4, 1, f);
	fread(&version, 4, 1, f);

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


int Egg::saveFile(char *filename)
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
	fwrite(&id, 4, 1, f);
	fwrite(&version, 4, 1, f);


	u = vertexCount;
	print("%i vertices", u);
	//u = getVertexCount();
	fwrite(&u, 4, 1, f);   


	u = texelCount;
	print("%i texels", u);
	//u = getTexelCount();
	fwrite(&u, 4, 1, f); 


	u = polygonCount;
	print("%i polygons", u);
	//u = getPolygonCount();
	fwrite(&u, 4, 1, f);  


	u = 0;  // Mongoose 2002.07.05, Marker system removed
	fwrite(&u, 4, 1, f);


	u = groupCount;
	print("%i groups", u);
	//u = getGroupCount();
	fwrite(&u, 4, 1, f);


	u = meshCount;
	print("%i meshes", u);
	//u = getMeshCount();
	fwrite(&u, 4, 1, f);


 	for (u = 0, i = mTags.begin(); i < mTags.end(); ++i)
	{
		if (mTags[i])
			++u;
	}
	print("%i tags", u);
	//u = getTagCount();
	fwrite(&u, 4, 1, f);
    
	
 	for (u = 0, i = mBoneFrames.begin(); i < mBoneFrames.end(); ++i)
	{
		if (mBoneFrames[i])
			++u;
	}
	print("%i boneframes", u);
	//u = getBoneFrameCount();
	fwrite(&u, 4, 1, f);

	
 	for (u = 0, i = mAnimations.begin(); i < mAnimations.end(); ++i)
	{
		if (mAnimations[i])
			++u;
	}
	print("%i animations", u);
	//u = getAnimationCount();
	fwrite(&u, 4, 1, f);
  
	////////////////////////////////////////

	for (i = mVertices.begin(); i < mVertices.end(); ++i)
	{
		vertex = mVertices[i];
    
		if (!vertex || !vertexCount)
			continue;
    
		u = vertex->id;
		fwrite(&u, 4, 1, f);
		fwrite(&vertex->pos[0], sizeof(vec_t), 1, f);
		fwrite(&vertex->pos[1], sizeof(vec_t), 1, f);
		fwrite(&vertex->pos[2], sizeof(vec_t), 1, f);
		fwrite(&vertex->uv[0], sizeof(vec_t), 1, f);
		fwrite(&vertex->uv[1], sizeof(vec_t), 1, f);
		fwrite(&vertex->norm[0], sizeof(vec_t), 1, f);
		fwrite(&vertex->norm[1], sizeof(vec_t), 1, f);
		fwrite(&vertex->norm[2], sizeof(vec_t), 1, f);
	}

	for (i = mTexels.begin(); i < mTexels.end(); ++i)
	{
		texel = mTexels[i];
    
		if (!texel || !texelCount)
			continue;
    
		u = texel->id;
		fwrite(&u, 4, 1, f);
		fwrite(&texel->st[0], sizeof(vec_t), 1, f);
		fwrite(&texel->st[1], sizeof(vec_t), 1, f);
	} 

	// PolyMesh //////////////////////

	for (i = mPolygons.begin(); i < mPolygons.end(); ++i)
	{
		polygon = mPolygons[i];

		if (!polygon || !polygonCount)
			continue;

		//print("polygon %i %p\n", i, polygon);

		u = polygon->id;
		fwrite(&u, 4, 1, f);
		fwrite(&polygon->shader, sizeof(int), 1, f);

		u = polygon->vertex.size();
		fwrite(&u, 4, 1, f);

		for (j = polygon->vertex.begin(); j < polygon->vertex.end(); ++j)
		{
			u = polygon->vertex[j];
			fwrite(&u, 4, 1, f);
		}

		u = polygon->texel.size();
		fwrite(&u, 4, 1, f);

		for (j = polygon->texel.begin(); j < polygon->texel.end(); ++j)
		{
			u = polygon->texel[j];
			fwrite(&u, 4, 1, f);
		}
	}

	////////////////////////////////////////

	for (i = mGroups.begin(); i < mGroups.end(); ++i)
	{
		group = mGroups[i];
    
		if (!group || !polygonCount)
			continue;
    
		u = group->id;
		fwrite(&u, 4, 1, f);
    
		u = group->vertex.size();
		fwrite(&u, 4, 1, f);
    
		for (j = group->vertex.begin(); j < group->vertex.end(); ++j)
		{
			u = group->vertex[j];
			fwrite(&u, 4, 1, f);
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

   fclose(f);

   return 0;
}


int Egg::loadFile(char *filename)
{
	long si;
	unsigned long u, version, id, i, j, n;
	float dummy;
	unsigned long num_vertex, num_texel, num_poly, num_marker;
	unsigned long num_grp, numMeshes, numTags, num_frame, num_aframe;
	FILE *f;
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
		 version != EGG_8_10 &&
		 version != EGG_8_09)
	{
		printError("Load> invalid or unsupported version 0x%x", version);
		return -3;
	}

	fread(&num_vertex, 4, 1, f);   
	fread(&num_texel, 4, 1, f);   
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

		s = t = nx = ny = nz = 0.0;

		switch (version)
		{
		case EGG_VERSION:
			fread(&s, sizeof(vec_t), 1, f);
			fread(&t, sizeof(vec_t), 1, f);

			fread(&nx, sizeof(vec_t), 1, f);
			fread(&ny, sizeof(vec_t), 1, f);
			fread(&nz, sizeof(vec_t), 1, f);
			break;
		case EGG_8_10:
			fread(&si, 4, 1, f);
			old_texel = si;

			// Mongoose 2002.03.02, This normal index was never 
			//   implemented in a public release - so don't use
			fread(&si, 4, 1, f);
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
		fread(&id, 4, 1, f); 
		fread(&s, sizeof(float), 1, f);
		fread(&t, sizeof(float), 1, f);

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
		fread(&id, 4, 1, f);
		fread(&shader, 4, 1, f);

		// Vertex indeces, translated to new ids
		fread(&n, 4, 1, f);

		for (j = 0; j < n; ++j)
		{
			fread(&u, 4, 1, f);
			vertexlist.pushBack(transV[u]);
		}

		// Texel indices, translated to new ids
		fread(&n, 4, 1, f);

		for (j = 0; j < n; ++j)
		{
			fread(&u, 4, 1, f);
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
		fread(&u, 4, 1, f); // id
		fread(&u, 4, 1, f); // type

		// pos
		fread(&dummy, sizeof(vec_t), 1, f);
		fread(&dummy, sizeof(vec_t), 1, f);
		fread(&dummy, sizeof(vec_t), 1, f);

		// sizeof data
		fread(&u, 4, 1, f);

		if (u)
		{
			unsigned char *marker_data = new unsigned char[u];

			// data
			fread(marker_data, u, 1, f);

			delete [] marker_data;
		}
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
		mesh = MeshLoad(f);

		if (!mesh)
		{
			printf("EGG::Load> Error loading mesh[%lu] resetting model\n", i);

			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			clear();
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
		boneframe = BoneFrameLoad(f);

		if (!boneframe)
		{
			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			fclose(f);
			clear();
			return -30;
		}

		BoneFrameAdd(boneframe);
	}

	for (i = 0; i < num_aframe; ++i)
	{
		animation = AnimationLoad(f);

		if (!animation)
		{
			// FIXME: Hhhmm... could be bad for prefabs... ( clears whole model )
			fclose(f);
			clear();
			return -40;
		}

		addAnimation(animation);
	}

	fclose(f);
	return 0;
}


/// Texels //////////////////////////////////////////////////

unsigned int Egg::getTexelCount()
{
	return mTexels.size();
}


egg_texel_t *Egg::getTexel(unsigned int id)
{
	if (mTexels.empty())
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
	if (mVertices.empty())
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
	if (mPolygons.empty() || id > mPolygons.end())
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
	mMeshes.remove(index);  // unlink from list
}


egg_mesh_t *Egg::getMesh(unsigned int id)
{
	if (mMeshes.empty() || id > mMeshes.end())
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


int Egg::MeshSave(egg_mesh_t *mesh, FILE *f)
{
	unsigned long u;
	unsigned int i;


	if (!mesh)
		return -1;
  
	// Check point
	u = EGG_MESH_CHUNK_START;
	fwrite(&u, 4, 1, f);

	// Vertex Group indices
	u = mesh->group.size();
	fwrite(&u, 4, 1, f);

	for (i = mesh->group.begin(); i < mesh->group.end(); ++i)
	{
		u = mesh->group[i];
		fwrite(&u, 4, 1, f);
	}

	// Check point
	u = 0xbdbdbdbd;
	fwrite(&u, 4, 1, f);

	// Polygon indices
	u = mesh->polygon.size();
	fwrite(&u, 4, 1, f);

	for (i = mesh->polygon.begin(); i < mesh->polygon.end(); ++i)
	{
		u = mesh->polygon[i];
		fwrite(&u, 4, 1, f);
	}

	// Check point
	u = EGG_MESH_CHUNK_END;
	fwrite(&u, 4, 1, f);

	return 0;
}


egg_mesh_t *Egg::MeshLoad(FILE *f)
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

//FIXME: Only size is handled, like the rest of this file I/O code
//       it needs ordering code in parts, and etc - but I have no
//       big endian machines at home
int Egg::saveTag(egg_tag_t *tag, FILE *f)
{
	long li;
	unsigned long lu;  
	unsigned int i;


	if (!tag)
		return -1;

	// Check point
	lu = EGG_BTAG_CHUNK_START;
	fwrite(&lu, 4, 1, f);

	/* Extention to 8.12 format!!! */
	lu = 0x454D414E;
	fwrite(&lu, 4, 1, f);
	fwrite(tag->name, 64, 1, f);

	li = tag->id;
	fwrite(&li, 4, 1, f);

	lu = tag->slave.size();
	fwrite(&lu, 4, 1, f);

	for (i = tag->slave.begin(); i < tag->slave.end(); ++i)
	{
		lu = tag->slave[i];
		fwrite(&lu, 4, 1, f);
	}

	lu = tag->mesh.size();
	fwrite(&lu, 4, 1, f);

	for (i = tag->mesh.begin(); i < tag->mesh.end(); ++i)
	{
		lu = tag->mesh[i];
		fwrite(&lu, 4, 1, f);
	}

	fwrite(&tag->flag, 1, 1, f);

	fwrite(&tag->center[0], sizeof(vec_t), 1, f);
	fwrite(&tag->center[1], sizeof(vec_t), 1, f);
	fwrite(&tag->center[2], sizeof(vec_t), 1, f);

	// tag's 'transform matrix' isn't used yet, so ignore it for now  =)

	fwrite(&tag->rot[0], sizeof(vec_t), 1, f);
	fwrite(&tag->rot[1], sizeof(vec_t), 1, f);
	fwrite(&tag->rot[2], sizeof(vec_t), 1, f);

	// Check point
	lu = EGG_BTAG_CHUNK_END;
	fwrite(&lu, 4, 1, f);

	return 0;
}


egg_tag_t *Egg::loadTag(FILE *f)
{
	egg_tag_t *tag;
	long li;
	unsigned long lu;  
	int i, n;


	tag = new egg_tag_t;

	// Check point
	fread(&lu, 4, 1, f);

	if (lu != EGG_BTAG_CHUNK_START)
	{
		printError("TagLoad> start checkpoint [ERROR]\n");
	}

	fread(&li, 4, 1, f);
	tag->id = li;

	/* Extention to 8.12 format!!! */
	if (li == 0x454D414E)
	{
		fread(tag->name, 64, 1, f);

		fread(&li, 4, 1, f);
		tag->id = li;
	}

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

	fread(&tag->center[0], sizeof(vec_t), 1, f);
	fread(&tag->center[1], sizeof(vec_t), 1, f);
	fread(&tag->center[2], sizeof(vec_t), 1, f);

	// tag's 'transform matrix' isn't used yet, so ignore it for now  =)

	fread(&tag->rot[0], sizeof(vec_t), 1, f);
	fread(&tag->rot[1], sizeof(vec_t), 1, f);
	fread(&tag->rot[2], sizeof(vec_t), 1, f);

	// Check point
	fread(&lu, 4, 1, f);  

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
	egg_tag_t *tag;
	Matrix m;


	tag = new egg_tag_t;  
	tag->center[0] = x;
	tag->center[1] = y;
	tag->center[2] = z;
	tag->flag = flag;
	tag->parent = -1;
	m.setIdentity();
	m.getMatrix(tag->transform);

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
   Matrix transform, inverse, normalTransform;
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

   x = etag->center[0];
   y = etag->center[1];
   z = etag->center[2];

   transform.setIdentity();
   transform.rotate(xr, yr, zr);
	/* Since we only do rotation here we can use vertex transform
	   for normals too */
	//transform.getInvert(inverse.mMatrix);
	//inverse.getTransposeMatrix(normalTransform.mMatrix);
	normalTransform.setMatrix(transform.mMatrix);

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

				transform.multiply3v(vert->pos, vert->pos);

				vert->pos[0] += etag->center[0];
				vert->pos[1] += etag->center[1];
				vert->pos[2] += etag->center[2];

				normalTransform.multiply3v(vert->norm, vert->norm);

			 
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
   Matrix m;
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

   m.setIdentity();
   m.rotate(xr, yr, zr);   

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
			  
				m.multiply3v(vert->pos, vert->pos);
			  
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
     
   m.multiply3v(etag->center, etag->center);
     
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


int Egg::BoneFrameSave(egg_boneframe_t *boneframe, FILE *f)
{
	long li;
	unsigned long lu;  
	unsigned int i;


	if (!boneframe)
		return -1;

	// Check point
	lu = EGG_TFRM_CHUNK_START;
	fwrite(&lu, 4, 1, f);

	li = boneframe->id;
	fwrite(&li, 4, 1, f);

	lu = boneframe->tag.size();
	fwrite(&lu, 4, 1, f);

	for (i = boneframe->tag.begin(); i < boneframe->tag.end(); ++i)
	{
		lu = boneframe->tag[i];
		fwrite(&lu, 4, 1, f);
	}

	fwrite(&boneframe->center[0], sizeof(vec_t), 1, f);
	fwrite(&boneframe->center[1], sizeof(vec_t), 1, f);
	fwrite(&boneframe->center[2], sizeof(vec_t), 1, f);

	// Check point
	lu = EGG_TFRM_CHUNK_END;
	fwrite(&lu, 4, 1, f);

	return 0;
}


egg_boneframe_t *Egg::BoneFrameLoad(FILE *f)
{
	egg_boneframe_t *boneframe;
	long li;
	unsigned long lu;  
	int i, n;


	boneframe = new egg_boneframe_t;
  
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

	fread(&boneframe->center[0], sizeof(vec_t), 1, f);
	fread(&boneframe->center[1], sizeof(vec_t), 1, f);
	fread(&boneframe->center[2], sizeof(vec_t), 1, f);

	// Check point
	fread(&lu, 4, 1, f);

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


int Egg::AnimationSave(egg_animation_t *a, FILE *f)
{
	long li;
	unsigned long lu;  
	unsigned int i;

	if (!a)
		return -1;

	// Check point
	lu = EGG_AFRM_CHUNK_START;
	fwrite(&lu, 4, 1, f);

	li = a->id;
	fwrite(&li, 4, 1, f);

	lu = a->frame.size();
	fwrite(&lu, 4, 1, f);

	for (i = a->frame.begin(); i < a->frame.end(); ++i)
	{
		lu = a->frame[i];
		fwrite(&lu, 4, 1, f);
	}

	// Check point
	lu = EGG_AFRM_CHUNK_END;
	fwrite(&lu, 4, 1, f);

	return 0;
}


egg_animation_t *Egg::AnimationLoad(FILE *f)
{
	egg_animation_t *a;
	long li;
	unsigned long lu;  
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

/////////////////////////////////////////////////////////////////


void Egg::Transform(Vector<egg_vertex_t *> *list, enum egg_transform type,
						  vec_t x, vec_t y, vec_t z)
{
	egg_vertex_t *vert;
	Matrix m, inverse, normalTransform;
	unsigned int i;


	if (!list)
		return;

	m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.scale(x, y, z);
		break;
	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.rotate(x, y, z);
		break;
	case TRANSLATE:
		m.translate(x, y, z);
		break;
	default:
		return;
	}

	m.getInvert(inverse.mMatrix);
	inverse.getTransposeMatrix(normalTransform.mMatrix);
	normalTransform.setMatrix(inverse.mMatrix);

	for (i = list->begin(); i < list->end(); ++i)
	{
		vert = (*list)[i];

		if (!vert)
			continue;

		m.multiply3v(vert->pos, vert->pos);
		normalTransform.multiply3v(vert->norm, vert->norm);
	}
}


void Egg::Transform(egg_tag_t *etag, enum egg_transform type, 
						  vec_t x, vec_t y, vec_t z)
{
	Matrix m;


	if (!etag)
		return;

	m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.scale(x, y, z);
		break;
	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.rotate(x, y, z);
		break;
	case TRANSLATE:
		m.translate(x, y, z);
		break;
	default:
		return;
	}

	m.multiply3v(etag->center, etag->center);

	//FIXME: transform groups
	printError("Egg::Transform> ( Tag ) Not fully implemented %s:%i\n", 
				  __FILE__, __LINE__);
}


void Egg::Transform(egg_group_t *grp, enum egg_transform type, 
						  vec_t x, vec_t y, vec_t z)
{
	egg_vertex_t *vert;
	Matrix m;
	unsigned int i, count;


	if (!grp)
		return;

	m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.scale(x, y, z);
		break;
	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);
		m.rotate(x, y, z);
		break;
	case TRANSLATE:
		m.translate(x, y, z);
		break;
	default:
		return;
	}

	m.multiply3v(grp->center, grp->center);
  
	for (count = 0, i = grp->vertex.begin(); i < grp->vertex.end(); ++i)
	{
		vert = getVertex(grp->vertex[i]);

		if (!grp)
			continue;

		m.multiply3v(vert->pos, vert->pos);

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
	Matrix m;
	egg_group_t *grp;
	egg_vertex_t *vert;
	unsigned int i, j, count;


	if (!mesh)
		return;

	m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.scale(x, y, z);
		break;
	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);

		m.rotate(x, y, z);
		break;
	case ROTATE_ABOUT_CENTER:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);

		m.rotate(x, y, z);

		//grp = Group(mesh->group.Current());
		//x = grp->center[0];
		//y = grp->center[1];
		//z = grp->center[2];
		break;
	case TRANSLATE:
		m.translate(x, y, z);
		break;
	default:
		return;
	}

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
			m.multiply3v(grp->center, grp->center);
		}
    
		count = 0;

		for (j = grp->vertex.begin(); j < grp->vertex.end(); ++j)
		{
			vert = getVertex(grp->vertex[j]);

			if (!grp)
				continue;

			switch (type)
			{
			case ROTATE_ABOUT_CENTER:
				vert->pos[0] -= grp->center[0];
				vert->pos[1] -= grp->center[1];
				vert->pos[2] -= grp->center[2];
	
				m.multiply3v(vert->pos, vert->pos);
	
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
				m.multiply3v(vert->pos, vert->pos);
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
	Matrix m;
	egg_tag_t *tag;
	egg_mesh_t *mesh;
	egg_group_t *grp;
	egg_vertex_t *vert;
	unsigned int i, j, k, count;


	m.setIdentity();

	switch (type)
	{
	case SCALE:
		m.scale(x, y, z);
		break;
	case ROTATE:
		x = helDegToRad(x);
		y = helDegToRad(y);
		z = helDegToRad(z);

		m.rotate(x, y, z);
		break;
	case TRANSLATE:
		m.translate(x, y, z);
		break;
	default:
		return;
	}

	for (i = mTags.begin(); i < mTags.end(); ++i)
	{
		tag = mTags[i];

		if (!tag)
			continue;  

		m.multiply3v(tag->center, tag->center);
	}

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

			m.multiply3v(grp->center, grp->center);

			count = 0;

			for (k = grp->vertex.begin(); k < grp->vertex.end(); ++k)
			{
				vert = getVertex(grp->vertex[k]);

				if (!vert)
					continue;

				m.multiply3v(vert->pos, vert->pos);

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
