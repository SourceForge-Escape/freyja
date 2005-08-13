/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : EggFileWriter
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class provides disk file write access
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.08.21:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <stdarg.h>

#include "EggFileWriter.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

EggFileWriter::EggFileWriter()
{
	mFileHandle = 0x0;
	mTempBufferHackSz = 2047;
	mTempBufferHack = new char[mTempBufferHackSz+1];
}


EggFileWriter::~EggFileWriter()
{
	closeFile();

	if (mTempBufferHack)
		delete [] mTempBufferHack;

}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool EggFileWriter::doesFileExist(const char *filename)
{
	if (!filename || !filename[0])
		return false;

	FILE *f = fopen(filename, "rb");

	if (f)
	{
		fclose(f);
		return true;
	}

	return false;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void EggFileWriter::closeFile()
{
	if (mFileHandle)
	{
		fclose(mFileHandle);
		mFileHandle = 0x0;
	}
}


bool EggFileWriter::openFile(const char *filename)
{
	if (!filename || !filename[0])
		return false;

	mFileHandle = fopen(filename, "wb");
	
	if (!mFileHandle)
	{
		perror(filename);
		return false;
	}

	return true;
}


void EggFileWriter::print(const char *s, ...)
{
	va_list args;

	va_start(args, s);
	vfprintf(mFileHandle, s, args);
	va_end(args);
}


void EggFileWriter::printFloat(double d)
{
	fprintf(mFileHandle, "%f", d);
}


void EggFileWriter::printInteger(int i)
{
	fprintf(mFileHandle, "%i", i);
}


void EggFileWriter::printString(const char *s)
{
	fprintf(mFileHandle, "%s", s);
}


void EggFileWriter::writeBufferUnsignedChar(unsigned int length, 
											unsigned char *buffer)
{
	fwrite(buffer, length, 1, mFileHandle);  // heh, yeah
}


void EggFileWriter::writeCharString(unsigned int length, char *buffer)
{
	fwrite(buffer, length, 1, mFileHandle);  // heh, yeah
}


float EggFileWriter::writeFloat32()
{	
	float r;
	void *ptr = &r;
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("EggFileReader: ERROR failed to read 32bit float\n");

#if HAVE_BIG_ENDIAN
	FIX_FLOAT(*ptr)
#endif

	return r;
}


char EggFileWriter::writeInt8()
{
	char c;

	fwrite(&c, 1, 1, mFileHandle);  // heh, yeah

	return c;
}


unsigned char EggFileWriter::writeInt8U()
{
	unsigned char u;

	fwrite(&u, 1, 1, mFileHandle);  // heh, yeah

	return u;
}


short EggFileWriter::writeInt16()
{
	short int i;
	void *ptr = &i;
	size_t sz = fwrite(ptr, 2, 1, mFileHandle);

	if (sz < 1)
		printf("EggFileReader: ERROR failed to read 16bit int\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_SHORT(*ptr)
#endif
	return i;
}

	
unsigned short EggFileWriter::writeInt16U()
{
	unsigned short int u;
	void *ptr = &u;
	size_t sz = fread(ptr, 2, 1, mFileHandle);

	if (sz < 1)
		printf("EggFileReader: ERROR failed to read 16bit uint\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_SHORT(*ptr)
#endif
	return u;
}


int EggFileWriter::writeInt32()
{
	int i;
	void *ptr = &i;
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("EggFileReader: ERROR failed to read 32bit int\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	return i;
}


unsigned int EggFileWriter::writeInt32U()
{
	unsigned int u;
	void *ptr = &u;
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("EggFileReader: ERROR failed to read 32bit uint\n");

#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	return u;
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

#ifdef UNIT_TEST_EGGFILEWRITER
int runEggFileWriterUnitTest(int argc, char *argv[])
{
	EggFileWriter test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[EggFileWriter class test]\n");

	return runEggFileWriterUnitTest(argc, argv);
}
#endif
