#include"options.h"
#include<stdio.h>
//#include<windows.h>

OptionsSet::OptionsSet()
{
 options.clear();
 return;
}

OptionsSet::~OptionsSet()
{
 options.clear();
 return;
}

int OptionsSet::openFile(char* fn)
{
 FILE* f=NULL;
 f=fopen(fn, "rt");
 if(f == NULL)return 1;
 options.clear();

 Option o;
 char str1[128], str2[128];
 int i, j, n;

 fscanf(f, "%d;\n", &n);
 for(i = 0; i < n; i++)
 {
  for(j = 0; j < 128; j++)
  {
   str1[j]=0;str2[j]=0;
  }

  j=0;
  do{
  str1[j]=fgetc(f);
  j++;
  }while(str1[j-1]!=';');
  str1[j-1]=0;

  j=0;
  do{
  str2[j]=fgetc(f);
  j++;
  }while(str2[j-1]!=';');
  str2[j-1]=0;
  fgetc(f);

  o.Name = str1;
  o.Value = str2;
  options.push_back(o);
  //MessageBox(NULL,str1,str2,0);
 }

 fclose(f);
 return 0;
}

int OptionsSet::saveFile(char* fn)
{
 FILE* f=NULL;
 f=fopen(fn,"wt");
 if(f == NULL)return 1;
 fprintf(f, "%d;\n", options.size());

 unsigned int i;
 for(i = 0; i < options.size(); i++)
 {
  fprintf(f, "%s;%s;\n", options[i].Name.c_str(), options[i].Value.c_str());
 }

 fclose(f);
 return 0;
}

string OptionsSet::Value(string Name)
{
 string str="";
 for(unsigned int i = 0; i < options.size(); i++)
 {
  if(options[i].Name.compare(Name) == 0)
  {
   str = options[i].Value;
   break;
  }
 }
 return str;
}

int OptionsSet::setValue(string Name, string Val)
{
    int res = 0;//если не найдено опции с именем Name то результат 0
    for(unsigned int i = 0; i < options.size(); i++)
    {
        if(options[i].Name.compare(Name) == 0)
        {
            options[i].Value = Val;
            res = 1;//если опция найдена то изменяем ее значение и возвращаем 1
            break;
        }
    }
    return res;
}
