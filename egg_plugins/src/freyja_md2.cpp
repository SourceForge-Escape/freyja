/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Md2 plug-in module
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
#include "Md2.h"



extern "C" {

  int freyja_model__md2_check(char *filename);
  int freyja_model__md2_import(char *filename);
  int freyja_model__md2_export(char *filename);
}



int freyja_model__md2_check(char *filename)
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

   header[4] = 0;

   // Quake2
   if (strcmp(header, "IDP2") == 0)
     return 0;

   return -2;
}


int freyja_model__md2_import(char *filename)
{
  MD2_t *md2_header;
  VertexIndex_t *md2_vertex;
  int f, v, p;
  short ss, tt;
  float s, t;
  Md2 md2;
  Map<unsigned int, unsigned int> trans;
  unsigned int vertex;


  if (md2.Load(filename))
    return -1;

  md2_header = md2.Header();

#ifdef FIXME
  //FIXME: No string based texture support

  if (md2.SkinName(0))
  {
    if (mtkTextureLoad(md2->SkinName(0)))
      printf("ERROR Loading indexed image referenced in MD2 model\n");
  }
#endif


  // Start a new mesh
  eggBegin(FREYJA_MESH);

  for (f = 0; f < md2_header->numFrames; f++)
  {
    md2_vertex = md2.Frame(f);
    
    // Start a new vertex group
    eggBegin(FREYJA_GROUP); 
    
    // Clear vertex index translation table
    trans.Clear();
    
    for (v = 0; v < md2_header->numXYZ; v++)
    {
      // Store vertices in group
      vertex = eggVertexStore3f(md2_vertex[v].x, 
				md2_vertex[v].z, 
				md2_vertex[v].y);  // Swap Y and Z for freyja
      
      // Generates id translator  table for this vertex morph frame
      trans.Add(v, vertex);
    }
    
    // End FREYJA_GROUP
    eggEnd();
    
    // Md2 uses symetric vertex morph frames, so only need it once
    for (p = 0; f == 0 && p < md2_header->numTris; p++)
    {
      // Start a new polygon
      eggBegin(FREYJA_POLYGON);
      
      // Store vertices by true id, using translation table
      eggVertex1i(trans[md2_header->tris[p].v[0]]);
      eggVertex1i(trans[md2_header->tris[p].v[1]]);
      eggVertex1i(trans[md2_header->tris[p].v[2]]);
      
       // Store texels into model and polygon ( store both at once in MD2 )
      md2.TexCoords(&ss, &tt, p, 0);
      s = (float)ss / (float)md2_header->skinWidth;
      t = (float)tt / (float)md2_header->skinHeight;
      eggTexCoord1i(eggTexCoordStore2f(s, t));
      
      md2.TexCoords(&ss, &tt, p, 1);
      s = (float)ss / (float)md2_header->skinWidth;
      t = (float)tt / (float)md2_header->skinHeight;
      eggTexCoord1i(eggTexCoordStore2f(s, t));
      
      md2.TexCoords(&ss, &tt, p, 2);
      s = (float)ss / (float)md2_header->skinWidth;
      t = (float)tt / (float)md2_header->skinHeight;
      eggTexCoord1i(eggTexCoordStore2f(s, t));
      
      eggTexture1i(0);
      
      // End FREYJA_POLYGON
      eggEnd(); 
    }
  }
  
  // End FREYJA_MESH
  eggEnd();
  
  return 0;
}


int freyja_model__md2_export(char *filename)
{
  printf("freyja_model__md2_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}
