#include "BiList.h"
#include <cassert>

using namespace ahrameev;

void test_empty()
{
  BiList<int> list;
  assert(list.empty());
}

void test_push_back()
{
  BiList<int> list;
  list.push_back(1);
  assert(list.front() == 1);
  assert(list.back() == 1);

  list.push_back(2);
  assert(list.back() == 2);
}

void test_push_front()
{
  BiList<int> list;
  list.push_front(2);
  list.push_front(1);
  assert(list.front() == 1);
  assert(list.back() == 2);
}

void test_pop_front()
{
  BiList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.pop_front();
  assert(list.front() == 2);

  list.pop_front();
  assert(list.front() == 3);

  list.pop_front();
  assert(list.empty());
}

void test_iterator()
{
  BiList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  int expected = 1;
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    assert(*it == expected++);
  }
}

void test_copy()
{
  BiList<int> list1;
  list1.push_back(1);
  list1.push_back(2);

  BiList<int> list2 = list1;
  assert(list2.front() == 1);
  assert(list2.back() == 2);
}

void test_clear()
{
  BiList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.clear();
  assert(list.empty());
}
