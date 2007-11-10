/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : mstl
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : BufferedTextFileReader
 * License : No use w/o permission (C) 2001-2007 Mongoose
 * Comments: Text file reader.  ( Read Only for API safety. )
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.06.23:
 * Mongoose - Created, refactored from SystemIO, now using C99 types internally.
 ==========================================================================*/

#ifndef GUARD__MSTL_SYSTEMIO_BUFFEREDTEXTFILEREADER_H_
#define GUARD__MSTL_SYSTEMIO_BUFFEREDTEXTFILEREADER_H_

#include "File.h"

namespace mstl {
namespace SystemIO {

class BufferedTextFileReader : public File
{
public:

	////////////////////////////////////////////////////////////
	// Public Methods.
	////////////////////////////////////////////////////////////

	BufferedTextFileReader() : File(), mFileData(NULL), 
				mCursor(0), mStart(0), mEnd(0)  { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~BufferedTextFileReader() { FlushBuffer();  }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	char *GetCompleteFileBuffer();
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

	void FlushBuffer();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual bool IsEndOfFile();
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

	virtual bool OpenChunk(char *buffer, unsigned int size) ;
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

	void AllocateBuffer();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool FindNextEOL();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Search for EOL, will return true if only encounters 
	 *        whitespace until EOL is found
	 *
	 ------------------------------------------------------*/

	bool FindNextChar(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Stops on '\0' and EOF
	 *
	 ------------------------------------------------------*/

	char ReadChar();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	char NextChar();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	double ParseDouble();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	float ParseFloat();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	int ParseInteger();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool ParseBool();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool ParseMatchingSymbol(const char *symbol);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char *ParseStringLiteral();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char *GetLine();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char *ParseSymbol(const char *until);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Delimited by until string eg "," stops parsing at the first ','
	 *
	 ------------------------------------------------------*/

	const char *ParseSymbol();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Supports C++ style comments, and strips whitespace
	 *
	 ------------------------------------------------------*/

 protected:

	char *mFileData;

	unsigned long mCursor;

	unsigned long mStart;

	unsigned long mEnd;
};


////////////////////////////////////////////////////////////
// Inline Methods.
////////////////////////////////////////////////////////////

inline
char *BufferedTextFileReader::GetCompleteFileBuffer() 
{ 
	return mFileData; 
}


inline
unsigned long BufferedTextFileReader::GetCompleteFileBufferSize() 
{ 
	return mEnd; 
}


inline
unsigned long BufferedTextFileReader::GetFileSize() 
{ 
	return mEnd; 
}


inline
void BufferedTextFileReader::FlushBuffer() 
{ 
	if (mFileData) 
		delete [] mFileData; 

	mCursor = mStart = mEnd = 0;
}


inline
bool BufferedTextFileReader::IsEndOfFile()
{
	return (mCursor == mEnd);
}


inline
bool BufferedTextFileReader::IsValidRead(long sz) 
{ 
	return ((mCursor + sz) <= mEnd); 
}


inline
bool BufferedTextFileReader::MemRead(void *ptr, size_t size)
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
long BufferedTextFileReader::GetOffset()
{
	return mCursor-mStart; // for chunks
}


inline
bool BufferedTextFileReader::SetOffset(long offset)
{
	offset += mStart; // for chunks
	bool t = (offset <= (long)mEnd);
	if (t) mCursor = offset;
	return t;
}


inline
void BufferedTextFileReader::SetOffsetToEnd()
{
	mCursor = mEnd;
}


inline
bool BufferedTextFileReader::OpenChunk(char *buffer, unsigned int size) 
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
bool BufferedTextFileReader::OpenChunk(const char *filename, 
							  unsigned int offset, unsigned int size) 
{
	bool load = false;
	FlushBuffer();

	if (File::Open(filename, "rb"))
	{
		fseek(mFileHandle, 0, SEEK_END);

		long tmp = ftell(mFileHandle);
		if ( (offset + size) > (unsigned long)tmp || tmp < 0 )
			return false;

		mEnd = offset + size;
		mStart = offset;
		fseek(mFileHandle, mStart, SEEK_SET);
		mFileData = new char[size+2];
		fread(mFileData, 1, size, mFileHandle); 
		mFileData[size+1] = 0;			

		mCursor = mStart;
		load = true;
	}

	return load;
}


inline
bool BufferedTextFileReader::Open(const char *filename) 
{
	bool load = false;
	FlushBuffer();

	if (File::Open(filename, "rb"))
	{
		fseek(mFileHandle, 0, SEEK_END);
		mEnd = ftell(mFileHandle);
		mStart = 0;
		fseek(mFileHandle, mStart, SEEK_SET);
		mFileData = new char[mEnd+2];
		fread(mFileData, 1, mEnd, mFileHandle); 
		mFileData[mEnd+1] = 0;
		mCursor = 0;

		load = true;
	}

	return load;
}


inline
bool BufferedTextFileReader::ReadBuffer(unsigned long length, unsigned char *buffer)
{
	return MemRead(buffer, length);
}


inline
bool BufferedTextFileReader::ReadString(unsigned long length, char *buffer)
{
	return MemRead(buffer, length);
}


inline
void BufferedTextFileReader::AllocateBuffer()
{
	if (mBufferSize == 0)
		SetBufferSize(2048);

	mBuffer[0] = '\0';
}


inline
bool BufferedTextFileReader::FindNextEOL()
{
	return false;
}


inline
bool BufferedTextFileReader::FindNextChar(char c)
{
	bool found = false;
	char lex;

	while ((lex = NextChar()) && !IsEndOfFile())
	{
		if (c == lex)
		{
			found = true;
			break;
		}
	}

	return found;
}


inline
char BufferedTextFileReader::ReadChar() 
{ 
	return NextChar(); 
}


inline
char BufferedTextFileReader::NextChar()
{
	char c;
	MemRead(&c, 1);
	return c;
}


inline
double BufferedTextFileReader::ParseDouble()
{
	double r;
	const char *s = ParseSymbol();
	sscanf(s, "%lf", &r);
	return r;
}


inline
float BufferedTextFileReader::ParseFloat()
{
	float r;
	const char *s = ParseSymbol();
	sscanf(s, "%f", &r);
	return r;
}


inline
int BufferedTextFileReader::ParseInteger()
{
	int i;
	const char *s = ParseSymbol();
	sscanf(s, "%i", &i);
	return i;
}


inline
bool BufferedTextFileReader::ParseBool()
{
	const char *sym = ParseSymbol();

	 if ( !strncmp(sym, "true", 4) || !strncmp(sym, "TRUE", 4) )
		 return true;

	return false;
}


inline
bool BufferedTextFileReader::ParseMatchingSymbol(const char *symbol)
{
	/* Assumes !(!symbol) && !(!symbol[0]) */
	long l = strlen(symbol);
	const char *sym = ParseSymbol();
	bool test = (!strncmp(symbol, sym, l));

	if (!test) printf("Not matched: '%s' != '%s'\n", symbol, sym);

	return test;
}


inline
const char *BufferedTextFileReader::ParseStringLiteral()
{
	long l, i = 0, state = 0;
	char *s;
	char c, lc = 0;

	AllocateBuffer();	

	while (i < mBufferSize && MemRead(&c, 1) )
	{
		switch (state)
		{
		case 0:
			if (c == '"')
				state = 1;
			break;

		case 1:
			if (c == '"' && lc != '\\')  // Allow quote escapes?
			{
				i = mBufferSize;
			}
			else
			{
				mBuffer[i++] = c;
				mBuffer[i] = 0;
			}
			break;
		}

		lc = c;
	}

	l = strlen(mBuffer);
	s = new char[l+1];
	strncpy(s, mBuffer, l);
	s[l] = 0;

	return s;
}


inline
const char *BufferedTextFileReader::GetLine()
{
	long i = 0;
	char c;

	AllocateBuffer();

	while (i < mBufferSize && MemRead(&c, 1) )
	{
		switch (c)
		{
		case '\r':
			break;

		case '\n':
			i = mBufferSize;
			break;

		default:
			mBuffer[i++] = c;
			mBuffer[i] = 0;
		}
	}

	return mBuffer;
}


inline
const char *BufferedTextFileReader::ParseSymbol(const char *until)
{
	long i = 0, state = 0, untilI = 0;
	bool untilFound = false;
	char c;

	AllocateBuffer();

	while (i < mBufferSize && MemRead(&c, 1) )
	{
		if (c == until[untilI])
		{
			++untilI;

			// Found the delimter, so quit parsing
			if ( untilI == '\0' )
				untilFound = true;;
		}
		else
		{
			untilI = 0;
		}
	  

		switch (state)
		{
		case 0:
			if (c == '/')
			{
				state = 1;
				mBuffer[i++] = c;
				mBuffer[i] = 0;	
			}
			else if (c == ' ' || c == '\r' || c == '\n' || c == '\t')
			{
				if (i > 0)
					i = mBufferSize;
			}
			else
			{
				mBuffer[i++] = c;
				mBuffer[i] = 0;
			}
			break;

		case 1:
			if (c == '/')
			{
				state = 2;
				--i;
				mBuffer[i] = 0;
			}
			else
			{
				state = 0;
				mBuffer[i++] = c;
				mBuffer[i] = 0;
			}
			break;

		case 2:
			if (c == '\n')
			{
				/* Only wrap lines when given a only comment line(s) */
				if (i > 0)
					i = mBufferSize;
				else
					state = 0;
			}
			break;
		}

		if (untilFound)
			break;
	}

	return mBuffer;
}


inline
const char *BufferedTextFileReader::ParseSymbol()
{
	long i = 0, state = 0;
	char c;

	AllocateBuffer();

	while (i < mBufferSize && MemRead(&c, 1) )
	{
		switch (state)
		{
		case 0:
			if (c == '/')
			{
				state = 1;
				mBuffer[i++] = c;
				mBuffer[i] = 0;	
			}
			else if (c == ' ' || c == '\r' || c == '\n' || c == '\t')
			{
				if (i > 0)
					i = mBufferSize;
			}
			else
			{
				mBuffer[i++] = c;
				mBuffer[i] = 0;
			}
			break;

		case 1:
			if (c == '/')
			{
				state = 2;
				--i;
				mBuffer[i] = 0;
			}
			else
			{
				state = 0;
				mBuffer[i++] = c;
				mBuffer[i] = 0;
			}
			break;

		case 2:
			if (c == '\n')
			{
				/* Only wrap lines when given a only comment line(s) */
				if (i > 0)
					i = mBufferSize;
				else
					state = 0;
			}
			break;
		}
	}

	return mBuffer;
}



} // namespace SystemIO
} // namespace mstl


#endif // GUARD__MSTL_SYSTEMIO_BUFFEREDTEXTFILEREADER_H_

