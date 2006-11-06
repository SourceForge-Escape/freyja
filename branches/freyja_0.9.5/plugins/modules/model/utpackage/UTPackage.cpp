/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : OpenRaider, PSKModel viewer, libhel_modelloader
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : UTPackage
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: This class handles UT packages
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2003.07.11:
 * Mongoose - Created, form old PSKModel prototype
 =================================================================*/

#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#ifdef WIN32
#   include <direct.h>
#endif

#include "URTexture.h"

#include "UTPackage.h"


// FIXME: fseek() calls could be wrapped to support dRead by
//        adjusting the file start position by 28bytes
//        right now that adjustment is made for each call

#define USE_PROPERTY_STRUCT


/* Hexdump util macros */
#define hex_dump_file(f, size) hexDump(f, 0x0, size)
#define hex_dump_buffer(buffer, size) hexDump(0x0, buffer, size)


unsigned char *buffer_from_utpak(FILE *f, unsigned int signature, unsigned int offset, unsigned int size, unsigned char key)
{
	unsigned char *buffer = 0x0;
	unsigned int i;


	if (!f)
	{
		return 0x0;
	}

	i = ftell(f);
	fseek(f, offset, SEEK_SET);
	buffer = new unsigned char[size];
	fread(buffer, 1, size, f);
	fseek(f, i, SEEK_SET);

	return buffer;
}


void decryptBufferXOR(unsigned char *buffer, unsigned int size, unsigned char key)
{
	unsigned int i;


	for (i = 0; i < size; ++i)
	{
		buffer[i] ^= key;
	}
}


int decryptDumpXOR(const char *filename, unsigned char key, FILE *in)
{
	FILE *out;
	unsigned int data;


	if (!in)
		return -1;

	out = fopen(filename, "wb");

	if (!out)
		return -2;

	printf("KEY is 0x%X\n", key);

	while (fread(&data, sizeof(data), 1, in))
	{
		decryptBufferXOR((unsigned char*)&data, 4, key);
		fwrite(&data, sizeof(data), 1, out);
	}

	fclose(out);

	return 0;
}



int read_index(FILE *f)
{
	int val;
	char b0, b1, b2, b3, b4;
                                                                               
	val = 0;
   
	fread(&b0, 1, 1, f);
   
	if (b0 & 0x40)
	{
		fread(&b1, 1, 1, f);
 
		if (b1 & 0x80)
		{
			fread(&b2, 1, 1, f);
 
			if (b2 & 0x80)
			{
				fread(&b3, 1, 1, f);
 
				if (b3 & 0x80)
				{
					fread(&b4, 1, 1, f);
					val = b4;
				}
				
				val = (val << 7) + (b3 & 0x7f);
			}
			
			val = (val << 7) + (b2 & 0x7f);
		}
		
		val = (val << 7) + (b1 & 0x7f);
	}
	
	val = (val << 6) + (b0 & 0x3f);
   
	if (b0 & 0x80)
		val = -val;

	return val;
}


//////////////////////////////////////////////////////////////////////
// UTPackageProperty Class
//////////////////////////////////////////////////////////////////////

UTPackageProperty::UTPackageProperty()
{
	index = 0;
	nameIndex = 0;
	size = 0;
	type = 0;
	flags = 0;

	data = 0x0;
}


UTPackageProperty::~UTPackageProperty()
{
	clear();
}


void UTPackageProperty::clear()
{
	index = 0;
	nameIndex = 0;
	size = 0;
	type = 0;
	flags = 0;

	if (data)
		delete [] data;
}


//////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

UTPackage::UTPackage()
{
	mFlags = 0;
	mKeyXOR = 0;  /* Don't decrypt by default */
	mOffset = 0;
	mRootDir = NULL;
	mStream = NULL;
	mDecyrptFilename = NULL;
}


