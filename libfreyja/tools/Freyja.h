/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja ( Umbra )
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaUtil
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This class provides a command line interface translation/test tool.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAUTIL - Builds FreyjaUtil class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.12.14:
 * Mongoose - Ported to Freyja ( Umbra )
 *
 * 2004.12.08:
 * Mongoose - Created, using code from Freyja ( Chimera build )
 ==========================================================================*/


#ifndef GUARD__LIBFREYJA_MONGOOSE_FREYJAUTIL_H_
#define GUARD__LIBFREYJA_MONGOOSE_FREYJAUTIL_H_

#include <freyja/Printer.h>
#include <freyja/Plugin.h>
#include <freyja/freyja.h>


class FreyjaUtilPrinter : public Printer
{
 public:

	virtual void errorArgs(char *format, va_list *args)
	{
		vfprintf(stderr, format, *args);
		fprintf(stderr, "\n");
	}


	virtual void messageArgs(char *format, va_list *args)
	{
		vprintf(format, *args);
		printf("\n");
	}
};


class FreyjaUtil
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaUtil();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaUtil
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaUtil();
	/*------------------------------------------------------
	 * Pre  : FreyjaUtil object is allocated
	 * Post : Deconstructs an object of FreyjaUtil
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	int exportModel(const char *filename, const char *type);
	/*------------------------------------------------------
	 * Pre  : type is the string name of the model type to export
	 * Post : exports a model if possible of type
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	int importModel(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Imports a model
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.12.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	FreyjaUtilPrinter mPrinter;
};

#endif
