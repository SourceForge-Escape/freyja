/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*==========================================================================
 * 
 * Project : MDDC 
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : Md2
 * Comments: This is the MD2 model class.
 *
 *           See file COPYING for license details.
 *
 *           Quake (c) id Software 1995 
 *
 *           Based on viewmd2 (c) Michael K Vance, 1999  
 *                                briareos@lokigames.com
 *                                mkv102@psu.edu 
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2000-03-16
 * Mongoose: Endian support
 *           Created from odd MDDC source
 ==========================================================================*/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef USING_OPENGL
#   include <GL/gl.h>
#endif

#include "Md2.h"


Md2::Md2()
{
   _valid = false;

   _header.texCoords = NULL;
   _header.tris = NULL;
   _header.frames = NULL;
   _header.commands = NULL;
}

Md2::Md2(char *filename)
{
   _valid = Load(filename);
}

Md2::~Md2()
{
   if (_header.texCoords)
      delete []  _header.texCoords;
 
   if (_header.tris)
      delete []  _header.tris;

   if (_header.frames)
      delete []  _header.frames;

   if (_header.commands)
      delete []  _header.commands;
}

MD2_t *Md2::Header()
{
   return &_header;
}

VertexIndex_t *Md2::Frame(int n)
{
  return _header.frames[n].verts;
}


unsigned int Md2::getNumFrames()
{
  return _header.numFrames;
}


AliasFrame_t *Md2::getFrame(unsigned int frame)
{
  if (frame > getNumFrames())
  {
    return NULL;
  }

  return _header.frames + frame;
}



void Md2::BoundingBox_Update(const float x, const float y, const float z)
{
    if (x < _bbox.xMin) 
       _bbox.xMin = x;
    else if ( x > _bbox.xMax )
       _bbox.xMax = x;

    if (y < _bbox.yMin)
       _bbox.yMin = y;
    else if (y > _bbox.yMax)
       _bbox.yMax = y;

    if (z < _bbox.zMin)
       _bbox.zMin = z;
    else if (z > _bbox.zMax)
	_bbox.zMax = z;
}

int Md2::SkinWidth()
{
  return _header.skinWidth;
}

int Md2::SkinHeight()
{
  return _header.skinHeight;
}

char *Md2::SkinName(int n)
{
  if (n < 0 || n > _header.numSkins)
    return NULL;

  return (char *)(&_skins[n]);
}

