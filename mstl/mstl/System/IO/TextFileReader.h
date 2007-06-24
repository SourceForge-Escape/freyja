/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : mstl
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : TextFileReader
 * License : No use w/o permission (C) 2001-2007 Mongoose
 * Comments: Text file reader.  ( Read Only for API safety. )
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.06.23:
 * Mongoose - Created, refactored from SystemIO, now using C99 types internally.
 ==========================================================================*/

#ifndef GUARD__MSTL_SYSTEMIO_TEXTFILEREADER_H_
#define GUARD__MSTL_SYSTEMIO_TEXTFILEREADER_H_

#include "File.h"

#define USE_PARSESTRINGLITERAL_ALLOCATION 1 

namespace mstl {
namespace SystemIO {

class TextFileReader : public File
{
public:

	////////////////////////////////////////////////////////////
	// Public Methods.
	////////////////////////////////////////////////////////////

	TextFileReader() : File() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	~TextFileReader() { }
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

	void AllocateBuffer();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool FindNextChar(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Stops on '\0' and EOF, Returns true if found.
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
	
};


////////////////////////////////////////////////////////////
// Inline Methods.
////////////////////////////////////////////////////////////

inline
bool TextFileReader::Open(const char *filename) 
{ 
	return File::Open(filename, "rb"); 
}


inline
void TextFileReader::AllocateBuffer()
{
	if (mBufferSize == 0)
		SetBufferSize(2048);

	mBuffer[0] = '\0';
}


inline
bool TextFileReader::FindNextChar(char c)
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
char TextFileReader::ReadChar() 
{ 
	return NextChar(); 
}


inline
char TextFileReader::NextChar()
{
	char c;
	fread(&c, 1, 1, mFileHandle);  // heh, yeah
	return c;
}


inline
double TextFileReader::ParseDouble()
{
	double r;
	fscanf(mFileHandle, "%lf", &r);
	return r;
}


inline
float TextFileReader::ParseFloat()
{
	float r;
	fscanf(mFileHandle, "%f", &r);
	return r;
}


inline
int TextFileReader::ParseInteger()
{
	int i;
	fscanf(mFileHandle, "%i", &i);
	return i;
}


inline
bool TextFileReader::ParseBool()
{
	const char *sym = ParseSymbol();

	 if (strncmp(sym, "true", 4) == 0 || strncmp(sym, "TRUE", 4) == 0 )
		 return true;

	return false;
}


inline
bool TextFileReader::ParseMatchingSymbol(const char *symbol)
{
	/* Assumes !(!symbol) && !(!symbol[0]) */
	long l = strlen(symbol);
	const char *sym = ParseSymbol();
	bool test = (!strncmp(symbol, sym, l));

	if (!test) printf("Not matched: '%s' != '%s'\n", symbol, sym);

	return test;
}


inline
const char *TextFileReader::ParseStringLiteral()
{
#if USE_PARSESTRINGLITERAL_ALLOCATION
	long l, i = 0, state = 0;
	char *s;
	char c, lc = 0;

	AllocateBuffer();	

	while (i < mBufferSize && fscanf(mFileHandle, "%c", &c) != EOF)
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
#else
	unsigned int start = GetOffset();
	long i = 0, state = 0;
	char c, lc = 0;
	bool done = false;

	while (!done && fscanf(mFileHandle, "%c", &c) != EOF)
	{
		switch (state)
		{
		case 0:
			if (c == '"')
			{
				state = 1;
				start = GetOffset();
			}
			break;

		case 1:
			if (c == '"' && lc != '\\')  // Allow quote escapes?
			{
				done = true;
			}
			else
			{
				i++;
			}
			break;
		}

		lc = c;
	}

	unsigned int offset = GetOffset();
	SetBufferSize(i+8);
	SetOffset(start);
	fread(mBuffer, i, 1, mFileHandle);
	mBuffer[i] = 0;

	SetOffset(offset);

	return mBuffer;
#endif
}


inline
const char *TextFileReader::GetLine()
{
	long i = 0;
	char c;

	AllocateBuffer();

	while (i < mBufferSize && fscanf(mFileHandle, "%c", &c) != EOF)
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
const char *TextFileReader::ParseSymbol(const char *until)
{
	long i = 0, state = 0, untilI = 0;
	bool untilFound = false;
	char c;

	AllocateBuffer();

	while (i < mBufferSize && fscanf(mFileHandle, "%c", &c) != EOF)
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
const char *TextFileReader::ParseSymbol()
{
	long i = 0, state = 0;
	char c;

	AllocateBuffer();

	while (i < mBufferSize && fscanf(mFileHandle, "%c", &c) != EOF)
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


#endif // GUARD__MSTL_SYSTEMIO_TEXTFILEREADER_H_


