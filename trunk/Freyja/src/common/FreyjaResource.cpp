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
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2002.01.12:
 * Mongoose - Created
 =================================================================*/

#include <stdlib.h>
#include <stdarg.h> 
#include <string.h>
#include "FreyjaResource.h"
#include "freyja_events.h"





////////////////////////////////////////////////////////////


FreyjaResource::FreyjaResource()
{
}


FreyjaResource::~FreyjaResource()
{
}


void FreyjaResource::RegisterCPPInt(char *symbol, int value)
{
	if (RegisterSymbol(symbol, INT, &value))
	{
		printf("ERROR couldn't register new resource symbol %s\n",
				 symbol);
	}
}

