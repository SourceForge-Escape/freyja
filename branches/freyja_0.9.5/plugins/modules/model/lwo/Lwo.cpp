/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Lwo
 * License : GPL
 * Comments: LWO module for freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.07.04:
 * Mongoose - Created, based on the Gtk LWO demo ( horrible glib dep )
 *                     by Janne Löf <jlof@mail.student.oulu.fi>
 =================================================================*/

// FIMXE: Add endian support back later

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Lwo.h"


#define MK_ID(a,b,c,d) ((((unsigned int)(a))<<24)| \
			(((unsigned int)(b))<<16)| \
			(((unsigned int)(c))<< 8)| \
			(((unsigned int)(d))    ))

#define ID_FORM MK_ID('F','O','R','M')
#define ID_LWOB MK_ID('L','W','O','B')
#define ID_PNTS MK_ID('P','N','T','S')
#define ID_SRFS MK_ID('S','R','F','S')
#define ID_SURF MK_ID('S','U','R','F')
#define ID_POLS MK_ID('P','O','L','S')
#define ID_COLR MK_ID('C','O','L','R')


int read_char(FILE *f)
{
  int c = fgetc(f);

  if (c != EOF)
	  return c;
  else
	  return -1;
}


int read_short(FILE *f)
{
  return (read_char(f)<<8) | read_char(f);
}


int read_long(FILE *f)
{
  return (read_char(f)<<24) | (read_char(f)<<16) | (read_char(f)<<8) | read_char(f);
}


float read_float(FILE *f)
{
  int x = read_long(f);
  return *(float*)&x;
}


int read_string(FILE *f, char *s)
{
  int c;
  int cnt = 0;
  do {
    c = read_char(f);
    if (cnt < LW_MAX_NAME_LEN)
      s[cnt] = c;
    else
      s[LW_MAX_NAME_LEN-1] = 0;
    cnt++;
  } while (c != 0);
  /* if length of string (including \0) is odd skip another byte */
  if (cnt%2) {
    read_char(f);
    cnt++;
  }
  return cnt;
}


/////////////////////////////////////////////////////////

Lwo::Lwo()
{
	_face_count = 0;
	_face = NULL;
	_material_count = 0;
	_material = NULL;
	_vertex_count = 0;
	_vertex = NULL;
	_mat_map = NULL;
	_face_map = NULL;
}


Lwo::~Lwo()
{
	lw_face_t *face;
	lw_material_t *material;


	while (_face)
	{
		delete [] _face->index;

		face = _face;
		_face = _face->next;

		delete face;
	}

	if (_face_map)
	{
		delete [] _face_map;
	}

	if (_mat_map)
	{
		delete [] _mat_map;
	}

	while (_material)
	{
		material = _material;
		_material = _material->next;

		delete material;
	}

	if (_vertex)
	{
		delete [] _vertex;
	}
}


int Lwo::VertexCount()
{
	return _vertex_count;
}


int Lwo::MaterialCount()
{
	return _material_count;
}


int Lwo::FaceCount()
{
	return _face_count;
}


void Lwo::Vertex(int i, float *vertex)
{
	vertex[0] = 0.0;
	vertex[1] = 0.0;
	vertex[2] = 0.0;

	if (i < 0 || i > _vertex_count)
		return;

	vertex[0] = _vertex[i*3];
	vertex[1] = _vertex[i*3 + 1];
	vertex[2] = _vertex[i*3 + 2];
}


lw_face_t *Lwo::Face(int i)
{
	if (i < 0 || i > _face_count)
		return NULL;

	return _face_map[i];
}


lw_material_t *Lwo::Material(int i)
{
	if (i < 0 || i > _material_count)
		return NULL;

	return _mat_map[i];
}


int Lwo::Check(const char *filename)
{
  FILE *f = fopen(filename, "rb");
  int form, nlen, lwob;


  if (f) 
  {
    form = read_long(f);
    nlen = read_long(f);
    lwob = read_long(f);

    fclose(f);
    
	 if (form == ID_FORM && nlen != 0 && lwob == ID_LWOB)
		 return 0;
  }
  
  return -1;
}


