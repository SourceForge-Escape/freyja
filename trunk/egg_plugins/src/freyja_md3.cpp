/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Md3 plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2001-05-09:
 * Mongoose - Created
 ==========================================================================*/

#include <string.h>
#include <stdio.h>
#include <freyja_model/EggPlugin.h>
#include <hel/Matrix.h>

#include "Md3.h"



extern "C" {

	int freyja_model__md3_check(char *filename);
	int freyja_model__md3_import(char *filename);
	int freyja_model__md3_export(char *filename);
}



int freyja_model__md3_check(char *filename)
{
	FILE *f;
   char header[32];


   f = fopen(filename, "rb");

   if (!f)
   { 
	   //sys_errlist[errno]
	   perror("mdl_check> fopen failed");
	   return -1;
   }

   fread(&header, 32, 1, f);      
   fclose(f);

   header[4] = 0;

   // Quake3
   if (strcmp(header, "IDP3") == 0)
		return 0;

   return -2;
}


int freyja_model__md3_import(char *filename)
{
	const float scale = 0.01;
	unsigned int i, m;
	int v, p, f, num_texels;
	md3_mesh_t *md3_mesh;
	md3_bone_t *md3_bone;
	md3_tag_t *md3_tag;
	float s, t;
	//Matrix matrix;
	float pos[3];
	Md3 md3;
	Map<unsigned int, unsigned int> trans;
	unsigned int vertex;


	if (md3.load(filename))
		return -1;

	md3_mesh = md3.getMeshes();
	md3_bone = md3.getBones();
	md3_tag = md3.getTags(); 

	for (m = 0; m < md3.getNumMeshes(); ++m)
	{
		// Start a new mesh
		eggBegin(FREYJA_MESH);
		eggMeshFlags1u(FL_MESH__VERTEX_FRAME_GROUPS);

		for (f = 0, v = 0; f < 1/*md3_mesh[m].num_frames*/; ++f)
		{  
			eggPrintMessage("Importing mesh: %d, frame: %d of %d\n", 
							m, f, md3_mesh[m].num_frames);

			//matrix.setIdentity();
			
			/***************
			mt[0][0] = tag[f].rotation[0][0];
			mt[0][1] = tag[f].rotation[0][1];
			mt[0][2] = tag[f].rotation[0][2];
			
			mt[1][0] = tag[f].rotation[1][0];
			mt[1][1] = tag[f].rotation[1][1];
			mt[1][2] = tag[f].rotation[1][2];
			
			mt[2][0] = tag[f].rotation[2][0];
			mt[2][1] = tag[f].rotation[2][1];
			mt[2][2] = tag[f].rotation[2][2];
			***************/

			// Start a new vertex group
			eggBegin(FREYJA_GROUP);

			for (; v < md3_mesh[m].num_vertices * (f+1); ++v)
			{
				pos[0] = md3_mesh[m].vertex[v].pos[0] * scale;
				pos[1] = md3_mesh[m].vertex[v].pos[1] * scale;
				pos[2] = md3_mesh[m].vertex[v].pos[2] * scale;

				//matrix.multiply3v(pos, pos);

				// Store vertices in group
				vertex = eggVertexStore3f(pos[1], pos[2], pos[0]); // XZY
      
				// Generates id translator  table for this vertex morph frame
				trans.Add(v, vertex);

				eggVertexNormalStore3f(vertex, 
									   md3_mesh[m].vertex[v].norm[1], 
									   md3_mesh[m].vertex[v].norm[2], 
									   md3_mesh[m].vertex[v].norm[0]);
			}

			eggGroupCenter3f(md3_bone[f].center[0]*scale, 
							 md3_bone[f].center[1]*scale, 
							 md3_bone[f].center[2]*scale);

			// End FREYJA_GROUP
			eggEnd();

			// FIXME: Handle Tags
		}

		num_texels = md3_mesh[m].num_vertices * md3_mesh[m].num_frames;

		for (p = 0; p < md3_mesh[m].num_triangles; p++)
		{
			// Start a new polygon
			eggBegin(FREYJA_POLYGON);

			for (i = 0; i < 3; i++)
			{
				// Store vertices by true id, using translation table
				eggVertex1i(trans[md3_mesh[m].tris[p].triangle[i]]);
	
				if (md3_mesh[m].tris[p].triangle[i] > num_texels)
				{
					printf("FreyjaModel::Md3Import> triangle[%i] texel overflow\n", p);
					s = 0.5;
					t = 0.5;
				}
				else
				{
					s = md3_mesh[m].texel[md3_mesh[m].tris[p].triangle[i]].st[0];
					t = md3_mesh[m].texel[md3_mesh[m].tris[p].triangle[i]].st[1];
				}

				eggTexCoord1i(eggTexCoordStore2f(s, t));
			}

#ifdef FIXME
			// FIXME: Handle texture/shader loading
			eggShaderStore(md3_mesh[m].skin[s].name, ...);
#endif

			eggTexture1i(0);
      
			// End FREYJA_POLYGON
			eggEnd(); 
		}

		// End FREYJA_MESH
		eggEnd();

		// Clear vertex index translation table
		trans.Clear();
	}

	return 0;
}


