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


#ifndef GUARD__MSTL_MONGOOSE_ACTIONMANAGER_H_
#define GUARD__MSTL_MONGOOSE_ACTIONMANAGER_H_


#include <mstl/Action.h>
#include <mstl/Stack.h>


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

	virtual bool Serialize(SystemIO::FileWriter &w);
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

	void PopScope();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : For multiple actions mostly
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Push(Action *action);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Redo();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handles pop/delete as needed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::Reader &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool Undo(Action *action);
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

	Stack<Action *> mActions;    	/* */
};

#endif // GUARD__MSTL_MONGOOSE_ACTIONMANAGER_H_
