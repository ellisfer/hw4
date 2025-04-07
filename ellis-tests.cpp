
#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

#include "avlbst.h"
#include "print_bst.h"


#include <initializer_list>
#include <set>
using namespace std;

// extensive combined insert-remove test
// author credit: Shreya Havaldar
int main()
{
	AVLTree<uint16_t, uint16_t> testTree;

	testTree.insert(std::make_pair(5, 95));
  testTree.insert(std::make_pair(3, 95));
  testTree.insert(std::make_pair(7, 95));
  testTree.insert(std::make_pair(4, 95));
  testTree.insert(std::make_pair(6, 95));
  testTree.insert(std::make_pair(9, 95));
  testTree.insert(std::make_pair(10, 95));
  
  testTree.print();

testTree.clear();
  cout << " made it to the end! " << endl;

}
