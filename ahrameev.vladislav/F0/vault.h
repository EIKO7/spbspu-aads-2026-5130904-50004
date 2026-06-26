// include/vault.h
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
  std::string generate_password(size_t length);
  std::string check_password_strength(const std::string& password);

 private:
  HashTable table_;
};

}

#endif