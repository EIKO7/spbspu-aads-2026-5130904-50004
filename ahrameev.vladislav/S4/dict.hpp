#ifndef DICT_HPP
#define DICT_HPP

#include <string>

#include "bst.hpp"

namespace ahrameev
{

using Dictionary = BSTree< int, std::string >;
using DictionaryStore = BSTree< std::string, Dictionary >;

} 

#endif 