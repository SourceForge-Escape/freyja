/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
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
 ==============================================================*/

#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Egg.h"


#define DEBUG_EGG_LOAD


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
	unsigned int key, vert;
	
	
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
	for (mesh->polygon.start(); mesh->polygon.forward(); mesh->polygon.next())
	{
		poly = _egg->getPolygon(mesh->polygon.current());

		if (!poly)
			continue;

		vlist.clear();

		for (poly->vertex.start(); poly->vertex.forward(); poly->vertex.next())
		{
			key = current->vertex[poly->vertex.current()];
			vert = grp->vertex[key];
			vlist.pushBack(vert);

			//printf("key = %u, vert = %u\n", key, vert);
		}
    
		poly->vertex.clear();

		for (vlist.start(); vlist.forward(); vlist.next())
			poly->vertex.pushBack(vlist.current());

		//printf("-\n");
	}
}

#endif

////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

egg_group_t *Egg::getNearestGroup(unit_t x, unit_t y, egg_plane plane)
{
	egg_group_t *best = NULL;
	egg_group_t *current = NULL;
	unit_t dist = 0.0;
	unit_t closest = 9999.0;
	int xx = 0, yy = 1;


	if (mGroups.empty())
	{
		printf("eggNearestGroup> Assertion failed, No groups\n");
		return NULL;
	}

	/* Avoid looping branch / switch */
	switch (plane)
	{
	case PLANE_XY: 
		xx = 0;
		yy = 1;
		break;
	case PLANE_XZ: 
		xx = 0;
		yy = 2;
		break;
	case PLANE_YZ: 
		xx = 1;
		yy = 2;    
		break;
	}
	
	for (mGroups.start(); mGroups.forward(); mGroups.next())
	{
		current = mGroups.current();

		if (!current)
			continue;

		dist = mtkDist2d(x, y, current->center[xx], current->center[yy]);

		// printf("*** dist %f\n", dist);

		if (!best || dist < closest)
		{
			best = current;
			closest = dist;
		}
	}

	return best;
}


egg_tag_t *Egg::getNearestTag(unit_t x, unit_t y, egg_plane plane)
{
	egg_tag_t *best = NULL;
	egg_tag_t *current = NULL;
	unit_t dist = 0.0;
	unit_t close = 0.0;
	int xx = 0, yy = 1;


	if (mTags.empty())
		return NULL;

	// Oh how cheap it is to avoid a looping branch
	switch (plane)
	{
	case PLANE_XY: 
		xx = 0;
		yy = 1;
		break;
	case PLANE_XZ: 
		xx = 0;
		yy = 2;
		break;
	case PLANE_YZ: 
		xx = 1;
		yy = 2;    
		break;
	}
     
	for (mTags.start(); mTags.forward(); mTags.next())
	{
		current = mTags.current();

		if (!current)
			continue;

		// Init dist
		if (!best) 
		{
			best = current;

			close = mtkDist2d(x, y, current->center[xx], current->center[yy]);
		}
		else
		{
			dist = mtkDist2d(x, y, current->center[xx], current->center[yy]);
			
			if (dist < close)
			{
				best = current;
				close = dist;
			}
		}
	}

	return best;
}


egg_vertex_t *Egg::getNearestVertex(egg_group_t *group, 
												unit_t x, unit_t y, egg_plane plane)
{
	unit_t dist = 0.0;
	unit_t close = 0.0;
	int xx = 0, yy = 1;
	egg_vertex_t *best = NULL;
	egg_vertex_t *current = NULL;
	
	
	if (mVertices.empty() || !group || group->vertex.empty())
	{
		return NULL;
	}
	
	// Oh how cheap it is to avoid a branch in loop
	switch (plane)
	{
	case PLANE_XY: 
		xx = 0;
		yy = 1;
		break;
	case PLANE_XZ: 
		xx = 0;
		yy = 2;
		break;
	case PLANE_YZ: 
		xx = 1;
		yy = 2;    
		break;
	}
	
	for (group->vertex.start(); group->vertex.forward(); group->vertex.next())
	{
		current = mVertices[group->vertex.current()];

		if (!current)
		{
			continue;
		}

		// Init dist
		if (!best) 
		{
			best = current;

			close = mtkDist2d(x, y, current->pos[xx], current->pos[yy]);
		}
		else
		{
			dist = mtkDist2d(x, y, current->pos[xx], current->pos[yy]);

			if (dist < close)
			{
				best = current;
				close = dist;
			}
		}
	}

	return best;
}


void Egg::print()
{
	if (mDebugLevel > 0)
	{
		printf("List <egg_vertex_t> ");
		mVertices.print(__print_egg_vertex_t);

		printf("List <egg_texel_t> ");
		mTexels.print(__print_egg_texel_t);

		printf("List <egg_polygon_t> ");
		mPolygons.print(__print_egg_polygon_t);
	}  
}


