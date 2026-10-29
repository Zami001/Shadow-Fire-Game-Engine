#pragma once

#include <Rendering.h>
#include <MinimalCore.h>

enum class CursorStyle : int {
	None,
	Arrow,
	IBeam,
	Wait,
	Cross,
	UpArrow,
	SizeNWSE,
	SizeNESW,
	SizeWE,
	SizeNS,
	SizeAll,
	No,
	Hand,
	AppStarting,
	Help,
	Pin,
	Person,
	Custom,

	Default = Arrow
};

class RENDERING_API Cursor {
public:
	void SetCursorStyle(CursorStyle NewCursor);
	CursorStyle GetCursorStyle() const;

private:
	bool AutoManaged = false;

};