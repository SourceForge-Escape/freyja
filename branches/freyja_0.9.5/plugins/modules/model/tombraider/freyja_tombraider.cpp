/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * Comments: This is the TombRaider plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Defines ----------------------------------------------------------
 *
 * TEST_PONYTAIL_MESH -- Load ponytail with lara model
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2007.06.29:
 * Mongoose - Updated map export to 0.9.5 API from legacy 0.8.x-0.9.3 FSM
 *
 * 2001.05.16:
 * Mongoose - Created
 ==========================================================================*/

//#define TEST_PONYTAIL_MESH

#if 0
#   define DEBUG_TR_FRAMES printf
#else
#   define DEBUG_TR_FRAMES(...)
#endif

#include <stdio.h>
#include <sys/types.h>

#include <mstl/Map.h>
#include <mstl/Vector.h>

#include <hel/Mat44.h>

#include <freyja/ModelABI.h>
#include <freyja/PluginABI.h>
#include <freyja/BoneABI.h>
#include <freyja/CameraABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/TextureABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/MeshABI.h>
#include <freyja/freyja.h>

#include "TombRaider.h"

using namespace mstl;
using namespace hel;

// FIXME: We don't need no stinking gobals in a dl module
vec_t gTRScaling = 0.05f;
int gPonyTailIDs[32];
int gPonyTailCount = 0;
bool gPonyTailCached = false;

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
	freyjaPluginArg1i("import_map", 1);
	freyjaPluginArg1i("moveable", 0);
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
  FILE *f = fopen(filename, "rb");

  if (!f)
  {
    perror("tombraider_check> fopen failed\n");
    return -1;
  }

	u_int32_t version;
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

