/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Ase
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: ASE model class (For single mesh/triangle polygon ASE)
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_ASE - Builds Ase class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.03.23:
 * Mongoose - Skeletal support via 'homemade' extention
 *            Freyja Modeler hooks to avoid need for wrapper
 *
 * 2004.03.22:
 * Mongoose - Created, replace aging class with better API
 *            New file I/O system
 *
 * 2004.03.21:
 * Mongoose - New exporter fixes, improved ASE spec support 
 *
 * 2000.10.15:
 * Mongoose - Created, ASE specs provided by nate miller
 ================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_ASE_H_
#define GUARD__FREYJA_MONGOOSE_ASE_H_

#include <stdlib.h>
#include <stdio.h>

#define NUM_VERTEX		"*MESH_NUMVERTEX"
#define NUM_FACES		"*MESH_NUMFACES"
#define NUM_TVERTEX		"*MESH_NUMTVERTEX"
#define NUM_TFACES		"*MESH_NUMTVFACES"
#define VERTEX_LIST		"*MESH_VERTEX_LIST"
#define VERTEX			"*MESH_VERTEX"
#define FACE_LIST		"*MESH_FACE_LIST"
#define FACE			"*MESH_FACE"
#define NORMALS			"*MESH_NORMALS"
#define FACE_NORMAL		"*MESH_FACENORMAL"
#define NVERTEX			"*MESH_VERTEXNORMAL"
#define TVERTEX			"*MESH_TVERT"
#define TFACE			"*MESH_TFACE"
#define TEXTURE			"*BITMAP"
#define UTILE			"*UVW_U_TILING"
#define VTILE			"*UVW_V_TILING"
#define UOFFSET			"*UVW_U_OFFSET"
#define VOFFSET			"*UVW_V_OFFSET"

#ifndef vec_t
typedef float vec_t;
#endif

#ifndef vec2_t
typedef vec_t vec2_t[2];
#endif

#ifndef vec3_t
typedef vec_t vec3_t[3];
#endif

#ifndef vec4_t
typedef vec_t vec4_t[4];
#endif


typedef struct
{
	int vertIndex[3];  /* Indicies for the verts that make up this triangle */
	int uvwIndex[3];   /* Indicies for the tex coords to texture this face */
	vec3_t normal;     /* Face normal */

} ase_triangle_t;


typedef struct 
{
	char name[255];    /* Texture name */
	int id;            /* Texture id */
	vec_t uTile;       /* U tiling of texture */
	vec_t vTile;       /* V tiling of texture */
	vec_t uOffset;     /* U offset of texture */
	vec_t vOffset;     /* V offset of texture */

} ase_texture_t;


/* EXT are not part of ASE, but extentions I made for conveince
 * for skeletal animation modelers and animators */
typedef struct ase_weight_EXT_s
{
	vec_t weight;
	int boneId;

	struct ase_weight_EXT_s *next;

} ase_weight_EXT_t;


typedef struct ase_bone_EXT_s
{
	char name[64];
	unsigned int id;
	unsigned int parent;
	unsigned int childrenCount; 
	unsigned int *children;

	vec3_t translation;     /* XYZ */
	vec4_t rotation;        /* XYZW Quaterion*/

} ase_bone_EXT_t;


class AseSkelEXT
{
public:

	AseSkelEXT();

	~AseSkelEXT();

	void print();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Prints out description of AseSkelEXT
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int load(char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string
	 * Post : Returns 0 if load is successful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int save(char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string
	 * Post : Returns 0 if load is successful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	char mName[64];
	unsigned int mBoneCount;
	unsigned int mWeightCount;
	
	ase_weight_EXT_t *mWeights;
	ase_bone_EXT_t *mBones;
};


class Ase
{
 public:

	enum {
		fShowInfoOnLoad = 1,
		fWorldCoordXYZ  = 2

	} AseFlags;

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Ase();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Ase
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.22: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Ase();
	/*------------------------------------------------------
	 * Pre  : Ase object is allocated
	 * Post : Deconstructs an object of Ase
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.22: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	int check(char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string
	 * Post : Returns 0 if file is readable and ASE header
	 *        is found
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.23:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void printInfo();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Prints information about model to stdout
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void erase();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deletes all member data
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int load(char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string
	 * Post : Returns 0 if load is successful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int save(char *filename);
	/*------------------------------------------------------
	 * Pre  : <filename> is valid string
	 * Post : Returns 0 if save is successful
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int mFlags;    /* Options for class */

	int mMaterialCount;     /* Number of materials */

	int mVertexCount;       /* Number of vertices */

	int mFaceCount;         /* Number of faces */

	int mTexFacesCount;     /* Number of textured faces in model */

	int mUVWCount;          /* Number of texture coords */

	int mNormalCount;       /* Not really always a counter, boolean  */ 

	vec3_t *mVertices;      /* Model vertices */

	vec3_t *mUVWs;          /* Texture coordinates */

	vec3_t *mNormals;       /* Vertex normals  */

	ase_triangle_t *mFaces; /* Faces of the model */

	ase_texture_t mTexture; /* Texture information */

	AseSkelEXT skel;

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	/* In 3ds Max Y and Z are reversed, and the Z is negative */
	void convertWorldXYZ(vec3_t xzy);

	void convertWorld3dsMax(vec3_t xyz);

	void readSymbol(FILE *f, char *s);

	int readIntASCII(FILE *f);

	vec_t readFloatASCII(FILE *f);

	void readVertex(FILE *f);

	void readUVW(FILE *f);

	void readNormal(FILE *f);

	void readFaceNormal(FILE *f);

	void readFace(FILE *f);

	void readTexFace(FILE *f);

	void readModelInfo(FILE *f);

	void readModelData(FILE *f);

	void readTextureName(FILE *f);
};

#endif
