#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
//#include "../../src/openAL/al.h"
//#include "../../src/openAL/alc.h"
//#include "../../src/openAL/CWaves.h"
#include "../../src/utils/options.h"
#include "../../src/utils/bmpreader.h"
#include "../../src/3D/simple_mesh.h"
#include "my_interface.h"
#include <GL/gl.h>
#include <GL/glu.h>

#define tex_count 11
#define wall_tex 0
#define flour_tex 1
#define ava_tex 2
#define day_tex 3
#define night_tex 4
#define hunter_tex 5
#define bulet_tex 6
#define dead_tex 7
#define live_tex 8
#define map_tex 9
#define key_pic 10
#define map_tex_size 256

#define sounds 9
#define theme_sound 0
#define gameover_sound 1
#define win_sound 2
#define bah_sound 3
#define smert_sound 4
#define pick_med 5
#define pick_ammo 6
#define lose_live_sound 7
#define kick_sound 8

struct bot
{
  Frame actor;
  int alive;
  float velocity;
  int rotate_flag;//1 - to the left 0 - to the right
  float time;
  float rotate_time;
};
class Game
{

public:
Game();
~Game();
int debug_ind;
int mousex;

int Score;
int Keys_picked;
int Bul_count;
int Live_count;
int inited;
OptionsSet os;
char debug_str[256];
int time, maxtime;
int day_time_flag;
int LN, LM;
int **LABIRINT;
vector<Point3f> deny;//������ ��� ����� ��������� ������� ����
vector<Point3f> win;//������ ����� ���������� ������ �� ������ �� ���������
vector<Point3f> yama;//���
int gameover;

GLbyte *map_tex_ptr;

GLuint myTexture[tex_count];
int bot_flag;
int bot_count;
int med_count;//���������� �������
int amm_count;//���������� ������ � ���������
int bblk_count;//���������� ����������� ������
int ublk_count;//���������� ������������� ������
int sbblk_count;//���������� ������������ ������
int keys_count;
int doors_count;
int mblk_count;//���������� ������������� ������
bot *bots, bulet, *medpaks, *ammunitions;//val on map: bot = 4 medpak = 5 ammunition = 6
bot *bad_blocks;//����� ������� ��������
bot *up_blocks;//����� ������� �����������
bot *keys;//����� ������� ����� ��� ������ �� ���������
bot *doors;//����� ������� ����������� ����� ���������� �������
bot *mblocks;//
int cam_flag;
int keyboard[255];//��������� ������ 0 - �� ������ 1 - ������//����� �� ������ 255
//����
GLfloat mat_specular[4];
GLfloat mat_shininess[1];
Point3f light_position;
GLfloat white_light[4];
GLfloat grey_light[4];
/*  Make the class name into a global variable  */

vectorFont font1;
MyInterface *intr;
SimpleMesh sm, sm1, nebo, bulet_mesh, hrt, ammo, key;
Frame cam0;//������ �� ������� ����
Frame cam1;//������ ��� ���������
Frame bird;//������ � ������ �������� ������
Point3f cam0pos0;
bool mainloopActive;
int win_flag;
int falling;//���� 1 �� �������
int fallingTime;//����� �������
int fallingVelocity;//�������� �������
int fallingEnd;
//sound variables
//bool SoundFlag;
//ALCdevice *SoundDevice;
//ALCcontext *SoundContext;
//ALuint SoundBuffers[sounds],SoundSources[sounds];
//CWaves *g_pWaveLoader;
//�������� �������
//ALboolean ALFWLoadWaveToBuffer(const char *szWaveFile, ALuint uiBufferID);
void CameraMovement();//��������� ��������� ������ � ����������� ������ ����, ���������� �� �������
void deinit();//prepare to soutdown application: release all resources
int deinitSound();
void drawLabirint(int mode);
void IdleFunc();//change scene use controls
void init();//init any data after window was painted
int initBots();
void initOptions();
int initTextures();
int initSound();
int isInside(Point3f p, Point3f A, Point3f B);
int LoadLabirint(char *fl);
int loadTexture(string opt_name, int tex_id, int rgba);

int prepareHeroTexCoord(SimpleMesh *mesh, int seg, int slice);
int prepareSkyTexCoord(SimpleMesh *mesh, int seg, int slice);
void RedrawInterface();//redraw 2d interface
void RedrawScene();//redraw 3d scene
void initmaptex();
int isPosInOneOf(Point3f pos, bot* ar, int ar_size, Point3f bbmin, Point3f bbmax);

int OnChar(int ch);
int OnKeyDown(int key);//key down event handler
int OnKeyUp(int key);//key up event handler
int OnLButtonDown(short x, short y);//left mouse button down event handler
int OnLButtonUp(short x, short y);//left mouse button up event handler
int OnMouseMove(short x, short y);//mouse move event handler
int OnPaint();//repaint
int OnSize(int wi, int he);//resize handle
int OnTimer();
void Gameover();
};
#endif // GAME_H_INCLUDED
