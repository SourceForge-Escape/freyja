/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the TombRaider plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Defines ----------------------------------------------------------
 *           
 * ROBERT_SMITH_TESSELATION_STREET -- Tesselate polygons on load
 *
 * TR_MAP_SUPPORT -- Allow maps to be loaded as model meshes
 *
 * TEST_PONYTAIL_MESH -- Load ponytail with lara model
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2001-05-16:
 * Mongoose - Created
 ==========================================================================*/

#define TR_MAP_SUPPORT
//#define TEST_PONYTAIL_MESH

#include <stdio.h>
#include <sys/types.h>

#include <mstl/List.h>
#include <mstl/Map.h>
#include <mstl/Vector.h>

#include <hel/Mat44.h>

#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>

#include "TombRaider.h"

using namespace mstl;
using namespace hel;


vec_t gTRScaling = 0.05f;

// Export functions
extern "C" {

	int freyja_model__tombraider_check(char *filename);
	int freyja_model__tombraider_import(char *filename);
	int freyja_model__tombraider_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	// FIXME: Should use pak interface not model!

	freyjaPluginName1s("tombraider");
	freyjaPluginDescription1s("Tombraider pak (*.phd,*.tr2,*.tr4)");
	freyjaPluginAddExtention1s("*.phd,*.tr2,*.tr4");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);// | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);//FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
}


int import_model(char *filename)
{
	if (!freyja_model__tombraider_check(filename))
	{
		return freyja_model__tombraider_import(filename);
	}

	return -1;
}

int freyja_model__tombraider_check(char *filename)
{
  FILE *f;
  u_int32_t version;


  f = fopen(filename, "rb");

  if (!f)
  {
    perror("tombraider_check> fopen failed\n");
    return -1;
  }

#ifdef ENDIAN
  FreadSmallU_Int(&version, f);
#else
  fread(&version, 4, 1, f);
#endif

  fclose(f);

  switch (version)
  {
  case 0x00000020:
  case 0x0000002d:
  case 0xff080038:
  case 0xff180038:
  case 0xfffffff0:
  case 0x00345254:
    return 0;
    break;
  default:
    ;
  }

  return -2;
}

/// TombRaider helper functions //////////////////////////

// Mongoose 2002.04.07, Ponytail is object_id 30 and it's a 
//   full moveable not in items list, to access it see my OR source

