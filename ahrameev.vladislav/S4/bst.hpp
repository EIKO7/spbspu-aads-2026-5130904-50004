#ifndef BST_HPP
#define BST_HPP

#include <cstddef>
#include <utility>

namespace ahrameev
{

template< class Key, class Value >
struct BSTNode
{
  std::pair< Key, Value > data;
  BSTNode* left;
  BSTNode* right;
  BSTNode* parent;

  BSTNode()
    : data()
    , left(nullptr)
    , right(nullptr)
    , parent(nullptr)
  {}

  BSTNode(const Key& k, const Value& v, BSTNode* p)
    : data(k, v)
    , left(nullptr)
    , right(nullptr)
    , parent(p)
  {}
};

template< class Key, class Value >
class BSTIterator
{
  template< class K, class V, class C >
  friend class BSTree;

  using Node = BSTNode< Key, Value >;

  Node* current_;
  Node* sentinel_;

public:
  BSTIterator()
    : current_(nullptr)
    , sentinel_(nullptr)
  {}

  BSTIterator(Node* node, Node* sentinel)
    : current_(node)
    , sentinel_(sentinel)
  {}

  std::pair< Key, Value >& operator*() const
  {
    return current_->data;
  }

  std::pair< Key, Value >* operator->() const
  {
    return &(current_->data);
  }

  BSTIterator& operator++()
  {
    if (current_->right != sentinel_) {
      current_ = current_->right;
      while (current_->left != sentinel_) {
        current_ = current_->left;
      }
    } else {
      Node* p = current_->parent;
      while (p != sentinel_ && current_ == p->right) {
        current_ = p;
        p = p->parent;
      }
      if (p != sentinel_ || current_ == sentinel_) {
        current_ = p;
      }
    }
    return *this;
  }

  BSTIterator operator++(int)
  {
    BSTIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const BSTIterator& other) const
  {
    return current_ == other.current_;
  }

  bool operator!=(const BSTIterator& other) const
  {
    return current_ != other.current_;
  }
};

} 

#endif 