int Md2::Load(char *filename)
{
   int i, j;
   FILE *f;
   int vertc = 0;
   

   f = fopen(filename, "rb");

   printf("QuakeModel2_Import\n |\n");

   if (!f)
   {
      printf(" |- File %s not found.\n", filename);
      return 1;
   }
   else
      printf(" |- Importing %s as MD2.\n", filename);

   fread(&_header.ident, sizeof(int), 1, f);
      
   if (_header.ident != MD2_IDALIASHEADER) 
   {
      printf(" |- File is not in MD2 format.\n");
      return 2;
   }

   fread(&_header.version, sizeof(int), 1, f);
   
   if (_header.version != MD2_ALIAS_VERSION) 
   {
      printf(" |- File is not in correct MD2 format.\n");
      return 3;
   }

   fread(&_header.skinWidth, sizeof(int), 1, f);
   fread(&_header.skinHeight, sizeof(int), 1, f);
   fread(&_header.frameSize, sizeof(int), 1, f);
   fread(&_header.numSkins, sizeof(int), 1, f);

   printf(" |- Skin width is %i.\n", _header.skinWidth);
   printf(" |- Skin height is %i.\n", _header.skinHeight);
   printf(" |- Frame size is %i.\n", _header.frameSize);
   printf(" |- Contains %i skins.\n", _header.numSkins);

   if (_header.numSkins > MD2_MAX_MD2SKINS) 
   {
      printf("MD2: numSkins > MD2_MAX_MD2SKINS\n");
      return 4;
   }

   fread(&_header.numXYZ, sizeof(int), 1, f);

   if (_header.numXYZ > MD2_MAX_VERTS) 
   {
      printf("MD2: numXYZ > MD2_MAX_VERTS\n");
      return 5;
   }

   fread(&_header.numST, sizeof(int), 1, f);
   fread(&_header.numTris, sizeof(int), 1, f);

   if (_header.numTris > MD2_MAX_TRIANGLES) 
   {
      printf("MD2: numTris > MD2_MAX_TRIANGLES\n");
      return 6;
   }

   fread(&_header.numGLCmds, sizeof(int), 1, f);
   fread(&_header.numFrames, sizeof(int), 1, f);

   if (_header.numFrames > MD2_MAX_FRAMES) 
   {
      printf("MD2: numFrames > MD2_MAX_FRAMES\n");
      return 7;
   }

   fread(&_header.ofsSkins, sizeof(int), 1, f);
   fread(&_header.ofsST, sizeof(int), 1, f);
   fread(&_header.ofsTris, sizeof(int), 1, f);
   fread(&_header.ofsFrames, sizeof(int), 1, f);
   fread(&_header.ofsGLCmds, sizeof(int), 1, f);
   fread(&_header.ofsEnd, sizeof(int), 1, f);

   /* John King (john.king@ucl.ac.uk): 
      This is where skin names are read.  I don't expose 
      the functionality, but it would be trivial to do so. */
   for (i = 0; i < _header.numSkins; i++) 
   {
      fread(&(_skins[i]), MD2_MAX_SKINNAME, 1, f);
   }

   printf(" |- Read %i skins.\n", i);   

   _header.texCoords = new TexCoordsIndex_t[_header.numST];

   /* Read texture coordinates in pixel values.*/
   for (i = 0; i < _header.numST; i++) 
   {
      short s, t;

      fread(&s, sizeof(short), 1, f);
      fread(&t, sizeof(short), 1, f);

      _header.texCoords[i].s = s;
      _header.texCoords[i].t = t;
   }
   printf(" |- Read %i texCoords.\n", i);   

   _header.tris = new TriangleIndex_t[_header.numTris];

   /* Read triangles. */
   for(i = 0; i < _header.numTris; i++ ) 
   {
      short v0, v1, v2;
      short st0, st1, st2;

      fread(&v0, sizeof(short), 1, f);
      fread(&v1, sizeof(short), 1, f);
      fread(&v2, sizeof(short), 1, f);
      fread(&st0, sizeof(short), 1, f);
      fread(&st1, sizeof(short), 1, f);
      fread(&st2, sizeof(short), 1, f);

      _header.tris[i].v[0] = v0;
      _header.tris[i].v[1] = v1;
      _header.tris[i].v[2] = v2;
      _header.tris[i].st[0] = st0;
      _header.tris[i].st[1] = st1;
      _header.tris[i].st[2] = st2;
   }
   printf(" |- Read %i triangles.\n", i);   

   _header.frames = new AliasFrame_t[_header.numFrames];

   /* Read frames. */
   for(i = 0; i < _header.numFrames; ++i) 
   {
      float s0, s1, s2;
      float t0, t1, t2;


      fread(&s0, sizeof(float), 1, f);
      fread(&s1, sizeof(float), 1, f);
      fread(&s2, sizeof(float), 1, f);
      fread(&t0, sizeof(float), 1, f);
      fread(&t1, sizeof(float), 1, f);
      fread(&t2, sizeof(float), 1, f);

      _header.frames[i].scale[0] = s0;
      _header.frames[i].scale[1] = s1;
      _header.frames[i].scale[2] = s2;
      _header.frames[i].translate[0] = t0;
      _header.frames[i].translate[1] = t1;
      _header.frames[i].translate[2] = t2;

      fread(&_header.frames[i].name, sizeof(char) * 16, 1, f);

      _header.frames[i].verts = new VertexIndex_t[_header.numXYZ];
      
      _bbox.xMin = 0;
      _bbox.xMax = 0;
      _bbox.yMin = 0;
      _bbox.yMax = 0;
      _bbox.zMin = 0;
      _bbox.zMax = 0;

      for (j = 0; j < _header.numXYZ; ++j) 
      {
         unsigned char v;


         fread(&v, sizeof(unsigned char), 1, f);  /* Mongoose: Read 
                                                       in the packed vert */

         /* Mongoose: pos[i] = (packed[i] &scale[i])/ origin[i] */

	 _header.frames[i].verts[j].x = ( ( v * _header.frames[i].scale[0] ) + 
                                   _header.frames[i].translate[0] );

         fread(&v, sizeof(unsigned char), 1, f);

	 _header.frames[i].verts[j].y = ( ( v * _header.frames[i].scale[1] ) + 
                                   _header.frames[i].translate[1] );

         fread(&v, sizeof(unsigned char), 1, f);

	 _header.frames[i].verts[j].z = ( ( v * _header.frames[i].scale[2] ) + 
                                   _header.frames[i].translate[2] );

         fread(&(_header.frames[i].verts[j].normal),sizeof(unsigned char),1,f);

	 BoundingBox_Update(_header.frames[i].verts[j].x, 
                            _header.frames[i].verts[j].y, 
                            _header.frames[i].verts[j].z);
	}
        vertc += j;
   }

   printf(" |- Read %i vertices in %i frames.\n", vertc, i);   

   if (i)
      printf(" |- Read %i vertices each frame.\n", vertc/i);   

   _header.commands = new int[_header.numGLCmds];

   for (i = 0; i < _header.numGLCmds; i++ ) 
   {
      int cmd;


      fread(&cmd, sizeof(int), 1, f);
      _header.commands[i] = cmd;
   }

   printf(" |- Done\n");
   return 0;
}

