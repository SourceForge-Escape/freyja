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

#include <string.h>


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
			mString = String::strdup(s);
		}
	}

	String(const String &s) :
		mString(NULL),
		mLength(0)
	{
		mLength = s.mLength;
		mString = String::strdup(s.mString);
	}


	String &operator=(const String &s) 
	{
		mLength = s.mLength;
		mString = String::strdup(s.mString);

		return *this;
	}


	String &operator+(const String &s);


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

	// This is better than the GNU extention   ;)
	static size_t strnlen(const char *s, size_t maxlen)
	{
		if (!s || !s[0]) return 0;

		size_t i;

		for (i = 0; s[i] && i < maxlen; ++i, --maxlen)
			;

		return i;
	}


	static char *strdup(const char *src)
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
