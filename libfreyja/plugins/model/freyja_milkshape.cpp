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


extern "C" {

	int freyja_model__milkshape_check(char *filename);
	int freyja_model__milkshape_import(char *filename);
	int freyja_model__milkshape_export(char *filename);
	int import_model(char *filename);
}

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef word
typedef unsigned short word;
#endif


typedef struct ms3d_header_s
{
	char id[10];
	long version;

}  ms3d_header_t;


typedef struct ms3d_vertex_s
{
	byte flags;
	float vertex[3];
	char boneID;
	byte refCount;

}  ms3d_vertex_t;


typedef struct ms3d_triangle_s
{
	word flags;
	word vertexIndices[3];
	float vertexNormals[3][3];
	float s[3], t[3];
	byte smoothingGroup;
	byte groupIndex;

}  ms3d_triangle_t;


typedef struct ms3d_material_s
{
	char name[32];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emissive[4];
	float shininess;	// 0.0f - 128.0f
	float transparency;	// 0.0f - 1.0f
	byte mode;	// 0, 1, 2 is unused now
	char texture[128];
	char alphamap[128];

}  ms3d_material_t;


typedef struct ms3d_joint_s
{
	byte flags;
	char name[32];
	char parentName[32];
	float rotation[3];
	float translation[3];
	word numRotationKeyframes;
	word numTranslationKeyframes;

}  ms3d_joint_t;


typedef struct ms3d_keyframe_s
{
	float time;
	float parm[3];

} ms3d_keyframe_t;


#ifdef WIN32
#   pragma pack(pop, pack_hack, 1)
#endif

#undef PACK_HACK

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
	if ((strncmp(header.id, "MS3D000000", 10) == 0) &&
		 header.version == 3 || header.version == 4)
		return 0;

	return -2;
}


