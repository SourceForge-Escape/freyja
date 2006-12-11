/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaPakReader
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is a facade for generic pak browsing
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_PAKBROWSER - Builds FreyjaPakReader class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.07.28:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_PAKBROWSER_H_
#define GUARD__FREYJA_PAKBROWSER_H_


#include <hel/math.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>
#include "freyja.h"

using namespace mstl;

class FreyjaPakFile
{
 public:

	FreyjaPakFile(const char *filename, uint32 offset, uint32 size) :
		mFilename(NULL),
		mXORKey(0x0),
		mOffset(offset),
		mSize(size)
	{
		mFilename = String::Strdup(filename);
	}

	~FreyjaPakFile()
	{
		if (mFilename != 0x0)
		{
			delete [] mFilename;
		}
	}

	void decryptBufferXOR(unsigned char *buffer, unsigned int size)
	{
		unsigned int i;


		for (i = 0; i < size; ++i)
		{
			buffer[i] ^= mXORKey;
		}
	}

	const char *getName() { return mFilename; }

	uint32 getDataOffset() { return mOffset; }

	uint32 getDataSize() { return mSize; }

	void setXORKey(unsigned char key) { mXORKey = key; }

	byte *getCopyOfData(SystemIO::FileReader &r)
	{
		byte *buffer = new byte[mSize];
		r.SetOffset(mOffset);
		r.ReadBuffer(mSize, buffer);

		if (mXORKey)
		{
			decryptBufferXOR(buffer, mSize);
		}

		return buffer;
	}

 private:

	FreyjaPakFile(const FreyjaPakFile &pak);

	FreyjaPakFile &operator=(const FreyjaPakFile &pak);


	char *mFilename;

	unsigned char mXORKey;

	uint32 mOffset;

	uint32 mSize;
};


class FreyjaPakDirectory
{
 public:

	FreyjaPakDirectory() :
		mDirName(NULL),
		mPakDirs(),
		mPakFiles()
	{
		mDirName = String::Strdup("vfs:/");
	}

	FreyjaPakDirectory(const char *dirname) :
		mDirName(NULL),
		mPakDirs(),
		mPakFiles()
	{
		mDirName = String::Strdup(dirname);
	}

	~FreyjaPakDirectory()
	{
		if (mDirName != 0x0)
		{
			delete [] mDirName;
		}

		mPakDirs.erase();
		mPakFiles.erase();
	}

	FreyjaPakDirectory *getPakDir(uint32 index)
	{
		if (index < mPakDirs.size() && mPakDirs[index] != 0x0)
			return mPakDirs[index];

		return 0x0;
	}

	FreyjaPakFile *getPakFile(uint32 index) 
	{
		if (index < mPakFiles.size() && mPakFiles[index] != 0x0)
			return mPakFiles[index];

		return 0x0;
	}

	const char *getDirName(uint32 index)
	{
		if (index < mPakDirs.size() && mPakDirs[index] != 0x0)
		{
			return mPakDirs[index]->getName();
		}

		return 0x0;
	}

	const char *getFileName(uint32 index) 
	{
		if (index < mPakFiles.size() && mPakFiles[index] != 0x0)
		{
			return mPakFiles[index]->getName();
		}

		return 0x0;
	}

	FreyjaPakDirectory *getPakDir(const char *dirname) 
	{
		uint32 i, l, count = mPakDirs.size();

		if (dirname == 0x0 || dirname[0] == 0)
			return 0x0;

		l = strlen(dirname);

		for (i = 0; i < count; ++i)
		{
			if (strncmp(dirname, mPakDirs[i]->getName(), l) == 0)
				return mPakDirs[i];
		}

		return 0x0;
	}

	FreyjaPakFile *getPakFile(const char *filename) 
	{
		uint32 i, l, count = mPakFiles.size();

		if (filename == 0x0 || filename[0] == 0)
			return 0x0;

		l = strlen(filename);

		for (i = 0; i < count; ++i)
		{
			if (strncmp(filename, mPakFiles[i]->getName(), l) == 0)
				return mPakFiles[i];
		}

		return 0x0;
	}

	uint32 getDirCount() { return mPakDirs.size(); }

	uint32 getFileCount() { return mPakFiles.size(); }

	const char *getName() { return mDirName; }

	void addDir(FreyjaPakDirectory *dir)
	{
		mPakDirs.pushBack(dir);
	}

	index_t addFile(FreyjaPakFile *file)
	{
		mPakFiles.pushBack(file);
		return (mPakFiles.size() - 1);
	}

 protected:

	FreyjaPakDirectory(const FreyjaPakDirectory &pak);

	FreyjaPakDirectory &operator=(const FreyjaPakDirectory &pak);


	char *mDirName;

	Vector <FreyjaPakDirectory *> mPakDirs;

	Vector <FreyjaPakFile *> mPakFiles;
};


class FreyjaPakReader
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaPakReader(const char *pakfile);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaPakReader
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.07.28: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaPakReader();
	/*------------------------------------------------------
	 * Pre  : FreyjaPakReader object is allocated
	 * Post : Deconstructs an object of FreyjaPakReader
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.07.28: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	byte *getFileByFullPathName(const char *vfsFilename);

	byte *getFileInDirectory(const char *vfsDir, const char *vfsFilename);

	byte *getFileInCurrentDirectory(const char *vfsFilename);

	FreyjaPakDirectory *getRoot() { return &mRoot; }

	const char *getPakFile() { return mPakFile; } 


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	index_t addFullPathFileDesc(const char *vfsFilename, uint32 offset, uint32 size);

	index_t addFullPathFileDesc(const char *vfsFilename, uint32 offset, uint32 size, unsigned char key);

	uint32 mUID;

	void changeDir(const char *dirname);

 private:

	FreyjaPakReader(const FreyjaPakReader&);

	FreyjaPakReader &operator =(const FreyjaPakReader&);


	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	char *mPakFile;                  /* Filename of pak file on disk */

	char *mCurrentDir;               /* String of current dir */ 

	FreyjaPakDirectory mRoot;        /* Virtual root directory */
};


extern "C"
{
	FreyjaPakReader *freyjaGetPakReader(index_t uid);
}

#endif
