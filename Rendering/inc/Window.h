#pragma once

#include <Rendering.h>
#include <RenderTarget.h>
#include <Math/Vector.h>
#include <Input/InputManager.h>
#include <unordered_map>
#include <string>
#include <WindowInput.h>

#ifdef _WIN32
#include <Platform/Windows/PreWindowsInclude.h>
#include <Windows.h>
#include <Platform/Windows/PostWindowsInclude.h>
#endif

enum class RENDERING_API WindowStyle {
	Windowed,
	Borderless,
	Fullscreen
};

struct RENDERING_API WindowParams {
	WindowParams() : size(640, 480), title("Title"), style(WindowStyle::Windowed), VSync(true) {}

	Vector2i size;
	const char* title;
	WindowStyle style;
	bool VSync;
};

class RENDERING_API Window {
public:
	Window(const WindowParams& params);
	virtual ~Window();

	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual bool GetShouldClose() const;
	virtual Vector2i GetSize() const;
	virtual void ProcessEvents();
	virtual void Present();
	virtual RenderTarget& GetRenderTarget();

	InputManager& GetInputManager();

#ifdef _WIN32
	HWND GetHandle() const;
#endif

protected:
	virtual void OnResize(Vector2i newSize);

	bool VSync;

private:
	static std::unordered_map<HWND, Window*> Windows;

	RenderTarget* target;
	WindowInput input;

	Vector2i Size;
	std::string Title;

#ifdef _WIN32
	WNDCLASS windowClass;
	HWND handle;
	bool Closing = false;
#endif

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static Keycode TranslateKeycode(WPARAM wParam, LPARAM lParam);
};