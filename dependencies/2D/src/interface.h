//  ☀️
///
/// 2d interface based on OpenGL
/// v1.0
/// Klepikov N.V.
///
#ifndef _2D_INTERFACE_H
#define _2D_INTERFACE_H
#include "vector_font.h"
#include <string>

// определение флагов выравнивания
#define iiNoAlign -1
#define iiAlignCenter 0
#define iiAlignMin 1
#define iiAlignMax 2

//-------------------------------------------------------------------------------------------------------
struct color4fStruct {
  float r, g, b, a;
  void Set(float red, float green, float blue, float alfa);
};
//-------------------------------------------------------------------------------------------------------
/// class InterfaceItem
// прародитель всех элементов
// отвечает за передачю системных событий по цепочке родитель потомок
// реализует связь родитель потомок
// поддерживает вывод строк текста и обработку фокуса ввода
class InterfaceItem // base element of all interface elements
{
public:
  // constructor
  InterfaceItem(InterfaceItem *prnt = NULL);
  // destructor
  virtual ~InterfaceItem();
  // data
  InterfaceItem *parent;
  std::vector<InterfaceItem *> childs;

  int x_pos, y_pos, width, height;
  vectorFont *font; // указатель на шрифт
  int font_size;    // собственное значение
  std::string caption;
  bool visible, focus, selected;
  int child_focus; // номер потомка имеющего фокус

  color4fStruct fnt_clr, frm_clr, bkg_clr, slct_clr;
  // functions
  InterfaceItem *GetParent();

  virtual void SetPos(int x,
                      int y); // задать позицию левого нижнего угла элемента
  virtual void SetSize(int w, int h); // задать размер элемента
  virtual void RedrawItem();

  /// OnMouseDown
  // btn: 1-left button 2-right 3-middle
  // возвращает в класс предок признак фокуса
  // переопределенный метод при нормальном поведении должен возвращать признак
  // фокуса 1-есть фокус ввода, 0-нет фокуса ввода
  virtual int OnMouseDown(int x, int y, int btn);

  virtual int OnMouseUp(int x, int y, int btn);
  virtual int OnType(int ch);
  virtual int OnKeyDown(int c);
  virtual int OnKeyUp(int c);
  virtual int OnIdle();
  virtual int OnMouseMove(int x, int y);
  virtual int OnSize(int w, int h);
  void Show();
  void Hide();
  void RenderString(std::string str);

  // длина строки в пикселах
  int SizeOfString(std::string str);

  /// Align
  // выравнивание относительно предка
  // выравниевает элемент относительно предка вдоль оси х и у
  // xflag yflag могут принимать значения
  // iiNoAlign -1
  // #define iiAlignCenter 0
  // #define iiAlignMin 1
  // #define iiAlignMax 2
  void Align(int xflag, int yflag);
};

//-------------------------------------------------------------------------------------------------------
/// Interface
// невидимое окно все формы приложения и элементы интерфейса являются его
// потомками. корневое окно вместо прямого использования в программе следует
// создать класс наследник к примеру MyInterface внутри которого определеить
// указатели на элементы интерфейса чтобы было удобнее в переопределенных
// методах класса реагировать на системные события и взаимодествовать элементам
// между собой в MyInterface можно включить метод OnInit() для удобства:
// определение(выделение памяти) всех его потомков
class Interface : public InterfaceItem {
public:
  Interface(int w = 640, int h = 480, vectorFont *f = NULL);
  virtual ~Interface();
  virtual void RedrawItem() override;
  virtual int OnSize(int w, int h) override;
};
//-------------------------------------------------------------------------------------------------------
class TextLabel : public InterfaceItem {
public:
  TextLabel(InterfaceItem *it = NULL);
  virtual ~TextLabel();
  virtual void RedrawItem() override;
  virtual void SetCaption(std::string str);
};
//-------------------------------------------------------------------------------------------------------
class ImageLabel : public InterfaceItem {
public:
  ImageLabel(InterfaceItem *it = NULL);
  virtual ~ImageLabel();
  virtual void RedrawItem() override;
  void SetImage(
      int index); // передается номер уже проинициализированной текстуры))))
  int tex_index;
};
//-------------------------------------------------------------------------------------------------------
class Form : public InterfaceItem {
public:
  Form(InterfaceItem *it = NULL, int esc_key_code = 27);
  virtual ~Form();
  virtual void RedrawItem() override;
  virtual int OnKeyDown(int c) override;
  int ESC_KEY_CODE = 27;
};
//-------------------------------------------------------------------------------------------------------
class Button : public InterfaceItem {
public:
  Button(InterfaceItem *it = NULL);
  virtual ~Button();
  virtual void RedrawItem() override;
  virtual int OnKeyDown(int c) override;
  virtual int OnMouseDown(int x, int y, int btn) override; // 1-l 2-r 3-m
  virtual int OnMouseUp(int x, int y, int btn) override;
  virtual void SetCaption(std::string str);
  int mousebtn;
};

//-------------------------------------------------------------------------------------------------------
class Menu : public InterfaceItem {
public:
  Menu(InterfaceItem *it = NULL);
  virtual ~Menu();

  virtual int OnMouseDown(int x, int y, int btn) override;
  virtual void RedrawItem() override;
  int SetStringItems(std::vector<std::string> strs);
};
//-------------------------------------------------------------------------------------------------------
class Message : public Form {
public:
  Message(InterfaceItem *it = NULL);
  virtual ~Message();
  int ShowMessage(std::string msg);
};
//-------------------------------------------------------------------------------------------------------
class ProgressBar : public InterfaceItem {
public:
  ProgressBar(InterfaceItem *it = NULL);
  virtual ~ProgressBar();
  virtual void RedrawItem() override;

  void SetString(std::string str);
  void SetPercent(float p);

  float percent;   // прогресс в процентах от 0 до 100%
  int string_flag; // флаг отображения строки caption
};

class EditBox : public InterfaceItem {
public:
  EditBox(InterfaceItem *it = NULL);
  virtual ~EditBox();
  virtual int OnKeyDown(int c) override;
  virtual int OnType(int c) override;
  virtual void RedrawItem() override;
};
#endif
