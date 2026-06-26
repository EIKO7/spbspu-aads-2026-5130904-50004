#include "cli.h"

#include <iostream>
#include <sstream>

namespace ahrameev {

CLI::CLI(PasswordVault& vault) : vault_(vault) {}

void CLI::run() {
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    handle_line(line);
  }
}

void CLI::handle_line(const std::string& line) {
  std::istringstream iss(line);
  std::vector<std::string> args;
  std::string token;
  while (iss >> token) {
    args.push_back(token);
  }
  if (args.empty()) {
    return;
  }

  const std::string& command = args[0];
  if (command == "add") {
    cmd_add(args);
  } else if (command == "get") {
    cmd_get(args);
  } else if (command == "update") {
    cmd_update(args);
  } else if (command == "delete") {
    cmd_delete(args);
  } else if (command == "list") {
    cmd_list(args);
  } else if (command == "generate") {
    cmd_generate(args);
  } else if (command == "check") {
    cmd_check(args);
  } else if (command == "search") {
    cmd_search(args);
  } else if (command == "set-key") {
    cmd_set_key(args);
  } else if (command == "save") {
    cmd_save(args);
  } else if (command == "load") {
    cmd_load(args);
  } else if (command == "help") {
    cmd_help(args);
  } else {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void CLI::cmd_add(const std::vector<std::string>& args) {
  std::cout << "<OK>\n";
}

void CLI::cmd_get(const std::vector<std::string>& args) {
  std::cout << "<NOT FOUND>\n";
}

void CLI::cmd_update(const std::vector<std::string>& args) {
  std::cout << "<OK>\n";
}

void CLI::cmd_delete(const std::vector<std::string>& args) {
  std::cout << "<OK>\n";
}

void CLI::cmd_list(const std::vector<std::string>& args) {
  std::cout << "<EMPTY>\n";
}

void CLI::cmd_generate(const std::vector<std::string>& args) {
  std::cout << "<INVALID COMMAND>\n";
}

void CLI::cmd_check(const std::vector<std::string>& args) {
  std::cout << "<INVALID COMMAND>\n";
}

void CLI::cmd_search(const std::vector<std::string>& args) {
  std::cout << "<NO RESULTS>\n";
}

void CLI::cmd_set_key(const std::vector<std::string>& args) {
  std::cout << "<KEY SET>\n";
}

void CLI::cmd_save(const std::vector<std::string>& args) {
  std::cout << "<SAVED>\n";
}

void CLI::cmd_load(const std::vector<std::string>& args) {
  std::cout << "<LOADED>\n";
}

void CLI::cmd_help(const std::vector<std::string>& args) {
  std::cout << "<Available commands:>\n"
            << "<add, get, update, delete, list>\n"
            << "<generate, check, search>\n"
            << "<set-key, save, load, help>\n";
}

}