int Lwo::Load(const char *filename)
{
	FILE *f = NULL;
	int form_bytes = 0;
	int read_bytes = 0;
	lw_material_t *material;
	lw_face_t *face;
	int i;

	
	/* Open file */
	f = fopen(filename, "rb");
	
	if (!f) 
	{
		perror("Lwo::Load> Can't open file\n");
		return -1;
	}
	
	/* Check for headers */
	if (read_long(f) != ID_FORM) 
	{
		printf("Lwo::Load> '%s' is not an IFF file", filename);
		fclose(f);
		return -2;
	}

	form_bytes = read_long(f);
	read_bytes += 4;
	
	if (read_long(f) != ID_LWOB) 
	{
		printf("Lwo::Load> '%s' is not an LWOB file", filename);
		fclose(f);
		return -3;
	}


	/* Read chunks */
	while (read_bytes < form_bytes) 
	{
		int  id     = read_long(f);
		int  nbytes = read_long(f);
		read_bytes += 8 + nbytes + (nbytes%2);
		
		switch (id) 
		{
		case ID_PNTS:
			ReadPnts(f, nbytes);
			break;
		case ID_POLS:
			ReadPols(f, nbytes);
			break;
		case ID_SRFS:
			ReadSrfs(f, nbytes);
			break;
		case ID_SURF:
			ReadSurf(f, nbytes);
			break;
		default:
			fseek(f, nbytes + (nbytes%2), SEEK_CUR);
		}
	}
	
	fclose(f);

	if (_material_count)
	{
		_mat_map = new lw_material_t*[_material_count];

		i = 0;
		material = _material;

		while (material)
		{
			_mat_map[i++] = material;
			material = material->next;
		}
	}

	if (_face_count)
	{
		_face_map = new lw_face_t*[_face_count];

		i = 0;
		face = _face;

		while (face)
		{
			_face_map[i++] = face;
			face = face->next;
		}
	}

	return 0;
}


float Lwo::Radius()
{
	int i;
	double max_radius = 0.0;


  	if (!_vertex_count)
		return 0.0;

	for (i = 0; i < _vertex_count; i++) 
	{
		float *v = &_vertex[i*3];
		double r = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];

		if (r > max_radius)
			max_radius = r;
  }
	
	return sqrt(max_radius);
}


void Lwo::Scale(float scale)
{
	int i;


	if (!_vertex_count || scale == 0.0)
		return;

	for (i = 0; i < _vertex_count; i++) 
	{
		_vertex[i*3+0] *= scale;
		_vertex[i*3+1] *= scale;
		_vertex[i*3+2] *= scale;
	}
}


////// PRIVATE METHODS ////////////////////////////////////////////



void Lwo::ReadSrfs(FILE *f, int nbytes)
{
	lw_material_t *material = NULL;
	lw_material_t *head = NULL;


	while (nbytes > 0) 
	{
		// Mongoose: 2001.11.01 Better than that realloc crap
		if (!material)
		{
			material = new lw_material_t;
			head = material;
		}
		else
		{
			material->next = new lw_material_t;
			material = material->next;
		}

		material->next = NULL;
		_material_count++;

		/* Read name */
		nbytes -= read_string(f, material->name);
		
		/* defaults */
		material->r = 0.7;
		material->g = 0.7;
		material->b = 0.7;
		material->a = 0.0; // Mongoose: You need alpha you silly foo  =)
	}

	if (!_material)
	{
		_material = head;
	}
	else
	{
		material = _material;

		while (material->next)
		{
			material = material->next;
		}
		
		material->next = head;
	}
}


