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

void testRotations()
{
  ahrameev::BSTree< int, std::string > tree;
  tree.push(2, "b");
  tree.push(1, "a");
  tree.push(3, "c");
  auto it = tree.begin();
  ++it;
  assert(it->first == 2);
  tree.rotateLeft(it);
  tree.push(4, "d");
  tree.push(5, "e");
  std::cout << "Rotations: PASSED" << '\n';
}

void testCopyConstructor()
{
  ahrameev::BSTree< int, std::string > tree1;
  tree1.push(1, "one");
  tree1.push(2, "two");
  tree1.push(3, "three");
  ahrameev::BSTree< int, std::string > tree2(tree1);
  assert(tree2.size() == 3);
  assert(tree2.get(1) == "one");
  assert(tree2.get(2) == "two");
  assert(tree2.get(3) == "three");
  tree2.push(4, "four");
  assert(tree1.size() == 3);
  assert(tree2.size() == 4);
  std::cout << "Copy constructor: PASSED" << '\n';
}

void testAssignmentOperator()
{
  ahrameev::BSTree< int, std::string > tree1;
  tree1.push(10, "ten");
  tree1.push(20, "twenty");
  ahrameev::BSTree< int, std::string > tree2;
  tree2 = tree1;
  assert(tree2.size() == 2);
  assert(tree2.get(10) == "ten");
  assert(tree2.get(20) == "twenty");
  std::cout << "Assignment operator: PASSED" << '\n';
}

void testDrop()
{
  ahrameev::BSTree< int, std::string > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  assert(tree.drop(3) == "three");
  assert(tree.size() == 2);
  try {
    tree.get(3);
    assert(false);
  } catch (...) {
  }
  std::cout << "Drop: PASSED" << '\n';
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
  testRotations();
  testCopyConstructor();
  testAssignmentOperator();
  testDrop();
  testHeight();
  testEmptyTree();
  std::cout << "All tests PASSED!" << '\n';
}