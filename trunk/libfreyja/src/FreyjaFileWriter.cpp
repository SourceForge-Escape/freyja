/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaFileWriter
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class provides disk file write access
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.12.14: 
 * Mongoose - Fixed generated code for binary file I/O
 *
 * 2004.08.21:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>
#include <stdarg.h>

#include "FreyjaFileWriter.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaFileWriter::FreyjaFileWriter()
{
	mFileHandle = 0x0;
	mTempBufferHackSz = 2047;
	mTempBufferHack = new char[mTempBufferHackSz+1];
}


FreyjaFileWriter::~FreyjaFileWriter()
{
	closeFile();

	if (mTempBufferHack)
		delete [] mTempBufferHack;

}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool FreyjaFileWriter::doesFileExist(const char *filename)
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

void FreyjaFileWriter::closeFile()
{
	if (mFileHandle)
	{
		fclose(mFileHandle);
		mFileHandle = 0x0;
	}
}


bool FreyjaFileWriter::openFile(const char *filename)
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


void FreyjaFileWriter::print(const char *s, ...)
{
	va_list args;

	va_start(args, s);
	vfprintf(mFileHandle, s, args);
	va_end(args);
}


void FreyjaFileWriter::printFloat(double d)
{
	fprintf(mFileHandle, "%f", d);
}


void FreyjaFileWriter::printInteger(int i)
{
	fprintf(mFileHandle, "%i", i);
}


void FreyjaFileWriter::printString(const char *s)
{
	fprintf(mFileHandle, "%s", s);
}


void FreyjaFileWriter::writeBufferUnsignedChar(unsigned int length, 
											unsigned char *buffer)
{
	fwrite(buffer, length, 1, mFileHandle);  // heh, yeah
}


void FreyjaFileWriter::writeCharString(unsigned int length, char *buffer)
{
	fwrite(buffer, length, 1, mFileHandle);  // heh, yeah
}


void FreyjaFileWriter::writeFloat32(float r)
{	
	void *ptr = &r;
#if HAVE_BIG_ENDIAN
	FIX_FLOAT(*ptr)
#endif
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to write 32bit float\n");
}


void FreyjaFileWriter::writeInt8(char c)
{
	fwrite(&c, 1, 1, mFileHandle);
}


void FreyjaFileWriter::writeInt8U(unsigned char u)
{
	fwrite(&u, 1, 1, mFileHandle);  // heh, yeah
}


void FreyjaFileWriter::writeInt16(short i)
{
	void *ptr = &i;
#ifdef HAVE_BIG_ENDIAN
	FIX_SHORT(*ptr)
#endif
	size_t sz = fwrite(ptr, 2, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 16bit int\n");
}

	
void FreyjaFileWriter::writeInt16U(unsigned short u)
{
	void *ptr = &u;
#ifdef HAVE_BIG_ENDIAN
	FIX_SHORT(*ptr)
#endif
	size_t sz = fread(ptr, 2, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 16bit uint\n");
}


void FreyjaFileWriter::writeInt32(int i)
{
	void *ptr = &i;
#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit int\n");
}


void FreyjaFileWriter::writeInt32U(unsigned int u)
{
	void *ptr = &u;
#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit uint\n");
}


void FreyjaFileWriter::writeLong(long l)
{
	void *ptr = &l;
	size_t sz;

#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif

	sz = fwrite(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit int\n");
}


void FreyjaFileWriter::writeLongU(unsigned long ul)
{
	void *ptr = &ul;
#ifdef HAVE_BIG_ENDIAN
	FIX_INT(*ptr)
#endif
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit uint\n");
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

#ifdef UNIT_TEST_FREYJAFILEWRITER
int runFreyjaFileWriterUnitTest(int argc, char *argv[])
{
	FreyjaFileWriter test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaFileWriter class test]\n");

	return runFreyjaFileWriterUnitTest(argc, argv);
}
#endif
