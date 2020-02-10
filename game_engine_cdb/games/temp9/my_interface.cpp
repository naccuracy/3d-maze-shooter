#include"my_interface.h"


MyInterface::MyInterface(int w, int h, vectorFont *f):Interface(w,h,f)
{
    quitFlag=0;
    gamePause=1;
    newGame = 1;
}

MyInterface::~MyInterface()
{
 f1=NULL;
 f2=NULL;
 f3=NULL;
 font=NULL;
 mainMenu=NULL;
 loadingBar = NULL;
 loadingStatus = NULL;
}
int MyInterface::setLoadingProgress(int p, string msg)
{
    loadingBar->SetPercent(p);
    loadingStatus->caption = msg;
    return 1;
}
void MyInterface::Init(OptionsSet *o)
{
    caption="MyInterface";

    f1=new OptForm(this);
    f1->caption="Options";
    f1->SetSize(width-font->aprosh,height-font->aprosh);
    f1->Align(iiAlignCenter,iiAlignCenter);
    f1->Hide();
    f1->init(o);

    f2=new MyForm1(this);
    f2->caption="Game";
    f2->SetSize(width-font->aprosh,height-font->aprosh);
    f2->Align(iiAlignCenter,iiAlignCenter);
    f2->Hide();

    f3=new AboutForm(this);
    f3->caption="About";
    f3->SetSize(width-font->aprosh,height-font->aprosh);
    f3->Align(iiAlignCenter,iiAlignCenter);
    f3->Hide();

    mainMenu=new Menu(this);
    mainMenu->caption="mainMenu";
    vector<string> sl;
    sl.push_back("New Game");
    sl.push_back("Game");
    sl.push_back("Options");
    sl.push_back("About");
    sl.push_back("Quit");
    mainMenu->SetStringItems(sl);
    mainMenu->Align(iiAlignCenter,iiAlignCenter);
    mainMenu->Show();

    loadingBar = new ProgressBar(this);
    loadingBar->SetSize(width-font->aprosh*2,font_size*0.2);
    loadingBar->SetPos(font->aprosh,font_size+font->aprosh);
    loadingBar->Show();

    loadingStatus = new TextLabel(this);
    loadingStatus->caption = "loading resources...";
    //loadingStatus->SetSize(width-font->aprosh*2,font_size*1.5);
    loadingStatus->SetPos(font->aprosh,font->aprosh);
    loadingStatus->Show();
}

int MyInterface::OnMouseDown(int x, int y, int btn)
{
    int n=InterfaceItem::OnMouseDown(x,y,btn);
    if(child_focus>=0 && n)
    {
     if(childs[child_focus]==mainMenu)
     {
         if(mainMenu->child_focus==1)//Game
         {
             mainMenu->Hide();
             loadingBar->Hide();
             loadingStatus->Hide();
             child_focus=1;
             f2->Show();
             f2->focus=1;
             gamePause=0;
         }
         if(mainMenu->child_focus==0)//New Game
         {
             mainMenu->Hide();
             loadingBar->Hide();
             loadingStatus->Hide();
             child_focus=1;
             f2->Show();
             f2->focus=1;
             newGame = 1;
             gamePause=0;
         }
         if(mainMenu->child_focus==2)//Options
         {
             f1->Show();
             f1->focus=1;
             child_focus=0;
             mainMenu->Hide();
             loadingBar->Hide();
             loadingStatus->Hide();
             gamePause=1;
         }
         if(mainMenu->child_focus==3)//About
         {
             f3->Show();
             f3->focus=1;
             child_focus=2;
             mainMenu->Hide();
             loadingBar->Hide();
             loadingStatus->Hide();
             gamePause=1;
         }
         if(mainMenu->child_focus==4)//Quit
         {
             f1->Hide();
             f2->Hide();
             mainMenu->Hide();
             loadingBar->Hide();
             loadingStatus->Hide();
             quitFlag=1;
         }
     }
    }
    return n;
}
//
int MyInterface::OnKeyDown(int c)
{
    InterfaceItem::OnKeyDown(c);
    //if(!f1->visible && !f2->visible)
    if(c==27)//ESC
    {
      mainMenu->Show();
      gamePause=1;
    }
    return 0;
}
