#ifndef AHFRAMEEV_XTEA_H
#define AHFRAMEEV_XTEA_H

#include <cstdint>
#include <string>
#include <vector>

namespace ahrameev {

std::vector<uint32_t> derive_key(const std::string& pass_key);
std::vector<uint8_t> xtea_encrypt(const std::vector<uint8_t>& data, const std::vector<uint32_t>& key);
std::vector<uint8_t> xtea_decrypt(const std::vector<uint8_t>& data, const std::vector<uint32_t>& key);

}

#endif