#pragma once

#ifdef DX11Renderer_EXPORTS
#define DX11_API __declspec(dllexport)
#else
#define DX11_API __declspec(dllimport)
#endif