/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : GLMModel
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: GHOUL 2 model format plugin for freyja.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.12.22:
 * Mongoose - Created
 ==========================================================================*/

#include <freyja/FreyjaFileReader.h>

#include "GLMModel.h"



////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

GLMModel::GLMModel()
{
}


GLMModel::~GLMModel()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void GLMModel::print()
{
	long i, j, k, bone, weightCount;


	printf("mMDXMHeader.ident = 0x%x\n", mMDXMHeader.ident);
	printf("mMDXMHeader.version = %i\n", mMDXMHeader.version);
	printf("mMDXMHeader.name = '%s'\n", mMDXMHeader.name);
	printf("mMDXMHeader.animName = '%s'\n", mMDXMHeader.animName);
	printf("mMDXMHeader.animIndex = %i\n", mMDXMHeader.animIndex);
	printf("mMDXMHeader.numBones = %i\n", mMDXMHeader.numBones);
	printf("mMDXMHeader.numLODs = %i\n", mMDXMHeader.numLODs);
	printf("mMDXMHeader.ofsLODs = %i\n", mMDXMHeader.ofsLODs);
	printf("mMDXMHeader.numSurfaces = %i\n", mMDXMHeader.numSurfaces);
	printf("mMDXMHeader.ofsSurfHierarchy = %i\n", mMDXMHeader.ofsSurfHierarchy);
	printf("mMDXMHeader.ofsEnd = %i\n", mMDXMHeader.ofsEnd);


	for (i = 0; i < mMDXMHeader.numSurfaces; ++i)
	{
		printf("mMDXMHierarchyOffsets[%li] = %i\n", i,
		mMDXMHierarchyOffsets[i].offsets);
	}

	for (i = 0; i < mMDXMHeader.numSurfaces; ++i)
	{
		printf("mMDXMHierarchy[%li].name = '%s'\n", i,
			   mMDXMHierarchy[i].name);
		printf("mMDXMHierarchy[%li].flags = %i\n", i,
			   mMDXMHierarchy[i].flags);
		printf("mMDXMHierarchy[%li].shader = '%s'\n", i,
			   mMDXMHierarchy[i].shader);
		printf("mMDXMHierarchy[%li].shaderIndex = %i\n", i,
			   mMDXMHierarchy[i].shaderIndex);
		printf("mMDXMHierarchy[%li].parentIndex = %i\n", i,
			   mMDXMHierarchy[i].parentIndex);
		printf("mMDXMHierarchy[%li].numChildren = %i\n", i,
			   mMDXMHierarchy[i].numChildren);
		printf("mMDXMHierarchy[%li].childIndexes = {", i);

		for (j = 0; j < mMDXMHierarchy[i].numChildren; ++j)
			printf("%i ", mMDXMHierarchy[i].childIndexes[j]);

		printf("}\n");
	}

	for (i = 0; i < 1/*mMDXMHeader.numLODs*/; ++i)
	{
		printf("mMDXMLODs[%li].ofsEnd = %i\n", i,
			   mMDXMLODs[i].ofsEnd);
	}

	printf("mMDXMSurfOffsets[%li].offsets ={", i);

	for (i = 0; i < mMDXMHeader.numSurfaces; ++i)
	{
		printf("%i ", mMDXMSurfOffsets[i].offsets);
	}
	printf("}\n");


	for (i = 0; i < mMDXMHeader.numSurfaces; ++i)
	{
		printf("mMDXMSurfaces[%li].ident = %i\n", i, mMDXMSurfaces[i].ident);
		printf("mMDXMSurfaces[%li].thisSurfaceIndex = %i\n", i, mMDXMSurfaces[i].thisSurfaceIndex);
		printf("mMDXMSurfaces[%li].ofsHeader = %i\n", i, mMDXMSurfaces[i].ofsHeader);
		printf("mMDXMSurfaces[%li].numVerts = %i\n", i, mMDXMSurfaces[i].numVerts);
		printf("mMDXMSurfaces[%li].ofsVerts = %i\n", i, mMDXMSurfaces[i].ofsVerts);
		printf("mMDXMSurfaces[%li].numTriangles = %i\n", i, mMDXMSurfaces[i].numTriangles);
		printf("mMDXMSurfaces[%li].ofsTriangles = %i\n", i, mMDXMSurfaces[i].ofsTriangles);
		printf("mMDXMSurfaces[%li].numBoneReferences = %i\n", i, mMDXMSurfaces[i].numBoneReferences);
		printf("mMDXMSurfaces[%li].ofsBoneReferences = %i\n", i, mMDXMSurfaces[i].ofsBoneReferences);
		printf("mMDXMSurfaces[%li].ofsEnd = %i\n", i, mMDXMSurfaces[i].ofsEnd);

		for (j = 0; j < mMDXMeshes[i].triangleCount; ++j)
		{
			printf("mMDXMeshes[%li].triangles[%li].indexes = {%i %i %i}\n",
				   i, j,
				   mMDXMeshes[i].triangles[j].indexes[0],
				   mMDXMeshes[i].triangles[j].indexes[1],
				   mMDXMeshes[i].triangles[j].indexes[2]);
		}

		for (j = 0; j < mMDXMeshes[i].vertexCount; ++j)
		{
			printf("mMDXMeshes[%li].vertices[%li].normal = {%.3f %.3f %.3f}\n",
				   i, j,
				   mMDXMeshes[i].vertices[j].normal[0],
				   mMDXMeshes[i].vertices[j].normal[1],
				   mMDXMeshes[i].vertices[j].normal[2]);

			printf("mMDXMeshes[%li].vertices[%li].vertCoords = {%.3f %.3f %.3f}\n",
				   i, j,
				   mMDXMeshes[i].vertices[j].vertCoords[0],
				   mMDXMeshes[i].vertices[j].vertCoords[1],
				   mMDXMeshes[i].vertices[j].vertCoords[2]);

			printf("mMDXMeshes[%li].vertices[%li].uiNmWeightsAndBoneIndexes = 0x%X\n",
				   i, j, mMDXMeshes[i].vertices[j].uiNmWeightsAndBoneIndexes);

			weightCount = (mMDXMeshes[i].vertices[j].uiNmWeightsAndBoneIndexes>>30)+1;

			printf("-> %li weight(s) : ", weightCount);

			k = (mMDXMeshes[i].vertices[j].uiNmWeightsAndBoneIndexes>>30)+1;

			for (; k > 0; --k)
			{
				bone = (mMDXMeshes[i].vertices[j].uiNmWeightsAndBoneIndexes>>(iG2_BITS_PER_BONEREF*k))&((1<<iG2_BITS_PER_BONEREF)-1);
				printf("(%li, ?) ", bone);
			}

			printf("\n");
		}

		for (j = 0; j < mMDXMeshes[i].texcoordCount; ++j)
		{
			printf("mMDXMeshes[%li].texcoords[%li].texCoords = {%.3f %.3f}\n",
				   i, j, 
				   mMDXMeshes[i].texcoords[j].texCoords[0],
				   mMDXMeshes[i].texcoords[j].texCoords[1]);
		}
	}
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool GLMModel::load(const char *filename)
{
	FreyjaFileReader r;
	long i, j, k, weightCount, tell;

	
	if (!r.openFile(filename))
		return false;

	mMDXMHeader.ident = r.readLong();
	mMDXMHeader.version = r.readLong();
	r.readCharString(MAX_QPATH, mMDXMHeader.name);
	r.readCharString(MAX_QPATH, mMDXMHeader.animName);
	mMDXMHeader.animIndex = r.readLong();
	mMDXMHeader.numBones = r.readLong();
	mMDXMHeader.numLODs = r.readLong();
	mMDXMHeader.ofsLODs = r.readLong();
	mMDXMHeader.numSurfaces = r.readLong();
	mMDXMHeader.ofsSurfHierarchy = r.readLong();
	mMDXMHeader.ofsEnd = r.readLong();

	if (mMDXMHeader.ident != MDXM_IDENT)
	{
		r.closeFile();
		return false;
	}

	mMDXMHierarchyOffsets = new mdxmHierarchyOffsets_t[mMDXMHeader.numSurfaces];

	for (i = 0; i < mMDXMHeader.numSurfaces; ++i)
	{
		mMDXMHierarchyOffsets[i].offsets = r.readLong();
	}


	//r.setFileOffset(mMDXMHeader.ofsSurfHierarchy);
	mMDXMHierarchy = new mdxmSurfHierarchy_t[mMDXMHeader.numSurfaces];

	for (i = 0; i < mMDXMHeader.numSurfaces; ++i)
	{
		r.readCharString(MAX_QPATH, mMDXMHierarchy[i].name);
		mMDXMHierarchy[i].flags = r.readLong();
		r.readCharString(MAX_QPATH, mMDXMHierarchy[i].shader);
		mMDXMHierarchy[i].shaderIndex = r.readLong();
		mMDXMHierarchy[i].parentIndex = r.readLong();
		mMDXMHierarchy[i].numChildren = r.readLong();
		mMDXMHierarchy[i].childIndexes = new int[mMDXMHierarchy[i].numChildren];

		for (j = 0; j < mMDXMHierarchy[i].numChildren; ++j)
			mMDXMHierarchy[i].childIndexes[j] = r.readLong();
	}


	mMDXMLODs = new mdxmLOD_t[mMDXMHeader.numLODs];

	for (i = 0; i < 1/*mMDXMHeader.numLODs*/; ++i)
	{
		mMDXMLODs[i].ofsEnd = r.readLong();
	}


	mMDXMSurfOffsets = new mdxmLODSurfOffset_t[mMDXMHeader.numSurfaces];

	for (i = 0; i < mMDXMHeader.numSurfaces; ++i)
	{
		mMDXMSurfOffsets[i].offsets = r.readLong();
	}



	mMDXMSurfaces = new mdxmSurface_t[mMDXMHeader.numSurfaces];
	mMDXMeshes = new MDXMMesh_t[mMDXMHeader.numSurfaces];

	for (i = 0; i < mMDXMHeader.numSurfaces; ++i)
	{
		/* Offsets are relative to this header */
		tell = r.getFileOffset();

		mMDXMSurfaces[i].ident = r.readLong();
		mMDXMSurfaces[i].thisSurfaceIndex = r.readLong();
		mMDXMSurfaces[i].ofsHeader = r.readLong();
		mMDXMSurfaces[i].numVerts = r.readLong();
		mMDXMSurfaces[i].ofsVerts = r.readLong();
		mMDXMSurfaces[i].numTriangles = r.readLong();
		mMDXMSurfaces[i].ofsTriangles = r.readLong();
		mMDXMSurfaces[i].numBoneReferences = r.readLong();
		mMDXMSurfaces[i].ofsBoneReferences = r.readLong();
		mMDXMSurfaces[i].ofsEnd = r.readLong();

		mMDXMeshes[i].triangleCount = mMDXMSurfaces[i].numTriangles;
		mMDXMeshes[i].vertexCount = mMDXMSurfaces[i].numVerts;
		mMDXMeshes[i].texcoordCount = mMDXMSurfaces[i].numVerts;
		mMDXMeshes[i].triangles = new mdxmTriangle_t[mMDXMeshes[i].triangleCount];
		mMDXMeshes[i].vertices = new mdxmVertex_t[mMDXMeshes[i].vertexCount];
		mMDXMeshes[i].texcoords = new mdxmVertexTexCoord_t[mMDXMeshes[i].texcoordCount];

		for (j = 0; j < mMDXMeshes[i].triangleCount; ++j)
		{
			mMDXMeshes[i].triangles[j].indexes[0] = r.readLong();
			mMDXMeshes[i].triangles[j].indexes[1] = r.readLong();
			mMDXMeshes[i].triangles[j].indexes[2] = r.readLong();
		}

		for (j = 0; j < mMDXMeshes[i].vertexCount; ++j)
		{
			mMDXMeshes[i].vertices[j].normal[0] = r.readFloat32();
			mMDXMeshes[i].vertices[j].normal[1] = r.readFloat32();
			mMDXMeshes[i].vertices[j].normal[2] = r.readFloat32();

			mMDXMeshes[i].vertices[j].vertCoords[0] = r.readFloat32();
			mMDXMeshes[i].vertices[j].vertCoords[1] = r.readFloat32();
			mMDXMeshes[i].vertices[j].vertCoords[2] = r.readFloat32();

			mMDXMeshes[i].vertices[j].uiNmWeightsAndBoneIndexes = r.readLongU();

			weightCount = (mMDXMeshes[i].vertices[j].uiNmWeightsAndBoneIndexes>>30)+1;

			for (k = 0; k < iMAX_G2_BONEWEIGHTS_PER_VERT; ++k)
			{
				mMDXMeshes[i].vertices[j].BoneWeightings[k] = r.readInt8U();
			}

			k = (mMDXMeshes[i].vertices[j].uiNmWeightsAndBoneIndexes>>30)+1;
		}

		for (j = 0; j < mMDXMeshes[i].texcoordCount; ++j)
		{
			mMDXMeshes[i].texcoords[j].texCoords[0] = r.readFloat32();
			mMDXMeshes[i].texcoords[j].texCoords[1] = r.readFloat32();
		}

		for (j = 0; j < mMDXMSurfaces[i].numBoneReferences; ++j)
		{
			k = r.readLongU(); /* Bone references aren't stored, just read */
		}

		if ((int)r.getFileOffset() != tell + mMDXMSurfaces[i].ofsEnd)
		{
			printf("@ %i, expected %li\n", 
				   r.getFileOffset(), tell + mMDXMSurfaces[i].ofsEnd);
		}

		r.setFileOffset(tell + mMDXMSurfaces[i].ofsEnd);
	}

	//////////////

	printf("@ %i \n", r.getFileOffset());

	if ((i = r.readLongU()) == MDXA_IDENT)
	{
		printf("FOUNDMDXA_IDENT \n");
	}
	else
	{
		printf("%li 0x%lx\n", i, i);
	}


	r.closeFile();

	return true;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_GLMMODEL
int runGLMModelUnitTest(int argc, char *argv[])
{
	GLMModel test;

	test.load(argv[1]);
	test.print();

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[GLMModel class test]\n");

	return runGLMModelUnitTest(argc, argv);
}
#endif

////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS

#include <stdio.h>
#include <string.h>
#include <freyja/FreyjaPlugin.h>
#include <hel/Matrix.h>
#include "Nod.h"


extern "C" {

	int freyja_model__glm_check(char *filename);
	int freyja_model__glm_import(char *filename);
	int freyja_model__glm_export(char *filename);
	int import_model(char *filename);
}

int import_model(char *filename)
{
	if (!freyja_model__glm_check(filename))
		return freyja_model__glm_import(filename);

	return -1;
}


int freyja_model__glm_check(char *filename)
{
	FreyjaFileReader r;
	long ident, version;

	
	if (!r.openFile(filename))
		return false;

	ident = r.readLong();
	version = r.readLong();
	r.closeFile();

	if (ident == MDXM_IDENT && version == 6)
	{
		return 0;
	}	

	return -1;
}


int freyja_model__glm_import(char *filename)
{
	const vec_t scale = 0.5;
	Vector<long> vertices;
	GLMModel glm;
	Vector3d v;
	long index;
	int i, j;


	if (!glm.load(filename))
		return -1;

	freyjaBegin(FREYJA_MODEL);

	for (i = 0; i < glm.mMDXMHeader.numSurfaces; i++)
	{
		freyjaBegin(FREYJA_MATERIAL);
		glm.mMDXMHierarchy[i].shaderIndex = freyjaTextureFilename1s(glm.mMDXMHierarchy[i].shader);
		freyjaEnd();
	}

	for (i = 0; i < glm.mMDXMHeader.numSurfaces; ++i)
	{
		// Start a new mesh
		freyjaBegin(FREYJA_MESH);
    
		// Start a new vertex group
		freyjaBegin(FREYJA_VERTEX_GROUP);

		for (j = 0; j < glm.mMDXMeshes[i].vertexCount; ++j)
		{
			v = Vector3d(glm.mMDXMeshes[i].vertices[j].vertCoords);
			v *= scale;
			index = freyjaVertex3f(v.mVec[0], v.mVec[2], -v.mVec[1]);	
			
			v = Vector3d(glm.mMDXMeshes[i].vertices[j].normal);
			freyjaVertexNormal3f(index, v.mVec[0], v.mVec[2], -v.mVec[1]);
			freyjaVertexTexCoord2fv(index,
									glm.mMDXMeshes[i].texcoords[j].texCoords);

			vertices.pushBack(index);
		}
		
		freyjaEnd(); // FREYJA_GROUP

		for (j = 0; j < glm.mMDXMeshes[i].triangleCount; ++j)
		{
			// Start a new polygon
			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonVertex1i(vertices[glm.mMDXMeshes[i].triangles[j].indexes[0]]);
			freyjaPolygonVertex1i(vertices[glm.mMDXMeshes[i].triangles[j].indexes[1]]);
			freyjaPolygonVertex1i(vertices[glm.mMDXMeshes[i].triangles[j].indexes[2]]);

			if (glm.mMDXMHierarchy[i].shaderIndex-1 < 0)
			{
//#define GLMMODEL_USE_COLOURS
#ifdef GLMMODEL_USE_COLOURS
				freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.0, 1.0));
				freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.0, 1.0));
				freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.0, 1.0));
				freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.0, 1.0));
				freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.0, 1.0));
				freyjaPolygonTexCoord1i(freyjaTexCoord2f(0.0, 1.0));
				freyjaPolygonMaterial1i(-1);
#else
				freyjaPolygonMaterial1i(0);
#endif
			}
			else
			{
				freyjaPolygonMaterial1i(glm.mMDXMHierarchy[i].shaderIndex-1);
			}

			freyjaEnd(); // FREYJA_POLYGON
		}
		
		freyjaEnd(); // FREYJA_MESH

		vertices.clear();

	}

	freyjaEnd(); // FREYJA_MODEL

	return 0;
}


int freyja_model__glm_export(char *filename)
{
	freyjaPrintError("!freyja_model__glm_export> Not implemented, %s:%i\n", 
					 __FILE__, __LINE__);
	return -1;
}
#endif