void Lwo::ReadSurf(FILE *f, int nbytes)
{
	char name[LW_MAX_NAME_LEN];
	lw_material_t *material = NULL;


	/* Read surface name */
	nbytes -= read_string(f, name);

	material = _material;
	
	while (material)
	{
		if (strcmp(material->name, name) == 0) 
		{
			break;
		}	

		material = material->next;
	}

	if (!material)
	{
		printf("Lwo::ReadSurf> Failed to load a material\n");
		return;
	}

	/* Read values */
	while (nbytes > 0) 
	{
		int id = read_long(f);
		int len = read_short(f);
		nbytes -= 6 + len + (len%2);
		
		switch (id) 
		{
		case ID_COLR:
			material->r = read_char(f) / 255.0;
			material->g = read_char(f) / 255.0;
			material->b = read_char(f) / 255.0;

			// Mongoose: Janne you foo this is alpha, not a dummy val
			material->a = read_char(f) / 255.0;
			break;
		default:
			fseek(f, len+(len%2), SEEK_CUR);
		}
	}
}


void Lwo::ReadPols(FILE *f, int nbytes)
{
	lw_face_t *face = NULL;
	lw_face_t *head;
	int i;
  

	while (nbytes > 0) 
	{
		// Mongoose: 2001.11.01 Better than that realloc crap
		if (!face)
		{
			face = new lw_face_t;
			head = face;
		}
		else
		{
			face->next = new lw_face_t;
			face = face->next;
		}

		face->next = NULL;
		_face_count++;

		/* Number of points in this face */
		face->index_cnt = read_short(f);
		nbytes -= 2;
		
		/* Allocate space for points */
		face->index = new int[face->index_cnt];
    
		/* Read points in */
		for (i = 0;  i < face->index_cnt; i++) 
		{
			face->index[i] = read_short(f);
			nbytes -= 2;
		}
    
		/* Read surface material */
		face->material = read_short(f);
		nbytes -= 2;
    
		/* Skip over detail  polygons */
		if (face->material < 0) 
		{
			int det_cnt;
			
			face->material = -face->material;
			det_cnt = read_short(f);
			nbytes -= 2;
			
			while (det_cnt-- > 0) 
			{
				int cnt = read_short(f);
				
				fseek(f, cnt*2+2, SEEK_CUR);
				nbytes -= cnt*2+2;
			}
		}

		face->material -= 1;
  }

 	if (!_face)
	{
		_face = head;
	}
	else
	{
		face = _face;

		while (face->next)
		{
			face = face->next;
		}
		
		face->next = head;
	} 
}


void Lwo::ReadPnts(FILE *f, int nbytes)
{
  int i;


  _vertex_count = nbytes / 12;
  _vertex = new float[_vertex_count*3];

  for (i = 0; i < _vertex_count; i++) 
  {
	  _vertex[i*3+0] = read_float(f);
	  _vertex[i*3+1] = read_float(f);
	  _vertex[i*3+2] = read_float(f);
  }
}


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS
#include <stdio.h>
#include <freyja/PluginABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/MeshABI.h>
#include <mstl/Map.h>

using namespace mstl;

extern "C" {

	int freyja_model__lwo_check(char *filename);
	int freyja_model__lwo_import(char *filename);
	int freyja_model__lwo_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginName1s("lwo");
	freyjaPluginDescription1s("Lightwave Object (*.lwo)");
	freyjaPluginAddExtention1s("*.lwo");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}

int import_model(char *filename)
{
	if (!freyja_model__lwo_check(filename))
		return freyja_model__lwo_import(filename);

	return -1;
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

  freyjaBegin(FREYJA_MODEL);

  // Start a new mesh
  freyjaBegin(FREYJA_MESH);

  // Start a new vertex group
  freyjaBegin(FREYJA_VERTEX_GROUP);

  for (i = 0; i < lwo.VertexCount(); i++)
  {
    lwo.Vertex(i, vert);

    // Store vertices in group
    v = freyjaVertexCreate3f(vert[0], vert[1], vert[2]);

#ifdef DEBUG_LWO_VERTS
    printf("%i = %f %f %f\n", v, vert[0], vert[1], vert[2]);
#endif

    // Generates id translator list
    trans.Add(i, v);
  }

  freyjaEnd(); // FREYJA_GROUP

  for (i = 0; i < lwo.FaceCount(); i++)
  {
    face = lwo.Face(i);

    if (!face)
      continue;

    // Ignore non polygons
    if (face->index_cnt < 3)
      continue;

    // Start a new polygon
    freyjaBegin(FREYJA_POLYGON);
    
    for (j = 0; j < face->index_cnt; j++)
    {
      // Store vertices by true id, using translator list
      freyjaPolygonVertex1i(trans[face->index[j]]);

#ifdef DEBUG_LWO_TRANS
      printf("trans[%i] = %i\n", face->index[j], trans[face->index[j]]);
#endif

      material = lwo.Material(face->material);

      if (material)
      {
	// Generate color by texel hack, store it's index
			t = freyjaTexCoordCreate2f(material->r, material->g);
			freyjaPolygonTexCoord1i(t);
	
			t = freyjaTexCoordCreate2f(material->b, material->a);
			freyjaPolygonTexCoord1i(t);
      }
      else
      {
			t = freyjaTexCoordCreate2f(0.0, 0.0);
			freyjaPolygonTexCoord1i(t);
	
			t = freyjaTexCoordCreate2f(0.0, 0.0);
			freyjaPolygonTexCoord1i(t);	
      }
    }

    freyjaPolygonMaterial1i(0);

    freyjaEnd(); // FREYJA_POLYGON
  }

  freyjaEnd(); // FREYJA_MESH
  freyjaEnd(); // FREYJA_MODEL
  
  return 0;
}


