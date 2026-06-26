#ifndef AHFRAMEEV_CLI_H
#define AHFRAMEEV_CLI_H

#include <string>
#include <vector>

#include "vault.h"

namespace ahrameev {

class CLI {
 public:
  explicit CLI(PasswordVault& vault);
  void run();

 private:
  PasswordVault& vault_;

  void handle_line(const std::string& line);
  void cmd_add(const std::vector<std::string>& args);
  void cmd_get(const std::vector<std::string>& args);
  void cmd_update(const std::vector<std::string>& args);
  void cmd_delete(const std::vector<std::string>& args);
  void cmd_list(const std::vector<std::string>& args);
  void cmd_generate(const std::vector<std::string>& args);
  void cmd_check(const std::vector<std::string>& args);
  void cmd_search(const std::vector<std::string>& args);
  void cmd_set_key(const std::vector<std::string>& args);
  void cmd_save(const std::vector<std::string>& args);
  void cmd_load(const std::vector<std::string>& args);
  void cmd_help(const std::vector<std::string>& args);
};

}

#endif