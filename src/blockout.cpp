/**
 * \file blockout.cpp
 * \brief 程序入口
 *
 * 主程序
 *
 * \author 陈瑞佳
 * \version 1.0
 * \date 2021/06/17
 */

#include "framework.h"
#include "freepainting.h"
#include <vector>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



/**
 * @brief 窗口主程序
 */
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR szCmdLine, _In_ int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("BlockOut2");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClassW(&wndclass))
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Mouse Button & Capture Demos"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void HistoryDraw(HDC hdc, POINT ptBegin, POINT ptEnd, std::vector<std::pair<POINT, POINT>>& history)
{
	history.push_back({ ptBegin, ptEnd });
	//SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	for (int i = 0; i < history.size(); i++)
	{
		Rectangle(hdc, history[i].first.x, history[i].first.y, history[i].second.x, history[i].second.y);
	}
}
void DrawBoxOutline(HWND hwnd, POINT ptBeg, POINT ptEnd)
{
	HDC hdc;
	hdc = GetDC(hwnd);
	SetROP2(hdc, R2_NOT); // 对前景色取反
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
	ReleaseDC(hwnd, hdc);
}
/**
 * @brief 消息回调函数
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL fBlocking, fValidBox;
	static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;
	static std::vector<std::pair<POINT, POINT>> history;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		ptBeg.x = ptEnd.x = LOWORD(lParam);
		ptBeg.y = ptEnd.y = HIWORD(lParam);
		//DrawBoxOutline(hwnd, ptBeg, ptEnd);
		
		SetCapture(hwnd);
		SetCursor(LoadCursor(NULL, IDC_CROSS));

		fBlocking = TRUE;
		return 0;
	case WM_MOUSEMOVE:
		if (fBlocking)
		{
			SetCursor(LoadCursor(NULL, IDC_CROSS));

			//DrawBoxOutline(hwnd, ptBeg, ptEnd);

			ptEnd.x = LOWORD(lParam);
			ptEnd.y = HIWORD(lParam);
			
			//DrawBoxOutline(hwnd, ptBeg, ptEnd);
		}
		return 0;
	case WM_LBUTTONUP:
		if (fBlocking)
		{
			//DrawBoxOutline(hwnd, ptBeg, ptEnd);

			ptBoxBeg = ptBeg;
			ptBoxEnd.x = LOWORD(lParam);
			ptBoxEnd.y = HIWORD(lParam);
			
			ReleaseCapture();
			SetCursor(LoadCursor(NULL, IDC_ARROW));

			fBlocking = FALSE;
			fValidBox = TRUE;

			InvalidateRect(hwnd, NULL, TRUE);
		}
		return 0;
	case WM_CHAR:
		if (fBlocking & (wParam == '\x1B'))
		{
			DrawBoxOutline(hwnd, ptBeg, ptEnd);
			ReleaseCapture();
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			fBlocking = FALSE;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (fValidBox)
		{
			/*SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Rectangle(hdc, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y);*/
			HistoryDraw(hdc, ptBoxBeg, ptBoxEnd, history);
		}
		if (fBlocking)
		{
			SetROP2(hdc, R2_NOT);
			Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

