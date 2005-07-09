/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Lwo
 * License : GPL
 * Comments: NOD module for freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2001.07.04:
 * Mongoose - Created, based on viewer code from anonymous
 ==========================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Nod.h"


Nod::Nod()
{
	_buffer = NULL;
	_header1 = NULL;
	_materials = NULL;
	_header2 = NULL;
	_bones = NULL;
	_meshes = NULL;
	_vertices = NULL;
	_mesh_groups = NULL;
	_faces = NULL;
}


Nod::~Nod()
{
	_header1 = NULL;
	_materials = NULL;
	_header2 = NULL;
	_bones = NULL;
	_meshes = NULL;
	_vertices = NULL;
	_mesh_groups = NULL;
	_faces = NULL;
	
	if (_buffer)
		delete[] _buffer;
}


void Nod::GetEulerAngles2(const float m[3][4], float angles[3])
{
	float cy;

	// order of angles x,y,z

	cy = (float) sqrt(m[0][0] * m[0][0] + m[0][1] * m[0][1]); // AX^2 + BX^2

	if (cy > 16 * 0.001)
	{
		angles[0] = (float) atan2(m[1][2], m[2][2]); // CY, CZ
		angles[1] = (float) atan2(-m[0][2], cy); // -CX, cy
		angles[2] = (float) atan2(m[0][1], m[0][0]); // BX, AX
	}
	else
	{
		angles[0] = (float) atan2(-m[2][1], m[1][1]); //-BZ, BY
		angles[1] = (float) atan2(-m[0][2], cy);	// -CX, cy
		angles[2] = 0;
	}
}



