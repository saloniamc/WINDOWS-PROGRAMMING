#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wnd;

	wnd.cbSize = sizeof(WNDCLASSEX);
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
		MessageBox(NULL, TEXT("This program requires windows NT!"), szAppName, MB_ICONERROR);
		return (0);
	}

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,
		TEXT("The Hello Program"),
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
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		PlaySound(TEXT("C:\\Users\\Saloni\\Documents\\WINDOWS PROGRAMMING\\oxp.wav"), NULL, SND_FILENAME | SND_ASYNC);
		return (0);

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);

		DrawText(hdc, TEXT("Hello, Windows 98!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hwnd, &ps); 
		return (0);

	case WM_DESTROY:
		PostQuitMessage(0);
		return(0);
	}

	return (DefWindowProc(hwnd, message, wParam, lParam));
}

