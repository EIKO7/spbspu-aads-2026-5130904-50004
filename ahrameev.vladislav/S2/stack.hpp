#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>

namespace akhrameev {

  template <typename T>
  class Stack {
    struct Node {
      T data;
      Node* next;
      Node(T val, Node* n) : data(val), next(n) {}
    };
    Node* top_ptr = nullptr;

  public:
    Stack() = default;
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    ~Stack() {
      while (top_ptr) {
        Node* temp = top_ptr;
        top_ptr = top_ptr->next;
        delete temp;
      }
    }

    void push(T rhs) {
      Node* new_node = new Node(rhs, top_ptr);
      top_ptr = new_node;
    }

    T drop() {
      if (!top_ptr) throw std::runtime_error("Стек пуст");
      T val = top_ptr->data;
      Node* temp = top_ptr;
      top_ptr = top_ptr->next;
      delete temp;
      return val;
    }

    bool empty() const { return top_ptr == nullptr; }
    const T& top() const {
      if (!top_ptr) throw std::runtime_error("Стек пуст");
      return top_ptr->data;
    }
  };

} 
#endif 