int load_mesh(TombRaider &tombraider, int index, bool tesselate)
{
	tr2_object_texture_t *object_texture = NULL;
	tr2_mesh_t *meshes = NULL;
   int i, v, triangles, t_index;
	//	float color_value;
	unsigned int mesh;
	unsigned int vertex;
	float rgba[4];
	float st[2];
	
	
	object_texture = tombraider.ObjectTextures();
	meshes = tombraider.Mesh();
	
	// Assert common sense, try to skip sprites and FX placeholders
	if (index >= 0 && meshes[index].num_vertices < 1)
	{
		printf("x");
		fflush(stdout);

		// Need to inculde for mesh compatibilty? (make empty mesh)
		freyjaBegin(FREYJA_MESH);
		mesh = freyjaGetCurrent(FREYJA_MESH);
		//freyjaBegin(FREYJA_GROUP);		
		//freyjaEnd();
		freyjaEnd();

		return mesh;
	}

	//r_mesh = new model_mesh_t;
	//r_mesh->vertex_light = NULL;
	//r_mesh->radius = meshes[index].collision_size;
	
	// Start a new mesh
	freyjaBegin(FREYJA_MESH);
	mesh = freyjaGetCurrent(FREYJA_MESH);

	// Start a new vertex group
	//freyjaBegin(FREYJA_GROUP); 

#ifdef FIXME
   if (meshes[index].mesh_lights && meshes[index].num_vertices != -1) 
   {
      // Vertex lighting
		r_mesh->vertex_light = new color_t[meshes[index].num_vertices];

      for (i = 0; i < meshes[index].num_vertices; i++) 
		{
			color_value = meshes[index].mesh_lights[i];

			switch (tombraider.Engine())
			{
			case TR_VERSION_4:
			case TR_VERSION_3:
				color_value /= 16384.0;
			default:
				color_value = (1.0 - (color_value / 8192.0));
				break;
			}

			r_mesh->vertex_light[i].rgba[0] = color_value;
			r_mesh->vertex_light[i].rgba[1] = color_value;
			r_mesh->vertex_light[i].rgba[2] = color_value;
			r_mesh->vertex_light[i].rgba[2] = 1.0;
      }
	}
#endif

	// FIXME: Should account for TR3+ alpha

   // Textured triangles ////////////////////////
	triangles = meshes[index].num_textured_triangles;

#ifdef TR_TRIS_DEBUG
	printf("textured triangles> %i, %p\n",
			 meshes[index].num_textured_triangles,
			 meshes[index].textured_triangles);
#endif

	for (i = 0; triangles > 0 && i < triangles; ++i)
   {
		t_index = meshes[index].textured_triangles[i].texture;

      // Start a new polygon
      freyjaBegin(FREYJA_POLYGON);

		// Store vertices
		v = meshes[index].textured_triangles[i].vertices[0];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
										  -meshes[index].vertices[v].y*gTRScaling,
										  meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].textured_triangles[i].vertices[1];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
										  -meshes[index].vertices[v].y*gTRScaling,
										  meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].textured_triangles[i].vertices[2];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
										  -meshes[index].vertices[v].y*gTRScaling,
										  meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);


		// Store texels
		tombraider.ComputeUV(object_texture[t_index].vertices, st, st+1);
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

		tombraider.ComputeUV(object_texture[t_index].vertices+1, st, st+1);
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

		tombraider.ComputeUV(object_texture[t_index].vertices+2, st, st+1);
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

		// Set texture index mapping
		freyjaPolygonMaterial1i(object_texture[t_index].tile);

		// set_transparency = object_texture[t_index].transparency_flags;
      
      // End FREYJA_POLYGON
      freyjaEnd();
	}


	// FIXME: Add back tesselation option
	for (i = 0; i < meshes[index].num_textured_rectangles; ++i)
   {
		t_index = meshes[index].textured_rectangles[i].texture;

      // Start a new polygon
      freyjaBegin(FREYJA_POLYGON);

		// Store vertices
		v = meshes[index].textured_rectangles[i].vertices[0];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
										  -meshes[index].vertices[v].y*gTRScaling,
										  meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].textured_rectangles[i].vertices[1];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
										  -meshes[index].vertices[v].y*gTRScaling,
										  meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].textured_rectangles[i].vertices[2];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
										  -meshes[index].vertices[v].y*gTRScaling,
										  meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].textured_rectangles[i].vertices[3];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
										  -meshes[index].vertices[v].y*gTRScaling,
										  meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		
		// Store texels
		tombraider.ComputeUV(object_texture[t_index].vertices, st, st+1);
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

		tombraider.ComputeUV(object_texture[t_index].vertices+1, st, st+1);
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

		tombraider.ComputeUV(object_texture[t_index].vertices+2, st, st+1);
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

		tombraider.ComputeUV(object_texture[t_index].vertices+3, st, st+1);
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

		// Set texture index mapping
		freyjaPolygonMaterial1i(object_texture[t_index].tile);

      // End FREYJA_POLYGON
      freyjaEnd();
	}
    

	// Coloured polygons
	for (i = 0; i < meshes[index].num_coloured_triangles; ++i)
   {
      // Start a new polygon
      freyjaBegin(FREYJA_POLYGON);

		// Store vertices
		v = meshes[index].coloured_triangles[i].vertices[0];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
									 -meshes[index].vertices[v].y*gTRScaling,
									 meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].coloured_triangles[i].vertices[1];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
									 -meshes[index].vertices[v].y*gTRScaling,
									 meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].coloured_triangles[i].vertices[2];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
									 -meshes[index].vertices[v].y*gTRScaling,
									 meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		
		// Store texels
		switch (tombraider.Engine())
		{
		case TR_VERSION_1:
			tombraider.ColorLookUp(meshes[index].coloured_triangles[i].texture
											& 0xff, rgba);
			break;
		default:
			tombraider.ColorLookUp((meshes[index].coloured_triangles[i].texture>>8)
											& 0xff, rgba);
		}

		// Color texel hack
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[0], rgba[1]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[2], rgba[3]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[0], rgba[1]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[2], rgba[3]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[0], rgba[1]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[2], rgba[3]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[0], rgba[1]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[2], rgba[3]));

		// Set texture index mapping
		freyjaPolygonMaterial1i(-1);//COLORED_POLYGON);

      // End FREYJA_POLYGON
      freyjaEnd();
	}


	for (i = 0; i < meshes[index].num_coloured_rectangles; ++i)
   {
      // Start a new polygon
      freyjaBegin(FREYJA_POLYGON);

		// Store vertices
		v = meshes[index].coloured_rectangles[i].vertices[0];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
									 -meshes[index].vertices[v].y*gTRScaling,
									 meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].coloured_rectangles[i].vertices[1];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
									 -meshes[index].vertices[v].y*gTRScaling,
									 meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].coloured_rectangles[i].vertices[2];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
									 -meshes[index].vertices[v].y*gTRScaling,
									 meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		v = meshes[index].coloured_rectangles[i].vertices[3];
		vertex = freyjaVertexCreate3f(meshes[index].vertices[v].x*gTRScaling,
									 -meshes[index].vertices[v].y*gTRScaling,
									 meshes[index].vertices[v].z*gTRScaling);
		freyjaPolygonVertex1i(vertex);

		// Store texels
		switch (tombraider.Engine())
		{
		case TR_VERSION_1:
			tombraider.ColorLookUp(meshes[index].coloured_rectangles[i].texture
											& 0xff, rgba);
			break;
		default:
			tombraider.ColorLookUp((meshes[index].coloured_rectangles[i].texture>>8)
											& 0xff, rgba);
		}

		// Color texel hack
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[0], rgba[1]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[2], rgba[3]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[0], rgba[1]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[2], rgba[3]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[0], rgba[1]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[2], rgba[3]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[0], rgba[1]));
		freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(rgba[2], rgba[3]));

		// Set texture index mapping
		freyjaPolygonMaterial1i(-1);//COLORED_POLYGON);

      // End FREYJA_POLYGON
      freyjaEnd();
	}
	
	// End FREYJA_GROUP
	//freyjaEnd();

	// End FREYJA_MESH
	freyjaEnd();
	
	printf("m");
	fflush(stdout);

	return mesh;
}


