#ifndef keys_h
#define keys_h

#include <string>
#include <vector>
#include <array>
using std::array;

int kName2Int(const std::string &name);

extern array<std::string, 128> keyNames;

#endif // keys_h
