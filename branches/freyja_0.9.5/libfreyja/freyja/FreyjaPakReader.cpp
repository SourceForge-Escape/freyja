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
 *-- History ------------------------------------------------- 
 *
 * 2004.07.28:
 * Mongoose - Created
 ==========================================================================*/

#include "FreyjaPakReader.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaPakReader::FreyjaPakReader(const char *pakfile) :
	mUID(0),
	mPakFile(NULL),
	mCurrentDir(NULL),
	mRoot()
{
	mPakFile = String::strdup(pakfile);
}


FreyjaPakReader::~FreyjaPakReader()
{
	if (mPakFile)
		delete [] mPakFile;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

byte *FreyjaPakReader::getFileByFullPathName(const char *vfsFilename)
{
	FreyjaFileReader r;
	FreyjaPakDirectory *pakDir = &mRoot; // cd /
	FreyjaPakFile *pakFile;
	const uint32 blimit = 127;
	char buffer[blimit+1];
	uint32 i, b, length;
	char c;


	if (vfsFilename == 0x0 || vfsFilename[0] == 0)
	{
		return 0x0;
	}

	length = strlen(vfsFilename);

#ifdef DEBUGPAK
	freyjaPrintMessage(vfsFilename);
	freyjaPrintMessage("cd /"); 
#endif

	for (i = 0, b = 0; i < length; ++i)
	{
		c = vfsFilename[i];

		switch (c)
		{
		case '\\':
		case '/':
			if (i > 0)
			{
				pakDir = pakDir->getPakDir(buffer); // cd 'buffer'/
#ifdef DEBUGPAK
				freyjaPrintMessage("cd %s/", buffer);
#endif
			}

			b = 0;
			buffer[0] = 0;
			break;

		default:
			if (b > blimit)
			{
			}
			else
			{
				buffer[b++] = c;
				buffer[b] = 0;
			}

			if (vfsFilename[i+1] == 0)
			{
				pakFile = pakDir->getPakFile(buffer);
#ifdef DEBUGPAK
				freyjaPrintMessage("x %s", buffer);
#endif
			}
		}
	}

	if (pakFile != 0x0 && r.openFile(mPakFile))
	{
		return pakFile->getCopyOfData(r);
	}

	return 0x0;
}


byte *FreyjaPakReader::getFileInDirectory(const char *vfsDir, const char *vfsFilename)
{
	freyjaPrintMessage("Not implemented %s:%i", __FILE__, __LINE__);
	return 0x0;
}


byte *FreyjaPakReader::getFileInCurrentDirectory(const char *vfsFilename)
{
	freyjaPrintMessage("Not implemented %s:%i", __FILE__, __LINE__);
	return 0x0;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

index_t FreyjaPakReader::addFullPathFileDesc(const char *vfsFilename, uint32 offset, uint32 size)
{
	return addFullPathFileDesc(vfsFilename, offset, size, 0x0);
}


index_t FreyjaPakReader::addFullPathFileDesc(const char *vfsFilename, uint32 offset, uint32 size, unsigned char key)
{
	FreyjaPakDirectory *pakDir = &mRoot, *tmpDir;
	FreyjaPakFile *pakFile;
	index_t fi = INDEX_INVALID;
	const uint32 blimit = 127;
	char buffer[blimit+1];
	uint32 i, b, length;
	char c;


	if (vfsFilename == 0x0 || vfsFilename[0] == 0)
	{
		return INDEX_INVALID;
	}

	length = strlen(vfsFilename);

#ifdef DEBUGPAK
	freyjaPrintMessage("\n%s", vfsFilename);
#endif

	for (i = 0, b = 0; i < length; ++i)
	{
		c = vfsFilename[i];

		switch (c)
		{
		case '\\':
		case '/':
			if (i > 0)
			{
#ifdef DEBUGPAK
				freyjaPrintMessage("+ %s/%s/", pakDir->getName(), buffer);
#endif

				tmpDir = pakDir->getPakDir(buffer);

				if (tmpDir == 0x0)
				{
					tmpDir = new FreyjaPakDirectory(buffer);
					/*fi =*/ pakDir->addDir(tmpDir);
				}

				pakDir = tmpDir;
			}

			b = 0;
			buffer[0] = 0;
			break;

		default:
			if (b > blimit)
			{
			}
			else
			{
				buffer[b++] = c;
				buffer[b] = 0;
			}

			if (vfsFilename[i+1] == 0)
			{
				pakFile = new FreyjaPakFile(buffer, offset, size);
				pakFile->setXORKey(key);

				fi = pakDir->addFile(pakFile);
#ifdef DEBUGPAK
				freyjaPrintMessage("+ %s/%s", pakDir->getName(), buffer);
#endif
			}
		}
	}

	return fi;
}


void FreyjaPakReader::changeDir(const char *dirname)
{
	freyjaPrintMessage("Not implemented %s:%i", __FILE__, __LINE__);
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

#ifdef UNIT_TEST_PAKBROWSER
int runFreyjaPakReaderUnitTest(int argc, char *argv[])
{
	FreyjaPakReader test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaPakReader class test]\n");

	return runFreyjaPakReaderUnitTest(argc, argv);
}
#endif
