/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the trmesh plug-in module
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

#include <freyja8/EggPlugin.h>


extern "C" {

  int freyja_model__trmesh_check(char *filename);
  int freyja_model__trmesh_import(char *filename);
  int freyja_model__trmesh_export(char *filename);
}



int freyja_model__trmesh_check(char *filename)
{
  FILE *f;
  unsigned int version;


  f = fopen(filename, "rb");

  if (!f)
  {
    perror("trmesh_check> fopen failed\n");
    return -1;
  }

  fread(&version, 4, 1, f);
  fclose(f);

  // Check for debug mesh exports ( mesh exported from pak )
  if (version == 0x454D5254)
    return 0;

  return -2;
}


int freyja_model__trmesh_import(char *filename)
{
  FILE *f;
  int i, triangles, rectangles;
  unsigned int check;
  float s, t, r, g, b, a;
  char id[8];
  int collision_size;
  short num_vertices, num_textured_triangles, num_textured_rectangles,
        num_coloured_triangles, num_coloured_rectangles;
  short transparency_flags, tile, x, y, z;


  f = fopen(filename, "rb");

  if (!f)
  {
    perror("Failed to read mesh :");
    return -1;
  }

  // Header ////////////////////////////////////
  fread(id, 8, 1, f);
  
  if (strcmp(id, "TRMESH"))
    return -2;

  fread(&num_vertices, 2, 1, f);
  fread(&num_textured_triangles, 2, 1, f);
  fread(&num_textured_rectangles, 2, 1, f);
  fread(&num_coloured_triangles, 2, 1, f);
  fread(&num_coloured_rectangles, 2, 1, f);
  fread(&collision_size, 4, 1, f);

  // Start a new mesh
  eggBegin(FREYJA_MESH);

  // Start a new vertex group
  eggBegin(FREYJA_GROUP);

  // Textured triangles ////////////////////////
  fread(&check, 4, 1, f);

  if (check != 0xcdcdcdcd)
    printf("Bad check point, data may be invalid\n");

  triangles = num_textured_triangles;

  for (i = 0; triangles > 0 && i < triangles; i++)
  {
    // Start a new polygon
    eggBegin(FREYJA_POLYGON);

    // Read texture info
    fread(&tile, 2, 1, f);
    fread(&transparency_flags, 2, 1, f);

    // Read and store vertices directly in TRMESH
    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));


    // Read and store texels directly in TRMESH
    fread(&s, 4, 1, f);
    fread(&t, 4, 1, f);
    eggTexCoord1i(eggTexCoordStore2f(s, t));

    fread(&s, 4, 1, f);
    fread(&t, 4, 1, f);
    eggTexCoord1i(eggTexCoordStore2f(s, t));

    fread(&s, 4, 1, f);
    fread(&t, 4, 1, f);
    eggTexCoord1i(eggTexCoordStore2f(s, t));

    eggTexture1i(tile);
    
    // End FREYJA_POLYGON
    eggEnd();
  }

  fread(&check, 4, 1, f);

  if (check != 0xcdcdcdcd)
    printf("Bad check point, data may be invalid\n");


  // Textured rectangles ////////////////////////
  fread(&check, 4, 1, f);

  if (check != 0xcdcdcdcd)
    printf("Bad check point, data may be invalid\n");

  rectangles = num_textured_rectangles;

  for (i = 0; rectangles > 0 && i < rectangles; i++)
  {
    // Start a new polygon
    eggBegin(FREYJA_POLYGON);

    // Read texture info
    fread(&tile, 2, 1, f);
    fread(&transparency_flags, 2, 1, f);

    // Read and store vertices directly in TRMESH
    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));


    // Read and store texels directly in TRMESH
    fread(&s, 4, 1, f);
    fread(&t, 4, 1, f);
    eggTexCoord1i(eggTexCoordStore2f(s, t));

    fread(&s, 4, 1, f);
    fread(&t, 4, 1, f);
    eggTexCoord1i(eggTexCoordStore2f(s, t));

    fread(&s, 4, 1, f);
    fread(&t, 4, 1, f);
    eggTexCoord1i(eggTexCoordStore2f(s, t));

    fread(&s, 4, 1, f);
    fread(&t, 4, 1, f);
    eggTexCoord1i(eggTexCoordStore2f(s, t));

    eggTexture1i(tile);
    
    // End FREYJA_POLYGON
    eggEnd();
  }

  fread(&check, 4, 1, f);

  if (check != 0xcdcdcdcd)
    printf("Bad check point, data may be invalid\n");


  // Coloured triangles ////////////////////////
  fread(&check, 4, 1, f);

  if (check != 0xcdcdcdcd)
    printf("Bad check point, data may be invalid\n");

  triangles = num_coloured_triangles;

  for (i = 0; triangles > 0 && i < triangles; i++)
  {
    // Start a new polygon
    eggBegin(FREYJA_POLYGON);

    // Read and store vertices directly in TRMESH
    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    // Read color hack
    fread(&r, 4, 1, f);
    fread(&g, 4, 1, f);
    fread(&b, 4, 1, f);
    fread(&a, 4, 1, f);

    eggTexCoord1i(eggTexCoordStore2f(r, g));
    eggTexCoord1i(eggTexCoordStore2f(a, b));
    eggTexCoord1i(eggTexCoordStore2f(r, g));
    eggTexCoord1i(eggTexCoordStore2f(a, b));
    eggTexCoord1i(eggTexCoordStore2f(r, g));
    eggTexCoord1i(eggTexCoordStore2f(a, b));

    eggTexture1i(COLORED_POLYGON);
    
    // End FREYJA_POLYGON
    eggEnd();
  }

  fread(&check, 4, 1, f);

  if (check != 0xcdcdcdcd)
    printf("Bad check point, data may be invalid\n");

  // Coloured rectangles ////////////////////////
  fread(&check, 4, 1, f);

  if (check != 0xcdcdcdcd)
    printf("Bad check point, data may be invalid\n");

  rectangles = num_coloured_rectangles;

  for (i = 0; rectangles > 0 && i < rectangles; i++)
  {
    // Start a new polygon
    eggBegin(FREYJA_POLYGON);

    // Read and store vertices directly in TRMESH
    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    fread(&x, 2, 1, f);
    fread(&y, 2, 1, f);
    fread(&z, 2, 1, f);
    eggVertex1i(eggVertexStore3f(x, y, z));

    // Read color
    fread(&r, 4, 1, f);
    fread(&g, 4, 1, f);
    fread(&b, 4, 1, f);
    fread(&a, 4, 1, f);

    eggTexCoord1i(eggTexCoordStore2f(r, g));
    eggTexCoord1i(eggTexCoordStore2f(a, b));

    eggTexCoord1i(eggTexCoordStore2f(r, g));
    eggTexCoord1i(eggTexCoordStore2f(a, b));

    eggTexCoord1i(eggTexCoordStore2f(r, g));
    eggTexCoord1i(eggTexCoordStore2f(a, b));

    eggTexCoord1i(eggTexCoordStore2f(r, g));
    eggTexCoord1i(eggTexCoordStore2f(a, b));

    eggTexture1i(COLORED_POLYGON);
    
    // End FREYJA_POLYGON
    eggEnd();
  }

  // End FREYJA_GROUP
  eggEnd();

  // End FREYJA_MESH
  eggEnd();

  fread(&check, 4, 1, f);

  if (check != 0xcdcdcdcd)
    printf("Bad check point, data may be invalid\n");

  fclose(f);

  return 0;
}


int freyja_model__trmesh_export(char *filename)
{
  printf("freyja_model__trmesh_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}
