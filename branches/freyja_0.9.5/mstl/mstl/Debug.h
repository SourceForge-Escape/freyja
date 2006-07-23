/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Assert
 * License : No use w/o permission (C) 2001-2006 Mongoose
 * Comments: Lots of nice debugging utils from freyja moved to mstl
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.07.23:
 * Mongoose - Barrowed from my libfreyja, and then made more generic
 *            Recently added software breakpoint asserts
 ================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_DEBUG_H_
#define GUARD__FREYJA_MONGOOSE_DEBUG_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>


namespace mstl {

class Debug
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Debug();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Debug
	 ------------------------------------------------------*/

	virtual ~Debug();
	/*------------------------------------------------------
	 * Pre  : Debug object is allocated
	 * Post : Deconstructs an object of Debug
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static bool Assert(bool expr, const char *format, ...)
	{
		if (expr)
			return false;

		va_list args;
		va_start(args, format);	
		vfprintf(stdout, format, args);
		printf("\n");
		va_end(args);
	
		// Soft breakpoint insertion
#if defined(__x86_64__) || defined(__x86_32__)
		asm(
			 "int $3 \n" 
		);
#elif defined(__powerpc__)
		asm(
			"tw 12,r2,r2 \n"
		);
#else
#   warning "No soft breakpoint can be inserted for this machine language"
#endif

		return true;
	}


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

		fprintf(stdout, "%s\n", buffer);

		va_end(args);
	}


	virtual void PrintArgs(const char *format, va_list *args)
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

		fprintf(stdout, "%s\n", buffer);
	}


	static void PrintError(const char *format, ...)
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


	virtual void PrintErrorArgs(const char *format, va_list *args)
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

} // namespace mstl

#endif // GUARD__FREYJA_MONGOOSE_DEBUG_H_