int freyja_model__milkshape_import(char *filename)
{
	FILE *f;
	ms3d_header_t *header;
	byte *buffer, *offset;
	unsigned int sz;


	if (freyja_model__milkshape_check(filename))
		return -1;

	f = fopen(filename, "rb");

	if (!f)
	{
		perror("milkshape_import> fopen failed");
		return -2;
	}

	fseek(f, 0, SEEK_END);
	sz = ftell(f);
	fseek(f, 0, SEEK_SET);

	buffer = new byte[sz];
	
	fread(buffer, sz, 1, f);
	fclose(f);

	offset = buffer;

	header = (ms3d_header_t *)offset;
	offset += sizeof(ms3d_header_t);

	printf("Doh I never finished this!!!\n");

	/////////////////////////////////////////////////////////////////

	/* Read number of vertices into nVertices... I think */
	int nVertices = *( word* )pPtr;
	m_numVertices = nVertices;
	/* Makes new array of vertices */
	m_pVertices = new Vertex[nVertices];
	
	/* Advance the pointer one "word" */
	pPtr += sizeof( word );

	int i;
	for ( i = 0; i < nVertices; i++ )
	{
		/* Read one vertex into pVertex */
		MS3DVertex *pVertex = ( MS3DVertex* )pPtr;
		
		m_pVertices[i].m_boneID = pVertex->m_boneID;
		
		/* Copies the vertice (3 floats) from pVertex, into the m_pVertices array */
		memcpy( m_pVertices[i].m_location, pVertex->m_vertex, sizeof( float )*3 );
		/* Advance the pointer one "MS3DVertex" */
		pPtr += sizeof( MS3DVertex );
	}

	/* Read in number of Triangles into nTriangles */
	int nTriangles = *( word* )pPtr;
	
	m_numTriangles = nTriangles;
	
	/* Create array of Triangles "nTriangles" big */
	m_pTriangles = new Triangle[nTriangles];
	/* Advance the pointer one "word" */
	pPtr += sizeof( word );

	/* m_s and m_t are the texture coordinates for each of the three vertices */
	for ( i = 0; i < nTriangles; i++ )
	{
		/* Read one triangle into pTriangle */
		MS3DTriangle *pTriangle = ( MS3DTriangle* )pPtr;
		
		/* In the file, the vertex indices are stores as an array of word values,
		   but in the model they are int values for consistency and simplicity.
		   So in other words, the vertexIndices must be ints in the model, so this
		   just converts them to integers */
		int vertexIndices[3] = { pTriangle->m_vertexIndices[0], pTriangle->m_vertexIndices[1], pTriangle->m_vertexIndices[2] };
		
		/* The t values are all set to 1.0-(original value). The reason for this
		   is that OpenGL uses a lower-left coordinate system, whereas Milkshape
		   uses an upper-left coordinate system for its texture coordinates.
		   This reverses the y coordinate */ 
		float t[3] = { 1.0f-pTriangle->m_t[0], 1.0f-pTriangle->m_t[1], 1.0f-pTriangle->m_t[2] };
		/* Copies vertexNormals from pTriangle to m_pTriangles */
		memcpy( m_pTriangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof( float )*3*3 );
		/* Copies the "s" value from pTriangle to m_pTriangles */
		memcpy( m_pTriangles[i].m_s, pTriangle->m_s, sizeof( float )*3 );
		/* Copies the "t" value from pTriangle to m_pTriangles */
		memcpy( m_pTriangles[i].m_t, t, sizeof( float )*3 );
		/* Copies the vertexIndices into m_pTriangles */
		memcpy( m_pTriangles[i].m_vertexIndices, vertexIndices, sizeof( int )*3 );
		/* Advance the pointer one MS3DTriangle */
		pPtr += sizeof( MS3DTriangle );
	}

	/* Read in number of groups into nGroups */
	int nGroups = *( word* )pPtr;
	m_numMeshes = nGroups;
	
	/* Create an array of Meshes, nGroups big */
	m_pMeshes = new Mesh[nGroups];
	
	/* Advance the pointer */
	pPtr += sizeof( word );
	
	for ( i = 0; i < nGroups; i++ )
	{
		pPtr += sizeof( byte );	// flags
		pPtr += 32;				// name

		/* Reads the number of triangles in this mesh */
		word nTriangles = *( word* )pPtr;
		/* Advance the pointer */
		pPtr += sizeof( word );
		
		/* Create an array to hold the indices of the triangles in this mesh */
		int *pTriangleIndices = new int[nTriangles];
		
		for ( int j = 0; j < nTriangles; j++ )
		{
			/* Read in one triangle index */
			pTriangleIndices[j] = *( word* )pPtr;
			/* Advance the pointer */
			pPtr += sizeof( word );
		}

		/* Read in the material index */
		char materialIndex = *( char* )pPtr;
		/* Advance the pointer */
		pPtr += sizeof( char );
	
		/* Store all our info for this mesh into the m_pMeshes array */
		m_pMeshes[i].m_materialIndex = materialIndex;
		m_pMeshes[i].m_numTriangles = nTriangles;
		m_pMeshes[i].m_pTriangleIndices = pTriangleIndices;
	}

	int nMaterials = *( word* )pPtr;
	m_numMaterials = nMaterials;
	m_pMaterials = new Material[nMaterials];
	pPtr += sizeof( word );
	for ( i = 0; i < nMaterials; i++ )
	{
		MS3DMaterial *pMaterial = ( MS3DMaterial* )pPtr;
		memcpy( m_pMaterials[i].m_ambient, pMaterial->m_ambient, sizeof( float )*4 );
		memcpy( m_pMaterials[i].m_diffuse, pMaterial->m_diffuse, sizeof( float )*4 );
		memcpy( m_pMaterials[i].m_specular, pMaterial->m_specular, sizeof( float )*4 );
		memcpy( m_pMaterials[i].m_emissive, pMaterial->m_emissive, sizeof( float )*4 );
		m_pMaterials[i].m_shininess = pMaterial->m_shininess;
		m_pMaterials[i].m_pTextureFilename = new char[strlen( pMaterial->m_texture )+1];
		strcpy( m_pMaterials[i].m_pTextureFilename, pMaterial->m_texture );
		pPtr += sizeof( MS3DMaterial );
	}


	delete[] buffer;

	return 0;
}


int freyja_model__milkshape_export(char *filename)
{
	printf("freyja_model__milkshape_export> Not implemented, %s:%i\n", 
			 __FILE__, __LINE__);
	return -1;
}
