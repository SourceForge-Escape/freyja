/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Nod plug-in module
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

#include <stdio.h>
#include <string.h>
#include <freyja_model/EggPlugin.h>
#include <hel/Matrix.h>
#include "Nod.h"


extern "C" {

  int freyja_model__nod_check(char *filename);
  int freyja_model__nod_import(char *filename);
  int freyja_model__nod_export(char *filename);
}

#define NO_BONES  // Try to transform vertices for boneless import
#define TRANS     // Translator mapping enabled ( to load full models )

int freyja_model__nod_check(char *filename)
{
  FILE *f;
  int id;


  f = fopen(filename, "rb");
  
  if (!f)
  {
    perror("mod_check> fopen failed");
    return -1;
  }

  fread(&id, 4, 1, f);      
  fclose(f);
  
  if (id == 7)
    return 0;

  return -2;
}


int freyja_model__nod_import(char *filename)
{
  Map<unsigned int, unsigned int> trans;
  int i, j, v, t, num_verts, num_faces;
  Nod nod;
  nod_header1_t *header1;
  nod_material_t *materials;
  nod_header2_t *header2;
  nod_bone_t *bones;
  nod_mesh_t *meshes;
  nod_vertex_t *vertices;
  nod_meshgroup_t *mesh_groups;
  nod_face_t *faces;
#ifdef NO_BONES
  Matrix matrix;
  vec3_t pos, rot;
  int b;
#endif

  if (nod.Load(filename))
    return -1;
				  
  header1 = nod.Header1();
  materials = nod.Materials();
  header2 = nod.Header2();
  bones = nod.Bones();
  meshes = nod.Meshes();
  vertices = nod.Vertices();
  mesh_groups = nod.MeshGroups();
  faces = nod.Faces();
  

  for (i = 0; i < header1->NumMaterials; i++)
  {
    printf("materials[%i].name = '%s'\n", i, materials[i].MaterialName);
  }
  
  num_verts = num_faces = 0;

  for (i = 0; i < header2->NumGroups; i++)
  {
    // Start a new mesh
    eggBegin(FREYJA_MESH);
    
    // Start a new vertex group
    eggBegin(FREYJA_GROUP);

    for (j = 0; j < mesh_groups[i].NumVertices; j++)
    {
#ifdef NO_BONES
      pos[0] = vertices[num_verts + j].Pos[0];
      pos[1] = vertices[num_verts + j].Pos[1];
      pos[2] = vertices[num_verts + j].Pos[2];

      b = vertices[num_verts + j].BoneNum;

      if (b > 0 && b < header2->NumBones)// && 
	  // vertices[num_verts + j].Weight < 1.0)
      {
	printf("nod_import> *** bone[%i] of %i\n", b, header2->NumBones);
	nod.GetEulerAngles2(bones[b].RestMatrixInverse, rot);

	printf("nod_import>   trans <%f %f %f>\n",
	       bones[b].RestTranslate[0],
	       bones[b].RestTranslate[1],
	       bones[b].RestTranslate[2]);

	printf("nod_import>   rotate <%f %f %f>\n",
	       rot[0], rot[1], rot[2]);

	matrix.setIdentity();
	matrix.translate(bones[b].RestTranslate[0],
			 bones[b].RestTranslate[1],
			 bones[b].RestTranslate[2]);
	matrix.rotate(rot[0], rot[1], rot[2]);
	matrix.multiply3v(pos, pos);
      }
      else
	printf("nod_import> xxx bone[%i]...\n", b);

      // Store vertices and texels in group
      v = eggVertexStore3f(pos[0], pos[1], pos[2]);
#else
      // b = eggBoneStore(rot, center, weight, mstr, slv_list);
      // weight_idx = 

      // Store vertices
      v = eggVertexStoreS(vertices[num_verts + j].Pos, 
			  vertices[num_verts + j].Norm,
			  vertices[num_verts + j].UV,
			  weight_idx);
#endif

      t = eggTexCoordStore2f(vertices[num_verts + j].UV[0], 
			  1.0 - vertices[num_verts + j].UV[1]);

      // Generates id translator list 
      // (In NOD texel and vertex are in the same referenced structure,
      //  so only one mapping needed )
      trans.Add(j, v);
    }

    eggEnd(); // FREYJA_GROUP

    num_verts += j;  //mesh_groups[i].NumVertices;

    for (j = 0; j < mesh_groups[i].NumFaces; j++)
    {
      // Start a new polygon
      eggBegin(FREYJA_POLYGON);

      printf("nod_import> tris[%i] = orig <%i %i %i >\ttrans <%i %i %i >\n", j,
	     faces[num_faces + j].indices[0],
	     faces[num_faces + j].indices[1],
	     faces[num_faces + j].indices[2],
	     trans[faces[num_faces + j].indices[0]],
	     trans[faces[num_faces + j].indices[1]],
	     trans[faces[num_faces + j].indices[2]]);

#ifdef TRANS
      eggVertex1i(trans[faces[num_faces + j].indices[0]]);
      eggTexCoord1i(trans[faces[num_faces + j].indices[0]]);
      eggVertex1i(trans[faces[num_faces + j].indices[1]]);
      eggTexCoord1i(trans[faces[num_faces + j].indices[1]]);
      eggVertex1i(trans[faces[num_faces + j].indices[2]]);
      eggTexCoord1i(trans[faces[num_faces + j].indices[2]]);
#else
      eggVertex1i(faces[num_faces + j].indices[0]);
      eggTexCoord1i(faces[num_faces + j].indices[0]);
      eggVertex1i(faces[num_faces + j].indices[1]);
      eggTexCoord1i(faces[num_faces + j].indices[1]);
      eggVertex1i(faces[num_faces + j].indices[2]);
      eggTexCoord1i(faces[num_faces + j].indices[2]);
#endif

      eggTexture1i(0);
      //eggTextureString("");

      eggEnd(); // FREYJA_POLYGON
    }
    
    eggEnd(); // FREYJA_MESH

    num_faces += j; //mesh_groups[i].NumFaces;
    trans.Clear();
  }

  return 0;
}


int freyja_model__nod_export(char *filename)
{
  printf("freyja_model__nod_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}