int load_mesh(TombRaider &tombraider, Map<int, int> &textureIds, 
					int idx, float scale)
{
	tr2_object_texture_t *object_texture = tombraider.ObjectTextures();
	tr2_mesh_t *meshes = tombraider.Mesh();

	// Assert common sense, try to skip sprites and FX placeholders
	if (index >= 0 && meshes[idx].num_vertices < 1)
	{
		printf("x");
		fflush(stdout);

		/* Create a 'stub' mesh for padding. */
		index_t mesh = freyjaMeshCreate();
		return mesh;
	}

	/* Create a new mesh */
	tr2_mesh_t &tr_mesh = meshes[idx];
	index_t mesh = freyjaMeshCreate();

#ifdef FIXME
   if (tr_mesh.mesh_lights && tr_mesh.num_vertices != -1) 
   {
      // Vertex lighting
		r_mesh->vertex_light = new color_t[tr_mesh.num_vertices];

      for (uint32 i = 0; i < tr_mesh.num_vertices; i++) 
		{
			float color_value = tr_mesh.mesh_lights[i];

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

	// FIXME: Should account for TR3+ alpha?

	/* Vertices */
	int vertex_count = tr_mesh.num_textured_triangles;
	vertex_count = (vertex_count < 0) ? 0 : vertex_count;
	for (uint32 i = 0, n = vertex_count; i < n; ++i)
	{	
		Vec3 p(tr_mesh.vertices[i].x, 
				 -tr_mesh.vertices[i].y,
				 tr_mesh.vertices[i].z);
		p *= scale;
		freyjaMeshVertexCreate3fv(mesh, p.mVec);
	}

   // Textured triangles ////////////////////////
	int triangles = tr_mesh.num_textured_triangles;
	triangles = (triangles < 0) ? 0 : triangles;

#ifdef TR_TRIS_DEBUG
	printf("textured triangles> %i, %p\n",
			 tr_mesh.num_textured_triangles,
			 tr_mesh.textured_triangles);
#endif

	for (uint32 i = 0, n = triangles; i < n; ++i)
	{
		// Create a new polygon
		index_t face = freyjaMeshPolygonCreate(mesh);
		int t_index = tr_mesh.textured_triangles[i].texture;
		int mat = textureIds[ object_texture[t_index].tile ];
		freyjaMeshPolygonMaterial(mesh, face, mat);
		freyjaMeshPolygonGroup1u(mesh, face, mesh);

		for (uint32 j = 0; j < 3; ++j)
		{
			int idx = tr_mesh.textured_triangles[i].vertices[j];
			freyjaMeshPolygonAddVertex1i(mesh, face, idx);

#if 1
			float uv[2];
			tombraider.ComputeUV(object_texture[t_index].vertices+j, uv, uv+1);
#else
			float uv[] = {
   	   	(float)object_texture[t_index].vertices[j].xpixel / 255.0f,
   	   	(float)object_texture[t_index].vertices[j].ypixel / 255.0f
			};
#endif

			index_t texcoord = freyjaMeshTexCoordCreate2fv(mesh, uv);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);

			// set_transparency = object_texture[t_index].transparency_flags;
		}
	}

	int rect = tr_mesh.num_textured_rectangles;
	rect = (rect < 0) ? 0 : rect;
	for (uint32 i = 0, n = rect; i < n; ++i)
   {
		// Create a new polygon
		index_t face = freyjaMeshPolygonCreate(mesh);
		int t_index = tr_mesh.textured_rectangles[i].texture;
		int mat = textureIds[ object_texture[t_index].tile ];
		freyjaMeshPolygonMaterial(mesh, face, mat);
		freyjaMeshPolygonGroup1u(mesh, face, mesh);

		for (int j = 0; j < 4; ++j)
		{
			int idx = tr_mesh.textured_rectangles[i].vertices[j];
			freyjaMeshPolygonAddVertex1i(mesh, face, idx);

#if 1
			float uv[2];
			tombraider.ComputeUV(object_texture[t_index].vertices+j, uv, uv+1);
#else
			float uv[] = {
   	   	(float)object_texture[t_index].vertices[j].xpixel / 255.0f,
   	   	(float)object_texture[t_index].vertices[j].ypixel / 255.0f
			};
#endif

			index_t texcoord = freyjaMeshTexCoordCreate2fv(mesh, uv);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);

			// set_transparency = object_texture[t_index].transparency_flags;
		}
	}
    

	// Coloured polygons
	int ctriangles = tr_mesh.num_coloured_triangles;
	ctriangles = (ctriangles < 0) ? 0 : ctriangles;
	for (uint32 i = 0, n = ctriangles; i < n; ++i)
   {
		// Create a new polygon
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, 0); // color
		freyjaMeshPolygonGroup1u(mesh, face, mesh);

		for (uint32 j = 0; j < 3; ++j)
		{
			int idx = tr_mesh.coloured_triangles[i].vertices[j];
			freyjaMeshPolygonAddVertex1i(mesh, face, idx);
		}

  
#if 0 // This is the old, legacy method of storing vertex colors... 
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
#endif
	}

	int crect = tr_mesh.num_coloured_rectangles;
	crect = (crect < 0) ? 0 : crect;
	for (uint32 i = 0, n = crect; i < n; ++i)
   {
		// Create a new polygon
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, 0); // color
		freyjaMeshPolygonGroup1u(mesh, face, mesh);

		for (uint32 j = 0; j < 4; ++j)
		{
			int idx = tr_mesh.coloured_rectangles[i].vertices[j];
			freyjaMeshPolygonAddVertex1i(mesh, face, idx);
		}

#if 0 // This is the old legacy method of storing vertex colors
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
#endif
	}
	
	printf("m");
	fflush(stdout);

	return mesh;
}

