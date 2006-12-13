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
#include <freyja/PluginABI.h>
#include <freyja/SkeletonABI.h>
#include <freyja/ModelABI.h>
#include <freyja/MeshABI.h>
#include <freyja/BoneABI.h>
#include <freyja/MaterialABI.h>
#include <mstl/SystemIO.h>
#include <mstl/Vector.h>
#include <hel/Vector3d.h>


using namespace mstl;

extern "C" {

	int freyja_model__milkshape_check(char *filename);
	int freyja_model__milkshape_import(char *filename);
	int freyja_model__milkshape_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginDescription1s("Milkshape Model (*.ms3d)");
	freyjaPluginAddExtention1s("*.ms3d");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
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

		nNumJoints = 0;
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
	SystemIO::FileReader r;
	long version;
	char buf[12];

	if (!r.Open(filename))
	{
		//perror("milkshape_check> fopen failed");
		return -1;
	}

	r.ReadString(10, buf);
	version = r.ReadLong();
	r.Close();

	/* MilkShape 1.3 or 1.4 model check */
	if (!strncmp(buf, "MS3D000000", 10) && version == 3 || version == 4)
		return 0;

	return -2;
}


int freyja_model__milkshape_import(char *filename)
{
	const vec_t scale = 0.4f; 
	SystemIO::BufferedFileReader r;
	Ms3dModel mdl;
	long i, j, k;


	if (freyja_model__milkshape_check(filename))
		return -1;

	if (!r.Open(filename))
	{
		MSTL_MSG("Failed to open file '%s'", filename);
		return -2;
	}

	r.ReadString(10, mdl.header.id);
	mdl.header.version = r.ReadLong();

	if (strncmp(mdl.header.id, "MS3D000000", 10) || 
		!(mdl.header.version == 3 || mdl.header.version == 4))
	{
		MSTL_MSG("Invalid header '%s' %i\n", mdl.header.id, mdl.header.version);
		r.Close();
		return 0;//-1;
	}

	mdl.nNumVertices = r.ReadInt16U();
	mdl.vertices = new ms3d_vertex_t[mdl.nNumVertices];

	for (i = 0; i < mdl.nNumVertices; ++i)
	{
		mdl.vertices[i].flags = r.ReadInt8U();
		mdl.vertices[i].vertex[0] = r.ReadFloat32();
		mdl.vertices[i].vertex[1] = r.ReadFloat32();
		mdl.vertices[i].vertex[2] = r.ReadFloat32();
		mdl.vertices[i].boneId = r.ReadInt8();
		mdl.vertices[i].refCount = r.ReadInt8U();
	}

	mdl.nNumTriangles = r.ReadInt16U();
	mdl.tris = new ms3d_triangle_t[mdl.nNumTriangles];
	
	for (i = 0; i < mdl.nNumTriangles; ++i)
	{
		mdl.tris[i].flags = r.ReadInt16();
		mdl.tris[i].vertexIndices[0] = r.ReadInt16();
		mdl.tris[i].vertexIndices[1] = r.ReadInt16();
		mdl.tris[i].vertexIndices[2] = r.ReadInt16();
	
		for (j = 0; j < 3; ++j)
		{
			for (k = 0; k < 3; ++k)
			{
				mdl.tris[i].vertexNormals[j][k] = r.ReadFloat32();
			}
		}

		mdl.tris[i].s[0] = r.ReadFloat32();
		mdl.tris[i].s[1] = r.ReadFloat32();
		mdl.tris[i].s[2] = r.ReadFloat32();
		mdl.tris[i].t[0] = r.ReadFloat32();
		mdl.tris[i].t[1] = r.ReadFloat32();
		mdl.tris[i].t[2] = r.ReadFloat32();
		mdl.tris[i].smoothingGroup = r.ReadInt8U();
		mdl.tris[i].groupIndex = r.ReadInt8U();
	}

	mdl.nNumGroups = r.ReadInt16U();
	mdl.groups = new ms3d_group_t[mdl.nNumGroups];
	
	for (i = 0; i < mdl.nNumGroups; ++i)
	{
		mdl.groups[i].flags = r.ReadInt8U();
		r.ReadString(32, mdl.groups[i].name);
		mdl.groups[i].numtriangles = r.ReadInt16U();

		mdl.groups[i].triangleIndices = new word[mdl.groups[i].numtriangles];

		for (j = 0; j < mdl.groups[i].numtriangles; ++j)
			mdl.groups[i].triangleIndices[j] = r.ReadInt16U();

		mdl.groups[i].materialIndex = r.ReadInt8();
	}


	mdl.nNumMaterials = r.ReadInt16U();
	mdl.materials = new ms3d_material_t[mdl.nNumMaterials];
	
	for (i = 0; i < mdl.nNumMaterials; ++i)
	{
		r.ReadString(32, mdl.materials[i].name);
		mdl.materials[i].ambient[0] = r.ReadFloat32();
		mdl.materials[i].ambient[1] = r.ReadFloat32();
		mdl.materials[i].ambient[2] = r.ReadFloat32();
		mdl.materials[i].ambient[3] = r.ReadFloat32();
		mdl.materials[i].diffuse[0] = r.ReadFloat32();
		mdl.materials[i].diffuse[1] = r.ReadFloat32();
		mdl.materials[i].diffuse[2] = r.ReadFloat32();
		mdl.materials[i].diffuse[3] = r.ReadFloat32();
		mdl.materials[i].specular[0] = r.ReadFloat32();
		mdl.materials[i].specular[1] = r.ReadFloat32();
		mdl.materials[i].specular[2] = r.ReadFloat32();
		mdl.materials[i].specular[3] = r.ReadFloat32();
		mdl.materials[i].emissive[0] = r.ReadFloat32();
		mdl.materials[i].emissive[1] = r.ReadFloat32();
		mdl.materials[i].emissive[2] = r.ReadFloat32();
		mdl.materials[i].emissive[3] = r.ReadFloat32();
		mdl.materials[i].shininess = r.ReadFloat32();
		mdl.materials[i].transparency = r.ReadFloat32();
		mdl.materials[i].mode = r.ReadInt8();
		r.ReadString(128, mdl.materials[i].texture);
		r.ReadString(128, mdl.materials[i].alphamap);
	}

	mdl.fAnimationFPS = r.ReadFloat32();
	mdl.fCurrentTime = r.ReadFloat32();
	mdl.iTotalFrames = r.ReadLong();

	mdl.nNumJoints = r.ReadInt16U();
	mdl.joints = new ms3d_joint_t[mdl.nNumJoints];

	for (i = 0; i < mdl.nNumJoints; ++i)
	{
		mdl.joints[i].flags = r.ReadInt8();
		r.ReadString(32, mdl.joints[i].name);
		r.ReadString(32, mdl.joints[i].parentName);
		mdl.joints[i].rotation[0] = r.ReadFloat32();
		mdl.joints[i].rotation[1] = r.ReadFloat32();
		mdl.joints[i].rotation[2] = r.ReadFloat32();
		mdl.joints[i].position[0] = r.ReadFloat32();
		mdl.joints[i].position[1] = r.ReadFloat32();
		mdl.joints[i].position[2] = r.ReadFloat32();
		mdl.joints[i].numRotationKeyframes = r.ReadInt16U();
		mdl.joints[i].keyFramesRot = new ms3d_keyframe_rot_t[mdl.joints[i].numRotationKeyframes];
		mdl.joints[i].numPositionKeyframes = r.ReadInt16U();
		mdl.joints[i].keyFramesPos = new ms3d_keyframe_pos_t[mdl.joints[i].numPositionKeyframes];

		for (j = 0; j < mdl.joints[i].numRotationKeyframes; ++j)
		{
			mdl.joints[i].keyFramesRot[j].time = r.ReadFloat32();
			mdl.joints[i].keyFramesRot[j].rotation[0] = r.ReadFloat32();
			mdl.joints[i].keyFramesRot[j].rotation[1] = r.ReadFloat32();
			mdl.joints[i].keyFramesRot[j].rotation[2] = r.ReadFloat32();
		}

		for (j = 0; j < mdl.joints[i].numPositionKeyframes; ++j)
		{
			mdl.joints[i].keyFramesPos[j].time = r.ReadFloat32();
			mdl.joints[i].keyFramesPos[j].position[0] = r.ReadFloat32();
			mdl.joints[i].keyFramesPos[j].position[1] = r.ReadFloat32();
			mdl.joints[i].keyFramesPos[j].position[2] = r.ReadFloat32();
		}
	}

	r.Close();


	//////////////////////////////////////////////////////////////////
	// Import, 0.9.5 ABI
	//////////////////////////////////////////////////////////////////

	//Vector<long> transV;
	//long index, materialIndex;

	for (i = 0; i < mdl.nNumMaterials; ++i)
	{
		index_t materialIndex = freyjaMaterialCreate();

		freyjaMaterialName(materialIndex, mdl.materials[i].name);
		freyjaMaterialAmbient(materialIndex, mdl.materials[i].ambient);
		freyjaMaterialDiffuse(materialIndex, mdl.materials[i].diffuse);
		freyjaMaterialSpecular(materialIndex, mdl.materials[i].specular);
		freyjaMaterialEmissive(materialIndex, mdl.materials[i].emissive);
		freyjaMaterialShininess(materialIndex, mdl.materials[i].shininess);
		freyjaMaterialTransparency(materialIndex,mdl.materials[i].transparency);

		// Texture ID will be overwritten if exists and loads
		freyjaMaterialTexture(materialIndex, 0);
		freyjaMaterialTextureName(materialIndex, mdl.materials[i].texture);

		// mdl.materials[i].mode; // Not used, milkshape flag
		// mdl.materials[i].alphamap; // Not used, alpha mask filename
	}

	index_t model = freyjaModelCreate();
	index_t mesh = freyjaMeshCreate();
	freyjaModelAddMesh(model, mesh);

	for (i = 0; i < mdl.nNumVertices; ++i)
	{
		Vec3 v(mdl.vertices[i].vertex);
		v *= scale;
		index_t vertex = freyjaMeshVertexCreate3fv(mesh, v.mVec);
		freyjaMeshVertexWeight(mesh, vertex, mdl.vertices[i].boneId, 1.0f);
		//transV.pushBack(index);
	}

	// Should use groups ( like freyja meshes )

	for (i = 0; i < mdl.nNumTriangles; ++i)
	{			
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, 0);
		freyjaMeshPolygonGroup1u(mesh, face, mdl.tris[i].smoothingGroup);

		for (j = 0; j < 3; ++j)
		{
			//index = transV[mdl.tris[i].vertexIndices[j]];
			index_t vertex = mdl.tris[i].vertexIndices[j];
			freyjaMeshPolygonAddVertex1i(mesh, face, vertex);

			// FIXME: These are really polymapped normals here
			freyjaMeshVertexNormal3fv(mesh, vertex, mdl.tris[i].vertexNormals[j]);
			index_t texcoord = freyjaMeshTexCoordCreate2f(mesh, mdl.tris[i].s[j], mdl.tris[i].t[j]);
			freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);
		}
	}

	index_t skeleton = freyjaSkeletonCreate();
	freyjaModelAddSkeleton(model, skeleton);
	
	for (i = 0; i < mdl.nNumJoints; ++i)
	{
		index_t bone = freyjaBoneCreate(skeleton);
		// This will nop if the allocation of bone happens after skeleton
		freyjaSkeletonAddBone(skeleton, bone); 
		freyjaBoneFlags(bone, 0x0);
		freyjaBoneName(bone, mdl.joints[i].name);
		freyjaBoneTranslate3f(bone,
							  mdl.joints[i].position[0]*scale,
							  mdl.joints[i].position[1]*scale,
							  mdl.joints[i].position[2]*scale);
		freyjaBoneRotateEuler3f(bone,
								mdl.joints[i].rotation[0],
								mdl.joints[i].rotation[1],
								mdl.joints[i].rotation[2]);

		for (j = 0; j < mdl.nNumJoints; ++j)
		{
			if (!strncmp(mdl.joints[i].name, mdl.joints[j].parentName, 32))
			{
				freyjaBoneAddChild(bone, j);
			}
			
			if (!strncmp(mdl.joints[i].parentName, mdl.joints[j].name, 32))
			{
				freyjaBoneParent(bone, j);
			}
		}

		index_t track = freyjaBoneTrackNew(bone);
		//freyjaBoneTrackRate(bone, track, 30.0f);  

		for (j = 0; j < mdl.joints[i].numRotationKeyframes; ++j)
		{
			vec_t t = mdl.joints[i].keyFramesRot[j].time;
			vec_t x = (mdl.joints[i].keyFramesRot[j].rotation[0]);
			vec_t y = (mdl.joints[i].keyFramesRot[j].rotation[1]);
			vec_t z = (mdl.joints[i].keyFramesRot[j].rotation[2]);

			//x += mdl.joints[i].rotation[0];
			//y += mdl.joints[i].rotation[1];
			//z += mdl.joints[i].rotation[2];

			index_t key = freyjaBoneKeyFrameNew(bone, track, t);
			freyjaBoneRotKeyFrameEuler3f(bone, track, key, x, y, z);
		}

		for (j = 0; j < mdl.joints[i].numPositionKeyframes; ++j)
		{
			vec_t t = mdl.joints[i].keyFramesPos[j].time;
			vec_t x = mdl.joints[i].keyFramesPos[j].position[0]*scale;
			vec_t y = mdl.joints[i].keyFramesPos[j].position[1]*scale;
			vec_t z = mdl.joints[i].keyFramesPos[j].position[2]*scale;

			//x += mdl.joints[i].position[0]*scale;
			//y += mdl.joints[i].position[1]*scale;
			//z += mdl.joints[i].position[2]*scale;

			index_t key = freyjaBoneKeyFrameNew(bone, track, t);
			freyjaBonePosKeyFrame3f(bone, track, key, x, y, z);
		}
	}

	return 0;
}


