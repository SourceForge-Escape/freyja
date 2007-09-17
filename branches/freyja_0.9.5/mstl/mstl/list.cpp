/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include "list.h"

template<>
int mstl::list<int>::mDefault = 0;


bool compare_array_and_list(unsigned int size, int array[], mstl::list<int>& lst)
{
	mstl::list<int>::iterator it = lst.begin();
	for (unsigned int i = 0; i < size; ++i)
	{
		if ( *it != array[i] )
		{
			printf("-- *it: %i != %i @ %i\n", *it, array[i], i);   
			return false;
		}

		it++;
	}

	if ( array[0] != lst.front() )
	{
		printf("-- front(): %i != %i\n", array[0], lst.front()); 
		return false;
	}

	if ( array[size-1] != lst.back() )
	{
		printf("-- back(): %i != %i\n", array[size-1], lst.back()); 
		return false;
	}

	return true;
}


bool list_test1()
{
  printf("\n-- Test #1\n");

  int array[] = { 213, 32, 27, 82, 62, 92, 91, 43, 6, 70 };
  //int expected = {}


  mstl::list<int> lst;

  for (unsigned int i = 0; i < 10; ++i)
  {
    lst.push_back( array[i] );
  }

  // Test storage from push_back().
  if ( !compare_array_and_list(10, array, lst) )
  {
	  printf("%s:%i\n", __FILE__, __LINE__);
	  return false;
  }

  // Test storage with pop_back().
  mstl::list<int>::iterator it = lst.begin();
  while ( it != lst.end() && *it != 91 )
  {
    it++;
    lst.pop_front();
  }

  it = lst.begin();
  for (unsigned int i = 6; i < 10; ++i)
  {
    if ( *it != array[i] )
    {
      printf("-- %i != %i @ %i\n", *it, array[i], i);   
      return false;
    }

    it++;
  }
 
  printf("passed\n");

  return true;
}


bool list_test2()
{
  printf("\n-- Test #2\n");

  int array[] = { 213, 32, 27, 82, 62, 92, 91, 43, 6, 70 };
  mstl::list<int> lst;

  for (int i = 9; i >= 0; --i)
  {
    lst.push_front( array[i] );
  }

  // Test storage from push_forward().
  if ( !compare_array_and_list(10, array, lst) )
  {
	  printf("%s:%i\n", __FILE__, __LINE__);
	  return false;
  }

  while ( lst.back() != 62 )
  {
    lst.pop_back();
  }

  if ( !compare_array_and_list(5, array, lst) )
  {
	  printf("%s:%i\n", __FILE__, __LINE__);
	  return false;
  }

  printf("passed\n");
  return true;
}


int main(int argc, char *argv[])
{
  bool ok = true;

  ok = list_test1() && ok;
  ok = list_test2() && ok;
 
  return ok ? 0 : -1;
}
