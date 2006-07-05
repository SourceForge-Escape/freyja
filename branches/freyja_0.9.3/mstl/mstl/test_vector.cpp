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

#include "Vector.h"

int testIterator()
{
	Vector<int> numbers;
	VectorIterator<int> iter = VectorIterator<int>(&numbers);
	int num[8];
	int fill = 32;
	unsigned int i;



   num[0] = 14;
   num[1] = 311;   

   for (i = 2; i < 8; ++i)
   {
		num[i] = fill;
   }

	///////////////////////////////////////////////

   for (i = 0; i < 8; ++i)
   {
		numbers.pushBack(num[i]);
   }
	
	///////////////////////////////////////////////


   for (iter.start(); iter.forward(); iter.next())
   {
		printf("numbers[%i] = %3i   |   iter[%i] = %3i\n",  
				 iter.index(), num[iter.index()],
				 iter.index(), iter.current());

		if (num[iter.index()] != iter.current())
			return -10;
   }

   printf("\n");

   for (iter.start(); iter.forward(); iter.next())
   {
		printf("numbers[%i] = %3i   |   iter[%i] = %3i\n",  
				 iter.index(), num[iter.index()],
				 iter.index(), iter.current());
		
		if (num[iter.index()] != iter.current())
			return -11;
   }

   printf("\n");

   for (iter.finish(); iter.backward(); iter.prev())
   {
		printf("numbers[%i] = %3i   |   iter[%i] = %3i\n",  
				 iter.index(), num[iter.index()],
				 iter.index(), iter.current());
		
		if (num[iter.index()] != iter.current())
			return -12;
   }
   

	return 0;
}


int main()
{
	if (testIterator() < 0)
	{
		printf("VECTOR TEST [FAILED]\n");
	}
	else
	{
		printf("VECTOR TEST [PASSED]\n");
	}

   return 0;
}
