#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>

namespace akhrameev {

  template <typename T>
  class Queue {
    struct Node {
      T data;
      Node* next;
      Node(T val, Node* n) : data(val), next(n) {}
    };
    Node* head = nullptr;
    Node* tail = nullptr;

  public:
    Queue() = default;
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    ~Queue() {
      while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
      }
    }

    void push(T rhs) {
      Node* new_node = new Node(rhs, nullptr);
      if (tail) tail->next = new_node;
      else head = new_node;
      tail = new_node;
    }

    T drop() {
      if (!head) throw std::runtime_error("Очередь пуста");
      T val = head->data;
      Node* temp = head;
      head = head->next;
      if (!head) tail = nullptr;
      delete temp;
      return val;
    }

    bool empty() const { return head == nullptr; }
  };

} 
#endif 