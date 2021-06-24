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
 * 
 * v2.0 2021.6.21
 * (1) 加入多线程读取xml方法，便于文件更新
 * (2) 使用MB弹出错误，更直观
 * (3) 完善error管理机制
 * 
 * v2.1 2021.6.23
 * (1) 添加画板类
 * (2) 添加多线程访问控制，原子量方法
 * 
 * v2.2 2021.6.23
 * (1) 添加数据存储类
 * (1) 增设数据库 + 缓存 Cache Aside Pattern 方法
 * 读的时候，先读缓存;
 * 如果没有缓存, 读数据库, 取出数据放入缓存, 同时返回响应;
 * 更新的时候, 先更新数据库, 再删除缓存

 * 
 * \author 陈瑞佳
 * \version 2.2
 * \date 2021/06/24
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
	// 开启读取文件的线程
	std::thread thread_read_xml_file(ThreadReadXmlFile, std::ref(board));
	thread_read_xml_file.detach();
	
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
 * @brief DrawingBoard类，析构函数
 */
DrawingBoard::~DrawingBoard()
{
	drawing_parm.clear();
}

/**
 * @brief Cache类，构造函数
 * 初始化
 */
Cache::Cache()
{
	state = DataState::NO_UPDATE;
	color_cache.clear();
	color_cache.clear();
}

/**
 * @brief Cache类, 构造函数
 * 初始化
 */
void Cache::deletecache()
{
	state = DataState::DELETED;
	color_cache.clear();
	linewidth_cache.clear();
}

/**
 * @brief Cache类, 拷贝构造函数
 * 相比于默认拷贝构造,因为只需要数据->缓存
 * 所以不拷贝状态
 * 初始化
 */
Cache& Cache::operator=(const Cache& cache)
{
	color_cache = cache.color_cache;
	linewidth_cache = cache.linewidth_cache;
	return *this;
}

/**
 * @brief DrawingBoard类, 画图函数
 * @param [in] hwnd				窗体句柄
 * @param [in] hdc				图像设备描述表
 */
void DrawingBoard::Drawing(HWND hwnd, HDC hdc)
{
	RECT rect;
	HPEN hpen;
	GetClientRect(hwnd, &rect);
	DrawText(hdc, TEXT("Please move leftbutton to draw, click rightbutton to erase!"), 59, &rect, DT_CENTER | DT_SINGLELINE);

	// 跳过最开始未读入数据库且未写入缓存的情况
	while (true)
	{
		if (color_linewidth_cache.state != DataState::NO_UPDATE)
			break;
	}

	// 如果满足缓存不存在,则将数据库拷贝到缓存
	if (color_linewidth_cache.state == DataState::DELETED)
	{
		color_linewidth_cache = color_linewidth_database;
		color_linewidth_cache.state = DataState::UPDATEED;
	}
	
	// 使用缓存数据参数绘图
	ErrorShow(ParsingDataCache(type_color, type_linewidth, color, linewidth, color_linewidth_cache.color_cache , color_linewidth_cache.linewidth_cache));

	// 下面为之前版本的原子实现
	//if (flag_read_xml_file)
		//ErrorShow(ParsingDataAtomic(type_color, type_linewidth, color, linewidth, color_cache_thread, linewidth_cache_thread));
	
	DrawingParm curr_parm;
	curr_parm.point_begin = point_begin;
	curr_parm.point_end = point_end;
	curr_parm.shape_parm = type_shape;
	curr_parm.color_parm = {color[0], color[1], color[2]};
	curr_parm.linewidth_parm = linewidth;

	drawing_parm.emplace_back(curr_parm); // 保存历史绘图

	for (DrawingParm& his_parm : drawing_parm)
	{
		hpen = CreatePen(PS_SOLID, his_parm.linewidth_parm, RGB(his_parm.color_parm[0], his_parm.color_parm[1], his_parm.color_parm[2]));
		SelectObject(hdc, hpen);
		SelectObject(hdc, GetStockObject(NULL_BRUSH)); // 该行可以控制是否画出填充图像
		switch (his_parm.shape_parm)
		{
			// TODO: 实时显示画图过程，而非两个点相连
		case ShapeParm::TYPE_LINE:
			MoveToEx(hdc, his_parm.point_begin.x, his_parm.point_begin.y, NULL);
			LineTo(hdc, his_parm.point_end.x, his_parm.point_end.y);
			break;
		case ShapeParm::TYPE_RECT:
			Rectangle(hdc, his_parm.point_begin.x, his_parm.point_begin.y, his_parm.point_end.x, his_parm.point_end.y);
			break;
		case ShapeParm::TYPE_ELLIPSE:
			Ellipse(hdc, his_parm.point_begin.x, his_parm.point_begin.y, his_parm.point_end.x, his_parm.point_end.y);
			break;
		default:
			break;
		}
		DeleteObject(hpen);
	}
}

/**
 * @brief DrawingBoard类,清除屏幕
 * 右键清屏实现
 */
void DrawingBoard::ClearDrawing()
{
	flag_clear = false;
	drawing_parm.clear();
	return;
}

/**
 * @brie 多线程读取文件
 * @param [in] board			画板对象
 */

void ThreadReadXmlFile(DrawingBoard& board)
{
	while (true)
	{
		ErrorShow(ReadXmlFile(board));
		Sleep(10 * 1000);
	}
}

