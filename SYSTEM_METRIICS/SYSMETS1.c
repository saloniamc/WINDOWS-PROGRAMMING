#include <windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SYSMETS1");
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wnd;

	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.lpfnWndProc = WndProc;
	wnd.hInstance = hInstance;
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
		TEXT("Get system metrics no 1"),
		WS_OVERLAPPEDWINDOW,
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
	static int cxChar, cxCaps, cyChar;
	HDC hdc;
	int i;
	PAINTSTRUCT ps;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3: 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd, hdc);
		return (0);


	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (i = 0; i < NUMLINES; i++)
		{
			TextOut(hdc, 0, cyChar * i, sysmetrics[i].szLabel, strlen(sysmetrics[i].szLabel));
			TextOut(hdc, 22 * cxCaps, cyChar * i, sysmetrics[i].szDesc, strlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			TextOut(hdc, 22 * cxCaps + 40 * cxChar, cyChar * i, szBuffer,
				wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}

		EndPaint(hwnd, &ps);
		return (0);

	case WM_DESTROY:
		PostQuitMessage(0);

	default:
		break;
	}


	return (DefWindowProc(hwnd, message, wParam, lParam));
}



/* #include <windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets1");
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wnd;

	wnd.cbSize = sizeof(WNDCLASSEX);                      //here an error had occured when wasn't included
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = WndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = hInstance;
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = szAppName;

	if (!RegisterClassEx(&wnd))
	{
		MessageBox(NULL, TEXT("This program requires Winows NT"), szAppName, MB_ICONERROR);

		return (0);
	}

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,
		TEXT("Get system metrics No. 1"),
		WS_OVERLAPPEDWINDOW,
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
	static int cxChar, cxCaps, cyChar;
	HDC hdc;
	int i;
	PAINTSTRUCT ps;
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
		return (0);

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for(i = 0; i < NUMLINES; i++)
		{
			TextOut(hdc, 0, cyChar * i, sysmetrics[i].szLabel, strlen(sysmetrics[i].szLabel));
			TextOut(hdc, 22 * cxCaps, cyChar * i, sysmetrics[i].szDesc, strlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);
			TextOut(hdc, 22 * cxCaps + 40 * cxChar, cyChar * i,szBuffer,
				wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc, TA_LEFT | TA_TOP);
		}

		EndPaint(hwnd, &ps);
		return (0);

	case WM_DESTROY:
		PostQuitMessage(0);
		return (0);
	}

	return(DefWindowProc(hwnd, message, wParam, lParam));

}
*/
