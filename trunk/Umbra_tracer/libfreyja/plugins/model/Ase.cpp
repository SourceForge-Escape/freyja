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
		printf("Ase::load> File may not be valid ASE file.\n");
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

	/* Look for a SKEL file matching the ASE
	 * Assume 3 letter extentions in filenames */
	char filenameSKEL[256];
	unsigned int len;

	snprintf(filenameSKEL, 255, "%s1", filename);
	len = strlen(filenameSKEL);
	filenameSKEL[len] = 0;
	filenameSKEL[len-1] = 'l';
	filenameSKEL[len-2] = 'e';
	filenameSKEL[len-3] = 'k';
	filenameSKEL[len-4] = 's';

	if (skel.load(filenameSKEL))
	{
		printf("Didn't find optional matching SKEL file\n");
		return 0;
	}

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


	/* Look for a SKEL file matching the ASE
	 * Assume 3 letter extentions in filenames */
	char filenameSKEL[256];
	unsigned int len;

	snprintf(filenameSKEL, 255, "%s1", filename);
	len = strlen(filenameSKEL);
	filenameSKEL[len] = 0;
	filenameSKEL[len-1] = 'l';
	filenameSKEL[len-2] = 'e';
	filenameSKEL[len-3] = 'k';
	filenameSKEL[len-4] = 's';

	if (skel.save(filenameSKEL))
	{
		printf("Didn't save optional matching SKEL file\n");
		return 0;
	}

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
#include <freyja/FreyjaPlugin.h>

