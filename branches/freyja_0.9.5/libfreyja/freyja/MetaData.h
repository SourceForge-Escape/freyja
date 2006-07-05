/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : MetaData
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: This is for doing things that can't be done.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_METADATA - Builds MetaData class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.04.29:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_METADATA_H_
#define GUARD__FREYJA_MONGOOSE_METADATA_H_

#include <hel/math.h>
#include "freyja.h"
#include "FreyjaFileWriter.h"
#include "FreyjaFileReader.h"

namespace freyja {

class MetaData
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	MetaData() :
		mId(INDEX_INVALID),
		mTypeId(0),
		mData(NULL),
		mDataSize(0)
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of MetaData
	 *
	 *-- History ------------------------------------------
	 *
	 * 2006.04.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~MetaData()
	{
		if (mData)
			delete mData;
	}
	/*------------------------------------------------------
	 * Pre  : MetaData object is allocated
	 * Post : Deconstructs an object of MetaData
	 *
	 *-- History ------------------------------------------
	 *
	 * 2006.04.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	uint32 GetDataSize() { return mDataSize; }


	uint32 GetTypeId() { return mTypeId; }


	byte *GetData() { return mData; }


	bool Serialize(FreyjaFileWriter &w)
	{
		freyja_file_chunk_t chunk;

		chunk.type = FREYJA_CHUNK_METADATA;
		chunk.size = 0;
		chunk.flags = 0x0;
		chunk.version = 1;

		/* Compute byte size here */
		chunk.size += 4;         // index_t mId;
		chunk.size += 4;         // uint32 mTypeId;
		chunk.size += mDataSize; // byte *mData;
		chunk.size += 4;         // uint32 mDataSize;

		/* Write chunk header to diskfile */
		w.writeLong(FREYJA_CHUNK_MESH);
		w.writeLong(chunk.size);
		w.writeLong(chunk.flags);
		w.writeLong(chunk.version);

		/* Write chunk data to diskfile */
		w.writeLong(mId);
		w.writeLong(mTypeId);
		w.writeLong(mDataSize);
		w.writeBuffer(mDataSize, mData);

		return true;
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void SetTypeId(uint32 type) { mTypeId = type; }

	void SetData(byte *data, uint32 size)
	{ 
		if ( mData )
			delete [] mData;

		mData = data;
		mDataSize = size;
	}

	bool Serialize(FreyjaFileReader &r)
	{
		freyja_file_chunk_t chunk;

		/* Read chunk header from diskfile */
		chunk.type = r.readLong();
		chunk.size = r.readLong();
		chunk.flags = r.readLong();
		chunk.version = r.readLong();

		/* Read chunk data from diskfile */
		mId = r.readLong();
		mTypeId = r.readLong();
		mDataSize = r.readLong();

		if ( mDataSize > 0 )
		{
			mData = new byte[mDataSize];
			r.readBuffer(mDataSize, mData);
		}

		if ( chunk.type != FREYJA_CHUNK_METADATA )
			return false;

		return true;
	}


 private:

	MetaData(const MetaData &md);

	MetaData &operator =(const MetaData &md);


	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	index_t mId;                /* Unique MetaData object identifier */

	uint32 mTypeId;             /* This should help you identify your data
								 * when it's read back from disk */

	byte *mData;                /* User defined data structure buffer */

	uint32 mDataSize;           /* User defined data structure buffer size */

	/* These were obsoleted to reduce overhead
	 * For example you wouldn't want all this per vertex...
	 */
	//char mSymbol[64];           /* User datatype symbolic string */
	//char mDescription[64];      /* User datatype description string */
	//index_t mMeshIndex;         /* Mesh which is linked to this MetaData */
	//index_t mBoneIndex;         /* Bone which is linked to this MetaData */
};

}

#endif