int Nod::Load(char *filename)
{
	//color_t ambient, diffuse;
	//int i, j, nMaterial, nBone, nMesh, nVertex, nTriangle;
	//int nVertexBase = 0;
	//int nFaceBase = 0;
	//char szTexture[256];
	//char szName[32];
	//vec3_t angles = { -90.0f, 0.0f, 0.0f };
	//float m[3][4];
	int sz;
	FILE *f;


	f = fopen(filename, "rb");
	
	if (!f)
		return -1;
	
	fseek(f, 0, SEEK_END);
	sz = (int)ftell(f);
	fseek(f, 0, SEEK_SET);
	
	_buffer = new byte[sz];
	
	if (!_buffer)
	{
		fclose (f);
		return -2;
	}
	
	fread(_buffer, sz, 1, f);
	fclose (f);
	
	// Setup pointers
	_header1 = (nod_header1_t *)_buffer;
	
	_materials = (nod_material_t *)((byte *)_header1 + sizeof(nod_header1_t));
	
	_header2 = (nod_header2_t *)((byte *)_materials + _header1->NumMaterials * 
										  sizeof(nod_material_t));
	
	_bones = (nod_bone_t *)((byte *)_header2 + sizeof(nod_header2_t));
	
	_meshes = (nod_mesh_t *)((byte *)_bones + _header2->NumBones * 
									 sizeof(nod_bone_t));	
	
	_vertices = (nod_vertex_t *)((byte *)_meshes + _header2->NumMeshs * 
										  sizeof(nod_mesh_t));
	
	_mesh_groups = (nod_meshgroup_t *)((byte *)_header1 + sz - 
												  _header2->NumGroups*sizeof(nod_meshgroup_t));
	
	_faces = (nod_face_t *) ((byte *)_mesh_groups - _header2->NumFaces * 
									 sizeof(nod_face_t));
	
	_vertices = (nod_vertex_t *) ((byte *)_faces - _header2->NumVertices * 
											sizeof(nod_vertex_t));
	
	if (_header2->ModelFlags & NOD_MF_HASLOD)
	{
		_vertices = (nod_vertex_t *)((byte *)_vertices - _header2->NumVertices * 
											  sizeof(word));
	}
  
#ifdef OBSOLETE
  // materials /////////////////////////

  for (i = 0; i < header1->NumMaterials; i++)
  {
    nMaterial = msModel_AddMaterial (pModel);
    msMaterial *pMaterial = msModel_GetMaterialAt (pModel, i);
    msMaterial_SetName (pMaterial, materials[i].MaterialName);
    sprintf (szTexture, "%s.tga", materials[i].MaterialName);
    msMaterial_SetDiffuseTexture (pMaterial, szTexture);
    ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
    msMaterial_SetAmbient (pMaterial, ambient);
    diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    msMaterial_SetDiffuse (pMaterial, diffuse);
  }

  // bones /////////////////////////////

  AngleMatrix (angles, m);

  if (nOptionFlags & eBones)
  {
    for (i = 0; i < header2->NumBones; i++)
    {
      nBone = msModel_AddBone (pModel);
      msBone *pBone = msModel_GetBoneAt (pModel, nBone);
      sprintf (szName, "Bone%02d", i + 1);
      msBone_SetName (pBone, szName);
      msVec3 Rot = { 0.0f, 0.0f, 0.0f };
      msVec3 Pos;


      GUtil_GetEulerAngles2 (bones[i].RestMatrixInverse, Rot);

      if (bones[i].ParentID != -1)
      {
			Pos[0] = bones[i].RestTranslate[0];
			Pos[1] = bones[i].RestTranslate[1];
			Pos[2] = bones[i].RestTranslate[2];
			sprintf (szName, "Bone%02d", bones[i].ParentID);
      }
      else
      {
			Pos[0] = bones[i].RestTranslate[0];
			Pos[1] = bones[i].RestTranslate[1];
			Pos[2] = bones[i].RestTranslate[2];
			//Rot[0] += 90.0f;
			strcpy (szName, "");
      }

      msBone_SetParentName (pBone, szName);
      msBone_SetPosition (pBone, Pos);
      msBone_SetRotation (pBone, Rot);
    }
  }

  // meshes ///////////////////////////////

  for (i = 0; i < header2->NumGroups; i++)
  {
    nMesh = msModel_AddMesh (pModel);
    msMesh *pMesh = msModel_GetMeshAt (pModel, nMesh);
    msMesh_SetName (pMesh, meshes[mesh_groups[i].MeshNum].MeshName);
    msMesh_SetMaterialIndex (pMesh, mesh_groups[i].MaterialID);

    for (j = 0; j < mesh_groups[i].NumVertices; j++)
    {
      nVertex = msMesh_AddVertex (pMesh);
      msVertex *pVertex = msMesh_GetVertexAt (pMesh, nVertex);
      pVertex->Vertex[0] = pVertices[nVertexBase + j].Pos[0];
      pVertex->Vertex[2] = pVertices[nVertexBase + j].Pos[1];
      pVertex->Vertex[1] = pVertices[nVertexBase + j].Pos[2];
      pVertex->u = pVertices[nVertexBase + j].UV[0];
      pVertex->v = 1.0f - pVertices[nVertexBase + j].UV[1];

      if (nOptionFlags & eBones)
			pVertex->nBoneIndex = pVertices[nVertexBase + j].BoneNum;
      else
			pVertex->nBoneIndex = -1;
    }

    for (j = 0; j < mesh_groups[i].NumFaces; j++)
    {
      nTriangle = msMesh_AddTriangle (pMesh);
      msTriangle *pTriangle = msMesh_GetTriangleAt (pMesh, nTriangle);
      word nIndices[3];
      nIndices[0] = faces[nFaceBase + j].indices[0];
      nIndices[2] = faces[nFaceBase + j].indices[1];
      nIndices[1] = faces[nFaceBase + j].indices[2];
      msTriangle_SetVertexIndices (pTriangle, nIndices);
    }

    nVertexBase += mesh_groups[i].NumVertices;
    nFaceBase += mesh_groups[i].NumFaces;
  }
#endif

  return 0;
}


nod_header1_t *Nod::Header1()
{
	return _header1;
}

nod_header2_t *Nod::Header2()
{
	return _header2;
}

nod_mesh_t *Nod::Meshes()
{
	return _meshes;
}

nod_material_t *Nod::Materials()
{
	return _materials;
}

nod_face_t *Nod::Faces()
{
	return _faces;
}

nod_vertex_t *Nod::Vertices()
{
	return _vertices;
}

nod_meshgroup_t *Nod::MeshGroups()
{
	return _mesh_groups;
}

nod_bone_t *Nod::Bones()
{
	return _bones;
}


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_MODEL_PLUGINS

#include <stdio.h>
#include <string.h>
#include <freyja8/EggPlugin.h>
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

#endif


