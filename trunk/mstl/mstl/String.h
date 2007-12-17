/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : MSTL
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : String
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: 
 *
 *-- History ------------------------------------------------ 
 *
 * 2003.07.05:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__MSTL_STRING_H_
#define GUARD__MSTL_STRING_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


namespace mstl {

class String
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	String();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of String.
	 *
	 ------------------------------------------------------*/

	String(const char* s);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of String.
	 *
	 ------------------------------------------------------*/

	String(const char* s, const int length);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of String.
	 *
	 ------------------------------------------------------*/

	String(const String& s);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of String.
	 *
	 ------------------------------------------------------*/

	~String();
	/*------------------------------------------------------
	 * Pre  : String object is allocated.
	 * Post : Deconstruction of an object of String.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Operators
	////////////////////////////////////////////////////////////

	String& operator=(const String& s); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assigns value of one string to another.
	 *
	 ------------------------------------------------------*/

	String operator+(const String& s); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Appends the value of one string to another.
	 *        Returns new string.
	 *
	 ------------------------------------------------------*/

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


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	char& at(unsigned int pos);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return reference to the character at <pos>.
	 *
	 ------------------------------------------------------*/

	bool empty() const 
	{ return !(mLength && mString && mString[0]); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Is the string empty?
	 *
	 ------------------------------------------------------*/

	int find_last_of(const char c, int idx = -1);
	/*------------------------------------------------------
	 * Pre  : Start at <idx> position in the string, 
	 *        or default to the beginning.
	 * Post : Returns the position of the last occurance of <c> or npos.
	 *
	 ------------------------------------------------------*/

	unsigned int length() 
	{ return mLength; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns length of String's c-string.
	 *
	 ------------------------------------------------------*/

	const char* c_str() 
	{ return mString; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the String's c-string.
	 *
	 ------------------------------------------------------*/

	String substr(unsigned int pos, int num = npos)
	{ return String( mString+pos, num ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns a substring of String's c-string.
	 *
	 ------------------------------------------------------*/

	int Strcmp(const char *s);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : strcmp() like function for Strings and c-strings.
	 *
	 ------------------------------------------------------*/

	static size_t Strnlen(const char* s, size_t maxlen);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Return lenght of string with given a max length.
	 *
	 ------------------------------------------------------*/

	static char* Strdup(const char* src);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Make a duplicate of a c-string and return it.
	 *
	 ------------------------------------------------------*/

	static const int npos = -1;


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deallocate String and set c-string to NULL.
	 *
	 ------------------------------------------------------*/

	void Replace(char a, char b);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Replace characters <a> with <b> in String.
	 *
	 ------------------------------------------------------*/

	void Set(const char *format, ...);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Printf style set method for String.
	 *
	 ------------------------------------------------------*/
	

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	char* mString;             /* C-string data. */

	unsigned int mLength;      /* Lenght of c-string. */
};




////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

inline
String::String() :
	mString(NULL),
	mLength(0)
{	}


inline
String::String(const char* s) :
	mString(NULL),
	mLength(0)
{
	if (s && s[0])
	{
		mLength = strlen(s);
		mString = String::Strdup(s);
	}
}


inline
String::String(const char* s, const int length) :
	mString(NULL),
	mLength(0)
{
	if (s && s[0] && length)
	{
		mLength = strlen(s);
		mString = String::Strdup(s);
	}

	if (length > 0)
	{
		mLength = length;
		mString[length] = 0;
	}
}


inline
String::String(const String& s) :
	mString(NULL),
	mLength(0)
{
	mLength = s.mLength;
	mString = String::Strdup(s.mString);
}


inline
String::~String()
{
	clear();
}


////////////////////////////////////////////////////////////
// Operators
////////////////////////////////////////////////////////////

inline
String& String::operator=(const String& s) 
{
	mLength = s.mLength;
	mString = String::Strdup(s.mString);
	return *this;
}


inline
String String::operator+(const String& s)
{
	String s2 = s;
	char* tmp = new char[mLength + s2.mLength + 2];
	snprintf(tmp, mLength + s2.mLength + 1, "%s%s", 
				c_str() ? c_str() : "", 
				s2.c_str() ? s2.c_str() : "");
	tmp[mLength + s2.mLength + 1] = 0;
	s2 = tmp;
	delete [] tmp;

	return s2;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

inline
char& String::at(unsigned int pos) 
{ 
	return (pos < mLength) ? mString[pos] : mString[mLength-1]; 
}


inline
int String::find_last_of(const char c, int idx)
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


inline
char* String::Strdup(const char* src)
{
	if (!src || !src[0])
		return NULL;
	
	int len = strlen(src);
	char* dest = new char[len + 1];
	strncpy(dest, src, len);
	dest[len] = 0;
	
	return dest;
}


inline
int String::Strcmp(const char* s)
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
inline
size_t String::Strnlen(const char *s, size_t maxlen)
{
	if (!s || !s[0]) return 0;

	size_t i = 0;

	for (; s[i] && i < maxlen; ++i, --maxlen)
		;

	return i;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

inline
void String::clear()
{
	mLength = 0;

	if (mString)
	{
		delete [] mString;
		mString = NULL;
	}
}


inline
void String::Replace(char a, char b)
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
inline
void String::Set(const char *format, ...)
{
	mString = NULL;
	mLength = 0;

	if ( format && format[0] )
	{
		const unsigned int sz = 1024;
		char buf[sz];

		va_list args;
		va_start(args, format);
		int truncated = vsnprintf(buf, sz, format, args);
		buf[sz-1] = 0;
		va_end(args);

		if ( truncated >= (int)sz )
		{
			unsigned int len = truncated + 1; // Doesn't include '\0'
			mString = new char[ len ];

			va_start( args, format );
			vsnprintf( mString, len, format, args );
			mString[len-1] = '\0';
			va_end( args );
			
			mLength = strlen( mString );
		}
		else
		{
			mLength = strlen(buf);
			mString = String::Strdup(buf);
		}
	}
}



#ifdef WIN32
#   define strdup String::Strdup
#endif

} // End namespace mst

#endif // GUARD__MSTL_STRING_H_

