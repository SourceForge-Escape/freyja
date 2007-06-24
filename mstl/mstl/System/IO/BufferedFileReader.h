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
 * Notes   : This should really use sliding windows instead of one large 
 *           buffer, so until then only load small files with this. 
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.06.23:
 * Mongoose - Created, refactored from SystemIO, now using C99 types internally.
 ==========================================================================*/

#ifndef GUARD__MSTL_SYSTEM_IO_BUFFEREDFILEREADER_H_
#define GUARD__MSTL_SYSTEM_IO_BUFFEREDFILEREADER_H_

#include <stdint.h>
#include "FileReader.h"

namespace mstl {
namespace SystemIO {


class BufferedFileReader : public FileReader
{
public:

	////////////////////////////////////////////////////////////
	// Public Methods.
	////////////////////////////////////////////////////////////

	BufferedFileReader();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	BufferedFileReader(const BufferedFileReader &b);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	BufferedFileReader &operator=(const BufferedFileReader &b);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assignment operator.
	 *
	 ------------------------------------------------------*/

	~BufferedFileReader();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	unsigned char *GetCompleteFileBuffer();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	unsigned long GetCompleteFileBufferSize();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual unsigned long GetFileSize();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void FlushBuffer() ;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool IsValidRead(long sz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool MemRead(void *ptr, size_t size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual long GetOffset();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual bool SetOffset(long offset);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void SetOffsetToEnd();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual bool OpenChunk(unsigned char *buffer, unsigned int size) ;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual bool OpenChunk(const char *filename, 
								  unsigned int offset, unsigned int size) ;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual bool Open(const char *filename) ;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
	
	virtual bool ReadBuffer(unsigned long length, unsigned char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual bool ReadString(unsigned long length, char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void ReadInt32(int &i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void ReadLong(long &l);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void ReadInt32U(unsigned int &u);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void ReadFloat32(float &f);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void ReadInt32Array(long size, int array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void ReadLongArray(long size, long array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void ReadFloat32Array(long size, float array[]);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual float ReadFloat32();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual char ReadInt8();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual unsigned char ReadByte();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual unsigned char ReadInt8U();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual short ReadInt16();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual unsigned short ReadInt16U();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual int ReadInt32();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual unsigned int ReadInt32U();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual long ReadLong();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual unsigned long ReadLongU();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


 protected:

	unsigned char *mFileData;

	unsigned long mCursor;

	unsigned long mStart;

	unsigned long mEnd;
};


////////////////////////////////////////////////////////////
// Inline Methods.
////////////////////////////////////////////////////////////

inline
BufferedFileReader::BufferedFileReader() : FileReader(), 
					  mFileData(NULL), mCursor(0), mStart(0), mEnd(0)
{
}


inline
BufferedFileReader::BufferedFileReader(const BufferedFileReader &b) : FileReader(), 
	mFileData(NULL), mCursor(b.mCursor), mStart(b.mStart), mEnd(b.mEnd) 
{
	mFileData = new unsigned char[b.mEnd+1];
	memcpy(mFileData, b.mFileData, b.mEnd);
}


inline
BufferedFileReader &BufferedFileReader::operator=(const BufferedFileReader &b) 
{
	FlushBuffer();
	mCursor = b.mCursor;
	mStart = b.mStart;
	mEnd = b.mEnd;
	mFileData = new unsigned char[b.mEnd+1];
	memcpy(mFileData, b.mFileData, b.mEnd);
	
	return *this;
}


inline
BufferedFileReader::~BufferedFileReader() 
{
	FlushBuffer();  
}


inline
unsigned char *BufferedFileReader::GetCompleteFileBuffer() 
{ 
	return mFileData; 
}


inline
unsigned long BufferedFileReader::GetCompleteFileBufferSize() 
{ 
	return mEnd; 
}


inline
unsigned long BufferedFileReader::GetFileSize() 
{ 
	return mEnd; 
}


inline
void BufferedFileReader::FlushBuffer() 
{ 
	if (mFileData) 
		delete [] mFileData; 

	mCursor = mStart = mEnd = 0;
}


inline
bool BufferedFileReader::IsValidRead(long sz) 
{ 
	return ((mCursor + sz) <= mEnd); 
}


inline
bool BufferedFileReader::MemRead(void *ptr, size_t size)
{
	bool read = false;

	// FIXME: Should I do partial reads here to match file I/O?
	if (IsValidRead(size))
	{
		memcpy(ptr, mFileData+mCursor, size);
		mCursor += size;
		read = true;
	}
	else
	{
		//MSTL_MSG("Invalid read size %i.  %i / %i", size, mCursor, mEnd);
	}

	return read;
}


inline
long BufferedFileReader::GetOffset()
{
	return mCursor-mStart; // for chunks
}


inline
bool BufferedFileReader::SetOffset(long offset)
{
	offset += mStart; // for chunks
	bool t = (offset <= (long)mEnd);
	if (t) mCursor = offset;
	return t;
}


inline
void BufferedFileReader::SetOffsetToEnd()
{
	mCursor = mEnd;
}


inline
bool BufferedFileReader::OpenChunk(unsigned char *buffer, unsigned int size) 
{
	bool load = false;
	FlushBuffer();

	if (buffer)
	{
		mEnd = size;
		mStart = 0;
		mCursor = mStart;
		mFileData = buffer;
		load = true;
	}

	return load;
}


inline
bool BufferedFileReader::OpenChunk(const char *filename, 
							  unsigned int offset, unsigned int size) 
{
	bool load = false;
	FlushBuffer();

	if (File::Open(filename, "rb"))
	{
		fseek(mFileHandle, 0, SEEK_END);

		if (offset + size > ftell(mFileHandle))
			return false;

		mEnd = offset + size;
		mStart = offset;
		fseek(mFileHandle, mStart, SEEK_SET);
		mFileData = new unsigned char[size+1];
#ifdef DEBUG
		//memset(mFileData, 0xcd, size);
#endif
		fread(mFileData, 1, size, mFileHandle); 
		//FileReader::ReadBuffer(size, mFileData);

		mCursor = mStart;

		load = true;
	}

	return load;
}


inline
bool BufferedFileReader::Open(const char *filename) 
{
	bool load = false;
	FlushBuffer();

	if (File::Open(filename, "rb"))
	{
		fseek(mFileHandle, 0, SEEK_END);
		mEnd = ftell(mFileHandle);
		mStart = 0;
		fseek(mFileHandle, mStart, SEEK_SET);
		mFileData = new unsigned char[mEnd+1];
#ifdef DEBUG
		//memset(mFileData, 0xcd, mEnd);
#endif
		fread(mFileData, 1, mEnd, mFileHandle); 
		//FileReader::ReadBuffer(mEnd, mFileData);

		mCursor = 0;

		load = true;
	}

	return load;
}


inline
bool BufferedFileReader::ReadBuffer(unsigned long length, unsigned char *buffer)
{
	return MemRead(buffer, length);
}


inline
bool BufferedFileReader::ReadString(unsigned long length, char *buffer)
{
	return MemRead(buffer, length);
}


inline
void BufferedFileReader::ReadInt32(int &i) 
{ 
	i = ReadInt32(); 
}


inline
void BufferedFileReader::ReadLong(long &l) 
{ 
	l = ReadLong(); 
}


inline
void BufferedFileReader::ReadInt32U(unsigned int &u) 
{ 
	u = ReadInt32U(); 
}


inline
void BufferedFileReader::ReadFloat32(float &f) 
{ 
	f = ReadFloat32(); 
}


inline
void BufferedFileReader::ReadInt32Array(long size, int array[])
{	
	for (unsigned int i = 0; i < size; ++i)
	{
		ReadInt32(array[i]);
	}
}


inline
void BufferedFileReader::ReadLongArray(long size, long array[])
{	
	for (unsigned int i = 0; i < size; ++i)
	{
		ReadLong(array[i]);
	}
}


inline
void BufferedFileReader::ReadFloat32Array(long size, float array[])
{	
	for (unsigned int i = 0; i < size; ++i)
	{
		ReadFloat32(array[i]);
	}
}


inline
float BufferedFileReader::ReadFloat32()
{	
	float r = 0.0f;
	void *ptr = &r;
	MemRead(ptr, 4);

#if __BIG_ENDIAN__
	FIX_FLOAT(*ptr)
#endif

	return r;
}


inline
char BufferedFileReader::ReadInt8()
{
	int8_t c;
	MemRead(&c, 1);
	return c;
}


inline
unsigned char BufferedFileReader::ReadByte()
{
	uint8_t u;
	MemRead(&u, 1);
	return u;
}


inline
unsigned char BufferedFileReader::ReadInt8U()
{
	uint8_t u;
	MemRead(&u, 1);
	return u;
}


inline
short BufferedFileReader::ReadInt16()
{
	int16_t i = 0;
	void *ptr = &i;
	MemRead(ptr, 2);

#ifdef __BIG_ENDIAN__
	FIX_SHORT(*ptr)
#endif
	return i;
}


inline
unsigned short BufferedFileReader::ReadInt16U()
{
	uint16_t u = 0;
	void *ptr = &u;
	MemRead(ptr, 2);

#ifdef __BIG_ENDIAN__
	FIX_SHORT(*ptr)
#endif
	return u;
}


inline
int BufferedFileReader::ReadInt32()
{
	int32_t i = 0;
	void *ptr = &i;
	MemRead(ptr, 4);

#ifdef __BIG_ENDIAN__
	FIX_INT(*ptr)
#endif
	return i;
}


inline
unsigned int BufferedFileReader::ReadInt32U()
{
	uint32_t u = 0;
	void *ptr = &u;
	MemRead(ptr, 4);

#ifdef __BIG_ENDIAN__
	FIX_INT(*ptr)
#endif
	return u;
}


inline
long BufferedFileReader::ReadLong()
{
	int32_t i = 0;
	void *ptr = &i;
	MemRead(ptr, 4);

#ifdef __BIG_ENDIAN__
	if (mHostOrder == LITTLE)
		FIX_INT(*ptr);
#else
	if (mHostOrder == BIG)
		FIX_INT(*(uint32_t*)ptr);//i = SWAP_4(i);
#endif
	return i;
}


inline
unsigned long BufferedFileReader::ReadLongU()
{
	uint32_t u = 0;
	void *ptr = &u;
	MemRead(ptr, 4);

#ifdef __BIG_ENDIAN__
	if (!mHostOrder == LITTLE)
		FIX_INT(*ptr);
#else
	if (mHostOrder == BIG)
		u = SWAP_4(u);
#endif
	return u;
}



} // namespace SystemIO
} // namespace mstl


#endif // GUARD__MSTL_SYSTEMIO_BUFFEREDFILEREADER_H_


