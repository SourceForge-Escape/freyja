/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the Mdl plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2001-05-16:
 * Mongoose - Created
 ==========================================================================*/

#include <string.h>
#include <stdio.h>
#include <freyja_model/EggPlugin.h>
#include "Mdl.h"


extern "C" {

  int freyja_model__mdl_check(char *filename);
  int freyja_model__mdl_import(char *filename);
  int freyja_model__mdl_export(char *filename);
}



int freyja_model__mdl_check(char *filename)
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

   // Quake, Hexen2
   if (strcmp(header, "IDPO") == 0)
     return 0;

   return -2;
}
 

int freyja_model__mdl_import(char *filename)
{
  float x, y, z, s, t;
  int i, f, v, num_frames;
  MDL_t *header;
  dtriangle_t *tris;
  MDL_Frame_t *frame;
  stvert_t *st_vert;
  Mdl mdl;
  Map<unsigned int, unsigned int> trans;
  unsigned int vertex;
  unsigned char *image;


  if (mdl.Load(filename))
    return -1;



/* Mongoose 2004.04.22, 
 * Store both a Quake and Hexen2 texture for now */
  image = mdl.getTextureRGB24(Mdl::Quake, 0);

  if (image)
  {
    eggTextureStoreBuffer(image, 3,
			  mdl.getTextureWidth(), mdl.getTextureHeight(), 
			  EGG_COLORMODE_RGB);
  }
  
  image = mdl.getTextureRGB24(Mdl::Hexen2, 0);

  if (image)
  {
    eggTextureStoreBuffer(image, 3,
			  mdl.getTextureWidth(), mdl.getTextureHeight(), 
			  EGG_COLORMODE_RGB);
  }

  header = mdl.Header();
  frame = mdl.Frame();
  tris = mdl.Triangle();
  st_vert = mdl.Vertex();

  num_frames = header->numframes;

  // Start a new mesh
  eggBegin(FREYJA_MESH);

  for (f = 0; f < num_frames; f++)
  {
    // Start a new vertex group
    eggBegin(FREYJA_GROUP);  

    // Clear for each group
    trans.Clear();

    for (v = 0; v < header->numverts; v++)
    {
      x = frame[f].sframe.tv[v].v[0] * header->scale[0]+header->scale_origin[0];
      y = frame[f].sframe.tv[v].v[1] * header->scale[1]+header->scale_origin[1];
      z = frame[f].sframe.tv[v].v[2] * header->scale[2]+header->scale_origin[2];

      // Store vertices in group
      vertex = eggVertexStore3f(x, z, y); // freyja coordinates

      // Generates id translator list
      trans.Add(v, vertex);
    }

    // End FREYJA_GROUP
    eggEnd(); 

    for (i = 0; f == 0 && i < header->numtris; i++)
    {
      // Start a new polygon
      eggBegin(FREYJA_POLYGON);

      // Store vertices by true id, using translator list
      eggVertex1i(trans[tris[i].vertindex[0]]);
      eggVertex1i(trans[tris[i].vertindex[1]]);
      eggVertex1i(trans[tris[i].vertindex[2]]);

      // Store texels into model and polygon ( store both at once in MDL )
      s = (float)st_vert[tris[i].vertindex[0]].s / (float)mdl.getTextureWidth();
      t = (float)st_vert[tris[i].vertindex[0]].t / (float)mdl.getTextureHeight();
      
      if (st_vert[tris[i].vertindex[0]].onseam == MDL_ALIAS_ONSEAM &&
	  tris[i].facesfront == 0)
	s = (float)(st_vert[tris[i].vertindex[0]].s + mdl.getTextureWidth()/2) / 
	  (float)mdl.getTextureWidth();

      eggTexel1i(eggTexelStore2f(s, t));
      
      s = (float)st_vert[tris[i].vertindex[1]].s / (float)mdl.getTextureWidth();
      t = (float)st_vert[tris[i].vertindex[1]].t / (float)mdl.getTextureHeight();

      if (st_vert[tris[i].vertindex[1]].onseam == MDL_ALIAS_ONSEAM &&
	  tris[i].facesfront == 0)
	s = (float)(st_vert[tris[i].vertindex[1]].s + mdl.getTextureWidth()/2) / 
	  (float)mdl.getTextureWidth();

      eggTexel1i(eggTexelStore2f(s, t));

      s = (float)st_vert[tris[i].vertindex[2]].s / (float)mdl.getTextureWidth();
      t = (float)st_vert[tris[i].vertindex[2]].t / (float)mdl.getTextureHeight();

      if (st_vert[tris[i].vertindex[2]].onseam == MDL_ALIAS_ONSEAM &&
	  tris[i].facesfront == 0)
	s = (float)(st_vert[tris[i].vertindex[2]].s + mdl.getTextureWidth()/2) / 
	  (float)mdl.getTextureWidth();

      eggTexel1i(eggTexelStore2f(s, t));

      eggTexture1i(0);

      // End FREYJA_POLYGON
      eggEnd();
    }
  }

  // End FREYJA_MESH
  eggEnd();

  return 0;
}