// a -> index into animations
// index == object index
// cache2/3 for mesh caching
// frame0 is for tr4 multimesh layering support
int load_animation(TombRaider &tombraider, 
						 int a, int index, int frame_offset, int frame_step, 
						 Vector<unsigned int> &meshtree_frame0,
						 Vector<unsigned int> &cache2,
						 Vector<unsigned int> &cache3)
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
		DEBUG_TR_FRAMES("\nframe_offset %i\t\t f %i\n", frame_offset, animation[a].frame_start + f);

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

		DEBUG_TR_FRAMES("\nf[%i]", frame_offset);
		
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

			DEBUG_TR_FRAMES("t");


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
					DEBUG_TR_FRAMES("L");
		 
				}
			}
			else // It's already cached, use cached
			{
				DEBUG_TR_FRAMES("c");
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


int tombraider_map_import(TombRaider *tombraider, Map<int, int> &textureIds)
{
	const float scale = 0.001f;

	tr2_object_texture_t *object_texture = tombraider->ObjectTextures();
	tr2_room_t *rooms = tombraider->Room();

	for (int i = 0; i < tombraider->NumRooms(); ++i)
	{
		tr2_room_t &room = rooms[i];

		Mat44 m;
		m.SetIdentity();
		m.Translate(room.info.x * scale,
					  (room.info.y_top - room.info.y_bottom) * scale, 
					  room.info.z * scale);
		m.Scale(scale, scale, scale);

		// Start new mesh
		index_t mesh = freyjaMeshCreate();

		{
			vec3_t p;

			for (int ii = 0; ii < room.room_data.num_vertices; ii++)
			{
			   tr2_vertex_t *vertex = &room.room_data.vertices[ii].vertex;

			   p[0] = vertex->x;
			   p[1] = vertex->y;
			   p[2] = vertex->z;

			   m.Multiply3fv(p);

				p[1] = -p[1]; // tombraider is upside down (-Y is up)
				freyjaMeshVertexCreate3fv(mesh, p);
			}
		}


		// Textured triangles
		for (int t = 0; t < room.room_data.num_triangles; t++)
		{
			// Create a new polygon
			index_t face = freyjaMeshPolygonCreate(mesh);
 			int t_index = room.room_data.triangles[t].texture;
			int mat = textureIds[ object_texture[t_index].tile ];
			freyjaMeshPolygonMaterial(mesh, face, mat);
			freyjaMeshPolygonGroup1u(mesh, face, mesh);

			for (int j = 0; j < 3; ++j)
			{
				int idx = room.room_data.triangles[t].vertices[j];
				freyjaMeshPolygonAddVertex1i(mesh, face, idx);

				float st[] = {
	   	   	(float)object_texture[t_index].vertices[j].xpixel / 255.0f,
	   	   	(float)object_texture[t_index].vertices[j].ypixel / 255.0f
				};

				index_t texcoord = freyjaMeshTexCoordCreate2fv(mesh, st);
				freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);
			}
    }

		// Textured quads
    	for (int r = 0; r < room.room_data.num_rectangles; r++)
    	{
#if 0
			// Make 2 triangles from one quad!
			t1_1 = 0;
			t1_2 = 1;
			t1_3 = 2;

			t2_1 = 3;
			t2_2 = 0;
			t2_3 = 2;
#endif
			// Create a new polygon
			index_t face = freyjaMeshPolygonCreate(mesh);
 			int t_index = room.room_data.rectangles[r].texture;
			int mat = textureIds[ object_texture[t_index].tile ];
			freyjaMeshPolygonMaterial(mesh, face, mat);
			freyjaMeshPolygonGroup1u(mesh, face, mesh);

			for (int j = 0; j < 4; ++j)
			{
				int idx = room.room_data.rectangles[r].vertices[j];
				freyjaMeshPolygonAddVertex1i(mesh, face, idx);

				// Transform TR xy pixel offsets and xy coors to GL texels
				// 1. All texture tiles are 256x256
				// 2. XY pixels map XY coords to use textiles pieces at a time
				// 3. Counting is standard C array style (thank god) 0 to 255
				float st[] = {
	   	   	(float)object_texture[t_index].vertices[j].xpixel / 255.0f,
	   	   	(float)object_texture[t_index].vertices[j].ypixel / 255.0f
				};

				index_t texcoord = freyjaMeshTexCoordCreate2fv(mesh, st);
				freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);
			}
		}
	}


	/* Cameras */
	{
		unsigned int count = tombraider->GetCameraCount();
		for (unsigned int i = 0; i < count; ++i)
		{
			tr2_camera_t* camera = tombraider->GetCamera(i);

			if (camera)
			{
				Vec3 v(camera->x, -camera->y, camera->z); // -Y up
#if 0
			/*	I honestly can't remember if these are in world or 
				'room' coordinates.  If they are just use this snippet:
			*/
				tr2_room_t &room = rooms[camera->room];
				Vec3 t(room.info.x * scale,
					    room.info.y_top - room.info.y_bottom) * scale, 
					    room.info.z * scale);
				v += t;
#endif
				index_t cameraIdx = freyjaCameraCreate();
				freyjaCameraPos3f(cameraIdx, v.mX, v.mY, v.mZ);
				freyjaCameraMetadata(cameraIdx, "<tombraider></tombraider>");
			}			
		}
	}

	return 0;
}


