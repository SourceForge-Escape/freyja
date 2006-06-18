#include <time.h>
#include <stdlib.h>
#include "Tree.h"

#define MIN 0.0
#define MAX 128.0


#ifdef __TEST__
int rand_int(float min, float max)
{
  return (int)(min+(int)(max*rand()/(RAND_MAX+1.0)));
}


void build_tree(Tree<int, int> *tree, int key_i, int keys)
{
  int i, num;


  if (!tree)
    return;

  for (i = key_i; i < keys; ++i)
  {
    num = rand_int(MIN, MAX);
    tree->Insert(i, num);
  }
}

void print_valid(Tree<int, int> *tree)
{
  bool valid;

  if (!tree)
    return;

  valid = tree->IsValidRedBlackTree();

  tree->Print();

  if (valid)
    return;

  cout << "***************ERROR*******************" << endl;
  cout << "*** Tree isn't valid red black tree ***" << endl;

  tree->PrintAsTree();
  cout << "***************************************" << endl;
}


int main(int argc, char *argv[])
{
  Tree<int, int> tree;
  unsigned int num, key, key2, root;
  bool error;


  srand(time(NULL));

  build_tree(&tree, 0, 20);

  key = rand_int(0, 20);
  key2 = rand_int(0, 20);
  num = tree.SearchByKey(key2, &error);

  print_valid(&tree);

  cout << endl << "[Remove by key " << key << "]" << endl;

  if (tree.RemoveByKey(key))
  {
    cout << "ERROR removing by key " << key << endl;
  }

  print_valid(&tree);

  ////////////////////////////////////////////////////////////////

  cout << endl << "[Remove ( key " << key2 << ") by data " << num << "]"
       << endl;

  if (tree.RemoveByData(num))
  {
    cout << "ERROR removing by data " << num << endl;
  }

  print_valid(&tree);

  ////////////////////////////////////////////////////////////////

  root = tree.Root();
  cout << endl << "[Remove root " << root << " by key " << root << "]" << endl;

  if (tree.RemoveByKey(root))
  {
    cout << "ERROR removing by key " << root << endl;
  }

  print_valid(&tree);

  ////////////////////////////////////////////////////////////////

  cout << endl << "[Insert]" << endl;
  build_tree(&tree, 20, 23);

  print_valid(&tree);

  ////////////////////////////////////////////////////////////////

  root = tree.Root();
  cout << endl << "[Remove root " << root << " by key " << root << "]" << endl;

  if (tree.RemoveByKey(root))
  {
    cout << "ERROR removing by key " << root << endl;
  }

  print_valid(&tree);

  ////////////////////////////////////////////////////////////////

  cout << endl << "[Clear tree]" << endl;
  tree.Clear();

  print_valid(&tree);

  ////////////////////////////////////////////////////////////////

  cout << endl << "[Build new tree]" << endl;
  build_tree(&tree, 0, 5);

  print_valid(&tree);

  return 0;
}
#endif
