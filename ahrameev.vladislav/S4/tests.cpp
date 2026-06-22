#include "bst.hpp"

#include <cassert>
#include <iostream>
#include <string>

void testBasicOperations()
{
  ahrameev::BSTree< int, std::string > tree;
  assert(tree.empty());
  assert(tree.size() == 0);
  assert(tree.height() == 0);
  tree.push(5, "five");
  assert(!tree.empty());
  assert(tree.size() == 1);
  assert(tree.get(5) == "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(1, "one");
  tree.push(4, "four");
  assert(tree.size() == 5);
  assert(tree.get(3) == "three");
  assert(tree.get(7) == "seven");
  assert(tree.get(1) == "one");
  assert(tree.get(4) == "four");
  std::cout << "Basic operations: PASSED" << '\n';
}

void testIterator()
{
  ahrameev::BSTree< int, std::string > tree;
  tree.push(2, "b");
  tree.push(1, "a");
  tree.push(3, "c");
  auto it = tree.begin();
  assert(it->first == 1);
  assert(it->second == "a");
  ++it;
  assert(it->first == 2);
  assert(it->second == "b");
  ++it;
  assert(it->first == 3);
  assert(it->second == "c");
  ++it;
  assert(it == tree.end());
  std::cout << "Iterators: PASSED" << '\n';
}

void testHeight()
{
  ahrameev::BSTree< int, std::string > tree;
  assert(tree.height() == 0);
  tree.push(5, "five");
  assert(tree.height() == 1);
  tree.push(3, "three");
  tree.push(7, "seven");
  assert(tree.height() == 2);
  tree.push(1, "one");
  assert(tree.height() == 3);
  std::cout << "Height: PASSED" << '\n';
}

void testEmptyTree()
{
  ahrameev::BSTree< int, std::string > tree;
  assert(tree.empty());
  assert(tree.begin() == tree.end());
  std::cout << "Empty tree: PASSED" << '\n';
}

int main()
{
  testBasicOperations();
  testIterator();
  testHeight();
  testEmptyTree();
  std::cout << "All tests PASSED!" << '\n';
}