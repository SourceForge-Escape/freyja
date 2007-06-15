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
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#   define strdup String::Strdup
#endif

//#ifndef strnlen
//#   define strnlen(s, maxlen) char *s__s; long s__maxlen; for (s__s = s, s__maxlen = maxlen; s__s && s__maxlen;  ++s__s, --s__maxlen) ; (!s__s) ? 0 : (s__maxlen > 0) ? 1 : -1
//#endif

namespace mstl {

class String
{
 public:

	static const int npos = -1;


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

	char &operator [] (unsigned int i) { return mString[i]; }

	String &operator+=(const String &s) { return *this = *this + s; }

	bool operator!=(const char *s) { return ( Strcmp(s) != 0 );	}
	bool operator!=(const String &s) { return ( Strcmp(s.mString) != 0 ); }

	bool operator==(const char *s) { return ( Strcmp(s) == 0 );	}
	bool operator==(const String &s) { return ( Strcmp(s.mString) == 0 );	}

	bool operator<(const String &s) { return ( Strcmp(s.mString) == -1 ); }
	bool operator<(const char *s) { return ( Strcmp(s) == -1 ); }

	bool operator<=(const String &s)	{ return ( Strcmp(s.mString) <= 0 ); }
	bool operator<=(const char *s) { return ( Strcmp(s) <= 0 ); }

	bool operator>(const String &s) { return ( Strcmp(s.mString) == 1 ); }
	bool operator>(const char *s) { return ( Strcmp(s) == 1 ); }

	bool operator>=(const String &s) { return ( Strcmp(s.mString) >= 0 ); }
	bool operator>=(const char *s) {	return ( Strcmp(s) >= 0 ); }

	String operator+(const String &s)
	{
		String s2 = s;
		char *tmp = new char[mLength + s2.mLength + 2];
		snprintf(tmp, mLength + s2.mLength + 1, "%s%s", 
					c_str() ? c_str() : "", 
					s2.c_str() ? s2.c_str() : "");
		tmp[mLength + s2.mLength + 1] = 0;
		s2 = tmp;
		delete [] tmp;

		return s2;
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
	// Legacy
	////////////////////////////////////////////////////////////

	void Clear() { clear(); }

	const char *GetCString() {	return mString; }
	
	unsigned int GetLength() { return mLength; }


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	bool Empty()
	{
		return !(mLength && mString && mString[0]);
	}


	int find_last_of(const char c, int idx = -1)
	{
		if (!mString)
			return npos;

		if (idx < 0 || idx > (int)mLength)
		{
			idx = mLength - 1;
		}

		for (unsigned int i = idx; i > 0; --i)
		{
			if (mString[i] == c)
				return i; 
		}

		return npos;
	}


	unsigned int length() { return mLength; }

	const char *c_str()
	{
		return mString;
	}


	void Replace(char a, char b)
	{
		if (!mString || a == '\0')
			return;

		for (unsigned int i = 0; i < mLength; ++i)
		{
			if (mString[i] == a)
				mString[i] = b; 
		}
	}


	// How overboard is this I ask you?
	void Set(const char *format, ...)
	{
		mString = NULL;
		mLength = 0;

		if (format && format[0])
		{
			const unsigned int sz = 1024;
			char buf[sz];

			va_list args;

			va_start(args, format);
			vsnprintf(buf, sz, format, args);
			buf[sz-1] = 0;
			va_end(args);

			mLength = strlen(buf);
			mString = String::Strdup(buf);
		}
	}


	int Strcmp(const char *s)
	{
		// Don't assume strlen can handle NULL or even empty strings
		size_t len = (s == NULL || s[0] == '\0') ? 0 : strlen(s);

		if (len == 0 || mLength == 0)
		{
			return ( ( !len && !mLength ) ? 0 :	( ( !mLength ) ? -1 : 1 ) );
		}

		len = ( len < mLength ) ? len : mLength;

		return strncmp(c_str(), s, len);
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

	void clear()
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

} // End namespace mst

#endif
