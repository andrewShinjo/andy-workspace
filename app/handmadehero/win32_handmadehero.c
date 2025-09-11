#include <windows.h>

LRESULT CALLBACK main_window_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;

	switch(message)
	{
		case(WM_SIZE):
		{
			OutputDebugStringA("WM_SIZE\n");
			break;
		}
		case(WM_DESTROY):
		{
			OutputDebugStringA("WM_DESTROY\n");
			break;
		}
		case(WM_PAINT):
		{
			PAINTSTRUCT paint;
			// Prepares the window for painting.
			HDC device_context = BeginPaint(window, &paint);
			RECT rect = paint.rcPaint;
			int x = rect.left;
			int y = rect.top;
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			// Paints the window white.
			PatBlt(device_context, x, y, width, height, WHITENESS);
			// Marks the end of painting the window.
			EndPaint(window, &paint);
			break;
		}
		case(WM_CLOSE):
		{
			OutputDebugStringA("WM_CLOSE\n");
			break;
		}
		case(WM_ACTIVATEAPP):
		{
			OutputDebugStringA("WM_ACTIVATEAPP\n");
			break;
		}
		default:
		{
			// Let Windows provide a default procedure to process the message.
			result = DefWindowProc(window, message, wparam, lparam);
			break;
		}
	}

	return result;
}

// Entry point of the Windows program.
int WINAPI wWinMain(
	HINSTANCE hInstance,     // Handle to an instance. Used to identify an executable when it's loaded into memory.
	HINSTANCE hPrevInstance, // Legacy value with no meaning.
	PWSTR pCmdLine, 		 // Command line arguments as a unicode string.
	int nCmdShow)            // A flag that represents whether the app is minimized, maximized, or shown normally.
{	
	// Define WNDCLASS struct.
	WNDCLASS window_class = {
		.style = 
			CS_OWNDC | 	                           // Allocates a unique device context for each window in the class
			CS_HREDRAW |                           // Redraw the entire window if the width changes
			CS_VREDRAW,                            // Redraws the entire window if the height changes
		.lpfnWndProc = main_window_callback,       // A pointer to the window procedure
		.hInstance = hInstance,                    // A handle to the window instance that contains the window procedure for the class
		.lpszClassName = "HandMadeHeroWindowClass" // The user-defined window class name
	};

	// Registers the window class so that it can be used to create a window.
	RegisterClass(&window_class);

	// Creates a window.
	HWND window_handle = CreateWindowEx(
		0,                                // Extended window style (none for now)
		window_class.lpszClassName,       // The user-defined window class name
		"Handmade Hero",                  // The window name
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style values
		CW_USEDEFAULT,                    // use default x position
		CW_USEDEFAULT,                    // use default y position
		CW_USEDEFAULT,                    // use default width
		CW_USEDEFAULT,                    // use default height
		0,                                // a handle to the parent window (don't have one)
		0,                                // a handle to the menu (don't have one)
		hInstance,                        // a handle to the instance
		0                                 // memory to pass additional data (none for now)
	);

	MSG message;

	// Loop forever
	for(;;)
	{
		// Retrieves a message from the calling thread's queue.
		// If 0, then message is WM_QUIT
		// If -1, then error.
		// Else, then message is anything other than WM_QUIT.
		BOOL message_result = GetMessage(&message, 0, 0, 0);

		if(message_result > 0)
		{
			TranslateMessage(&message); // Translates virtual-key messages into character messages.
			DispatchMessage(&message);  // Sends a message to the window procedure.
		}
		else // if message is WM_QUIT or an error happened:
		{
			break;
		}
	}
	
	
	return 0;
}