int Egg::checkFile(char *filename)
{
	u_int32_t id, version;
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
	for (mVertices.start(); mVertices.forward(); mVertices.next())
	{
		if (mVertices.current())
		{
			delete mVertices.current();
		}
	}  
	
	mVertices.clear();
	
	for (mTexels.start(); mTexels.forward(); mTexels.next())
	{
		if (mTexels.current())
		{
			delete mTexels.current();
		}
	}  
	
	mTexels.clear();
	
	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		if (mPolygons.current())
			delete mPolygons.current();
	}

	mPolygons.clear();
	
	for (mGroups.start(); mGroups.forward(); mGroups.next())
	{
		if (mGroups.current())
			delete mGroups.current();
	}
	
	mGroups.clear();
	
	for (mMeshes.start(); mMeshes.forward(); mMeshes.next())
	{
		if (mMeshes.current())
			delete mMeshes.current();
	}
	
	mMeshes.clear();
	
	for (mTags.start(); mTags.forward(); mTags.next())
	{
		if (mTags.current())
			delete mTags.current();
	}
	
	mTags.clear();
	
	for (mBoneFrames.start(); mBoneFrames.forward(); mBoneFrames.next())
	{
		if (mBoneFrames.current())
			delete mBoneFrames.current();
	}
	
	mBoneFrames.clear();
	
	for (mAnimations.start(); mAnimations.forward(); mAnimations.next())
	{
		if (mAnimations.current())
			delete mAnimations.current();
	}
	
	mAnimations.clear();
}


int Egg::saveFile(char *filename)
{
	FILE *f;
	u_int32_t u, version, id;
	//int32_t i;
	//unsigned char c;
	egg_vertex_t *vertex;
	egg_texel_t *texel;
	egg_polygon_t *polygon;
	egg_group_t *group;


	f = fopen(filename, "wb");

	if (!f)
	{
		perror("Egg::saveFile> ");
		return -1;
	}

	// Header ///////////////////////////////
	id = EGG_FILE;
	version = EGG_VERSION;
	fwrite(&id, 4, 1, f);
	fwrite(&version, 4, 1, f);

	u = getVertexCount();
	fwrite(&u, 4, 1, f);   
	u = getTexelCount();
	fwrite(&u, 4, 1, f);   
	u = getPolygonCount();
	fwrite(&u, 4, 1, f);  
	u = 0;  // Mongoose 2002.07.05, Marker system removed
	fwrite(&u, 4, 1, f);
	u = getGroupCount();
	fwrite(&u, 4, 1, f);
	u = getMeshCount();
	fwrite(&u, 4, 1, f);
	u = getTagCount();
	fwrite(&u, 4, 1, f);    
	u = getBoneFrameCount();
	fwrite(&u, 4, 1, f);
	u = getAnimationCount();
	fwrite(&u, 4, 1, f);
  
	////////////////////////////////////////

	for (mVertices.start(); mVertices.forward(); mVertices.next())
	{
		vertex = mVertices.current();
    
		if (!vertex)
			continue;
    
		u = vertex->id;
		fwrite(&u, 4, 1, f);
		fwrite(&vertex->pos[0], sizeof(unit_t), 1, f);
		fwrite(&vertex->pos[1], sizeof(unit_t), 1, f);
		fwrite(&vertex->pos[2], sizeof(unit_t), 1, f);
		fwrite(&vertex->uv[0], sizeof(unit_t), 1, f);
		fwrite(&vertex->uv[1], sizeof(unit_t), 1, f);
		fwrite(&vertex->norm[0], sizeof(unit_t), 1, f);
		fwrite(&vertex->norm[1], sizeof(unit_t), 1, f);
		fwrite(&vertex->norm[2], sizeof(unit_t), 1, f);

#ifdef OBSOLETE
		// Mongoose 2002.03.02, Texel
		c = (unsigned char)(vertex->uv[0] / 255.0);
		fwrite(&c, 1, 1, f);
		c = (unsigned char)(vertex->uv[1] / 255.0);
		fwrite(&c, 1, 1, f);
		c = 0;
		fwrite(&c, 1, 1, f);
		c = 0;
		fwrite(&c, 1, 1, f);

		// Mongoose 2002.03.02, Normal vertex
		c = (unsigned char)(vertex->norm[0] / 255.0);
		fwrite(&c, 1, 1, f);
		c = (unsigned char)(vertex->norm[1] / 255.0);
		fwrite(&c, 1, 1, f);
		c = (unsigned char)(vertex->norm[2] / 255.0);
		fwrite(&c, 1, 1, f);
		c = 0;
		fwrite(&c, 1, 1, f);
#endif
	}

	for (mTexels.start(); mTexels.forward(); mTexels.next())
	{
		texel = mTexels.current();
    
		if (!texel)
			continue;
    
		u = texel->id;
		fwrite(&u, 4, 1, f);
		fwrite(&texel->st[0], sizeof(unit_t), 1, f);
		fwrite(&texel->st[1], sizeof(unit_t), 1, f);
	} 

	// PolyMesh //////////////////////

	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		polygon = mPolygons.current();

		if (!polygon)
			continue;

		u = polygon->id;
		fwrite(&u, 4, 1, f);
		fwrite(&polygon->shader, sizeof(int), 1, f);

		u = polygon->vertex.size();
		fwrite(&u, 4, 1, f);

		for (polygon->vertex.start(); polygon->vertex.forward(); polygon->vertex.next())
		{
			u = polygon->vertex.current();
			fwrite(&u, 4, 1, f);
		}

		u = polygon->texel.size();
		fwrite(&u, 4, 1, f);

		for (polygon->texel.start(); polygon->texel.forward(); polygon->texel.next())
		{
			u = polygon->texel.current();
			fwrite(&u, 4, 1, f);
		}
	}

	////////////////////////////////////////

	for (mGroups.start(); mGroups.forward(); mGroups.next())
	{
		group = mGroups.current();
    
		if (!group)
			continue;
    
		u = group->id;
		fwrite(&u, 4, 1, f);
    
		u = group->vertex.size();
		fwrite(&u, 4, 1, f);
    
		for (group->vertex.start(); group->vertex.forward(); group->vertex.next())
		{
			u = group->vertex.current();
			fwrite(&u, 4, 1, f);
		}
	}

   for (mMeshes.start(); mMeshes.forward(); mMeshes.next())
   {
		MeshSave(mMeshes.current(), f);
   }

   for (mTags.start(); mTags.forward(); mTags.next())
   {
		saveTag(mTags.current(), f);
   }

   for (mBoneFrames.start(); mBoneFrames.forward(); mBoneFrames.next())
   {
		BoneFrameSave(mBoneFrames.current(), f);
   }

   for (mAnimations.start(); mAnimations.forward(); mAnimations.next())
   {
		AnimationSave(mAnimations.current(), f);
   }

   fclose(f);

   return 0;
}


