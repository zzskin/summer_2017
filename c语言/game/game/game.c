#include <Windows.h>
#include <time.h>
#include <mmsystem.h>
#include "game.h"

#pragma comment(lib, "WINMM.LIB")
#pragma comment(lib, "MSimg32.LIB")

int BirdWidth;           //小鸟图片的宽度
int wallLength;          //墙的长度
int BirdX;               //小鸟的横坐标
int wallX;               //最右侧墙的位置
int wallDistance;        //墙间距
BOOL walls[Num][3];      //二维数组存障碍物（线）
double blueSpeed = 2;    //障碍物初始的移动速度
int timer = 0;           //计时器
int bird_pos = 0;        //表示鸟所在区域
double dh = 0;            //表示小鸟的位移

void CreateBlueGame()
{
	int width = rect.right - rect.left;        //窗口宽度
	int height = rect.bottom - rect.top;       //窗口高度
	int i, j;
	BirdWidth = width / 25;                    //取宽度的二十五分之一
	wallLength = height / 3;                   //取高度的三分之一
	BirdX = width * 8 / 9;                     //距离右边缘九分之一宽度的位置
	wallX = width * (Num - 1) / Num;           //最右侧墙的横坐标，距离右边缘N分之一宽度的位置
	wallDistance = width / Num;                //取宽度的N分之一
	srand(time(0) + rand());                   //随机种子
	point_bird.y = rect.top + height / 2;      //小鸟初始位置
							
	//建立墙壁
	for (i = 0; i < Num; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			walls[i][j] = TRUE;
		}
	}

	//每层墙有一个缺口，缺口位置随机，在每一个横坐标位置都有可能为缺口
	for (i = 0; i < Num; ++i)
	{
		walls[i][rand() % 3] = FALSE;
	}
      
	SetTimer(hwnd, WALL_TIMER, 20, NULL);    //创建墙壁移动的计时器
}

int OnGameTimer(void)
{
	int i, j;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	if (pause)           //暂停
	{
		return 1;
	}

	dh = v * t + 1 / 2 * g * t * t;
	v += g * t;
	point_bird.y += dh;

	if (timer/32 % 8 == 0)//游戏升级
	{
		blueSpeed += 0.1;
	}
	wallX += blueSpeed;  //创建墙的移动,加速


   //分屏，存小鸟，判断小鸟的区域
	if (point_bird.y < 0)
	{
		bird_pos = 0;
		dead = TRUE;
	}
	else if (point_bird.y > 0 && point_bird.y < height / 3)
	{
		bird_pos = 0;
	}
	else if (point_bird.y < height * 2 / 3)
	{
		bird_pos = 1;
	}
	else if (point_bird.y < height)
	{
		bird_pos = 2;
	}
	else
	{
		bird_pos >= 2; 
		dead = TRUE;
	}


	for (i = 0; i < Num; ++i) //判断有没有撞到障碍物（坐标重合）
	{
		if (walls[i][bird_pos] && BirdX <= wallX - i * wallDistance && BirdX + BirdWidth >= wallX - i * wallDistance)
		{
			dead = TRUE;
		}
	}

	if (wallX >= width)      //墙超出窗口回到初始位置
	{
		wallX = width * (Num - 1) / Num;  //最右侧墙的横坐标，距离右边缘N分之一宽度的位置，规定位置
		for (i = 0; i < Num - 1; ++i)     //障碍物移动后后一个到达前一个的位置
		{
			for (j = 0; j < 3; ++j)
			{
				walls[i][j] = walls[i + 1][j];
			}
		}
		for (j = 0; j < 3; ++j)          //移动后的随机缺口
		{
			walls[Num - 1][j] = TRUE;
		}
		walls[Num - 1][rand() % 3] = FALSE;
	}
}

