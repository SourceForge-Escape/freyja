/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : UTPakBrowser
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: Unreal package browser
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_UTPAKBROWSER - Builds UTPakBrowser class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.07.30:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_UTPAKBROWSER_H_
#define GUARD__FREYJA_MONGOOSE_UTPAKBROWSER_H_

#include "UTPackage.h"

class UTPakBrowser
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	UTPakBrowser();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of UTPakBrowser
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.07.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~UTPakBrowser();
	/*------------------------------------------------------
	 * Pre  : UTPakBrowser object is allocated
	 * Post : Deconstructs an object of UTPakBrowser
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.07.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



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


	UTPackage pak; /* */
};

#endif