void freyja_model__tombraider_load_callback(int p)
{
}


int freyja_model__tombraider_import(char *filename)
{
	TombRaider _tombraider;


#ifdef TR_DEBUG_ALL
	_tombraider.Debug(true);
#endif

	if (_tombraider.Load(filename, freyja_model__tombraider_load_callback))
	{
		return -1002;
	}

	int pluginId = freyjaGetPluginId();

	Map <int, int> textureIds;
	for (int i = 0; i < _tombraider.NumTextures(); ++i)
   {
		if (!i)
		{
			printf("Processing textures: ");
		}

		unsigned char *image = NULL;
		unsigned char *bumpmap = NULL;
		_tombraider.Texture(i, &image, &bumpmap);

		index_t mat = freyjaMaterialCreate();
		index_t tex = freyjaTextureCreateBuffer(image, 4, 256, 256, RGBA_32);
		freyjaMaterialTexture(mat, tex+2);
		freyjaMaterialSetFlag(mat, fFreyjaMaterial_Texture);
		textureIds.Add(i, mat);

		if (bumpmap)
			delete [] bumpmap;

		if (image)
			delete [] image;

		printf(".");
		fflush(stdout);
	}
	
	printf("\n");


	// Import map?
	{
		int import_map = 0;
		freyjaGetPluginArg1i(pluginId, "import_map", &import_map);

		if (import_map)
		{
			tombraider_map_import( &_tombraider, textureIds );
			return 0;
		}
	}

	// FIXME: Remove legacy containers!
	Vector<unsigned int> cache;
	Vector<unsigned int> meshtree_frame0;

	tr2_moveable_t *moveable = _tombraider.Moveable();
	//unsigned short *frame = _tombraider.Frame();
	//tr2_meshtree_t *meshtree = _tombraider.MeshTree();
	//tr2_mesh_t *mesh = _tombraider.Mesh();
	//tr2_object_texture_t *object_texture = _tombraider.ObjectTextures();
	tr2_item_t *item = _tombraider.Item();
	tr2_animation_t *animation = _tombraider.Animation();	
	tr2_sprite_sequence_t *sprite_sequence = _tombraider.SpriteSequence();
	bool lara = false;

	printf("Processing skeletal models: ");
	
	for (int i = 0, j = 0; i < _tombraider.NumItems(); i++)
	{
	  int object_id = item[i].object_id; 
	  
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
	  
	  int idx = j;
	  //float pos[] = { item[i].x, item[i].y, item[i].z };
	  //float yaw = ( ((item[i].angle >> 14) & 0x03) ) * 90.0f;

	  // Gather more info if this is lara
	  if (moveable[idx].object_id == 0)
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
	  if (moveable[idx].object_id != 0)
	  {
		  continue;  
	  }

	  // FIXME: Might be better UID for each model, but this seems
	  //        to work well
	  j = object_id;

	  // We only want one copy of the skeletal model
	  if (cache.empty() || cache.SearchIndex(j) == UINT_MAX)
	  {
		  cache.push_back(j);
	  }
	  else // It's already cached, skip it
	  {
		  continue;
	  }

	  int frame_cycle = 0;

	  // FIXME: Not sure loop invariant is right still
	  //        But this generates 512 unique frames for lara in TR3 now
	  //        so it can't be too far off
	  // Animation
	  int a = moveable[idx].animation;
	  int frame_offset = animation[a].frame_offset / 2;
	  int frame_step = animation[a].frame_size;

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
	  meshtree_frame0.clear();

		Vector<unsigned int> cache2;
		Vector<unsigned int> cache3;

	  int aloop = _tombraider.getNumAnimsForMoveable(idx);

	  for (; a < aloop; a++,
			 frame_offset = animation[a].frame_offset / 2,
			 frame_step = animation[a].frame_size)
	  {
		  load_animation(_tombraider, 
							  a, idx, frame_offset, frame_step,
							  meshtree_frame0, 	
							  cache2,
							  cache3);
	  }

	  cache2.clear();
	  cache3.clear();

	  printf(".");
	  fflush(stdout);
	}

	printf("\n");

	return 0;
}


int freyja_model__tombraider_export(char *filename)
{
  printf("freyja_model__tombraider_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}
