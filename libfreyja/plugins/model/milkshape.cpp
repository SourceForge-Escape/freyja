/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : 
 * Comments: This is the Milkshape plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2004.12.25:
 * Mongoose - Created, specs from MilkShape 3D 1.4.0 File Format Specification
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include <freyja/FreyjaPlugin.h>
#include <freyja/FreyjaFileReader.h>


extern "C" {

	int freyja_model__milkshape_check(char *filename);
	int freyja_model__milkshape_import(char *filename);
	int freyja_model__milkshape_export(char *filename);
	int import_model(char *filename);
}

#define MAX_VERTICES    8192
#define MAX_TRIANGLES   16384
#define MAX_GROUPS      128
#define MAX_MATERIALS   128
#define MAX_JOINTS      128
#define MAX_KEYFRAMES   216     // increase when needed

/* Flags */
typedef enum
{ 
	SELECTED       = 1,
	HIDDEN         = 2,
	SELECTED2      = 4,
	DIRTY          = 8
} ms3d_flags_t;

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef word
typedef unsigned short word;
#endif


typedef struct ms3d_header_s
{
	char id[10];          // Always "MS3D000000"
	long version;         // 3

}  ms3d_header_t;


typedef struct ms3d_vertex_s
{
	byte flags;           // SELECTED | SELECTED2 | HIDDEN
	float vertex[3];
	char boneId;          // -1 = no bone
	byte refCount;

}  ms3d_vertex_t;


typedef struct ms3d_triangle_s
{
	word flags;           // SELECTED | SELECTED2 | HIDDEN
	word vertexIndices[3];
	float vertexNormals[3][3];
	float s[3];
	float t[3];
	byte smoothingGroup;  // 1 - 32
	byte groupIndex;

}  ms3d_triangle_t;


typedef struct ms3d_group_s {

	byte flags;                            // SELECTED | HIDDEN
	char name[32];                         //
	word numtriangles;                     //
	word *triangleIndices;//[numtriangles] // the groups group the triangles
	char materialIndex;                    // -1 = no material

} ms3d_group_t;


typedef struct ms3d_material_s
{
	char name[32];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emissive[4];
	float shininess;	    // 0.0f - 128.0f
	float transparency;	 // 0.0f - 1.0f
	byte mode;	          // 0, 1, 2 is unused now
	char texture[128];    // texture.bmp
	char alphamap[128];   // alpha.bmp

}  ms3d_material_t;


typedef struct ms3d_keyframe_rot_s
{
	float time;             // time in seconds
	float rotation[3];      // x, y, z angles

} ms3d_keyframe_rot_t;


typedef struct ms3d_keyframe_pos_s
{
	float time;             // time in seconds
	float position[3];      // local position

} ms3d_keyframe_pos_t;


typedef struct ms3d_joint_s
{
	byte flags;
	char name[32];
	char parentName[32];
	float rotation[3];
	float position[3];
	word numRotationKeyframes;
	word numPositionKeyframes;

	ms3d_keyframe_rot_t *keyFramesRot;
	ms3d_keyframe_pos_t *keyFramesPos;

}  ms3d_joint_t;


class Ms3dModel
{
public:
	Ms3dModel()
	{
		vertices = 0x0;
		tris = 0x0;
		groups = 0x0;
		materials = 0x0;
		joints = 0x0;
	}

	~Ms3dModel()
	{
		if (vertices) delete [] vertices;
		if (tris) delete [] tris;

		if (groups)
		{
			long i;
			for (i = 0; i < nNumGroups; ++i)
			{
				if (groups[i].triangleIndices) delete [] groups[i].triangleIndices;
			}

			delete [] groups;
		}

		if (materials) delete [] materials;

		if (joints)
		{
			long i;
			for (i = 0; i < nNumJoints; ++i)
			{
				if (joints[i].keyFramesRot) delete [] joints[i].keyFramesRot;
				if (joints[i].keyFramesPos) delete [] joints[i].keyFramesPos;
			}

			delete [] joints;
		}
	}

	ms3d_header_t header;
	word nNumVertices;
	ms3d_vertex_t *vertices;
	word nNumTriangles;
	ms3d_triangle_t *tris;
	word nNumGroups;
	ms3d_group_t *groups;
	word nNumMaterials;
	ms3d_material_t *materials;
	float fAnimationFPS;
	float fCurrentTime;
	long iTotalFrames;
	word nNumJoints;
	ms3d_joint_t *joints;
};


//
// Mesh Transformation:
// 
// 0. Build the transformation matrices from the rotation and position
// 1. Multiply the vertices by the inverse of local reference matrix (lmatrix0)
// 2. then translate the result by (lmatrix0 * keyFramesTrans)
// 3. then multiply the result by (lmatrix0 * keyFramesRot)
//
// For normals skip step 2.
//
// NOTE:  this file format may change in future versions!
//
// - Mete Ciragan
//

//////////////////////////////////////////////////////////////////////


int import_model(char *filename)
{
	
	if (!freyja_model__milkshape_check(filename))
		return freyja_model__milkshape_import(filename);

	return -1;
}


