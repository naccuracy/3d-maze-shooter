//  ☀️
#include "interface.h"
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>

//--------------------------------------------------------------------------------------------
void color4fStruct::Set(float red, float green, float blue, float alfa) {
  r = red;
  g = green;
  b = blue;
  a = alfa;
  return;
}
//--------------------------------------------------------------------------------------------
InterfaceItem::InterfaceItem(InterfaceItem *prnt) {
  caption = "Caption";
  x_pos = 0;
  y_pos = 0;
  width = 80;
  height = 40;
  selected = 0;
  focus = 0;        // 0 no focus; 1 has focus
  child_focus = -1; // no one child has focus
  parent = prnt;
  if (parent != NULL) {
    parent->childs.push_back(this);
    font = parent->font;
    bkg_clr = parent->bkg_clr;
    fnt_clr = parent->fnt_clr;
    slct_clr = parent->slct_clr;
    frm_clr = parent->frm_clr;
    visible = parent->visible;
    font_size = parent->font_size;
  } else {
    font = NULL;
    bkg_clr.Set(0.8, 0.8, 0.8, 0.8);
    fnt_clr.Set(0.0, 0.0, 0.0, 1.0);
    slct_clr.Set(1.0, 0.5, 0.0, 1.0);
    frm_clr.Set(0.0, 0.0, 1.0, 1.0);
    visible = 1;
    font_size = 30;
  }
}

void InterfaceItem::SetPos(int x, int y) {
  x_pos = x;
  y_pos = y;
  return;
}

void InterfaceItem::SetSize(int w, int h) {
  width = w;
  height = h;
  OnSize(w, h);
  return;
}

InterfaceItem::~InterfaceItem() {
  parent = NULL;
  font = NULL;
  for (unsigned int i = 0; i < childs.size(); i++) {
    delete childs[i];
  }
  childs.clear();
}

InterfaceItem *InterfaceItem::GetParent() { return parent; }

/// RedrawItem
// метод перерисовывает сначала всех потомков элемента не имеющих фокус
// затем перерисовывает потомка имеющего фокус
// сам элемент невидимый - так как не имеет кода отрисовки
// в классах наследниках предполагается в переопределенном методе
// сначала перерисовать сам экземпляр
// элемента интерфейса а затем вызвать этот метод для корректного отображения
// всех элементов интерфейса
void InterfaceItem::RedrawItem() {
  if (!visible)
    return;
  int n;
  for (unsigned int i = 0; i < childs.size(); i++) {
    n = i;
    if (n != child_focus && childs[i]->visible)
      childs[i]->RedrawItem();
  }

  if (child_focus >= 0)
    childs[child_focus]->RedrawItem();
  return;
}

//
int InterfaceItem::OnMouseDown(int x, int y, int btn) // 1-l 2-r 3-m
{
  int n = 0;
  if (!visible)
    return 0;
  if (x_pos <= x && x <= x_pos + width && y_pos <= y && y <= y_pos + height) {
    focus = 1;
    child_focus = -1;
    for (unsigned int i = 0; i < childs.size(); i++) {
      // позиция потомка отсчитывается от нижнего левого угла родителя!
      n = childs[i]->OnMouseDown(x - x_pos, y - y_pos, btn);
      if (n == 1) {
        child_focus = i;
      }
    }

  } else
    focus = 0;
  return focus;
}

// просто передает сообщение всем дочерним элементам
int InterfaceItem::OnMouseUp(int x, int y, int btn) {
  if (!visible)
    return 0;
  for (unsigned int i = 0; i < childs.size(); i++) {
    childs[i]->OnMouseUp(x - x_pos, y - y_pos, btn);
  }
  return 0;
}

// передает сообщение всем дочерним элементам
int InterfaceItem::OnType(int ch) {
  int n = childs.size();
  int i;
  if (!visible)
    return 0;
  if (focus)
    for (i = 0; i < n; i++) {
      if (i == child_focus && childs[i]->focus)
        childs[i]->OnType(ch);
    }
  return 0;
}

// передает сообщение потомку, имеющему фокус, если сам элемент находится в
// фокусе ввода
int InterfaceItem::OnKeyDown(int c) {
  int n = childs.size();
  int i;
  if (!visible)
    return 0;
  if (focus)
    for (i = 0; i < n; i++) {
      if (i == child_focus && childs[i]->focus)
        childs[i]->OnKeyDown(c);
    }
  return 0;
}

