/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : mstl
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : FileWriter
 * License : No use w/o permission (C) 2001-2007 Mongoose
 * Comments: Binary file writer.
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.06.23:
 * Mongoose - Created, refactored from SystemIO, now using C99 types internally.
 ==========================================================================*/

#ifndef GUARD__MSTL_SYSTEMIO_FILEWRITER_H_
#define GUARD__MSTL_SYSTEMIO_FILEWRITER_H_

#include <stdint.h>
#include "File.h"

namespace mstl {
namespace SystemIO {

class FileWriter : public File
{
public:

	////////////////////////////////////////////////////////////
	// Public Methods.
	////////////////////////////////////////////////////////////

	FileWriter();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~FileWriter();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/
	
	bool Open(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteBuffer(unsigned int length, unsigned char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteString(unsigned int length, const char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteString(unsigned int length, char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteInt16Array(long size, short int array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteInt32Array(long size, int array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteFloat32Array(long size, float array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteFloat32(float r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteByte(unsigned char u);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteInt8(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteInt8U(unsigned char u);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteInt16(short i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
		
	bool WriteInt16U(unsigned short u);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteInt32(int i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteInt32U(unsigned int u);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteLong(long l);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool WriteLongU(unsigned long ul);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

};


////////////////////////////////////////////////////////////
// Inline Methods.
////////////////////////////////////////////////////////////

inline
FileWriter::FileWriter() : File() 
{ 
}


inline
FileWriter::~FileWriter() 
{ 
}


inline
bool FileWriter::Open(const char *filename) 
{ 
	return File::Open(filename, "wb"); 
}


inline
bool FileWriter::WriteBuffer(unsigned int length, unsigned char *buffer)
{
	return (fwrite(buffer, length, 1, mFileHandle) == 1);  // heh, yeah
}


inline
bool FileWriter::WriteString(unsigned int length, const char *buffer)
{
	return (fwrite(buffer, length, 1, mFileHandle) == 1);  // heh, yeah
}


inline
bool FileWriter::WriteString(unsigned int length, char *buffer)
{
	return (fwrite(buffer, length, 1, mFileHandle) == 1);  // heh, yeah
}


inline
bool FileWriter::WriteInt16Array(long size, short int array[])
{	
	bool t = false;
	for (unsigned int i = 0; i < size; ++i)
	{
		t = WriteInt16(array[i]);
	}
	return t;
}


inline
bool FileWriter::WriteInt32Array(long size, int array[])
{	
	bool t = false;
	for (unsigned int i = 0; i < size; ++i)
	{
		t = WriteInt32(array[i]);
	}
	return t;
}


inline
bool FileWriter::WriteFloat32Array(long size, float array[])
{
	bool t = false;
	for (unsigned int i = 0; i < size; ++i)
	{
		t = WriteFloat32(array[i]);
	}
	return t;
}


inline
bool FileWriter::WriteFloat32(float r)
{	
	void *ptr = &r;
#if __BIG_ENDIAN__
	FIX_FLOAT(*ptr)
#endif
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	return !(sz < 1);
}


inline
bool FileWriter::WriteByte(unsigned char u)
{
	return (fwrite(&u, 1, 1, mFileHandle) == 1);
}


inline
bool FileWriter::WriteInt8(char c)
{
	return (fwrite(&c, 1, 1, mFileHandle) == 1);
}


inline
bool FileWriter::WriteInt8U(unsigned char u)
{
	return (fwrite(&u, 1, 1, mFileHandle) == 1);  // heh, yeah
}


inline
bool FileWriter::WriteInt16(short i)
{
	int16_t ii = i;
	void *ptr = &ii;
#ifdef __BIG_ENDIAN__
	FIX_SHORT(*ptr)
#endif
	size_t sz = fwrite(ptr, 2, 1, mFileHandle);

	return !(sz < 1);
}

	
inline
bool FileWriter::WriteInt16U(unsigned short u)
{
	uint16_t uu = u;
	void *ptr = &uu;
#ifdef __BIG_ENDIAN__
	FIX_SHORT(*ptr)
#endif
	size_t sz = fwrite(ptr, 2, 1, mFileHandle);

	return !(sz < 1);
}


inline
bool FileWriter::WriteInt32(int i)
{
	int32_t ii = i;
	void *ptr = &ii;
#ifdef __BIG_ENDIAN__
	FIX_INT(*ptr)
#endif
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	return !(sz < 1);
}


inline
bool FileWriter::WriteInt32U(unsigned int u)
{
	uint32_t uu = u;
	void *ptr = &uu;
#ifdef __BIG_ENDIAN__
	FIX_INT(*ptr)
#endif
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	return !(sz < 1);
}


inline
bool FileWriter::WriteLong(long l)
{
	int32_t ll = l;
	void *ptr = &ll;
	size_t sz;

#ifdef __BIG_ENDIAN__
	FIX_INT(*ptr)
#endif

	sz = fwrite(ptr, 4, 1, mFileHandle);

	return !(sz < 1);
}


inline
bool FileWriter::WriteLongU(unsigned long ul)
{
	uint32_t ll = ul;
	void *ptr = &ll;
#ifdef __BIG_ENDIAN__
	FIX_INT(*ptr)
#endif
	size_t sz = fwrite(ptr, 4, 1, mFileHandle);

	return !(sz < 1);
}

} // namespace SystemIO
} // namespace mstl


#endif // GUARD__MSTL_SYSTEMIO_FILEWRITER_H_


