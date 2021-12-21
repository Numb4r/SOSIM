#include "disk.hpp"
void Disk::reset() {}
void Disk::createDir(const char *name) {
  this->path.push_back(nodePath(name, true));
}
void Disk::createFile(const char *name) {
  this->path.push_back(nodePath(name, false));
}