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
 *-- History ------------------------------------------------- 
 *
 * 2005.01.02:
 * Mongoose - Created
 ==========================================================================*/

#include <freyja/FreyjaFileWriter.h>
#include <freyja/FreyjaFileReader.h>

#include "BSAPakReader.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

BSAPakReader::BSAPakReader()
{
	mTable = 0x0;
}


BSAPakReader::~BSAPakReader()
{
	if (mTable)
		delete [] mTable;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

bool BSAPakReader::load(const char *filename)
{
	FreyjaFileReader r;
	char buffer[128];
	long i, j, base;


	if (!r.openFile(filename))
	{
		return false;
	}

	r.littleEndianMode();

	mHeader.mVersion = r.readLong();

	if (mHeader.mVersion != BSA_VERSION)
		return false;

	mHeader.mTableOffset = r.readLong();
	mHeader.mFileCount = r.readLong();

	mTable = new BSATable[mHeader.mFileCount];

	for (i = 0; i < mHeader.mFileCount; ++i)
	{
		mTable[i].mSize = r.readLong();
		mTable[i].mOffset = r.readLong();
	}

	for (i = 0; i < mHeader.mFileCount; ++i)
	{
		mTable[i].mFilenameOffset = r.readLong();
	}

	base = r.getFileOffset();

	for (i = 0; i < mHeader.mFileCount; ++i)
	{
		r.setFileOffset(base + mTable[i].mFilenameOffset);
		
		for (j = 0; j < 126; ++j)
		{
			buffer[j] = r.readInt8();
			buffer[j+1] = 0;

			if (buffer[j] == 0)
				break;
		}

		mTable[i].setFilename(buffer);
	}

	for (i = 0; i < mHeader.mFileCount; ++i)
	{
		mTable[i].mHash1 = r.readLong();
		mTable[i].mHash2 = r.readLong();
	}

	mDataOffset = r.getFileOffset();

	return true;
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_BSAPAKREADER
#include <mstl/Vector.h>

void bsa_get_command(char *cmd, long size)
{
	char c;
	long i = 0;


	fflush(stdout);

	while (i < size)
	{
		fflush(stdin);
		c = fgetc(stdin);

		if (c == '\n')
		{
			//cmd[i++] = '\r';
			//cmd[i++] = '\n';
			cmd[i] = 0;
			i = size + 1;
		}
		else if (c == 8)
		{
			--i;
		}
		else
		{
			cmd[i++] = c;
			cmd[i] = 0;
		}
	}
}


int runBSAPakReaderUnitTest(int argc, char *argv[])
{
	BSAPakReader bsa;
	FreyjaFileReader r;
	FreyjaFileWriter w;
	Vector<char *> cache;
	char *s;
	char cmd[128];
	char dir[256];
	char filename[256];
	long i, count, len;


	if (argc < 2)
	{
		printf("%s pak_filename\n", argv[0]);
		return -1;
	}

	if (!bsa.load(argv[1]) || !r.openFile(argv[1]))
	{
		printf("Couldn't load pakfile %s \n", argv[1]);
		return -1;
	}

	dir[0] = 0;

	for (;;)
	{
		printf("bsa:\\%s$ ", dir);
		bsa_get_command(cmd, 128);
		printf("\n");

		cache.clear();

		len = 0;

		if (dir[0])
			len = strlen(dir);

		if (!strncmp(cmd, "exit", 4))
		{
			break;
		}
		else if (!strncmp(cmd, "help", 4))
		{
			printf("exit - exit bsa shell\n");
			printf("cd - vfs change directory\n");
			printf("ls - vfs list directory\n");
			printf("x - extract filename ( full bsa path )\n");
		}
		else if (!strncmp(cmd, "cd ", 3))
		{
			s = cmd+3;

			if (s[0] == '\\')
			{
				if (s[1] == 0)
				{
					dir[0] = 0;
				}
				else
				{
					s++;
					strcpy(dir, s);
				}
			}
			else if (s[0] == '.')
			{
				if (s[1] == '.')
				{
					char *last = dir;

					for (s = dir; s[0]; ++s)
					{
						if (s[0] == '\\')
						{
							last = s;
						}
					}

					last[0] = 0;
				}
			}
			else
			{
				sprintf(dir, "%s\\%s", dir, s);

				if (dir[0] == '\\')
				{
					s = dir;
					s++;
					sprintf(dir, "%s", s);
				}
			}

		}
		else if (!strncmp(cmd, "ls", 2))
		{
			for (i = 0, count = 0; i < bsa.mHeader.mFileCount; ++i)
			{
				if (!strncmp(dir, bsa.mTable[i].mFilename, len))
				{
					printf("  %s  \t%li\n",
						   bsa.mTable[i].mFilename,
						   bsa.mTable[i].mSize);
					++count;
				}
			}

			printf("  %li files\n", count); //bsa.mHeader.mFileCount);
		}
		else if (!strncmp(cmd, "x ", 2))
		{
			s = cmd+2;

			if (s[0] == '\\')
				++s;

			for (i = 0; i < bsa.mHeader.mFileCount; ++i)
			{
				if (!strcmp(s, bsa.mTable[i].mFilename))
				{
					unsigned char *data =new unsigned char[bsa.mTable[i].mSize];
					r.setFileOffset(bsa.mDataOffset+bsa.mTable[i].mOffset);
					r.readBuffer(bsa.mTable[i].mSize, data);

					strcpy(filename, bsa.mTable[i].mFilename);
					
					for (s = filename; s[0]; ++s)
					{
						if (s[0] == '\\')
						{
							s[0] = 0;
							FreyjaFileWriter::createDirectory(filename);
							s[0] = '/';
						}
					}

					if (w.openFile(filename))
					{
						w.writeBuffer(bsa.mTable[i].mSize, data);
						w.closeFile();

						printf("  extracted %s\n", filename);
					}

					delete [] data;
					break;
				}
			}
		}
		else
		{
			printf("  %s unknown command\n", cmd);
		}

		cache.clear();
	}

	r.closeFile();

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[BSAPakReader class test]\n");

	return runBSAPakReaderUnitTest(argc, argv);
}
#endif


////////////////////////////////////////////////////////////
// Special Interface code
////////////////////////////////////////////////////////////

#ifdef FREYJA_PLUGINS

#include <string.h>
#include <stdio.h>
#include <freyja/FreyjaPluginABI.h>
#include <hel/Matrix.h>


extern "C" {

	int freyja_model__bsa_check(char *filename);
	int freyja_model__bsa_import(char *filename);
	int freyja_model__bsa_export(char *filename);
	int import_model(char *filename);
	void freyja_init();
}

void freyja_init()
{
	freyjaPluginDescription1s("Morrowind Pak (*.BSA)");
	freyjaPluginAddExtention1s("*.bsa");
	freyjaPluginImport1i(FREYJA_PLUGIN_MESH);
	freyjaPluginExport1i(FREYJA_PLUGIN_NONE);
}

int freyja_model__bsa_check(char *filename)
{
	FreyjaFileReader r;
	long version;


	if (!r.openFile(filename))
	{
		return false;
	}

	r.littleEndianMode();

	version = r.readLong();

	if (version != BSA_VERSION)
		return -1;

	r.closeFile();

	return 0;
}


int import_model(char *filename)
{
	if (!freyja_model__bsa_check(filename))
		return freyja_model__bsa_import(filename);

	return -1;
}


int freyja_model__bsa_import(char *filename)
{
	BSAPakReader bsa;
	char file[1024];
	char *s;
	long i, pakIndex;


	if (bsa.load(filename))
	{
		pakIndex = freyjaPakBegin(filename);

		for (i = 0; i < bsa.mHeader.mFileCount; ++i)
		{
			strncpy(file, bsa.mTable[i].mFilename, 1023);
			file[1023] = 0;

			for (s = file; s[0]; ++s)
			{
				if (s[0] == '\\')
				{
					s[0] = '/';
				}
			}

			freyjaPakAddFullPathFile(pakIndex, file,
									(bsa.mDataOffset + bsa.mTable[i].mOffset),
									bsa.mTable[i].mSize);
		}

		freyjaPakEnd(pakIndex);
		return 0;
	}

	return -1;
}

#endif
