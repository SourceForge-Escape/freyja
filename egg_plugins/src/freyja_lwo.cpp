/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Lwo plug-in module
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
#include "Lwo.h"
#include <stdio.h>


extern "C" {

  int freyja_model__lwo_check(char *filename);
  int freyja_model__lwo_import(char *filename);
  int freyja_model__lwo_export(char *filename);
}



int freyja_model__lwo_check(char *filename)
{
  Lwo lwo;
  
  return lwo.Check(filename);
}


int freyja_model__lwo_import(char *filename)
{
  Map<unsigned int, unsigned int> trans;
  lw_face_t *face;
  lw_material_t *material;
  Lwo lwo;
  float vert[3];
  int i, j, v, t;


  if (lwo.Load(filename))
    return -1;

  // Start a new mesh
  eggBegin(FREYJA_MESH);

  // Start a new vertex group
  eggBegin(FREYJA_GROUP);

  for (i = 0; i < lwo.VertexCount(); i++)
  {
    lwo.Vertex(i, vert);

    // Store vertices in group
    v = eggVertexStore3f(vert[0], vert[1], vert[2]);

#ifdef DEBUG_LWO_VERTS
    printf("%i = %f %f %f\n", v, vert[0], vert[1], vert[2]);
#endif

    // Generates id translator list
    trans.Add(i, v);
  }

  eggEnd(); // FREYJA_GROUP

  for (i = 0; i < lwo.FaceCount(); i++)
  {
    face = lwo.Face(i);

    if (!face)
      continue;

    // Ignore non polygons
    if (face->index_cnt < 3)
      continue;

    // Start a new polygon
    eggBegin(FREYJA_POLYGON);
    
    for (j = 0; j < face->index_cnt; j++)
    {
      // Store vertices by true id, using translator list
      eggVertex1i(trans[face->index[j]]);

#ifdef DEBUG_LWO_TRANS
      printf("trans[%i] = %i\n", face->index[j], trans[face->index[j]]);
#endif

      material = lwo.Material(face->material);

      if (material)
      {
	// Generate color by texel hack, store it's index
	t = eggTexelStore2f(material->r, material->g);
	eggTexel1i(t);
	
	t = eggTexelStore2f(material->b, material->a);
	eggTexel1i(t);
      }
      else
      {
	t = eggTexelStore2f(0.0, 0.0);
	eggTexel1i(t);
	
	t = eggTexelStore2f(0.0, 0.0);
	eggTexel1i(t);	
      }
    }

    eggTexture1i(COLORED_POLYGON);

    eggEnd(); // FREYJA_POLYGON
  }

  eggEnd(); // FREYJA_MESH
  
  return 0;
}


int freyja_model__lwo_export(char *filename)
{
  printf("freyja_model__lwo_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}