int freyja_model__milkshape_check(char *filename)
{
	FILE *f;
	ms3d_header_t header;


	f = fopen(filename, "rb");

	if (!f)
	{
		//perror("milkshape_check> fopen failed");
		return -1;
	}

	fread(&header.id, 10, 1, f);
	fread(&header.version, 4, 1, f);
	fclose(f);

	/* MilkShape 1.3 or 1.4 model check */
	if (!strncmp(header.id, "MS3D000000", 10) &&  header.version == 3)
		return 0;

	return -2;
}


int freyja_model__milkshape_import(char *filename)
{
	const vec_t scale = 0.4f; 
	FreyjaFileReader r;
	Ms3dModel mdl;
	long i, j, k;


	if (freyja_model__milkshape_check(filename))
		return -1;

	if (!r.openFile(filename))
	{
		//perror("milkshape_import> fopen failed");
		return -2;
	}

	r.readCharString(10, mdl.header.id);
	mdl.header.version = r.readLong();

	if (strncmp(mdl.header.id, "MS3D000000", 10) || mdl.header.version != 3)
	{
		r.closeFile();
		return -1;
	}

	mdl.nNumVertices = r.readInt16U();
	mdl.vertices = new ms3d_vertex_t[mdl.nNumVertices];

	for (i = 0; i < mdl.nNumVertices; ++i)
	{
		mdl.vertices[i].flags = r.readInt8U();
		mdl.vertices[i].vertex[0] = r.readFloat32();
		mdl.vertices[i].vertex[1] = r.readFloat32();
		mdl.vertices[i].vertex[2] = r.readFloat32();
		mdl.vertices[i].boneId = r.readInt8();
		mdl.vertices[i].refCount = r.readInt8U();
	}

	mdl.nNumTriangles = r.readInt16U();
	mdl.tris = new ms3d_triangle_t[mdl.nNumTriangles];
	
	for (i = 0; i < mdl.nNumTriangles; ++i)
	{
		mdl.tris[i].flags = r.readInt8U();
		mdl.tris[i].vertexIndices[0] = r.readInt8U();
		mdl.tris[i].vertexIndices[1] = r.readInt8U();
		mdl.tris[i].vertexIndices[2] = r.readInt8U();
	
		for (j = 0; j < 3; ++j)
		{
			for (k = 0; k < 3; ++k)
			{
				mdl.tris[i].vertexNormals[j][k] = r.readFloat32();
			}
		}

		mdl.tris[i].s[0] = r.readFloat32();
		mdl.tris[i].s[1] = r.readFloat32();
		mdl.tris[i].s[2] = r.readFloat32();
		mdl.tris[i].t[0] = r.readFloat32();
		mdl.tris[i].t[1] = r.readFloat32();
		mdl.tris[i].t[2] = r.readFloat32();
		mdl.tris[i].smoothingGroup = r.readInt8U();
		mdl.tris[i].groupIndex = r.readInt8U();
	}

	mdl.nNumGroups = r.readInt16U();
	mdl.groups = new ms3d_group_t[mdl.nNumGroups];
	
	for (i = 0; i < mdl.nNumGroups; ++i)
	{
		mdl.groups[i].flags = r.readInt8U();
		r.readCharString(32, mdl.groups[i].name);
		mdl.groups[i].numtriangles = r.readInt16U();

		mdl.groups[i].triangleIndices = new word[mdl.groups[i].numtriangles];

		for (j = 0; j < mdl.groups[i].numtriangles; ++j)
			mdl.groups[i].triangleIndices[j] = r.readInt16U();

		mdl.groups[i].materialIndex = r.readInt8();
	}


	mdl.nNumMaterials = r.readInt16U();
	mdl.materials = new ms3d_material_t[mdl.nNumMaterials];
	
	for (i = 0; i < mdl.nNumMaterials; ++i)
	{
		r.readCharString(32, mdl.materials[i].name);
		mdl.materials[i].ambient[0] = r.readFloat32();
		mdl.materials[i].ambient[1] = r.readFloat32();
		mdl.materials[i].ambient[2] = r.readFloat32();
		mdl.materials[i].ambient[3] = r.readFloat32();
		mdl.materials[i].diffuse[0] = r.readFloat32();
		mdl.materials[i].diffuse[1] = r.readFloat32();
		mdl.materials[i].diffuse[2] = r.readFloat32();
		mdl.materials[i].diffuse[3] = r.readFloat32();
		mdl.materials[i].specular[0] = r.readFloat32();
		mdl.materials[i].specular[1] = r.readFloat32();
		mdl.materials[i].specular[2] = r.readFloat32();
		mdl.materials[i].specular[3] = r.readFloat32();
		mdl.materials[i].emissive[0] = r.readFloat32();
		mdl.materials[i].emissive[1] = r.readFloat32();
		mdl.materials[i].emissive[2] = r.readFloat32();
		mdl.materials[i].emissive[3] = r.readFloat32();
		mdl.materials[i].shininess = r.readFloat32();
		mdl.materials[i].transparency = r.readFloat32();
		mdl.materials[i].mode = r.readInt8();
		r.readCharString(128, mdl.materials[i].texture);
		r.readCharString(128, mdl.materials[i].alphamap);
	}

	mdl.fAnimationFPS = r.readFloat32();
	mdl.fCurrentTime = r.readFloat32();
	mdl.iTotalFrames = r.readLong();

	mdl.nNumJoints = r.readInt16U();
	mdl.joints = new ms3d_joint_t[mdl.nNumJoints];

	for (i = 0; i < mdl.nNumJoints; ++i)
	{
		mdl.joints[i].flags = r.readInt8();
		r.readCharString(32, mdl.joints[i].name);
		r.readCharString(32, mdl.joints[i].parentName);
		mdl.joints[i].rotation[0] = r.readFloat32();
		mdl.joints[i].rotation[1] = r.readFloat32();
		mdl.joints[i].rotation[2] = r.readFloat32();
		mdl.joints[i].position[0] = r.readFloat32();
		mdl.joints[i].position[1] = r.readFloat32();
		mdl.joints[i].position[2] = r.readFloat32();
		mdl.joints[i].numRotationKeyframes = r.readInt16U();
		mdl.joints[i].keyFramesRot = new ms3d_keyframe_rot_t[mdl.joints[i].numRotationKeyframes];
		mdl.joints[i].numPositionKeyframes = r.readInt16U();
		mdl.joints[i].keyFramesPos = new ms3d_keyframe_pos_t[mdl.joints[i].numPositionKeyframes];

		for (j = 0; j < mdl.joints[i].numRotationKeyframes; ++j)
		{
			mdl.joints[i].keyFramesRot[j].time = r.readFloat32();
			mdl.joints[i].keyFramesRot[j].rotation[0] = r.readFloat32();
			mdl.joints[i].keyFramesRot[j].rotation[1] = r.readFloat32();
			mdl.joints[i].keyFramesRot[j].rotation[2] = r.readFloat32();
		}

		for (j = 0; j < mdl.joints[i].numPositionKeyframes; ++j)
		{
			mdl.joints[i].keyFramesPos[j].time = r.readFloat32();
			mdl.joints[i].keyFramesPos[j].position[0] = r.readFloat32();
			mdl.joints[i].keyFramesPos[j].position[1] = r.readFloat32();
			mdl.joints[i].keyFramesPos[j].position[2] = r.readFloat32();
		}
	}

	r.closeFile();


	Vector<long> transV;
	long index;

	
	freyjaBegin(FREYJA_MODEL);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);
	
	for (i = 0; i < mdl.nNumVertices; ++i)
	{
		index = freyjaVertex3f(mdl.vertices[i].vertex[0]*scale,
									  mdl.vertices[i].vertex[1]*scale,
									  mdl.vertices[i].vertex[2]*scale);
		freyjaVertexWeight(index, 1.0f, mdl.vertices[i].boneId);
		transV.pushBack(index);
	}

	for (i = 0; i < mdl.nNumTriangles; ++i)
	{			
		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonMaterial1i(index);

		for (j = 0; j < 3; ++j)
		{
			index = transV[mdl.tris[i].vertexIndices[j]];
			freyjaPolygonVertex1i(index);
			freyjaVertexNormal3f(index, 
										mdl.tris[i].vertexNormals[j][0],
										mdl.tris[i].vertexNormals[j][1],
										mdl.tris[i].vertexNormals[j][2]);
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(mdl.tris[i].s[j],
																  mdl.tris[i].t[j]));

		}
		freyjaEnd(); // FREYJA_POLYGON
	}
		
	freyjaEnd(); // FREYJA_GROUP
	freyjaEnd(); // FREYJA_MESH

	freyjaBegin(FREYJA_SKELETON);
	
	for (i = 0; i < mdl.nNumJoints; ++i)
	{
		freyjaBegin(FREYJA_BONE);
		freyjaBoneFlags1u(0x0);
		freyjaBoneName(mdl.joints[i].name);
		freyjaBonePos3f(mdl.joints[i].position[0]*scale,
							 mdl.joints[i].position[1]*scale,
							 mdl.joints[i].position[2]*scale);
		freyjaBoneRotate3f(mdl.joints[i].rotation[0],
								 mdl.joints[i].rotation[1],
								 mdl.joints[i].rotation[2]);

		for (j = 0; j < mdl.nNumJoints; ++j)
		{
			if (!strncmp(mdl.joints[i].name, mdl.joints[j].parentName, 32))
			{
				freyjaBoneAddChild1u(j);
			}
			
			if (!strncmp(mdl.joints[i].parentName, mdl.joints[j].name, 32))
			{
				freyjaBoneParent(j);
			}
		}

		freyjaEnd(); // FREYJA_BONE
	}

	freyjaEnd(); // FREYJA_SKELETON

	freyjaEnd(); // FREYJA_MODEL

	return 0;
}


int freyja_model__milkshape_export(char *filename)
{
	printf("freyja_model__milkshape_export> Not implemented, %s:%i\n", 
			 __FILE__, __LINE__);
	return -1;
}
