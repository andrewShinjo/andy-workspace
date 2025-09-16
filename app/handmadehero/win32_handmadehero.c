#include <stdbool.h>
#include <stdint.h>
#include <windows.h>
#include <Xinput.h>

#define global_variable static
#define local_variable static
#define private_function static

typedef struct 
{
	BITMAPINFO info;
	void *memory;
	int width;
	int height;
	int pitch;
	int bytes_per_pixel;
 } win32_offscreen_buffer;

typedef struct
{
	int width;
	int height;
} win32_window_dimension;

// Explicit function pointer typedefs for C
typedef DWORD WINAPI x_input_get_state(DWORD dwUserIndex, XINPUT_STATE *pState);
typedef DWORD WINAPI x_input_set_state(
	DWORD dwUserIndex, 
	XINPUT_VIBRATION *pVibration
);

// Stub implementations
DWORD WINAPI XInputGetStateStub(DWORD dwUserIndex, XINPUT_STATE *pState)
{
    return 0;
}

DWORD WINAPI XInputSetStateStub(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
{
    return 0;
}

// Function pointers initialized to stubs
global_variable x_input_get_state *XInputGetState_ = XInputGetStateStub;
global_variable x_input_set_state *XInputSetState_ = XInputSetStateStub;

// Macro overrides to redirect calls
#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

private_function void win32_load_xinput()
{
	HMODULE x_input_library = LoadLibraryA("xinput1_3.dll");

	if(x_input_library)
	{
		XInputGetState = (x_input_get_state*)
			GetProcAddress(x_input_library, "XInputGetState");
		XInputSetState = (x_input_set_state*)
			GetProcAddress(x_input_library, "XInputSetState");
	}
}

global_variable int running = 1;
global_variable win32_offscreen_buffer global_backbuffer;

private_function win32_window_dimension get_window_dimension(HWND window)
{
	RECT rect_client;
	GetClientRect(window, &rect_client);
	int width = rect_client.right - rect_client.left;
	int height = rect_client.bottom - rect_client.top;
	return (win32_window_dimension) {
		.width = width,
		.height = height
	};
}

private_function void render_weird_gradient(
	win32_offscreen_buffer *offscreen_buffer, 
	int x_offset, 
	int y_offset)
{
	uint8_t *row = (uint8_t *)offscreen_buffer->memory;

	for(int y = 0; y < offscreen_buffer->height; y++)
	{
		uint32_t *pixel = (uint32_t*)row;

		for(int x = 0; x < offscreen_buffer->width; x++)
		{
			uint8_t blue = (x + x_offset);
			uint8_t green = (y + y_offset);
			*pixel = ((green << 8) | blue);
			pixel++;
		}

		row += offscreen_buffer->pitch;
	}
}

private_function win32_resize_device_independent_bitmap_section(
	win32_offscreen_buffer *offscreen_buffer,
	int width, 
	int height)
{

	if(offscreen_buffer->memory)
	{
		VirtualFree(offscreen_buffer->memory, 0, MEM_RELEASE);
	}

	offscreen_buffer->width = width;
	offscreen_buffer->height = height;
	offscreen_buffer->bytes_per_pixel = 4;

	offscreen_buffer->info.bmiHeader.biSize = sizeof(
		offscreen_buffer->info.bmiHeader);
	offscreen_buffer->info.bmiHeader.biWidth = offscreen_buffer->width;
	offscreen_buffer->info.bmiHeader.biHeight = offscreen_buffer->height;
	offscreen_buffer->info.bmiHeader.biPlanes = 1;
	offscreen_buffer->info.bmiHeader.biBitCount = 32;
	offscreen_buffer->info.bmiHeader.biCompression = BI_RGB;

	int bitmap_memory_size = offscreen_buffer->bytes_per_pixel *
		offscreen_buffer->width * offscreen_buffer->height;
	offscreen_buffer->memory = VirtualAlloc(
		0, 
		bitmap_memory_size,
		MEM_COMMIT, 
		PAGE_READWRITE);
	offscreen_buffer->pitch = offscreen_buffer->width * 
		offscreen_buffer->bytes_per_pixel;
}


private_function win32_display_buffer_in_window(
	HDC device_context, 
	int window_width, int window_height,
	win32_offscreen_buffer *offscreen_buffer,
	int x, 
	int y, 
	int width, 
	int height)
{
	StretchDIBits(
		device_context,
		0, 0, window_width, window_height,
		0, 0, offscreen_buffer->width, offscreen_buffer->height,
		offscreen_buffer->memory,
		&offscreen_buffer->info,
		DIB_RGB_COLORS,
		SRCCOPY
	);
}

private_function LRESULT CALLBACK win32_main_window_callback(
	HWND window_handle, 
	UINT message, 
	WPARAM wparam, 
	LPARAM lparam)
{
	LRESULT result = 0;

	switch(message)
	{
		case(WM_SIZE):
		{
			break;
		}
		case(WM_DESTROY):
		{
			running = 0;
			break;
		}
		case(WM_KEYDOWN):
		{
			break;
		}
		case(WM_KEYUP):
		{
			break;
		}
		case(WM_SYSKEYDOWN):
		{
			uint32_t vk_code = wparam;
			bool was_down = ((lparam & (1 << 30)) != 0);
			bool is_down = ((lparam & (1 << 31)) == 0);
			if(vk_code == 'W')
			{

			}
			else if(vk_code == 'A')
			{

			}
			else if(vk_code == 'S')
			{
				
			}
			else if(vk_code == 'D')
			{
				
			}
			else if(vk_code == 'Q')
			{
				
			}
			else if(vk_code == 'E')
			{
				
			}
			else if(vk_code == VK_UP)
			{
				
			}
			else if(vk_code == VK_LEFT)
			{
				
			}
			else if(vk_code == VK_RIGHT)
			{
				
			}
			else if(vk_code == VK_DOWN)
			{
				
			}
			else if(vk_code == VK_SPACE)
			{
				
			}
			else if(vk_code == VK_ESCAPE)
			{
				
			}
			break;
		}
		case(WM_SYSKEYUP):
		{
			break;
		}
		case(WM_PAINT):
		{
			PAINTSTRUCT paint;
			HDC device_context = BeginPaint(window_handle, &paint);
			RECT rect = paint.rcPaint;
			int x = rect.left;
			int y = rect.top;
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			
			win32_window_dimension window_dimensions =
				get_window_dimension(window_handle);

			win32_display_buffer_in_window(
				device_context, 
				window_dimensions.width,
				window_dimensions.height,
				&global_backbuffer,
				x, 
				y, 
				width, 
				height);

			EndPaint(window_handle, &paint);
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
			result = DefWindowProc(window_handle, message, wparam, lparam);
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
	win32_load_xinput();

	win32_resize_device_independent_bitmap_section(
		&global_backbuffer,
		1280, 
		720
	);

	WNDCLASSA window_class = {
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

		while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if(message.message == WM_QUIT)
			{
				running = 0;
			}
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		for(DWORD controller_index = 0; 
			controller_index < XUSER_MAX_COUNT; 
			controller_index++)
		{
			XINPUT_STATE controller_state;
			if(XInputGetState(controller_index, &controller_state) == 
				ERROR_SUCCESS)
			{
				// The controller is plugged in
				XINPUT_GAMEPAD *gamepad = &controller_state.Gamepad;
				bool dpad_up = gamepad->wButtons && XINPUT_GAMEPAD_DPAD_UP;
				bool dpad_down = gamepad->wButtons && XINPUT_GAMEPAD_DPAD_DOWN;
				bool dpad_left = gamepad->wButtons && XINPUT_GAMEPAD_DPAD_LEFT;
				bool dpad_right = gamepad->wButtons && 
					XINPUT_GAMEPAD_DPAD_RIGHT;
				bool start = gamepad->wButtons && XINPUT_GAMEPAD_START;
				bool back = gamepad->wButtons && XINPUT_GAMEPAD_BACK;
				bool left_shoulder = gamepad->wButtons && 
					XINPUT_GAMEPAD_LEFT_SHOULDER;
				bool right_shoulder = gamepad->wButtons && 
					XINPUT_GAMEPAD_RIGHT_SHOULDER;
				bool a_button_pressed = gamepad->wButtons && XINPUT_GAMEPAD_A;
				bool b = gamepad->wButtons && XINPUT_GAMEPAD_B;
				bool x = gamepad->wButtons && XINPUT_GAMEPAD_X;
				bool y = gamepad->wButtons && XINPUT_GAMEPAD_Y;

				int16_t stick_x = gamepad->sThumbLX;
				int16_t stick_y = gamepad->sThumbLY;

				if(a_button_pressed)
				{
					y_offset += 2;
				}
			}
			else
			{
				// The controller isn't plugged in
			}
		}

		XINPUT_VIBRATION vibration;
		vibration.wLeftMotorSpeed = 60000;
		vibration.wRightMotorSpeed = 60000;
		XInputSetState(0, &vibration);

		render_weird_gradient(&global_backbuffer, x_offset, y_offset);

		HDC device_context = GetDC(window_handle);
		RECT client_rect;
			win32_window_dimension window_dimension =  
				get_window_dimension(window_handle);
			
		win32_display_buffer_in_window(
			device_context, 
			window_dimension.width,
			window_dimension.height,
			&global_backbuffer,
			0, 
			0, 
			window_dimension.width, 
			window_dimension.height);

		ReleaseDC(window_handle, device_context);
		x_offset++;
	}
	
	return 0;
}