int freyja_model__mdl_export(char *filename)
{
#define STILL_WORKING_ON_IT
#ifdef STILL_WORKING_ON_IT
  printf("freyja_model__mdl_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);

  return -1;
#else
  MtkImage img;
  Mdl *model;
  MDL_t *mdl;
  stvert_t *vertex; 
  dtriangle_t *triangle;
  MDL_Frame_t *frame;
  MDL_Skin_t *qskin;
  egg_vertex_list_t *evertex;
  egg_frame_list_t *eframe;
  egg_polygon_list_t *polygon;
  char buffer[32];
  int i, j;
  float radius;
  int skin_w;
  int skin_h;
  unsigned char *skin;
  egg_mesh_t *mesh;
  int err = 0;
  float bbox_min[3];
  float epos[3] = {0.0, 64.0, 0.0}; // FIXME: Hack
  bool a, b, c;


  mesh = egg->FindMesh(mesh_id);

  // Load texture
  img.Load(freyja_rc_map("textures/mdltest.pcx"));

  // Convert into quake palette
  img.PaletteLoad(freyja_rc_map("palettes/quake.pal"));
  img.IndexedImage(&skin);
  skin_w = img.Width();
  skin_h = img.Height();

  if (!mesh || !mesh->frames || !skin)
  {
    mtkPrint("FreyjaModel::MdlExport> Assertion error missing skin or mesh\n");
    return -1;
  }

  printf("FreyjaModel::MdlExport> WARNING this is EXPERIMENTAL\n");

  if (mesh->frames->frame)
  {
    radius =  fabs(mesh->frames->frame->bbox_min[0]);
    
    if (fabs(mesh->frames->frame->bbox_min[1]) > radius) 
      radius = mesh->frames->frame->bbox_min[1];
    if (fabs(mesh->frames->frame->bbox_min[2]) > radius) 
      radius = mesh->frames->frame->bbox_min[2];
    
    if (fabs(mesh->frames->frame->bbox_max[0]) > radius) 
      radius = mesh->frames->frame->bbox_max[0];
    if (fabs(mesh->frames->frame->bbox_max[1]) > radius) 
      radius = mesh->frames->frame->bbox_max[1];
    if (fabs(mesh->frames->frame->bbox_max[2]) > radius) 
      radius = mesh->frames->frame->bbox_max[2];
  }
  else
  {
    radius = 24;
    printf("FreyjaModel::MdlExport> ERROR bad raduis\n");
  }

  // Force all polygons to triangles
  //egg->Tesselate(mesh); // FIXME: Not impelmented yet 

  // Initialize MDL header
  mdl = new MDL_t;

  mdl.ident = MDL_IDPOLYGON;
  mdl.version = MDL_ALIAS_VERSION;
  mdl.boundingradius = radius;
  mdl.skinwidth = skin_w;
  mdl.skinheight = skin_h;
  mdl.scale[0] = 1.0;
  mdl.scale[1] = 1.0;
  mdl.scale[2] = 1.0;
  mdl.scale_origin[0] = mesh->frames->frame->center[0];
  mdl.scale_origin[1] = mesh->frames->frame->center[1];
  mdl.scale_origin[2] = mesh->frames->frame->center[2]; 
  mdl.numskins = 1;
  mdl.numverts = mesh->frames->frame->vertex_count;
  mdl.numtris = mesh->polygon_count; 
  mdl.numframes = mesh->frame_count; 
  mdl.flags = flags;
  mdl.eyeposition[0] = epos[0];
  mdl.eyeposition[1] = epos[1];
  mdl.eyeposition[2] = epos[2];
  mdl.synctype = ST_SYNC;       
  mdl.size = 3.0;              

  vertex = new stvert_t[mdl.numverts];
  triangle = new dtriangle_t[mdl.numtris];
  frame = new MDL_Frame_t[mdl.numframes];
  qskin = new MDL_Skin_t;

  qskin->type = 0;
  qskin->pixmap = skin;
  qskin->gskin_interval = NULL;

  polygon = mesh->polys;

  for (i = 0; i < mdl.numtris && polygon; i++)
  {
    if (polygon->poly && polygon->poly->index_list)
    {
      triangle[i].vertindex[0] = polygon->poly->index_list[0];
      triangle[i].vertindex[1] = polygon->poly->index_list[1];
      triangle[i].vertindex[2] = polygon->poly->index_list[2];

      vertex[triangle[i].vertindex[0]].s = (int)(skin_w *polygon->poly->st[0]);
      vertex[triangle[i].vertindex[0]].t = (int)(skin_h *polygon->poly->st[1]);
      vertex[triangle[i].vertindex[0]].onseam = 0;

      vertex[triangle[i].vertindex[1]].s = (int)(skin_w *polygon->poly->st[2]);
      vertex[triangle[i].vertindex[1]].t = (int)(skin_h *polygon->poly->st[3]);
      vertex[triangle[i].vertindex[1]].onseam = 0;

      vertex[triangle[i].vertindex[2]].s = (int)(skin_w *polygon->poly->st[4]);
      vertex[triangle[i].vertindex[2]].t = (int)(skin_h *polygon->poly->st[5]);
      vertex[triangle[i].vertindex[2]].onseam = 0;

      /*****************
      // From loader code, this shows how onseam and facesfront change
      // texel info on load

	if (st_vert[tris[i].vertindex[1]].onseam == MDL_ALIAS_ONSEAM &&
	    tris[i].facesfront == 0)
	  st[2+0] = (float)(st_vert[tris[i].vertindex[1]].s + 
	             mdl.SkinWidth()/2) / (float)mdl.SkinWidth();
      *****************/

      ////////////////////////////////////////////////////
      // Figure out facesfront set for value triangle
      // it faces front if s < 0.5, t isn't considered
      ////////////////////////////////////////////////////
      a = b = c = false;

      if (vertex[triangle[i].vertindex[0]].s < skin_w/2)
	a = true;

      if (vertex[triangle[i].vertindex[1]].s < skin_w/2)
	b = true;

      if (vertex[triangle[i].vertindex[2]].s < skin_w/2)
	c = true;
    
      if (a && b && c)
	triangle[i].facesfront = MDL_DT_FACES_FRONT;
      else
	triangle[i].facesfront = 0;

      ////////////////////////////////////////////////////
      // Figure out onseam value
      ////////////////////////////////////////////////////
//#define TEST_MDL_ONSEAM
#ifdef TEST_MDL_ONSEAM
      if (!a && (b || c))
	vertex[triangle[i].vertindex[0]].onseam = MDL_ALIAS_ONSEAM;

      if (!b && ( a || c))
	vertex[triangle[i].vertindex[1]].onseam = MDL_ALIAS_ONSEAM;

      if (!c && ( a || b))
	vertex[triangle[i].vertindex[2]].onseam = MDL_ALIAS_ONSEAM;
#endif
    }
    else
    {
      // FIXME: Handle error
      printf("FreyjaModel::MdlExport> ERROR in polygon, tris[%i]\n", i);
      triangle[i].vertindex[0] = 0;
      triangle[i].vertindex[1] = 0;
      triangle[i].vertindex[2] = 0;
    }

    polygon = polygon->next;
  }

  ///////////////////////////////////////////////////////
  // NOTE:
  // The trick of the pack hack is to scale your model
  // down to fit within a 0-255 bbox so it can be
  // converted to 0-255, so we transform the model to
  // and all postive bbox here
  //
  // FIXME: Add scaling to fit into 0-255
  ///////////////////////////////////////////////////////

  eframe = mesh->frames;

  bbox_min[0] = bbox_min[1] = bbox_min[2] = 0.0;

  while (eframe)
  {
    if (eframe->frame)
    {
      if (eframe->frame->bbox_min[0] < bbox_min[0])
	bbox_min[0] = eframe->frame->bbox_min[0];

      if (eframe->frame->bbox_min[1] < bbox_min[1])
	bbox_min[1] = eframe->frame->bbox_min[1];

      if (eframe->frame->bbox_min[2] < bbox_min[2])
	bbox_min[2] = eframe->frame->bbox_min[2];
    }

    eframe = eframe->next;
  }

  egg->Transform(mesh, TRANSLATE, 
		  fabs(bbox_min[0]), fabs(bbox_min[1]), fabs(bbox_min[2]));

  eframe = mesh->frames;

  for(i = 0; i < mdl.numframes && eframe; i++)
  {
    frame[i].type = 0;  // single frames

    // FIXME: Frame name hack
    sprintf(buffer, "frame%i", i); 

    strcpy(frame[i].sframe.name, buffer);

    frame[i].sframe.tv = new trivertex_t[mdl.numverts];

    if (eframe->frame)
    {
      // Bounding box lightnormals aren't used, so 0 is ok
      frame[i].sframe.bboxmin.lightnormalindex = 0;
      frame[i].sframe.bboxmax.lightnormalindex = 0;

      // Init bboxmin at greatest possible value
      frame[i].sframe.bboxmin.v[0] = 
	frame[i].sframe.bboxmin.v[1] = 
	  frame[i].sframe.bboxmin.v[2] = 255;
       
      // Init bboxmax at least possible value
      frame[i].sframe.bboxmax.v[0] = 
	frame[i].sframe.bboxmax.v[1] = 
	  frame[i].sframe.bboxmax.v[2] = 0;

      evertex = eframe->frame->verts;

      for (j = 0; j < mdl.numverts && evertex; j++)
      {
	if (evertex->vertex)
	{
	  // Convert vertices to packed postions
	  frame[i].sframe.tv[j].v[0] = (unsigned char)
	    ((evertex->vertex->pos[0] - mdl.scale_origin[0]) / mdl.scale[0]);

	  frame[i].sframe.tv[j].v[1] = (unsigned char)
	    ((evertex->vertex->pos[1] - mdl.scale_origin[1]) / mdl.scale[1]);

	  frame[i].sframe.tv[j].v[2] = (unsigned char) 
	    ((evertex->vertex->pos[2] - mdl.scale_origin[2]) / mdl.scale[2]);

	  if (frame[i].sframe.tv[j].v[0] < frame[i].sframe.bboxmin.v[0])
	    frame[i].sframe.bboxmin.v[0] = frame[i].sframe.tv[j].v[0];

	  if (frame[i].sframe.tv[j].v[1] < frame[i].sframe.bboxmin.v[1])
	    frame[i].sframe.bboxmin.v[1] = frame[i].sframe.tv[j].v[1];

	  if (frame[i].sframe.tv[j].v[2] < frame[i].sframe.bboxmin.v[2])
	    frame[i].sframe.bboxmin.v[2] = frame[i].sframe.tv[j].v[2];
	
	  if (frame[i].sframe.tv[j].v[0] > frame[i].sframe.bboxmax.v[0])
	    frame[i].sframe.bboxmax.v[0] = frame[i].sframe.tv[j].v[0];

	  if (frame[i].sframe.tv[j].v[1] > frame[i].sframe.bboxmax.v[1])
	    frame[i].sframe.bboxmax.v[1] = frame[i].sframe.tv[j].v[1];

	  if (frame[i].sframe.tv[j].v[2] > frame[i].sframe.bboxmax.v[2])
	    frame[i].sframe.bboxmax.v[2] = frame[i].sframe.tv[j].v[2];

	  //FIXME: Shouldn't just 'have one'
	  frame[i].sframe.tv[j].lightnormalindex = 1; 
	}

	evertex = evertex->next;
      }
    }

//#define DEBUG_MDL_BBOX
#ifdef DEBUG_MDL_BBOX
    // DEBUG: Dump bbox info to console
    printf("egg bbox ( %.0f, %.0f, %.0f), ( %.0f, %.0f, %.0f)\n",
	   eframe->frame->bbox_min[0], 
	   eframe->frame->bbox_min[1],
	   eframe->frame->bbox_min[2],
	   eframe->frame->bbox_max[0],
	   eframe->frame->bbox_max[1],
	   eframe->frame->bbox_max[2]);

    printf("mdl bbox ( %u, %u, %u), ( %u, %u, %u)\n",
	   frame[i].sframe.bboxmin.v[0],
	   frame[i].sframe.bboxmin.v[1],
	   frame[i].sframe.bboxmin.v[2],
	   frame[i].sframe.bboxmax.v[0],
	   frame[i].sframe.bboxmax.v[1],
	   frame[i].sframe.bboxmax.v[2]);
#endif

    frame[i].gframe.num_gsframes = 0;
    frame[i].gframe.gsframe = NULL;
    frame[i].gframe.gsframe_interval = NULL;

    eframe = eframe->next;
  }

  model = new Mdl();
  model->Header(mdl);
  model->Vertex(vertex);
  model->Triangle(triangle);
  model->Frame(frame);
  model->Skin(qskin);

  printf("FreyjaModel::MdlExport> Exporting mesh to '%s'\n", filename);
  err = model->Save(filename);

  delete model;

  return err;
#endif
}
