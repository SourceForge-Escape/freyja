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
 *-- History ------------------------------------------------ 
 *
 * 2001.07.04:
 * Mongoose - Created, based on the Gtk LWO demo ( horrible glib dep )
 *                     by Janne Löf <jlof@mail.student.oulu.fi>
 ================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_LWO_H
#define GUARD__FREYJA_MONGOOSE_LWO_H

#include <stdio.h>

#define LW_MAX_POINTS   200
#define LW_MAX_NAME_LEN 500


typedef struct lw_material_s
{
	char name[LW_MAX_NAME_LEN];
	float r,g,b,a;

	struct lw_material_s *next;

} lw_material_t;


typedef struct lw_face_s
{
	int material;         /* Material of this face */
	int index_cnt;        /* Number of vertices */
	int *index;           /* Index to vertex */
	float *texcoord;      /* The u,v texture coordinates */

	struct lw_face_s *next;

} lw_face_t;


class Lwo
{
 public:

  Lwo();
  /*------------------------------------------------------
   * Pre  : 
   * Post : Constructs an object of Lwo
   *
   *-- History ------------------------------------------
   *
   * 2001.07.04: 
   * Mongoose - Created
   ------------------------------------------------------*/


  ~Lwo();
  /*------------------------------------------------------
   * Pre  : Lwo object is allocated
   * Post : Deconstructs an object of Lwo
   *
   *-- History ------------------------------------------
   *
   * 2001.07.04: 
   * Mongoose - Created
   ------------------------------------------------------*/

	int VertexCount();
	int MaterialCount();
	int FaceCount();
	void Vertex(int i, float *vertex);
	lw_face_t *Face(int i);
	lw_material_t *Material(int i);

	int Check(const char *filename);
  /*------------------------------------------------------
   * Pre  : 
   * Post : Returns 0 if it's LWO, else returns error
   *
   *-- History ------------------------------------------
   *
   * 2001.07.04: 
   * Mongoose - Created
   ------------------------------------------------------*/

	int Load(const char *filename);
  /*------------------------------------------------------
   * Pre  : 
   * Post : Returns 0 if loaded, else returns error
   *
   *-- History ------------------------------------------
   *
   * 2001.07.04: 
   * Mongoose - Created
   ------------------------------------------------------*/


	float Radius();
  /*------------------------------------------------------
   * Pre  : LWO must me allocated and loaded
   * Post : Returns radius of LWO, else returns 0.0 error
   *
   *-- History ------------------------------------------
   *
   * 2001.07.04: 
   * Mongoose - Created
   ------------------------------------------------------*/


	void Scale(float scale);
  /*------------------------------------------------------
   * Pre  : LWO must me allocated and loaded, scale != 0.0
   * Post : Scales LWO by scale, or does nothing on error
   *
   *-- History ------------------------------------------
   *
   * 2001.07.04: 
   * Mongoose - Created
   ------------------------------------------------------*/

 private:

	void ReadSrfs(FILE *f, int nbytes);
	void ReadSurf(FILE *f, int nbytes);
	void ReadPols(FILE *f, int nbytes);
	void ReadPnts(FILE *f, int nbytes);
	
	int _face_count;             /* Number of polygons */

	lw_face_t *_face;            /* Polygons */

	lw_face_t **_face_map;       /* Map for polygons */

	int _material_count;         /* Number of materials */

	lw_material_t *_material;    /* Materials */

	lw_material_t **_mat_map;    /* Map for materials */

	int _vertex_count;           /* Number of vertices */

	float *_vertex;              /* Vertices */
};

#endif