// определить!
int InterfaceItem::OnKeyUp(int c) {
  if (!visible)
    return 0;
  return 0;
}

// определить
int InterfaceItem::OnIdle() {
  if (!visible)
    return 0;
  return 0;
}

// реакция на сообщение системы изменить размер
// дописать передачу сообщения потомкам
int InterfaceItem::OnSize(int w, int h) {
  width = w;
  height = h;
  return 0;
}

//
int InterfaceItem::OnMouseMove(int x, int y) {
  if (!visible)
    return 0;
  if (x_pos <= x && x <= x_pos + width && y_pos <= y && y <= y_pos + height) {
    selected = 1;
    for (unsigned int i = 0; i < childs.size(); i++)
      childs[i]->OnMouseMove(x - x_pos, y - y_pos);
  } else
    selected = 0;
  return 0;
}

// Делает элемент видимым
void InterfaceItem::Show() {
  visible = 1;
  return;
}

// Делает элемент невидимым
void InterfaceItem::Hide() {
  visible = 0;
  return;
}

// отображает строку текста в текущей точке растра
void InterfaceItem::RenderString(std::string str) {
  int X = 0, Y = 0, i, j, px, py, len, fs;
  unsigned int k;
  fs = font->font_size;
  font->SetSize(font_size);
  len = str.length();
  glLineWidth(font->l_width);
  for (i = 0; i < len; i++) {
    j = (int)(unsigned char)str[i];
    if (j != 32 && font->symbols[j].size()) {
      glBegin(GL_LINES);
      for (k = 0; k < font->symbols[j].size(); k++) {
        if (k % 2 == 1) {
          px = (int)(font->symbols[j][k - 1].x * (float)font_size) + X;
          py = Y - (int)(font->symbols[j][k - 1].y * (float)font_size) +
               font_size;
          glVertex3d(px, py, 0.0);
          px = (int)(font->symbols[j][k].x * (float)font_size) + X;
          py = Y - (int)(font->symbols[j][k].y * (float)font_size) + font_size;
          glVertex3d(px, py, 0.0);
        }
      }
      glEnd();
      X = X + (int)((float)font->font_size * font->s_width[j]) + font->aprosh;
    }
    if (j == 32)
      X += (int)(font->s_width[32] * (float)font->font_size) + font->aprosh;
    // glPopMatrix();
  }
  glLineWidth(1);
  font->SetSize(fs);
  return;
}

// calculate size of std::string str in pixels
int InterfaceItem::SizeOfString(std::string str) {
  int n = str.size();
  int j;
  int i, res = 0, fs = font->font_size;
  font->SetSize(font_size);
  for (i = 0; i < n; i++) {
    j = (int)(unsigned char)str[i];
    res += (int)(font->font_size * font->s_width[j] + font->aprosh);
  }
  res -= font->aprosh;
  font->SetSize(fs);
  return res;
}

void InterfaceItem::Align(int xflag, int yflag) {
  if (parent) {
    int ap = parent->font->aprosh;

    if (xflag == iiAlignCenter)
      x_pos = (parent->width - width) / 2;
    if (xflag == iiAlignMin)
      x_pos = ap;
    if (xflag == iiAlignMax)
      x_pos = parent->width - width - ap;

    if (yflag == iiAlignCenter)
      y_pos = (parent->height - height) / 2;
    if (yflag == iiAlignMin)
      y_pos = ap;
    if (yflag == iiAlignMax)
      y_pos = parent->height - height - ap;
  }
  return;
}
//--------------------------------------------------------------------------------------------
Interface::Interface(int w, int h, vectorFont *f) : InterfaceItem() {
  SetPos(0, 0);
  SetSize(w, h);
  font = f;
}
//
int Interface::OnSize(int w, int h) {
  InterfaceItem::OnSize(w, h);
  return 0;
}
//
Interface::~Interface() {}
//
void Interface::RedrawItem() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  InterfaceItem::RedrawItem();
  return;
}
//--------------------------------------------------------------------------------------------
TextLabel::TextLabel(InterfaceItem *it) : InterfaceItem(it) {}
//
void TextLabel::SetCaption(std::string str) {
  caption = str;
  int fs = font->font_size;
  font->SetSize(font_size);
  width = SizeOfString(caption) + font->aprosh * 2;
  height = font_size + font->aprosh * 2;
  font->SetSize(fs);
  return;
}
//
TextLabel::~TextLabel() {}
//
void TextLabel::RedrawItem() {
  int fs = font->font_size;
  font->SetSize(font_size);
  width = SizeOfString(caption) + 2 * font->aprosh;
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(x_pos, y_pos, 0);

  glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
  if (focus || selected) {
    glColor4f(slct_clr.r, slct_clr.g, slct_clr.b, slct_clr.a);
  }

  glLineWidth(font->l_width);
  glBegin(GL_LINES);
  glVertex2d(0, 0);
  glVertex2d(width, 0);
  glEnd();
  // text
  glTranslated(font->aprosh, font->aprosh, 0);
  RenderString(caption);
  font->SetSize(fs);
  glPopMatrix();
  return;
}
//----------------------------------------------------------------------------------------------------
Form::Form(InterfaceItem *it, int esc_key_code)
    : InterfaceItem(it), ESC_KEY_CODE(esc_key_code) {}
