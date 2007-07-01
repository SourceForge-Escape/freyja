

#include "list.h"

template<>
int mstl::list<int>::mDefault = 0;


void test_list0()
{
  mstl::list<int> lst;

  for (int i = 0; i < 10; ++i)
    lst.push_back(i);

  mstl::list<int>::iterator it = lst.begin();

  printf("-- %i\n\n", *it );

  for (int i = 0; i < 10; ++i)
  {
    printf("-- %i\n", *it );

    it++;
  }


  printf("\nwhile\n" );
  it = lst.begin();
  while ( it != lst.end() )
  {
    printf("-- %i\n", *it );
    it++;
  }


  printf("\npop_front, for\n" );
  lst.pop_front();
  for (it = lst.begin(); it != lst.end(); ++it)
  {
    printf("-- %i\n", *it );
  }
  
}



int main(int argc, char *argv[])
{
  test_list0();
  return 0;
}
