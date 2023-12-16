#ifdef _WIN32
#include <Windows.h>
#endif

#include <Window.h>
#include <iostream>

std::unordered_map<HWND, Window*> Window::Windows;

Window::Window(const WindowParams& params) : windowClass(), Size(params.size), Title(params.title), VSync(params.VSync), handle(0), manager(nullptr), target(nullptr) {
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

int Window::GetWidth() {
	return Size.x;
}

int Window::GetHeight() {
	return Size.y;
}

bool Window::GetShouldClose() {
	return Closing;
}

Vector2i Window::GetSize() {
	return Size;
}

void Window::ProcessEvents() {
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
	return *manager;
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
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}