void createbk(void)
{
	hdcmem_BK = CreateCompatibleDC(hdc);

	hBK = LoadImage(NULL, BITMAP_FILE_BK,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BitBlt(hdcMem, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcmem_BK, 0, 0, SRCCOPY);

	hdcmem_Bird = CreateCompatibleDC(hdc);
	hBird = LoadImage(NULL, BITMAP_FILE_BIRD,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void OneGamePaint(void)
{
	HPEN wallPen;
	POINT poly[2];
	int i, j;
	int width = rect.right - rect.left, height = rect.bottom - rect.top;
	RECT rect;

	//绘制背景
	SelectObject(hdcmem_BK, hBK);
	GetClientRect(hwnd, &rect);
	GetObjectA(hBK, sizeof(BITMAP), &bmp_bk);
	StretchBlt(hdcMem, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		hdcmem_BK,
		0, 0,
		bmp_bk.bmWidth,
		bmp_bk.bmHeight,
		SRCCOPY);

	//创造所需画笔画刷
	wallPen = greenPen(BirdWidth / 3);

	//绘制墙壁
	SelectObject(hdcMem, wallPen);
	for (i = 0; i < Num; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			//从右往左第i列，从上往下第j个
			if (walls[i][j])
			{
				poly[0].x = poly[1].x = rect.left + wallX - i * wallDistance;  //表示竖直线段
				poly[0].y = rect.top + j * wallLength;
				poly[1].y = rect.top + (j + 1) * wallLength;
				Polyline(hdcMem, poly, 2);
			}
		}
	}

	//画小鸟
	SelectObject(hdcmem_Bird, hBird);
	GetObject(hBird, sizeof(BITMAP), &bmp_bird);
	TransparentBlt(hdcMem, BirdX, point_bird.y,
		BirdWidth, BirdWidth,
		hdcmem_Bird,
		0, 0,
		bmp_bird.bmWidth,
		bmp_bird.bmHeight,
		RGB(255, 255, 255));

	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));
	//将这个对象放入DC中
	if(hOldFont = (HFONT)SelectObject(hdcMem, hFont))
	{
	    CHAR szSourceInfo[1024];
	    wsprintf(szSourceInfo, "Time = %ds", timer/32);
	    COLORREF crOld = SetTextColor(hdcMem, COLOR_FONT);
	    int nOldMode = SetBkMode(hdcMem, TRANSPARENT);
    //输出字符
	    TextOut(hdcMem, 10, 10,
		szSourceInfo, lstrlen(szSourceInfo));
	    SelectObject(hdcMem, hOldFont);
}

	//回收资源
	DeleteObject(wallPen);
	DeleteObject(hBK);
	DeleteObject(hBird);
	DeleteObject(hFont);
}

void CreateGame()
{
	hdc = GetDC(hwnd);
	dead = FALSE;
	pause = TRUE;
	t = 1;
	GetClientRect(hwnd, &rect);
	CreateBlueGame();
    SetTimer(hwnd, PAINTER_TIMER, 10, NULL);//用来保持游戏流畅度
	createbk();
	PlayMusic();
}

void PlayMusic()
{
	PlaySound("OMFG - Hello", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void OnTimer(int timerID)
{
	switch (timerID)
	{
	case WALL_TIMER:
		timer++;
		OnGameTimer();
		if (dead)
		{
			KillAllTimer();
			MessageBox(NULL, "Game Over!You Bird Is Die！", "游戏结束", MB_ICONERROR | MB_OK);
			ExitProcess(0);
		}
		break;

	case JUMP_TIMER:
		SetTimer(hwnd, JUMP_TIMER, TimerInitInterval, NULL);
		dh = v * t + 1 / 2 * g * t * t;
		v += g * t;
		point_bird.y += dh;
		break;
	default:
		break;
	}
}

void OnKeyDown(DWORD vk,HWND _hwnd)
{
	switch (vk)
	{
	case VK_SPACE:
		pause = !pause;
		break;
	case VK_UP:
		v = INIT_v0;
		g = INIT_g;
		break;
	default:
		break;
	}
}

void GamePaint(void)
{
	HBITMAP hbmMem;

	hdcMem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(hdcMem, hbmMem);

	OneGamePaint();

	//输出
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, SRCCOPY);

	//回收资源
	DeleteObject(hbmMem);
	DeleteObject(hdcMem);
}

void KillAllTimer(void)
{
	KillTimer(hwnd, WALL_TIMER);
	KillTimer(hwnd, PAINTER_TIMER);
}

HPEN greenPen(int cWidth)
{
	return CreatePen(PS_SOLID, cWidth, RGB(153, 255, 0));
}