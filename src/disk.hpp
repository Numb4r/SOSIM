#pragma once
#include <cstddef>
#include <list>
#include <string>
#include <vector>
class nodePath {
  bool isDirectory;
  std::string name;

public:
  nodePath(std::string name, bool isDirectory)
      : name(name), isDirectory(isDirectory) {}
};
class Disk {
  std::vector<nodePath> path;

public:
  void reset();
  void createDir(const char *name);
  void createFile(const char *name);
};