void Md2::TexCoords(short *s, short *t, int tri, int vert)
{
   int index = _header.tris[tri].st[vert];


   *s = _header.texCoords[index].s;
   *t = _header.texCoords[index].t;
}

double Md2::GetRadius()
{
    double radius;


    radius = fabs(_bbox.xMin);

    if (fabs(_bbox.xMax) > radius) 
	radius = _bbox.xMax;

    if (fabs(_bbox.yMin) > radius)
	radius = _bbox.yMin;

    if (fabs(_bbox.yMax) > radius)
	radius = _bbox.yMax;

    if (fabs(_bbox.zMin) > radius)
	radius = _bbox.zMin;

    if (fabs(_bbox.zMax) > radius)
	radius = _bbox.zMax;

    return radius;
}


#ifdef USING_OPENGL
void Md2::render(unsigned int frame, unsigned int texture)
{
	short ss, tt;
	unsigned int i, j;
	MD2_t *md2_header;
	VertexIndex_t *md2_vertex;
	AliasFrame_t *md2_frame;
 

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	md2_header = Header();
	md2_vertex = Frame(frame);
	md2_frame = getFrame(frame);

	if (!md2_frame || !md2_header)
	{
		return;
	}
    
	glPushMatrix();

	for (i = 0; (int)i < md2_header->numTris; ++i)
	{
		glBegin(GL_TRIANGLES);
		
		for (j = 0; j < 3; ++j)
		{
			TexCoords(&ss, &tt, i, j);

			glTexCoord2f((float)ss / (float)md2_header->skinWidth,
							 (float)tt / (float)md2_header->skinHeight);
#ifdef OBSOLETE
			glVertex3f(md2_frame->verts[md2_header->tris[i].v[j]].x,
						  md2_frame->verts[md2_header->tris[i].v[j]].y,
						  md2_frame->verts[md2_header->tris[i].v[j]].z);
#else
			glVertex3f(md2_vertex[md2_header->tris[i].v[j]].x*md2_frame->scale[0]+md2_frame->translate[0],
						  md2_vertex[md2_header->tris[i].v[j]].y*md2_frame->scale[1]+md2_frame->translate[1],
						  md2_vertex[md2_header->tris[i].v[j]].z*md2_frame->scale[2]+md2_frame->translate[2]);
#endif
		}
		
		glEnd();
	}
	
	glPopMatrix();
}
#endif

////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS
#include <string.h>
#include <stdio.h>
#include <freyja/FreyjaPlugin.h>




