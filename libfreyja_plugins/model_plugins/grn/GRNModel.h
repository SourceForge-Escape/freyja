/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
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
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_GRNMODEL - Builds GRNModel class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.02.28:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__GRN_MODEL_MONGOOSE_GRNMODEL_H_
#define GUARD__GRN_MODEL_MONGOOSE_GRNMODEL_H_


typedef struct { /* 64 bytes */
	unsigned char unknown[64];

} grn_header_t;

typedef struct { /* 16 bytes */
	unsigned int unknown;
	unsigned int pointer;
	unsigned char unknown2[8];

} grn_itemlist_chunk_t;

typedef struct { /* 12 bytes */
	unsigned int chunkID;
	unsigned int dataPointer;
	unsigned int numChildren;

} grn_itemlist_child_t;

typedef struct { /* 16 bytes */
	unsigned int numChildren;
	unsigned char unknown[12];

	grn_itemlist_child_t *children;
} grn_itemlist_t;


class GRNModel
{
 public:

	typedef enum
	{
		cMain          = 0x0000,
		cCopyright     = 0x0102,
		cObjItemList   = 0x0103,
		cFinalItemList = 0x0101,
		cDemoItemList  = 0x0100,

		/* ItemList chunks */
		cTextChunk        = 0x0200,

		cTextureInfo      = 0x0301,
		cTextureGeometry  = 0x0303,
		cTextureInfoChunk = 0x0304,
		cTextureGeometrys = 0x0305,

		cBoneObject       = 0x0505, /* Skeleton */
		cBoneData         = 0x0506,
		cBone             = 0x0507,
		cBoneList         = 0x0508,

		cMeshObject       = 0x0601,
		cMesh             = 0x0602,
		cPointContainer   = 0x0603,
		cPoints           = 0x0604,

		cUnknownData      = 0x0702,

		cPointData        = 0x0801,
		cNormalData       = 0x0802,
		cTexMapData       = 0x0803,
		cTexMapContainer  = 0x0804,

		cPolygonData      = 0x0901,

		cUnknownChunk     = 0x0B00,
		cUnknownList      = 0x0B01,

		cUnknownList2     = 0x0C00,
		cUnknownChunk2    = 0x0C01,
		cUnknown          = 0x0C03,
		cUnknown2         = 0x0C04,
		cUnknown3         = 0x0C05,
		cUnknownContianer = 0x0C06,
		cUnknownBlock     = 0x0C07,
		cUnknownContianer2= 0x0C08,
		cUnknownList3     = 0x0C09,
		cUnknown4         = 0x0C0A,

		cUnknownList4     = 0x0D00,
		cUnknownChunk3    = 0x0D01,
		cUnknown5         = 0x0D03,

		cTexture          = 0x0E00,
		cTextureChunk     = 0x0E01,
		cTexturePoly      = 0x0E02,
		cTexturePolyDatas = 0x0E03,
		cTexturePolyData  = 0x0E04,
		cTexturePolyData2 = 0x0E05,
		cTexturePolys     = 0x0E06,
		cTexturePolyList  = 0x0E07,

		cObject           = 0x0F00,
		cObjectKey        = 0x0F01,
		cObjectValue      = 0x0F02,
		cObjectList       = 0x0F03,
		cObjectID         = 0x0F04,
		cObjectKeys       = 0x0F05,
		cObjectValueContianer = 0x0F06,

		cCopyrightOrder   = 0x1000,
		cCopyrightDataContainer = 0x1001,
		cCopyrightData    = 0x1002,
		cCopyrightBlock   = 0x1003,

		cAnimation        = 0x1200,
		cAnimationTag     = 0x1201,
		cAnimationList    = 0x1203,
		cAnimationData    = 0x1204,
		cAnimationChunk   = 0x1205,

		cUnknown6         = 0xC202,

		cEndofList        = 0xFFFF

	} GRNChunkType;

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	GRNModel();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of GRNModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.28: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~GRNModel();
	/*------------------------------------------------------
	 * Pre  : GRNModel object is allocated
	 * Post : Deconstructs an object of GRNModel
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.28: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void printChunkName(unsigned int chunkType);

	int load(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Filename is valid string and file is valid
	 * Post : Returns 0 on success
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.28:
	 * Mongoose - Created
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	int readChunk(FILE *f);
	/*------------------------------------------------------
	 * Pre  : File <F> is valid
	 * Post : Returns 0 on success
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.28:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int readSubChunk(FILE *f, unsigned int offset);
	/*------------------------------------------------------
	 * Pre  : File <F> is valid
	 * Post : Returns number of subchunks read, -1 on error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.02.28:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	grn_header_t mHeader;  /* This is the model's diskfile header */
};

#endif
