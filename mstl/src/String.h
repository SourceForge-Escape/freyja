/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : MSTL
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : String
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_STRING - Builds String class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2003.07.05:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__MSTL_MONGOOSE_STRING_H_
#define GUARD__MSTL_MONGOOSE_STRING_H_


class String
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	String()
	{
		mString = 0x0;
		mLength = 0;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of String
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.07.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~String()
	{
		if (mString)
		{
			delete [] mString;
		}
	}
	/*------------------------------------------------------
	 * Pre  : String object is allocated
	 * Post : Deconstructs an object of String
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.07.05: 
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


	char *mString;             /*  */

	unsigned int mLength;      /*  */
};

#endif
