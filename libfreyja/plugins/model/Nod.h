/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Lwo
 * License : GPL
 * Comments: NOD module for freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2001.07.04:
 * Mongoose - Created, based on viewer code from anonymous
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_NOD_H
#define GUARD__FREYJA_MONGOOSE_NOD_H

#ifndef color_t
typedef float color_t[4];
#endif

#ifndef vec3_t
typedef float vec3_t[3];
#endif

////////////////////////////////////////

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef word
typedef unsigned short word;
#endif

// Model flags
#define NOD_MF_HASLOD		0x1    /* Model has LOD information */
#define NOD_MF_INLINE		0x2    /* Model was generated from Inline 
												 level editor data */
#define NOD_MF_STATIC		0x4    /* Model does not use skeletal animations 
												 and is just a static rigid object 
												 (faster rendering path) */
#define NOD_MF_RESERVED1	0x8    /* Do not use */
#define NOD_MF_RESERVED2	0x10   /* Do not use */

// Group flags
#define NOD_GF_HASLOD		0x1    /* The mesh group contains level-of-detail 
												 information. */
#define NOD_GF_NOWEIGHTS	0x2    /* The mesh group contains no weighted 
												 vertices */
#define NOD_GF_NOSKINNING	0x4    /* The mesh group contains no skinning. 
												 All vertex bindings are ignored and 
												 replaced by the BoneNum value in the 
												 mesh group definition. */
#define NOD_GF_MULTITEXTURE	0x8 /* The mesh group should use multitexture 
												 vertices for rendering 
												 (NOT USED in 1.1) */

typedef struct
{
	int		Version;			 /* Version integer indication the version 
										 of the file format. Vampire 1.1 uses version 7. */
	int		NumMaterials;	 /* Specifies the number of materials used by 
										 this model. */
} nod_header1_t;


typedef struct
{
	char	MaterialName[32];	  /* Name of each material used. Each material 
										  has a 32-character name. */
} nod_material_t;


typedef struct
{ 
	word NumBones; 			  /* The number of bones defined in the file */
	word NumMeshs;	  		     /* The number of meshes defined in the file */
	int NumVertices;			  /* The number of vertices in the file (all meshes) */
	int NumFaces;	  			  /* The number of faces in the file (all mehes) */ 
	word NumGroups; 			  /* The number of "mesh groups" in the file */ 
	int ModelFlags;			  /* Bitvector of flags for model behavior */ 
	float Bounds[6];			  /* Specifies the axis-alignged bounding box 
										  in model coordinates that bounds the 
										  model's at-rest pose */
} nod_header2_t;


typedef struct
{
	float	RestTranslate[3];			 /* The centerpoint of the bone in
										  * the at-rest pose of the model */

	float	RestMatrixInverse[3][4];	 /* The inverse orientation of the 
										  * bone in the at-rest pose */

	short	SiblingID;					 /* The index of the next "sibling"
 										  * bone, or -1 if this is the last */

	short	ChildID;					 /* The index of the first child of this
										  * bone or -1 if there is no child. */

	short	ParentID;					 /* The index of the parent of this bone
										  * The root bone is always defined 
										  * first so -1 is only valid for that
										  *  bone. */
} nod_bone_t;


typedef struct
{
	char	MeshName[32];
} nod_mesh_t;


typedef struct
{
	float	Pos[3];	  /* This is the location of the vertex in 
							  3D space in its at-rest pose. */
	float	Norm[3];	  /* This is the normal vector at this vertex 
							  in the mesh, used for real-time lighting. */
	float	UV[2];	  /* This is the texture vertex controlling how 
							  the texture in anchored to the mesh at this point. */
	float	Weight;	  /* This is the weighting factor. If the weighting 
							  factor is < 1.0, the vertex is using weighted skinning */
	 int BoneNum;	  /* Which bone (by index) is this vertex bound to. 
							  If the weight is < 1.0, the remaining 
							  weight as assumed to be on "BoneNum" parent. */
} nod_vertex_t;


typedef struct
{
	word indices[3];	  /* Indices into the vertex array */
} nod_face_t;


typedef struct
{
	int MaterialID;		/* This is the index into the material array 
									in the preamble which is used to texture 
									this group. -1 = not texture. */
	byte RESERVED[12];	/* This field should be ignored */
	word NumFaces;			/* Defines how many faces are in this group */
	word NumVertices;		/* How many vertices are in this group */
	word MinVertices;		/* How many vertices are in the minimum-LOD set 
									for this group (invalid but present if LOD 
									not indicated) */
	word dummy;
	word GroupFlags;		/* A bitvector to control per-group rendering behavior */
	byte BoneNum;			/* Defines binding of the entire group to a
									single bone only if NOSKINNING bit is set, 
									always present in the file */ 
	byte MeshNum;			/* Defines which mesh (array of names) this group
									is a member of. Used simple to map older 
									mesh structures to newer MeshGroup structures */
} nod_meshgroup_t;


class Nod
{
 public:

  Nod();
  /*------------------------------------------------------
   * Pre  : 
   * Post : Constructs an object of Lwo
   *
   *-- History ------------------------------------------
   *
   * 2001.07.04: 
   * Mongoose - Created
   ------------------------------------------------------*/


  ~Nod();
  /*------------------------------------------------------
   * Pre  : Lwo object is allocated
   * Post : Deconstructs an object of Lwo
   *
   *-- History ------------------------------------------
   *
   * 2001.07.04: 
   * Mongoose - Created
   ------------------------------------------------------*/

	void GetEulerAngles2(const float m[3][4], float angles[3]);

	void print();

	bool load(const char *filename);

	nod_header1_t header1;
	nod_material_t *materials;
	nod_header2_t header2;
	nod_bone_t *bones;
	nod_mesh_t *meshes;
	nod_vertex_t *vertices;
	nod_meshgroup_t *mesh_groups;
	nod_face_t *faces;
};
#endif
