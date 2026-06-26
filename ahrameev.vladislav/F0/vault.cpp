#include "vault.h"

#include <random>

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

std::string PasswordVault::generate_password(size_t length) {
  const std::string charset =
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "0123456789!@#$%^&*";
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, static_cast<int>(charset.size()) - 1);
  std::string result;
  result.reserve(length);
  for (size_t i = 0; i < length; ++i) {
    result += charset[static_cast<size_t>(dis(gen))];
  }
  return result;
}

std::string
PasswordVault::check_password_strength(const std::string& password) {
  if (password.size() < 6) {
    return "WEAK";
  }
  bool has_digit = false;
  bool has_upper = false;
  bool has_lower = false;
  bool has_special = false;
  for (unsigned char c : password) {
    if (std::isdigit(c)) {
      has_digit = true;
    } else if (std::isupper(c)) {
      has_upper = true;
    } else if (std::islower(c)) {
      has_lower = true;
    } else {
      has_special = true;
    }
  }
  int score = has_digit + has_upper + has_lower + has_special;
  if (score >= 3 && password.size() >= 10) {
    return "STRONG";
  }
  return "WEAK";
}

}