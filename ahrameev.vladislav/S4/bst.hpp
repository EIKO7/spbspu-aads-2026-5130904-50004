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

} // namespace ahrameev

#endif // BST_HPP