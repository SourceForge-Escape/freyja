/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : MDDC 
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : Mdl
 * Comments: This is the MDL model class.
 *
 *           See file COPYING for license details.
 *
 *           Quake (c) id Software 1995 
 * 
 *-- History ---------------------------------------------------------- 
 *
 * 2004.04.22:
 * Mongoose - RGB Texture generation and internal palettes
 *
 * 2000-10-12:
 * Mongoose - Updated max verts
 *
 * See the source file's header.
 ==========================================================================*/


#ifndef GUARD__QUAKEFORGE_MONGOOSE_MDL_H
#define GUARD__QUAKEFORGE_MONGOOSE_MDL_H

/* Model general defines */
#define MDL_IDPOLYGON         0x4F504449
#define MDL_ALIAS_VERSION     6
#define MDL_ALIAS_ONSEAM      0x0020
#define MDL_DT_FACES_FRONT    0x0010

/* Model limits */
#define MDL_MAXALIASTRIS      2048
#define MDL_MAXALIASVERTS     2048
#define MDL_MAXALIASFRAMES    256
#define MDL_MAX_SKINS         100

/* Model effects */
#define MDL_EF_ROCKET         1          // Leave a trail
#define MDL_EF_GRENADE        2          // Leave a trail
#define MDL_EF_GIB            4          // Leave a trail
#define MDL_EF_ROTATE         8          // Rotate (bonus items)
#define MDL_EF_TRACER         16         // Green split trail
#define MDL_EF_ZOMGIB         32         // Small blood trail
#define MDL_EF_TRACER2        64         // Orange split trail + rotate
#define MDL_EF_TRACER3        128        // Purple trail

#ifndef _ID_VECS
#define _ID_VECS
typedef float  vec_t;
typedef vec_t  vec3_t[3];
typedef vec_t  vec5_t[5];
#endif

typedef enum { ALIAS_SINGLE=0, ALIAS_GROUP } aliasframetype_t;

typedef enum { ALIAS_SKIN_SINGLE=0, ALIAS_SKIN_GROUP } aliasskintype_t;

typedef enum 
{ 
   ST_SYNC = 0, 
   ST_RAND 
} synctype_t;

typedef struct 
{
   int             onseam;          /* 0 or 0x20 */ 
   int             s;               /* Position (X), horizontally  
                                          in range [0,skinwidth] */
   int             t;               /* Position (Y), vertically 
                                          in range [0,skinheight] */
} stvert_t;

typedef struct dtriangle_s 
{
   int   facesfront;
   int   vertindex[3];

} dtriangle_t;

/// Modified id structs //////////////////////////////

typedef struct trivertex_s
{
   unsigned char    v[3];
   unsigned char    lightnormalindex;

} trivertex_t;

/// Below are non-id structs - move to using id later////////////////////

typedef struct MDL_GSkin_s
{
   unsigned char *pixmap;

} MDL_GSkin_t;

typedef struct MDL_Skin_s
{
   unsigned int type;
   unsigned char *pixmap;
   unsigned int num_gskins;
   float *gskin_interval;
   MDL_GSkin_t *gskin;

} MDL_Skin_t;

typedef struct MDL_MFrame_s
{
   trivertex_t   bboxmin;   /* lightnormal isn't used */
   trivertex_t   bboxmax;   /* lightnormal isn't used */
   char          name[16];  /* frame name from grabbing */
   trivertex_t  *tv;

} MDL_MFrame_t;

typedef struct MDL_MFrame_Group_s
{
   unsigned num_gsframes;
   trivertex_t   bboxmin;   /* lightnormal isn't used */
   trivertex_t   bboxmax;   /* lightnormal isn't used */
   float *gsframe_interval;
   MDL_MFrame_t *gsframe;

} MDL_MFrame_Group_t;

typedef struct MDL_Frame_s
{
   unsigned type;
   MDL_MFrame_Group_t  gframe;
   MDL_MFrame_t        sframe;

} MDL_Frame_t;

typedef struct MDL_s
{
   int      ident;           /* 0x4F504449 = "IDPO" for IDPOLYGON */
   int      version;         /* Version = 6 */
   vec3_t   scale;           /* Model scale factors. */
   vec3_t   scale_origin;    /* Model origin. */
   float    boundingradius;  /* Model bounding radius.*/
   vec3_t   eyeposition;     /* Eye position (useless?) */
   int      numskins;        /* the number of skin textures */
   int      skinwidth;       /* Width of skin texture 
                                         must be multiple of 8 */
   int      skinheight;      /* Height of skin texture
                                         must be multiple of 8 */
   int      numverts;        /* Number of vertices */
   int      numtris;         /* Number of triangles surfaces */
   int      numframes;       /* Number of frames */
   int      synctype;        /* 0 = synchron, 1 = random */
   int      flags;           /* 0 (see Alias models) */
   float    size;            /* average size of triangles */
} MDL_t;

class Mdl
{

public:

	typedef enum {
		Quake = 1,
		Hexen2

	} palette_type;

   Mdl();

   Mdl(char *filename);

   ~Mdl();

   void Header(MDL_t *header);

   MDL_t *Header();

   stvert_t *Vertex();

   void Vertex(stvert_t *vertex);

   dtriangle_t *Triangle();

   void Triangle(dtriangle_t *triangle);

	unsigned char *getTextureRGB24(palette_type type, int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns either a quake or hexen2 palette based
	 *        image based on internal indexed texture
	 *
	 *        image is RGB and 24bpp
	 *
	 *        You must free the returned image yourself
	 *        when done
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int getTextureWidth();
	
	int getTextureHeight();

   void Skin(MDL_Skin_t *skin);

   MDL_Frame_t *Frame();

   void Frame(MDL_Frame_t *frame);

   void Sign(bool set);

   int Save(char *filename);

   int Load(char *filename);

private:

   static unsigned char mPaletteQuake[768];
   static unsigned char mPaletteHexen2[768];

   /* Mirror of Mdl_t struct, for convence inside class */

   int             _ident;           /* 0x4F504449 = "IDPO" for IDPOLYGON */
   int             _version;         /* Version = 6 */
   vec3_t          _scale;           /* Model scale factors. */
   vec3_t          _scale_origin;    /* Model origin. */
   float           _boundingradius;  /* Model bounding radius.*/
   vec3_t          _eyeposition;     /* Eye position (useless?) */
   int             _numskins;        /* the number of skin textures */
   int             _skinwidth;       /* Width of skin texture 
                                         must be multiple of 8 */
   int             _skinheight;      /* Height of skin texture
                                         must be multiple of 8 */
   int             _numverts;        /* Number of vertices */
   int             _numtris;         /* Number of triangles surfaces */
   int             _numframes;       /* Number of frames */
   int             _synctype;        /* 0 = synchron, 1 = random */
   int             _flags;           /* 0 (see Alias models) */
   float           _size;            /* average size of triangles */

   stvert_t *_vertex;                /* Vertex list */

   dtriangle_t *_triangle;           /* Triangle list */

   MDL_Skin_t *_skin;                /* Model skin(s) */

   MDL_Frame_t *_frame;              /* Model frame(s) */

   bool _sign;                       /* Toggle signing on MDL saves */
};

#endif
