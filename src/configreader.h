#ifndef _CONFIGREADER_H_
#define _CONFIGREADER_H_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>

using namespace std;

class ConfigReader {
public:
  explicit ConfigReader(string fileName) : filename(std::move(fileName)) {}

  // clear content
  ~ConfigReader() { data.clear(); }

  // This method returns true if the file contains data.
  bool parse();

  std::map<std::string, std::string> &getData() { return data; }

  // Read/Write values for an existing token
  std::string &operator[](const std::string &token) { return data[token]; }

  std::string &getValue(const std::string &token) { return data[token]; }

  void setValue(const std::string &token, const std::string &value) {
    data[token] = value;
  }

  bool isEmpty();

  // Save
  // void Save();

private:
  std::map<std::string, std::string> data;
  std::string filename;
};

#endif
