/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaResource
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: Resource system for Freyja
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2002.01.12:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJARESOURCE_H_
#define GUARD__FREYJA_MONGOOSE_FREYJARESOURCE_H_

#include "Resource.h"


char *freyja_rc_map(char *s);
/*------------------------------------------------------
 * Pre  : s is a valid filename or path/filename 
 *        under resouce dir
 *
 * Post : Returns full path filename as string or 
 *        NULL on error
 *
 *-- History ------------------------------------------
 *
 * 2002.01.12: 
 * Mongoose - Created
 ------------------------------------------------------*/


class FreyjaResource : public Resource
{
 public:

	FreyjaResource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaResource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.12: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaResource();
	/*------------------------------------------------------
	 * Pre  : FreyjaResource object is allocated
	 * Post : Deconstructs an object of FreyjaResource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.12: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	void RegisterCPPInt(char *symbol, int value);
	/*------------------------------------------------------
	 * Pre  : C++ Symbol is valid and matches Value
	 * Post : Binds C++ to matching symbol name in resource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

 private:

	/* */
};

#endif