/**
 * @brief 消息回调函数
 * @param [in] hwnd				窗体句柄
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
		//_beginthread(ThreadReadXmlFile, 0, NULL);	std::thread thread_read_xml_file(ThreadReadXmlFile);
		return 0;
	case WM_LBUTTONDOWN:
		point_curr_begin.x = LOWORD(lParam);
		point_curr_begin.y   = HIWORD(lParam);
		//GetCursorPos(&ptShapeBegin); //获取坐标不准确
		return 0;
	case WM_LBUTTONUP:
		point_curr_end.x = LOWORD(lParam);
		point_curr_end.y   = HIWORD(lParam);
		//GetCursorPos(&ptShapeEnd); //获取坐标不准确
		InvalidateRect(hwnd, NULL, TRUE); //关键，使区域无效化，可以调用WM_PAINT函数，不然画不出来
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (board.GetClear() == false)
		{
			board.SetCurrPoint(point_curr_begin, point_curr_end);
			board.Drawing(hwnd, hdc);
		}
		else
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			DrawText(hdc, TEXT("Please move leftbutton to draw, click rightbutton to erase!"), 59, &rect, DT_CENTER | DT_SINGLELINE);
			board.ClearDrawing();
		}
		EndPaint(hwnd, &ps);
		return 0;
	case WM_RBUTTONDOWN:
		board.SetClear();
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
			board.SetType(ShapeParm::TYPE_LINE);
			break;
		case ID_TYPE_RECTANGLE:
			board.SetType(ShapeParm::TYPE_RECT);
			break;
		case ID_TYPE_ELLIPSE:
			board.SetType(ShapeParm::TYPE_ELLIPSE);
			break;
		case ID_COLOR_RED:
			board.SetColor(ColorParm::TYPE_RED);
			break;
		case ID_COLOR_BLUE:
			board.SetColor(ColorParm::TYPE_BLUE);
			break;
		case ID_COLOR_GR:
			board.SetColor(ColorParm::TYPE_GREEN);
			break;
		case ID_LINEWIDTH_THIN:
			board.SetLinewidth(LinewidthParm::TYPE_THIN);
			break;
		case ID_LINEWIDTH_MEDIUM:
			board.SetLinewidth(LinewidthParm::TYPE_MEDIUM);
			break;
		case ID_LINEWIDTH_THICK:
			board.SetLinewidth(LinewidthParm::TYPE_THICK);
			break;
		default:
			break;
			return  DefWindowProc(hwnd, message, wParam, lParam);
		}
		return 0;
	case WM_DESTROY:
		//_endthread(); // 线程退出代码？
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/**
 * @brief 读取XML文件
 * version 2.2 
 * 采用数据库+缓存形式
 * @param [in]  board		画板对象
 * @param [out] board		画板对象
 */
int ReadXmlFile(DrawingBoard& board)
{
	// 先更新数据库
	color_linewidth_database.state = DataState::NO_UPDATE;

	std::string color_value_string;
	std::string linewidth_value_string;
	int* color_transform = new int[3];

	//打开文件
	TiXmlDocument doc;
	if (!doc.LoadFile("xml/config.xml"))
	{
		return ERROR_XML_PATH;
	}
	TiXmlElement* xml_configure = doc.RootElement();
	if (!xml_configure)
	{
		return ERROR_XML_CONFIGURE;
	}
	TiXmlElement* xml_color_configure = xml_configure->FirstChildElement("color_configure");
	TiXmlElement* xml_linewidth_configure = xml_color_configure->NextSiblingElement("linewidth_configure");

	bool flag_get_color = 0;
	bool flag_get_linewidth = 0;

	// 将颜色存入数据库
	TiXmlElement* xml_color = xml_color_configure->FirstChildElement("color");
	while (xml_color != nullptr)
	{
		TiXmlElement* xml_color_name = xml_color->FirstChildElement("name");
		std::string s = xml_color_name->GetText();

		color_value_string = xml_color_name->NextSiblingElement("value")->GetText();
		Transform(color_transform, color_value_string);

		color_linewidth_database.color_cache[s] = new int[3];
		for (int i = 0; i < 3; i++)
		{
			color_linewidth_database.color_cache[s][i] = color_transform[i];
		}

		xml_color = xml_color->NextSiblingElement();
	}

	// 将线条存入数据库
	TiXmlElement* xml_linewidth = xml_linewidth_configure->FirstChildElement("linewidth");
	while (xml_linewidth != nullptr)
	{
		TiXmlElement* xml_linewidth_name = xml_linewidth->FirstChildElement("name");
		std::string s = xml_linewidth_name->GetText();
		color_linewidth_database.linewidth_cache[s] = std::stoi(xml_linewidth_name->NextSiblingElement("value")->GetText());
		xml_linewidth = xml_linewidth->NextSiblingElement();
	}

	// 异常判断
	if (color_linewidth_database.color_cache.empty())
	{
		return ERROR_EMPTY_COLOR;
	}
	if (color_linewidth_database.linewidth_cache.empty())
	{
		return ERROR_EMPTY_LINEWIDTH;
	}

	//数据库已更新
	color_linewidth_database.state = DataState::UPDATEED;
	// 删除缓存
	color_linewidth_cache.deletecache();
	board.SetRead(true);
	return SUCCESS;
}
