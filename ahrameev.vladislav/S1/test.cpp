#include "BiList.h"
#include <cassert>
#include <iostream>

using namespace ahrameev;

int main()
{
  BiList<int> list1;
  assert(list1.empty());

  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  assert(!list1.empty());
  assert(list1.front() == 1);

  list1.push_front(0);
  assert(list1.front() == 0);

  list1.pop_front();
  assert(list1.front() == 1);

  int expected = 1;
  for (auto it = list1.begin(); it != list1.end(); ++it)
  {
    assert(*it == expected++);
  }

  BiList<int> list2 = list1;
  assert(list2.front() == 1);

  list1.clear();
  assert(list1.empty());

  std::cout << "All tests passed" << "\n";
  return 0;
}
