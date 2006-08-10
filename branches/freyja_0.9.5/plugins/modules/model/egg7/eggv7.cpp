/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the EggV7 plug-in module
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

#include <mstl/List.h>

#include <freyja/FreyjaPlugin.h>
#include <freyja/BoneABI.h>

#include <mstl/Map.h>

using namespace mstl;

extern "C" {

  int freyja_model__eggv7_check(char *filename);
  int freyja_model__eggv7_import(char *filename);
  int freyja_model__eggv7_export(char *filename);
  int import_model(char *filename);
  void freyja_init();
}

void freyja_init()
{
	freyjaPluginDescription1s("GooseEgg 7 Model (*.egg)");
	freyjaPluginAddExtention1s("*.egg");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | 
			     FREYJA_PLUGIN_SKELETON |
			     FREYJA_PLUGIN_VERTEX_MORPHING);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH |
			     FREYJA_PLUGIN_SKELETON |
			     FREYJA_PLUGIN_VERTEX_MORPHING);
}

int import_model(char *filename)
{
  if (!freyja_model__eggv7_check(filename))
    return freyja_model__eggv7_import(filename);

  return -1;
}


int freyja_model__eggv7_check(char *filename)
{
  FILE *f;
  char header[32];


  f = fopen(filename, "rb");

  if (!f)
  {
    perror("mdl_check> fopen failed");
    return -1;
  }

  fread(&header, 32, 1, f);      
  fclose(f);

  header[9] = 0;

  if (strcmp(header, "Egg v7B") == 0)
    return 0;

  return -2;
}


int freyja_model__eggv7_import(char *filename)
{
  FILE *f;
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


  f = fopen(filename, "rb");

  if (!f)
  {
    perror("Egg::LoadV7> ");
    return -1;
  }

  fread(&header, 8, 1, f);

  if (!(strcmp(header, "Egg v7B") == 0))
  {
    printf("Load> ERROR %s isn't Egg v7B.\n", filename);
    fclose(f);
    return -2;
  }
  
  fread(&mesh_count, sizeof(int), 1, f);    
  fread(&free_poly_count, sizeof(int), 1, f);    
  fread(&bone_count, sizeof(int), 1, f);
  fread(&animation_count, sizeof(int), 1, f);

  freyjaBegin(FREYJA_MODEL);

  for (m = 0; m < mesh_count; m++)
  {
    fread(&header, 8, 1, f);

    if (!(strcmp(header, "Egg m7D") == 0))
    {
      printf("LoadV7> Mesh isn't a valid 'Egg m7D' mesh\n");
      return -3;
    }

    freyjaBegin(FREYJA_MESH);
    transM.Add(m, freyjaGetCurrent(FREYJA_MESH));

    fread(&frame_count, sizeof(int), 1, f);    
    fread(&marker_count, sizeof(int), 1, f);
    fread(&polygon_count, sizeof(int), 1, f);

    printf("LoadV7> %i frames, %i markers\n", frame_count, marker_count);

    vt = 0;

    for (i = 0; i < frame_count; i++)
    {
      fread(&center[0], sizeof(vec_t), 1, f);
      fread(&center[1], sizeof(vec_t), 1, f);
      fread(&center[2], sizeof(vec_t), 1, f);

      fread(&tmp, sizeof(int), 1, f);
      fread(&vertex_count, sizeof(int), 1, f);
    
      printf("LoadV7> group[%i] { %i vertices }\n", i, vertex_count);

      // Start a new vertex group
      freyjaBegin(FREYJA_VERTEX_GROUP);
      freyjaGroupCenter3f(center[0], center[1], center[2]);

      // ALL frames (groups) have same number of vertices in V7
      for (ii = 0; ii < vertex_count; ii++)
      {
	fread(&pos[0], sizeof(vec_t), 1, f);
	fread(&pos[1], sizeof(vec_t), 1, f);
	fread(&pos[2], sizeof(vec_t), 1, f);
	fread(&id, sizeof(int), 1, f);
	
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
      
       fread(&size, sizeof(int), 1, f);
       fread(&texture, sizeof(int), 1, f);
       fread(&material, sizeof(int), 1, f);

       bad_poly = false;
       
       for (j = 0; j < size; j++)
       {
	 fread(&id, sizeof(int), 1, f);
	 fread(&st[0], sizeof(float), 1, f);
	 fread(&st[1], sizeof(float), 1, f);

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
       fread(&id, sizeof(int), 1, f);
       fread(&type, sizeof(unsigned char), 1, f);
       fread(&pos[0], sizeof(vec_t), 1, f);
       fread(&pos[1], sizeof(vec_t), 1, f);
       fread(&pos[2], sizeof(vec_t), 1, f);
       
       // FIXME: No support for markers, but never used as far as I rememeber
       printf("eggv7_import> Eggv7 Metadata not supported in EggPlugin API\n");
     }

     // End FREYJA_MESH
     freyjaEnd();
   }

   for (j = 0; j < bone_count; j++)
   {
     fread(&id, sizeof(int), 1, f);

     // Note: No longer allow plugin to control backend
     //       and the backend supports more than one model,
     //       so swaping m and mesh_count works around that
     //       by seeing how many meshes where loaded from above
     fread(&mesh_count, sizeof(int), 1, f);

     if (mesh_count > m)
       return -5;

     fread(&slaves, sizeof(int), 1, f);
     fread(&pos[0], sizeof(vec_t), 1, f);
     fread(&pos[1], sizeof(vec_t), 1, f);
     fread(&pos[2], sizeof(vec_t), 1, f);

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
       fread(&id, sizeof(int), 1, f);
       printf(" %i", id);

       freyjaBoneAddMesh(index, transM[id]);
     }

     printf("\n %i slaves:\n   ", slaves);

     for (i = 0; i < slaves; i++)
     {
       fread(&id, sizeof(int), 1, f);
       printf(" %i", id);

       freyjaBoneAddChild(index, transT[id]);
     }

     printf("\n}\n");

     // End FREYJA_BONE_TAG
     freyjaEnd();
   }

   freyjaEnd(); // FREYJA_MODEL

   fclose(f);
   return 0;
}


int freyja_model__eggv7_export(char *filename)
{
  printf("freyja_model__eggv7_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);

  return -1;
}
