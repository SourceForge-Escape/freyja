/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Vector
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: mtk template vector
 *
 *           UINT_MAX is an error condition, used in place of -1
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2002.08.31:
 * Mongoose - Created
 ================================================================*/

#include <mstl/Vector.h>

using namespace mstl;

int main()
{
	int test = VectorUnitTest::Test();

	if (test)
	{
		printf("VECTOR TEST %i [FAILED]\n", test);
	}
	else
	{
		printf("VECTOR TEST [PASSED]\n");
	}

   return 0;
}
