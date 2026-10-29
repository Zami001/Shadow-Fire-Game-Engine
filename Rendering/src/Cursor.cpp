#include <Cursor.h>

#ifdef _WIN32
#include <Platform/Windows/PreWindowsInclude.h>
#include <Windows.h>
#include <Platform/Windows/PostWindowsInclude.h>
#endif

bool TestCursor(LPCSTR name) {
	return ::GetCursor() == LoadCursor(NULL, name);
}

void Cursor::SetCursorStyle(CursorStyle NewCursor) {
	SF_ASSERT(NewCursor != CursorStyle::Custom, "Custom cursors cannot be set using the SetCursor method");

	LPCSTR cursor;

	switch (NewCursor) {
		// None requires unique logic, handle it directly instead of mapping to a value
		case CursorStyle::None:
			::SetCursor(NULL);
			return;

		case CursorStyle::Arrow:
			cursor = IDC_ARROW;
			break;

		case CursorStyle::IBeam:
			cursor = IDC_IBEAM;
			break;

		case CursorStyle::Wait:
			cursor = IDC_WAIT;
			break;

		case CursorStyle::Cross:
			cursor = IDC_CROSS;
			break;

		case CursorStyle::UpArrow:
			cursor = IDC_UPARROW;
			break;

		case CursorStyle::SizeNWSE:
			cursor = IDC_SIZENWSE;
			break;

		case CursorStyle::SizeNESW:
			cursor = IDC_SIZENESW;
			break;

		case CursorStyle::SizeWE:
			cursor = IDC_SIZEWE;
			break;

		case CursorStyle::SizeNS:
			cursor = IDC_SIZENS;
			break;

		case CursorStyle::SizeAll:
			cursor = IDC_SIZEALL;
			break;

		case CursorStyle::No:
			cursor = IDC_NO;
			break;

		case CursorStyle::Hand:
			cursor = IDC_HAND;
			break;

		case CursorStyle::AppStarting:
			cursor = IDC_APPSTARTING;
			break;

		case CursorStyle::Help:
			cursor = IDC_HELP;
			break;

		case CursorStyle::Pin:
			cursor = IDC_PIN;
			break;

		case CursorStyle::Person:
			cursor = IDC_PERSON;
			break;
	}

	::SetCursor(LoadCursor(NULL, cursor));
}

CursorStyle Cursor::GetCursorStyle() const {
	HCURSOR currentCursor = ::GetCursor();

	if (::GetCursor() == NULL) return CursorStyle::None;

	if (TestCursor(IDC_ARROW)) return CursorStyle::Arrow;
	if (TestCursor(IDC_IBEAM)) return CursorStyle::IBeam;
	if (TestCursor(IDC_WAIT)) return CursorStyle::Wait;
	if (TestCursor(IDC_CROSS)) return CursorStyle::Cross;
	if (TestCursor(IDC_UPARROW)) return CursorStyle::UpArrow;
	if (TestCursor(IDC_SIZENWSE)) return CursorStyle::SizeNWSE;
	if (TestCursor(IDC_SIZENESW)) return CursorStyle::SizeNESW;
	if (TestCursor(IDC_SIZEWE)) return CursorStyle::SizeWE;
	if (TestCursor(IDC_SIZENS)) return CursorStyle::SizeNS;
	if (TestCursor(IDC_SIZEALL)) return CursorStyle::SizeAll;
	if (TestCursor(IDC_NO)) return CursorStyle::No;
	if (TestCursor(IDC_HAND)) return CursorStyle::Hand;
	if (TestCursor(IDC_APPSTARTING)) return CursorStyle::AppStarting;
	if (TestCursor(IDC_HELP)) return CursorStyle::Help;
	if (TestCursor(IDC_PIN)) return CursorStyle::Pin;
	if (TestCursor(IDC_PERSON)) return CursorStyle::Person;

	return CursorStyle::Custom;
}