//
Form::~Form() {}
int Form::OnKeyDown(int c) {
  if (c == ESC_KEY_CODE) {
    Hide();
    return 0;
  }
  return InterfaceItem::OnKeyDown(c);
}
//
void Form::RedrawItem() {
  int fs = font->font_size;
  if (visible) {

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(x_pos, y_pos, 0);
    InterfaceItem::RedrawItem();
    // frame
    font->SetSize(font_size);
    glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
    if (focus || selected) {
      glColor4f(slct_clr.r, slct_clr.g, slct_clr.b, slct_clr.a);
    }
    glLineWidth(font->l_width);
    int a, b;
    a = font->font_size + font->aprosh;
    b = height - 2 * font->aprosh - font_size;
    glBegin(GL_LINES);
    glVertex2d(0, 0);
    glVertex2d(width, 0);
    glVertex2d(0, 0);
    glVertex2d(0, height);
    glVertex2d(width, 0);
    glVertex2d(width, height);
    glVertex2d(0, height);
    glVertex2d(width, height);

    glVertex2d(0, a);
    glVertex2d(width, a);

    glVertex2d(0, b);
    glVertex2d(width, b);
    glEnd();
    // text
    std::string status = "Esc - hide";
    glTranslated(font->aprosh, font->aprosh, 0);
    RenderString(status);

    glTranslated(0, height - 2 * font->aprosh - font_size, 0);
    RenderString(caption);

    glPopMatrix();
    font->SetSize(fs);
  }
  return;
}
//-------------------------------------------------------------------------
// constructor
Button::Button(InterfaceItem *it) : InterfaceItem(it) { mousebtn = 0; }
// destructor
Button::~Button() {}
//
void Button::SetCaption(std::string str) {
  caption = str;
  int fs = font->font_size;
  font->SetSize(font_size);
  width = SizeOfString(caption) + font->aprosh * 2;
  height = font_size + font->aprosh * 2;
  font->SetSize(fs);
  return;
}

void Button::RedrawItem() {
  int fs = font->font_size;
  font->SetSize(font_size);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(x_pos, y_pos, 0);
  /*
    //fon
    glColor4f(bkg_clr.r, bkg_clr.g, bkg_clr.b, bkg_clr.a);
    glBegin(GL_QUADS);
       glVertex2d(0,0);
       glVertex2d(width,0);
       glVertex2d(width,height);
       glVertex2d(0,height);
    glEnd();
  */
  // frame
  glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
  if (focus || selected) {
    if (mousebtn)
      glColor4f(1.0, 0.6, 0.0, 1.0);
    else
      glColor4f(slct_clr.r, slct_clr.g, slct_clr.b, slct_clr.a);
  }
  glLineWidth(font->l_width);
  glBegin(GL_LINES);
  glVertex2d(0, 0);
  glVertex2d(width, 0);
  glVertex2d(0, 0);
  glVertex2d(0, height);
  glVertex2d(width, 0);
  glVertex2d(width, height);
  glVertex2d(0, height);
  glVertex2d(width, height);
  glEnd();
  // text
  glTranslated(font->aprosh, font->aprosh, 0);
  RenderString(caption);
  font->SetSize(fs);
  glPopMatrix();
  return;
}

