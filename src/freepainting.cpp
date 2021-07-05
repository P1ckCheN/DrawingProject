// @brief: main file
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.6
// @reversion: last revised by Buckychen on 2021-07-02

#include "freepainting.h"

static DrawingBoard board;
void TaskReadXmlFile(void* pointer_param);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR szCmdLine, _In_ int iCmdShow) {
  MSG msg;
  HWND hwnd;
  WNDCLASS wndclass;
  ThreadPool threadpool;
  static HBRUSH hBrush;
  static TCHAR szAppName[] = TEXT("FreePainting");

  // Exit semaphore should have higher priority
  board.read_and_exit_handle[0] = CreateSemaphore(NULL, 0, 1, NULL);
  // Auto reset timer, otherwise use SetWaitableTimer to reset
  board.read_and_exit_handle[1] = CreateWaitableTimer(NULL, FALSE, NULL);
  
  // Create a thread pool of 2 threads, submit a task into one of them
  ThreadPool::CreateThreadPool(2);
  ThreadPool::SubmitTaskIntoThreadPool(TaskReadXmlFile, NULL);

  // Call timer after one second
  LARGE_INTEGER timer_unit_per_second;
  timer_unit_per_second.QuadPart = 10000000;
  SetWaitableTimer(board.read_and_exit_handle[1], &timer_unit_per_second, 10 * 1000, NULL, NULL, FALSE);

  // Window's propertiy
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
    Error::ErrorShow(ERROR_REGISTER_CLASS);
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

  ThreadPool::CleanThreadPool();
  board.CloseBoardHandle();
  //threadpool.ClearThreadPool();
  return msg.wParam;  
} 

// Task which will submit into the thread pool
void TaskReadXmlFile(void* pointer_param) {
  while (TRUE)
  {
    int read_and_exit_event = WaitForMultipleObjects(2, board.read_and_exit_handle, FALSE, INFINITE);
    if (read_and_exit_event == WAIT_OBJECT_0)
    {
      break;
    }
    // Two kind of timer
    else if (read_and_exit_event == WAIT_OBJECT_0 + 1)
    {
      Error::ErrorShow(ReadXmlFile(board, board.color_linewidth_database, board.color_linewidth_cache));
    }
    else if (read_and_exit_event == WAIT_FAILED)
    {
      Error::ErrorShow(ERROR_HANDLE_WAITOBJECT);
      break;
    }
  }
  return;
}


// Draw lines by using the data from xml files 
void DrawingBoard::Drawing(HWND hwnd, HDC hdc) {
  RECT rect;
  HPEN hpen;
  GetClientRect(hwnd, &rect);
  DrawText(hdc, TEXT("Please move leftbutton to draw, click rightbutton to erase!"), 59, &rect, DT_CENTER | DT_SINGLELINE);

  // Wait for the first read
  while (true)
  {
    if (board.color_linewidth_cache.state != DataState::NO_UPDATE)
      break;
  }

  // If there's no copy, create one and use data to assign to it
  if (board.color_linewidth_cache.state == DataState::DELETED)
  {
    board.color_linewidth_cache = board.color_linewidth_database;
    board.color_linewidth_cache.state = DataState::UPDATEED;
  }

  // Parse copy data and write into local variable
  Error::ErrorShow(ParsingDataCache(type_color, type_linewidth, color, linewidth, board.color_linewidth_cache.color_cache, board.color_linewidth_cache.linewidth_cache));
  DrawingParm curr_parm;
  curr_parm.point_begin = point_begin;
  curr_parm.point_end = point_end;
  curr_parm.shape_parm = type_shape;
  curr_parm.color_parm = { color[0], color[1], color[2] };
  curr_parm.linewidth_parm = linewidth;

  // Save history drawing
  drawing_parm.emplace_back(curr_parm);

  for (DrawingParm& his_parm : drawing_parm)
  {
    hpen = CreatePen(PS_SOLID, his_parm.linewidth_parm, RGB(his_parm.color_parm[0], his_parm.color_parm[1], his_parm.color_parm[2]));
    SelectObject(hdc, hpen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    switch (his_parm.shape_parm)
    {
    // TODO: real-time display
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  HDC hdc;
  PAINTSTRUCT ps;
  int wmId, wmEvent;
  int flag_xml_read = 0;
  switch (message)
  {
  case WM_CREATE:
    //ReleaseSemaphore(semaphore[0], 1, NULL);
    //SetTimer(hwnd, NULL, 10 * 1000, NULL);
    return 0;
  case WM_LBUTTONDOWN:
    board.point_curr_begin.x = LOWORD(lParam);
    board.point_curr_begin.y   = HIWORD(lParam);
    return 0;
  case WM_LBUTTONUP:
    board.point_curr_end.x = LOWORD(lParam);
    board.point_curr_end.y   = HIWORD(lParam);
    InvalidateRect(hwnd, NULL, TRUE);
    return 0;
  case WM_PAINT:
    hdc = BeginPaint(hwnd, &ps);
    if (board.GetClear() == false)
    {
      board.SetCurrPoint();
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
    ReleaseSemaphore(board.read_and_exit_handle[0], 1, NULL);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, message, wParam, lParam);
}


