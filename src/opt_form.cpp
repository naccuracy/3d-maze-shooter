//  ☀️
#include "opt_form.h"
#include <string>
#include <vector>

OptForm::OptForm(InterfaceItem *it, int esc_key_code) : Form(it, esc_key_code) {
  saveBtn = new Button(this);
  saveBtn->SetCaption("Save");
  saveBtn->Show();

  cancelBtn = new Button(this);
  cancelBtn->SetCaption("Cancel");
  cancelBtn->Show();

  lengthLabel = new TextLabel(this);
  lengthLabel->caption = "Width";
  lengthLabel->Show();

  lengthBox = new EditBox(this);
  lengthBox->caption = "0.0";
  lengthBox->Show();

  heightLabel = new TextLabel(this);
  heightLabel->caption = "Height";
  heightLabel->Show();

  heightBox = new EditBox(this);
  heightBox->caption = "0.0";
  heightBox->Show();

  fontLabel = new TextLabel(this);
  fontLabel->caption = "Font";
  fontLabel->Show();

  fontBox = new EditBox(this);
  fontBox->caption = "0.0";
  fontBox->Show();

  mapLabel = new TextLabel(this);
  mapLabel->caption = "Level";
  mapLabel->Show();

  mapBox = new EditBox(this);
  mapBox->caption = "0.0";
  mapBox->Show();

  statusLabel = new TextLabel(this);
  statusLabel->caption = "Options v1.1";
  statusLabel->Show();
}
int OptForm::OnSize(int w, int h) {
  Form::OnSize(w, h);
  int l = 350;
  int fa2 = font->aprosh * 2;
  int x_c = fa2 + w / 2 - l;
  int y_c = fa2 - 1.5 * font->font_size + h / 2;
  saveBtn->SetPos(x_c, y_c);
  saveBtn->SetSize(l, font_size + fa2);
  x_c += l + fa2;
  cancelBtn->SetPos(x_c, y_c);
  cancelBtn->SetSize(l, font_size + fa2);
  x_c = fa2 + w / 2 - l;
  y_c += saveBtn->height + fa2;
  lengthLabel->SetPos(x_c, y_c);
  lengthLabel->SetSize(l, font_size + fa2);
  x_c += l + fa2;
  lengthBox->SetSize(l, font_size + fa2);
  lengthBox->SetPos(x_c, y_c);
  x_c = fa2 + w / 2 - l;
  y_c += lengthLabel->height + fa2;
  heightLabel->SetPos(x_c, y_c);
  heightLabel->SetSize(l, font_size + fa2);
  x_c += l + fa2;
  heightBox->SetSize(l, font_size + fa2);
  heightBox->SetPos(x_c, y_c);
  x_c = fa2 + w / 2 - l;
  y_c += heightLabel->height + fa2;
  fontLabel->SetPos(x_c, y_c);
  fontLabel->SetSize(l, font_size + fa2);
  x_c += l + fa2;
  fontBox->SetSize(l, font_size + fa2);
  fontBox->SetPos(x_c, y_c);
  x_c = fa2 + w / 2 - l;
  y_c += fontLabel->height + fa2;
  mapLabel->SetPos(x_c, y_c);
  mapLabel->SetSize(l, font_size + fa2);
  x_c += l + fa2;
  mapBox->SetPos(x_c, y_c);
  mapBox->SetSize(l, font_size + fa2);
  x_c = fa2;
  y_c = fa2 + font->font_size;
  statusLabel->SetPos(x_c, y_c);
  statusLabel->SetSize(width, font_size + fa2);
  return 1;
}
int OptForm::OnMouseDown(int x, int y, int btn) {
  int res = Form::OnMouseDown(x, y, btn);
  if (res > 0 && child_focus >= 0) {
    if (childs[child_focus] == saveBtn)
      SaveClick();
    if (childs[child_focus] == cancelBtn)
      CancelClick();
  }
  return res;
}
void OptForm::init(OptionsSet *ptr) {
  os = ptr;
  ShowOptionsList();
}
void OptForm::ShowOptionsList(void) {
  if (os) {
    lengthBox->caption = os->Value("width");
    heightBox->caption = os->Value("height");
    mapBox->caption = os->Value("map");
    fontBox->caption = os->Value("font");
  } else
    fontBox->caption = "FONT";
  return;
}
OptForm::~OptForm() { os = NULL; }
void OptForm::RedrawItem() {
  Form::RedrawItem();
  return;
}

void OptForm::SaveClick() {
  os->setValue("width", lengthBox->caption);
  os->setValue("height", heightBox->caption);
  os->setValue("map", mapBox->caption);
  os->setValue("font", fontBox->caption);
  os->saveFile((char *)"settings.csv");
  statusLabel->caption = "press Save and restart game!";
  return;
}
void OptForm::CancelClick() {
  ShowOptionsList();
  return;
}