int gPonyTailIDs[32];
int gPonyTailCount = 0;
bool gPonyTailCached = false;

// a -> index into animations
// index == object index
// cache2/3 for mesh caching
// frame0 is for tr4 multimesh layering support
int load_animation(TombRaider &tombraider, 
						 int a, int index, int frame_offset, int frame_step, 
						 List<unsigned int> &meshtree_frame0,
						 List<unsigned int> &cache2,
						 List<unsigned int> &cache3)
{
	bool lara = false;
	unsigned short *frame = NULL;
	tr2_moveable_t *moveable = NULL;
	tr2_animation_t *animation = NULL;
	tr2_meshtree_t *meshtree = NULL;
	unsigned int anim;


	frame = tombraider.Frame();
	moveable = tombraider.Moveable();
	meshtree = tombraider.MeshTree();
	//mesh = tombraider.Mesh();
	//object_texture = tombraider.ObjectTextures();
	//item = tombraider.Item();
	animation = tombraider.Animation();	
	//sprite_sequence = tombraider.SpriteSequence();


	if (moveable[index].object_id == 0)
	{
		lara = true;
	}
	else
	{
		lara = false;
	}

	//////////////////////////////////////////////////////////////////////////
	// FIXME: This code needs to be reworked to use modern freyja keyframing,
	//        skeletons, and joints API
	//////////////////////////////////////////////////////////////////////////
#if 0
	//int k, n, , ponytail = -1;
	int l, f, j, frame_count;
	bool layer = false;
	int mesh_id, model_id;
	int *tree;
	unsigned short itmp, itmp2;
	float rot[3];
	double angle;
	tr2_meshtree_t *mesh_tree = NULL;
	unsigned int anim, tag;

	// Start new mesh tree animation
	freyjaBegin(FREYJA_SKEL_ANIM);
	anim = freyjaGetCurrent(FREYJA_SKEL_ANIM);
	freyjaAnim1u(ANIM_MESH_TREE);

	frame_count = (animation[a].frame_end - animation[a].frame_start) + 1;

	// Get all the frames for aniamtion
	for (f = 0; f < frame_count; ++f, frame_offset += frame_step)
	{
#ifdef DEBUG_TR_FRAMES
		printf("\nframe_offset %i\t\t f %i\n",
				 frame_offset,
				 animation[a].frame_start + f);
#endif

		switch (tombraider.Engine())
		{
		case TR_VERSION_3:
			// HACK: Lara's ObjectID is 315, but her meshes start at 0, so make a
			// quick substitution (so she doesn't appear as a bunch of thighs)
			if (index == 0)
			{
				for (j = 0; j < (int)tombraider.NumMoveables() && index == 0; ++j)
				{
					if (moveable[j].object_id == 315)
						index = j;
				}
			}

			layer = false;
			break;
		case TR_VERSION_4:
			// Fix Lara in TR4
			if (index == 0)
			{
				for (j = 0; j < (int)tombraider.NumMoveables() && index == 0; ++j)
				{
					// Body is ItemID 8, joints are ItemID 9
					//  (TR4 demo: body is ItemID 10, joints are ItemID 11)
					if (moveable[j].object_id == 8)   
						index = j;
				}
			}
			else if (moveable[index].object_id == 8)
			{  
				// KLUDGE to do "skinning"	
				index = 0;
	
				for (j = 0; j < (int)tombraider.NumMoveables() && index == 0; ++j)
				{
					// Body is ItemID 8, joints are ItemID 9
					//  (TR4 demo: body is ItemID 10, joints are ItemID 11)
					if (moveable[j].object_id == 9)
						index = j;
				}
			}

			// Only TR4 lara has 2 layer bone tags/meshes per bone frame
			if (lara)
			{
				layer = true;
			}
			else
			{
				layer = false;
			}
			break;
		default:
			layer = false;
		}

		// Generate bone frames and tags per frame ////////////
		freyjaMeshTreeAddFrame3f((short)frame[frame_offset + 6]*gTRScaling,
									 -(short)frame[frame_offset + 7]*gTRScaling,
									 (short)frame[frame_offset + 8]*gTRScaling);

#ifdef DEBUG_TR_FRAMES
		printf("\nf[%i]", frame_offset);
		fflush(stdout);
#endif
		
		l = 9;   // First angle offset in this Frame
		
		// Run through the tags and calculate the rotation and offset
		for (j = 0; j < (int)moveable[index].num_meshes; ++j) 
		{ 
			freyjaBegin(FREYJA_BONE);
			index_t bone = freyjaGetCurrent(FREYJA_BONE);
			freyjaBoneTranslate3f(bone, 0.0f, 0.0f, 0.0f);
			freyjaBoneRotateEuler3f(bone, 0.0f, 0.0f, 0.0f);
			freyjaBoneFlags(bone, 0x0);
			freyjaEnd();

			tag = freyjaGetCurrent(FREYJA_BONE);
			
			// Link to mesh tree frame
			freyjaMeshTreeTag1u(tag);

#ifdef DEBUG_TR_FRAMES
			printf("t");
			fflush(stdout);
#endif

			// We only want one copy of the mesh in the model
			mesh_id = moveable[index].starting_mesh + j;
			
			if (cache2.Empty() || cache2.SearchKey(mesh_id) == UINT_MAX)
			{
				model_id = load_mesh(tombraider, mesh_id, false);

				freyjaIterator(FREYJA_BONE, tag);
				freyjaBoneAddMesh1u(model_id);

				cache2.Add(mesh_id);
				cache3.Add(model_id);

				// Mongoose 2002.01.19, Add this mesh to 'first boneframe' list
				if (f == 0)
				{
					meshtree_frame0.Add(model_id);
#ifdef DEBUG_TR_FRAMES
					printf("L");
					fflush(stdout);
#endif		 
				}
			}
			else // It's already cached, use cached
			{
#ifdef DEBUG_TR_FRAMES
				printf("c");
				fflush(stdout);
#endif

				freyjaIterator(FREYJA_BONE, tag);
				freyjaBoneAddMesh1u(cache3[cache2.SearchKey(mesh_id)]);
			}

	
			if (layer)
			{
				// Mongoose 2002.01.19, FIXME, should weld vertices 
				//   here between layer mesh and two main ones
				//   instead of just pushing the layer mesh on 
				//   mesh tree

				// Mongoose 2002.01.19, Add layer mesh to tag
				freyjaBoneAddMesh1u(meshtree_frame0[j]);
			}

			
			// Setup offsets to produce skeletion
			if (j == 0)
			{
				// Since we use bone's offset, these aren't used
				freyjaIterator(FREYJA_BONE, tag);
				freyjaBonePos3f(0.0, 0.0, 0.0);

				// Always push tag[0], this isn't really used either
				freyjaBoneFlags(0x02);
			}
			else  // Nonprimary tag - position relative to first tag
			{
				// Hack: moveable[index].mesh_tree is a byte offset 
				//       into mesh_tree[], so we have to convert to index
				tree = (int *)meshtree;
				mesh_tree = (tr2_meshtree_t *)&tree[moveable[index].mesh_tree 
																+ ((j - 1) * 4)];
				
				freyjaIterator(FREYJA_BONE, tag);
				freyjaBonePos3f(mesh_tree->x*gTRScaling, -mesh_tree->y*gTRScaling, -mesh_tree->z*gTRScaling);
				freyjaBoneFlags(mesh_tree->flags);
			}
			
			// Setup tag rotations
			itmp = frame[frame_offset + l++];
			
			if (tombraider.Engine() == TR_VERSION_1)
			{
				// All angles are three-axis
				angle = (itmp >> 4) & 0x03ff;
				angle *= 360.0 / 1024.0;
				rot[0] = angle;
				
				itmp2 = (itmp << 6) & 0x03c0;
				
				// Get Z rotation
				itmp = frame[frame_offset + l];
				++l;
				
				itmp2 |= (itmp >> 10) & 0x003f;
				angle = itmp2;
				angle *= 360.0 / 1024.0;
				rot[1] = angle;
				
				angle = itmp & 0x3ff;
				angle *= 360.0 / 1024.0;
				rot[2] = angle;

				freyjaIterator(FREYJA_BONE, tag);
				freyjaBoneRotate3f(-rot[0], rot[1], -rot[2]);
			}
			else if (itmp & 0xc000) 
			{ 
				// TR2, TR3, TR4 - single axis of rotation
				if (tombraider.Engine() == TR_VERSION_4)
				{
					angle = itmp & 0x0fff;
					angle /= 4096.0;
					angle *= 360.0;
				}
				else 
				{
					angle = itmp & 0x3ff;
					angle /= 1024.0;
					angle *= 360.0;
				}
				
				freyjaIterator(FREYJA_BONE, tag);
				freyjaGetBoneRotate3f(&rot[0], &rot[1], &rot[2]);

				switch (itmp & 0xc000)
				{
				case 0x4000:
					rot[0] = angle;
					break;
				case 0x8000:
					rot[1] = angle;
					break;
				case 0xc000:
					rot[2] = angle;
					break;
				}

				freyjaBoneRotate3f(-rot[0], rot[1], -rot[2]);
			}
			else    // TR2, TR3, TR4 - three axes
			{
				angle = (itmp >> 4) & 0x03ff;
				angle *= 360.0 / 1024.0;
				rot[0] = angle;
				
				itmp2 = (itmp << 6) & 0x03c0;
				itmp = frame[frame_offset + l++]; // get Z rotation
				
				itmp2 |= (itmp >> 10) & 0x003f;
				angle = itmp2;
				angle *= 360.0 / 1024.0;
				rot[1] = angle;
				
				angle = itmp & 0x3ff;
				angle *= 360.0 / 1024.0;
				rot[2] = angle;

				freyjaIterator(FREYJA_BONE, tag);
				freyjaBoneRotate3f(-rot[0], rot[1], -rot[2]);
			}
		}

#ifdef TEST_PONYTAIL_MESH
		if (!gPonyTailCached)
		{
			gPonyTailCached = true;
			
			for (k = 0; k < (int)tombraider.NumMoveables(); ++k) 
			{
				// PonyTail object_id is 30
				if (moveable[k].object_id == 30)
					ponytail = k;
			}
			
			gPonyTailCount = moveable[ponytail].num_meshes;
			n = moveable[ponytail].starting_mesh;
			
			for (k = 0; k < gPonyTailCount; ++k)
			{
				gPonyTailIDs[k] = load_mesh(tombraider, n+k, false);
			}
		}
		
		for (k = 0; k < gPonyTailCount; ++k)
		{			
			freyjaBegin(FREYJA_BONE);
			
			switch (k)
			{
			case 0:
				freyjaTagPos3f(0.0, -16.0*gTRScaling, -32.0*gTRScaling);
				freyjaTagRotate3f(60.0, 0.0, 0.0);
				break;
			case 1:
				freyjaTagPos3f(0.0, 0.0, 32.0*gTRScaling);
				//freyjaTagRotate3f(5.0, 0.0, 0.0);
				break;
			default:
				freyjaTagPos3f(0.0, 0.0, 32.0*gTRScaling);
				//freyjaTagRotate3f(1.0, 0.0, 0.0);
			}

			freyjaTagFlags1u(0x02);
			freyjaTagAddMesh1u(gPonyTailIDs[k]);
			freyjaEnd();
			
			// Link to mesh tree frame
			freyjaMeshTreeTag1u(freyjaGetCurrent(FREYJA_BONE));
		}
		
		freyjaIterator(FREYJA_BONE, tag);
#endif

		// End FREYJA_MESHTREE_ANIM_FRAME
		freyjaEnd();
	}

	// End FREYJA_MESHTREE_ANIM
	freyjaEnd();
#endif

	return anim;
}


