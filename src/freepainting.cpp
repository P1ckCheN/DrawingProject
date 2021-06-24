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
 * (1) �������ݿ� + ���� Cache Aside Pattern ����
 * ����ʱ���ȶ�����;
 * ���û�л���, �����ݿ�, ȡ�����ݷ��뻺��, ͬʱ������Ӧ;
 * ���µ�ʱ��, �ȸ������ݿ�, ��ɾ������

 * 
 * \author �����
 * \version 2.2
 * \date 2021/06/24
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
	// ������ȡ�ļ����߳�
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
 * @brief DrawingBoard�࣬��������
 */
DrawingBoard::~DrawingBoard()
{
	drawing_parm.clear();
}

/**
 * @brief Cache�࣬���캯��
 * ��ʼ��
 */
Cache::Cache()
{
	state = DataState::NO_UPDATE;
	color_cache.clear();
	color_cache.clear();
}

/**
 * @brief Cache��, ���캯��
 * ��ʼ��
 */
void Cache::deletecache()
{
	state = DataState::DELETED;
	color_cache.clear();
	linewidth_cache.clear();
}

/**
 * @brief Cache��, �������캯��
 * �����Ĭ�Ͽ�������,��Ϊֻ��Ҫ����->����
 * ���Բ�����״̬
 * ��ʼ��
 */
Cache& Cache::operator=(const Cache& cache)
{
	color_cache = cache.color_cache;
	linewidth_cache = cache.linewidth_cache;
	return *this;
}

/**
 * @brief DrawingBoard��, ��ͼ����
 * @param [in] hwnd				������
 * @param [in] hdc				ͼ���豸������
 */
void DrawingBoard::Drawing(HWND hwnd, HDC hdc)
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
	ErrorShow(ParsingDataCache(type_color, type_linewidth, color, linewidth, color_linewidth_cache.color_cache , color_linewidth_cache.linewidth_cache));

	// ����Ϊ֮ǰ�汾��ԭ��ʵ��
	//if (flag_read_xml_file)
		//ErrorShow(ParsingDataAtomic(type_color, type_linewidth, color, linewidth, color_cache_thread, linewidth_cache_thread));
	
	DrawingParm curr_parm;
	curr_parm.point_begin = point_begin;
	curr_parm.point_end = point_end;
	curr_parm.shape_parm = type_shape;
	curr_parm.color_parm = {color[0], color[1], color[2]};
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
 * @brief DrawingBoard��,�����Ļ
 * �Ҽ�����ʵ��
 */
void DrawingBoard::ClearDrawing()
{
	flag_clear = false;
	drawing_parm.clear();
	return;
}

/**
 * @brie ���̶߳�ȡ�ļ�
 * @param [in] board			�������
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
		//_beginthread(ThreadReadXmlFile, 0, NULL);	std::thread thread_read_xml_file(ThreadReadXmlFile);
		return 0;
	case WM_LBUTTONDOWN:
		point_curr_begin.x = LOWORD(lParam);
		point_curr_begin.y   = HIWORD(lParam);
		//GetCursorPos(&ptShapeBegin); //��ȡ���겻׼ȷ
		return 0;
	case WM_LBUTTONUP:
		point_curr_end.x = LOWORD(lParam);
		point_curr_end.y   = HIWORD(lParam);
		//GetCursorPos(&ptShapeEnd); //��ȡ���겻׼ȷ
		InvalidateRect(hwnd, NULL, TRUE); //�ؼ���ʹ������Ч�������Ե���WM_PAINT��������Ȼ��������
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
		//_endthread(); // �߳��˳����룿
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

/**
 * @brief ��ȡXML�ļ�
 * version 2.2 
 * �������ݿ�+������ʽ
 * @param [in]  board		�������
 * @param [out] board		�������
 */
int ReadXmlFile(DrawingBoard& board)
{
	// �ȸ������ݿ�
	color_linewidth_database.state = DataState::NO_UPDATE;

	std::string color_value_string;
	std::string linewidth_value_string;
	int* color_transform = new int[3];

	//���ļ�
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

	// ����ɫ�������ݿ�
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

	// �������������ݿ�
	TiXmlElement* xml_linewidth = xml_linewidth_configure->FirstChildElement("linewidth");
	while (xml_linewidth != nullptr)
	{
		TiXmlElement* xml_linewidth_name = xml_linewidth->FirstChildElement("name");
		std::string s = xml_linewidth_name->GetText();
		color_linewidth_database.linewidth_cache[s] = std::stoi(xml_linewidth_name->NextSiblingElement("value")->GetText());
		xml_linewidth = xml_linewidth->NextSiblingElement();
	}

	// �쳣�ж�
	if (color_linewidth_database.color_cache.empty())
	{
		return ERROR_EMPTY_COLOR;
	}
	if (color_linewidth_database.linewidth_cache.empty())
	{
		return ERROR_EMPTY_LINEWIDTH;
	}

	//���ݿ��Ѹ���
	color_linewidth_database.state = DataState::UPDATEED;
	// ɾ������
	color_linewidth_cache.deletecache();
	board.SetRead(true);
	return SUCCESS;
}
