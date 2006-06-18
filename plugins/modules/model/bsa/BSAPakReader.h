/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : BSAPakReader
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: BethSoft BSA pak support
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_BSAPAKREADER - Builds BSAPakReader class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.01.02:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_BSAPAKREADER_H_
#define GUARD__FREYJA_MONGOOSE_BSAPAKREADER_H_

#include <string.h>


#define BSA_VERSION 0x00000100  // little endian order


class BSAPakReader
{
 public:
	
	class BSAHeader
	{
	public:
		long mVersion;
		long mTableOffset;
		long mFileCount;
	};

	
	class BSATable
	{
	public:
		
		BSATable()
		{
			mFilename = 0x0;
		}
	
		~BSATable()
		{
			if (mFilename) delete [] mFilename;
		}
		
		void setFilename(const char *filename)
		{
			long lenght;

			if (mFilename) delete [] mFilename;
			
			if (!filename || !filename[0])
				return;

			lenght = strlen(filename);
			mFilename = new char[lenght+1];
			strncpy(mFilename, filename, lenght);
			mFilename[lenght] = 0;
		}

		long mSize;
		long mOffset;

		long mFilenameOffset;
		char *mFilename;      /* Includes relative path from root */

		long mHash1;
		long mHash2;
	};

	

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	BSAPakReader();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of BSAPakReader
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~BSAPakReader();
	/*------------------------------------------------------
	 * Pre  : BSAPakReader object is allocated
	 * Post : Deconstructs an object of BSAPakReader
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	bool load(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	BSAHeader mHeader;

	BSATable *mTable;

	long mDataOffset;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	/* */
};

#endif
