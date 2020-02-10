//  ☀️
#include "vector_font.h"
#include <stdio.h>
vectorFont::vectorFont() { SetSize(25); }

vectorFont::~vectorFont() {
  int i;
  for (i = 0; i < 256; i++)
    symbols[i].clear();
}

int vectorFont::LoadFont(char *file_name) {
  FILE *f = NULL;
  unsigned char c;
  int sz, i, j = 0;
  font_point2f fp;
  float w;
  f = fopen(file_name, "rb");
  if (f == NULL)
    return 0;
  while (!feof(f)) {
    j++;
    fread(&c, 1, 1, f);             // code of symbol
    fread(&sz, sizeof(int), 1, f);  // lines number
    fread(&w, sizeof(float), 1, f); // width of symbol
    s_width[c] = w;
    for (i = 0; i < sz; i++) {
      fread(&fp.x, sizeof(float), 1, f);
      fread(&fp.y, sizeof(float), 1, f);
      symbols[c].push_back(fp);
    }
  }
  s_width[32] = 0.3; // пробел
  fclose(f);
  return 1;
}

int vectorFont::SetSize(int s) {
  font_size = s;
  aprosh = font_size / 9.0;
  l_width = font_size / 18.0;
  return 1;
}

int vectorFont::SaveFont(char *info_file_name) {
  FILE *f = NULL;
  unsigned int i, j, sz;
  unsigned char c;
  f = fopen(info_file_name, "wb");
  if (f != NULL) {
    for (i = 0; i < 256; i++) {
      c = i;
      if (symbols[i].size()) {
        fwrite(&c, 1, 1, f);
        sz = symbols[i].size();
        fwrite(&sz, sizeof(unsigned int), 1, f);
        fwrite(&(s_width[i]), sizeof(float), 1, f);
        for (j = 0; j < symbols[i].size(); j++) {
          fwrite(&symbols[i][j].x, sizeof(float), 1, f);
          fwrite(&symbols[i][j].y, sizeof(float), 1, f);
        }
      }
    }
    fclose(f);
  }

  return 0;
}

int vectorFont::Clear() {
  int i;
  for (i = 0; i < 256; i++) {
    if (symbols[i].size())
      symbols[i].clear();
    s_width[i] = 0;
  }
  return 0;
}
