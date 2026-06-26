#include "xtea.h"

#include <cstring>

namespace ahrameev {

void encrypt_block(uint32_t v[2], const uint32_t key[4], uint32_t rounds) {
  uint32_t sum = 0;
  const uint32_t delta = 0x9E3779B9;
  for (uint32_t i = 0; i < rounds; ++i) {
    v[0] += ((v[1] << 4 ^ v[1] >> 5) + v[1]) ^ (sum + key[sum & 3]);
    sum += delta;
    v[1] += ((v[0] << 4 ^ v[0] >> 5) + v[0]) ^ (sum + key[(sum >> 11) & 3]);
  }
}

void decrypt_block(uint32_t v[2], const uint32_t key[4], uint32_t rounds) {
  uint32_t sum = 0x9E3779B9 * rounds;
  const uint32_t delta = 0x9E3779B9;
  for (uint32_t i = 0; i < rounds; ++i) {
    v[1] -= ((v[0] << 4 ^ v[0] >> 5) + v[0]) ^ (sum + key[(sum >> 11) & 3]);
    sum -= delta;
    v[0] -= ((v[1] << 4 ^ v[1] >> 5) + v[1]) ^ (sum + key[sum & 3]);
  }
}

std::vector<uint32_t> derive_key(const std::string& pass_key) {
  std::vector<uint8_t> raw(16, 0);
  for (size_t i = 0; i < 16; ++i) {
    raw[i] = static_cast<uint8_t>(pass_key[i % pass_key.size()]);
  }
  std::vector<uint32_t> key(4);
  std::memcpy(key.data(), raw.data(), 16);
  return key;
}

std::vector<uint8_t> pad_pkcs7(const std::vector<uint8_t>& data) {
  size_t pad_len = 8 - (data.size() % 8);
  std::vector<uint8_t> padded = data;
  padded.insert(padded.end(), pad_len, static_cast<uint8_t>(pad_len));
  return padded;
}

std::vector<uint8_t> unpad_pkcs7(const std::vector<uint8_t>& data) {
  if (data.empty()) {
    return {};
  }
  uint8_t pad_len = data.back();
  if (pad_len == 0 || pad_len > 8) {
    return {};
  }
  for (size_t i = data.size() - pad_len; i < data.size(); ++i) {
    if (data[i] != pad_len) {
      return {};
    }
  }
  return std::vector<uint8_t>(data.begin(), data.end() - pad_len);
}

std::vector<uint8_t> xtea_encrypt(const std::vector<uint8_t>& data, const std::vector<uint32_t>& key) {
  auto padded = pad_pkcs7(data);
  std::vector<uint8_t> out(padded.size());
  for (size_t i = 0; i < padded.size(); i += 8) {
    uint32_t block[2];
    std::memcpy(block, &padded[i], 8);
    encrypt_block(block, key.data(), 32);
    std::memcpy(&out[i], block, 8);
  }
  return out;
}

std::vector<uint8_t> xtea_decrypt(const std::vector<uint8_t>& data, const std::vector<uint32_t>& key) {
  if (data.size() % 8 != 0) {
    return {};
  }
  std::vector<uint8_t> out(data.size());
  for (size_t i = 0; i < data.size(); i += 8) {
    uint32_t block[2];
    std::memcpy(block, &data[i], 8);
    decrypt_block(block, key.data(), 32);
    std::memcpy(&out[i], block, 8);
  }
  return unpad_pkcs7(out);
}

}