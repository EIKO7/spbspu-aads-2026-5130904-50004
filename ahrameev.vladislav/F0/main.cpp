#include <iostream>

#include "cli.h"

int main() {
  ahrameev::PasswordVault vault;
  ahrameev::CLI cli(vault);

  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  cli.run();
}