extern "C" {

	int freyja_model__ase_check(char *filename);
	int freyja_model__ase_import(char *filename);
	int freyja_model__ase_export(char *filename);
	int import_model(char *filename);
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
	unsigned int i, j, v, t, textureId;


	if (ase.load(filename))
	{
		fprintf(stderr, "ase_import> ERROR Failed to load file.\n");
		return -1;
	}

	ase.printInfo();
	
	printf("ase_import> WARNING Not fully implemented\n");

	/* Start a new model */
	freyjaBegin(FREYJA_MODEL);

	textureId = 0;

	if (ase.mMaterialCount > 0)
	{
		freyjaBegin(FREYJA_MATERIAL);
		
		freyjaEnd();

		int texture = freyjaTextureFilename1s(ase.mTexture.name);

		if (texture > -1)
			textureId = texture;
	}

	/* Start a new mesh */
	freyjaBegin(FREYJA_MESH);
	
	/* Vertices */
	for (i = 0; (int)i < ase.mVertexCount; ++i)
	{
		/* Store vertices in group */
		v = freyjaVertex3fv(ase.mVertices[i]);
		
		/* Generates id translator list */
		trans.Add(i, v);
	}
	
	/* TexCoords */	
	for (i = 0; (int)i < ase.mUVWCount; ++i)
	{
		/* Store texels */
		t = freyjaTexCoord2f(ase.mUVWs[i][0], ase.mUVWs[i][1]);
		
		/* Generates id translator list */
		trans2.Add(i, t);
	}

	/* Normals */
	if (ase.mNormalCount > 0)
	{
		for (i = 0; (int)i < ase.mVertexCount; ++i)
		{
			//freyjaVertexNormal3fv(trans[i], ase.mNormals[i]);
			freyjaNormal3fv(ase.mNormals[i]);
		}
	}

	/* Polygons */
	for (i = 0; (int)i < ase.mFaceCount; ++i)
	{
		/* Start a new polygon */
		freyjaBegin(FREYJA_POLYGON);

		/* Store vertices and texels by true id, using translator lists */
		freyjaPolygonVertex1i(trans[ase.mFaces[i].vertIndex[0]]);
		freyjaPolygonVertex1i(trans[ase.mFaces[i].vertIndex[1]]);
		freyjaPolygonVertex1i(trans[ase.mFaces[i].vertIndex[2]]);

		if (!ase.mUVWCount)
		{
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.0, 0.5));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.5, 0.5));
			freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.0, 0.0));			
		}
		else
		{
			freyjaPolygonTexCoord1i(trans2[ase.mFaces[i].uvwIndex[0]]);
			freyjaPolygonTexCoord1i(trans2[ase.mFaces[i].uvwIndex[1]]);
			freyjaPolygonTexCoord1i(trans2[ase.mFaces[i].uvwIndex[2]]);
		}
		
		freyjaPolygonMaterial1i(textureId);
		
		freyjaEnd(); // FREYJA_POLYGON
	}

	freyjaEnd(); // FREYJA_MESH
	freyjaEnd(); // FREYJA_MODEL


	/* Skeleton */
	if (ase.skel.mBoneCount)
	{
		ase_weight_EXT_t *weight;
		
		for (i = 0; i < ase.skel.mWeightCount; ++i)
		{
			weight = &ase.skel.mWeights[i];
			
			while (weight)
			{
				freyjaVertexWeight(i, 
								   ase.skel.mWeights[i].weight,
								   ase.skel.mWeights[i].boneId);
				weight = weight->next;
			}
		}

		freyjaBegin(FREYJA_SKELETON);

		for (i = 0; i < ase.skel.mBoneCount; ++i)
		{
			/* Start a new tag */
			freyjaBegin(FREYJA_BONE);
			//freyjaTagFlags1u(0x0);
			freyjaBoneName(ase.skel.mBones[i].name);
			freyjaBonePos3f(ase.skel.mBones[i].translation[0],
						   ase.skel.mBones[i].translation[1],
						   ase.skel.mBones[i].translation[2]);
			freyjaBoneRotateQuaternion4f(ase.skel.mBones[i].rotation[0], // w?
										 ase.skel.mBones[i].rotation[1],
										 ase.skel.mBones[i].rotation[2],
										 ase.skel.mBones[i].rotation[3]);
			
			printf("++ Bone[%d] :: ", i);
			
			for (j = 0; j <  ase.skel.mBones[i].childrenCount; ++j)
			{
				printf("%d ", ase.skel.mBones[i].children[j]);
				freyjaBoneAddChild1u(ase.skel.mBones[i].children[j]);
			}
			
			printf("\n");
			
			freyjaEnd(); // FREYJA_TAG
			
			//	freyjaMeshTreeTag1u(t);
		}

		freyjaEnd(); // FREYJA_BONE_FRAME
	}

	return 0;
}


