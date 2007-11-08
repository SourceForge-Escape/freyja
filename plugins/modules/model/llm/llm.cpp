/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * Comments: This is the LLM plug-in module
 *
 *           See file COPYING for license details.
 *
 *
 *-- Test Defines -----------------------------------------------------
 *           
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2007.10.29:
 * Mongoose - Created, based on specs from:
 *            https://wiki.secondlife.com/wiki/Avatar_Appearance
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
#include <hel/Vec3.h>


using namespace mstl;
using namespace hel;

extern "C" {

	int freyja_model__llm_check(char *filename);
	int freyja_model__llm_import(char *filename);
	int freyja_model__llm_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginName1s("llm");
	freyjaPluginDescription1s("Linden Binary Mesh (*.llm)");
	freyjaPluginAddExtention1s("*.llm");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH);
}


int import_model(char *filename)
{
	
	if ( !freyja_model__llm_check(filename) )
		return freyja_model__llm_import(filename);

	return -1;
}


int freyja_model__llm_check(char *filename)
{
	SystemIO::FileReader r;

	if ( !r.Open(filename) )
	{
		return -1;
	}

	char buf[32];
	r.ReadString(26, buf);
	buf[25] = '\0';
	r.Close();

	if ( !strncmp(buf, "Linden Binary Mesh 1.0", 24) )
		return 0;

	return -2;
}


typedef struct llm_face {
     int16 face[3];
} llm_face_t;


typedef struct llm_morph_vertex {

	uint32 vertexIndex;
	vec3_t coord;
	vec3_t normal;
	vec3_t binormal;
	vec2_t texCoord;

} llm_morph_vertex;


typedef struct llm_morph {

	char morphName[64]; 	
	int32 numVertices; 
	llm_morph_vertex* vertices;

} llm_morph_t;


