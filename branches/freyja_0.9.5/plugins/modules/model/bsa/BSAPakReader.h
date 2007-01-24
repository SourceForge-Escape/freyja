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
 * 2007.01.22:
 * Mongoose - Oblivion support based on UESPWiki spec
 *
 * 2005.01.02:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_BSAPAKREADER_H_
#define GUARD__FREYJA_MONGOOSE_BSAPAKREADER_H_

#include <string.h>


#define BSA_MORROWIND 0x00000100  // little endian order
#define BSA_OBLIVION  0x67

class BSAPakReader
{
 public:
	

	class OblivionBSAHeader
	{
	public:
		typedef enum {
			fDirectoryNames     = (1<<0),
			fFileNames          = (1<<1),
			fDefaultCompression = (1<<2),
			fUnknown3           = (1<<3),
			fUnknown4           = (1<<4),
			fUnknown5           = (1<<5),
			fUnknown6           = (1<<6),
			fUnknown7           = (1<<7),
			fUnknown8           = (1<<8),
			fUnknown9           = (1<<9),
			fUnknown10          = (1<<10)

		} ArchiveFlag;

		typedef enum {
			fNIF = (1<<0),
			fDDS = (1<<1),
			fXML = (1<<2),
			fWAV = (1<<3),
			fMP3 = (1<<4),
			fTXT = (1<<5),
			fSPT = (1<<7),
			fFNT = (1<<8),
			fCTL = (1<<9)

		} FileFlag;

		char mMagic[4];
		unsigned long mVersion;
		unsigned long mOffset;
		unsigned long mArchiveFlags;
		unsigned long mFolderCount;
		unsigned long mFileCount;

		/* Includes C style NULL termination, but not PASCAL style count */
		unsigned long mTotalFolderNameLenght;
		unsigned long mTotalFileNameLenght;
		
		unsigned long mFileFlags;

		FolderRecord *mFolderRecords; // mFolderCount
		FileRecordBlock *mFileRecordBlocks;
		// FileNameBlock // All the C style strings appended to each other with NULL termination between each entry
		// Files // Raw data, if compressed will have compressed file block
	};


	class OblivionBSAFolderRecord
	{
	public:
		
	};

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

	bool CheckMagic(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Load(const char *filename);
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

	char mMagic[4];

	long mVersion;

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