int freyja_model__ase_export(char *filename)
{
	Map<unsigned int, unsigned int> trans;
	unsigned int vert;
	float st[2];
	int v, t, texel;
	unsigned int i, j, b, bone;
	Ase ase;
	

	printf("ase_export> WARNING Not fully implemented\n");
	
	if (!freyjaGetCount(FREYJA_MESH))
	{
		return -1;
	}
	
	// Don't allow use of internal iterators or
	// changes of data by other processes
	freyjaCriticalSection(FREYJA_WRITE_LOCK);
	
	ase.mVertexCount = freyjaGetCount(FREYJA_VERTEX);
	ase.mUVWCount = freyjaGetCount(FREYJA_POLYGON) * 3;
	ase.mFaceCount = freyjaGetCount(FREYJA_POLYGON); 
	ase.mNormalCount = ase.mVertexCount;

	ase.mVertices = new vec3_t[ase.mVertexCount];
	ase.mNormals = new vec3_t[ase.mVertexCount];
	ase.mFaces = new ase_triangle_t[ase.mUVWCount];
	ase.mUVWs = new vec3_t[ase.mUVWCount];
	
	/* Using freyja iterator interface */
	freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_RESET);
	
	for (v = 0; v < ase.mVertexCount; ++v)
	{
		freyjaGetVertex3fv(freyjaGetCurrent(FREYJA_VERTEX), ase.mVertices[v]);
		freyjaGetNormal3fv(freyjaGetCurrent(FREYJA_VERTEX), ase.mNormals[v]);
		
		// Use translator list
		vert = freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_CURRENT);
		trans.Add(vert, v);
		
		//printf("trans.Add(%i, %i)\n", vert, v);
		freyjaIterator(FREYJA_VERTEX, FREYJA_LIST_NEXT);
	}
	
	// Using list interface, as opposed to array
	freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_RESET);
	
	for (t = 0, texel = 0; t < ase.mFaceCount; ++t)
	{
		// Use translator list
		freyjaGetPolygon1u(FREYJA_VERTEX, 0, &vert);
		
		//printf("trans[%i] = %i\n", vert, trans[vert]);
		
		ase.mFaces[t].vertIndex[0] = trans[vert];
		freyjaGetPolygon1u(FREYJA_VERTEX, 1, &vert);
		ase.mFaces[t].vertIndex[1] = trans[vert];
		freyjaGetPolygon1u(FREYJA_VERTEX, 2, &vert);
		ase.mFaces[t].vertIndex[2] = trans[vert];
		
		// FIXME: Texels aren't right for multiple texels per vertex
		freyjaGetPolygon3f(FREYJA_TEXCOORD, 0, st);
		ase.mFaces[t].uvwIndex[0] = texel++;
		ase.mUVWs[ase.mFaces[t].uvwIndex[0]][0] = st[0];
		ase.mUVWs[ase.mFaces[t].uvwIndex[0]][1] = st[1];
		
		freyjaGetPolygon3f(FREYJA_TEXCOORD, 1, st);
		ase.mFaces[t].uvwIndex[1] = texel++;
		ase.mUVWs[ase.mFaces[t].uvwIndex[1]][0] = st[0];
		ase.mUVWs[ase.mFaces[t].uvwIndex[1]][1] = st[1];
		
		freyjaGetPolygon3f(FREYJA_TEXCOORD, 2, st);
		ase.mFaces[t].uvwIndex[2] = texel++;
		ase.mUVWs[ase.mFaces[t].uvwIndex[2]][0] = st[0];
		ase.mUVWs[ase.mFaces[t].uvwIndex[2]][1] = st[1];
		
		// Using list interface, as opposed to array
		freyjaIterator(FREYJA_POLYGON, FREYJA_LIST_NEXT);
	}
	

	/* SKELETON */
	freyjaIterator(FREYJA_SKELETON, FREYJA_LIST_RESET);

	ase.skel.mBoneCount = freyjaGetCount(FREYJA_BONE);
	ase.skel.mBones = new ase_bone_EXT_t[ase.skel.mBoneCount];
	freyjaIterator(FREYJA_BONE, FREYJA_LIST_RESET);

	for (b = 0; b < ase.skel.mBoneCount; ++b)
	{
		ase.skel.mBones[b].childrenCount = 0;
	}

	for (b = 0; b < ase.skel.mBoneCount; ++b)
	{
		bone = freyjaIterator(FREYJA_BONE, FREYJA_LIST_CURRENT);
		bone = freyjaGetCurrent(FREYJA_BONE);

		ase.skel.mBones[b].parent = freyjaGetBoneParent(bone);
		freyjaGetBoneRotationXYZW4fv(bone, ase.skel.mBones[b].rotation);
		freyjaGetBoneTranslation3fv(bone, ase.skel.mBones[b].translation);
		freyjaGetBoneName(bone, 64, ase.skel.mBones[b].name);

		ase.skel.mBones[ase.skel.mBones[b].parent].childrenCount++;

		printf("ase.so: bone[%i].parent = %i\n", 
			   b, ase.skel.mBones[b].parent);

		// Use translator list
		//trans.Add(bone, b);
		//printf("trans.Add(%i, %i)\n", bone, b);

		freyjaIterator(FREYJA_BONE, FREYJA_LIST_NEXT);
	}

	// Loop to populate children lists here
	for (b = 0; b < ase.skel.mBoneCount; ++b)
	{
		if (ase.skel.mBones[b].childrenCount == 0)
			continue;

		ase.skel.mBones[b].children = new unsigned int[ase.skel.mBones[b].childrenCount];

		for (i = 0, j  = 0; i < ase.skel.mBoneCount; ++i)
		{
			if (ase.skel.mBones[i].parent == (int)b)
				ase.skel.mBones[b].children[j++] = i;
		}
	}

	freyjaCriticalSection(FREYJA_WRITE_UNLOCK);
	
	return (ase.save(filename));
}
#endif


