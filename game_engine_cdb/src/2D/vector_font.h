#ifndef VECTORFONT_H_INCLUDED
#define VECTORFONT_H_INCLUDED
#include <vector>
using namespace std;

struct font_point2f
{
  float x, y;
};

typedef vector<font_point2f> symbol;

//������ ������ ��������
//������ ����� �������� ����������� �����
class vectorFont
{
 public:
 vectorFont();
 ~vectorFont();

 int LoadFont(char* file_name);
 int SaveFont(char* file_name);
 int Clear();
 int SetSize(int s);

 symbol symbols[256];//����� ������� ������������� ��� ���� � ���� ������� ��������
 float s_width[256];//����� �������

 int font_size;//������ �������
 int aprosh;//���������� ����� �������
 int l_width;//������� �����
};

#endif // VECTORFONT_H_INCLUDED
