//  ☀️
#ifndef VECTORFONT_H_INCLUDED
#define VECTORFONT_H_INCLUDED
#include <vector>

struct font_point2f {
  float x, y;
};

using symbol = std::vector<font_point2f>;

// õðàíèò ñïèñîê ñèìâîëîâ
// ñïèñêè ëèíèé âåêòîðîâ îïèñûâàþùèõ áóêâû
class vectorFont {
public:
  vectorFont();
  ~vectorFont();

  int LoadFont(char *file_name);
  int SaveFont(char *file_name);
  int Clear();
  int SetSize(int s);

  symbol symbols[256]; // íîìåð ñèìâîëà ñîîòâåòñòâóåò åãî êîäó â àíñè òàáëèöå
                       // ñèìâîëîâ
  float s_width[256]; // äëèíà ñèìâîëà

  int font_size; // âûñîòà ñèìâîëà
  int aprosh;    // ðàññòîÿíèå ìåæäó áóêâàìè
  int l_width;   // òîëùèíà ëèíèé
};

#endif // VECTORFONT_H_INCLUDED
