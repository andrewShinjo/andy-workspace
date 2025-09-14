#include <windows.h>

#define global_variable static
#define local_variable static
#define private_function static

global_variable int running = 1;
global_variable BITMAPINFO bitmap_info;
global_variable void *bitmap_memory;
global_variable HBITMAP bitmap_handle;
global_variable HDC bitmap_device_context;

private_function win32_resize_device_independent_bitmap_section(
	int width, 
	int height)
{
	if(bitmap_handle)
	{
		DeleteObject(bitmap_handle);
	}
	else
	{
		bitmap_device_context = CreateCompatibleDC(0);
	}

	bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
	bitmap_info.bmiHeader.biWidth = width;
	bitmap_info.bmiHeader.biHeight = height;
	bitmap_info.bmiHeader.biPlanes = 1;
	bitmap_info.bmiHeader.biBitCount = 32;
	bitmap_info.bmiHeader.biCompression = BI_RGB;

	bitmap_handle = CreateDIBSection(
		bitmap_device_context,
		&bitmap_info,
		DIB_RGB_COLORS,
		&bitmap_memory,
		0,
		0
	);

	ReleaseDC(0, bitmap_device_context);
}


private_function win32_update_window(
	HDC device_context, 
	int x, 
	int y, 
	int width, 
	int height)
{
	StretchDIBits(
		device_context,
		x, y, width, height,
		x, y, width, height,
		bitmap_memory,
		&bitmap_info,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

LRESULT CALLBACK win32_main_window_callback(
	HWND window, 
	UINT message, 
	WPARAM wparam, 
	LPARAM lparam)
{
	LRESULT result = 0;

	switch(message)
	{
		case(WM_SIZE):
		{
			RECT client_rect;
			GetClientRect(window, &client_rect);
			int width = client_rect.right - client_rect.left;
			int height = client_rect.bottom - client_rect.top;
			win32_resize_device_independent_bitmap_section(width, height);
			break;
		}
		case(WM_DESTROY):
		{
			running = 0;
			break;
		}
		case(WM_PAINT):
		{
			PAINTSTRUCT paint;
			HDC device_context = BeginPaint(window, &paint);
			RECT rect = paint.rcPaint;
			int x = rect.left;
			int y = rect.top;
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			win32_update_window(device_context, x, y, width, height);
			EndPaint(window, &paint);
			break;
		}
		case(WM_CLOSE):
		{
			running = 0;
			break;
		}
		case(WM_ACTIVATEAPP):
		{
			break;
		}
		default:
		{
			result = DefWindowProc(window, message, wparam, lparam);
			break;
		}
	}

	return result;
}

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine,
	int nCmdShow)
{	
	WNDCLASS window_class = {
		.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = win32_main_window_callback,
		.hInstance = hInstance,                   
		.lpszClassName = "HandMadeHeroWindowClass" 
	};

	RegisterClass(&window_class);

	HWND window_handle = CreateWindowEx(
		0,
		window_class.lpszClassName,
		"Handmade Hero",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		hInstance,
		0
	);

	running = 1;

	MSG message;

	while(running)
	{
		BOOL message_result = GetMessage(&message, 0, 0, 0);

		if(message_result > 0)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			break;
		}
	}
	
	return 0;
}