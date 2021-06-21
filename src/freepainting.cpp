/**
 * \file freepainting.cpp
 * \brief �������
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
 * (4) ʹ��MB�������󣬸�ֱ��
 * 
 * v2.0 2021.6.21
 * (1) ������̶߳�ȡxml�����������ļ�����
 * (2) ����error�������
 * 
 * \author �����
 * \version 2.0
 * \date 2021/06/21
 */


#include "freepainting.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/**
 * @brief WIN32����������
 * @param [in] hInstance		����ǰ���е�ʵ�����
 * @param [in] hPreInstance		ǰһ��ʵ�������NULL
 * @param [in] szCmdLine		���ݸ�����������в���
 * @param [in] iCmdShow			������ʾ�ķ�ʽ
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
 * @brief ���壬ÿ��ˢ�´������»�����ʷͼ��
 * @param [in] hwnd				������
 * @param [in] hdc				ͼ���豸������
 * @param [in] ptBegin			�滭��ʼ��
 * @param [in] ptEnd			�滭��ֹ��
 * @param [in] type_shape		�˵�ѡ���������״
 * @param [in] type_color		�˵�ѡ���ͼ����ɫ
 * @param [in] type_linewidth	�˵�ѡ����������
 * @param [out] draw_parm		���������������
 */
VOID SaveDrawing(HWND hwnd, HDC hdc, POINT ptBegin, POINT ptEnd, 
	std::vector<DrawParm>& draw_parm, ShapeParm type_shape, ColorParm type_color, LinewidthParm type_linewidth)
{
	RECT rect; //ʹ�ò���NULL���棬��ʾ������Ļ
	HPEN hpen;
	
	GetClientRect(hwnd, &rect);
	DrawText(hdc, TEXT("Please move leftbutton to draw, click rightbutton to erase!"), 59, &rect, DT_CENTER | DT_SINGLELINE);
	
	// TODO:�����쳣�������
	
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
	
	// TODO: ��ѭ����ͼ�ٶ�������ͼ�����ܶ����ֻ�ͼ֡�����������
	for (int i = 0; i < draw_parm.size(); i++)
	{
		hpen = CreatePen(PS_SOLID, draw_parm[i].LinewidthParm, RGB(draw_parm[i].color_parm[0], draw_parm[i].color_parm[1], draw_parm[i].color_parm[2]));
		SelectObject(hdc, hpen);
		SelectObject(hdc, GetStockObject(NULL_BRUSH)); // ���п��Կ����Ƿ񻭳����ͼ��
		switch (draw_parm[i].shape_parm)
		{
			// TODO: ʵʱ��ʾ��ͼ���̣���������������
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
 * @brie �����ʷͼ��
 * @param [in] draw_parm		���������������
 */
VOID ClearDrawing(std::vector<DrawParm>& draw_parm)
{
	draw_parm.clear();
	return;
}

/**
 * @brie ���̶߳�ȡ�ļ�
 * @param [in] pvoid			�߳�ָ��
 */
VOID ThreadReadXmlFile(PVOID pvoid)
{
	while (TRUE)
	{
		ErrorShow(ReadXmlFile(color_cache, linewidth_cache));
		flag_readxmlfile = true;
		// TODO:���Ӷ�ʱ�����ܣ�
		Sleep(10 * 1000); //�ӳ�10s
	}
}

/**
 * @brief ��Ϣ�ص�����
 * @param [in] hwnd			������
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
		_beginthread(ThreadReadXmlFile, 0, NULL);
		return 0;
	case WM_LBUTTONDOWN:
		ptShapeBegin.x = LOWORD(lParam);
		ptShapeBegin.y = HIWORD(lParam);
		//GetCursorPos(&ptShapeBegin); //��ȡ���겻׼ȷ
		return 0;
	case WM_LBUTTONUP:
		ptShapeEnd.x = LOWORD(lParam);
		ptShapeEnd.y = HIWORD(lParam);
		//GetCursorPos(&ptShapeEnd); //��ȡ���겻׼ȷ
		
		InvalidateRect(hwnd, NULL, TRUE); //�ؼ���ʹ������Ч�������Ե���WM_PAINT��������Ȼ��������
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