int Button::OnMouseDown(int x, int y, int btn) // 1-l 2-r 3-m
{
  int res = InterfaceItem::OnMouseDown(x, y, btn);
  if (focus && btn == 1) {
    mousebtn = 1;
  }
  return res;
}

int Button::OnMouseUp(int x, int y, int btn) {
  InterfaceItem::OnMouseUp(x, y, btn);
  if (focus && btn == 1) {
    mousebtn = 0;
  }
  return 0;
}

int Button::OnKeyDown(int c) {
  if (c == 32) {
    mousebtn = 0;
    return 0;
  }
  return InterfaceItem::OnKeyDown(c);
}

//---------------------------------------------------------------------------------------------
Menu::Menu(InterfaceItem *it) : InterfaceItem(it) {
  // item_number=-1;
}

Menu::~Menu() {}

int Menu::OnMouseDown(int x, int y, int btn) {
  int res = -1;
  if (selected) {
    res = InterfaceItem::OnMouseDown(x, y, btn);
  }
  return res;
}

void Menu::RedrawItem() {
  if (visible) {
    font->SetSize(font_size);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(x_pos, y_pos, 0);
    InterfaceItem::RedrawItem();
    // frame
    glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
    // if(focus || selected)
    //{
    //     glColor4f(slct_clr.r,slct_clr.g,slct_clr.b,slct_clr.a);
    // }
    // else glColor4f(frm_clr.r,frm_clr.g,frm_clr.b,frm_clr.a);
    glLineWidth(font_size / 18.0);
    glBegin(GL_LINES);
    glVertex2d(0, 0);
    glVertex2d(width, 0);
    glVertex2d(0, 0);
    glVertex2d(0, height);
    glVertex2d(width, 0);
    glVertex2d(width, height);
    glVertex2d(0, height);
    glVertex2d(width, height);
    glEnd();
    glPopMatrix();
  }
  return;
}

int Menu::SetStringItems(std::vector<std::string> strs) {
  TextLabel *tl = NULL;
  int fs = font->font_size;
  font->SetSize(font_size);
  int n = strs.size();
  height = n * (font_size + font->aprosh) + font->aprosh * 2;
  int wdz = 0, sos = 0, i;
  for (i = 0; i < n; i++) {
    sos = SizeOfString(strs[i]) + font->aprosh * 2;
    if (wdz <= sos)
      wdz = sos;
  }
  width = wdz;
  for (i = 0; i < n; i++) {
    tl = new TextLabel(this);
    tl->SetCaption(strs[i]);
    tl->Show();
    tl->SetPos(0, height - (i + 1) * (font_size + font->aprosh));
    tl->Align(iiAlignCenter, iiNoAlign);
  }
  font->SetSize(fs);
  tl = NULL;
  return 0;
}

//--------------------------------------------------------------------------------------------------------------
Message::Message(InterfaceItem *it) : Form(it) {
  caption = "Message";
  float w = SizeOfString(caption);
  float h = font_size * 3.0;
  SetSize(w, h);
  Align(iiAlignCenter, iiAlignCenter);
  visible = 0;
}

Message::~Message() {}

int Message::ShowMessage(std::string mssg) {
  caption = mssg;
  float w = SizeOfString(caption);
  float h = font_size * 3.0;
  SetSize(w, h);
  Align(iiAlignCenter, iiAlignCenter);
  Show();
  return 0;
}
//--------------------------------------------------------------------------------------------------------------
ProgressBar::ProgressBar(InterfaceItem *it) : InterfaceItem(it) {
  percent = 0.0;
  string_flag = 0;
}

ProgressBar::~ProgressBar() {}

