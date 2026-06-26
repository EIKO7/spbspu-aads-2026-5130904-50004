#include "vault.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <random>
#include <sstream>

#include "xtea.h"

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

bool PasswordVault::update(
    const std::string& service,
    const std::string& field,
    const std::string& value) {
  const Record* rec = table_.find(service);
  if (!rec) {
    return false;
  }
  Record updated = *rec;
  if (field == "login") {
    updated.login = value;
  } else if (field == "password") {
    updated.password = value;
  } else {
    return false;
  }
  table_.erase(service);
  table_.insert(service, updated);
  return true;
}

bool PasswordVault::remove(const std::string& service) {
  return table_.erase(service);
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

std::vector<std::string> PasswordVault::search(const std::string& query) const {
  std::vector<std::string> result;
  std::string query_lower = query;
  std::transform(
      query_lower.begin(),
      query_lower.end(),
      query_lower.begin(),
      static_cast<int (*)(int)>(std::tolower));
  for (const auto& entry : table_.entries()) {
    if (!entry.occupied) {
      continue;
    }
    std::string key_lower = entry.key;
    std::transform(
        key_lower.begin(),
        key_lower.end(),
        key_lower.begin(),
        static_cast<int (*)(int)>(std::tolower));
    if (key_lower.find(query_lower) != std::string::npos) {
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

void PasswordVault::set_key(const std::string& key) {
  encryption_key_ = {};
  if (!key.empty()) {
    encryption_key_.resize(4);
    for (size_t i = 0; i < 16 && i < key.size(); ++i) {
      reinterpret_cast<uint8_t*>(encryption_key_.data())[i] =
          static_cast<uint8_t>(key[i]);
    }
  }
}

bool PasswordVault::is_key_set() const {
  return !encryption_key_.empty();
}

std::vector<uint8_t> PasswordVault::serialize() const {
  std::vector<uint8_t> buffer;
  std::vector<std::string> services = list();
  for (size_t i = 0; i < services.size(); ++i) {
    const std::string& service = services[i];
    const Record* record = table_.find(service);
    uint32_t len = static_cast<uint32_t>(service.size());
    buffer.insert(
        buffer.end(),
        reinterpret_cast<const uint8_t*>(&len),
        reinterpret_cast<const uint8_t*>(&len) + 4);
    buffer.insert(buffer.end(), service.begin(), service.end());

    len = static_cast<uint32_t>(record->login.size());
    buffer.insert(
        buffer.end(),
        reinterpret_cast<const uint8_t*>(&len),
        reinterpret_cast<const uint8_t*>(&len) + 4);
    buffer.insert(buffer.end(), record->login.begin(), record->login.end());

    len = static_cast<uint32_t>(record->password.size());
    buffer.insert(
        buffer.end(),
        reinterpret_cast<const uint8_t*>(&len),
        reinterpret_cast<const uint8_t*>(&len) + 4);
    buffer.insert(
        buffer.end(), record->password.begin(), record->password.end());
  }
  return buffer;
}

bool PasswordVault::deserialize(const std::vector<uint8_t>& data) {
  table_ = HashTable();
  size_t pos = 0;
  while (pos + 4 <= data.size()) {
    uint32_t len;
    std::memcpy(&len, &data[pos], 4);
    pos += 4;
    if (pos + len > data.size()) {
      return false;
    }
    std::string service(
        reinterpret_cast<const char*>(&data[pos]), len);
    pos += len;

    if (pos + 4 > data.size()) {
      return false;
    }
    std::memcpy(&len, &data[pos], 4);
    pos += 4;
    if (pos + len > data.size()) {
      return false;
    }
    std::string login(
        reinterpret_cast<const char*>(&data[pos]), len);
    pos += len;

    if (pos + 4 > data.size()) {
      return false;
    }
    std::memcpy(&len, &data[pos], 4);
    pos += 4;
    if (pos + len > data.size()) {
      return false;
    }
    std::string password(
        reinterpret_cast<const char*>(&data[pos]), len);
    pos += len;

    table_.insert(service, {login, password});
  }
  return true;
}

bool PasswordVault::save_to_file(const std::string& filename) {
  std::vector<uint8_t> raw_data = serialize();
  std::vector<uint8_t> output = raw_data;
  if (is_key_set()) {
    output = xtea_encrypt(raw_data, encryption_key_);
  }
  return write_file_bytes(filename, output);
}

bool PasswordVault::load_from_file(const std::string& filename) {
  std::vector<uint8_t> raw_data = read_file_bytes(filename);
  if (raw_data.empty()) {
    return false;
  }
  if (is_key_set()) {
    raw_data = xtea_decrypt(raw_data, encryption_key_);
    if (raw_data.empty()) {
      return false;
    }
  }
  return deserialize(raw_data);
}

std::vector<uint8_t>
PasswordVault::read_file_bytes(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) {
    return std::vector<uint8_t>();
  }
  return std::vector<uint8_t>(
      std::istreambuf_iterator<char>(file),
      std::istreambuf_iterator<char>());
}

bool PasswordVault::write_file_bytes(
    const std::string& filename,
    const std::vector<uint8_t>& data) {
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    return false;
  }
  file.write(
      reinterpret_cast<const char*>(data.data()),
      static_cast<std::streamsize>(data.size()));
  return file.good();
}

}