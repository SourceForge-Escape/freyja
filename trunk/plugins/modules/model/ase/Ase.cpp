/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Ase
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: ASE model class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
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
 =================================================================*/

#include "string.h"

#include "Ase.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Ase::Ase()
{
	mFlags = fWorldCoordXYZ;

	mMaterialCount = 0;
	mVertexCount = 0;
	mFaceCount = 0;
	mTexFacesCount = 0;
	mUVWCount = 0;
	mNormalCount = 0;
	
	mVertices = 0x0;
	mUVWs = 0x0;
	mNormals = 0x0;
	mFaces = 0x0;

	mTexture.id = 0;       
	mTexture.uTile = 1.0f;  
	mTexture.vTile = 1.0f;  
	mTexture.uOffset = 0.0f;
	mTexture.vOffset = 0.0f;
	memset(mTexture.name, 0, sizeof(mTexture.name));
}


Ase::~Ase()
{
	erase();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

int Ase::check(char *filename)
{
	char header[64];
	FILE *f;
	char c;
	
	
	f = fopen(filename, "rb");

	if (!f)
	{
		printf("Ase::check> ERROR file '%s' could not be read.\n", filename);
		perror("Couldn't read file:");
		return -1;
	}

	fread(&header, 32, 1, f);      
	fclose(f);
  
	header[31] = 0;
	c = header[19];
	header[19] = 0;  

	if (strcmp(header, "*3DSMAX_ASCIIEXPORT") == 0)
	{
		return 0;
	}
	else
	{
		return -3;
	}
}


void Ase::printInfo()
{
	printf("ASE {\n");
	printf("%i vertices\n", mVertexCount);
	printf("%i faces\n", mFaceCount);
	printf("%i textured faces\n", mTexFacesCount);
	printf("%i texture coordinates\n", mUVWCount);
	printf("%s normals\n", mNormals ? "Has" : "Doesn't have");
	printf("%s\n", (mTexture.name[0]) ? "No materials" : mTexture.name);
	printf("}\n");
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Ase::convertWorldXYZ(vec3_t xzy)
{
	vec_t swap;

	/* In 3ds Max Y and Z are reversed, and the Z is negative */
	swap = xzy[1];
	xzy[1] = xzy[2];
	xzy[2] = -swap;
}


void Ase::convertWorld3dsMax(vec3_t xyz)
{
	vec_t swap;

	/* 3ds Max revserses Y and Z, and the Z is negative */
	swap = xyz[1];
	xyz[1] = -xyz[2];
	xyz[2] = swap;
}


void Ase::erase()
{
	mMaterialCount = 0;
	mVertexCount = 0;
	mFaceCount = 0;
	mTexFacesCount = 0;
	mUVWCount = 0;
	mNormalCount = 0;

	mTexture.name[0] = 0;
	mTexture.id = 0;       
	mTexture.uTile = 1.0f;  
	mTexture.vTile = 1.0f;  
	mTexture.uOffset = 0.0f;
	mTexture.vOffset = 0.0f;

	if (mVertices)
		delete [] mVertices;

	if (mUVWs)
		delete [] mUVWs;

	if (mNormals)
		delete [] mNormals;

	if (mFaces)
		delete [] mFaces;
}


int Ase::load(char *filename)
{
	FILE *f;
  

	if (check(filename))
	{
		//printf("Ase::load> File may not be valid ASE file.\n");
		return -1;
	}
	
	f = fopen(filename, "r");

	if (!f)
	{
		printf("Ase::load> ERROR file '%s' could not be read.\n", filename);
		perror("Couldn't read file:");
		return -1;
	}

	erase();

	readModelInfo(f);

	mVertices = new vec3_t[mVertexCount];
	mFaces = new ase_triangle_t[mFaceCount];

	if (mUVWCount)
		mUVWs = new vec3_t[mUVWCount];
	
	if (mNormalCount)
		mNormals = new vec3_t[mVertexCount];

	readModelData(f);

	if (mFlags & fShowInfoOnLoad)
		printInfo();

	fclose(f);

	return 0;
}


int Ase::save(char *filename)
{
	FILE *f;
	int i, j, k;
  
	
	f = fopen(filename, "w");

	if (!f)
	{
		printf("Ase::save> ERROR Couldn't write file '%s'.\n", filename);	 
		perror("Couldn't write file:");
		return -1;
	}
	
	if (!mVertexCount || !mFaceCount)
	{
		printf("Ase::save> ERROR No data to save\n");
		return -2;
	}

	/* FIXME: Dummy values for unimplemented/unused values */

	/* Header */
	fprintf(f, "*3DSMAX_ASCIIEXPORT 200\n");
	fprintf(f, "*COMMENT \"AsciiExport Version  2.00 - %s\"\n", "Mongoose ASE");
	fprintf(f, "*SCENE {\n");
	fprintf(f, "\t*SCENE_FILENAME \"%s\"\n", filename);
	fprintf(f, "\t*SCENE_FIRSTFRAME %d\n", 0);
	fprintf(f, "\t*SCENE_LASTFRAME %d\n", 20);
	fprintf(f, "\t*SCENE_FRAMESPEED %d\n", 30);
	fprintf(f, "\t*SCENE_TICKSPERFRAME %d\n", 160);
	fprintf(f, "\t*SCENE_BACKGROUND_STATIC %f\t%f\t%f\n", 0.0f, 0.0f, 0.0f);
	fprintf(f, "\t*SCENE_AMBIENT_STATIC %f\t%f\t%f\n", 0.0f, 0.0f, 0.0f);
	fprintf(f, "}\n");


	/* Materials */
	fprintf(f, "*MATERIAL_LIST {\n");
	fprintf(f, "\t*MATERIAL_COUNT %d\n", mMaterialCount);

	for (i = 0; i < mMaterialCount; ++i)
	{
		fprintf(f, "\t*MATERIAL %d {\n", i);
		fprintf(f, "\t\t*MATERIAL_NAME \"%s #%d\"\n", "Material", i+1);
		fprintf(f, "\t\t*MATERIAL_CLASS \"%s\"\n", "Standard");
		fprintf(f, "\t\t*MATERIAL_AMBIENT %f\t%f\t%f\n", 0.1f, 0.1f, 0.1f);
		fprintf(f, "\t\t*MATERIAL_DIFFUSE %f\t%f\t%f\n", 0.5f, 0.5f, 0.5f);
		fprintf(f, "\t\t*MATERIAL_SPECULAR %f\t%f\t%f\n", 0.9f, 0.9f, 0.9f);
		fprintf(f, "\t\t*MATERIAL_SHINE %f\n", 0.25f);
		fprintf(f, "\t\t*MATERIAL_SHINESTRENGTH %f\n", 0.05f);
		fprintf(f, "\t\t*MATERIAL_TRANSPARENCY %f\n", 0.0f);
		fprintf(f, "\t\t*MATERIAL_WIRESIZE %f\n", 1.0f);
		fprintf(f, "\t\t*MATERIAL_SHADING %s\n", "Blinn");
		fprintf(f, "\t\t*MATERIAL_XP_FALLOFF %f\n", 0.0f);
		fprintf(f, "\t\t*MATERIAL_SELFILLUM %f\n", 0.0f);
		fprintf(f, "\t\t*MATERIAL_FALLOFF %s\n", "In");
		fprintf(f, "\t\t*MATERIAL_SOFTEN\n");
		fprintf(f, "\t\t*MATERIAL_XP_TYPE %s\n", "Filter");

		fprintf(f, "\t\t*MAP_DIFFUSE {\n");
		fprintf(f, "\t\t\t*MAP_NAME \"%s\"\n", "Map #8");
		fprintf(f, "\t\t\t*MAP_CLASS \"%s\"\n", "Bitmap");
		fprintf(f, "\t\t\t*MAP_SUBNO %d\n", 1);
		fprintf(f, "\t\t\t*MAP_AMOUNT %f\n", 1.0f);
		fprintf(f, "\t\t\t*BITMAP \"%s\"\n", mTexture.name); // mTextures[i]
		fprintf(f, "\t\t\t*MAP_TYPE \"%s\"\n", "Spherical");
		fprintf(f, "\t\t\t*UVW_U_OFFSET %f\n", mTexture.uOffset);
		fprintf(f, "\t\t\t*UVW_V_OFFSET %f\n", mTexture.vOffset);
		fprintf(f, "\t\t\t*UVW_U_TILING %f\n", mTexture.uTile);
		fprintf(f, "\t\t\t*UVW_V_TILING %f\n", mTexture.vTile);
		fprintf(f, "\t\t\t*UVW_ANGLE %f\n", 0.0f);
		fprintf(f, "\t\t\t*UVW_BLUR %f\n", 1.0f);
		fprintf(f, "\t\t\t*UVW_BLUR_OFFSET %f\n", 0.0f);
		fprintf(f, "\t\t\t*UVW_NOISE_AMT %f\n", 1.0f);
		fprintf(f, "\t\t\t*UVW_NOISE_SIZE %f\n", 1.0f);
		fprintf(f, "\t\t\t*UVW_NOISE_LEVEL %d\n", 1);
		fprintf(f, "\t\t\t*UVW_NOISE_PHASE %f\n", 0.0f);
		fprintf(f, "\t\t\t*BITMAP_FILTER \"%s\"\n", "Pyramidal");
		fprintf(f, "\t\t}\n");

		fprintf(f, "\t}\n");
	}

	fprintf(f, "}\n");


	/* Model Geometery */
	fprintf(f, "*GEOMOBJECT {\n");
	fprintf(f, "\t*NODE_NAME \"%s\"\n", "SkeletalMesh");
	fprintf(f, "\t*NODE_TM {\n");
	fprintf(f, "\t\t*NODE_NAME \"%s\"\n", "SkeletalMesh");
	fprintf(f, "\t\t*INHERIT_POS %d %d %d\n", 0, 0, 0);
	fprintf(f, "\t\t*INHERIT_ROT %d %d %d\n", 0, 0, 0);
	fprintf(f, "\t\t*INHERIT_SCL %d %d %d\n", 0, 0, 0);
	fprintf(f, "\t\t*TM_ROW0 %f\t%f\t%f\n", 1.0f, 0.0f, 0.0f);
	fprintf(f, "\t\t*TM_ROW1 %f\t%f\t%f\n", 0.0f, 1.0f, 0.0f);
	fprintf(f, "\t\t*TM_ROW2 %f\t%f\t%f\n", 0.0f, 0.0f, 1.0f);
	fprintf(f, "\t\t*TM_ROW3 %f\t%f\t%f\n", 1.0f, 1.0f, 1.0f);
	fprintf(f, "\t\t*TM_POS %f\t%f\t%f\n", 0.0f, 0.0f, 0.0f);
	fprintf(f, "\t\t*TM_ROTAXIS %f\t%f\t%f\n", 0.0f, 0.0f, 0.0f);
	fprintf(f, "\t\t*TM_ROTANGLE %f\n", 0.0f);
	fprintf(f, "\t\t*TM_SCALE %f\t%f\t%f\n", 1.0f, 1.0f, 1.0f);
	fprintf(f, "\t\t*TM_SCALEAXIS %f\t%f\t%f\n", 0.0f, 0.0f, 0.0f);
	fprintf(f, "\t\t*TM_SCALEAXISANG %f\n", 0.0f);
	fprintf(f, "\t}\n");
	
	/* FIXME: Loop here for multi mesh, when supported later */

	/* Mesh(s) */
	fprintf(f, "\t*MESH {\n");
	fprintf(f, "\t\t*TIMEVALUE\t%i\n", 0);
	fprintf(f, "\t\t*MESH_NUMVERTEX\t%i\n", mVertexCount);
	fprintf(f, "\t\t*MESH_NUMFACES\t%i\n", mFaceCount);
	
	fprintf(f, "\t\t*MESH_VERTEX_LIST {\n");

	for (i = 0; i < mVertexCount; ++i)
	{
		fprintf(f, "\t\t\t*MESH_VERTEX  %i\t%f\t%f\t%f\n", i,	
				 mVertices[i][0], mVertices[i][1], mVertices[i][2]);
	}

	fprintf(f, "\t\t}\n");

	fprintf(f, "\t\t*MESH_FACE_LIST {\n");

	for (i = 0; i < mFaceCount; ++i)
	{
		fprintf(f, "\t\t\t*MESH_FACE %i:\tA:\t%i B:\t%i C:\t%i AB:\t%i BC:\t%i CA:\t%i \t*MESH_SMOOTHING %i\t*MESH_MTLID %i\n", i,
				  mFaces[i].vertIndex[0], 
				  mFaces[i].vertIndex[1], 
				  mFaces[i].vertIndex[2],
				  0, 1, 1, 1, 1);
	}

	fprintf(f, "\t\t}\n");


	/* Texture coordinates */
	fprintf(f, "\t\t*MESH_NUMTVERTEX %i\n", mUVWCount);
	fprintf(f, "\t\t*MESH_TVERTLIST {\n");
	
	for (i = 0; i < mUVWCount; ++i)
	{ 
		fprintf(f, "\t\t\t*MESH_TVERT %i\t%f\t%f\t%f\n", i, 
				  mUVWs[i][0], mUVWs[i][1], mUVWs[i][2]);
	}

	fprintf(f, "\t\t}\n");


	/* Normals (vertex normals) */
	if (mNormalCount)
	{
		fprintf(f, "\t\t*MESH_NORMALS {\n");
		
		for (i = 0; i < mFaceCount; ++i)
		{ 
			fprintf(f, "\t\t\t*MESH_FACENORMAL %i\t%f\t%f\t%f\n", i, 
					  mFaces[i].normal[0], 
					  mFaces[i].normal[1],
					  mFaces[i].normal[2]);
			
			for (j = 0; j < 3; ++j)
			{
				k = mFaces[i].vertIndex[j];
				
				fprintf(f, "\t\t\t*MESH_VERTEXNORMAL %i\t%f\t%f\t%f\n", k, 
						  mNormals[k][0], 
						  mNormals[k][1],
						  mNormals[k][2]);
			}
		}

		fprintf(f, "\t\t}\n");
	}

	/* Faces (triangles) */
	fprintf(f, "\t\t*MESH_NUMTVFACES %i\n", mFaceCount);
	fprintf(f, "\t\t*MESH_TFACELIST {\n");

	for (i = 0; i < mFaceCount; i++)
	{ 
		fprintf(f, "\t\t\t*MESH_TFACE %i\t%i\t%i\t%i\n", i,
				  mFaces[i].uvwIndex[0], 
				  mFaces[i].uvwIndex[1], 
				  mFaces[i].uvwIndex[2]);
	}
	
	fprintf(f, "\t\t}\n");
	
	
	/* Footer */
	fprintf(f, "\t}\n");
	fprintf(f, "\t*PROP_MOTIONBLUR 0\n");      
	fprintf(f, "\t*PROP_CASTSHADOW 1\n");
	fprintf(f, "\t*PROP_RECVSHADOW 1\n");      
	fprintf(f, "\t*MATERIAL_REF 0\n"); 
	fprintf(f, "}\n");

	fclose(f);

	return 0;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void Ase::readModelInfo(FILE *f)
{
  char data[255];

  rewind(f);

  while (!feof(f))
  {
    fscanf(f, "%s", data);
    
    if (!strcmp(data, NUM_VERTEX))
	 {
      fscanf(f, "%d", &mVertexCount);
	 }
    else if (!strcmp(data, NUM_FACES))
	 {
      fscanf(f, "%d", &mFaceCount);
	 }
    else if (!strcmp(data, NUM_TVERTEX))
	 {
      fscanf(f, "%d", &mUVWCount);
	 }
    else if (!strcmp(data, NUM_TFACES))
	 {
      fscanf(f, "%d", &mTexFacesCount);
	 }
    else if (!strcmp(data, NORMALS))
	 {
      mNormalCount = 1;
	 }
    else 
	 {
      fgets(data, sizeof(data), f);
	 }
  }	
}


void Ase::readSymbol(FILE *f, char *s)
{
	fscanf(f, s);
}


int Ase::readIntASCII(FILE *f)
{
	int i;

	fscanf(f, "%d", &i);

	return i;
}


vec_t Ase::readFloatASCII(FILE *f)
{
	vec_t r;

	fscanf(f, "%f", &r);

	return r;
}


void Ase::readVertex(FILE *f)
{
	int index;


	index = readIntASCII(f);
	mVertices[index][0] = readFloatASCII(f);
	mVertices[index][1] = readFloatASCII(f);
	mVertices[index][2] = readFloatASCII(f);

	if (mFlags & fWorldCoordXYZ)
	{
		convertWorldXYZ(mVertices[index]);
	}
}


void Ase::readUVW(FILE *f)
{
	int index;


	index = readIntASCII(f);
	mUVWs[index][0] = readFloatASCII(f);
	mUVWs[index][1] = readFloatASCII(f);
	mUVWs[index][2] = readFloatASCII(f);
	
	/* 2000.??.??: Mongoose - Ah, don't make it clamp to 0.0, 0.0 */
	if (mTexture.uTile)
		mUVWs[index][0] *= mTexture.uTile;
	
	if (mTexture.vTile)
		mUVWs[index][1] *= mTexture.vTile;
}


void Ase::readNormal(FILE *f)
{
	int index;
	

	index = readIntASCII(f);
	mNormals[index][0] = readFloatASCII(f);
	mNormals[index][1] = readFloatASCII(f);
	mNormals[index][2] = readFloatASCII(f);

	if (mFlags & fWorldCoordXYZ)
	{
		convertWorldXYZ(mNormals[index]);
	}
}


void Ase::readFaceNormal(FILE *f)
{
	int index;
	

	index = readIntASCII(f);
	mFaces[index].normal[0] = readFloatASCII(f);
	mFaces[index].normal[1] = readFloatASCII(f);
	mFaces[index].normal[2] = readFloatASCII(f);

	if (mFlags & fWorldCoordXYZ)
	{
		convertWorldXYZ(mFaces[index].normal);
	}
}


void Ase::readFace(FILE *f)
{
	int index;


	index = readIntASCII(f);

#ifdef OLD
	fscanf(f, ":\tA:\t%d B:\t%d C:\t%d", 
			 &mFaces[index].vertIndex[0],
			 &mFaces[index].vertIndex[1],
			 &mFaces[index].vertIndex[2]);
#else
	readSymbol(f, ":\t");
	readSymbol(f, "A:\t");
	mFaces[index].vertIndex[0] = readIntASCII(f);
	readSymbol(f, " B:\t");
	mFaces[index].vertIndex[1] = readIntASCII(f);
	readSymbol(f, " C:\t");
	mFaces[index].vertIndex[2] = readIntASCII(f);
#endif
}


void Ase::readTexFace(FILE *f)
{
	int index;


	index = readIntASCII(f);
	mFaces[index].uvwIndex[0] = readIntASCII(f);
	mFaces[index].uvwIndex[1] = readIntASCII(f);
	mFaces[index].uvwIndex[2] = readIntASCII(f);
}


void Ase::readTextureName(FILE *f)
{
	fscanf(f, " \"%s", mTexture.name);
	mTexture.name[strlen(mTexture.name) - 1] = '\0';
	mMaterialCount = 1;
}


void Ase::readModelData(FILE *f)
{
	char data[255];
	
	rewind(f);
	
	while (!feof(f))
	{
		fscanf (f, "%s", data);
		
		if (!strcmp(data, VERTEX))
			readVertex(f);
		else if (!strcmp(data, TVERTEX))
			readUVW(f);
		else if (!strcmp(data, NVERTEX))
			readNormal(f);
		else if (!strcmp(data, FACE_NORMAL))
			readFaceNormal(f);
		else if (!strcmp(data, FACE))
			readFace(f);
		else if (!strcmp(data, TFACE))
			readTexFace(f);
		else if (!strcmp(data, TEXTURE))
			readTextureName(f);
		else if (!strcmp(data, UTILE))
			mTexture.uTile = readFloatASCII(f);
		else if (!strcmp(data, VTILE))
			mTexture.vTile = readFloatASCII(f);
		else if (!strcmp(data, UOFFSET))
			mTexture.uOffset = readFloatASCII(f);
		else if (!strcmp(data, VOFFSET))
			mTexture.vOffset = readFloatASCII(f);
		else 
			fgets(data, sizeof(data), f);
	}
}


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS
#include <freyja/PluginABI.h>
#include <freyja/ModelABI.h>
#include <freyja/MeshABI.h>
#include <freyja/TextureABI.h>
#include <mstl/Map.h>

using namespace mstl;


extern "C" {

	int freyja_model__ase_check(char *filename);
	int freyja_model__ase_import(char *filename);
	int freyja_model__ase_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginName1s("ase");
	freyjaPluginDescription1s("ASE model (*.ase)");
	freyjaPluginAddExtention1s("*.ase");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_MESH);
}


int freyja_model__ase_check(char *filename)
{
	Ase ase;

	return ase.check(filename);
}


int import_model(char *filename)
{
	return freyja_model__ase_import(filename);
}


int freyja_model__ase_import(char *filename)
{
	Ase ase;
	Map<unsigned int, unsigned int> trans;
	Map<unsigned int, unsigned int> trans2;
	unsigned int i, textureId;


	if (ase.load(filename))
	{
		//freyjaPrintError("ase_import> ERROR Failed to load file.\n");
		return -1;
	}

	ase.printInfo();
	
	freyjaPrintMessage("ase_import> WARNING Not fully implemented\n");

	/* Load material texture */
	textureId = 0;

	if (ase.mMaterialCount > 0)
	{
		int texture = freyjaTextureCreateFilename(ase.mTexture.name);

		if (texture > -1)
			textureId = texture;
	}

	/* Start a new model */
	index_t model = freyjaModelCreate();
	index_t mesh = freyjaMeshCreate();
	freyjaModelAddMesh(model, mesh);

	for (i = 0; (int)i < ase.mVertexCount; ++i)
	{
		/* Store vertices in group */
		index_t vertex = freyjaMeshVertexCreate3fv(mesh, ase.mVertices[i]);
	   		
		/* Generates id translator list */
		trans.Add(i, vertex);
	}

	/* TexCoords */	
	for (i = 0; (int)i < ase.mUVWCount; ++i)
	{
		/* Store texels */
		index_t texcoord = freyjaMeshTexCoordCreate2f(mesh, ase.mUVWs[i][0], ase.mUVWs[i][1]);
		
		/* Generates id translator list */
		trans2.Add(i, texcoord);
	}

	/* Normals */
	if (ase.mNormalCount > 0)
	{
		for (i = 0; (int)i < ase.mVertexCount; ++i)
		{
			freyjaMeshVertexNormal3fv(mesh, trans[i], ase.mNormals[i]);
		}
	}

	/* Polygons */
	for (i = 0; (int)i < ase.mFaceCount; ++i)
	{
		/* Start a new polygon */
		index_t face = freyjaMeshPolygonCreate(mesh);

		for (uint32 j = 0, texcoord = 0, vertex = 0; j < 3; ++j)
		{ 
			/* Store vertices and texels by true id, using translator lists */
			vertex = trans[ase.mFaces[i].vertIndex[0]];
			freyjaMeshPolygonAddVertex1i(mesh, face, vertex);

			if (ase.mUVWCount)
			{
				texcoord = trans2[ase.mFaces[i].uvwIndex[j]];
				freyjaMeshPolygonAddTexCoord1i(mesh, face, texcoord);
			}
		}
		
		freyjaMeshPolygonMaterial(mesh, face, textureId);
	}


	return 0;
}


int freyja_model__ase_export(char *filename)
{	
	if (!freyjaGetMeshCount())
	{
		return -1;
	}
	
	index_t mesh = 0;

	for (uint32 i = 0; i < freyjaGetMeshCount(); ++i)
	{
		if (freyjaIsMeshSelected(mesh))
			mesh = i;
	}

	if (!freyjaIsMeshAllocated(mesh))
	{
		return -1;
	}

	Ase ase;
	ase.mVertexCount = freyjaGetMeshVertexCount(mesh);
	ase.mUVWCount = freyjaGetMeshTexCoordCount(mesh);
	ase.mFaceCount = freyjaGetMeshPolygonCount(mesh); 
	ase.mNormalCount = ase.mVertexCount;
	ase.mVertices = new vec3_t[ase.mVertexCount];
	ase.mNormals = new vec3_t[ase.mVertexCount];
	ase.mFaces = new ase_triangle_t[ase.mUVWCount];
	ase.mUVWs = new vec3_t[ase.mUVWCount];

	for (uint32 v = 0; (int)v < ase.mVertexCount; ++v)
	{
		freyjaGetMeshVertexPos3fv(mesh, v, ase.mVertices[v]);
		freyjaGetMeshVertexNormal3fv(mesh, v, ase.mNormals[v]);
	}

	for (uint32 f = 0, uv = 0, count = 0, tcount = 0; (int)f < ase.mFaceCount; ++f)
	{
		count = freyjaGetMeshPolygonVertexCount(mesh, f);
		
		for (uint32 i = 0; i < count; ++i)
		{
			index_t vertex = freyjaGetMeshPolygonVertexIndex(mesh, f, i);
			ase.mFaces[f].vertIndex[i] = vertex;
		}

		// Polymapped UVs
		tcount = freyjaGetMeshPolygonTexCoordCount(mesh, f);
		if (tcount)
		{
			for (uint32 i = 0; i < tcount; ++i)
			{
				uv = freyjaGetMeshPolygonTexCoordIndex(mesh, f, i);
				ase.mFaces[f].uvwIndex[i] = uv;
				freyjaGetMeshTexCoord2fv(mesh, uv, ase.mUVWs[uv]);
			}
		}
		else
		{
			tcount = count;
			for (uint32 i = 0; i < tcount; ++i)
			{
				index_t vertex = freyjaGetMeshPolygonVertexIndex(mesh, f, i);
				uv = freyjaGetMeshVertexTexCoord(mesh, vertex);
				ase.mFaces[f].uvwIndex[i] = uv;
				freyjaGetMeshTexCoord2fv(mesh, uv, ase.mUVWs[uv]);
			}
		}
	}

	//freyjaCriticalSectionUnlock();
	
	return (ase.save(filename));
}
#endif


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_ASE

void make_ase(Ase &ase)
{
	/*==========================================================
	 * FIXME: Add code here to generate a small ase model
	 *
	 *        Note I haven't added 'build your own' methods yet
	 *        so I can't make the driver for it yet  =)
	 ==========================================================*/
}


int runAseUnitTest(int argc, char *argv[])
{
	Ase ase;


	if (argc > 2)
	{
		if (strcmp(argv[1], "load") == 0)
		{
			if (!ase.load(argv[2]))
				printf("UNIT_TEST: Load reports success.\n");

			ase.printInfo();
		}
		else if (strcmp(argv[1], "save") == 0)
		{
			make_ase(ase);

			if (!ase.save(argv[2]))
				printf("UNIT_TEST: Save reports success.\n");
		}
		else if (strcmp(argv[1], "test") == 0 && argc > 3)
		{
			if (!ase.load(argv[2]))
				printf("UNIT_TEST: Load reports success.\n");

			ase.printInfo();

			if (!ase.save(argv[3]))
				printf("UNIT_TEST: Save reports success.\n");

			ase.printInfo();
		}
		else
			printf("\n\n%s [save | load | test] filename.ase [testout.ase]\n", 
					 argv[0]);
	}
	else
	{
		printf("\n\n%s [save | load | test] filename.ase [testout.ase]\n", 
				 argv[0]);
	}


	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Ase class test]\n");

	return runAseUnitTest(argc, argv);
}
#endif
