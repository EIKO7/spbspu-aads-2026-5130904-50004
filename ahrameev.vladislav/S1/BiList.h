#ifndef BILIST_H
#define BILIST_H

#include <cstddef>
#include <utility>

namespace ahrameev
{

template<class T>
class BiList;

template<class T>
class LIter
{
  typename BiList<T>::Node* node;
public:
  LIter(typename BiList<T>::Node* n = nullptr) : node(n) {}

  T& operator*() { return node->val; }
  T* operator->() { return &(node->val); }

  LIter& operator++() { node = node->next; return *this; }
  LIter operator++(int) { LIter tmp = *this; node = node->next; return tmp; }
  LIter& operator--() { node = node->prev; return *this; }
  LIter operator--(int) { LIter tmp = *this; node = node->prev; return tmp; }

  bool operator==(const LIter& other) const { return node == other.node; }
  bool operator!=(const LIter& other) const { return node != other.node; }

  friend class BiList<T>;
};

template<class T>
class LCIter
{
  const typename BiList<T>::Node* node;
public:
  LCIter(const typename BiList<T>::Node* n = nullptr) : node(n) {}
  LCIter(const LIter<T>& other) : node(other.node) {}

  const T& operator*() const { return node->val; }
  const T* operator->() const { return &(node->val); }

  LCIter& operator++() { node = node->next; return *this; }
  LCIter operator++(int) { LCIter tmp = *this; node = node->next; return tmp; }
  LCIter& operator--() { node = node->prev; return *this; }
  LCIter operator--(int) { LCIter tmp = *this; node = node->prev; return tmp; }

  bool operator==(const LCIter& other) const { return node == other.node; }
  bool operator!=(const LCIter& other) const { return node != other.node; }

  friend class BiList<T>;
};

template<class T>
class BiList
{
public:
  struct Node
  {
    T val;
    Node* next;
    Node* prev;

    Node(const T& value) : val(value), next(nullptr), prev(nullptr) {}
  };

private:
  Node* head;
  Node* tail;
  std::size_t count;

public:
  BiList() : head(nullptr), tail(nullptr), count(0) {}

  BiList(const BiList& other) : head(nullptr), tail(nullptr), count(0)
  {
    Node* cur = other.head;
    while (cur != nullptr)
    {
      push_back(cur->val);
      cur = cur->next;
    }
  }

  BiList& operator=(const BiList& other)
  {
    if (this == &other) return *this;
    clear();
    Node* cur = other.head;
    while (cur != nullptr)
    {
      push_back(cur->val);
      cur = cur->next;
    }
    return *this;
  }

  BiList(BiList&& other) noexcept : head(other.head), tail(other.tail), count(other.count)
  {
    other.head = nullptr;
    other.tail = nullptr;
    other.count = 0;
  }

  BiList& operator=(BiList&& other) noexcept
  {
    if (this == &other) return *this;
    clear();
    head = other.head;
    tail = other.tail;
    count = other.count;

    other.head = nullptr;
    other.tail = nullptr;
    other.count = 0;
    return *this;
  }

  bool empty() const { return head == nullptr; }
  std::size_t size() const { return count; }

  void push_back(const T& value)
  {
    Node* node = new Node(value);
    if (empty()) { head = node; tail = node; }
    else { tail->next = node; node->prev = tail; tail = node; }
    ++count;
  }

  void push_front(const T& value)
  {
    Node* node = new Node(value);
    if (empty()) { head = node; tail = node; }
    else { node->next = head; head->prev = node; head = node; }
    ++count;
  }

  void pop_front()
  {
    if (empty()) return;
    Node* tmp = head;
    head = head->next;
    if (head != nullptr) head->prev = nullptr;
    else tail = nullptr;
    delete tmp;
    --count;
  }

  T& back() { return tail->val; }
  const T& back() const { return tail->val; }
  T& front() { return head->val; }
  const T& front() const { return head->val; }

  void clear() { while (!empty()) pop_front(); }
  ~BiList() { clear(); }

  LIter<T> begin() { return LIter<T>(head); }
  LIter<T> end() { return LIter<T>(nullptr); }

  LCIter<T> begin() const { return LCIter<T>(head); }
  LCIter<T> end() const { return LCIter<T>(nullptr); }
  LCIter<T> cbegin() const { return LCIter<T>(head); }
  LCIter<T> cend() const { return LCIter<T>(nullptr); }
};

}

#endif
