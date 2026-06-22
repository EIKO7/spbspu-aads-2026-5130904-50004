#include "manager.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Cannot open file: " << argv[1] << std::endl;
    return 1;
  }
  ahrameev::DictionaryManager manager;
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    size_t pos = 0;
    std::string dictName;
    while (pos < line.size() && line[pos] != ' ') {
      dictName += line[pos++];
    }
    if (pos < line.size()) {
      pos++;
    }
    ahrameev::Dictionary dict;
    while (pos < line.size()) {
      int key = 0;
      bool neg = false;
      if (line[pos] == '-') {
        neg = true;
        pos++;
      }
      while (pos < line.size() && line[pos] >= '0' && line[pos] <= '9') {
        key = key * 10 + (line[pos] - '0');
        pos++;
      }
      if (neg) {
        key = -key;
      }
      if (pos < line.size()) {
        pos++;
      }
      std::string value;
      while (pos < line.size() && line[pos] != ' ') {
        value += line[pos++];
      }
      if (pos < line.size()) {
        pos++;
      }
      dict.push(key, value);
    }
    manager.addDictionary(dictName, dict);
  }
}