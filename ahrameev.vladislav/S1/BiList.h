#ifndef BILIST_H
#define BILIST_H

namespace ahrameev
{

template<class T>
class BiList;

template<class T>
class LIter
{
  typename BiList<T>::Node* node;

public:

  LIter(typename BiList<T>::Node* n = nullptr)
  {
    node = n;
  }

  T& operator*()
  {
    return node->val;
  }

  LIter& operator++()
  {
    node = node->next;
    return *this;
  }

  bool operator!=(const LIter& other) const
  {
    return node != other.node;
  }

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

    Node(const T& value)
    {
      val = value;
      next = nullptr;
      prev = nullptr;
    }
  };

  Node* head;
  Node* tail;

  BiList()
  {
    head = nullptr;
    tail = nullptr;
  }

  BiList(const BiList& other)
  {
    head = nullptr;
    tail = nullptr;

    Node* cur = other.head;

    while (cur != nullptr)
    {
      push_back(cur->val);
      cur = cur->next;
    }
  }

  BiList& operator=(const BiList& other)
  {
    if (this == &other)
      return *this;

    clear();

    Node* cur = other.head;

    while (cur != nullptr)
    {
      push_back(cur->val);
      cur = cur->next;
    }

    return *this;
  }

  bool empty() const
  {
    return head == nullptr;
  }

  void push_back(const T& value)
  {
    Node* node = new Node(value);

    if (empty())
    {
      head = node;
      tail = node;
    }
    else
    {
      tail->next = node;
      node->prev = tail;
      tail = node;
    }
  }

  void push_front(const T& value)
  {
    Node* node = new Node(value);

    if (empty())
    {
      head = node;
      tail = node;
    }
    else
    {
      node->next = head;
      head->prev = node;
      head = node;
    }
  }

  void pop_front()
  {
    if (empty())
      return;

    Node* tmp = head;

    head = head->next;

    if (head != nullptr)
      head->prev = nullptr;
    else
      tail = nullptr;

    delete tmp;
  }

  T& front()
  {
    return head->val;
  }

  void clear()
  {
    while (!empty())
      pop_front();
  }

  ~BiList()
  {
    clear();
  }

  LIter<T> begin()
  {
    return LIter<T>(head);
  }

  LIter<T> end()
  {
    return LIter<T>(nullptr);
  }

};

}

#endif