int freyja_model__milkshape_export(char *filename)
{
	const vec_t scale = 2.5;
	SystemIO::FileWriter w;
	char name[128];
	long modelIndex = 0;    // make plugin option
	long skeletonIndex = 0; // make plugin option
	long i, j, k;
	long meshCount, meshIndex;
	long vertexCount, vertexIndex;
	long polygonCount, polygonIndex, faceVertexCount;//, faceVertex;
	vec3_t xyz;
	//vec2_t uv;


	if (!w.Open(filename))
	{
		return -1;
	}

	w.WriteString(10, "MS3D000000");
	w.WriteLong(4);

	meshCount = freyjaGetModelMeshCount(modelIndex);

	for (vertexCount = 0, polygonCount = 0, i = 0; i < meshCount; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(modelIndex, i);
		vertexCount += freyjaGetMeshVertexCount(meshIndex);
		polygonCount += freyjaGetMeshPolygonCount(meshIndex);
	}

	w.WriteInt16U(vertexCount);

	for (i = 0; i < meshCount; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(modelIndex, i);
		vertexCount = freyjaGetMeshVertexCount(meshIndex);

		for (j = 0; j < vertexCount; ++j)
		{
			vertexIndex = j;//freyjaGetMeshVertexIndex(meshIndex, j);
			freyjaGetMeshVertexPos3fv(meshIndex, vertexIndex, xyz);

			w.WriteInt8U(0); // flags
			w.WriteFloat32(xyz[0]*scale); // x
			w.WriteFloat32(xyz[1]*scale); // y
			w.WriteFloat32(xyz[2]*scale); // z
			w.WriteInt8(-1); // boneId FIXME: ( ms is 1:1 vertex:bone, so for now skip )
			w.WriteInt8U(0); // refCount
		}
	}

	w.WriteInt16U(polygonCount);
	
	for (i = 0; i < meshCount; ++i)
	{
		meshIndex = freyjaGetModelMeshIndex(modelIndex, i);
		polygonCount = freyjaGetMeshPolygonCount(meshIndex);

		for (j = 0; j < polygonCount; ++j)
		{
			polygonIndex = j;//freyjaGetMeshPolygonIndex(meshIndex, j);
			faceVertexCount = freyjaGetMeshPolygonVertexCount(meshIndex, polygonIndex);

			faceVertexCount = 3; /* Milkshape only handles triangles,
								  * make user tesselate model to triangles
								  * to avoid 'write' to model state from here
								  * or handle it here, don't fuck with freyja 
								  * object state from export plugins! */

			w.WriteInt16(0); // flags

			for (k = 0; k < faceVertexCount; ++k)
			{
				vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, j, k);
				w.WriteInt16(vertexIndex);
			}

			for (k = 0; k < faceVertexCount; ++k)
			{
				vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, j, k);
				freyjaGetMeshVertexNormal3fv(meshIndex, vertexIndex, xyz);
				
				w.WriteFloat32(xyz[0]);
				w.WriteFloat32(xyz[1]);
				w.WriteFloat32(xyz[2]);
			}

			for (k = 0; k < faceVertexCount; ++k)
			{
				vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, j, k);
				freyjaGetMeshVertexTexCoord3fv(meshIndex, vertexIndex, xyz);
				
				w.WriteFloat32(xyz[0]);
			}

			for (k = 0; k < faceVertexCount; ++k)
			{
				vertexIndex = freyjaGetMeshPolygonVertexIndex(meshIndex, j, k);
				freyjaGetMeshVertexTexCoord3fv(meshIndex, vertexIndex, xyz);
				
				w.WriteFloat32(xyz[1]);
			}

			w.WriteInt8U(0); // smoothingGroup
			w.WriteInt8U(i);
		}
	}


	w.WriteInt16U(meshCount);

	for (k = 0, i = 0; i < meshCount; ++i)
	{
		polygonCount = freyjaGetMeshPolygonCount(meshIndex);

		snprintf(name, 32, "mesh%li", i);

		w.WriteInt8U(0); // flags
		w.WriteString(32, name); // name
		w.WriteInt16U(polygonCount);  // tris count

		for (j = 0; j < polygonCount; ++j, ++k)
		{
			w.WriteInt16U(j+k); // tris indeices
		}

		w.WriteInt8(-1); // materialIndex
	}


	w.WriteInt16U(0); // materialCount
 
	for (i = 0; i < 0; ++i)
	{
		snprintf(name, 32, "mat%li", i);
		w.WriteString(32, name);

		w.WriteFloat32(0); // ambient
		w.WriteFloat32(0);
		w.WriteFloat32(0);
		w.WriteFloat32(0);
		w.WriteFloat32(0); // diffuse
		w.WriteFloat32(0);
		w.WriteFloat32(0);
		w.WriteFloat32(0);
		w.WriteFloat32(0); // specular
		w.WriteFloat32(0);
		w.WriteFloat32(0);
		w.WriteFloat32(0);
		w.WriteFloat32(0); // emissive
		w.WriteFloat32(0);
		w.WriteFloat32(0);
		w.WriteFloat32(0);
		w.WriteFloat32(0); // shininess
		w.WriteFloat32(0); // transparency

		w.WriteInt8(0); // mode
		
		snprintf(name, 128, "texturemap.png"); // he he he
		w.WriteString(128, name); // texturemap

		snprintf(name, 128, "alphamap.png");
		w.WriteString(128, name); // alphamap
	}

	w.WriteFloat32(30.0f); // fAnimationFPS
	w.WriteFloat32(0.0f);  // fCurrentTime
	w.WriteLong(0);        // iTotalFrames

	long boneIndex;
	long boneCount = freyjaGetSkeletonBoneCount(skeletonIndex);

	w.WriteInt16U(boneCount); // boneCount

	for (i = 0; i < boneCount; ++i)
	{
		boneIndex = freyjaGetSkeletonBoneIndex(skeletonIndex, i);

		w.WriteInt8(0); // flags

		freyjaGetBoneName(boneIndex, 32, name);
		w.WriteString(32, name); // this bone's name

		name[0] = 0;
		freyjaGetBoneName(freyjaGetBoneParent(boneIndex), 32, name);
		w.WriteString(32, name); // parent name

		freyjaGetBoneRotationEuler3fv(boneIndex, xyz);
		w.WriteFloat32(helDegToRad(xyz[0]));
		w.WriteFloat32(helDegToRad(xyz[1]));
		w.WriteFloat32(helDegToRad(xyz[2]));

		freyjaGetBoneTranslation3fv(boneIndex, xyz);
		w.WriteFloat32(xyz[0]*scale);
		w.WriteFloat32(xyz[1]*scale);
		w.WriteFloat32(xyz[2]*scale);

		w.WriteInt16U(0); // numRotationKeyframes
		w.WriteInt16U(0); // numPositionKeyframes

		for (j = 0; j < 0; ++j) // numRotationKeyframes
		{
			w.WriteFloat32(0.0f);  // time
			w.WriteFloat32(0.0f);  // x
			w.WriteFloat32(0.0f);  // y
			w.WriteFloat32(0.0f);  // z
		}

		for (j = 0; j < 0; ++j) // numPositionKeyframes
		{
			w.WriteFloat32(0.0f);  // time
			w.WriteFloat32(0.0f);  // x
			w.WriteFloat32(0.0f);  // y
			w.WriteFloat32(0.0f);  // z
		}
	}

	w.Close();

	return 0;
}
