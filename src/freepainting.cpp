/**
 * \file freepainting.cpp
 * \brief ����������
 *
 * ������
 * v1.1 2021.6.19
 * (1) �޸���ͼʱ��ʼ����յ㲻׼������
 * (2) �޸��Ŵ���С���ڻ�����ۼ�������
 * (3) ���õ��������ֲ�ͬ�Ļ�ͼ�����ṹ
 * 
 * v1.2 2021.6.20
 * (1) ����������ʾ����ʾλ��
 * (2) �����Ҽ�����޷���������
 * (3) ���Կ����Ƿ����ͼ��
 * 
 * v2.0 2021.6.21
 * (1) ������̶߳�ȡxml�����������ļ�����
 * (2) ʹ��MB�������󣬸�ֱ��
 * (3) ����error�������
 * 
 * v2.1 2021.6.23
 * (1) ��ӻ�����
 * (2) ��Ӷ��̷߳��ʿ��ƣ�ԭ��������
 * 
 * v2.2 2021.6.23
 * (1) ������ݴ洢��
 * (2) �������ݿ� + ���� Cache Aside Pattern ����
 * ����ʱ���ȶ�����;
 * ���û�л���, �����ݿ�, ȡ�����ݷ��뻺��, ͬʱ������Ӧ;
 * ���µ�ʱ��, �ȸ������ݿ�, ��ɾ������
 * (3) RCU������
 * 
 * v2.3 2021.6.29
 * (1) ���ʹ�����߳����ڶ��߳��˳�����
 * (2) ����CPUռ�ø�or�������صĶ�ѡһ����
 * \author �����
 * \version 2.2
 * \date 2021/06/24
 */

#include "freepainting.h"

 // ȫ�ֱ�����
 // �����Է���ͷ�ļ��У�������ظ�������
static bool			main_thread_exit = false;							///< ���߳��˳���־
static POINT		point_curr_begin;									///< ��ʼ��
static POINT		point_curr_end;										///< ��ֹ��
static Cache		color_linewidth_cache;								///< ����
static Cache		color_linewidth_database;							///< ���ݿ�
static DrawingBoard board;												///< �������

/**
 * @brief WIN32����������
 * @param [in] hInstance		����ǰ���е�ʵ�����
 * @param [in] hPreInstance		ǰһ��ʵ�������NULL
 * @param [in] szCmdLine		���ݸ�����������в���
 * @param [in] iCmdShow			������ʾ�ķ�ʽ
 */
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR szCmdLine, _In_ int iCmdShow)
{
	MSG msg;
	HWND hwnd;
	WNDCLASS wndclass;
	static HBRUSH hBrush;
	static TCHAR szAppName[] = TEXT("FreePainting");

	// ������ȡ�ļ����߳�, 1.win32 ������2.C++11 thread��
	HANDLE thread_h;
	unsigned thread_id;
	thread_h = (HANDLE)_beginthreadex(NULL, 0, ThreadReadXmlFile, NULL, 0, &thread_id);
	//std::thread thread_read_xml_file(ThreadReadXmlFile, std::ref(board));
	//thread_read_xml_file.detach();
	
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
	WaitForSingleObject(thread_h, INFINITE);
	CloseHandle(thread_h);
	return msg.wParam;	
} 

/**
 * @brie ���̶߳�ȡ�ļ�
 * @param [in] p_parm						����ָ��
 */
unsigned int WINAPI ThreadReadXmlFile(void *p_parm)
{
	while (true && !main_thread_exit)
	{
		// CPUռ�ø�
		/*ErrorShow(ReadXmlFile(board, color_linewidth_database, color_linewidth_cache));
		for (int i = 0; i < 10; ++i)
		{
			if (main_thread_exit)
			{
				return 0;
			}
			Sleep(1000);
		}*/

		// ��Ӧ��
		ErrorShow(ReadXmlFile(board, color_linewidth_database, color_linewidth_cache));
		Sleep(10*1000);
	}
	return 0;
}

/**
 * @brief DrawingBoard�ࣺ��ͼ����
 * @param [in] hwnd							������
 * @param [in] hdc							ͼ���豸������
 * @param [in] color_linewidth_database		���ݿ�
 * @param [in] color_linewidth_cache		����
 */
void DrawingBoard::Drawing(HWND hwnd, HDC hdc, Cache& color_linewidth_database, Cache& color_linewidth_cache)
{
	RECT rect;
	HPEN hpen;
	GetClientRect(hwnd, &rect);
	DrawText(hdc, TEXT("Please move leftbutton to draw, click rightbutton to erase!"), 59, &rect, DT_CENTER | DT_SINGLELINE);

	// �����ʼδ�������ݿ���δд�뻺������
	while (true)
	{
		if (color_linewidth_cache.state != DataState::NO_UPDATE)
			break;
	}

	// ������㻺�治����,�����ݿ⿽��������
	if (color_linewidth_cache.state == DataState::DELETED)
	{
		color_linewidth_cache = color_linewidth_database;
		color_linewidth_cache.state = DataState::UPDATEED;
	}

	// ʹ�û������ݲ�����ͼ

	ErrorShow(ParsingDataCache(type_color, type_linewidth, color, linewidth, color_linewidth_cache.color_cache, color_linewidth_cache.linewidth_cache));

	// ����Ϊ֮ǰ�汾��ԭ��ʵ��
	//if (flag_read_xml_file)
		//ErrorShow(ParsingDataAtomic(type_color, type_linewidth, color, linewidth, color_cache_thread, linewidth_cache_thread));

	DrawingParm curr_parm;
	curr_parm.point_begin = point_begin;
	curr_parm.point_end = point_end;
	curr_parm.shape_parm = type_shape;
	curr_parm.color_parm = { color[0], color[1], color[2] };
	curr_parm.linewidth_parm = linewidth;

	drawing_parm.emplace_back(curr_parm); // ������ʷ��ͼ

	for (DrawingParm& his_parm : drawing_parm)
	{
		hpen = CreatePen(PS_SOLID, his_parm.linewidth_parm, RGB(his_parm.color_parm[0], his_parm.color_parm[1], his_parm.color_parm[2]));
		SelectObject(hdc, hpen);
		SelectObject(hdc, GetStockObject(NULL_BRUSH)); // ���п��Կ����Ƿ񻭳����ͼ��
		switch (his_parm.shape_parm)
		{
			// TODO: ʵʱ��ʾ��ͼ���̣���������������
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
 * @brief ��Ϣ�ص�����
 * @param [in] hwnd				������
 * @param [in] message			��Ϣ������ʶ��
 * @param [in] wParam			��Ϣ�йصĳ���ֵ�����ߴ��ںͿؼ��ľ��
 * @param [in] lParam			ָ���ڴ������ݵ�ָ��
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
		return 0;
	case WM_LBUTTONDOWN:
		point_curr_begin.x = LOWORD(lParam);
		point_curr_begin.y   = HIWORD(lParam);
		return 0;
	case WM_LBUTTONUP:
		point_curr_end.x = LOWORD(lParam);
		point_curr_end.y   = HIWORD(lParam);
		InvalidateRect(hwnd, NULL, TRUE);		//�ؼ���ʹ������Ч��������WM_PAINT��������Ȼ��������
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if (board.GetClear() == false)
		{
			board.SetCurrPoint(point_curr_begin, point_curr_end);
			board.Drawing(hwnd, hdc, color_linewidth_database, color_linewidth_cache);
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
		main_thread_exit = true;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


