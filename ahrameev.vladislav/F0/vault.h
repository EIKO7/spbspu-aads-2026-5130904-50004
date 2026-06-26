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
  bool update(
      const std::string& service,
      const std::string& field,
      const std::string& value);
  bool remove(const std::string& service);
  std::vector<std::string> list() const;
  std::vector<std::string> search(const std::string& query) const;
  std::string generate_password(size_t length);
  std::string check_password_strength(const std::string& password);

  void set_key(const std::string& key);
  bool is_key_set() const;
  bool save_to_file(const std::string& filename);
  bool load_from_file(const std::string& filename);

 private:
  HashTable table_;
  std::vector<uint32_t> encryption_key_;

  std::vector<uint8_t> serialize() const;
  bool deserialize(const std::vector<uint8_t>& data);
  static std::vector<uint8_t> read_file_bytes(const std::string& filename);
  static bool
  write_file_bytes(const std::string& filename, const std::vector<uint8_t>& data);
};

}

#endif