int Egg::loadFile(char *filename)
{
	int32_t si;
	u_int32_t u, version, id, i, j, n;
	float dummy;
	u_int32_t num_vertex, num_texel, num_poly, num_marker;
	u_int32_t num_grp, numMeshes, numTags, num_frame, num_aframe;
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
		fread(&x, sizeof(unit_t), 1, f);
		fread(&y, sizeof(unit_t), 1, f);
		fread(&z, sizeof(unit_t), 1, f);

		s = t = nx = ny = nz = 0.0;

		switch (version)
		{
		case EGG_VERSION:
			fread(&s, sizeof(unit_t), 1, f);
			fread(&t, sizeof(unit_t), 1, f);

			fread(&nx, sizeof(unit_t), 1, f);
			fread(&ny, sizeof(unit_t), 1, f);
			fread(&nz, sizeof(unit_t), 1, f);
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

#ifdef DEBUG_EGG_LOAD
		printDebug(5, "*** %i %i %i %i\n", 
					  i, id, vertexlist.size(), texellist.size());
#endif

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
		fread(&dummy, sizeof(unit_t), 1, f);
		fread(&dummy, sizeof(unit_t), 1, f);
		fread(&dummy, sizeof(unit_t), 1, f);

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
			printf("EGG::Load> Error loading mesh[%i] resetting model\n", i);

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

		printf("-- Loading Tag #%i of %i\n", i, numTags);

		addTag(tag);
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


	if (!a || !b)
		return;

	for (b->ref.start(); b->ref.forward(); b->ref.next())
	{
		polygon = getPolygon(b->ref.current());

		if (polygon)
		{
			// Must be replace B with A to match sorted list ids
			polygon->texel.Replace(B, A);

			a->ref.pushBack(b->ref.current());
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


	if (!a || !b)
		return;

	for (b->ref.start(); b->ref.forward(); b->ref.next())
	{
		polygon = getPolygon(b->ref.current());

		if (polygon)
		{
			// Must be replace B with A to match sorted list ids
			polygon->vertex.Replace(B, A);
			a->ref.pushBack(b->ref.current());
		}
	}

	// Mongoose: Don't let VertexDel see we refence any polygons
	b->ref.clear();

	// Mongoose 2002.01.19, handle groups here
	// Remove all references to dead vertex from groups
	for (mGroups.start(); mGroups.forward(); mGroups.next())
	{
		grp = mGroups.current();
		
		for (grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
		{
			if (grp->vertex.current() == B)
			{
				grp->vertex.remove(grp->vertex.currentIndex());
			}
		}
	}

	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		if (VertexInPolygon(B, mPolygons.current()))
		{
			(mPolygons.current())->vertex.Replace(B, A);
		}
	}

	mVertices.remove(B);
	delete b;
}


egg_vertex_t *Egg::addVertex(unit_t x, unit_t y, unit_t z)
{
	return addVertex(x, y, z, 0.0, 1.0, 0.0, 0.5, 0.5);
}


egg_vertex_t *Egg::addVertex(unit_t x, unit_t y, unit_t z, 
									  unit_t nx, unit_t ny, unit_t nz,
									  unit_t u, unit_t v)

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
	egg_group_t *grp;


	if (!v)
		return;

	// FIMXE: I don't think you can do this with the list
	//        I think it'll only get the first match and stop
	//        See hack in loop

	// Remove all polygons using this vertex
	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		if (VertexInPolygon(v->id, mPolygons.current()))
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


unsigned int Egg::getPolygonCount()
{
	return mPolygons.size();
}


egg_polygon_t *Egg::getPolygon(unsigned int id)
{
	if (mPolygons.empty())
		return NULL;
  
	return mPolygons[id];
}

/* FIXME: Really need 2 polygon types (classes?) 
 * One with and one without external texels */
unsigned int Egg::addPolygon(Vector<unsigned int> &vertex,
									  Vector<unsigned int> &texel, 
									  int shader)
{
	egg_polygon_t *polygon;
	egg_vertex_t *vert;
	egg_texel_t *tex;
	//int hError = 0;


	if (vertex.empty())
	{
		printError("PolygonAdd> Assertion failed, No vertices\n");
		return UINT_MAX;
	}

	if (texel.empty())
	{
		printError("PolygonAdd> WARNING: No external texels\n");

		//printError("PolygonAdd> Assertion failed, No texels\n");
		//return UINT_MAX;
	}

	polygon = new egg_polygon_t;
	mPolygons.pushBack(polygon);
	polygon->id = mPolygons.size() - 1;
	polygon->shader = shader;

	for (texel.start(); texel.forward(); texel.next())
	{
		tex = getTexel(texel.current());

		if (!tex)
		{
			printf("Invalid texel %u\n", texel.current());
			delete polygon;
			return UINT_MAX;
		}

		tex->ref.pushBack(polygon->id);
		polygon->texel.pushBack(tex->id);
		polygon->r_texel.pushBack(tex);
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
		polygon->r_vertex.pushBack(vert);
	}

#ifdef FIXME
	if (mDebugLevel)
	{
		polygon->vertex.print(__print_unsigned_int);
		polygon->r_vertex.print(__print_egg_vertex_t);
		polygon->texel.print(__print_unsigned_int);
	}
#endif

	return polygon->id;
}


void Egg::delPolygon(unsigned int polygon)
{
	delPolygon(getPolygon(polygon));
}


void Egg::delPolygon(egg_polygon_t *polygon)
{
	egg_mesh_t *mesh;
	egg_texel_t *texel;


	if (!polygon)
		return;

	// Remove all references to polygon from meshes
	for (mMeshes.start(); mMeshes.forward(); mMeshes.next())
	{
		mesh = mMeshes.current();
		mesh->polygon.remove(polygon->id);
	}

	// Remove texels used by polygon
	for (polygon->texel.start(); polygon->texel.forward(); polygon->texel.next())
	{
		texel = getTexel(polygon->texel.current());

		if (!texel)
			continue;
    
		polygon->texel.remove(texel->id);
		texel->ref.remove(polygon->id);

		// Mongoose: account for shared texels!
		if (texel->ref.empty())
			delTexel(texel);
	}

	mPolygons.remove(polygon->id);  
	delete polygon;
}


void Egg::delPolygon(Vector<unsigned int> *list)
{
	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		if (PolygonMatch(list, mPolygons.current()))
		{
			delPolygon(mPolygons.current());
			return;
		}
	}
}


unsigned int Egg::selectPolygon(Vector<unsigned int> *list)
{
	egg_polygon_t *polygon;


	for (mPolygons.start(); mPolygons.forward(); mPolygons.next())
	{
		if (PolygonMatch(list, mPolygons.current()))
		{
			polygon = mPolygons.current();
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
	if (!group)
		return;

	// FIXME: Maybe it would be nice to check and see if it's used
	//        by another group ( but groups may be obsoleted soon )
	for (group->vertex.start(); group->vertex.forward(); group->vertex.next())
	{
		delVertex(group->vertex.current());
	}

   mGroups.remove(group->id);
   delete group;
}


/// Utils /////////////////////////////////////////


void Egg::GroupTransform(unsigned int group, enum egg_transform type, 
								 unit_t x, unit_t y, unit_t z)
{
	egg_group_t *grp = getGroup(group);


	Transform(grp, type, x, y, z);
}


void Egg::GroupMirror(unsigned int group, bool x, bool y, bool z)
{
	egg_group_t *grp;
	egg_vertex_t *v;

  
	grp = getGroup(group);

	if (!grp)
		return;

	for (grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
	{
		v = getVertex(grp->vertex.current());

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

#ifdef OBSOLETE

// FIXME: Make code not scare small children
egg_mesh_t *Egg::MeshSplit(egg_mesh_t *mesh, Vector<egg_vertex_t *> *list)
{
	egg_mesh_t *new_mesh;
	egg_vertex_t *vert;
	egg_vertex_t *vert2;
	egg_group_t *new_frame;
	List <egg_group_t *> *frame;
	List <egg_group_t *> *frame2;
	Vector<egg_vertex_t *> *vertex;
	List <egg_polygon_t *> *polygon;
	List <egg_polygon_t *> *swap = NULL;
	List <egg_polygon_t *> *last = NULL;
	int *trans;
	int n, i, j;
	bool add;
	float *st;


	if (!mesh || !list)
		return NULL;

	vertex = list;
	n = 0;

	while (vertex)
	{
		if (vertex->vertex)
			n++;

		vertex = vertex->next;
	}
  
	trans = new int[n];

	new_mesh = NewMesh();
	new_frame = AddFrame(new_mesh);

	// EXP //////
	for (i = 0; i < mesh->frame_count - 1; i++)
		AddFrame(new_mesh);
	////////////

	vertex = list;
	i = 0;  

	while (vertex)
	{
		if (vertex->vertex)
		{
			trans[i++] = vertex->vertex->id;
			AddVertex(new_mesh, new_frame, vertex->vertex->pos[0], 
						 vertex->vertex->pos[1], vertex->vertex->pos[2]);
		}

		vertex = vertex->next;
	} 

	// Copy vertex pos
	frame = mesh->frames;
	frame2 = new_mesh->frames;

	while (frame && frame2)
	{
		for (i = 0; i < n; i++)
		{
			vert = FindVertex(frame->frame, trans[i]);
			vert2 = FindVertex(frame2->frame, i);

			vert2->pos[0] = vert->pos[0];
			vert2->pos[1] = vert->pos[1];
			vert2->pos[2] = vert->pos[2];
		}
    
		frame = frame->next;
		frame2 = frame2->next;
	}

	polygon = mesh->polys;

	while (polygon)
	{
		add = false;

		if (polygon->poly)
		{
			vertex = NULL;

			for (i = 0; i < polygon->poly->sides; i++)
			{
				add = false;

				for (j = 0; j < n; j++)
				{
					if (trans[j] == polygon->poly->index_list[i])
					{
						add = true;
						break;
					}
				}

				if (!add)
				{
					break;
				}
				else
					VertexListBuild(&vertex, new_frame, j);
			}

			if (add)
			{
				st = new float[2*polygon->poly->sides];
	
				for (i = 0; i < polygon->poly->sides*2; i++)
				{
					st[i] = polygon->poly->st[i];
				}
	
				AddPolygon(new_mesh, vertex, polygon->poly->sides, 
							  polygon->poly->texture, polygon->poly->material, st);

				// This removes polygons
				mesh->polygon_count--;

				swap = polygon;
				polygon = polygon->next;

				if (last == NULL)      
					mesh->polys = swap->next;
				else
					last->next = swap->next;

				if (swap->poly)
				{
					if (swap->poly->list)
						VertexListDelete(&swap->poly->list);
	
					if (swap->poly->st)
						delete [] swap->poly->st;

					if (swap->poly->index_list)
						delete [] swap->poly->index_list;

					delete swap->poly;
				}
     
				delete swap;
			}
			else
				VertexListDelete(&vertex);
		}

		if (!add)
		{
			last = polygon;
			polygon = polygon->next;
		}
	}

	VertexListDelete(&list);

	// Remove vertices we used to split mesh, unless still used 
	// by other polygons
	for (i = 0; i < n; i++)
	{
		if (!VertexUsedByPolygon(mesh, trans[i]))
			DelVertex(mesh, trans[i]);
	}

	return new_mesh;
}
#endif


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
	for (vertices->start(); vertices->forward(); vertices->next())
	{
		vert = getVertex(vertices->current());
		
		if (!vert)
			continue;

		vert2 = addVertex(vert->pos[0], vert->pos[1], vert->pos[2]);

		if (!vert2)
			continue;

		grp->vertex.pushBack(vert2->id); // Add(vert2->id);
		trans.Add(vert->id, vert2->id);
	} 

	// Copy polygons, add to mesh, and use the translation table from above
	for (mesh->polygon.start(); mesh->polygon.forward(); mesh->polygon.next())
	{
		poly = getPolygon(mesh->polygon.current());
		
		if (!poly)
			continue;

		vertex_list.clear();
		texel_list.clear();

		partial = false;

		// Weed out parital polygons, since we may only have some of the vertices
		for (poly->vertex.start(); poly->vertex.forward(); poly->vertex.next())
		{
			if (trans[poly->vertex.current()] == UINT_MAX)
			{
				partial = true;
				break;
			}
			else
			{
				vertex_list.pushBack(trans[poly->vertex.current()]);
			}
		}
		
		if (partial)
			continue;

		for (poly->texel.start(); poly->texel.forward(); poly->texel.next())
		{
			texel = getTexel(poly->texel.current());

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


int Egg::MeshSave(egg_mesh_t *mesh, FILE *f)
{
	u_int32_t u;


	if (!mesh)
		return -1;
  
	// Check point
	u = EGG_MESH_CHUNK_START;
	fwrite(&u, 4, 1, f);

	// Vertex Group indices
	u = mesh->group.size();
	fwrite(&u, 4, 1, f);

	for (mesh->group.start(); mesh->group.forward(); mesh->group.next())
	{
		u = mesh->group.current();
		fwrite(&u, 4, 1, f);
	}

	// Check point
	u = 0xbdbdbdbd;
	fwrite(&u, 4, 1, f);

	// Polygon indices
	u = mesh->polygon.size();
	fwrite(&u, 4, 1, f);

	for (mesh->polygon.start(); mesh->polygon.forward(); mesh->polygon.next())
	{
		u = mesh->polygon.current();
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

unsigned int Egg::getTagCount()
{
	return mTags.size();
}


egg_tag_t *Egg::getTag(unsigned int id)
{
	if (mTags.empty())
		return NULL;

	return mTags[id];
}

//FIXME: Only size is handled, like the rest of this file I/O code
//       it needs ordering code in parts, and etc - but I have no
//       big endian machines at home
int Egg::saveTag(egg_tag_t *tag, FILE *f)
{
	int32_t li;
	u_int32_t lu;  


	if (!tag)
		return -1;

	// Check point
	lu = EGG_BTAG_CHUNK_START;
	fwrite(&lu, 4, 1, f);

	li = tag->id;
	fwrite(&li, 4, 1, f);

	lu = tag->slave.size();
	fwrite(&lu, 4, 1, f);

	for (tag->slave.start(); tag->slave.forward(); tag->slave.next())
	{
		lu = tag->slave.current();
		fwrite(&lu, 4, 1, f);
	}

	lu = tag->mesh.size();
	fwrite(&lu, 4, 1, f);

	for (tag->mesh.start(); tag->mesh.forward(); tag->mesh.next())
	{
		lu = tag->mesh.current();
		fwrite(&lu, 4, 1, f);
	}

	fwrite(&tag->flag, 1, 1, f);

	fwrite(&tag->center[0], sizeof(unit_t), 1, f);
	fwrite(&tag->center[1], sizeof(unit_t), 1, f);
	fwrite(&tag->center[2], sizeof(unit_t), 1, f);

	// tag's 'transform matrix' isn't used yet, so ignore it for now  =)

	fwrite(&tag->rot[0], sizeof(unit_t), 1, f);
	fwrite(&tag->rot[1], sizeof(unit_t), 1, f);
	fwrite(&tag->rot[2], sizeof(unit_t), 1, f);

	// Check point
	lu = EGG_BTAG_CHUNK_END;
	fwrite(&lu, 4, 1, f);

	return 0;
}


egg_tag_t *Egg::loadTag(FILE *f)
{
	egg_tag_t *tag;
	int32_t li;
	u_int32_t lu;  
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

	fread(&tag->center[0], sizeof(unit_t), 1, f);
	fread(&tag->center[1], sizeof(unit_t), 1, f);
	fread(&tag->center[2], sizeof(unit_t), 1, f);

	// tag's 'transform matrix' isn't used yet, so ignore it for now  =)

	fread(&tag->rot[0], sizeof(unit_t), 1, f);
	fread(&tag->rot[1], sizeof(unit_t), 1, f);
	fread(&tag->rot[2], sizeof(unit_t), 1, f);

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


egg_tag_t *Egg::addTag(unit_t x, unit_t y, unit_t z, char flag)
{
	egg_tag_t *tag;
	matrix_t m;


	tag = new egg_tag_t;  
	tag->center[0] = x;
	tag->center[1] = y;
	tag->center[2] = z;
	tag->flag = flag;

	mtkIdentity(m);
	mtkCopy(m, tag->transform);

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


void Egg::TagRotateAbout(unsigned int tag, unit_t rx, unit_t ry, unit_t rz)
{
   egg_tag_t *etag;
   float x, y, z;
   matrix_t transform;
   egg_mesh_t *mesh;
   egg_group_t *grp;
   egg_vertex_t *vert;
   unsigned int count;
   unit_t xr = mtkDegToRad(rx);
   unit_t yr = mtkDegToRad(ry);
   unit_t zr = mtkDegToRad(rz);


   etag = getTag(tag);

   if (!etag)
		return;

   x = etag->center[0];
   y = etag->center[1];
   z = etag->center[2];

   mtkIdentity(transform);
   mtkRotate(transform, xr, yr, zr);

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
			 
				vert->pos[0] -= etag->center[0];
				vert->pos[1] -= etag->center[1];
				vert->pos[2] -= etag->center[2];
			 
				mtkTransform(transform, vert->pos);
			 
				vert->pos[0] += etag->center[0];
				vert->pos[1] += etag->center[1];
				vert->pos[2] += etag->center[2];
			 
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
	
   for (etag->slave.start(); etag->slave.forward(); etag->slave.next())
   {
		TagRotateAboutPoint(etag->slave.current(), etag->center, rx, ry, rz);
   }
}


void Egg::TagRotateAboutPoint(unsigned int tag, point_t p,
										unit_t rx, unit_t ry, unit_t rz)
{
   egg_tag_t *etag;
   matrix_t transform;
   egg_mesh_t *mesh;
   egg_group_t *grp;
   egg_vertex_t *vert;
   unsigned int count;
   unit_t xr = mtkDegToRad(rx);
   unit_t yr = mtkDegToRad(ry);
   unit_t zr = mtkDegToRad(rz);


   etag = getTag(tag);

   if (!etag)
		return;

   mtkIdentity(transform);
   mtkRotate(transform, xr, yr, zr);   

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
			  
				mtkTransform(transform, vert->pos);
			  
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
					resizeBoundingBox(grp, vert->pos);
			}
		}
   }

   etag->center[0] -= p[0];
   etag->center[1] -= p[1];
   etag->center[2] -= p[2];
     
   mtkTransform(transform, etag->center);
     
   etag->center[0] += p[0];
   etag->center[1] += p[1];
   etag->center[2] += p[2];  
     
   for (etag->slave.start(); etag->slave.forward(); etag->slave.next())
   {
		TagRotateAboutPoint(etag->slave.current(), p, rx, ry, rz);
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
	int32_t li;
	u_int32_t lu;  


	if (!boneframe)
		return -1;

	// Check point
	lu = EGG_TFRM_CHUNK_START;
	fwrite(&lu, 4, 1, f);

	li = boneframe->id;
	fwrite(&li, 4, 1, f);

	lu = boneframe->tag.size();
	fwrite(&lu, 4, 1, f);

	for (boneframe->tag.start(); boneframe->tag.forward(); boneframe->tag.next())
	{
		lu = boneframe->tag.current();
		fwrite(&lu, 4, 1, f);
	}

	fwrite(&boneframe->center[0], sizeof(unit_t), 1, f);
	fwrite(&boneframe->center[1], sizeof(unit_t), 1, f);
	fwrite(&boneframe->center[2], sizeof(unit_t), 1, f);

	// Check point
	lu = EGG_TFRM_CHUNK_END;
	fwrite(&lu, 4, 1, f);

	return 0;
}


egg_boneframe_t *Egg::BoneFrameLoad(FILE *f)
{
	egg_boneframe_t *boneframe;
	int32_t li;
	u_int32_t lu;  
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

	fread(&boneframe->center[0], sizeof(unit_t), 1, f);
	fread(&boneframe->center[1], sizeof(unit_t), 1, f);
	fread(&boneframe->center[2], sizeof(unit_t), 1, f);

	// Check point
	fread(&lu, 4, 1, f);

	if (lu != EGG_TFRM_CHUNK_END)
	{
		printError("BoneFrameLoad> end checkpoint [ERROR]\n");
	}

	return boneframe;
}


unsigned int Egg::BoneFrameAdd(unit_t x, unit_t y, unit_t z)
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
	int32_t li;
	u_int32_t lu;  


	if (!a)
		return -1;

	// Check point
	lu = EGG_AFRM_CHUNK_START;
	fwrite(&lu, 4, 1, f);

	li = a->id;
	fwrite(&li, 4, 1, f);

	lu = a->frame.size();
	fwrite(&lu, 4, 1, f);

	for (a->frame.start(); a->frame.forward(); a->frame.next())
	{
		lu = a->frame.current();
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

/////////////////////////////////////////////////////////////////


void Egg::Transform(Vector<egg_vertex_t *> *list, enum egg_transform type,
						  unit_t x, unit_t y, unit_t z)
{
	egg_vertex_t *vert;
	matrix_t transform;


	if (!list)
		return;

	mtkIdentity(transform);

	switch (type)
	{
	case SCALE:
		mtkScale(transform, x, y, z);
		break;
	case ROTATE:
		x = mtkDegToRad(x);
		y = mtkDegToRad(y);
		z = mtkDegToRad(z);
		mtkRotate(transform, x, y, z);
		break;
	case TRANSLATE:
		mtkTranslate(transform, x, y, z);
		break;
	default:
		return;
	}

	for (list->start(); list->forward(); list->next())
	{
		vert = list->current();

		if (!vert)
			continue;

		mtkTransform(transform, vert->pos);
	}
}


void Egg::Transform(egg_tag_t *etag, enum egg_transform type, 
						  unit_t x, unit_t y, unit_t z)
{
	matrix_t transform;


	if (!etag)
		return;

	mtkIdentity(transform);

	switch (type)
	{
	case SCALE:
		mtkScale(transform, x, y, z);
		break;
	case ROTATE:
		x = mtkDegToRad(x);
		y = mtkDegToRad(y);
		z = mtkDegToRad(z);
		mtkRotate(transform, x, y, z);
		break;
	case TRANSLATE:
		mtkTranslate(transform, x, y, z);
		break;
	default:
		return;
	}


	mtkTransform(transform, etag->center);

	//FIXME: transform groups
	printError("Egg::Transform> ( Tag ) Not fully implemented %s:%i\n", 
				  __FILE__, __LINE__);
}


void Egg::Transform(egg_group_t *grp, enum egg_transform type, 
						  unit_t x, unit_t y, unit_t z)
{
	egg_vertex_t *vert;
	matrix_t transform;
	unsigned int count;


	if (!grp)
		return;

	mtkIdentity(transform);

	switch (type)
	{
	case SCALE:
		mtkScale(transform, x, y, z);
		break;
	case ROTATE:
		x = mtkDegToRad(x);
		y = mtkDegToRad(y);
		z = mtkDegToRad(z);
		mtkRotate(transform, x, y, z);
		break;
	case TRANSLATE:
		mtkTranslate(transform, x, y, z);
		break;
	default:
		return;
	}

	mtkTransform(transform, grp->center);
  
	for (count = 0,grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
	{
		vert = getVertex(grp->vertex.current());

		if (!grp)
			continue;

		mtkTransform(transform, vert->pos);

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
						  unit_t x, unit_t y, unit_t z)
{
	matrix_t transform;
	egg_group_t *grp;
	egg_vertex_t *vert;
	int count;


	if (!mesh)
		return;

	mtkIdentity(transform);

	switch (type)
	{
	case SCALE:
		mtkScale(transform, x, y, z);
		break;
	case ROTATE:
		x = mtkDegToRad(x);
		y = mtkDegToRad(y);
		z = mtkDegToRad(z);

		mtkRotate(transform, x, y, z);
		break;
	case ROTATE_ABOUT_CENTER:
		mtkRotate(transform, x, y, z);

		//grp = Group(mesh->group.Current());
		//x = grp->center[0];
		//y = grp->center[1];
		//z = grp->center[2];
		break;
	case TRANSLATE:
		mtkTranslate(transform, x, y, z);
		break;
	default:
		return;
	}

	for (mesh->group.start(); mesh->group.forward(); mesh->group.next())
	{
		grp = getGroup(mesh->group.current());

		if (!grp)
			continue;

		switch (type)
		{
		case ROTATE_ABOUT_CENTER:
			break;
		default:
			mtkTransform(transform, grp->center);
		}
    
		count = 0;

		for (grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
		{
			vert = getVertex(grp->vertex.current());

			if (!grp)
				continue;

			switch (type)
			{
			case ROTATE_ABOUT_CENTER:
				vert->pos[0] -= grp->center[0];
				vert->pos[1] -= grp->center[1];
				vert->pos[2] -= grp->center[2];
	
				mtkTransform(transform, vert->pos);
	
				vert->pos[0] += grp->center[0];
				vert->pos[1] += grp->center[1];
				vert->pos[2] += grp->center[2];
				break;
			default:
				mtkTransform(transform, vert->pos);
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


void Egg::Transform(enum egg_transform type, unit_t x, unit_t y, unit_t z)
{
	matrix_t transform;
	egg_tag_t *tag;
	egg_mesh_t *mesh;
	egg_group_t *grp;
	egg_vertex_t *vert;
	int count;


	mtkIdentity(transform);

	switch (type)
	{
	case SCALE:
		mtkScale(transform, x, y, z);
		break;
	case ROTATE:
		x = mtkDegToRad(x);
		y = mtkDegToRad(y);
		z = mtkDegToRad(z);

		mtkRotate(transform, x, y, z);
		break;
	case TRANSLATE:
		mtkTranslate(transform, x, y, z);
		break;
	default:
		return;
	}

	for (mTags.start(); mTags.forward(); mTags.next())
	{
		tag = mTags.current();

		if (!tag)
			continue;  

		mtkTransform(transform, tag->center);
	}

	for (mMeshes.start(); mMeshes.forward(); mMeshes.next())
	{
		mesh = mMeshes.current();

		if (!mesh)
			continue;

		for (mesh->group.start(); mesh->group.forward(); mesh->group.next())
		{
			grp = getGroup(mesh->group.current());

			if (!grp)
				continue;

			mtkTransform(transform, grp->center);

			count = 0;

			for (grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
			{
				vert = getVertex(grp->vertex.current());

				if (!grp)
					continue;

				mtkTransform(transform, vert->pos);

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


////////////////////////////////////////////////////////////
// Private methods                                        
////////////////////////////////////////////////////////////


bool Egg::PolygonMatch(Vector<unsigned int> *list, egg_polygon_t *polygon)
{
	if (!list || !polygon)
		return false;

	if (list->size() != polygon->vertex.size())
		return false;

	for (list->start(); list->forward(); list->next())
	{
		if (!VertexInPolygon(list->current(), polygon))
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


void Egg::resizeBoundingBox(egg_group_t *grp, point_t p)
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


	if (!grp || grp->vertex.empty())
		return;

	for (grp->vertex.start(); grp->vertex.forward(); grp->vertex.next())
	{
		vertex = mVertices.current();
    
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

void Egg::printDebug(unsigned int level, char *s, ...)
{
	va_list args;


	if (mDebugLevel >= level)
	{
		va_start(args, s);
		fprintf(stdout, "Egg::");
		vfprintf(stdout, s, args);
		fprintf(stdout, "\n");
		va_end(args);
	}
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
