//  ☀️
#include "my_form1.h"
#include <GL/gl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

MyForm1::MyForm1(InterfaceItem *it, int esc_key_code) : Form(it, esc_key_code) {
  mmvisible = 1;
  edit1 = new EditBox(this);

  edit2 = new EditBox(this);
  edit2->fnt_clr.Set(1.0, 0.1, 0.0, 0.8);
  edit2->font_size = 100;
  edit2->caption = "You Win";
  edit2->Hide();
  buletsImg = new ImageLabel(this);
  buletsImg->caption = "b";
  buletsImg->Show();
  buletsLabel = new TextLabel(this);
  buletsLabel->caption = "0";
  buletsLabel->Show();
  livesImg = new ImageLabel(this);
  livesImg->caption = "l";
  livesImg->Show();
  livesLabel = new TextLabel(this);
  livesLabel->caption = "0";
  livesLabel->Show();
  deadsImg = new ImageLabel(this);
  deadsImg->caption = "d";
  deadsImg->Show();
  deadsLabel = new TextLabel(this);
  deadsLabel->caption = "0";
  deadsLabel->Show();
  keysImg = new ImageLabel(this);
  keysImg->caption = "d";
  keysImg->Show();
  keysLabel = new TextLabel(this);
  keysLabel->caption = "0";
  keysLabel->Show();
  aim = 1;
  map_tex_id = 0;
}

MyForm1::~MyForm1() {
  edit1 = NULL;
  edit2 = NULL;
}

void MyForm1::RedrawItem() {
  int size = 20;
  if (visible) {
    InterfaceItem::RedrawItem();
    if (aim) {
      // курсор
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glTranslatef(x_pos + width / 2 - size / 2, y_pos + height / 2 - size / 2,
                   0);
      glColor4f(1.0, 0.5, 0.5, 0.5);
      glLineWidth(3);
      glBegin(GL_LINES);
      glVertex2d(0, size);
      glVertex2d(0, 0);
      glVertex2d(0, 0);
      glVertex2d(size, 0);
      glVertex2d(size, 0);
      glVertex2d(size, size);
      glVertex2d(size, size);
      glVertex2d(0, size);

      glVertex2d(size / 2.0, -size / 2.0);
      glVertex2d(size / 2.0, size / 3.0);
      glVertex2d(size / 2.0, size * 3.0 / 2.0);
      glVertex2d(size / 2.0, size * 2.0 / 3.0);

      glVertex2d(-size / 2.0, size / 2.0);
      glVertex2d(size / 3.0, size / 2.0);
      glVertex2d(size * 3.0 / 2.0, size / 2.0);
      glVertex2d(size * 2.0 / 3.0, size / 2.0);
      glEnd();
      glPopMatrix();
      glLineWidth(1);
    }
    if (mmvisible) {
      glPushMatrix();
      glLoadIdentity();
      glTranslatef(mmposx, mmposy, 0);
      // миникарта
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, map_tex_id);
      glColor4f(1.0, 1.0, 1.0, 1.0);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex2d(0, 0);
      glTexCoord2f(1, 0);
      glVertex2d(mmwidth, 0);
      glTexCoord2f(1, 1);
      glVertex2d(mmwidth, mmheight);
      glTexCoord2f(0, 1);
      glVertex2d(0, mmheight);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      // рамка миникарты
      glColor4f(1.0, 0.5, 0.5, 0.5);
      glLineWidth(3);
      glBegin(GL_LINES);
      glVertex2d(0, mmheight);
      glVertex2d(0, 0);
      glVertex2d(0, 0);
      glVertex2d(mmwidth, 0);
      glVertex2d(mmwidth, 0);
      glVertex2d(mmwidth, mmheight);
      glVertex2d(mmwidth, mmheight);
      glVertex2d(0, mmheight);
      glEnd();
      glPopMatrix();
      glLineWidth(1);
    }
  }
  return;
}
int MyForm1::OnSize(int w, int h) {
  int mrg = font->aprosh * 2;
  int x_c = mrg;
  int y_c = height - mrg - font->font_size;
  int res = Form::OnSize(w, h);

  edit2->fnt_clr.Set(1.0, 0.1, 0.0, 0.8);
  edit2->font_size = 100;
  edit2->SetSize(600, 110);
  edit2->SetPos(width / 2 - 300, height / 2 - 55);
  edit2->Hide();

  buletsImg->SetSize(font_size, font_size);
  buletsImg->SetPos(x_c, y_c);
  x_c += font_size + mrg;
  buletsLabel->SetPos(x_c, y_c);
  x_c += font_size * 3 + mrg;
  livesImg->SetSize(font_size, font_size);
  livesImg->SetPos(x_c, y_c);
  x_c += font_size + mrg;
  livesLabel->SetPos(x_c, y_c);
  x_c += font_size * 3 + mrg;
  deadsImg->SetSize(font_size, font_size);
  deadsImg->SetPos(x_c, y_c);
  x_c += font_size + mrg;
  deadsLabel->SetPos(x_c, y_c);
  x_c += font_size * 3 + mrg;
  keysImg->SetSize(font_size, font_size);
  keysImg->SetPos(x_c, y_c);
  x_c += font_size + mrg;
  keysLabel->SetPos(x_c, y_c);

  // миникарта
  mmwidth = width / 4 - mrg * 2;
  mmheight = height / 4 - mrg * 2;
  mmposx = width - mmwidth;
  mmposy = mrg;

  edit1->SetSize(mmposx - 2 * mrg, font_size + font->aprosh * 2);
  edit1->SetPos(mrg, mrg);
  edit1->Hide();
  return res;
}
int MyForm1::OnMouseDown(int x, int y, int btn) {
  int res = Form::OnMouseDown(x, y, btn);
  return res;
}

void MyForm1::updateBuletsCount(int b) {
  char str[8];
  sprintf(str, "%d", b);
  buletsLabel->caption = str;
}
void MyForm1::updateLivesCount(int l) {
  char str[8];
  sprintf(str, "%d", l);
  livesLabel->caption = str;
}
void MyForm1::updateDeadsCount(int d) {
  char str[8];
  sprintf(str, "%d", d);
  deadsLabel->caption = str;
}

void MyForm1::updateKeysCount(int b) {
  char str[8];
  sprintf(str, "%d", b);
  keysLabel->caption = str;
}
