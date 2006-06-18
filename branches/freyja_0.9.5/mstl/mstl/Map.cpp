
#include "Map.h"

void _print__unsigned_int_t(unsigned int u)
{
  printf("<%u>\n", u);
}


void _print__float_t(float f)
{
  printf("<%f>\n", f);
}


#ifdef _TEST_MAP
int main(int argc, char *argv[])
{
  Map<unsigned int, float> map;
 

  map.Add(0, 75.01);
  map.Add(1, 75.01);
  map.Add(2, 100.23);
  map.Add(3, 50.023);
  map.Add(4, 50.03);
  map.Add(5, 31.2);
  map.Add(6, 100.3);
  map.Add(7, 31.2);
  map.Add(8, 4.0213);
  map.Print(_print__unsigned_int_t, _print__float_t);

  printf("##Print item 5##\n");
  _print__float_t(map[5]);

  cout << "Remove 31.2" << endl;
  map.Remove(31.2);
  map.Print();

  cout << "##Print item 5## " << map[5] << endl;

#ifdef FIXME
  cout << "Add 312.21" << endl;
  cout << "Add 312.21" << endl;
  map.Add(312.21);
  map.Add(312.21);
  map.Print();

  cout << "Remove 75.01" << endl;
  map.Remove(75.01);
  map.Print();

  cout << "##Print item 5## " << map[5] << endl;

  cout << "Clear()" << endl;
  map.Clear();
  map.Print();

  map.Add(75.01);
  map.Add(75.01);
  map.Add(100.23);


  map.Reset();

  while (map.CurrentExists())
  {
    cout << map.Current() << " ";

    map++;
  }

  cout << endl;


  map.Print();

  cout << "##Print item 5## " << map[5] << endl;
#endif

  return 0;
}
#endif