AseSkelEXT::AseSkelEXT()
{
	mBoneCount = 0;
	mWeightCount = 0;
	
	mName[0] = 0;
	mBones = 0x0;
	mWeights = 0x0;
}


AseSkelEXT::~AseSkelEXT()
{
}


int AseSkelEXT::load(char *filename)
{
	FILE *f;
	char buffer[256];
	unsigned int i, j, k, a, b, state, last;
	float n;
	char c;
	ase_weight_EXT_t *weight;


	if (!filename || !filename[0])
		return -1;
	
	f = fopen(filename, "r");

	if (!f)
	{
		perror(filename);
		return -2;
	}

	printf("Loading SKEL\n"); //  %s...\n", filename);

	i = 0;
	state = 0;
	last = 100;

	while (fscanf(f, "%c", &c) != EOF && state != 666)
	{
		// printf("%c\n", c);

		switch (c)
		{
		case '\"':
		case '\t':
			break;
		case '\n':
			// printf("LINE  %s\n", buffer);

			switch (state)
			{
			case 0:
				if (!strncmp(buffer, "*SKELETON {", 11)) 
					state = 1;
				break;
			case 1:
				if (!strncmp(buffer, "*BONE_WEIGHTS {", 14))
				{
					state = 2;
				}
				else if (!strncmp(buffer, "*NAME ", 6))
				{
					strncpy(mName, (*(&buffer)+6), 64);
				}
				else if (!strncmp(buffer, "*BONE_COUNT ", 12))
				{
					mBoneCount = atoi(*(&buffer)+12);
					mBones = new ase_bone_EXT_t[mBoneCount];
				}
				else if (!strncmp(buffer, "*WEIGHT_COUNT ", 14))
				{
					mWeightCount = atoi(*(&buffer)+14);
					mWeights = new ase_weight_EXT_t[mWeightCount];
				}
				break;
			case 2:
				if (!strncmp(buffer, "*WEIGHT", 7)) 
				{
					sscanf(buffer, "*WEIGHT %d BONE: %d %f", &a, &b, &n);

					if (last != a)
					{
						mWeights[a].weight = n;
						mWeights[a].boneId = b;
						mWeights[a].next = 0x0;

						weight = &mWeights[a];
					}
					else
					{
						weight = weight->next = new ase_weight_EXT_t;

						weight->weight = n;
						weight->boneId = b;
						weight->next = 0x0;
					}

					last = a;
				}				
				else if (!strncmp(buffer, "}", 1)) 
				{
					state = 3;
				}
				break;
			case 3:
				if (!strncmp(buffer, "*BONE {", 7)) 
				{
					b = 0;
				}
				else if (!strncmp(buffer, "*ID ", 4))
				{
					sscanf(buffer, "*ID %d", &b);
					mBones[b].id = b;
					mBones[b].name[0] = 0;
				}
				else if (!strncmp(buffer, "*PARENT ", 7))
				{
					sscanf(buffer, "*PARENT %d", &mBones[b].parent);
				}
				else if (!strncmp(buffer, "*NAME ", 6))
				{
					sscanf(buffer, "*NAME %s", mBones[b].name);
				}
				else if (!strncmp(buffer, "*CHILDREN_COUNT ", 16))
				{
					sscanf(buffer, "*CHILDREN_COUNT %d\n", &mBones[b].childrenCount);
				}
				else if (!strncmp(buffer, "*TRANSLATE_XYZ ", 15))
				{
					sscanf(buffer, "*TRANSLATE_XYZ %f %f %f",
						   &mBones[b].translation[0],
						   &mBones[b].translation[1],
						   &mBones[b].translation[2]);
				}
				else if (!strncmp(buffer, "*ROTATE_XYZW ", 12))
				{
					sscanf(buffer, "*ROTATE_XYZW %f %f %f %f",
						   &mBones[b].rotation[0],
						   &mBones[b].rotation[1],
						   &mBones[b].rotation[2],
						   &mBones[b].rotation[3]);
				}
				if (!strncmp(buffer, "}", 1)) 
				{
				}
				break;
			}

			buffer[0] = i = 0;
			break;
		default:
			buffer[i++] = c;
			buffer[i] = 0;
		}
	}


	/* Setup bone's children feild */
	for (i = 0; i < mBoneCount; ++i)
	{
		printf("++ Bone[%d] :: ", i);

		if (mBones[i].childrenCount > 0)
		{
			mBones[i].children = new unsigned int[mBones[i].childrenCount];

			for (j = 0, k = 0; j < mBoneCount; ++j)
			{
				if (mBones[j].parent == (int)i)// && k < mBones[i].childrenCount)
				{
					if (j == i) // Don't self link, jesus
					{
						//mBones[i].childrenCount--;
					}
					else
					{
						printf(" %d ", j);
						mBones[i].children[k++] = j; 
					}
				}
			}
		}

		printf("\n");
	}

	fclose(f);

	return 0;
}


