/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : Printer
 * License : No use w/o permission (C) 2004-2007 Mongoose
 * Comments: This is for rerouting output and error messages.
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Refactored.
 *
 * 2004.10.30:
 * Mongoose - Created.
 ==========================================================================*/

#ifndef GUARD__FREYJA_PRINTER_H_
#define GUARD__FREYJA_PRINTER_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <mstl/String.h>


namespace freyja {

class Printer
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Printer();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~Printer();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	virtual void Log( const char* filename, const char* format, ... );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Print( const char* format, ... );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void PrintArgs( const char* format, va_list* args );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void PrintMessage( const char* s );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void PrintError( const char* format, ... );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void PrintErrorArgs( const char* format, va_list* args );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	virtual void PrintErrorMessage( const char* s );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/
};


////////////////////////////////////////////////////////////
// Inline methods
////////////////////////////////////////////////////////////

inline
Printer::Printer()
{ 
}


inline
Printer::~Printer()
{ 
}


inline
void Printer::Log( const char* filename, const char* format, ... )
{
	FILE* f = fopen( filename, "a" );
	
	if ( !f )
	{
		perror( filename );
		return;
	}

	if ( format && format[0] )
	{
		const unsigned int sz = 1024;
		char buf[sz];

		va_list args;
		va_start(args, format);
		int truncated = vsnprintf(buf, sz, format, args);
		buf[sz-1] = 0;
		va_end(args);

		/* More than 1k string was needed, so allocate one. */
		if ( truncated >= (int)sz )
		{
			unsigned int len = truncated + 1; // Doesn't include '\0'
			char* s = new char[ len ];

			va_start( args, format );
			vsnprintf( s, len, format, args );
			s[len-1] = '\0';
			va_end( args );

			fprintf(f, "%s\n", s);

			delete [] s;
		}
		else
		{
			fprintf(f, "%s\n", buf);
		}
	}

	fclose( f );
}


inline
void Printer::Print( const char* format, ... )
{
	if ( format && format[0] )
	{
		const unsigned int sz = 1024;
		char buf[sz];

		va_list args;
		va_start(args, format);
		int truncated = vsnprintf(buf, sz, format, args);
		buf[sz-1] = 0;
		va_end(args);

		/* More than 1k string was needed, so allocate one. */
		if ( truncated >= (int)sz )
		{
			unsigned int len = truncated + 1; // Doesn't include '\0'
			char* s = new char[ len ];

			va_start( args, format );
			vsnprintf( s, len, format, args );
			s[len-1] = '\0';
			va_end( args );

			PrintMessage( s );
			delete [] s;
		}
		else
		{
			PrintMessage( buf );
		}
	}
}


inline
void Printer::PrintErrorArgs( const char* format, va_list* args )
{
	char buffer[1024];
	unsigned int l;

	/* Strip message of an trailing carrage return 
	 *  and print to stdout and the status bar */
	vsnprintf(buffer, 1024, format, *args);
	
	l = strlen(buffer);
  
	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

	fprintf(stderr, "%s\n", buffer);
}

	
inline
void Printer::PrintArgs( const char* format, va_list* args )
{
	char buffer[1024];
	unsigned int l;

	/* Strip message of an trailing carrage return 
	 *  and print to stdout and the status bar */
	vsnprintf(buffer, 1024, format, *args);
	
	l = strlen(buffer);
  
	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

	fprintf(stderr, "%s\n", buffer);
}


inline
void Printer::PrintErrorMessage( const char* s )
{
	fprintf( stderr, "ERROR: %s\n", s );
}


inline
void Printer::PrintMessage( const char* s )
{
	fprintf( stderr, "%s\n", s );
}

} // namespace freyja






#endif // GUARD__FREYJA_PRINTER_H_
