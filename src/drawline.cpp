#include <Windows.h>
#include <math.h>

#define NUM 1000
#define TWOPI (2*3.14159)
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR szCmdLine, _In_ int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("drawline");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClassW(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName, TEXT("Line Demostration"),
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

void DrawBezier(HDC hdc, POINT apt[])
{
	PolyBezier(hdc, apt, 4);

	MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
	LineTo(hdc, apt[1].x, apt[1].y);

	MoveToEx(hdc, apt[2].x, apt[2].y, NULL);
	LineTo(hdc, apt[3].x, apt[3].y);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	static int cxClient, cyClient;
	int i;
	//POINT apt[NUM];
	//POINT apt[5] = { 100, 100, 200, 100, 200, 200, 100, 200, 100, 100 };
	static POINT apt[4];
	HDC hdc;
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		apt[0].x = cxClient / 4;
		apt[0].y = cyClient / 2;

		apt[1].x = cxClient / 2;
		apt[1].y = cyClient / 4;

		apt[2].x = cxClient / 2;
		apt[2].y = 3 * cyClient / 4;

		apt[3].x = 3 * cxClient / 4;
		apt[3].y = cyClient / 2;

		return 0;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON || wParam & MK_RBUTTON)
		{
			hdc = GetDC(hwnd);
			SelectObject(hdc, GetStockObject(WHITE_PEN));
			DrawBezier(hdc, apt);
			if (wParam & MK_LBUTTON)
			{
				apt[1].x = LOWORD(lParam);
				apt[1].y = HIWORD(lParam);
			}
			if (wParam & MK_RBUTTON)
			{
				apt[2].x = LOWORD(lParam);
				apt[2].y = HIWORD(lParam);
			}
			SelectObject(hdc, GetStockObject(BLACK_PEN));
			DrawBezier(hdc, apt);
			ReleaseDC(hwnd, hdc);
		}
		return 0;
	case WM_PAINT:
		InvalidateRect(hwnd, NULL, TRUE);
		hdc = BeginPaint(hwnd, &ps);
		// 画正弦曲线
		/*MoveToEx(hdc, 0, cyClient / 2, NULL);
		LineTo(hdc, cxClient, cyClient / 2);
		for (i = 0; i < NUM; i++)
		{
			apt[i].x = i * cxClient / NUM;
			apt[i].y = (int)(cyClient / 2 * (1 - sin(TWOPI * i / NUM)));
		}
		Polyline(hdc, apt, NUM);*/
		
		// 画网格
		/*RECT rect;
		GetClientRect(hwnd, &rect);
		for (int x = 0; x < rect.right; x += 100)
		{
			MoveToEx(hdc, x, 0, NULL);
			LineTo(hdc, x, rect.bottom);
		}
		for (int y = 0; y < rect.bottom; y += 100)
		{
			MoveToEx(hdc, 0, y, NULL);
			LineTo(hdc, rect.right, y);
		}*/
		
		// 画矩形
		/*MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
		for (int i = 1; i < 5; i++)
			LineTo(hdc, apt[i].x, apt[i].y);*/
		//Polyline(hdc, apt, 5); //和上面效果一样

		// 使用函数画矩形
		//Rectangle(hdc, 0, 0, 100, 100);

		// 使用函数画椭圆形
		//Ellipse(hdc, cxClient / 8, cyClient / 8, 7 * cxClient / 8, 7 * cyClient / 8);

		// 贝塞尔曲线
		DrawBezier(hdc, apt);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}