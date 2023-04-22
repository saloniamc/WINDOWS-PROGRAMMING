#include <Windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets3");
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wnd;

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = WndProc;
	wnd.lpszClassName = szAppName;
	wnd.lpszMenuName = NULL;
	wnd.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wnd))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT"), szAppName, MB_ICONERROR);
		return (0);
	}

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,
		TEXT("Get system metrics no. 3"),
		WS_OVERLAPPEDWINDOW | WM_VSCROLL | WM_HSCROLL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxChar, cxCaps, cyChar, cyClient, cxClient, iMaxWidth;
	HDC hdc;
	int i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
	PAINTSTRUCT ps;
	SCROLLINFO si;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc, &tm);

		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);

		//Save the width of the three columns
		iMaxWidth = 40 * cxChar + 22 * cxCaps;
		return (0);

	case WM_SIZE:
		cxClient = LOWORD(wParam);
		cyClient = HIWORD(wParam);

		//Set vertical scroll bar range and page size

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = NUMLINES - 1;
		si.nPage = cyClient / cyChar;
		
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

		//Set horizontal scroll bar range and page size

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 2 + iMaxWidth/cxChar;
		si.nPage = cxClient / cxChar;

		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

		return (0);

	case WM_VSCROLL:

		//Get all vertical scroll bar info

		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hwnd, SB_VERT, &si);

		//Save the position for further comparisons

		iVertPos = si.nPos;

		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			si.nPos -= 1;
			break;

		case SB_LINEDOWN:
			si.nPos += 1;
			break;

		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;

		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;

		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;

		default:
			break;
		}

		//Set the position and then retrieve it . Due to adjustments
		// by windows it may not be the same as the value set

		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hwnd, SB_VERT, &si);

		//If the position has change, scroll the window and update it

		if (si.nPos != iVertPos)
		{
			ScrollWindow(hwnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);

			UpdateWindow(hwnd);
		}

		return (0);

	case WM_HSCROLL:

		//Get all the horizontal scroll bar info

		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;

		//Save the position for comparison later on

		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;

		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			si.nPos -= 1;
			break;

		case SB_LINERIGHT:
			si.nPos += 1;
			break;

		case SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;

		case SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;

		case SB_THUMBPOSITION:
			si.nPos = si.nTrackPos;
			break;
		}

		//Set the position and retrieve it due to adjustments 
		// by windows it may not be the same as value set

		si.fMask = SIF_POS;
		SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		GetScrollInfo(hwnd, SB_HORZ, &si);

		//If the position has changed scroll the window

			if(si.nPos != iHorzPos)
			{
				ScrollWindow(hwnd, cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
				UpdateWindow(hwnd);
			}

		return (0);

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		//Get vertical Scroll Bar info
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_VERT, &si);
		iVertPos = si.nPos;

		//Get horizontal Position bar info
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hwnd, SB_HORZ, &si);
		iHorzPos = si.nPos;

		//Finding painting limits

		iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
		iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);

		for (i = iPaintBeg; i < iPaintEnd; i++)
		{
			x = cxChar * (1 - iHorzPos);
			y = cyChar * (i - iVertPos);

			TextOut(hdc, x, y, sysmetrics[i].szLabel, strlen(sysmetrics[i].szLabel));
			TextOut(hdc, x + 22 * cxCaps, y, sysmetrics[i].szDesc, strlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			
			TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y, szBuffer,
				wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}

		EndPaint(hwnd, &ps);
		return (0);

	case WM_DESTROY:
		PostQuitMessage(0);
		return (0);

	}

	return (DefWindowProc(hwnd, message, wParam, lParam));

}