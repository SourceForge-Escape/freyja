/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the 3DS plug-in module
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

#include <freyja_model/EggPlugin.h>
#include <stdio.h>

extern "C" {
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <lib3ds/vector.h>
}

/* Export as C functions */
extern "C" {

  int freyja_model__3ds_check(char *filename);
  int freyja_model__3ds_import(char *filename);
  int freyja_model__3ds_export(char *filename);
}

int freyja_model__3ds_check(char *filename)
{
  char header[8];
  FILE *f;


  f = fopen(filename, "rb");
  
  if (!f)
    return -1;

  fread(&header, 8, 1, f);
  fclose(f);

  if (header[0] == 'M' && header[1] == 'M')
    return 0;
  else
    return -3;
}


int freyja_model__3ds_import(char *filename)
{
  Map<unsigned int, unsigned int> trans;
  Map<unsigned int, unsigned int> trans2;
  Lib3dsMesh *m;
  Lib3dsVector pos;
  Lib3dsFile *f = NULL;
  int points = 0;
  int faces = 0;
  unsigned int i, t, v;


  f = lib3ds_file_load(filename);

  if (!f)
  {
    fprintf(stderr, "freyja_model__3ds_import> Failed to load file\n");
    return -1;
  }
  
  lib3ds_file_eval(f, 0);

  for (m = f->meshes; m; m = m->next) 
  {
    // Start a new mesh
    eggBegin(FREYJA_MESH);

    // Start a new vertex group
    eggBegin(FREYJA_GROUP);  

    // Clear for each group
    trans.Clear();
        
    for (i = 0; i < m->points; i++) 
    {
      lib3ds_vector_transform(pos, m->matrix, m->pointL[i].pos);
 
      // Store vertices in group
      v = eggVertexStore3f(pos[0], pos[1], pos[2]);

      // Generates id translator list
      trans.Add(i, v);
    }

    if (m->texels)
    {
      for (i = 0; i < m->texels; i++) 
      {
	t = eggTexelStore2f(m->texelL[i][0], m->texelL[i][1]);
	trans2.Add(i, t);
      }
    }
    else
    {
      for (i = 0; i < m->points; i++) 
      {
	t = eggTexelStore2f(0.5, 0.5);
	trans2.Add(i, t);
      }
    }

    eggEnd(); // group

    for (i = 0; i < m->faces; i++) 
    {
      // Start a new polygon
      eggBegin(FREYJA_POLYGON);

      // Store vertices by true id, using translator list
      eggVertex1i(trans[m->faceL[i].points[0]]);
      eggVertex1i(trans[m->faceL[i].points[1]]);
      eggVertex1i(trans[m->faceL[i].points[2]]);

      // Store texels by true id, using translator list
      eggTexel1i(trans2[m->faceL[i].points[0]]);
      eggTexel1i(trans2[m->faceL[i].points[1]]);
      eggTexel1i(trans2[m->faceL[i].points[2]]);

      eggTexture1i(0);

      eggEnd(); // polygon
    }

    eggEnd(); // mesh

    points += m->points;
    faces += m->faces;
  }

  lib3ds_file_free(f);
  return 0;
}


int freyja_model__3ds_export(char *filename)
{
  printf("freyja_model__3ds_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);

  return -1;
}

