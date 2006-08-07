/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : MSTL
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : String
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_STRING - Builds String class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2003.07.05:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__MSTL_MONGOOSE_STRING_H_
#define GUARD__MSTL_MONGOOSE_STRING_H_

#include <stdarg.h>
#include <string.h>

#ifdef WIN32
#   define strdup String::Strdup
#endif

//#ifndef strnlen
//#   define strnlen(s, maxlen) char *s__s; long s__maxlen; for (s__s = s, s__maxlen = maxlen; s__s && s__maxlen;  ++s__s, --s__maxlen) ; (!s__s) ? 0 : (s__maxlen > 0) ? 1 : -1
//#endif

class String
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	String() :
		mString(NULL),
		mLength(0)
	{
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of String
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.07.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	String(const char *s) :
		mString(NULL),
		mLength(0)
	{
		if (s && s[0])
		{
			mLength = strlen(s);
			mString = String::Strdup(s);
		}
	}


	String(const String &s) :
		mString(NULL),
		mLength(0)
	{
		mLength = s.mLength;
		mString = String::Strdup(s.mString);
	}


	String &operator=(const String &s) 
	{
		mLength = s.mLength;
		mString = String::Strdup(s.mString);

		return *this;
	}


	String operator+(const String &s)
	{
		String t(s); // argh
		char tmp[mLength + s.mLength + 1];
		snprintf("%s%s", mLength + s.mLength, GetCString(), t.GetCString());
		return String(tmp);
	}


	~String()
	{
		Clear();
	}
	/*------------------------------------------------------
	 * Pre  : String object is allocated
	 * Post : Deconstructs an object of String
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.07.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool Empty()
	{
		return !(mLength && mString && mString[0]);
	}


	const char *GetCString()
	{
		return mString;
	}


	// How overboard is this I ask you?
	void Set(const char *format, ...)
	{
		mString = NULL;
		mLength = 0;

		if (format && format[0])
		{
			char buf[1024];

			va_list args;

			va_start(args, format);
			vsnprintf(buf, 1024, format, args);
			va_end(args);

			mLength = strlen(buf);
			mString = String::Strdup(buf);
		}
	}


	// This is better than the GNU extention   ;)
	static size_t Strnlen(const char *s, size_t maxlen)
	{
		if (!s || !s[0]) return 0;

		size_t i;

		for (i = 0; s[i] && i < maxlen; ++i, --maxlen)
			;

		return i;
	}


	static char *Strdup(const char *src)
	{
		char *dest = NULL;
		int len;
		
		
		if (!src || !src[0])
			return NULL;
		
		len = strlen(src);
		dest = new char[len + 1];
		strncpy(dest, src, len);
		dest[len] = 0;
		
		return dest;
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void Clear()
	{
		mLength = 0;

		if (mString)
		{
			delete [] mString;
			mString = NULL;
		}
	}
	

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	char *mString;             /*  */

	unsigned int mLength;      /*  */
};

#endif
