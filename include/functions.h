#ifndef FUNCTIONS_H__
#define FUNCTIONS_H__
/*
void testmouse()
{
	case WM_RBUTTONDOWN:
		InvalidateRect(hwnd, NULL, false); //��Ч����
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("�Ҽ�������������"), 13, &rect, DT_VCENTER | DT_SINGLELINE);
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_RBUTTONUP:
		InvalidateRect(hwnd, NULL, true); //��Ч���� ˢ��
		return 0;
	case WM_LBUTTONDOWN:
		InvalidateRect(hwnd, NULL, false);
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("�������������"), 13, &rect, DT_CENTER | DT_SINGLELINE);
		ReleaseDC(hwnd, hdc);
		return 0;
	case WM_LBUTTONUP:
		InvalidateRect(hwnd, NULL, true);
		return 0;

		case WM_LBUTTONDOWN:
			iCount = 0;
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		case WM_MOUSEMOVE:
			if (wParam & MK_LBUTTON && iCount < MAXPOINTS)
			{
				pt[iCount].x = LOWORD(lParam);
				pt[iCount++].y = HIWORD(lParam);
				hdc = GetDC(hwnd);
				SetPixel(hdc, LOWORD(lParam), HIWORD(lParam), 0);
				ReleaseDC(hwnd, hdc);
			}
			return 0;
		case WM_LBUTTONUP:
			InvalidateRect(hwnd, NULL, FALSE);
			return 0;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			SetCursor(LoadCursor(NULL, IDC_WAIT));
			ShowCursor(TRUE);
			for (i = 0; i < iCount - 1; i++)
			{
				for (j = i + 1; j < iCount; j++)
				{
					MoveToEx(hdc, pt[i].x, pt[i].y, NULL);
					LineTo(hdc, pt[j].x, pt[j].y);
				}
			}
			ShowCursor(FALSE);
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			EndPaint(hwnd, &ps);
			return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
}
return DefWindowProc(hwnd, message, wParam, lParam);
}*/


#endif // FUNCTIONS_H__