int freyja_model__lwo_export(char *filename)
{
  printf("freyja_model__lwo_export> Not implemented, %s:%i\n", 
	 __FILE__, __LINE__);
  return -1;
}
#endif

#ifdef NEVER_USE_THIS
#define PX(i) (lw_object->vertex[face->index[i]*3+0])
#define PY(i) (lw_object->vertex[face->index[i]*3+1])
#define PZ(i) (lw_object->vertex[face->index[i]*3+2])


void lw_object_show(const lwObject *lw_object)
{
	int i,j;
	int prev_index_cnt = -1;
	int prev_material  = -1;
	GLfloat prev_nx = 0;
	GLfloat prev_ny = 0;
	GLfloat prev_nz = 0;


	g_return_if_fail(lw_object != NULL);

	for (i=0; i<lw_object->face_cnt; i++) 
	{
		GLfloat ax,ay,az,bx,by,bz,nx,ny,nz,r;
		const lwFace *face = lw_object->face+i;

		/* ignore faces with less than 3 points */
		if (face->index_cnt < 3)
			continue;

		/* calculate normal */
		ax = PX(1) - PX(0);
		ay = PY(1) - PY(0);
		az = PZ(1) - PZ(0);

		bx = PX(face->index_cnt-1) - PX(0);
		by = PY(face->index_cnt-1) - PY(0);
		bz = PZ(face->index_cnt-1) - PZ(0);

		nx = ay * bz - az * by;
		ny = az * bx - ax * bz;
		nz = ax * by - ay * bx;

		r = sqrt(nx*nx + ny*ny + nz*nz);
		if (r < 0.000001) /* avoid division by zero */
			continue;
		nx /= r;
		ny /= r;
		nz /= r;

		/* glBegin/glEnd */
		if (prev_index_cnt != face->index_cnt || prev_index_cnt > 4) {
			if (prev_index_cnt > 0) glEnd();
			prev_index_cnt = face->index_cnt;
			switch (face->index_cnt) {
			case 3:
				glBegin(GL_TRIANGLES);
				break;
			case 4:
				glBegin(GL_QUADS);
				break;
			default:
				glBegin(GL_POLYGON);
			}
		}

		/* update material if necessary */
		if (prev_material != face->material) {
			prev_material = face->material;
			glColor3f(lw_object->material[face->material].r,
						 lw_object->material[face->material].g,
						 lw_object->material[face->material].b);
		}

		/* update normal if necessary */
		if (nx != prev_nx || ny != prev_ny || nz != prev_nz) {
			prev_nx = nx;
			prev_ny = ny;
			prev_nz = nz;
			glNormal3f(nx,ny,nz);
		}

		/* draw polygon/triangle/quad */
		for (j=0; j<face->index_cnt; j++)
			glVertex3f(PX(j),PY(j),PZ(j));

	}

	/* if glBegin was called call glEnd */
	if (prev_index_cnt > 0)
		glEnd();
}
#endif
