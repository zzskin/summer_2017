#pragma once
#include <Windows.h>

#define INIT_g         3.5                    //���ٶ�
#define INIT_v0        -15                    //��ʼ�ٶȣ�����Ϊ����
#define JUMP_TIMER     10000
#define PAINTER_TIMER  5001
#define WALL_TIMER     5002
#define BITMAP_FILE_BK   "bk.bmp"
#define BITMAP_FILE_BIRD  "bird.bmp"
#define TimerInitInterval 2500            //ʱ����
#define COLOR_FONT RGB(0,0,0)             //������ɫ
#define Num 5                            //����ÿ����ʾ���ϰ�����


//===========
//ȫ�ֱ���
//===========
HWND hwnd;
HDC hdc, hdcMem;
RECT rect;
BOOL dead, pause;
POINT point_bird; //С������
HDC hdcmem_BK, hdcmem_Bird;
HBITMAP hBK, hBird;
BITMAP bmp_bk, bmp_bird;
HFONT hFont, hOldFont;

double v, g;
int t;

void PlayMusic();
void CreateGame(void);
void OnTimer(int);
void OnKeyDown(DWORD, HWND _hwnd);
void OnKeyUp(DWORD);
void GamePaint(void);
void ResizeBluePart(void);
void Resize(void);
void KillAllTimer(void);
void createbk(void);

HPEN greenPen(int);

void CreateBlueGame(void);
int OnGameTimer(void);
void BlueGamePaint(void);