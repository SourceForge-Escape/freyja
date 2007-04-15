/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaPrinter
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is for rerouting output and error messages.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAPRINTER - Builds FreyjaPrinter class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.30:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_PRINTER_H_
#define GUARD__FREYJA_PRINTER_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

namespace freyja {

class Printer
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Printer() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaPrinter
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~Printer() {}
	/*------------------------------------------------------
	 * Pre  : FreyjaPrinter object is allocated
	 * Post : Deconstructs an object of FreyjaPrinter
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static void Log(const char *filename, const char *format, ...)
	{
		FILE *f = fopen(filename, "a");
		va_list args;
		char buffer[1024];
		unsigned int l;
		
		if (!f)
		{
			perror( filename );
			return;
		}

		va_start(args, format);

		vsnprintf(buffer, 1024, format, args);
	
		l = strlen(buffer);
  
		if (!l || !buffer[0])
			return;

		if (buffer[l-1] == '\n')
			buffer[l-1] = 0;

		fprintf(f, "%s\n", buffer);

		va_end(args);
	}

	static void Print(const char *format, ...)
	{
		va_list args;
		char buffer[1024];
		unsigned int l;
		
		va_start(args, format);

		vsnprintf(buffer, 1024, format, args);
	
		l = strlen(buffer);
  
		if (!l || !buffer[0])
			return;

		if (buffer[l-1] == '\n')
			buffer[l-1] = 0;

		fprintf(stderr, "%s\n", buffer);

		va_end(args);
	}


	// This only works for glibc!
	virtual void ErrorArgs(const char *format, va_list *args)
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

	
	// This only works for glibc!
	virtual void MessageArgs(const char *format, va_list *args)
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


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};


} // namespace freyja

#endif // GUARD__FREYJA_PRINTER_H_
