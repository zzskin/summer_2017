#include <Windows.h>
#include <time.h>
#include <mmsystem.h>
#include "game.h"

#pragma comment(lib, "WINMM.LIB")
#pragma comment(lib, "MSimg32.LIB")

int BirdWidth;           //С��ͼƬ�Ŀ��
int wallLength;          //ǽ�ĳ���
int BirdX;               //С��ĺ�����
int wallX;               //���Ҳ�ǽ��λ��
int wallDistance;        //ǽ���
BOOL walls[Num][3];      //��ά������ϰ���ߣ�
double blueSpeed = 2;    //�ϰ����ʼ���ƶ��ٶ�
int timer = 0;           //��ʱ��
int bird_pos = 0;        //��ʾ����������
double dh = 0;            //��ʾС���λ��

void CreateBlueGame()
{
	int width = rect.right - rect.left;        //���ڿ��
	int height = rect.bottom - rect.top;       //���ڸ߶�
	int i, j;
	BirdWidth = width / 25;                    //ȡ��ȵĶ�ʮ���֮һ
	wallLength = height / 3;                   //ȡ�߶ȵ�����֮һ
	BirdX = width * 8 / 9;                     //�����ұ�Ե�ŷ�֮һ��ȵ�λ��
	wallX = width * (Num - 1) / Num;           //���Ҳ�ǽ�ĺ����꣬�����ұ�ԵN��֮һ��ȵ�λ��
	wallDistance = width / Num;                //ȡ��ȵ�N��֮һ
	srand(time(0) + rand());                   //�������
	point_bird.y = rect.top + height / 2;      //С���ʼλ��
							
	//����ǽ��
	for (i = 0; i < Num; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			walls[i][j] = TRUE;
		}
	}

	//ÿ��ǽ��һ��ȱ�ڣ�ȱ��λ���������ÿһ��������λ�ö��п���Ϊȱ��
	for (i = 0; i < Num; ++i)
	{
		walls[i][rand() % 3] = FALSE;
	}
      
	SetTimer(hwnd, WALL_TIMER, 20, NULL);    //����ǽ���ƶ��ļ�ʱ��
}

int OnGameTimer(void)
{
	int i, j;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	if (pause)           //��ͣ
	{
		return 1;
	}

	dh = v * t + 1 / 2 * g * t * t;
	v += g * t;
	point_bird.y += dh;

	if (timer/32 % 8 == 0)//��Ϸ����
	{
		blueSpeed += 0.1;
	}
	wallX += blueSpeed;  //����ǽ���ƶ�,����


   //��������С���ж�С�������
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


	for (i = 0; i < Num; ++i) //�ж���û��ײ���ϰ�������غϣ�
	{
		if (walls[i][bird_pos] && BirdX <= wallX - i * wallDistance && BirdX + BirdWidth >= wallX - i * wallDistance)
		{
			dead = TRUE;
		}
	}

	if (wallX >= width)      //ǽ�������ڻص���ʼλ��
	{
		wallX = width * (Num - 1) / Num;  //���Ҳ�ǽ�ĺ����꣬�����ұ�ԵN��֮һ��ȵ�λ�ã��涨λ��
		for (i = 0; i < Num - 1; ++i)     //�ϰ����ƶ����һ������ǰһ����λ��
		{
			for (j = 0; j < 3; ++j)
			{
				walls[i][j] = walls[i + 1][j];
			}
		}
		for (j = 0; j < 3; ++j)          //�ƶ�������ȱ��
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

	//���Ʊ���
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

	//�������軭�ʻ�ˢ
	wallPen = greenPen(BirdWidth / 3);

	//����ǽ��
	SelectObject(hdcMem, wallPen);
	for (i = 0; i < Num; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			//���������i�У��������µ�j��
			if (walls[i][j])
			{
				poly[0].x = poly[1].x = rect.left + wallX - i * wallDistance;  //��ʾ��ֱ�߶�
				poly[0].y = rect.top + j * wallLength;
				poly[1].y = rect.top + (j + 1) * wallLength;
				Polyline(hdcMem, poly, 2);
			}
		}
	}

	//��С��
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
	//������������DC��
	if(hOldFont = (HFONT)SelectObject(hdcMem, hFont))
	{
	    CHAR szSourceInfo[1024];
	    wsprintf(szSourceInfo, "Time = %ds", timer/32);
	    COLORREF crOld = SetTextColor(hdcMem, COLOR_FONT);
	    int nOldMode = SetBkMode(hdcMem, TRANSPARENT);
    //����ַ�
	    TextOut(hdcMem, 10, 10,
		szSourceInfo, lstrlen(szSourceInfo));
	    SelectObject(hdcMem, hOldFont);
}

	//������Դ
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
    SetTimer(hwnd, PAINTER_TIMER, 10, NULL);//����������Ϸ������
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
			MessageBox(NULL, "Game Over!You Bird Is Die��", "��Ϸ����", MB_ICONERROR | MB_OK);
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

	//���
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0, SRCCOPY);

	//������Դ
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