#ifdef TR_MAP_SUPPORT
int tombraider_map_import(TombRaider *tombraider)
{
  int i, ii, j, r, t, tt, rt, n, g, t_index;
  float st[2];
  tr2_vertex_t *vertex = NULL;
  tr2_object_texture_t *object_texture = NULL;
  tr2_room_t *room;
  Mat44 m;
  vec3_t p;
  Map<unsigned int, unsigned int> trans;
  unsigned int mesh;


  i = ii = j = r = t = tt = rt = n = g = t_index = 0;

  object_texture = tombraider->ObjectTextures();
  room = tombraider->Room();

  for (i = 0; i < tombraider->NumRooms(); i++)
  {
	  m.SetIdentity();
	  m.Translate(room[i].info.x,
					  room[i].info.y_top - room[i].info.y_bottom, 
					  room[i].info.z);

    // Start new mesh
	 freyjaBegin(FREYJA_MESH);
	 mesh = freyjaGetCurrent(FREYJA_MESH);

	 // Clear translation table
    trans.Clear();

	 // Start new vertex group
	 //freyjaBegin(FREYJA_GROUP);

    for (ii = 0; ii < room[i].room_data.num_vertices; ii++)
    {
      vertex = &room[i].room_data.vertices[ii].vertex;

      p[0] = vertex->x;
      p[1] = vertex->y;
      p[2] = vertex->z;

      m.Multiply3fv(p);

      // Alloc vertex and keep a vertex index translation table
      trans.Add(ii, freyjaVertexCreate3f(p[0], -p[1], p[2]));
    }

    // End FREYJA_GROUP
    //freyjaEnd();


    // Textured polygons
    for (t = 0; t < room[i].room_data.num_triangles; t++, tt++)
    {
      // Start a new polygon
      freyjaBegin(FREYJA_POLYGON);

      // Store vertices by true id, using translation table
      freyjaPolygonVertex1i(trans[room[i].room_data.triangles[t].vertices[0]]);
      freyjaPolygonVertex1i(trans[room[i].room_data.triangles[t].vertices[1]]);
      freyjaPolygonVertex1i(trans[room[i].room_data.triangles[t].vertices[2]]);

      t_index = room[i].room_data.triangles[t].texture;

      st[0] = (float)object_texture[t_index].vertices[0].xpixel / 255.0;
      st[1] = (float)object_texture[t_index].vertices[0].ypixel / 255.0;
      freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

      st[0] = (float)object_texture[t_index].vertices[1].xpixel / 255.0;
      st[1] = (float)object_texture[t_index].vertices[1].ypixel / 255.0;
      freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

      st[0] = (float)object_texture[t_index].vertices[2].xpixel / 255.0;
      st[1] = (float)object_texture[t_index].vertices[2].ypixel / 255.0;
      freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

      freyjaPolygonMaterial1i(object_texture[t_index].tile);
      
      // End FREYJA_POLYGON
      freyjaEnd();
    }

    for (r = 0; r < room[i].room_data.num_rectangles; r++, rt++)
    {
#ifdef ROBERT_SMITH_TESSELATION_STREET
	unsigned int t1_1, t1_2, t1_3, t2_1, t2_2, t2_3;

	// Make 2 triangles from one quad!
	t1_1 = 0;
	t1_2 = 1;
	t1_3 = 2;

	t2_1 = 3;
	t2_2 = 0;
	t2_3 = 2;

	
	///////////////////////////////////////////////////////////////////

	// Start a new polygon
	freyjaBegin(FREYJA_POLYGON);

	// Store vertices by true id, using translation table
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[t1_1]]);
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[t1_2]]);
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[t1_3]]);

	t_index = room[i].room_data.rectangles[r].texture;

	// Transform TR xy pixel offsets and xy coors to GL texels
	// 1. All texture tiles are 256x256
	// 2. XY pixels map XY coords to use textiles pieces at a time
	// 3. Counting is standard C array style (thank god) 0 to 255

	// Asumming no mapping needed for tris? ( can't be complex polygons )
	st[0] = (float)object_texture[t_index].vertices[t1_1].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[t1_1].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

	st[0] = (float)object_texture[t_index].vertices[t1_2].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[t1_2].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

	st[0] = (float)object_texture[t_index].vertices[t1_3].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[t1_3].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

	// Set texture index mapping
	freyjaTexture1i(object_texture[t_index].tile);
	
	// End FREYJA_POLYGON
	freyjaEnd();

	///////////////////////////////////////////////////////////////

	// Start a new polygon
	freyjaBegin(FREYJA_POLYGON);

	// Store vertices by true id, using translation table
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[t2_1]]);
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[t2_2]]);
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[t2_3]]);

	t_index = room[i].room_data.rectangles[r].texture;

	// Transform TR xy pixel offsets and xy coors to GL texels
	// 1. All texture tiles are 256x256
	// 2. XY pixels map XY coords to use textiles pieces at a time
	// 3. Counting is standard C array style (thank god) 0 to 255

	// Asumming no mapping needed for tris? ( can't be complex polygons )
	st[0] = (float)object_texture[t_index].vertices[t2_1].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[t2_1].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

	st[0] = (float)object_texture[t_index].vertices[t2_2].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[t2_2].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

	st[0] = (float)object_texture[t_index].vertices[t2_3].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[t2_3].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));

	// Set texture index mapping
	freyjaTexture1i(object_texture[t_index].tile);
	
	// End FREYJA_POLYGON
	freyjaEnd();
