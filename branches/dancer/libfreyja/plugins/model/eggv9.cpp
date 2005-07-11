/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the EggV9 plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2001.05.09:
 * Mongoose - Created
 ==========================================================================*/

#include <string.h>
#include <stdio.h>

#include <mstl/Vector.h>
#include <freyja/PluginABI.h>
#include <freyja/FileReader.h>
#include <freyja/FileWriter.h>


extern "C" {

  int freyja_model__eggv9_check(char *filename);
  int freyja_model__eggv9_import(char *filename);
  int freyja_model__eggv9_export(char *filename);
  int import_model(char *filename);
}


int import_model(char *filename)
{
  if (!freyja_model__eggv9_check(filename))
    return freyja_model__eggv9_import(filename);

  return -1;
}

int freyja_model__eggv9_check(char *filename)
{
	FILE *f;
	long magic, version;

	return -1;  // FIXME

	f = fopen(filename, "rb");

	if (!f)
	{
		perror("mdl_check> fopen failed");
		return -1;
	}

	// Header ////////////////////////////
	fread(&magic, 4, 1, f);

	if (magic != EGG_FILE)
	{
		freyjaPrintError("Load> invalid header 0x%x", magic);
		return -2;
	}

	fread(&version, 4, 1, f);

	if (version != EGG_VERSION && 
		version != EGG_8_10 &&
		version != EGG_8_09)
	{
		freyjaPrintError("Load> invalid or unsupported version 0x%x", version);
		return -3;
	}

	fclose(f);

	return 0;
}


egg_mesh_t *MeshLoad(FILE *f)
{
	egg_mesh_t *mesh;
	unsigned int u, i, n;


	mesh = new egg_mesh_t;

	// Check point
	fread(&u, 4, 1, f);

	if (u != EGG_MESH_CHUNK_START)
	{
		freyjaPrintError("MeshLoad> start checkpoint [ERROR]\n");
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
		freyjaPrintError("MeshLoad> internal checkpoint [ERROR]\n");
	}

	// Polygon indices
	fread(&n, 4, 1, f);

	for (i = 0; i < n; ++i)
	{
		fread(&u, 4, 1, f);
		mesh->polygon.pushBack(u);
	}

	// Check point
	fread(&u, 4, 1, f);
  
	if (u != EGG_MESH_CHUNK_END)
	{
		freyjaPrintError("MeshLoad> end checkpoint [ERROR]\n");
	}

	return mesh;
}


int freyja_model__eggv8_import(char *filename)
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
		freyjaPrintError("Load> invalid header 0x%x", id);
		return -2;
	}

	fread(&version, 4, 1, f);

	if (version != EGG_VERSION && 
		version != EGG_8_10 &&
		version != EGG_8_09)
	{
		freyjaPrintError("Load> invalid or unsupported version 0x%x", version);
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


int freyja_model__eggv8_export(char *filename)
{
	FILE *f;
	u_int32_t u, version, id;
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


