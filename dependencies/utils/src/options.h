//  ☀️
#ifndef OPTIONS_H
#define OPTIONS_H
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

struct Option {
  string Name;
  string Value;
};

class OptionsSet {
public:
  OptionsSet();
  ~OptionsSet();
  vector<Option> options;
  // формат файлов таблица с разделителем ';'
  int openFile(char *fn);
  int saveFile(char *fn);
  string Value(string Name);
  int setValue(string Name, string Val);
};
#endif