void ProgressBar::RedrawItem() {
  if (visible) {
    // calc str length in pixels
    int n = caption.size();
    font->SetSize(font_size);
    int str_len = SizeOfString(caption);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(x_pos, y_pos, 0);
    // frame
    glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
    glLineWidth(font->l_width);
    glBegin(GL_LINES);
    glVertex2d(0, 0);
    glVertex2d(width, 0);
    glVertex2d(0, 0);
    glVertex2d(0, height);
    glVertex2d(width, 0);
    glVertex2d(width, height);
    glVertex2d(0, height);
    glVertex2d(width, height);
    glEnd();
    // progress
    glColor4f(slct_clr.r, slct_clr.g, slct_clr.b, slct_clr.a);
    float pr_pix = (percent * (float)width / 100.0);
    glBegin(GL_QUADS);
    glVertex2d(0, 0);
    glVertex2d(pr_pix, 0);
    glVertex2d(pr_pix, height);
    glVertex2d(0, height);
    glEnd();
    // std::string
    glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
    if (string_flag) {
      std::string temp_str;
      // int too_big_flag=0, i;
      temp_str.clear();
      int i = (width - str_len) / 2; // позиция начала строки
      if (str_len > width) {
        n = width / (font->s_width[32] + font->aprosh);
        for (i = 0; i < n; i++)
          temp_str.push_back(caption[i]);
        i = font->aprosh;
        // too_big_flag=1;
      } else
        temp_str = caption;
      glTranslated(i, (height - font_size) / 2, -1);
      RenderString(temp_str);
    }
    glPopMatrix();
  }
  return;
}

void ProgressBar::SetString(std::string str) {
  caption = str;
  return;
}

void ProgressBar::SetPercent(float p) {
  if (p > 100.0)
    percent = 100.0;
  else
    percent = p;
  return;
}
//-------------------------------------------------------------------------------------
EditBox::EditBox(InterfaceItem *it) : InterfaceItem(it) { caption = ""; }

EditBox::~EditBox() {}
int EditBox::OnType(int c) {
  font->SetSize(font_size);
  int ml =
      SizeOfString(caption) + font->aprosh * 2 + font->s_width[32] * font_size;
  if (c >= 32 && c <= 255 && ml <= width)
    caption.push_back((unsigned char)c);
  return InterfaceItem::OnType(c);
}
int EditBox::OnKeyDown(int c) {
  if (c == 8 && caption.length()) {
    std::string temp;
    for (int i = 0; i < (int)caption.length() - 1; i++)
      temp.push_back(caption[i]);
    caption = temp;
  }
  return 0;
}

void EditBox::RedrawItem() {
  if (!visible)
    return;
  int fs = font->font_size;
  font->SetSize(font_size);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(x_pos, y_pos, 0);

  glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
  if (focus || selected) {
    glColor4f(slct_clr.r, slct_clr.g, slct_clr.b, slct_clr.a);
  }
  // frame
  glLineWidth(font->l_width);
  glBegin(GL_LINES);
  glVertex2d(0, 0);
  glVertex2d(width, 0);
  glVertex2d(width, 0);
  glVertex2d(width, height);
  glVertex2d(width, height);
  glVertex2d(0, height);
  glVertex2d(0, height);
  glVertex2d(0, 0);
  glEnd();

  // text
  glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
  glTranslated(font->aprosh, font->aprosh, 0);
  RenderString(caption);

  font->SetSize(fs);

  glPopMatrix();
  return;
}

ImageLabel::ImageLabel(InterfaceItem *it) : InterfaceItem(it) {
  tex_index = 21; // нет текстуры
}
ImageLabel::~ImageLabel() {}
void ImageLabel::RedrawItem() {
  if (!visible)
    return;
  int fs = font->font_size;
  font->SetSize(font_size);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslated(x_pos, y_pos, 0);

  glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
  if (focus || selected) {
    glColor4f(slct_clr.r, slct_clr.g, slct_clr.b, slct_clr.a);
  }
  // frame
  /*glLineWidth(font->l_width);
  glBegin(GL_LINES);
      glVertex2d(0,0);
      glVertex2d(width,0);
      glVertex2d(width,0);
      glVertex2d(width,height);
      glVertex2d(width,height);
      glVertex2d(0,height);
      glVertex2d(0,height);
      glVertex2d(0,0);
  glEnd();*/
  // image
  if (tex_index != 21) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_index);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2d(0, 0);
    glTexCoord2f(1, 0);
    glVertex2d(width, 0);
    glTexCoord2f(1, 1);
    glVertex2d(width, height);
    glTexCoord2f(0, 1);
    glVertex2d(0, height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  }
  // text
  else {
    glColor4f(fnt_clr.r, fnt_clr.g, fnt_clr.b, fnt_clr.a);
    glTranslated(font->aprosh, font->aprosh, 0);
    RenderString(caption);
    font->SetSize(fs);
  }
  glPopMatrix();
}
void ImageLabel::SetImage(
    int ix) // передается номер уже проинициализированной текстуры))))
{
  tex_index = ix;
}