#else
	// Start a new polygon
	freyjaBegin(FREYJA_POLYGON);
	
	// Store vertices by true id, using translation table
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[0]]);
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[1]]);
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[2]]);
	freyjaPolygonVertex1i(trans[room[i].room_data.rectangles[r].vertices[3]]);
	
	t_index = room[i].room_data.rectangles[r].texture;
	
	//printf("texture = %i\n", t_index);
	
	// Transform TR xy pixel offsets and xy coors to GL texels
	// 1. All texture tiles are 256x256
	// 2. XY pixels map XY coords to use textiles pieces at a time
	// 3. Counting is standard C array style (thank god) 0 to 255
	
	// FIXME: Account for ordering later
	st[0] = (float)object_texture[t_index].vertices[0].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[0].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));
	
	st[0] = (float)object_texture[t_index].vertices[1].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[1].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));
	
	st[0] = (float)object_texture[t_index].vertices[2].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[2].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));
	
	st[0] = (float)object_texture[t_index].vertices[3].xpixel / 255.0;
	st[1] = (float)object_texture[t_index].vertices[3].ypixel / 255.0;
	freyjaPolygonTexCoord1i(freyjaTexCoordCreate2f(st[0], st[1]));
	
  	// Set texture index mapping
	freyjaPolygonMaterial1i(object_texture[t_index].tile);
	
	// End FREYJA_POLYGON
	freyjaEnd();