UTPackage::~UTPackage()
{
	if (mRootDir)
		delete [] mRootDir; 
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

int UTPackage::loadProperty(UTPackageProperty &prop)
{
	return loadProperty(prop, &mHeader, mStream);
}


unsigned int UTPackage::dReadIndexValue()
{
	return getIndexValue(mStream);
}

size_t UTPackage::dSeek(long offset, int wence)
{
	offset = (wence == SEEK_SET) ? offset + mOffset : offset;

	return fseek(mStream, offset, wence);
}


long UTPackage::dTell()
{
	return ftell(mStream);
}


size_t UTPackage::dRead(void *ptr, size_t size, size_t nmemb)
{
	return dRead(ptr, size, nmemb, mStream);
}


size_t UTPackage::dRead(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t ret = 0;


	switch (mKeyXOR)
	{
	case 0:
		ret = fread(ptr, size, nmemb, stream);
		break;
	default:
		ret = fread(ptr, size, nmemb, stream);
		decryptBufferXOR((unsigned char*)ptr, (size*nmemb), mKeyXOR);
	}

	return ret;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

/* Ignore the table type and get unsigned numeric value */
unsigned int UTPackage::getIndexValue(FILE *f)
{
	unsigned int type;
	int idx;


	idx = getIndex(f);
	return useIndex(idx, &type) + 1;
}


int UTPackage::loadMesh(FILE *f)
{
	unsigned int offset; // u32
	// unsigned short u16;
	int idx;
	UTPackageProperty prop;
	UTMesh mesh;


	offset = ftell(f);

	/* Unreal mesh class */	
	printf("Loading mesh class...\n");

#ifdef FIXME
	/* Property list */
	while ((idx = loadProperty(prop)) == 0)
	{
		printf("*** Property '%s' ***************\n", 
				 mHeader.nameTable[prop.nameIndex].objName);

		if (strcmp("None", prop.name) == 0)
		{
			break;
		}
	}

	if (idx == -2)
	{
		printf("FATAL ERROR: Invalid property returned for class!\n");
		return -1;
	}
#else
	idx = dReadIndexValue();  // If always "None" then this will work
#endif

#define DONT_IGNORE_PRIMITIVES
#ifdef DONT_IGNORE_PRIMITIVES
	dRead(&mesh.mPrimativeBoundingbox.min.x, 4, 1, f);
	dRead(&mesh.mPrimativeBoundingbox.min.y, 4, 1, f);
	dRead(&mesh.mPrimativeBoundingbox.min.z, 4, 1, f);
	dRead(&mesh.mPrimativeBoundingbox.max.x, 4, 1, f);
	dRead(&mesh.mPrimativeBoundingbox.max.y, 4, 1, f);
	dRead(&mesh.mPrimativeBoundingbox.max.z, 4, 1, f);
	dRead(&mesh.mPrimativeBoundingbox.isValid, 1, 1, f);

	printf("PrimativeBoundingbox = {\nmin %f %f %f\nmax %f %f %f\nv %u\n}\n",
			 mesh.mPrimativeBoundingbox.min.x,
			 mesh.mPrimativeBoundingbox.min.y,
			 mesh.mPrimativeBoundingbox.min.z,
			 mesh.mPrimativeBoundingbox.max.x,
			 mesh.mPrimativeBoundingbox.max.y,
			 mesh.mPrimativeBoundingbox.max.z,
			 mesh.mPrimativeBoundingbox.isValid);

	dRead(&mesh.mPrimativeBoundingSphere.pos.x, 4, 1, f);
	dRead(&mesh.mPrimativeBoundingSphere.pos.y, 4, 1, f);
	dRead(&mesh.mPrimativeBoundingSphere.pos.z, 4, 1, f);	

	if (mHeader.version > 61)
	{
		dRead(&mesh.mPrimativeBoundingSphere.w, 4, 1, f);
	}
	else
	{
		mesh.mPrimativeBoundingSphere.w = -1;
	}

	printf("PrimativeBoundingSphere = {\npos %f %f %f\nw %f\n}\n",
			 mesh.mPrimativeBoundingSphere.pos.x,
			 mesh.mPrimativeBoundingSphere.pos.x,
			 mesh.mPrimativeBoundingSphere.pos.x,
			 mesh.mPrimativeBoundingSphere.w);
#endif

	mesh.mVertexJump = 0;

	if (mHeader.version > 61)
	{
		dRead(&mesh.mVertexJump, 4, 1, f);
	}

	mesh.mVertexCount = dReadIndexValue();
	printf("VertexCount = %u, off = %u\n", 
			 mesh.mVertexCount, mesh.mVertexJump);

#ifdef FIXME

	unsigned int mVertexJump;  /* In package version > 61 only */
	unsigned int mVertexCount; /* INDEX */
	UTMesh_Point *mVertices; /* UT diskformat differs from DEUS EX 
									  *------------------------------------
									  *	UT: DWORD xyz
									  * x = (xyz && 0x7ff) / 8;
									  * x = - ((x > 128) ? (x - 256) : x);
									  * y = ((xyz >> 11) && 0x7ff) / 8;
									  * y = - ((y > 128) ? (y - 256) : y);
									  * z = ((xyz >> 22) && 0x3ff) / 4;
									  * z = ((z > 128) ? (z - 256) : z);
									  *------------------------------------
									  *	DeusEx: QUADWORD xyz
									  * x = (xyz && 0xffff) / 256;
									  * x = - ((x > 128) ? (x - 256) : x);
									  * y = ((xyz >> 16) && 0xffff) / 256;
									  * y = - ((y > 128) ? (y - 256) : y);
									  * z = ((xyz >> 32) && 0xffff) / 256;
									  * z = ((z > 128) ? (z - 256) : z);
									  *------------------------------------
									  */

	unsigned int mTriangleJump;  /* In package version > 61 only */

	unsigned int mTriangleCount; /* INDEX */
	UTMesh_Triangle *mTriangles;

	unsigned int mAnimSequenceCount; /* INDEX */
	UTMesh_AnimSequence *mAnimSequences;

	unsigned int mConnectsJump;
	unsigned int mConnectsCount; /* INDEX */
	UTMesh_Connect *mConnects;
	
	UTBoundingBox mBoundingBox;
	UTBoundingSphere mBoundingSphere;

	unsigned int mVertexLinksJump;
	unsigned int mVertexLinksCount; /* INDEX */
	unsigned int *mVertexLinks;

	unsigned int mTextureCount; /* INDEX */
	unsigned int *mTextures; /* INDEXs, Object references */

	unsigned int mBoundingBoxesCount; /* INDEX */
	UTBoundingBox *mBoundingBoxes;

	unsigned int mBoundingSpheresCount; /* INDEX */
	UTBoundingSphere *mBoundingSpheres;

	unsigned int mFrameVertices;
	unsigned int mAnimFrames;
	unsigned int mANDFlags;
	unsigned int mORFlags;

	UTVector mScale;
	UTVector mOrigin;

	unsigned int mCurrentPolygon;
	unsigned int mCurrentVertex;

	
	unsigned int mTextureLODCount; /* INDEX, 
											  * Only read if Package version >= 66,
											  *
											  * If version == 65, then set this to 1
											  * and read 1 float for mTextureLOD
											  */
	
	float mTextureLOD;

#endif

	printf("Read %lu bytes from Mesh\n", ftell(f) - offset);

	return 0;
}


int UTPackage::loadSkeletalMesh(FILE *f)
{
	unsigned int i, type, animNameIndex, offset; // u32
	// unsigned short u16;
	int idx;
	float r;
	UTPackageProperty prop;
	UTSkeletalMesh mesh;


	offset = ftell(f);

	//#define TEST_SERIALIZED_PARENT_CLASS
#ifdef TEST_SERIALIZED_PARENT_CLASS
	loadMesh(f);
#else
	idx = dReadIndexValue();  // If always "None" then this will work
#endif

	/* Unreal Skeletal mesh class */	
	printf("Loading skeletal mesh class...\n");
	
	/* mExtWedges */
	mesh.mExtWedgeCount = getIndexValue(f);
	printf("mExtWedgeCount = %i\n", mesh.mExtWedgeCount);
	
	mesh.mExtWedges = new UTSkeletalMesh_ExtWedge[mesh.mExtWedgeCount];

	for (i = 0; i < mesh.mExtWedgeCount; ++i)
	{
		dRead(&mesh.mExtWedges[i].vertexIndex, 2, 1, f);
		dRead(&mesh.mExtWedges[i].flags, 2, 1, f);
		dRead(&mesh.mExtWedges[i].u, 4, 1, f);
		dRead(&mesh.mExtWedges[i].v, 4, 1, f);
	}
	
	/* mPoints */
	mesh.mPointCount = getIndexValue(f);
	printf("mesh.mPointCount = %i\n", mesh.mPointCount);

	mesh.mPoints = new UTSkeletalMesh_Point[mesh.mPointCount];

	for (i = 0; i < mesh.mPointCount; ++i)
	{
		dRead(&mesh.mPoints[i].x, 4, 1, f);
		dRead(&mesh.mPoints[i].y, 4, 1, f);
		dRead(&mesh.mPoints[i].z, 4, 1, f);
	}
	
	/* mBones */
	mesh.mBoneCount = getIndexValue(f);
	printf("mesh.mBoneCount = %i\n", mesh.mBoneCount);
	mesh.mBones = new UTSkeletalMesh_Bone[mesh.mBoneCount];

	for (i = 0; i < mesh.mBoneCount; ++i)
	{
		idx = getIndex(f);
		animNameIndex = useIndex(idx, &type) + 1;
		//printf("INDEX of default animation: %i %u\n", idx, animNameIndex);

		strcpy(mesh.mBones[i].name, "NotImplementedYet");

		dRead(&mesh.mBones[i].flags, 4, 1, f);

		dRead(&r, 4, 1, f);
		mesh.mBones[i].restDir[0] = r;
		dRead(&r, 4, 1, f);
		mesh.mBones[i].restDir[1] = r;
		dRead(&r, 4, 1, f);
		mesh.mBones[i].restDir[2] = r;
		dRead(&r, 4, 1, f);
		mesh.mBones[i].restDir[3] = r;

		dRead(&r, 4, 1, f);
		mesh.mBones[i].restLoc[0] = r;
		dRead(&r, 4, 1, f);
		mesh.mBones[i].restLoc[1] = r;
		dRead(&r, 4, 1, f);
		mesh.mBones[i].restLoc[2] = r;

		dRead(&mesh.mBones[i].length, 1, 4, f);

		dRead(&mesh.mBones[i].xSize, 1, 4, f);
		dRead(&mesh.mBones[i].ySize, 1, 4, f);
		dRead(&mesh.mBones[i].zSize, 1, 4, f);

		dRead(&mesh.mBones[i].numChildren, 4, 1, f);
		dRead(&mesh.mBones[i].parentIndex, 4, 1, f);
	}

	/* mWeightIndices */
	mesh.mWeightIndexCount = getIndexValue(f);
	printf("mesh.mWeightIndexCount = %i\n", mesh.mWeightIndexCount);
	mesh.mWeightIndices =new UTSkeletalMesh_WeightIndex[mesh.mWeightIndexCount];

	for (i = 0; i < mesh.mWeightIndexCount; ++i)
	{
		dRead(&mesh.mWeightIndices[i].weightIndex, 1, 2, f);
		dRead(&mesh.mWeightIndices[i].number, 1, 2, f);
		dRead(&mesh.mWeightIndices[i].detailA, 1, 2, f);
		dRead(&mesh.mWeightIndices[i].detailB, 1, 2, f);
	}

	/* mWeights */
	mesh.mWeightCount = getIndexValue(f);
	printf("mesh.mWeightCount = %i\n", mesh.mWeightCount);
	mesh.mWeights =new UTSkeletalMesh_Weight[mesh.mWeightCount];

	for (i = 0; i < mesh.mWeightCount; ++i)
	{
		dRead(&mesh.mWeights[i].pointIndex, 1, 2, f);
		dRead(&mesh.mWeights[i].boneWeight, 1, 2, f);
	}
	
	/* mLocalPoints */
	mesh.mLocalPointCount = getIndexValue(f);
	printf("mesh.mLocalPointCount = %i\n", mesh.mLocalPointCount);
	mesh.mLocalPoints = new UTSkeletalMesh_Point[mesh.mLocalPointCount];

	for (i = 0; i < mesh.mLocalPointCount; ++i)
	{
		dRead(&mesh.mLocalPoints[i].x, 1, 4, f);
		dRead(&mesh.mLocalPoints[i].y, 1, 4, f);
		dRead(&mesh.mLocalPoints[i].z, 1, 4, f);
	}
	
	/* Skeletal depth */
	dRead(&mesh.mSkeletalDepth, 1, 4, f);

	/* Default animation name */
	idx = getIndex(f);
	animNameIndex = useIndex(idx, &type) + 1;
	//printf("INDEX of default animation: %i %u\n", idx, animNameIndex);
	strcpy(mesh.mDefaultAnimation, "NotImplementedYet");

	/* Weapon bone */
	dRead(&mesh.mWeaponBoneIndex, 1, 4, f); 

	/* Weapon transforms/adjust  */
	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.originXYZ[0] = r;
	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.originXYZ[1] = r;
	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.originXYZ[2] = r;

	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.xAxisXYZ[0] = r;
	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.xAxisXYZ[1] = r;
	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.xAxisXYZ[2] = r;

	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.yAxisXYZ[0] = r;
	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.yAxisXYZ[1] = r;
	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.yAxisXYZ[2] = r;

	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.zAxisXYZ[0] = r;
	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.zAxisXYZ[1] = r;
	dRead(&r, 1, 4, f);
	mesh.mWeaponAdjust.zAxisXYZ[2] = r;

	printf("Read %lu bytes from SkeletalMesh\n", ftell(f) - offset);

	return 0;
}


bool UTPackage::LoadPakVFS(const char *filename)
{
	unsigned int i, index, type;
	char stringHeader[16];
	char stringVersion[16];


	mStream = fopen(filename, "rb");

	if (!mStream)
	{
		perror(filename);
		return false;
	}

	fread(&mHeader.signature, 4, 1, mStream);

	/* UT package signature 0x9E2a83c1 */
	switch (mHeader.signature)
	{
	case 0x9E2a83c1:
		printf("Standard Unreal package format?\n");
		break;

	case 0x0069004c:
		/*  16 bytes Header   ( 16bit char string )
		 *  12 bytes Version  ( 16bit char string )
		 * [ 2 bytes Minor? ] ( Always Ver111 0x53 0x52  "SR"? )
		 * [ 2 bytes longer?] ( Always Ver121 0x50E44D0F ".M.P"? )
		 *
		 *  The rest seems to be XOR encrypted data by 0xAC in Ver111	
		 */

		fread(stringHeader, 12, 1, mStream);
		fread(stringVersion, 12, 1, mStream);
	
		/* Mongoose: 2 Byte wide chars used in string header, 
			assuming english, so strip every other byte */
		stringHeader[5] = stringHeader[6];
		stringHeader[4] = stringHeader[4];
		stringHeader[3] = stringHeader[2];
		stringHeader[2] = stringHeader[0];
		stringHeader[6] = stringHeader[8];
		stringHeader[7] = stringHeader[10];
		stringHeader[1] = ((char *)(&mHeader.signature))[2];
		stringHeader[0] = ((char *)(&mHeader.signature))[0];
		stringHeader[8] = 0;

		for (i = 1; i < 6; ++i)
		{
			stringVersion[i] = stringVersion[i*2];
		}

		stringVersion[6] = 0;

		printf("Lineage II encrypted package '%s' '%s'\n", 
				stringHeader, stringVersion);

		if (strcmp(stringVersion, "Ver121") == 0)
		{
			/* Multiple valid keys, so let's look for common 0 byte */
			// mKeyXOR = 0xCE;
			// mKeyXOR = 0x73;
			unsigned int whence = ftell(mStream);
			fseek(mStream, 0x23, SEEK_SET);
			fread(&mKeyXOR, 1, 1, mStream);
			fseek(mStream, whence, SEEK_SET);
		}
		else
		{
			mKeyXOR = 0xAC;
		}

		mOffset = 28;

		dRead(&mHeader.signature, 4, 1, mStream);
		break;

	default:
		printf("Not a known UT package 0x%x\n", mHeader.signature);
		return false;
	}


	/* From here on transparent decryption is done with dRead() */

	dRead(&mHeader.version, 2, 1, mStream);
	dRead(&mHeader.version2, 2, 1, mStream);
	dRead(&mHeader.flags, 4, 1, mStream);
	dRead(&mHeader.nameCount, 4, 1, mStream);
	dRead(&mHeader.nameOffset, 4, 1, mStream);
	dRead(&mHeader.exportCount, 4, 1, mStream);
	dRead(&mHeader.exportOffset, 4, 1, mStream);
	dRead(&mHeader.importCount, 4, 1, mStream);
	dRead(&mHeader.importOffset, 4, 1, mStream);

	mHeader.nameOffset += mOffset;
	mHeader.exportOffset += mOffset;
	mHeader.importOffset += mOffset;

	printf("header {\n");
	printf("\tsignature = 0x%x\n", mHeader.signature);
	printf("\tversion = %u, version2 (mode) = %u\n", 
			 mHeader.version, mHeader.version2);
	printf("\tflags = 0x%x\n", mHeader.flags);
	printf("\tnameCount = %u, nameOffset = 0x%x\n", 
			 mHeader.nameCount, mHeader.nameOffset);
	printf("\texportCount = %u, exportOffset = 0x%x\n", 
			 mHeader.exportCount, mHeader.exportOffset);
	printf("\timportCount = %u, importOffset = 0x%x\n", 
			 mHeader.importCount, mHeader.importOffset);

	if (mHeader.version < 68)
	{
		mHeader.generationCount = 0;
		mHeader.generations = 0x0;

		dRead(&mHeader.heritageCount, 4, 1, mStream);
		dRead(&mHeader.heritageOffset, 4, 1, mStream);

		printf("\theritageCount = %u, heritageOffset = 0x%x\n", 
				 mHeader.heritageCount, mHeader.heritageOffset);
	}
	else
	{
		mHeader.heritageCount = 0;
		mHeader.heritageOffset = 0;

		dRead(mHeader.guid, 16, 1, mStream);
		dRead(&mHeader.generationCount, 4, 1, mStream);

		printf("\tguid = 0x%x%x%x%x\n",
				 mHeader.guid[0], mHeader.guid[1], mHeader.guid[2], mHeader.guid[3]);

		printf("\tgenerationCount = %u\n", 
				 mHeader.generationCount);

		printf("\n\tGeneration table\n");

		mHeader.generations = new utx_generation_t[mHeader.generationCount];
		
		for (i = 0; i < mHeader.generationCount; ++i)
		{	
			dRead(&mHeader.generations[i].exportCount, 4, 1, mStream);
			dRead(&mHeader.generations[i].nameCount, 4, 1, mStream);
			
			printf("\tgeneration[%u] { exportCount = %u, nameCount = %u }\n", i,
					 mHeader.generations[i].exportCount, 
					 mHeader.generations[i].nameCount);
		}
	}

	/* 2003.06.14, Mongoose - These tables are not sequentally read,
		you have to skip around a lot */

	printf("\n\tName table\n");

	/* Read Name table */
	if ((int)mHeader.nameOffset != ftell(mStream))
	{
		fseek(mStream, mHeader.nameOffset, SEEK_SET);
	}

	mHeader.nameTable = new utx_name_table_t[mHeader.nameCount];

	for (i = 0; i < mHeader.nameCount; ++i)
	{
		mHeader.nameTable[i].objName = getName(mHeader.version, mStream);
		dRead(&mHeader.nameTable[i].objFlags, 4, 1, mStream);

		printf("\tnameTable[%u] { objName = '%s', objFlags = 0x%x }\n", i,
				 mHeader.nameTable[i].objName, mHeader.nameTable[i].objFlags);
	}

	printf("\n\tExport table\n");

	/* Read export table */
	if ((int)mHeader.exportOffset != ftell(mStream))
	{
		fseek(mStream, mHeader.exportOffset, SEEK_SET);
	}

	mHeader.exportTable = new utx_export_table_t[mHeader.exportCount];

	for (i = 0; i < mHeader.exportCount; ++i)
	{
		mHeader.exportTable[i].objClass = getIndex(mStream);
		mHeader.exportTable[i].objSuper = getIndex(mStream);
		dRead(&mHeader.exportTable[i].package, 4, 1, mStream);
		mHeader.exportTable[i].objName = getIndex(mStream);
		dRead(&mHeader.exportTable[i].objFlags, 4, 1, mStream);
		mHeader.exportTable[i].serialSize = getIndex(mStream);
		mHeader.exportTable[i].serialOffset = getIndex(mStream) + mOffset;

		printf("\texportTable[%u] { class %i, super %i, package %u, name %i\n",i,
				 mHeader.exportTable[i].objClass, mHeader.exportTable[i].objSuper,
				 mHeader.exportTable[i].package, mHeader.exportTable[i].objName);
		printf("\t   flags %u, serialSz %i bytes, serialOff %i (0x%x) }\n",
				 mHeader.exportTable[i].objFlags,
				 mHeader.exportTable[i].serialSize, 
				 mHeader.exportTable[i].serialOffset, 
				 mHeader.exportTable[i].serialOffset);
	}

	printf("\n\tImport table\n");

	/* Read import table */
	if ((int)mHeader.importOffset != ftell(mStream))
	{
		fseek(mStream, mHeader.importOffset, SEEK_SET);
	}

	mHeader.importTable = new utx_import_table_t[mHeader.importCount];

	for (i = 0; i < mHeader.importCount; ++i)
	{
		mHeader.importTable[i].objClass = getIndex(mStream);
		mHeader.importTable[i].objSuper = getIndex(mStream);
		dRead(&mHeader.importTable[i].package, 4, 1, mStream);
		mHeader.importTable[i].objName = getIndex(mStream);

		printf("\timportTable[%u] { class %i, super %i, package %u, name %i}\n",
				 i,
				 mHeader.importTable[i].objClass, mHeader.importTable[i].objSuper,
				 mHeader.importTable[i].package, mHeader.importTable[i].objName);
	}

	/* Read Heritage table */
	if (mHeader.heritageCount > 0)
	{
		printf("\n\tHeritage table\n");

		if ((int)mHeader.heritageOffset != ftell(mStream))
		{
			fseek(mStream, mHeader.exportOffset, SEEK_SET);
		}

		mHeader.heritageTable = new unsigned int[mHeader.heritageCount * 4];

		for (i = 0; i < mHeader.heritageCount; ++i)
		{
			dRead(mHeader.heritageTable+(i*16), 16, 1, mStream);

			printf("\theritageTable[%u] = 0x%x%x%x%x\n", i,
					 mHeader.heritageTable[i*16], mHeader.heritageTable[i*16+1],
					 mHeader.heritageTable[i*16+2], mHeader.heritageTable[i*16+3]);
		}
	}

	printf("}\n");

	/* End reading Header and tables */


	/* Testing the export table here to find objects in the file */
	for (i = 0; i < mHeader.exportCount; ++i)
	{
		unsigned int sz, off;
		int nameIndex;


		printf("- %i --------------------------------------\n", i);

		index = useIndex(mHeader.exportTable[i].objClass, &type);
		nameIndex = ((type == UTPackage::UT_NULL) ? -1 :
						 (type == UTPackage::UT_IMPORT) ? 
						 mHeader.importTable[index].objName :
						 mHeader.exportTable[index].objName);

		printf("* Export Class '%s'\n", 
				 ((nameIndex < 0) ? "Null" :
				  mHeader.nameTable[nameIndex].objName));

		index = mHeader.exportTable[i].objName;

		printf("         Name '%s'\n", 
				 ((type == UTPackage::UT_NULL) ? "Null" :
				  mHeader.nameTable[index].objName));

		sz = mHeader.exportTable[i].serialSize;
		printf("         Size %i bytes\n", sz);

		off = mHeader.exportTable[i].serialOffset;
		printf("         Offset 0x%x (%u bytes)\n", off, off);


		/* Seek back to the object */
		fseek(mStream, off, SEEK_SET);

		/* HexDump to stdout, seek back */
		if (mFlags & fHexDump)
		{
			hex_dump_file(mStream, sz);
			fseek(mStream, off, SEEK_SET);
		}

		/* Add paths based on object class names */
		mVFS.AddDir( mHeader.nameTable[nameIndex].objName );

		//char buf[512];
		//snprintf(buf, 511, "%s/%s", 
		//			mHeader.nameTable[nameIndex].objName,
		//			mHeader.nameTable[index].objName);
		
		mVFS.AddFile(mHeader.nameTable[nameIndex].objName, 
						 mHeader.nameTable[index].objName,
						 mHeader.signature, off, sz, mKeyXOR );

		fseek(mStream, off, SEEK_SET);
	}

	fclose(mStream);

	return true;
}


UTPackage::UTVFSDir *UTPackage::GetVFSRoot( )
{
	return mVFS.root;
}


unsigned char *UTPackage::GetVFSObject( const char *dir, const char *file )
{
	unsigned char *buffer;
	UTVFSObj *obj = mVFS.Find( dir, file );

	if ( obj == NULL )
		return NULL;

	FILE *f = fopen( mVFS.pakfile, "rb" );


	if ( !f )
	{
		perror( mVFS.pakfile );
		return NULL;
	}

	buffer = buffer_from_utpak(f, obj->signature, obj->offset, obj->size, obj->key);
	
	if (buffer && obj->key)
	{
		decryptBufferXOR(buffer, obj->size, obj->key);
	}

	return buffer;
}


int UTPackage::load(const char *filename)
{
	unsigned int i, index, type;
	char stringHeader[16];
	char stringVersion[16];


	mStream = fopen(filename, "rb");

	if (!mStream)
	{
		perror(filename);
		return -1;
	}

	fread(&mHeader.signature, 4, 1, mStream);

	/* UT package signature 0x9E2a83c1 */
	switch (mHeader.signature)
	{
	case 0x9E2a83c1:
		printf("Standard Unreal package format?\n");
		break;
	case 0x0069004c:
		/*  16 bytes Header   ( 16bit char string )
		 *  12 bytes Version  ( 16bit char string )
		 * [ 2 bytes Minor? ] ( Always Ver111 0x53 0x52  "SR"? )
		 * [ 2 bytes longer?] ( Always Ver121 0x50E44D0F ".M.P"? )
		 *
		 *  The rest seems to be XOR encrypted data by 0xAC in Ver111	
		 */

		fread(stringHeader, 12, 1, mStream);
		fread(stringVersion, 12, 1, mStream);
	
		/* Mongoose: 2 Byte wide chars used in string header, 
			assuming english, so strip every other byte */
		stringHeader[5] = stringHeader[6];
		stringHeader[4] = stringHeader[4];
		stringHeader[3] = stringHeader[2];
		stringHeader[2] = stringHeader[0];
		stringHeader[6] = stringHeader[8];
		stringHeader[7] = stringHeader[10];
		stringHeader[1] = ((char *)(&mHeader.signature))[2];
		stringHeader[0] = ((char *)(&mHeader.signature))[0];
		stringHeader[8] = 0;

		for (i = 1; i < 6; ++i)
		{
			stringVersion[i] = stringVersion[i*2];
		}

		stringVersion[6] = 0;

		printf("Lineage II encrypted package '%s' '%s'\n", 
				stringHeader, stringVersion);

		if (strcmp(stringVersion, "Ver121") == 0)
		{
			/* Multiple valid keys, so let's look for common 0 byte */
			// mKeyXOR = 0xCE;
			// mKeyXOR = 0x73;
			unsigned int whence = ftell(mStream);
			fseek(mStream, 0x23, SEEK_SET);
			fread(&mKeyXOR, 1, 1, mStream);
			fseek(mStream, whence, SEEK_SET);
		}
		else
		{
			mKeyXOR = 0xAC;
		}

		if (mFlags & fDecryptOnly)
		{
			printf("Writing decrypted file to /tmp/utpak.decrypted\n");

			if (mDecyrptFilename && mDecyrptFilename[0])
			{
				decryptDumpXOR(mDecyrptFilename, mKeyXOR, mStream);
			}
			else
			{
				decryptDumpXOR("/tmp/utpak.decrypted", mKeyXOR, mStream);
			}

			fclose(mStream);
			return 0;
		}

		mOffset = 28;

		dRead(&mHeader.signature, 4, 1, mStream);
		break;
	default:
		printf("Not a known UT package 0x%x\n", mHeader.signature);
		return -2;
	}


	/* From here on transparent decryption is done with dRead() */

	dRead(&mHeader.version, 2, 1, mStream);
	dRead(&mHeader.version2, 2, 1, mStream);
	dRead(&mHeader.flags, 4, 1, mStream);
	dRead(&mHeader.nameCount, 4, 1, mStream);
	dRead(&mHeader.nameOffset, 4, 1, mStream);
	dRead(&mHeader.exportCount, 4, 1, mStream);
	dRead(&mHeader.exportOffset, 4, 1, mStream);
	dRead(&mHeader.importCount, 4, 1, mStream);
	dRead(&mHeader.importOffset, 4, 1, mStream);

	mHeader.nameOffset += mOffset;
	mHeader.exportOffset += mOffset;
	mHeader.importOffset += mOffset;

	printf("header {\n");
	printf("\tsignature = 0x%x\n", mHeader.signature);
	printf("\tversion = %u, version2 (mode) = %u\n", 
			 mHeader.version, mHeader.version2);
	printf("\tflags = 0x%x\n", mHeader.flags);
	printf("\tnameCount = %u, nameOffset = 0x%x\n", 
			 mHeader.nameCount, mHeader.nameOffset);
	printf("\texportCount = %u, exportOffset = 0x%x\n", 
			 mHeader.exportCount, mHeader.exportOffset);
	printf("\timportCount = %u, importOffset = 0x%x\n", 
			 mHeader.importCount, mHeader.importOffset);

	if (mHeader.version < 68)
	{
		mHeader.generationCount = 0;
		mHeader.generations = 0x0;

		dRead(&mHeader.heritageCount, 4, 1, mStream);
		dRead(&mHeader.heritageOffset, 4, 1, mStream);

		printf("\theritageCount = %u, heritageOffset = 0x%x\n", 
				 mHeader.heritageCount, mHeader.heritageOffset);
	}
	else
	{
		mHeader.heritageCount = 0;
		mHeader.heritageOffset = 0;

		dRead(mHeader.guid, 16, 1, mStream);
		dRead(&mHeader.generationCount, 4, 1, mStream);

		printf("\tguid = 0x%x%x%x%x\n",
				 mHeader.guid[0], mHeader.guid[1], mHeader.guid[2], mHeader.guid[3]);

		printf("\tgenerationCount = %u\n", 
				 mHeader.generationCount);

		printf("\n\tGeneration table\n");

		mHeader.generations = new utx_generation_t[mHeader.generationCount];
		
		for (i = 0; i < mHeader.generationCount; ++i)
		{	
			dRead(&mHeader.generations[i].exportCount, 4, 1, mStream);
			dRead(&mHeader.generations[i].nameCount, 4, 1, mStream);
			
			printf("\tgeneration[%u] { exportCount = %u, nameCount = %u }\n", i,
					 mHeader.generations[i].exportCount, 
					 mHeader.generations[i].nameCount);
		}
	}

	/* 2003.06.14, Mongoose - These tables are not sequentally read,
		you have to skip around a lot */

	printf("\n\tName table\n");

	/* Read Name table */
	if ((int)mHeader.nameOffset != ftell(mStream))
	{
		fseek(mStream, mHeader.nameOffset, SEEK_SET);
	}

	mHeader.nameTable = new utx_name_table_t[mHeader.nameCount];

	for (i = 0; i < mHeader.nameCount; ++i)
	{
		mHeader.nameTable[i].objName = getName(mHeader.version, mStream);
		dRead(&mHeader.nameTable[i].objFlags, 4, 1, mStream);

		printf("\tnameTable[%u] { objName = '%s', objFlags = 0x%x }\n", i,
				 mHeader.nameTable[i].objName, mHeader.nameTable[i].objFlags);
	}

	printf("\n\tExport table\n");

	/* Read export table */
	if ((int)mHeader.exportOffset != ftell(mStream))
	{
		fseek(mStream, mHeader.exportOffset, SEEK_SET);
	}

	mHeader.exportTable = new utx_export_table_t[mHeader.exportCount];

	for (i = 0; i < mHeader.exportCount; ++i)
	{
		mHeader.exportTable[i].objClass = getIndex(mStream);
		mHeader.exportTable[i].objSuper = getIndex(mStream);
		dRead(&mHeader.exportTable[i].package, 4, 1, mStream);
		mHeader.exportTable[i].objName = getIndex(mStream);
		dRead(&mHeader.exportTable[i].objFlags, 4, 1, mStream);
		mHeader.exportTable[i].serialSize = getIndex(mStream);
		mHeader.exportTable[i].serialOffset = getIndex(mStream) + mOffset;

		printf("\texportTable[%u] { class %i, super %i, package %u, name %i\n",i,
				 mHeader.exportTable[i].objClass, mHeader.exportTable[i].objSuper,
				 mHeader.exportTable[i].package, mHeader.exportTable[i].objName);
		printf("\t   flags %u, serialSz %i bytes, serialOff %i (0x%x) }\n",
				 mHeader.exportTable[i].objFlags,
				 mHeader.exportTable[i].serialSize, 
				 mHeader.exportTable[i].serialOffset, 
				 mHeader.exportTable[i].serialOffset);
	}

	printf("\n\tImport table\n");

	/* Read import table */
	if ((int)mHeader.importOffset != ftell(mStream))
	{
		fseek(mStream, mHeader.importOffset, SEEK_SET);
	}

	mHeader.importTable = new utx_import_table_t[mHeader.importCount];

	for (i = 0; i < mHeader.importCount; ++i)
	{
		mHeader.importTable[i].objClass = getIndex(mStream);
		mHeader.importTable[i].objSuper = getIndex(mStream);
		dRead(&mHeader.importTable[i].package, 4, 1, mStream);
		mHeader.importTable[i].objName = getIndex(mStream);

		printf("\timportTable[%u] { class %i, super %i, package %u, name %i}\n",
				 i,
				 mHeader.importTable[i].objClass, mHeader.importTable[i].objSuper,
				 mHeader.importTable[i].package, mHeader.importTable[i].objName);
	}

	/* Read Heritage table */
	if (mHeader.heritageCount > 0)
	{
		printf("\n\tHeritage table\n");

		if ((int)mHeader.heritageOffset != ftell(mStream))
		{
			fseek(mStream, mHeader.exportOffset, SEEK_SET);
		}

		mHeader.heritageTable = new unsigned int[mHeader.heritageCount * 4];

		for (i = 0; i < mHeader.heritageCount; ++i)
		{
			dRead(mHeader.heritageTable+(i*16), 16, 1, mStream);

			printf("\theritageTable[%u] = 0x%x%x%x%x\n", i,
					 mHeader.heritageTable[i*16], mHeader.heritageTable[i*16+1],
					 mHeader.heritageTable[i*16+2], mHeader.heritageTable[i*16+3]);
		}
	}

	printf("}\n");

	/* End reading Header and tables */


	/* Testing the export table here to find objects in the file */
	for (i = 0; i < mHeader.exportCount; ++i)
	{
		unsigned int sz, off;
		int nameIndex;


		printf("- %i --------------------------------------\n", i);

		index = useIndex(mHeader.exportTable[i].objClass, &type);
		nameIndex = ((type == UTPackage::UT_NULL) ? -1 :
						 (type == UTPackage::UT_IMPORT) ? 
						 mHeader.importTable[index].objName :
						 mHeader.exportTable[index].objName);

		printf("* Export Class '%s'\n", 
				 ((nameIndex < 0) ? "Null" :
				  mHeader.nameTable[nameIndex].objName));

		index = mHeader.exportTable[i].objName;

		printf("         Name '%s'\n", 
				 ((type == UTPackage::UT_NULL) ? "Null" :
				  mHeader.nameTable[index].objName));

		sz = mHeader.exportTable[i].serialSize;
		printf("         Size %i bytes\n", sz);

		off = mHeader.exportTable[i].serialOffset;
		printf("         Offset 0x%x (%u bytes)\n", off, off);


		/* Seek back to the object */
		fseek(mStream, off, SEEK_SET);

		/* HexDump to stdout, seek back */
		if (mFlags & fHexDump)
		{
			hex_dump_file(mStream, sz);
			fseek(mStream, off, SEEK_SET);
		}

		/* RAW dump to disk, seek back */
		if (mFlags & fDiskDump)
		{
#ifdef WIN32
			char *dir = "utpak";
#else
			char *dir = "/tmp/utpak";
#endif
			char buf[512];

			snprintf(buf, 511, "%s/%s",
						dir,
						mHeader.nameTable[nameIndex].objName);
			buf[511] = 0;
#ifdef WIN32
			mkdir(dir);
			mkdir(buf);
#else
			mkdir(dir, S_IRWXU | S_IRWXG);
			mkdir(buf, S_IRWXU | S_IRWXG);
#endif

			snprintf(buf, 511, "%s/%s/%s.raw", 
						dir,
						mHeader.nameTable[nameIndex].objName,
						mHeader.nameTable[index].objName);
			buf[511] = 0;

			printf("Writing %s\n", buf );
			FILE *f2 = fopen(buf, "wb");

			if (f2)
			{
#define UTP_DUMP_WITH_BUFFERS
#ifdef UTP_DUMP_WITH_BUFFERS
#  ifdef UTP_STACK_BUFFERS
				unsigned int k, j;

				for (k = 0, j = 0; k < sz; k += j)
				{
					if (sz - k >= 512)
					{
						j = 512;
					}
					else
					{
						j = sz - k;
					}
					
					dRead(buf, j, 1, mStream);
					fwrite(buf, j, 1, f2);
				}
#   else  // HEAP BUFFERS
				unsigned char *buffer = buffer_from_utpak(mStream, mHeader.signature, off, sz, mKeyXOR);

				if (buffer)
				{
					if (mKeyXOR)
						decryptBufferXOR(buffer, sz, mKeyXOR);
					fwrite(buffer, sz, 1, f2);
					delete [] buffer;
				}
#   endif
#else // Make sure it's correct, before optimizing...
				for (j = 0; j < sz; ++j)
				{
					dRead(buf, 1, 1, mStream);
					fwrite(buf, 1, 1, f2);
				}
#endif

				fclose(f2);
			}

			fseek(mStream, off, SEEK_SET);
		}

#ifdef NON_RAW_EXPORT_ALLOWED
		/* Exported dump to usable external formats */
		if (mFlags & fDiskDump && nameIndex > 0 &&
			 strcmp("Texture", mHeader.nameTable[nameIndex].objName) == 0)
		{
			URTexture test;

			test.load(*this, index);
		}
		else if (mFlags & fDiskDump && nameIndex > 0 &&
					strcmp("SkeletalMesh", mHeader.nameTable[nameIndex].objName) == 0)
		{
			loadSkeletalMesh(mStream);
		}
#endif
	}

	fclose(mStream);

	return 0;
}


void UTPackage::setFlags(unsigned int flags)
{
	mFlags |= flags;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

void UTPackage::hexDump(FILE *f, unsigned char *buffer, unsigned int size)
{
	unsigned int i, j;
	unsigned char u;
	char s[16];
	char none = 183;

	memset(s, 0, 16);

	/* Dump it out all pretty */
	if (f)
	{
		printf("[Hexdumping, Offset 0x%08lx to 0x%08lx, %u bytes]\n", 
				 ftell(f), ftell(f)+size, size);
	}
	else if (buffer)
	{
		printf("[Hexdumping buffer, %u bytes]\n", size);
	}
	else
	{
		printf("Nothing to hexdump!\n");
		return;
	}

	for (i = 0, j = 0; i < size; ++i, ++j)
	{
		if (f) // Dump from file
		{
			dRead(&u, 1, 1, f); 
		}
		else  // Dump from buffer
		{
			u = buffer[i];
		}

		s[j] = u;

		/* Print ASCII */
		if (j == 16)
		{
			printf("     ");

			for (j = 0; j < 16; ++j)
			{
				printf("%s",
							((j == 0) ? "" :
							 (j%8 == 0) ? "  ": 
							 (j%4 == 0) ? " " : ""));

				if (s[j] > 32 && s[j] < 127)
					printf("%c", s[j]);
				else
					printf("%c", none);
			}

			j = 0;
			memset(s, 0, 16);
		}

		/* Print Hex */
		printf("%s%02X", 
				 ((i == 0) ? "" :
				  ((i%16 == 0) ? "\n" : 
					((i%8 == 0) ? "   ": 
					 (i%4 == 0) ? "  " : " "))), 
				 u);
	}

	if (j < 16 && j != 0)
	{
		i = 16 - j;

		for (j = 0; j < i; ++j)
		{
			printf("%s  ", 
					 ((j == 0) ? " " :
					  ((j%16 == 0) ? "\n" : 
						((j%8 == 0) ? "   ": 
						 (j%4 == 0) ? "  " : " "))));
		}

		printf("     ");
		
		for (j = 0; j < 16; ++j)
		{
			printf("%s",
					 ((j == 0) ? "" :
					  (j%8 == 0) ? "  ": 
					  (j%4 == 0) ? " " : ""));

			if (s[j] > 32 && s[j] < 127)
				printf("%c", s[j]);
			else
				printf("%c", none);
		}
	}

	printf("\n");
}


int UTPackage::dumpFile(FILE *f, unsigned int size, char *s, ...)
{
	char buffer[1024];
	FILE *f2;
	va_list args;

	
	if (!f || size <= 0)
		return -1;

	va_start(args, s);
	vsnprintf(buffer, 1023, s, args);
	va_end(args);

	f2 = fopen(buffer, "wb");

	if (!f2)
	{
		perror(buffer);
		return -2;
	}

	while (size > 1024)
	{
		dRead(buffer, 1, 1024, f);
		fwrite(buffer, 1, 1024, f2);

		if (size - 1024 < 0)
			break;

		size -= 1024;
	}

	dRead(buffer, 1, size, f);
	fwrite(buffer, 1, size, f2);

	fclose(f2);
	return 0;
}


char *UTPackage::getName(unsigned int version, FILE *f)
{
	unsigned int i, l;
	unsigned char sl;
	char buffer[256];
	char *s;


	if (version < 64)
	{
		for (i = 0; i < 255; ++i)
		{
			dRead(buffer+i, 1, 1, f);
			buffer[i+1] = 0;

			if (buffer[i] == '\0')
			{
				break;
			}
		}

		l = i;
		s = new char[l];
		strcpy(s, buffer);
	}
	else
	{
		dRead(&sl, 1, 1, f);

		s = new char[sl];
		dRead(s, sl, 1, f);
	}

	return s;
}


int UTPackage::getArrayIndex(FILE *f)
{
	unsigned char u8;
	short u16;
	int index, u32;


	
	dRead(&u8, 1, 1, f);

	if (u8 < 128)
	{
		index = (unsigned int)u8;
	}
	else 
	{
		//fseek(f, -1, SEEK_SET);
		fseek(f, -1, SEEK_CUR);
		dRead(&u16, 2, 1, f);
		
		if (u16 < 16384 )
		{
			index = (unsigned int)u16;
		}
		else
		{
			//fseek(f, -2, SEEK_SET);
			fseek(f, -2, SEEK_CUR);
			dRead(&u32, 4, 1, f);
			index = u32 & 0x3fffffff;
		}
	}
		
	return index;
}


int UTPackage::getIndex(FILE *f)
{
#define DONT_USE_OLD_SCHOOL_INDEX_READER_ALGORITHM
#ifdef DONT_USE_OLD_SCHOOL_INDEX_READER_ALGORITHM
	unsigned int index, data;
	int vindex;
	unsigned short shift = 6;
	unsigned char b;
	bool sign;


	dRead(&b, 1, 1, f);
	sign = (b & INDEX_SIGN_BIT) != 0;
	index = b & INDEX_BYT1_MASK;

	if (b & INDEX_CONT1_BIT)
	{
		do  {
			dRead(&b, 1, 1, f);
			data = b & INDEX_BYTE_MASK;
			data = ((unsigned int)data) << shift;
			index = index | data;
			shift = (unsigned short)(shift + 7);

		} while ((b & INDEX_CONT_BIT) && (shift < 32));
	}

	vindex = index;
	
	return ((sign) ? -vindex : index); // vindex);
#else
	int val;
	char b0, b1, b2, b3, b4;
                                                                               
	val = 0;
   
	dRead(&b0, 1, 1, f);
   
	if (b0 & 0x40)
	{
		dRead(&b1, 1, 1, f);
 
		if (b1 & 0x80)
		{
			dRead(&b2, 1, 1, f);
 
			if (b2 & 0x80)
			{
				dRead(&b3, 1, 1, f);
 
				if (b3 & 0x80)
				{
					dRead(&b4, 1, 1, f);
					val = b4;
				}
				
				val = (val << 7) + (b3 & 0x7f);
			}
			
			val = (val << 7) + (b2 & 0x7f);
		}
		
		val = (val << 7) + (b1 & 0x7f);
	}
	
	val = (val << 6) + (b0 & 0x3f);
   
	if (b0 & 0x80)
		val = -val;

	return val;
#endif
}


unsigned int UTPackage::useIndex(int index, unsigned int *type)
{
	if (index == 0)
	{
		*type = UTPackage::UT_NULL;
		return 0;
	}
	else if (index < 0)
	{
		*type = UTPackage::UT_IMPORT; // import entry
		//		return ((-index)-1);
		return (-index - 1);
	}

	*type = UTPackage::UT_EXPORT; // export entry
	return (index-1);
}


/* FIXME: New property class system
 * [Property
 *  [Field 
 *   INDEX - SuperField Parent Ref
 *   INDEX - Next Object Ref ]
 *  WORD  - Array dim
 *  WORD  - ElementSz
 *  DWORD - PropertyFlags
 *  INDEX - Cat ( Name ref )
 *  WORD  - Replication offset ( Only if PropertyFlags & CPF_Net 0x00000020)
 *
*/
int UTPackage::loadProperty(UTPackageProperty &prop,
									 utx_header_t *header, FILE *f)
{
	bool isArray;
	unsigned char info, u8;
	unsigned short u16;
	unsigned int u32;
	int idx;
	
	
	/* Property */
	prop.name = NULL;
	prop.index = getIndex(f);
	prop.nameIndex = ((prop.type == UTPackage::UT_NULL) ? 0 :
							(prop.type == UTPackage::UT_IMPORT) ?
							header->importTable[prop.index].objName :
							header->exportTable[prop.index].objName);
	
	printf("<%i :: %i :: %i>\n",
			 prop.index,
			 useIndex(prop.index, &u32),
			 prop.nameIndex);

	if (prop.index >= 0)
		prop.nameIndex = prop.index;
	
	if (prop.nameIndex > header->nameCount)
	{
		printf("index = %i nameIndex = %i ERROR Out of bounds\n",
				 prop.index, prop.nameIndex);
		return -2;
	}
   
	printf("\n{ index = %i, '%s'\n", prop.nameIndex,
			 ((prop.nameIndex >= 0) ? header->nameTable[prop.nameIndex].objName :
			  "Bad Index"));

	prop.name = header->nameTable[prop.nameIndex].objName; /* So safe */

	if (prop.nameIndex < 0 ||
		 strcmp("None", header->nameTable[prop.nameIndex].objName) == 0)
	{
		printf(" Stopping at none }\n");
		return -1;
	}

	/* Read info byte */
	dRead(&info, 1, 1, f);
	
	isArray = (info & PROPERTY_ARRAY_MASK) != 0;
	prop.type = (info & PROPERTY_TYPE_MASK);
	
	switch (info & PROPERTY_SIZE_MASK)
	{
	case 0x00:
		prop.size = 1;
		break;
	case 0x10:
		prop.size = 2;
		break;
	case 0x20:
		prop.size = 4;
		break;
	case 0x30:  
		prop.size = 12;
		break;
	case 0x40:  
		prop.size = 16;
		break;
	case 0x50:  
		dRead(&info, 1, 1, f);
		prop.size = info;
		break;
	case 0x60:
		dRead(&u16, 2, 1, f);
		prop.size = u16;
		break;
	case 0x70:
		dRead(&prop.size, 4, 1, f);
		break;
	}
	
	printf(" size %u, type 0x%x, flags 0x%x\n", 
			 prop.size, prop.type, prop.flags);

	idx = 1;

	switch (prop.type)
	{
	case UNP_ByteProperty:
		if (isArray)
		{
			idx = getArrayIndex(f);
		}
		
		prop.data = new unsigned char[1*idx];
		dRead(prop.data, 1, idx, f);
		printf(" %i byte 0x%02x", idx, prop.data[0]);
		break;
	case UNP_IntProperty:
		if (isArray)
		{
			idx = getArrayIndex(f);
		}
		
		prop.data = new unsigned char[4*idx];
		dRead(prop.data, 4, idx, f);
		printf(" %i int  %i", idx, *((int *)prop.data));
		break;
	case UNP_BoolProperty:
		printf(" bool 0x%x\n", isArray);
		prop.data = new unsigned char[1];
		prop.data[0] = isArray;
		break;
	case UNP_FloatProperty:
		if (isArray)
		{
			idx = getArrayIndex(f);
		}

		dRead(&u32, 4, 1, f);
		printf(" float %f", *((float *)&u32));
		break;
	case UNP_ObjectProperty:
	case UNP_NameProperty: //  SetObjectProperty( aUf );  
		if (isArray)
		{
			idx = getArrayIndex(f);
		}

		if (isArray)
		{
			idx = getIndex(f);
		}
		
		hex_dump_buffer((unsigned char *)&idx, 4);     
		break;
	case UNP_StrProperty:
		printf(" String:\n"); 
		hex_dump_file(f, prop.size);     
		break;
	case UNP_StringProperty:
		printf(" ASCII String:\n");   
		hex_dump_file(f, prop.size);     
		break;
	case UNP_StructProperty:
		printf(" Struct:\n"); 

#ifdef USE_PROPERTY_STRUCT
		idx = getIndex(f);

		if (isArray)
		{
			getArrayIndex(f);
		}

		if (idx > (int)header->nameCount || idx < 0)
		{
			printf(" idx = %i\n", idx);
			hex_dump_file(f, prop.size);
		}
		else if (strcmp("Color", header->nameTable[idx].objName) == 0)
		{
			//dRead(&u32, 4, 1, f);
			hex_dump_file(f, 4);  /* RGBA */
		}
		else if (strcmp("Vector", header->nameTable[idx].objName) == 0)
		{
			/* XYZ */
			dRead(&u32, 4, 1, f);
			printf(" float %f", *((float *)&u32));
			dRead(&u32, 4, 1, f);
			printf(" float %f", *((float *)&u32));
			dRead(&u32, 4, 1, f);
			printf(" float %f", *((float *)&u32));
		}
		else if (strcmp("Rotator", header->nameTable[idx].objName) == 0)
		{
			/* Pitch, Roll, Yaw */
			dRead(&u32, 4, 1, f);
			printf(" float %f", *((float *)&u32));
			dRead(&u32, 4, 1, f);
			printf(" float %f", *((float *)&u32));
			dRead(&u32, 4, 1, f);
			printf(" float %f", *((float *)&u32));
		}
		else if (strcmp("Scale", header->nameTable[idx].objName) == 0)
		{
			/* XYZ, sheer rate, sheer axis */
			dRead(&u32, 4, 1, f);
			printf(" float %f", *((float *)&u32));
			dRead(&u32, 4, 1, f);
			printf(" float %f", *((float *)&u32));
			dRead(&u32, 4, 1, f);
			printf(" float %f", *((float *)&u32));
			dRead(&u32, 4, 1, f);
			printf(" %i\n", u32);
			dRead(&u8, 1, 1, f);
			printf(" %i\n", u8);
		}
		else if (strcmp("PointRegion", header->nameTable[idx].objName) == 0)
		{
			getIndex(f);
			dRead(&u32, 4, 1, f);
			dRead(&u8, 1, 1, f);
		}
		else
		{
			printf("UNKOWN Struct '%s'\n", header->nameTable[idx].objName);
			hex_dump_file(f, prop.size);
		}
#else
		hex_dump_file(f, prop.size);
#endif
		break;
	case UNP_ArrayProperty:  //SetArrayProperty( aUf, aUn, aUi, aUe );
		printf(" ** Array Property!\n"); 
		hex_dump_file(f, prop.size);
		break;
	case UNP_ClassProperty:
	case UNP_VectorProperty:
	case UNP_RotatorProperty:
	case UNP_FixedArrayProperty: //    SetUnknownProperty( aUn,  aUf )
		printf(" ** Unknown/Unsupported Property!\n");

		//data = new unsigned char[size];
		//dRead(data, 1, size, f);
		hex_dump_file(f, prop.size);
		break;
	case UNP_MapProperty:
		printf(" ** Map Property!\n");
		hex_dump_file(f, prop.size);
		break;
	}

	printf(" }\n");

	return 0; 
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_UTPACKAGE
int runUTPackageUnitTest(int argc, char *argv[])
{
	UTPackage utpak;


	if (argc > 2)
	{
		for (int i = 0; argv[1][i] != 0; ++i)
		{
			switch (argv[1][i])
			{
			case 'p':
				utpak.load(argv[2]);
				break;
			case 'x':  /* Bindump UTX to disk decrypted */
				utpak.setFlags(UTPackage::fDecryptOnly);
				
				if (argc > 3)
				{
					utpak.mDecyrptFilename = argv[3];
				}

				utpak.load(argv[2]);
				return 0;
				break;
			case 'd':  /* Bindump UTX to disk */
				utpak.setFlags(UTPackage::fDiskDump);
				utpak.load(argv[2]);
				return 0;
				break;
			case 'h':  /* Hexdump UTX to stdout */
				utpak.setFlags(UTPackage::fHexDump);
				utpak.load(argv[2]);
				return 0;
				break;
			}
		}
	}
	else
	{
		printf("UTPackage.test <options> <UT_Packge_Filename>\n");
		printf("UTPackage.test d callisto.ukx\n");
		printf("\th - Hexdump to console.\n");
		printf("\td - Dump all objects to file possible.\n");
		printf("\tp - Just attempt to parse.\n");
		printf("\tx - Just decrypt LineageII packages.\n");
	}

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[UTPackage class test]\n");
	runUTPackageUnitTest(argc, argv);

	return 0;
}
#endif


#ifdef OBSOLETE
int get_utx_property(utx_header_t *header, FILE *f,
							unsigned int *rsize, unsigned int *rtype,
							unsigned char **data)
{
	unsigned char info, isArray;
	unsigned short us;
	unsigned int u, size, type, flags;
	int index, nameIndex;

	/* Property */
	index = getIndex(f);
	nameIndex = ((type == UTPackage::UT_NULL) ? -1 :
					 (type == UTPackage::UT_IMPORT) ? 
					 header->importTable[index].objName :
					 header->exportTable[index].objName);

	if (index < 0) index = nameIndex;

	printf("\n{ index = %i, '%s'\n", index, 
			 ((index >= 0) ? header->nameTable[index].objName : 
			  "Bad Index"));

	if (index < 0 || strcmp("None", header->nameTable[index].objName) == 0)
	{
		return -1;
	}

	/* Read info byte */
	dRead(&info, 1, 1, f);
	
	isArray = (info & PROPERTY_ARRAY_MASK) != 0;
	type = (info & PROPERTY_TYPE_MASK);
	
	switch (info & PROPERTY_SIZE_MASK)
	{
	case 0x00:
		size = 1;
		break;
	case 0x10:
		size = 2;
		break;
	case 0x20:
		size = 4;
		break;
	case 0x30:  
		size = 12;
		break;
	case 0x40:  
		size = 16;
		break;
	case 0x50:  
		dRead(&info, 1, 1, f);
		size = info;
		break;
	case 0x60:
		dRead(&us, 2, 1, f);
		size = us;
		break;
	case 0x70:
		dRead(&size, 4, 1, f);
		break;
	}
	
	printf(" size %u, type 0x%x, flags 0x%x\n", 
			 size, type, flags);
	
	*rsize = size;
	*rtype = type;

	switch (type)
	{
	case UNP_ByteProperty:
		index = 1;

		if (isArray)
		{
			index = getIndex(f);
		}
		
		//*data = new unsigned char[1*index];
		//dRead(*data, 1, index, f);

		dRead(&info, 1, 1, f);		
		printf(" byte 0x%02x", info);
		break;
	case UNP_IntProperty:
		if (isArray)
		{
			index = getIndex(f);
		}
		
		dRead(&u, 4, 1, f);
		printf(" int  %i", *((int *)&u));
		break;
	case UNP_BoolProperty:
		printf(" bool 0x%x\n", isArray);
		break;
	case UNP_FloatProperty:
		if (isArray)
		{
			index = getIndex(f);
		}

		dRead(&u, 4, 1, f);
		printf(" float %f", *((float *)&u));
		break;
	case UNP_ObjectProperty:
	case UNP_NameProperty: //  SetObjectProperty( aUf );  
		if (isArray)
		{
			index = getIndex(f);
		}

		if (isArray)
		{
				index = getIndex(f);
		}
		
		hex_dump_buffer((unsigned char *)&index, 4);     
		break;
	case UNP_StrProperty: //   SetStringProperty( aUf );  
		hex_dump_file(f, size);     
		break;
	case UNP_StringProperty: //SetAsciizProperty( aUf );  
		hex_dump_file(f, size);     
		break;
	case UNP_StructProperty: //SetStructProperty( aUf, aUn ); 
		hex_dump_file(f, size); 
		break;
	case UNP_ArrayProperty:  //SetArrayProperty( aUf, aUn, aUi, aUe ); 
		hex_dump_file(f, size);
		break;
	case UNP_ClassProperty:
	case UNP_VectorProperty:
	case UNP_RotatorProperty:
	case UNP_MapProperty:
	case UNP_FixedArrayProperty: //    SetUnknownProperty( aUn,  aUf )
		hex_dump_file(f, size);
		break;
	}

	printf(" }\n");

	return 0;
}
#endif




#ifdef FREYJA_PLUGINS
#include <freyja/FreyjaPlugin.h>
#include <mstl/Map.h>
#include <hel/Matrix.h>
#include <hel/Vector3d.h>
#include <hel/Quaternion.h>


extern "C" {

	int freyja_model__utpackage_check(char *filename);
	int freyja_model__utpackage_import(char *filename);
	int freyja_model__utpackage_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}


void freyja_init()
{
	freyjaPluginDescription1s("UE2 Animation (*.ukx)");
	freyjaPluginAddExtention1s("*.ukx");
	//freyjaPluginAddExtention1s("*.utx");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH | FREYJA_PLUGIN_SKELETON);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}


int import_model(char *filename)
{
	return freyja_model__utpackage_import( filename );
}


int freyja_model__utpackage_check(char *filename)
{
	FILE *f = fopen(filename, "rb");											   
	unsigned int u;
	char stringHeader[16];
	char stringVersion[16];


	if (!f)
	{
		perror(filename);
		return -1;
	}

	fread(&u, 4, 1, f);

	/* UT package signature 0x9E2a83c1 */
	switch (u)
	{
	case 0x9E2a83c1:
		freyjaPrintError("Standard Unreal package format?\n");
		return -1;
		break;

	case 0x0069004c:
		/*  16 bytes Header   ( 16bit char string )
		 *  12 bytes Version  ( 16bit char string )
		 * [ 2 bytes Minor? ] ( Always Ver111 0x53 0x52  "SR"? )
		 * [ 2 bytes longer?] ( Always Ver121 0x50E44D0F ".M.P"? )
		 *
		 *  The rest seems to be XOR encrypted data by 0xAC in Ver111	
		 */

		fread(stringHeader, 12, 1, f);
		fread(stringVersion, 12, 1, f);
	
		/* Mongoose: 2 Byte wide chars used in string header, 
			assuming english, so strip every other byte */
		stringHeader[5] = stringHeader[6];
		stringHeader[4] = stringHeader[4];
		stringHeader[3] = stringHeader[2];
		stringHeader[2] = stringHeader[0];
		stringHeader[6] = stringHeader[8];
		stringHeader[7] = stringHeader[10];
		stringHeader[1] = ((char *)(&u))[2];
		stringHeader[0] = ((char *)(&u))[0];
		stringHeader[8] = 0;

		for (u = 1; u < 6; ++u)
		{
			stringVersion[u] = stringVersion[u*2];
		}

		stringVersion[6] = 0;

		freyjaPrintMessage("Lineage II encrypted package '%s' '%s'\n", 
								 stringHeader, stringVersion);

		if (strcmp(stringVersion, "Ver121") == 0)
		{
			return 0;
		}
		else
		{
			return 0; // Guess it's ok for now, since it's a L2 pak  =)
		}
		break;

	default:
		freyjaPrintError("Not a known UT package 0x%x\n", u);
	}


	return -1;
}



int freyja_model__utpackage_import(char *filename)
{
	UTPackage ut;

	if ( ut.LoadPakVFS( filename ) )
	{
		UTPackage::UTVFSDir *dir = ut.GetVFSRoot( );
		UTPackage::UTVFSDir *cur = dir;
		UTPackage::UTVFSObj *obj;
		long pakIndex = freyjaPakBegin(filename);
		char buf[256];

		while ( cur )
		{
			obj = cur->obj;
			
			while ( obj )
			{
				freyjaPrintMessage("%s/%s", cur->name, obj->filename);
				snprintf(buf, 255, "%s/%s", cur->name, obj->filename);
				buf[255] = 0;
				freyjaPakAddFullPathFile(pakIndex, buf, obj->offset, obj->size);
				obj = obj->next;
			}

			cur = cur->next;
		}

		freyjaPakEnd(pakIndex);

		return 0;
	}

	return -1;
}


int freyja_model__utpackage_export(char *filename)
{
	freyjaPrintError("freyja_model__utpackage_export> Not implemented, %s:%i\n", 
					 __FILE__, __LINE__);

	return -1;
}
#endif
