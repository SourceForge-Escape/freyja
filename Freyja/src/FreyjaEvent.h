/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaEvent
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the plugin event class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAEVENT - Builds FreyjaEvent class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.02:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAEVENT_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAEVENT_H_


#include <mstl/Vector.h>
#include <mgtk/Resource.h>


class FreyjaEvent
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaEvent(Resource *rcSys, char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaEvent
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~FreyjaEvent();
	/*------------------------------------------------------
	 * Pre  : FreyjaEvent object is allocated
	 * Post : Deconstructs an object of FreyjaEvent
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void setName(char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets name and exports it to resource system
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void action() = 0;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	Resource *mResource;            /* Resource system pointer */

	unsigned int mId;               /* Numeric id of event */

	char *mName;                    /* Symbolic name of event */

	static unsigned int mNextId;    /* Simple UID generator scheme */

	static Vector<FreyjaEvent*> mEventStore; /* Event store for control use */
};

#endif