#ifdef UNIT_TEST_NOD
int main(int argc, char *argv[])
{
	int i, j, num_verts, num_faces;
	Nod nod;
	nod_header1_t *header1;
	nod_material_t *materials;
	nod_header2_t *header2;
	nod_bone_t *bones;
	nod_mesh_t *meshes;
	nod_vertex_t *vertices;
	nod_meshgroup_t *mesh_groups;
	nod_face_t *faces;
	

	printf("[NOD class test]\n");
	
	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
		  if (!nod.Load(argv[2]))
		  {
			  printf("main: Nod::Load(%s) reports success.\n", argv[2]);
			  
			  header1 = nod.Header1();
			  materials = nod.Materials();
			  header2 = nod.Header2();
			  bones = nod.Bones();
			  meshes = nod.Meshes();
			  vertices = nod.Vertices();
			  mesh_groups = nod.MeshGroups();
			  faces = nod.Faces();
			  
			  printf("\n\n%s\n", argv[1]);
			  
			  printf("version = %i\n", header1->Version);
			  printf("num_materials = %i\n", header1->NumMaterials);
			  printf("num_bones = %i\n", header2->NumBones);
			  printf("num_meshes = %i\n", header2->NumMeshs);
			  printf("num_vertices = %i\n", header2->NumVertices);
			  printf("num_faces = %i\n", header2->NumFaces);
			  printf("num_groups = %i\n", header2->NumGroups);
			  printf("flags = { ");
			  
			  if (!header2->ModelFlags)
				  printf("NONE ");
			  
			  if (header2->ModelFlags & NOD_MF_HASLOD)
			  {
				  printf("NOD_MF_HASLOD ");
			  }
			  
			  if (header2->ModelFlags & NOD_MF_INLINE)
			  {
				  printf("NOD_MF_INLINE ");
			  }
			  
			  if (header2->ModelFlags & NOD_MF_STATIC)
			  {
				  printf("NOD_MF_STATIC ");
			  }
			  
			  if (header2->ModelFlags & NOD_MF_RESERVED1)
			  {
				  printf("NOD_MF_RESERVED1 ");
			  }
			  
			  if (header2->ModelFlags & NOD_MF_RESERVED1)
			  {
				  printf("NOD_MF_RESERVED1 ");
			  }
			  
			  printf("}\n");
			  
			  printf("bbox = (%.3f, %.3f, %.3f)\n       (%.3f, %.3f, %.3f)\n",
						header2->Bounds[0], header2->Bounds[1], header2->Bounds[2],
						header2->Bounds[3], header2->Bounds[4], header2->Bounds[5]);
		  
			  for (i = 0; i < header1->NumMaterials; i++)
			  {
				  printf("materials[%i].name = '%s'\n", 
							i, materials[i].MaterialName);
			  }

			  for (i = 0; i < header2->NumBones; i++)
			  {
				  printf("bones[%i].RestTranslate = <%.3f %.3f %.3f>\n", 
							i, 
							bones[i].RestTranslate[0],
							bones[i].RestTranslate[1],
							bones[i].RestTranslate[2]);

			  }
			  
			  num_verts = num_faces = 0;
			  
			  for (i = 0; i < header2->NumGroups; i++)
			  {  
				  printf("group[%02i].mesh = '%s'\n",
							i, meshes[mesh_groups[i].MeshNum].MeshName);

				  printf("group[%02i].flags = {", i);

				  if (!mesh_groups[i].GroupFlags)
					  printf("NONE ");

				  if (mesh_groups[i].GroupFlags & NOD_GF_HASLOD)
					  printf("NOD_GF_HASLOD ");

				  if (mesh_groups[i].GroupFlags & NOD_GF_NOWEIGHTS)
					  printf("NOD_GF_NOWEIGHTS ");

				  if (mesh_groups[i].GroupFlags & NOD_GF_NOSKINNING)
					  printf("NOD_GF_NOSKINNING ");

				  if (mesh_groups[i].GroupFlags & NOD_GF_MULTITEXTURE)
					  printf("NOD_GF_MULTITEXTURE ");

				  printf("}\n");

				  for (j = 0; j < mesh_groups[i].NumVertices; j++)
				  {
					  printf("group[%02i].vertex[%04i] { pos %.3f %.3f %.3f; \tuv %.3f %.3f }\n",
								i, j, 
								vertices[num_verts + j].Pos[0],
								vertices[num_verts + j].Pos[1],
								vertices[num_verts + j].Pos[2],
								vertices[num_verts + j].UV[0],
								1.0 - vertices[num_verts + j].UV[1]);
				  }

				  for (j = 0; j < mesh_groups[i].NumFaces; j++)
				  {
					  printf("group[%02i].tris[%04i] { %u, %u, %u }\n",
								i, j, 
								faces[num_faces + j].indices[0],
								faces[num_faces + j].indices[1],
								faces[num_faces + j].indices[2]);
				  }

				  num_verts += mesh_groups[i].NumVertices;
				  num_faces += mesh_groups[i].NumFaces;
			  }
		  }
	  }
	  else
		  printf("\n\n%s [ load ] filename.nod\n", argv[0]);
  }
  else
  {
	  printf("\n\n%s [ load ] filename.nod\n", argv[0]);
  }
}
#endif
