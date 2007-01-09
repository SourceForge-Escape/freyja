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
#include <mstl/stack.h>
#include <mstl/SystemIO.h>


class ActionManager
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	ActionManager() : mActions() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of ActionManager
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~ActionManager() { while ( mActions.pop()) ; }
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

	virtual bool Serialize(mstl::SystemIO::FileWriter &w) 
	{
		mstl::stack<mstl::Action *> *r = mActions.get_reverse();
		mstl::Action *a;

		while ( (a = r->pop()) )
		{
			if (a)
				a->Serialize(w);
		}

		delete r;

		return true; 
	}
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

	virtual mstl::Action *Pop() { return mActions.pop(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void Push(mstl::Action *action) { if (action) mActions.push(action); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Redo() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handles pop/delete as needed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool Serialize(mstl::SystemIO::FileReader &r) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool Undo() 
	{ 
		mstl::Action *a = Pop(); 
		if (!a) return false; 
		bool b = a->Undo(); 
		delete a; 
		return b; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Handles push/append as needed ( deletetion )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.06:
	 * Mongoose - Created
	 ------------------------------------------------------*/


 protected:

	mstl::stack<mstl::Action *> mActions;    	/* 'Undo' stack */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};

#endif // GUARD__MSTL_MONGOOSE_ACTIONMANAGER_H_
