#ifndef VECTORFONT_H_INCLUDED
#define VECTORFONT_H_INCLUDED
#include <vector>
using namespace std;

struct font_point2f
{
  float x, y;
};

typedef vector<font_point2f> symbol;

//хранит список символов
//списки линий векторов описывающих буквы
class vectorFont
{
 public:
 vectorFont();
 ~vectorFont();

 int LoadFont(char* file_name);
 int SaveFont(char* file_name);
 int Clear();
 int SetSize(int s);

 symbol symbols[256];//номер символа соответствует его коду в анси таблице символов
 float s_width[256];//длина символа

 int font_size;//высота символа
 int aprosh;//расстояние между буквами
 int l_width;//толщина линий
};

#endif // VECTORFONT_H_INCLUDED
