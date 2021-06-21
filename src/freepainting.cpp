/**
 * \file freepainting.cpp
 * \brief 程序入口
 *
 * 主程序
 * v1.1 2021.6.19
 * (1) 修复绘图时起始点和终点不准的问题
 * (2) 修复放大缩小窗口会擦除痕迹的问题
 * (3) 更好地区分三种不同的绘图参数结构
 * 
 * v1.2 2021.6.20
 * (1) 更改文字提示框显示位置
 * (2) 修正右键点击无法清屏问题
 * (3) 可以控制是否填充图像
 * (4) 使用MB弹出错误，更直观
 * 
 * v2.0 2021.6.21
 * (1) 加入多线程读取xml方法，便于文件更新
 * (2) 完善error管理机制
 * 
 * \author 陈瑞佳
 * \version 2.0
 * \date 2021/06/21
 */


#include "freepainting.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**
 * @brief WIN32窗口主程序
 * @param [in] hInstance		程序当前运行的实例句柄
 * @param [in] hPreInstance		前一个实例句柄，NULL
 * @param [in] szCmdLine		传递给程序的命令行参数
 * @param [in] iCmdShow			窗口显示的方式
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
		ErrorShow(ERROR_REGISTER_CLASS);
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
 * @brief 画板，每次刷新窗口重新绘制历史图像
 * @param [in] hwnd				窗体句柄
 * @param [in] hdc				图像设备描述表
 * @param [in] ptBegin			绘画起始点
 * @param [in] ptEnd			绘画终止点
 * @param [in] type_shape		菜单选择的线条形状
 * @param [in] type_color		菜单选择的图形颜色
 * @param [in] type_linewidth	菜单选择的线条宽度
 * @param [out] draw_parm		绘制所需参数数组
 */
VOID SaveDrawing(HWND hwnd, HDC hdc, POINT ptBegin, POINT ptEnd, 
	std::vector<DrawParm>& draw_parm, ShapeParm type_shape, ColorParm type_color, LinewidthParm type_linewidth)
{
	RECT rect; //使用参数NULL代替，表示整个屏幕
	HPEN hpen;
	
	GetClientRect(hwnd, &rect);
	DrawText(hdc, TEXT("Please move leftbutton to draw, click rightbutton to erase!"), 59, &rect, DT_CENTER | DT_SINGLELINE);
	
	// TODO:完善异常处理机制
	
	if(flag_readxmlfile)
		ErrorShow(ParsingData(type_color, type_linewidth, color, linewidth, color_cache, linewidth_cache));

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
	
	// TODO: 用循环画图速度慢，即图数量很多会出现画图帧数不够的情况
	for (int i = 0; i < draw_parm.size(); i++)
	{
		hpen = CreatePen(PS_SOLID, draw_parm[i].LinewidthParm, RGB(draw_parm[i].color_parm[0], draw_parm[i].color_parm[1], draw_parm[i].color_parm[2]));
		SelectObject(hdc, hpen);
		SelectObject(hdc, GetStockObject(NULL_BRUSH)); // 该行可以控制是否画出填充图像
		switch (draw_parm[i].shape_parm)
		{
			// TODO: 实时显示画图过程，而非两个点相连
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


/**
 * @brie 清除历史图像
 * @param [in] draw_parm		绘制所需参数数组
 */
VOID ClearDrawing(std::vector<DrawParm>& draw_parm)
{
	draw_parm.clear();
	return;
}

/**
 * @brie 多线程读取文件
 * @param [in] pvoid			线程指针
 */
VOID ThreadReadXmlFile(PVOID pvoid)
{
	while (TRUE)
	{
		ErrorShow(ReadXmlFile(color_cache, linewidth_cache));
		flag_readxmlfile = true;
		// TODO:增加定时器功能？
		Sleep(10 * 1000); //延迟10s
	}
}

/**
 * @brief 消息回调函数
 * @param [in] hwnd			窗体句柄
 * @param [in] message			消息常量标识符
 * @param [in] wParam			消息有关的常量值，或者窗口和控件的句柄
 * @param [in] lParam			指向内存中数据的指针
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int wmId, wmEvent;
	int flag_xml_read = 0;
	switch (message)
	{
	case WM_CREATE:
		_beginthread(ThreadReadXmlFile, 0, NULL);
		return 0;
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