extern "C" {

	int import_model(char *filename);
	int export_model(char *filename);

	int freyja_model__md2_check(char *filename);
	int freyja_model__md2_import(char *filename);
	int freyja_model__md2_export(char *filename);
}


int import_model(char *filename)
{
	if (freyja_model__md2_check(filename) != 0)
		return -1;

	return freyja_model__md2_import(filename);
}


int export_model(char *filename)
{
	return freyja_model__md2_export(filename);
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
	unsigned int vertex;
	int textureId = -1;


	if (md2.Load(filename))
		return -1;

	md2_header = md2.Header();

	if (md2.SkinName(0))
	{
		textureId = freyjaTextureFilename1s(md2.SkinName(0));
	}


	freyjaBegin(FREYJA_MODEL);
	freyjaBegin(FREYJA_MESH);

	for (f = 0; f < md2_header->numFrames; f++)
	{
		md2_vertex = md2.Frame(f);
		    
		if (f != 0)
			freyjaBegin(FREYJA_VERTEX_GROUP);

		for (v = 0; v < md2_header->numXYZ; v++)
		{
			// Store vertices in group
			vertex = freyjaVertex3f(md2_vertex[v].x, 
											md2_vertex[v].z, 
											md2_vertex[v].y);  // Swap Y and Z for freyja
			
			// Generates id translator  table for this vertex morph frame
			//trans.Add(v, vertex);
		}
		
		if (f != 0)
			freyjaEnd(); // FREYJA_VERTEX_GROUP
	}

	// Md2 uses symetric vertex morph frames, so only need it once
	for (p = 0; f == 0 && p < md2_header->numTris; p++)
	{
      // Start a new polygon
      freyjaBegin(FREYJA_POLYGON);
      
		// REMOVED trans[] use
      // Store vertices by true id, using translation table
      freyjaPolygonVertex1i(md2_header->tris[p].v[0]);
      freyjaPolygonVertex1i(md2_header->tris[p].v[1]);
      freyjaPolygonVertex1i(md2_header->tris[p].v[2]);
      
       // Store texels into model and polygon ( store both at once in MD2 )
      md2.TexCoords(&ss, &tt, p, 0);
      s = (float)ss / (float)md2_header->skinWidth;
      t = (float)tt / (float)md2_header->skinHeight;
      freyjaPolygonTexCoord1i(freyjaTexCoord2f(s, t));
      
      md2.TexCoords(&ss, &tt, p, 1);
      s = (float)ss / (float)md2_header->skinWidth;
      t = (float)tt / (float)md2_header->skinHeight;
      freyjaPolygonTexCoord1i(freyjaTexCoord2f(s, t));
      
      md2.TexCoords(&ss, &tt, p, 2);
      s = (float)ss / (float)md2_header->skinWidth;
      t = (float)tt / (float)md2_header->skinHeight;
      freyjaPolygonTexCoord1i(freyjaTexCoord2f(s, t));
      
      freyjaPolygonMaterial1i(textureId);
      
      freyjaEnd(); // FREYJA_POLYGON
	}
   
	freyjaEnd(); // FREYJA_MESH
	freyjaEnd(); // FREYJA_MODEL
	
	return 0;
}


int freyja_model__md2_export(char *filename)
{
  printf("freyja_model__md2_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}

#endif

#ifdef UNIT_TEST_MD2
int main(int argc, char *argv[])
{
  Md2 md2;


  printf("[MD2 class test]\n");

  if (argc > 2)
  {
    if (strcmp(argv[1], "load") == 0)
    {
      if (!md2.Load(argv[2]))
	printf("main: Load reports success.\n");
    }
    else if (strcmp(argv[1], "save") == 0)
    {
      printf("Not Implemented\n");
    }
    else if (strcmp(argv[1], "test") == 0 && argc > 3)
    {
      printf("Not Implemented\n");
    }
    else
      printf("\n\n%s [save | load | test] filename.md2 [testout.md2]\n", 
	     argv[0]);
  }
  else
  {
    printf("\n\n%s [save | load | test] filename.md2 [testout.md2]\n", 
	   argv[0]);
  }
}
#endif
