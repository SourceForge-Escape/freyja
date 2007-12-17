/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : mstl
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : FileReader
 * License : No use w/o permission (C) 2001-2007 Mongoose
 * Comments: Binary file reader.  ( Read Only for API safety. )
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.06.23:
 * Mongoose - Created, refactored from SystemIO, now using C99 types internally.
 ==========================================================================*/

#ifndef GUARD__MSTL_SYSTEMIO_FILEREADER_H_
#define GUARD__MSTL_SYSTEMIO_FILEREADER_H_

#include <stdint.h>
#include "File.h"

namespace mstl {
namespace SystemIO {

class FileReader : public File
{
public:

	////////////////////////////////////////////////////////////
	// Public Methods.
	////////////////////////////////////////////////////////////

	FileReader() : File() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~FileReader() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	virtual bool Open(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if the file was opened read only.
	 *
	 ------------------------------------------------------*/
	
	virtual bool ReadBuffer(unsigned long length, unsigned char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read a buffer from the file, moves offset.
	 *
	 ------------------------------------------------------*/

	virtual bool ReadString(unsigned long length, char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read a string from the file, moves offset.
	 *
	 ------------------------------------------------------*/

	virtual void ReadInt32(int &i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read a 32bit int.
	 *
	 ------------------------------------------------------*/

	virtual void ReadLong(long &l);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read a 32bit int.
	 *
	 ------------------------------------------------------*/

	virtual void ReadInt32U(unsigned int &u);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read a 32bit unsigned int.
	 *
	 ------------------------------------------------------*/

	virtual void ReadFloat32(float &f);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read a 32bit float.
	 *
	 ------------------------------------------------------*/

	virtual void ReadInt16Array(unsigned long size, short array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an array of 16bit ints.
	 *
	 ------------------------------------------------------*/

	virtual void ReadInt32Array(unsigned long size, int array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an array of 32bit ints.
	 *
	 ------------------------------------------------------*/

	virtual void ReadLongArray(unsigned long size, long array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an array of 32bit ints.
	 *
	 ------------------------------------------------------*/

	virtual void ReadFloat32Array(unsigned long size, float array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an array of 32bit floats.
	 *
	 ------------------------------------------------------*/

	virtual float ReadFloat32();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read a 32bit float.
	 *
	 ------------------------------------------------------*/

	virtual char ReadInt8();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read a 8bit integer.
	 *
	 ------------------------------------------------------*/

	virtual unsigned char ReadByte();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an 8bit unsigned integer. 
	 *
	 ------------------------------------------------------*/

	virtual unsigned char ReadInt8U();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an 8bit unsigned integer. 
	 *
	 ------------------------------------------------------*/

	virtual short ReadInt16();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an 16bit signed integer. 
	 *
	 ------------------------------------------------------*/

	virtual unsigned short ReadInt16U();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an 16bit unsigned integer.
	 *
	 ------------------------------------------------------*/

	virtual int ReadInt32();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an 32bit signed integer.
	 *
	 ------------------------------------------------------*/

	virtual unsigned int ReadInt32U();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an 32bit unsigned integer.
	 *
	 ------------------------------------------------------*/

	virtual long ReadLong();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an 32bit unsigned integer.
	 *
	 ------------------------------------------------------*/

	virtual unsigned long ReadLongU();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read an 32bit unsigned integer.
	 *
	 ------------------------------------------------------*/

};


////////////////////////////////////////////////////////////
// Inline Methods.
////////////////////////////////////////////////////////////

inline
bool FileReader::Open(const char *filename) 
{
	return File::Open(filename, "rb");
}


inline
bool FileReader::ReadBuffer(unsigned long length, unsigned char *buffer)
{
	return ( fread(buffer, length, 1, mFileHandle) == 1 );	
}


inline
bool FileReader::ReadString(unsigned long length, char *buffer)
{
	return ( fread(buffer, length, 1, mFileHandle) == 1 );
}


inline
void FileReader::ReadInt32(int &i) 
{ 
	i = ReadInt32(); 
}


inline
void FileReader::ReadLong(long &l)
{ 
	l = ReadLong(); 
}


inline
void FileReader::ReadInt32U(unsigned int &u) 
{ 
	u = ReadInt32U(); 
}


inline
void FileReader::ReadFloat32(float &f) 
{ 
	f = ReadFloat32(); 
}


inline
void FileReader::ReadInt16Array(unsigned long size, short array[])
{
	fread(array, size*2, 1, mFileHandle);
}


inline
void FileReader::ReadInt32Array(unsigned long size, int array[])
{	
	for (unsigned int i = 0; i < size; ++i)
	{
		ReadInt32(array[i]);
	}
}


inline
void FileReader::ReadLongArray(unsigned long size, long array[])
{	
	for (unsigned int i = 0; i < size; ++i)
	{
		ReadLong(array[i]);
	}
}


inline
void FileReader::ReadFloat32Array(unsigned long size, float array[])
{	
	for (unsigned int i = 0; i < size; ++i)
	{
		ReadFloat32(array[i]);
	}
}


inline
float FileReader::ReadFloat32()
{	
	float r;
	void *ptr = &r;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit float\n");

#if __BIG_ENDIAN__
	FIX_FLOAT(ptr);
#endif

	return r;
}


inline
char FileReader::ReadInt8()
{
	int8_t c;
	fread(&c, 1, 1, mFileHandle);  
	return c;
}


inline
unsigned char FileReader::ReadByte()
{
	uint8_t u;
	fread(&u, 1, 1, mFileHandle);  
	return u;
}


inline
unsigned char FileReader::ReadInt8U()
{
	uint8_t u;
	fread(&u, 1, 1, mFileHandle);  
	return u;
}


inline
short FileReader::ReadInt16()
{
	int16_t i = 0;
	void *ptr = &i;
	size_t sz = fread(ptr, 2, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 16bit int\n");

#ifdef __BIG_ENDIAN__
	FIX_SHORT(ptr);
#endif
	return i;
}


inline
unsigned short FileReader::ReadInt16U()
{
	uint16_t u = 0;
	void *ptr = &u;
	size_t sz = fread(ptr, 2, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 16bit uint\n");

#ifdef __BIG_ENDIAN__
	FIX_SHORT(ptr);
#endif
	return u;
}


inline
int FileReader::ReadInt32()
{
	int32_t i = 0;
	void *ptr = &i;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit int\n");

#ifdef __BIG_ENDIAN__
	FIX_INT(ptr);
#endif
	return i;
}


inline
unsigned int FileReader::ReadInt32U()
{
	uint32_t u = 0;
	void *ptr = &u;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit uint\n");

#ifdef __BIG_ENDIAN__
	FIX_INT(ptr);
#endif
	return u;
}


inline
long FileReader::ReadLong()
{
	int32_t i = 0;
	void *ptr = &i;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit int\n");

#ifdef __BIG_ENDIAN__
	if (mHostOrder == LITTLE)
		FIX_INT(ptr);
#else
	if (mHostOrder == BIG)
		FIX_INT(*(unsigned int*)ptr);//i = SWAP_4(i);
#endif
	return i;
}


inline
unsigned long FileReader::ReadLongU()
{
	uint32_t u = 0;
	void *ptr = &u;
	size_t sz = fread(ptr, 4, 1, mFileHandle);

	if (sz < 1)
		printf("FreyjaFileReader: ERROR failed to read 32bit uint\n");

#ifdef __BIG_ENDIAN__
	if (!mHostOrder == LITTLE)
		FIX_INT(ptr);
#else
	if (mHostOrder == BIG)
		u = SWAP_4(u);
#endif
	return u;
}


} // namespace SystemIO
} // namespace mstl


#endif // GUARD__MSTL_SYSTEMIO_FILEREADER_H_


