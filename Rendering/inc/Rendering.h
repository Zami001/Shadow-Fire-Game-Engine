#pragma once

#ifdef Rendering_EXPORTS
	#define RENDERING_API __declspec(dllexport)
#else
	#define RENDERING_API __declspec(dllimport)
#endif