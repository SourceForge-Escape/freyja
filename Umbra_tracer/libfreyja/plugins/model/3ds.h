/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://gooseegg.sourceforge.net
 * Email   : stu7440@westga.edu
 * Object  : 
 * Comments: This is the 3DS class
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2004.04.27:
 * Mongoose - Created, port and fix of Load 3ds by Alexi Leontopolis, WVU VEL
 ==========================================================================*/


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

using namespace std;

#define MAXLEN		256
#define MAIN_CHUNK		0x4D4D
#define VERSION_3DS		0x0002
#define	ONE_UNIT		0x0100
#define BG_BMAP			0x1100
#define USE_BGBMAP		0x1101
#define BG_COLOR		0x1200
#define USE_BGCOLOR		0x1201

#define EDITOR			0x3D3D
#define MESH_VERSION    0x3D3E

#define OBJECT_BLOCK	0x4000
#define OBJECT_HIDDEN	0x4010
#define OBJECT_NOCAST	0x4012
#define OBJECT_MATTE	0x4013
#define TRIANGLE_MESH		0x4100
#define VERTEX_LIST			0x4110
#define FACE_LIST			0x4120
#define FACE_MAT_LIST		0x4130	
#define VERTEX_MAP_COORD	0x4140
#define LOCAL_COORD			0x4160

#define MATERIAL_EDITOR		0xAFFF
#define MATERIAL_NAME		0xA000
#define MATERIAL_AMBIENT	0xA010
#define MATERIAL_DIFFUSE	0xA020
#define MATERIAL_SPECULAR	0xA030
#define MATERIAL_SHINE_PER	0xA040
#define MATERIAL_SHINE_STR	0xA041
#define MATERIAL_TRANS_PER	0xA050
#define MATERIAL_TEXTURE1	0xA200
#define MATERIAL_TEXTURE2	0xA33A

#define MAPPING_FILENAME	0xA300
#define V_SCALE				0xA354 
#define U_SCALE				0xA356

typedef unsigned long	dword;
typedef unsigned char	byte;
typedef unsigned short	word;

class Material;
///////////////////////////////////////////////////////////////////
typedef struct 
{
	float x;				// X coordinate
	float y;				// Y coordinate
	float z;				// Z coordinate
} XYZ3DS;

///////////////////////////////////////////////////////////////////
typedef struct
{
	word	vcA;			// Vertex for corner A
	word    vcB;			// Vertex for corner B
	word	vcC;			// Vertex for corner C
	word	fFlag;			// Face Flag
							//	* bit 0: CA visible
							//	* bit 1: BC visible
							//  * bit 2: AB visible
	char	matName[MAXLEN];
	Material *material;
} Face3DS;
///////////////////////////////////////////////////////////////////
typedef struct
{
	float U;				// U coordinate
	float V;				// V coordinate
} MatMap;

///////////////////////////////////////////////////////////////////
typedef struct
{
	byte r;
	byte g;
	byte b;
} RGBByte;
///////////////////////////////////////////////////////////////////
typedef struct
{
	float r;
	float g;
	float b;
} RGBFloat;

///////////////////////////////////////////////////////////////////
typedef struct
{
	char	filename[MAXLEN];
	short	MapParams;
	float	blur;
	float	UScale;
	float	VScale;
	float	UOffset;
	float	VOffset;
	float	RotAngle;
} SubMap;

///////////////////////////////////////////////////////////////////


class Object3d
{

public:
	char	name[MAXLEN];	// Name of the 3D Object
	byte	color;			// Color in 3D Editor
	Object3d *next;

	//Vertex Info
	word	nVertices;		// Number of vertices
	XYZ3DS  *vertex;		// List of vertices

	//Local Coordinates
	XYZ3DS X1;				// Represents Axis
	XYZ3DS X2;				// ""    ""    ""
	XYZ3DS X3;				// ""    ""    ""
	XYZ3DS O;				// Represents Origin
	
	//Face3DS Info
	word	nFaces;			// Number of faces
	Face3DS	*face;			// List of faces
	MatMap	*matmap;		// UV coordinates 	

	//Min and Max XYZ3DS
	XYZ3DS min;				//Not a vertex but contains info on min boundary
	XYZ3DS max;				//Not a vertex but contains info on max boundary
	

	Object3d();
	~Object3d();

	void ParseMeshBlock		(long len, ifstream *fin);
};

class Material
{
public:
	Material	*next;

	char		name[MAXLEN];
	RGBByte		ambient;
	RGBByte		diffuse;
	RGBByte		specular;
	word		MatShinPer;
	word		MatShinStr;
	word		MatTransPer;	
	SubMap		texture1;

	Material();
};

//////////////////////////////////////////////////////////////////

class File3ds 
{
protected:
	ifstream	fin;			// file stream for reading input
	
	long		fsize;			// file size
	dword		version;		// 3DS version
	word		nObjects;		// Number of objects
	Object3d	*headObject;	// Pointer to first object in linked list
	Object3d	*tailObject;	// "        " last  "         "         "
	Object3d	*object;		// Pointer to current 3D object

	word		nMaterials;		// Number of materials
	Material	*headMaterial;	// Pointer to first Material in linked list
	Material	*tailMaterial;	// "        " last  "         "         "
	Material	*material;		// Pointer to current 3D Material
	dword		mversion;		// mesh version


	void Parse3dsMain			();
	void Parse3dsEditor			(long len);
	void ParseObjectBlock		(long len);
	void ParseMaterialBlock		(long len);
	void ParseSubMaterialBlock	(long len);
	void ParseSubMapBlock		(long len, SubMap *sm);
	void NewObject				();
	void NewMaterial			();
	
public:
	File3ds();
	File3ds(char *fname);
	~File3ds();

	word		numObjects		()	{return nObjects;};	
	word		numMaterials	()	{return nMaterials;};
	Material*   getMaterialHead ()	{return headMaterial;};

	word		numFaces		(word object);
	word		numVertices		(word object);
	
	XYZ3DS		getOrigin		(word object);
	XYZ3DS		getMidpoint		(word object);
	char* 		getName			(word object);
	Face3DS*	getFaceHead		(word object);
	XYZ3DS*		getVertexHead	(word object);
	MatMap*		getMatMap		(word object);
	Object3d* 	getObject		(word index);
	Material*	getMaterial		(char *name);
	
	void		DeleteMaterial  (Material *material);
	void		DeleteObject	(Object3d *object);


};

void AssignMat(Face3DS *f);
void XYZ3DSInit(XYZ3DS *v);

