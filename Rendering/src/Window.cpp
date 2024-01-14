#ifdef _WIN32
#include <Windows.h>
#include <windowsx.h>
#include <hidusage.h>
#endif

#include <Window.h>
#include <iostream>

std::unordered_map<HWND, Window*> Window::Windows;

Window::Window(const WindowParams& params) : windowClass(), Size(params.size), Title(params.title), VSync(params.VSync), handle(0), input(), target(nullptr) {
	windowClass = {};
	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.lpszClassName = "ShadowFireWindow";

	if (RegisterClass(&windowClass) == 0) {
		std::cout << "RegisterClass failed: " << GetLastError() << "\n";
		return;
	}

	unsigned long WindowStyle;
	switch (params.style) {
		case WindowStyle::Fullscreen:
		case WindowStyle::Borderless:
			WindowStyle = WS_POPUP;
			break;

		case WindowStyle::Windowed:
		default:
			WindowStyle = WS_OVERLAPPEDWINDOW;
			break;
	}

	handle = CreateWindowEx(CS_OWNDC, "ShadowFireWindow", Title.c_str(), WindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, Size.x, Size.y, NULL, NULL, windowClass.hInstance, NULL);
	
	if (handle == 0) {
		SF_LOG(LogWindow, Error, "CreateWindow failed: error code %i", GetLastError())
		return;
	}

	Windows.insert({ handle, this });

	ShowWindow(handle, SW_NORMAL);
}

Window::~Window() {
	Windows.erase(handle);
}

int Window::GetWidth() const {
	return Size.x;
}

int Window::GetHeight() const {
	return Size.y;
}

bool Window::GetShouldClose() const {
	return Closing;
}

Vector2i Window::GetSize() const {
	return Size;
}

void Window::ProcessEvents() {
	input.UpdateControllerInput();

	MSG msg = {};
	if (PeekMessage(&msg, handle, 0, 0, PM_REMOVE) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::Present() {

}

RenderTarget& Window::GetRenderTarget() {
	return *target;
}

InputManager& Window::GetInputManager() {
	return input;
}

#ifdef _WIN32
HWND Window::GetHandle() const {
	return handle;
}
#endif

void Window::OnResize(Vector2i newSize) {
	Size = newSize;
}

LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto windowIterator = Windows.find(hwnd);
	if (windowIterator == Windows.end()) {
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	Window* window = windowIterator->second;
	switch (msg) {
		case WM_QUIT:
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			window->Closing = true;
			break;
		case WM_SIZE:
			window->OnResize({ LOWORD(lParam), HIWORD(lParam) });
			break;
		case WM_MOUSEMOVE:
			window->input.SetMousePosition({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
			break;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			window->input.SetButtonDown(TranslateKeycode(wParam, lParam));
			break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
			window->input.SetButtonUp(TranslateKeycode(wParam, lParam));
			break;

		case WM_LBUTTONDOWN:
			window->input.SetButtonDown(Keycode::MouseLeft);
			break;
		case WM_LBUTTONUP:
			window->input.SetButtonUp(Keycode::MouseLeft);
			break;
		case WM_RBUTTONDOWN:
			window->input.SetButtonDown(Keycode::MouseRight);
			break;
		case WM_RBUTTONUP:
			window->input.SetButtonUp(Keycode::MouseRight);
			break;
		case WM_MBUTTONDOWN:
			window->input.SetButtonDown(Keycode::MouseMiddle);
			break;
		case WM_MBUTTONUP:
			window->input.SetButtonUp(Keycode::MouseMiddle);
			break;
		case WM_XBUTTONDOWN:
			if (HIWORD(wParam) == XBUTTON1) {
				window->input.SetButtonUp(Keycode::XButton1);
			} else {
				window->input.SetButtonUp(Keycode::XButton2);
			}
			break;

		case WM_XBUTTONUP:
			if (HIWORD(wParam) == XBUTTON1) {
				window->input.SetButtonDown(Keycode::XButton1);
			} else {
				window->input.SetButtonDown(Keycode::XButton2);
			}

		case WM_MOUSEWHEEL:
			window->input.MouseWheelMoved(static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA));
			break;
		case WM_MOUSEHWHEEL:
			window->input.HorizontalMouseWheelMoved(static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA));
			break;
			
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

Keycode Window::TranslateKeycode(WPARAM wParam, LPARAM lParam) {
	unsigned int scancode = (lParam & 0x00ff0000) >> 16;
	bool extendedbit = 0x01000000 & lParam;

	switch (static_cast<Keycode>(wParam)) {
		case Keycode::ShiftKey:
			return static_cast<Keycode>(MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX));

		case Keycode::AltKey:
			return extendedbit ? Keycode::RMenu : Keycode::LMenu;

		case Keycode::ControlKey:
			return extendedbit ? Keycode::RControlKey : Keycode::LControlKey;

		default:
			return static_cast<Keycode>(wParam);
	}
}