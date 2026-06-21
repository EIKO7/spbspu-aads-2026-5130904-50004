#ifndef BST_HPP
#define BST_HPP

#include <cstddef>
#include <utility>
#include <functional>

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

template< class Key, class Value >
class BSTConstIterator
{
  template< class K, class V, class C >
  friend class BSTree;

  using Node = BSTNode< Key, Value >;

  const Node* current_;
  const Node* sentinel_;

public:
  BSTConstIterator()
    : current_(nullptr)
    , sentinel_(nullptr)
  {}

  BSTConstIterator(const Node* node, const Node* sentinel)
    : current_(node)
    , sentinel_(sentinel)
  {}

  BSTConstIterator(const BSTIterator< Key, Value >& it)
    : current_(it.current_)
    , sentinel_(it.sentinel_)
  {}

  const std::pair< Key, Value >& operator*() const
  {
    return current_->data;
  }

  const std::pair< Key, Value >* operator->() const
  {
    return &(current_->data);
  }

  BSTConstIterator& operator++()
  {
    if (current_->right != sentinel_) {
      current_ = current_->right;
      while (current_->left != sentinel_) {
        current_ = current_->left;
      }
    } else {
      const Node* p = current_->parent;
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

  BSTConstIterator operator++(int)
  {
    BSTConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const BSTConstIterator& other) const
  {
    return current_ == other.current_;
  }

  bool operator!=(const BSTConstIterator& other) const
  {
    return current_ != other.current_;
  }
};

template< class Key, class Value, class Compare = std::less< Key > >
class BSTree
{
  using Node = BSTNode< Key, Value >;

  Node* root_;
  Node* sentinel_;
  size_t size_;
  Compare comp_;

  void clear(Node* node)
  {
    if (node == sentinel_) {
      return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
  }

  Node* copy(Node* node, Node* parent)
  {
    if (node == sentinel_) {
      return sentinel_;
    }
    Node* newNode = new Node(node->data.first, node->data.second, parent);
    newNode->left = copy(node->left, newNode);
    newNode->right = copy(node->right, newNode);
    return newNode;
  }

public:
  using iterator = BSTIterator< Key, Value >;
  using const_iterator = BSTConstIterator< Key, Value >;

  BSTree()
    : root_(nullptr)
    , sentinel_(nullptr)
    , size_(0)
    , comp_()
  {
    sentinel_ = new Node();
    sentinel_->left = sentinel_;
    sentinel_->right = sentinel_;
    sentinel_->parent = sentinel_;
    root_ = sentinel_;
  }

  BSTree(const BSTree& other)
    : root_(nullptr)
    , sentinel_(nullptr)
    , size_(0)
    , comp_(other.comp_)
  {
    sentinel_ = new Node();
    sentinel_->left = sentinel_;
    sentinel_->right = sentinel_;
    sentinel_->parent = sentinel_;
    if (other.root_ != other.sentinel_) {
      root_ = copy(other.root_, sentinel_);
      size_ = other.size_;
    } else {
      root_ = sentinel_;
    }
  }

  BSTree& operator=(const BSTree& other)
  {
    if (this != &other) {
      clear(root_);
      delete sentinel_;
      sentinel_ = new Node();
      sentinel_->left = sentinel_;
      sentinel_->right = sentinel_;
      sentinel_->parent = sentinel_;
      comp_ = other.comp_;
      if (other.root_ != other.sentinel_) {
        root_ = copy(other.root_, sentinel_);
        size_ = other.size_;
      } else {
        root_ = sentinel_;
        size_ = 0;
      }
    }
    return *this;
  }

  ~BSTree()
  {
    clear(root_);
    delete sentinel_;
  }

  iterator begin()
  {
    if (root_ == sentinel_) {
      return end();
    }
    Node* node = root_;
    while (node->left != sentinel_) {
      node = node->left;
    }
    return iterator(node, sentinel_);
  }

  iterator end()
  {
    return iterator(sentinel_, sentinel_);
  }

  const_iterator begin() const
  {
    if (root_ == sentinel_) {
      return end();
    }
    const Node* node = root_;
    while (node->left != sentinel_) {
      node = node->left;
    }
    return const_iterator(node, sentinel_);
  }

  const_iterator end() const
  {
    return const_iterator(sentinel_, sentinel_);
  }

  bool empty() const
  {
    return root_ == sentinel_;
  }

  size_t size() const
  {
    return size_;
  }
};

} 

#endif 