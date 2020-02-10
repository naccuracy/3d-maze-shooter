#ifndef OPT_FORM_H_INCLUDED
#define OPT_FORM_H_INCLUDED

#include"../../src/2D/interface.h"
#include"../../src/utils/options.h"

class OptForm: public Form
{
    public:
        OptForm(InterfaceItem *it=NULL);
        ~OptForm();
        void RedrawItem();

        TextLabel *lengthLabel;
        TextLabel *heightLabel;
        TextLabel *mapLabel;
        TextLabel *fontLabel;
        TextLabel *statusLabel;

        EditBox *lengthBox;
        EditBox *heightBox;
        EditBox *mapBox;
        EditBox *fontBox;

        Button* saveBtn;
        Button* cancelBtn;

        void ShowOptionsList(void);
        void SaveClick();
        void CancelClick();
        int OnSize(int w, int h);
        int OnMouseDown(int x, int y, int btn);//1-l 2-r 3-m
        void init(OptionsSet *ptr);
        private:
        OptionsSet *os;
};


#endif // OPT_FORM_H_INCLUDED
