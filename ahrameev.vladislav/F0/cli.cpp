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
  if (args.size() < 4) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (vault_.add(args[1], args[2], args[3])) {
    std::cout << "<OK>\n";
  } else {
    std::cout << "<ERROR: service already exists>\n";
  }
}

void CLI::cmd_get(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  const Record* record = vault_.get(args[1]);
  if (record) {
    std::cout << "<login: " << record->login
              << ", password: " << record->password << ">\n";
  } else {
    std::cout << "<NOT FOUND>\n";
  }
}

void CLI::cmd_update(const std::vector<std::string>& args) {
  if (args.size() < 4) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (args[2] != "login" && args[2] != "password") {
    std::cout << "<ERROR: invalid field>\n";
    return;
  }
  if (vault_.update(args[1], args[2], args[3])) {
    std::cout << "<OK>\n";
  } else {
    std::cout << "<NOT FOUND>\n";
  }
}

void CLI::cmd_delete(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (vault_.remove(args[1])) {
    std::cout << "<OK>\n";
  } else {
    std::cout << "<NOT FOUND>\n";
  }
}

void CLI::cmd_list(const std::vector<std::string>&) {
  std::vector<std::string> services = vault_.list();
  if (services.empty()) {
    std::cout << "<EMPTY>\n";
    return;
  }
  for (size_t i = 0; i < services.size(); ++i) {
    std::cout << "<" << services[i] << ">\n";
  }
}

void CLI::cmd_generate(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  try {
    size_t length = std::stoul(args[1]);
    if (length < 6) {
      std::cout << "<ERROR: min length is 6>\n";
      return;
    }
    std::cout << "<" << vault_.generate_password(length) << ">\n";
  } catch (...) {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void CLI::cmd_check(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::cout << "<" << vault_.check_password_strength(args[1]) << ">\n";
}

void CLI::cmd_search(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  std::vector<std::string> results = vault_.search(args[1]);
  if (results.empty()) {
    std::cout << "<NO RESULTS>\n";
    return;
  }
  for (size_t i = 0; i < results.size(); ++i) {
    std::cout << "<" << results[i] << ">\n";
  }
}

void CLI::cmd_set_key(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  vault_.set_key(args[1]);
  std::cout << "<KEY SET>\n";
}

void CLI::cmd_save(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (vault_.save_to_file(args[1])) {
    std::string status =
        vault_.is_key_set() ? "(encrypted)" : "(plaintext)";
    std::cout << "<SAVED: " << vault_.list().size()
              << " records " << status << ">\n";
  } else {
    std::cout << "<ERROR: failed to save>\n";
  }
}

void CLI::cmd_load(const std::vector<std::string>& args) {
  if (args.size() < 2) {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  if (vault_.load_from_file(args[1])) {
    std::cout << "<LOADED: " << vault_.list().size() << " records>\n";
  } else {
    std::cout << "<ERROR: file not found>\n";
  }
}

void CLI::cmd_help(const std::vector<std::string>&) {
  std::cout << "<Available commands:>\n"
            << "<add, get, update, delete, list>\n"
            << "<generate, check, search>\n"
            << "<set-key, save, load, help>\n";
}

}