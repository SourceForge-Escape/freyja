/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : GRN File module
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : GRNModel
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class loads and parses GRN files
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.02.28:
 * Mongoose - Created
 =================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GRNModel.h"


int ENABLE_GUESSING = 0;

void guess_data_type(unsigned char *data, unsigned int size, 
							unsigned int offset)
{
	unsigned int i, size2;

	if (ENABLE_GUESSING == 0)
	{
		// printf("Guessing disabled\n");
		return;
	}

	size2 = size - 4;

	for (i = 0, --size; i < size; ++i)
	{
		printf("Guessing @ 0x%x: F %f, D %u, B %u\n", i + offset, 
				 //*(char*)(data + i),
				 (i > size2) ? 0.0 : *(float*)(data + i),
				 (i > size2) ? 0 : *(unsigned int*)(data + i),
				 *(unsigned char*)(data + i));
	}
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

GRNModel::GRNModel()
{
}


GRNModel::~GRNModel()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void GRNModel::printChunkName(unsigned int chunkType)
{
	switch (chunkType)
	{
	case cMain:
		printf("Main chunk\n");
		break;
	case cObjItemList:
		printf("ObjectItemList chunk\n");
		break;
	case cFinalItemList:
		printf("FinalItemList chunk\n");
		break;
	case cDemoItemList:
		printf("DemoItemList chunk\n");
		break;
	case cCopyright:
		printf("Copyright chunk\n");
		break;

	/* ItemList chunks*/

	case cTextChunk: /* 0x0200 */ 
		printf("cTextChunk  0x0200\n");
		break;
	case cTextureInfo: /* 0x0301 */ 
		printf("cTextureInfo  0x0301\n"); 
		break;
	case cTextureGeometry: /* 0x0303 */ 
		printf("cTextureGeometry  0x0303\n"); 
		break;
	case cTextureInfoChunk: /* 0x0304 */ 
		printf("cTextureInfoChunk  0x0304\n"); 
		break;
	case cTextureGeometrys: /* 0x0305 */ 
		printf("cTextureGeometrys  0x0305\n"); 
		break;
	case cBoneObject: /* 0x0505 */ 
		printf("cBoneObject  0x0505\n"); 
		break; /* Skeleton */
	case cBoneData: /* 0x0506 */ 
		printf("cBoneData  0x0506\n"); 
		break;
	case cBone: /* 0x0507 */ 
		printf("cBone  0x0507\n"); 
		break;
	case cBoneList: /* 0x0508 */ 
		printf("cBoneList  0x0508\n"); 
		break;
	case cMeshObject: /* 0x0601 */ 
		printf("cMeshObject  0x0601\n"); 
		break;
	case cMesh: /* 0x0602 */ 
		printf("cMesh  0x0602\n"); 
		break;
	case cPointContainer: /* 0x0603 */ 
		printf("cPointContainer  0x0603\n"); 
		break;
	case cPoints: /* 0x0604 */ 
		printf("cPoints  0x0604\n"); 
		break;
	case cUnknownData: /* 0x0702 */ 
		printf("cUnknownData  0x0702\n"); 
		break;
	case cPointData: /* 0x0801 */ 
		printf("cPointData  0x0801\n"); 
		break;
	case cNormalData: /* 0x0802 */ 
		printf("cNormalData  0x0802\n"); 
		break;
	case cTexMapData: /* 0x0803 */ 
		printf("cTexMapData  0x0803\n"); 
		break;
	case cTexMapContainer: /* 0x0804 */ 
		printf("cTexMapContainer  0x0804\n"); 
		break;
	case cPolygonData: /* 0x0901 */ 
		printf("cPolygonData  0x0901\n"); 
		break;
	case cUnknownChunk: /* 0x0B00 */ 
		printf("cUnknownChunk  0x0B00\n"); 
		break;
	case cUnknownList: /* 0x0B01 */ 
		printf("cUnknownList  0x0B01\n"); 
		break;
	case cUnknownList2: /* 0x0C00 */ 
		printf("cUnknownList2  0x0C00\n"); 
		break;
	case cUnknownChunk2: /* 0x0C01 */ 
		printf("cUnknownChunk2  0x0C01\n"); 
		break;
	case cUnknown: /* 0x0C03 */ 
		printf("cUnknown  0x0C03\n"); 
		break;
	case cUnknown2: /* 0x0C04 */ 
		printf("cUnknown2  0x0C04\n"); 
		break;
	case cUnknown3: /* 0x0C05 */ 
		printf("cUnknown3  0x0C05\n"); 
		break;
	case cUnknownContianer: /* 0x0C06 */ 
		printf("cUnknownContianer  0x0C06\n"); 
		break;
	case cUnknownBlock: /* 0x0C07 */ 
		printf("cUnknownBlock  0x0C07\n"); 
		break;
	case cUnknownContianer2: /* 0x0C08 */ 
		printf("cUnknownContianer2  0x0C08\n"); 
		break;
	case cUnknownList3: /* 0x0C09 */ 
		printf("cUnknownList3  0x0C09\n"); 
		break;
	case cUnknown4: /* 0x0C0A */ 
		printf("cUnknown4  0x0C0A\n"); 
		break;
	case cUnknownList4: /* 0x0D00 */ 
		printf("cUnknownList4  0x0D00\n"); 
		break;
	case cUnknownChunk3: /* 0x0D01 */ 
		printf("cUnknownChunk3  0x0D01\n"); 
		break;
	case cUnknown5: /* 0x0D03 */ 
		printf("cUnknown5  0x0D03\n"); 
		break;
	case cTexture: /* 0x0E00 */ 
		printf("cTexture  0x0E00\n"); 
		break;
	case cTextureChunk: /* 0x0E01 */ 
		printf("cTextureChunk  0x0E01\n"); 
		break;
	case cTexturePoly: /* 0x0E02 */ 
		printf("cTexturePoly  0x0E02\n"); 
		break;
	case cTexturePolyDatas: /* 0x0E03 */ 
		printf("cTexturePolyDatas  0x0E03\n"); 
		break;
	case cTexturePolyData: /* 0x0E04 */ 
		printf("cTexturePolyData  0x0E04\n"); 
		break;
	case cTexturePolyData2: /* 0x0E05 */ 
		printf("cTexturePolyData2  0x0E05\n"); 
		break;
	case cTexturePolys: /* 0x0E06 */ 
		printf("cTexturePolys  0x0E06\n"); 
		break;
	case cTexturePolyList: /* 0x0E07 */ 
		printf("cTexturePolyList  0x0E07\n"); 
		break;
	case cObject: /* 0x0F00 */ 
		printf("cObject  0x0F00\n"); 
		break;
	case cObjectKey: /* 0x0F01 */ 
		printf("cObjectKey  0x0F01\n"); 
		break;
	case cObjectValue: /* 0x0F02 */ 
		printf("cObjectValue  0x0F02\n"); 
		break;
	case cObjectList: /* 0x0F03 */ 
		printf("cObjectList  0x0F03\n"); 
		break;
	case cObjectID: /* 0x0F04 */ 
		printf("cObjectID  0x0F04\n"); 
		break;
	case cObjectKeys: /* 0x0F05 */ 
		printf("cObjectKeys  0x0F05\n"); 
		break;
	case cObjectValueContianer: /* 0x0F06 */ 
		printf("cObjectValueContianer  0x0F06\n"); 
		break;
	case cCopyrightOrder: /* 0x1000 */ 
		printf("cCopyrightOrder  0x1000\n"); 
		break;
	case cCopyrightDataContainer: /* 0x1001 */ 
		printf("cCopyrightDataContainer  0x1001\n"); 
		break;
	case cCopyrightData: /* 0x1002 */ 
		printf("cCopyrightData  0x1002\n"); 
		break;
	case cCopyrightBlock: /* 0x1003 */ 
		printf("cCopyrightBlock  0x1003\n"); 
		break;
	case cAnimation: /* 0x1200 */ 
		printf("cAnimation  0x1200\n"); 
		break;
	case cAnimationTag: /* 0x1201 */ 
		printf("cAnimationTag  0x1201\n"); 
		break;
	case cAnimationList: /* 0x1203 */ 
		printf("cAnimationList  0x1203\n"); 
		break;
	case cAnimationData: /* 0x1204 */ 
		printf("cAnimationData  0x1204\n"); 
		break;
	case cAnimationChunk: /* 0x1205 */ 
		printf("cAnimationChunk  0x1205\n"); 
		break;
	case cUnknown6: /* 0xC202 */ 
		printf("cUnknown6  0xC202\n"); 
		break;
	case cEndofList: /* 0xFFFF */ 
		printf("cEndofList  0xFFFF\n"); 
		break;
	default:
		printf("Unknown item chunk type\n");
	}
}


/* This method eats a lot of stack space, and implements several
 * methods at once for ease of reverse engineering
 *
 * It's got comments on where you can factor out some of them.
 */
int GRNModel::readChunk(FILE *f)
{
	grn_itemlist_chunk_t itemChunk;
	grn_itemlist_t itemList;
	unsigned int i, count, numChildren, offset;
	unsigned short chunk, chunkType;
	unsigned char unknown[24];


	fread(&chunkType, 2, 1, f);
	fread(&chunk, 2, 1, f);
	printf("---------------------------------------------------\n");
	printf("Read chunk: %04x %04x\n", chunk, chunkType);

	if (chunk != 0xca5e)
	{
		printf("ERROR: Invalid chunk!\n");
		return -1;
	}

	printChunkName(chunkType);

	/* parse_chunk() function switch */
	switch (chunkType)
	{
	case cMain:
		fread(&numChildren, 4, 1, f);
		fread(unknown, 24, 1, f);
		printf("\tnumChildren = %u\n", numChildren);
		printf("\tunknown 24bytes\n");
		guess_data_type(unknown, 24, ftell(f));

		for (i = 0; i < numChildren; ++i)
		{
			if (readChunk(f) < 0)
			{
				printf("Failed to read this child, stopping...\n");
				break;
			}
		}
		break;
	case cObjItemList:
	case cFinalItemList:
	case cDemoItemList:
	case cCopyright:
		fread(&itemChunk, 16, 1, f);

		printf(" unknown %i\n pointer 0x%X\n unknown 8bytes\n",
				 itemChunk.unknown, itemChunk.pointer);
		guess_data_type(itemChunk.unknown2, 8, ftell(f));
		
		offset = ftell(f);

		printf("\n Seeking to 0x%X...\n\n", itemChunk.pointer);
		fseek(f, itemChunk.pointer, SEEK_SET);

		printf("## ItemList @ 0x%lX ##############\n", ftell(f));
		fread(&itemList.numChildren, 4, 1, f);
		fread(&itemList.unknown, 12, 1, f);
		
		printf("\t numChildren = %u\n", itemList.numChildren);
		printf("\t unknown 12btyes\n");
		guess_data_type(unknown, 12, 0);

#define READ_TREE_AS_ARRAY
#ifdef READ_TREE_AS_ARRAY
		itemList.children = new grn_itemlist_child_t[itemList.numChildren];
		fread(itemList.children, 12, itemList.numChildren, f);

		//mItemLists.pushBack(itemList);

		for (i = 0, count = 0; i < itemList.numChildren; ++i)
		{
			printf("\n");
			printf("### ItemListChild %i / %i ###\n",
					 i+1, itemList.numChildren);
			printChunkName(*(unsigned short int *)(&itemList.children[i].chunkID));
			printf("\t\t chunkID = %x\n", itemList.children[i].chunkID);
			printf("\t\t dataPointer = 0x%X -> 0x%X\n",
					 itemList.children[i].dataPointer,
					 itemList.children[i].dataPointer + itemChunk.pointer);
			printf("\t\t numChildren = %i\n", itemList.children[i].numChildren);

			switch (itemList.children[i].chunkID)
			{
			case 0xca5e0801:
				printf("pointOffset = 0x%X\n", itemList.children[i].dataPointer + itemChunk.pointer);
				//jumpBack = ftell(f);
				//fseek(f, offset + pointer, SEEK_SET);
				//
				//fseek(f, jumpBack, SEEK_SET);
				break;
			}
		}
#else
		for (i = 0, count = 0; i < itemList.numChildren; ++i)
		{
			//#define PROMOTE_TO_CHUNK
#   ifdef PROMOTE_TO_CHUNK
			printf("-- Subchunk %i / %i ------\n", 
					 i, itemList.numChildren);

			if ((j = readSubChunk(f, itemChunk.pointer)) < 0)
			{
				printf("Failed to read this child, stopping...\n");
				break;
			}

			count += j;

			if (count >= itemList.numChildren)
				break;
#   else
			grn_itemlist_child_t itemListChild;


			fread(&itemListChild, 12, 1, f);

			printf("\n");
			printf("\t\t #ItemListChild#\n");
			printf("\t\t chunkID = %x\n", itemListChild.chunkID);
			printf("\t\t dataPointer = 0x%X -> 0x%X\n",
					 itemListChild.dataPointer,
					 itemListChild.dataPointer + itemChunk.pointer);
			printf("\t\t numChildren = %i\n", itemListChild.numChildren);
#   endif
		}
#endif

		fseek(f, offset, SEEK_SET);
		break;
   default:
		printf("Unknown or unimplemented chunk type: 0x%x\n", chunkType);
	}

	return 0;
}


int GRNModel::readSubChunk(FILE *f, unsigned int offset)
{
	unsigned int pointer, numChildren, count, i; //, jumpBack;
	unsigned short int chunk, chunkType;


	fread(&chunkType, 2, 1, f);
	fread(&chunk, 2, 1, f);
	printf("_______________________________\n");
	printf("Read subchunk: %04x %04x\n", chunk, chunkType);

	if (chunk != 0xca5e)
	{
		printf("ERROR: Invalid chunk!\n");
		return -1;
	}

	if (chunk == 0xffff)
	{
		printf("Final chunk!\n");
		return 999999999;
	}

	count = 1;

	printChunkName(chunkType);

	fread(&pointer, 4, 1, f);
	fread(&numChildren, 4, 1, f);
	
	printf("\n");
	printf("\t #ItemListChild#\n");
	printf("\t chunkID = %x\n", chunkType);
	printf("\t dataPointer = 0x%X -> 0x%X\n",
			 pointer, offset + pointer);
	printf("\t numChildren = %i\n", numChildren);


	switch (chunkType)
	{
	case cPointData:
		printf("pointOffset = 0x%X\n", offset + pointer);
		//jumpBack = ftell(f);
		//fseek(f, offset + pointer, SEEK_SET);
		//
		//fseek(f, jumpBack, SEEK_SET);
		break;
	}

	for (i = 0; i < numChildren; ++i)
	{
		count += readSubChunk(f, offset);
	}

	return count;
}

int GRNModel::load(const char *filename)
{
	FILE *f;
	unsigned int i;


	f = fopen(filename, "rb");

	if (!f)
	{
		printf("Failed to open file '%s'\n", filename);
		perror("Couldn't open file");
		return -1;
	}

	printf("Reading header...\n");
	fread(mHeader.unknown, 64, 1, f);

	printf("Likely just a MIME type if it doesn't change\n");


	printf("0x%x\n", mHeader.unknown[i]);

	// printf("Guessing header types anyway...\n");
	// guess_data_type(mHeader.unknown, 64, ftell(f));

	printf("Reading chunks...\n");
	readChunk(f);

	printf("Done\n");
	fclose(f);
	return 0;
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_GRNMODEL
int runGRNModelUnitTest(int argc, char *argv[])
{
	GRNModel test;
	unsigned int i, len;


	if (argc < 3)
	{
		printf("%s <options> <filename.grn>\n", argv[0]);
		printf("\t%s l filename.grn\n", argv[0]);
		return 0;
	}


	len = strlen(argv[1]);

	for (i = 0; i < len; ++i)
	{
		switch (argv[1][i])
		{
		case 'g':
			ENABLE_GUESSING = 1;
			break;
		}
	}

	test.load(argv[2]);

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[GRNModel class test]\n");
	runGRNModelUnitTest(argc, argv);

	return 0;
}
#endif
