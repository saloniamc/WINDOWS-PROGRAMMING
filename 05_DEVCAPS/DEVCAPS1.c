#include <Windows.h>

#define NUMLINES ((int) sizeof(devcaps)/ sizeof(devcaps[0]))

struct
{
	int iIndex;
	TCHAR* szLabel;
	TCHAR* szDesc;
}
devcaps[] =
{
	HORZSIZE,     TEXT("HORZSIZE"),    TEXT("Width in millimeters:"),
	VERTSIZE,     TEXT("VERTSIZE"),    TEXT("Height in millimeters:"),
	HORZRES,      TEXT("HORZRES"),     TEXT("Width in pixels:"),
	VERTRES,      TEXT("VERTRES"),     TEXT("Height in raster colors:"),
	BITSPIXEL,    TEXT("BITSPIXEL"),   TEXT("Color bit per pixels:"),
	PLANES,       TEXT("PLANES"),      TEXT("Number of color planes:"),
	NUMBRUSHES,   TEXT("NUMBRUSHES"),  TEXT("Number of device brushes:"),
	NUMPENS,      TEXT("NUMPENS"),     TEXT("Number of device pens:"),
	NUMMARKERS,   TEXT("NUMMARKERS"),  TEXT("Number of device markers:"),
	NUMFONTS,     TEXT("NUMFONTS"),    TEXT("Number of device fonts:"),
	NUMCOLORS,    TEXT("NUMCOLORS"),   TEXT("Number of device colors:"),
	PDEVICESIZE,  TEXT("PDEVICESIZE"), TEXT("Size of device structure:"),
	ASPECTX,      TEXT("ASPECTX"),     TEXT("Relative width of pixel:"),
	ASPECTY,      TEXT("ASPECTY"),     TEXT("Relative height of pixel:"),
	ASPECTXY,     TEXT("ASPECTXY"),    TEXT("Relative diagonal of pixel:"),
	LOGPIXELSX,   TEXT("LOGPIXELSX"),  TEXT("Horizontal dots per inch:"),
	LOGPIXELSY,   TEXT("LOGPIXELSY"),  TEXT("Vertical dots per inch:"),
	SIZEPALETTE,  TEXT("SIZEPALETTE"), TEXT("Number of pallete entries:"),
	NUMRESERVED,  TEXT("NUMRESERVED"), TEXT("Reserved palette entries:"),
	COLORRES,     TEXT("COLORRES"),    TEXT("Actual color resolution:")
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("DevCaps1");
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
	wnd.lpszClassName = szAppName;
	wnd.lpszMenuName = NULL;
	wnd.lpfnWndProc = WndProc;
	wnd.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wnd))
	{
		MessageBox(NULL, TEXT("This program requires windows NT"), szAppName, MB_ICONERROR);
		return(0);
	}

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,
		TEXT("Device capabilities"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL,
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
	int i, y;
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
		return(0);

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (i = 0; i < NUMLINES; i++)
		{

			TextOut(hdc, 0, cyChar * i, devcaps[i].szLabel, lstrlen(devcaps[i].szLabel));
			TextOut(hdc, 14 * cxCaps, cyChar * i, devcaps[i].szDesc, lstrlen(devcaps[i].szDesc));

			SetTextAlign(hdc, TA_RIGHT | TA_TOP);

			TextOut(hdc, 14 * cxCaps + 35 * cxChar, cyChar * i, szBuffer,
				wsprintf(szBuffer, TEXT("%5d"), GetDeviceCaps(hdc, devcaps[i].iIndex)));

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