void AseSkelEXT::print()
{
	unsigned int i, j;


	for (i = 0; i < mBoneCount; ++i)
	{
		printf("Bone[%d] { ", i);
	
		for (j = 0; j < mBones[i].childrenCount; ++j)
		{
			printf("%d ", mBones[i].children[j]);
		}

		printf("}\n");
	}
}


int AseSkelEXT::save(char *filename)
{
	FILE *f;
	unsigned int a, b;
	ase_weight_EXT_t *weight;


	if (!filename || !filename[0])
	{
		return -1;
	}

	f = fopen(filename, "w");

	if (!f)
	{
		perror(filename);
		return -2;
	}

	fprintf(f, "*SKELETON {\n");
	fprintf(f, "\t*NAME \"%s\"\n", "RestSkeleton");
	fprintf(f, "\t*BONE_COUNT %d\n", mBoneCount);
	fprintf(f, "\t*WEIGHT_COUNT %d\n", mWeightCount);

	fprintf(f, "\t*BONE_WEIGHTS {\n");

	for (a = 0; a < mWeightCount; ++a)
	{
		weight = &mWeights[a];

		while (weight)
		{
			fprintf(f, "\t\t*WEIGHT %d BONE: %d %f\n", 
					a, mWeights[a].boneId, mWeights[a].weight);

			weight = weight->next;
		}
	}
	
	fprintf(f, "\t}\n");

	for (b = 0; b < mBoneCount; ++b)
	{
		fprintf(f, "\t*BONE {\n");
		fprintf(f, "\t\t*ID %d\n", mBones[b].id);
		fprintf(f, "\t\t*PARENT %d\n", mBones[b].parent);
		fprintf(f, "\t\t*CHILDREN_COUNT %d\n", mBones[b].childrenCount);
		fprintf(f, "\t\t*TRANSLATE_XYZ %f %f %f\n",
				mBones[b].translation[1],
				mBones[b].translation[0],
				mBones[b].translation[2]);
		fprintf(f, "\t\t*ROTATE_XYZW %f %f %f %f\n",
				mBones[b].rotation[0],
				mBones[b].rotation[1],
				mBones[b].rotation[2],
				mBones[b].rotation[3]);
		fprintf(f, "\t}\n");
	}

	fprintf(f, "}\n");

	fclose(f);

	return 0;
}


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
			ase.skel.print();
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