int freyja_model__md3_export(char *filename)
{
#ifdef EXPERIMENTAL_FIXME
	int num_bone_frames, num_tags, num_meshes, num_frames;
	int k, v, t, f;
	md3_mesh_t *mesh;
	md3_boneframe_t *bone;
	md3_tag_t *tag;
	Md3 md3;


	num_meshes = eggGetNum(FREYJA_MESH);

	if (!num_meshes)
	{
		return -1;
	}

	// Don't allow use of internal iterators or
	// changes of data by other processes
	eggCriticalSection(WRITE_LOCK);

	// Meshes ////////////////////////

	mesh = new md3_mesh_t[num_meshes];
	memset(mesh, 0, sizeof(md3_mesh_t) * num_meshes);

	md3.Mesh(mesh);
	md3.NumMeshes(num_meshes);

	// Using list interface, as opposed to array
	eggIterator(FREYJA_MESH, FREYJA_LIST_RESET);

	for (k = 0; k < num_meshes; k++)
	{
		transM.Add(eggIterator(FREYJA_MESH, FREYJA_LIST_CURRENT), k);

		strcpy(mesh[k].name, "freyja-test");
		mesh[k].num_frames = 0; 
		mesh[k].num_vertices = 0;
		mesh[k].num_skins = 0;
		mesh[k].num_triangles = eggMeshIterator(FREYJA_POLYGON, FREYJA_LIST_SIZE);
    
		num_frames = eggMeshIterator(FREYJA_MESH_FRAME, FREYJA_LIST_SIZE);
		eggMeshIterator(FREYJA_MESH_FRAME, FREYJA_LIST_RESET);

		for (f = 0, v = 0; f < num_frames; f++)
		{
			eggMeshIterator(FREYJA_MESH_FRAME, f);
       
			if (eggMeshIterator(FREYJA_MESH_FRAME, FREYJA_LIST_CURRENT_EXIST))
			{
				mesh[k].num_frames++;
				mesh[k].num_vertices += eframe->vertex_count; 
			}
		}
    
		mesh[k].vertex = new md3_vertex_t[mesh[k].num_vertices];
    
		for (f = 0, v = 0; f < mesh[k].num_frames; f++)
		{
			eframe = _egg->FindFrame(emesh, f);
      
			for (; v < mesh[k].num_vertices*(f+1); v++)
			{
				if (eframe)
				{
					vert = _egg->FindVertex(eframe, v);

					if (vert)
					{
						// FIXME: may need bounds ckecking for short
						mesh[k].vertex[v].pos[0] = (short)vert->pos[0];
						mesh[k].vertex[v].pos[1] = (short)vert->pos[1];
						mesh[k].vertex[v].pos[2] = (short)vert->pos[2];

						// FIXME: env texels need real values
						mesh[k].vertex[v].st[0] = 128;  
						mesh[k].vertex[v].st[1] = 128;  
					}
				}
			}
		}

		mesh[k].texel = new md3_texel_t[mesh[k].num_vertices*mesh[k].num_frames];
		mesh[k].tris = new md3_tri_index_t[mesh[k].num_triangles];

		poly = emesh->polys;

		for (t = 0; t < mesh[k].num_triangles; t++)
		{
			mesh[k].tris[t].triangle[0] = poly->poly->index_list[0];
			mesh[k].tris[t].triangle[1] = poly->poly->index_list[1];
			mesh[k].tris[t].triangle[2] = poly->poly->index_list[2];

			mesh[k].texel[mesh[k].tris[t].triangle[0]].st[0] = poly->poly->st[0];
			mesh[k].texel[mesh[k].tris[t].triangle[0]].st[1] = poly->poly->st[1];
			mesh[k].texel[mesh[k].tris[t].triangle[1]].st[0] = poly->poly->st[2];
			mesh[k].texel[mesh[k].tris[t].triangle[1]].st[1] = poly->poly->st[3];
			mesh[k].texel[mesh[k].tris[t].triangle[2]].st[0] = poly->poly->st[4];
			mesh[k].texel[mesh[k].tris[t].triangle[2]].st[1] = poly->poly->st[5];

			poly = poly->next;
		}

		//FIXME: add skin info here
		mesh[k].skin = NULL;
   }

   // Bone frames ////////////////////////

   num_bone_frames = 0;

   if (num_bone_frames)
   {
		bone = new md3_boneframe_t[num_bone_frames];
		memset(bone, 0, sizeof(md3_boneframe_t) * num_bone_frames);

		md3.Bone(bone);

		//FIXME: Add converted bone frames from egg to bone
   }
   else
		md3.Bone(NULL);

   md3.NumBones(num_bone_frames);

   // Bone tags aka Bolt-ons ////////////////////////

   num_tags = 0;

   if (num_tags)
   {
		tag = new md3_tag_t[num_tags * num_bone_frames];
		memset(tag, 0, sizeof(md3_tag_t) * num_tags * num_bone_frames);

		md3.Tag(tag);

		//FIXME: Add converted bone tags from egg to bone
   }
   else
	{
		md3.Tag(NULL);
	}

   md3.NumTags(num_tags);

   eggCriticalSection(WRITE_UNLOCK);

   return md3.Save(filename);
#else
	printf("md3_export> Not enabled in this build\n");
	return -1;
#endif
}