int freyja_model__llm_import(char* filename)
{
	if ( freyja_model__llm_check(filename) )
		return -1;

	SystemIO::BufferedFileReader r;

	if ( !r.Open(filename) )
	{
		MSTL_MSG("Failed to open file '%s'", filename);
		return -2;
	}

	int pluginId = freyjaGetPluginId();
	//vec_t scale = 1.0f;
	//freyjaGetPluginArg1f(pluginId, "scale", &scale);

    // char[24] "Linden Binary Mesh 1.0" 
	char magic[24];
	r.ReadString(24, magic);

	byte hasWeights = r.ReadByte();

    byte hasDetailTexCoords = r.ReadByte();

	vec3_t position;
	r.ReadFloat32Array(3, position);

    vec3_t rotationAngles;
	r.ReadFloat32Array(3, rotationAngles);	

    byte rotationOrder = r.ReadByte(); //(ignored, set to 0) 
	rotationOrder = 0;

    vec3_t scale; 
	r.ReadFloat32Array(3, scale);

    uint16 numVertices = r.ReadInt16U(); 
	MSTL_MSG("%i vertices", numVertices);

	vec3_t* baseCoords = new vec3_t[ numVertices ];

	for (uint32 i = 0; i < numVertices; ++i)
	{
		//r.ReadFloat32Array(3, baseCoords[i]);
		baseCoords[i][0] = r.ReadFloat32();
		baseCoords[i][1] = r.ReadFloat32();
		baseCoords[i][2] = r.ReadFloat32();
	}

	vec3_t* baseNormals = new vec3_t[ numVertices ];

	for (uint32 i = 0; i < numVertices; ++i)
		r.ReadFloat32Array(3, baseNormals[i]);

	vec3_t* baseBinormals = new vec3_t[ numVertices ];

	for (uint32 i = 0; i < numVertices; ++i)
		r.ReadFloat32Array(3, baseBinormals[i]);

	vec2_t* texCoords = new vec2_t[ numVertices ];

	for (uint32 i = 0; i < numVertices; ++i)
		r.ReadFloat32Array(2, texCoords[i]);

	vec2_t* detailTexCoords = NULL;

	vec_t* weights = NULL;

    if (hasDetailTexCoords)
	{
		detailTexCoords = new vec2_t[ numVertices ];

		for (uint32 i = 0; i < numVertices; ++i)
			r.ReadFloat32Array(2, detailTexCoords[i]);
	}

    if (hasWeights)
	{
        weights = new vec_t[ numVertices ];
		r.ReadFloat32Array(numVertices, weights);
	}

    uint16 numFaces = r.ReadInt16U(); 
	MSTL_MSG("%i faces", numFaces);

	llm_face_t* faces = new llm_face_t[ numFaces ];
	for (uint32 i = 0; i < numFaces; ++i)
	{
		faces[i].face[0] = r.ReadInt16();
		faces[i].face[1] = r.ReadInt16();
		faces[i].face[2] = r.ReadInt16();

		// no padding should be needed for alignment
	}
	
 
    if ( hasWeights ) 
    {        
		uint16 numSkinJoints = r.ReadInt16U() + 1; 
		MSTL_MSG("joints '%i'", numSkinJoints);

		char jointName[64];

		for (uint32 i = 0; i < numSkinJoints; ++i)
		{
			r.ReadString(64, jointName);
			MSTL_MSG("%i. '%s'", i, jointName);
		}
    }

#if 0 // disabled

	// Look for vertex morph frames.
	char morphName[64];
	
	while ( r.ReadString(64, morphName) && strncmp(morphName, "End Morphs", 10) )
	{
		MSTL_MSG("'%s'", morphName);

		llm_morph_t morph;

		strncpy(morph.morphName, morphName, 64);
		morph.numVertices = r.ReadInt32();

		if ( morph.numVertices > 0 )
		{
			morph.vertices = new llm_morph_vertex[ morph.numVertices ];

			for (uint32 i = 0; i < morph.numVertices; ++i)
			{
				morph.vertices[i].vertexIndex = r.ReadInt32U();
				r.ReadFloat32Array(3, morph.vertices[i].coord);
				r.ReadFloat32Array(3, morph.vertices[i].normal);
				r.ReadFloat32Array(3, morph.vertices[i].binormal);
				r.ReadFloat32Array(2, morph.vertices[i].texCoord);
			}

			// FIXME: Import mesh animation using 0.9.5 API.

			delete [] morph.vertices;
		}

		r.ReadString(64, morphName);
	}
#endif

#if 0 // Not supporting remaps
    S32 numRemaps (or EOF) 
    struct vertexRemap[ numRemaps ] 
    {

        S32 remapSource 
        S32 remapDestination 

    }	
#endif

	r.Close();

	
	// Import to freyja
	index_t model = 0; // FIXME: This should be a plugin option later.
	index_t mesh = freyjaMeshCreate();
	freyjaModelAddMesh(model, mesh);

	for (uint32 i = 0; i < numVertices; ++i)
	{
		index_t vertex = freyjaMeshVertexCreate3fv(mesh, baseCoords[i]);
		index_t bone = 0; // FIXME: These need to be remapped

		freyjaMeshVertexWeight(mesh, vertex, bone, weights[i]);

		freyjaMeshVertexNormal3fv(mesh, vertex, baseNormals[i]);

		vec3_t uvw = { texCoords[i][0], texCoords[i][1], 0.0f };
		freyjaMeshVertexTexCoord3fv(mesh, vertex, uvw);
	}

	for (uint32 i = 0; i < numFaces; ++i)
	{			
		index_t face = freyjaMeshPolygonCreate(mesh);
		freyjaMeshPolygonMaterial(mesh, face, 0);
		freyjaMeshPolygonGroup1u(mesh, face, 1);
		freyjaMeshPolygonAddVertex1i(mesh, face, faces[i].face[0]);
		freyjaMeshPolygonAddVertex1i(mesh, face, faces[i].face[1]);
		freyjaMeshPolygonAddVertex1i(mesh, face, faces[i].face[2]);
	}

	if ( baseCoords ) delete [] baseCoords; 

	if ( baseNormals ) delete [] baseNormals;
 
	if ( baseBinormals ) delete [] baseBinormals; 

	if ( texCoords ) delete [] texCoords;  

	if ( detailTexCoords ) delete [] detailTexCoords;  

	if ( weights ) delete [] weights;  

	if ( faces ) delete [] faces;

	return 0;
}


int freyja_model__llm_export(char *filename)
{
	return -1;
}


