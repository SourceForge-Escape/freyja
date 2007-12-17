/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : MLisp
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : MLispWX
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_MLISPWX - Builds MLispWX class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.04.26:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__MLISP_MONGOOSE_MLISPWX_H_
#define GUARD__MLISP_MONGOOSE_MLISPWX_H_


#include "MLisp.h"


class MLispWX
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	MLispWX(MLisp *mlisp);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of MLispWX
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.04.26: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~MLispWX();
	/*------------------------------------------------------
	 * Pre  : MLispWX object is allocated
	 * Post : Deconstructs an object of MLispWX
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.04.26: 
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


	MLisp *mRC;   /* MLisp resource */
};

#endif
