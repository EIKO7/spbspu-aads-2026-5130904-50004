#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "dict.hpp"

namespace ahrameev
{

class DictionaryManager
{
  DictionaryStore store_;

public:
  bool addDictionary(const std::string& name, const Dictionary& dict)
  {
    try {
      store_.get(name);
      return false;
    } catch (...) {
      store_.push(name, dict);
      return true;
    }
  }

  bool hasDictionary(const std::string& name) const
  {
    try {
      store_.get(name);
      return true;
    } catch (...) {
      return false;
    }
  }

  const Dictionary& getDictionary(const std::string& name) const
  {
    return store_.get(name);
  }

  bool complement(const std::string& newName, const std::string& n1,
    const std::string& n2)
  {
    if (!hasDictionary(n1) || !hasDictionary(n2) || hasDictionary(newName)) {
      return false;
    }
    Dictionary res;
    const Dictionary& d1 = getDictionary(n1);
    const Dictionary& d2 = getDictionary(n2);
    for (auto it = d1.begin(); it != d1.end(); ++it) {
      try {
        d2.get(it->first);
      } catch (...) {
        res.push(it->first, it->second);
      }
    }
    return addDictionary(newName, res);
  }

  bool intersect(const std::string& newName, const std::string& n1,
    const std::string& n2)
  {
    if (!hasDictionary(n1) || !hasDictionary(n2) || hasDictionary(newName)) {
      return false;
    }
    Dictionary res;
    const Dictionary& d1 = getDictionary(n1);
    const Dictionary& d2 = getDictionary(n2);
    for (auto it = d1.begin(); it != d1.end(); ++it) {
      try {
        d2.get(it->first);
        res.push(it->first, it->second);
      } catch (...) {
      }
    }
    return addDictionary(newName, res);
  }

  bool unionDicts(const std::string& newName, const std::string& n1,
    const std::string& n2)
  {
    if (!hasDictionary(n1) || !hasDictionary(n2) || hasDictionary(newName)) {
      return false;
    }
    Dictionary res = getDictionary(n1);
    const Dictionary& d2 = getDictionary(n2);
    for (auto it = d2.begin(); it != d2.end(); ++it) {
      try {
        res.get(it->first);
      } catch (...) {
        res.push(it->first, it->second);
      }
    }
    return addDictionary(newName, res);
  }
};

} 

#endif 