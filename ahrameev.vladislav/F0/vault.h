#ifndef AHFRAMEEV_VAULT_H
#define AHFRAMEEV_VAULT_H

#include <cstdint>
#include <string>
#include <vector>

#include "hashtable.h"

namespace ahrameev {

class PasswordVault {
 public:
  PasswordVault();

  bool add(
      const std::string& service,
      const std::string& login,
      const std::string& password);
  const Record* get(const std::string& service) const;
  std::vector<std::string> list() const;

 private:
  HashTable table_;
};

}

#endif