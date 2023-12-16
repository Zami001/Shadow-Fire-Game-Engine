#pragma once

#include <Core.h>

enum class CORE_API Keycode : unsigned short {
	Backspace = 8,
	Alt = 18,
	LeftArrow = 37,
	UpArrow = 38,
	RightArrow = 39, 
	DownArrow = 40,

};

enum class CORE_API ButtonState {
	Pressed,
	Down,
	Up,
	Released
};