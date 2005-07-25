/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : ActionManager
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: Action reel manager
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_ACTIONMANAGER - Builds ActionManager class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.05.06:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_ACTIONMANAGER_H_
#define GUARD__FREYJA_MONGOOSE_ACTIONMANAGER_H_


#include "Action.h"

namespace freyja {

class ActionManager
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	ActionManager();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of ActionManager
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~ActionManager();
	/*------------------------------------------------------
	 * Pre  : ActionManager object is allocated
	 * Post : Deconstructs an object of ActionManager
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	virtual bool serialize(FreyjaFileWriter &w) { return false; } // FIXME
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

	void popScope();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : For multiple actions mostly
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void pushScope();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : For multiple actions mostly
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void redo();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handles pop/delete as needed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool serialize(FreyjaFileReader &r) { return false; } // FIXME
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int undo(Action *action);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handles push/append as needed
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

}

#endif
