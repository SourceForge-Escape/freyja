/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Action
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the 'event reel atom' for freyja undo/redo/macro usage
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_ACTION - Builds Action class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.05.06:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_ACTION_H_
#define GUARD__FREYJA_MONGOOSE_ACTION_H_

#include "FreyjaFileReader.h"
#include "FreyjaFileWriter.h"


class Action
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Action();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Action
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~Action();
	/*------------------------------------------------------
	 * Pre  : Action object is allocated
	 * Post : Deconstructs an object of Action
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	virtual bool Serialize(FreyjaFileWriter &w) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	virtual void Redo() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool Serialize(FreyjaFileReader &r) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

 	virtual void Undo() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/



 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};

#endif
