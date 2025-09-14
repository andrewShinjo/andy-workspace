#include <stdint.h>
#include <windows.h>

#define global_variable static
#define local_variable static
#define private_function static

global_variable int running = 1;
global_variable BITMAPINFO bitmap_info;
global_variable void *bitmap_memory;
global_variable int bitmap_width;
global_variable int bitmap_height;
global_variable int bytes_per_pixel = 4;

private_function render_weird_gradient(int x_offset, int y_offset)
{
	int pitch = bitmap_width * bytes_per_pixel;
	uint8_t *row = (uint8_t *)bitmap_memory;

	for(int y = 0; y < bitmap_height; y++)
	{
		uint32_t *pixel = (uint32_t*)row;

		for(int x = 0; x < bitmap_width; x++)
		{
			uint8_t blue = (x + x_offset);
			uint8_t green = (y + y_offset);
			*pixel = ((green << 8) | blue);
			pixel++;
		}

		row += pitch;
	}
}

private_function win32_resize_device_independent_bitmap_section(
	int width, 
	int height)
{

	if(bitmap_memory)
	{
		VirtualFree(bitmap_memory, 0, MEM_RELEASE);
	}

	bitmap_width = width;
	bitmap_height = height;

	bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
	bitmap_info.bmiHeader.biWidth = bitmap_width;
	bitmap_info.bmiHeader.biHeight = bitmap_height;
	bitmap_info.bmiHeader.biPlanes = 1;
	bitmap_info.bmiHeader.biBitCount = 32;
	bitmap_info.bmiHeader.biCompression = BI_RGB;

	int bitmap_memory_size = bytes_per_pixel * width * height;
	bitmap_memory = VirtualAlloc(
		0, 
		bitmap_memory_size,
		MEM_COMMIT, 
		PAGE_READWRITE);
}


private_function win32_update_window(
	HDC device_context, 
	RECT client_rect,
	int x, 
	int y, 
	int width, 
	int height)
{
	int window_width = client_rect.right - client_rect.left;
	int window_height = client_rect.bottom - client_rect.top;

	StretchDIBits(
		device_context,
		0, 0, bitmap_width, bitmap_height,
		0, 0, window_width, window_height,
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

			RECT client_rect;
			GetClientRect(window, &client_rect);

			win32_update_window(
				device_context, 
				client_rect, 
				x, 
				y, 
				width, 
				height);

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
	int x_offset = 0, y_offset = 0;

	while(running)
	{
		MSG message;

		if(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if(message.message == WM_QUIT)
			{
				running = 0;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		render_weird_gradient(x_offset, y_offset);

		HDC device_context = GetDC(window_handle);
		RECT client_rect;
		GetClientRect(window_handle, &client_rect);
		int width = client_rect.right - client_rect.left;
		int height = client_rect.bottom - client_rect.top;
		win32_update_window(
			device_context, 
			client_rect,
			0, 
			0, 
			width, 
			height);

		ReleaseDC(window_handle, device_context);
		x_offset++;
		y_offset++;
	}
	
	return 0;
}