#endif
    }

    n += room[i].room_data.num_vertices;

	 // End FREYJA_MESH
	 freyjaEnd();
  }

  printf("%i vertices and %i polygons in freyja\n", n, tt + rt);

  return n;
}
#endif


void freyja_model__tombraider_load_callback(int p)
{
}


int freyja_model__tombraider_import(char *filename)
{
	TombRaider _tombraider;
	tr2_mesh_t *mesh = NULL;
	tr2_moveable_t *moveable = NULL;
	tr2_meshtree_t *meshtree = NULL;
	tr2_item_t *item = NULL;
	tr2_animation_t *animation = NULL;
	tr2_sprite_sequence_t *sprite_sequence = NULL;
	tr2_object_texture_t *object_texture = NULL;
	int i, j, a, index, frame_offset, frame_step, object_id;
	int aloop, frame_cycle;
	float pos[3];
	float yaw;
	unsigned char *image;
	unsigned char *bumpmap;
	unsigned short *frame = NULL;
	bool lara = false;
	List<unsigned int> cache;
	List<unsigned int> cache2;
	List<unsigned int> cache3;
	List<unsigned int> meshtree_frame0;


#ifdef TR_DEBUG_ALL
	_tombraider.Debug(true);
#endif

	if (_tombraider.Load(filename, freyja_model__tombraider_load_callback))
	{
		return -1002;
	}

	frame = _tombraider.Frame();
	moveable = _tombraider.Moveable();
	meshtree = _tombraider.MeshTree();
	mesh = _tombraider.Mesh();
	object_texture = _tombraider.ObjectTextures();
	item = _tombraider.Item();
	animation = _tombraider.Animation();	
	sprite_sequence = _tombraider.SpriteSequence();
	
	for (i = 0; i < _tombraider.NumTextures(); ++i)
   {
		if (!i)
		{
			printf("Processing textures: ");
		}

		_tombraider.Texture(i, &image, &bumpmap);

		freyjaTextureCreateBuffer(image, 4, 256, 256, RGBA_32);

		if (bumpmap)
			delete [] bumpmap;

		if (image)
			delete [] image;

		printf(".");
		fflush(stdout);
	}
	
	printf("\n");

	printf("Processing skeletal models: ");
	
	for (i = 0; i < _tombraider.NumItems(); i++)
	{
	  object_id = item[i].object_id; 
	  
	  // It may not be a moveable, test for sprite
	  if (!(_tombraider.Engine() == TR_VERSION_1 && item[i].intensity1 == -1))
	  {
		  for (j = 0; j < (int)_tombraider.NumSpriteSequences(); ++j) 
		  {
			  if (sprite_sequence[j].object_id == object_id)
				  break;
		  }

		  // It's not a moveable, skip sprite
		  if (j != (int)_tombraider.NumSpriteSequences())
			  continue;
	  }
	  
	  for (j = 0; j < (int)_tombraider.NumMoveables(); ++j)
		  if ((int)moveable[j].object_id == object_id) 
			  break;
	  
	  // It's not a moveable or even a sprite?, skip unknown
	  if (j == (int)_tombraider.NumMoveables())
		  continue;
	  
	  index = j;
	  
	  pos[0] = item[i].x;
	  pos[1] = item[i].y;
	  pos[2] = item[i].z;
	  
	  yaw = ((item[i].angle >> 14) & 0x03);
	  yaw *= 90;

#ifdef OBSOLETE
	  thing = new entity_t;
	  thing->id = ent++;
	  thing->type = 0x00;
	  thing->pos[0] = item[i].x;
	  thing->pos[1] = item[i].y;
	  thing->pos[2] = item[i].z;
	  thing->yaw = yaw;
	  thing->animate = false;
	  thing->aframe = 1;

	  r_model = new skeletal_model_t;
	  r_model->id = moveable[index].object_id;
#endif

	  // Gather more info if this is lara
	  if (moveable[index].object_id == 0)
	  {
		  lara = true;
		  //_camera.Translate(pos[0], pos[1] - 470, pos[2]);
		  //_render.ViewModel(i);
	  }
	  else
	  {
		  lara = false;
	  }

	  // FIXME: For now we just want to edit lara and ponytail
	  if (moveable[index].object_id != 0)
	  {
		  continue;  
	  }

	  // FIXME: Might be better UID for each model, but this seems
	  //        to work well
	  j = object_id;

	  // We only want one copy of the skeletal model
	  if (cache.Empty() || cache.SearchKey(j) == UINT_MAX)
	  {
		  cache.Add(j);
	  }
	  else // It's already cached, skip it
	  {
		  continue;
	  }

	  frame_cycle = 0;

	  // FIXME: Not sure loop invariant is right still
	  //        But this generates 512 unique frames for lara in TR3 now
	  //        so it can't be too far off
	  // Animation
	  a = moveable[index].animation;
	  frame_offset = animation[a].frame_offset / 2;
	  frame_step = animation[a].frame_size;

	  if (a >= (int)_tombraider.NumAnimations())
		  a = _tombraider.NumFrames() - frame_offset;
	  else
		  a = (animation[a].frame_offset / 2) - frame_offset;

	  if (frame_step != 0)  // prevent divide-by-zero errors
	    a /= frame_step;

	  if (a != 0) // prevent divide-by-zero errors
		  frame_offset += frame_step * (frame_cycle % a);

	  if (a < 0)
	  {
		  printf("\n\ttombraider_import> ERROR: No animations\n");
		  continue;
	  }

	  // Clear caching for new model
	  meshtree_frame0.Clear();
	  cache2.Clear();
	  cache3.Clear();

	  aloop = _tombraider.getNumAnimsForMoveable(index);

	  for (; a < aloop; a++,
			 frame_offset = animation[a].frame_offset / 2,
			 frame_step = animation[a].frame_size)
	  {
		  load_animation(_tombraider, 
							  a, index, frame_offset, frame_step,
							  meshtree_frame0, 	
							  cache2,
							  cache3);
	  }

	  printf(".");
	  fflush(stdout);
	}

	printf("\n");
	printf("Loaded ? Mesh tree animations\n");//,
	//freyjaGetCount(FREYJA_SKEL_ANIM));

	printf("Loaded ? Mesh tree animation frames\n");//, 
	//freyjaGetCount(FREYJA_MESHTREE_ANIM_FRAME));

	return 0;
}


int freyja_model__tombraider_export(char *filename)
{
  printf("freyja_model__tombraider_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}
