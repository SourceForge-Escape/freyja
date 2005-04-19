/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libfreyja
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
 * 2004.12.08:
 * Mongoose - Created, using code from Freyja ( Chimera build )
 ==========================================================================*/


#ifndef GUARD__LIBFREYJA_MONGOOSE_FREYJAUTIL_H_
#define GUARD__LIBFREYJA_MONGOOSE_FREYJAUTIL_H_

#include <freyja8/EggPlugin.h>
#include <freyja8/Egg.h>


class FreyjaEgg : public Egg
{
public:
	FreyjaEgg::FreyjaEgg() : Egg()
	{
	}


	virtual void print(char *format, ...)
	{	
		va_list args;

		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}

	virtual void printError(char *format, ...)
	{	
		va_list args;

		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
};


class FreyjaEggPlugin : public EggPlugin
{
public:
	FreyjaEggPlugin(FreyjaEgg *egg, char *plugindir) : 
		EggPlugin(egg, plugindir)
	{
	}

	
	virtual void eggPrintError(char *format, va_list *args)
	{
		vprintf(format, *args);
	}

	
	virtual void eggPrintMessage(char *format, va_list *args)
	{
		vprintf(format, *args);
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

	FreyjaEgg *mEgg;                   /* The data model */
	
	FreyjaEggPlugin *mEggPlugin;       /* Standard interface to data model */
};

#endif
