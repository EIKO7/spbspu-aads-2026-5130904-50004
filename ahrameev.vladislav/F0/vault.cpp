#include "vault.h"

namespace ahrameev {

PasswordVault::PasswordVault() = default;

bool PasswordVault::add(
    const std::string& service,
    const std::string& login,
    const std::string& password) {
  if (!table_.find(service)) {
    return table_.insert(service, {login, password});
  }
  return false;
}

const Record* PasswordVault::get(const std::string& service) const {
  return table_.find(service);
}

std::vector<std::string> PasswordVault::list() const {
  std::vector<std::string> result;
  for (const auto& entry : table_.entries()) {
    if (entry.occupied) {
      result.push_back(entry.key);
    }
  }
  return result;
}

}