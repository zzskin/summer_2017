#include <Windows.h>
#include "game.h"

#define TITLE "Flappy Bird!"

LRESULT CALLBACK MainWndProc(HWND _hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		hwnd = _hwnd;
		CreateGame();
		break;
	case WM_TIMER:
		OnTimer(wParam);
		GamePaint();
		break;
	case WM_KEYDOWN:
		OnKeyDown(wParam, _hwnd);
		break;
	case WM_PAINT:
		GamePaint();
		break;
	case WM_DESTROY:
		ExitProcess(0);
		break;
	default:
		break;
	}
	return DefWindowProc(_hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(
	HINSTANCE hInstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
{
	WNDCLASS wc;
	//HWND hwnd;
	MSG msg;
	int fGotMessage;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = "MainMenu";
	wc.lpszClassName = "MainWClass";
	RegisterClass(&wc);

	hwnd = CreateWindow("MainWClass", TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		(HWND)NULL, (HMENU)NULL, hInstance, (LPVOID)NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && fGotMessage != -1) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}