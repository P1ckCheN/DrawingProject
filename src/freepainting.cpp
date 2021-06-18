/**
 * \file freepainting.cpp
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

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/**
 * @brief 窗口主程序
 */
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR szCmdLine, _In_ int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("FreePainting");
	HWND hwnd;
	MSG msg;
	static HBRUSH hBrush;
	WNDCLASS wndclass;
	
	LoadMenu(hInstance, TEXT("Menu"));
	hBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = szAppName;
	
	if (!RegisterClassW(&wndclass))
	{
		MessageBox(NULL, TEXT("Program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("FreePaintingTools"),
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

/**
 * @brief 消息回调函数
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	RECT rect;
	PAINTSTRUCT ps;
	int wmId, wmEvent;
	static POINT ptBegin;
	static POINT ptEnd;	
	static Parm type_drawing = TYPE_LINE;								///< 默认形状：直线
	static Parm type_color = TYPE_RED;									///< 默认色彩：红色
	static Parm type_linewidth = TYPE_THIN;								///< 默认粗细度：细
	static int color[3];
	static int linewidth;
	int flag_xml_read = 0;
	HPEN hPen;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		GetCursorPos(&ptBegin);
		return 0;
	case WM_LBUTTONUP:
		GetCursorPos(&ptEnd);
		hdc = GetDC(hwnd);
		// TODO:完善异常处理机制
		flag_xml_read = ReadXmlFile(type_color, type_linewidth, color, &linewidth);
		if (!flag_xml_read)
		{
			exit(-1);
		}
		hPen = CreatePen(PS_SOLID, linewidth, RGB(color[0], color[1], color[2]));
		SelectObject(hdc, hPen);
		switch (type_drawing)
		{
			// TODO: 
			// 1:非最大化窗口时，画图的位置会有偏差
			// 2:实时显示画图过程，而非两个点相连
			case TYPE_LINE:
				MoveToEx(hdc, ptBegin.x, ptBegin.y, NULL);
				LineTo(hdc, ptEnd.x, ptEnd.y);
				break;
			case TYPE_RECT:
				Rectangle(hdc, ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y);
				break;
			case TYPE_ELLIPSE:
				Ellipse(hdc, ptBegin.x, ptBegin.y, ptEnd.x, ptEnd.y);
				break;
			default:
				break;
		}
		DeleteObject(hPen);
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_RBUTTONDOWN:
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		ReleaseDC(hwnd, hdc);
		InvalidateRect(hwnd, NULL, true);
		return 0;
	case WM_RBUTTONUP:
		return 0;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
			case ID_TYPE_LINE:
				type_drawing = TYPE_LINE;
				break;
			case ID_TYPE_RECTANGLE:
				type_drawing = TYPE_RECT;
				break;
			case ID_TYPE_ELLIPSE:
				type_drawing = TYPE_ELLIPSE;
				break;
			case ID_COLOR_RED:
				type_color = TYPE_RED;
				break;
			case ID_COLOR_BLUE:
				type_color = TYPE_BLUE;
				break;
			case ID_COLOR_GR:
				type_color = TYPE_GREEN;
				break;
			case ID_LINEWIDTH_THIN:
				type_linewidth = TYPE_THIN;
				break;
			case ID_LINEWIDTH_MEDIUM:
				type_linewidth = TYPE_MEDIUM;
				break;
			case ID_LINEWIDTH_THICK:
				type_linewidth = TYPE_THICK;
				break;
			default:
				break;
			return  DefWindowProc(hwnd, message, wParam, lParam);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("Please move leftbutton to draw, click rightbutton to erase!"), 59, &rect, DT_CENTER | DT_SINGLELINE);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}