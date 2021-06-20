/**
 * \file freepainting.cpp
 * \brief 程序入口
 *
 * 主程序
 * v1.1 
 * (1) 修复绘图时起始点和终点不准的问题
 * (2) 修复放大缩小窗口会擦除痕迹的问题
 * (3) 更好地区分三种不同的绘图参数结构
 * \author 陈瑞佳
 * \version 1.1
 * \date 2021/06/20
 */


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

void SaveDrawing(HWND hwnd, HDC hdc, POINT ptBegin, POINT ptEnd, 
	std::vector<DrawParm>& draw_parm, ShapeParm type_shape, ColorParm type_color, LinewidthParm type_linewidth)
{
	RECT rect; //使用参数NULL代替，表示整个屏幕
	HPEN hpen;
	int color[3];
	int linewidth;

	GetClientRect(hwnd, &rect);
	DrawText(hdc, TEXT("Please move leftbutton to draw, click rightbutton to erase!"), 59, &rect, DT_CENTER | DT_SINGLELINE);
	// TODO:完善异常处理机制
	int flag_xml_read = ReadXmlFile(type_color, type_linewidth, color, &linewidth);
	if (!flag_xml_read)
	{
		exit(-1);
	}
	DrawParm curr_parm;
	curr_parm.ptBegin = ptBegin;
	curr_parm.ptEnd = ptEnd;
	curr_parm.shape_parm = type_shape;
	for (int i = 0; i < 3; i++)
	{
		curr_parm.color_parm.push_back(color[i]);
	}
	curr_parm.LinewidthParm = linewidth;

	draw_parm.emplace_back(curr_parm);
	
	// TODO: 用循环画图出现速度很慢，图数量多会出现画图帧数人眼可分辨的情况
	for (int i = 0; i < draw_parm.size(); i++)
	{
		hpen = CreatePen(PS_SOLID, draw_parm[i].LinewidthParm, RGB(draw_parm[i].color_parm[0], draw_parm[i].color_parm[1], draw_parm[i].color_parm[2]));
		SelectObject(hdc, hpen);
		switch (draw_parm[i].shape_parm)
		{
			// TODO: 
			// 2:实时显示画图过程，而非两个点相连
		case TYPE_LINE:
			MoveToEx(hdc, draw_parm[i].ptBegin.x, draw_parm[i].ptBegin.y, NULL);
			LineTo(hdc, draw_parm[i].ptEnd.x, draw_parm[i].ptEnd.y);
			break;
		case TYPE_RECT:
			Rectangle(hdc, draw_parm[i].ptBegin.x, draw_parm[i].ptBegin.y, draw_parm[i].ptEnd.x, draw_parm[i].ptEnd.y);
			break;
		case TYPE_ELLIPSE:
			Ellipse(hdc, draw_parm[i].ptBegin.x, draw_parm[i].ptBegin.y, draw_parm[i].ptEnd.x, draw_parm[i].ptEnd.y);
			break;
		default:
			break;
		}
		DeleteObject(hpen);
	}

	
}

void ClearDrawing(std::vector<DrawParm>& draw_parm)
{
	draw_parm.clear();
	return;
}
/**
 * @brief 消息回调函数
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int wmId, wmEvent;
	static int flag_clear = false;
	static POINT ptShapeBegin, ptShapeEnd;

	static ShapeParm type_shape;								///< 默认形状：直线
	static ColorParm type_color;								///< 默认色彩：红色
	static LinewidthParm type_linewidth;						///< 默认粗细度：细
	static std::vector<DrawParm> draw_parm;
	int flag_xml_read = 0;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		ptShapeBegin.x = LOWORD(lParam);
		ptShapeBegin.y = HIWORD(lParam);
		//GetCursorPos(&ptShapeBegin); //获取坐标不准确
		return 0;
	case WM_LBUTTONUP:
		ptShapeEnd.x = LOWORD(lParam);
		ptShapeEnd.y = HIWORD(lParam);
		//GetCursorPos(&ptShapeEnd); //获取坐标不准确
		
		InvalidateRect(hwnd, NULL, TRUE); //关键，使区域无效化，可以调用WM_PAINT函数，不然画不出来
		return 0;
		
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (flag_clear == false)
		{
			SaveDrawing(hwnd, hdc, ptShapeBegin, ptShapeEnd, draw_parm, type_shape, type_color, type_linewidth);
		}
		else
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			DrawText(hdc, TEXT("Please move leftbutton to draw, click rightbutton to erase!"), 59, &rect, DT_CENTER | DT_SINGLELINE);
			ClearDrawing(draw_parm);
			flag_clear = false;
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_RBUTTONDOWN:
		flag_clear = true;
		return 0;
	case WM_RBUTTONUP:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case ID_TYPE_LINE:
			type_shape = TYPE_LINE;
			break;
		case ID_TYPE_RECTANGLE:
			type_shape = TYPE_RECT;
			break;
		case ID_TYPE_ELLIPSE:
			type_shape = TYPE_ELLIPSE;
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
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
