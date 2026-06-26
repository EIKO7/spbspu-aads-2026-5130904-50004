#include <iostream>

#include "vault.h"

int main() {
  ahrameev::PasswordVault vault;
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::cout << "<OK>\n";
  }
}