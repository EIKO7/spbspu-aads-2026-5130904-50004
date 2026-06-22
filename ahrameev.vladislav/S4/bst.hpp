#ifndef BST_HPP
#define BST_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
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

  size_t height(Node* node) const
  {
    if (node == sentinel_) {
      return 0;
    }
    size_t lh = height(node->left);
    size_t rh = height(node->right);
    return 1 + (lh > rh ? lh : rh);
  }

  Node* findNode(const Key& k) const
  {
    Node* cur = root_;
    while (cur != sentinel_) {
      if (comp_(k, cur->data.first)) {
        cur = cur->left;
      } else if (comp_(cur->data.first, k)) {
        cur = cur->right;
      } else {
        return cur;
      }
    }
    return nullptr;
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

  void push(const Key& k, const Value& v)
  {
    if (root_ == sentinel_) {
      root_ = new Node(k, v, sentinel_);
      root_->left = sentinel_;
      root_->right = sentinel_;
      size_++;
      return;
    }
    Node* cur = root_;
    while (true) {
      if (comp_(k, cur->data.first)) {
        if (cur->left == sentinel_) {
          cur->left = new Node(k, v, cur);
          cur->left->left = sentinel_;
          cur->left->right = sentinel_;
          size_++;
          return;
        }
        cur = cur->left;
      } else if (comp_(cur->data.first, k)) {
        if (cur->right == sentinel_) {
          cur->right = new Node(k, v, cur);
          cur->right->left = sentinel_;
          cur->right->right = sentinel_;
          size_++;
          return;
        }
        cur = cur->right;
      } else {
        cur->data.second = v;
        return;
      }
    }
  }

  Value get(const Key& k) const
  {
    Node* node = findNode(k);
    if (!node) {
      throw std::runtime_error("Key not found");
    }
    return node->data.second;
  }

  Value drop(const Key& k)
  {
    Node* node = findNode(k);
    if (!node) {
      throw std::runtime_error("Key not found");
    }
    Value res = node->data.second;
    if (node->left == sentinel_ && node->right == sentinel_) {
      if (node->parent == sentinel_) {
        root_ = sentinel_;
      } else if (node == node->parent->left) {
        node->parent->left = sentinel_;
      } else {
        node->parent->right = sentinel_;
      }
      delete node;
    } else if (node->left == sentinel_ || node->right == sentinel_) {
      Node* child = (node->left != sentinel_) ? node->left : node->right;
      if (node->parent == sentinel_) {
        root_ = child;
      } else if (node == node->parent->left) {
        node->parent->left = child;
      } else {
        node->parent->right = child;
      }
      child->parent = node->parent;
      delete node;
    } else {
      Node* succ = node->right;
      while (succ->left != sentinel_) {
        succ = succ->left;
      }
      node->data = succ->data;
      if (succ->right == sentinel_) {
        if (succ == succ->parent->left) {
          succ->parent->left = sentinel_;
        } else {
          succ->parent->right = sentinel_;
        }
      } else {
        succ->right->parent = succ->parent;
        if (succ == succ->parent->left) {
          succ->parent->left = succ->right;
        } else {
          succ->parent->right = succ->right;
        }
      }
      delete succ;
    }
    size_--;
    return res;
  }

  const_iterator rotateLeft(const_iterator it) const
  {
    Node* x = it.current_;
    if (x == sentinel_ || x->right == sentinel_) {
      return it;
    }
    Node* y = x->right;
    x->right = y->left;
    if (y->left != sentinel_) {
      y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == sentinel_) {
      root_ = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    return const_iterator(y, sentinel_);
  }

  const_iterator rotateRight(const_iterator it) const
  {
    Node* x = it.current_;
    if (x == sentinel_ || x->left == sentinel_) {
      return it;
    }
    Node* y = x->left;
    x->left = y->right;
    if (y->right != sentinel_) {
      y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == sentinel_) {
      root_ = y;
    } else if (x == x->parent->right) {
      x->parent->right = y;
    } else {
      x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
    return const_iterator(y, sentinel_);
  }

  const_iterator rotateLargeLeft(const_iterator it) const
  {
    Node* x = it.current_;
    if (x == sentinel_ || x->right == sentinel_ ||
        x->right->left == sentinel_) {
      return it;
    }
    rotateRight(const_iterator(x->right, sentinel_));
    return rotateLeft(it);
  }

  const_iterator rotateLargeRight(const_iterator it) const
  {
    Node* x = it.current_;
    if (x == sentinel_ || x->left == sentinel_ ||
        x->left->right == sentinel_) {
      return it;
    }
    rotateLeft(const_iterator(x->left, sentinel_));
    return rotateRight(it);
  }

  size_t height() const
  {
    return height(root_);
  }

  size_t height(const_iterator it) const
  {
    return height(it.current_);
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