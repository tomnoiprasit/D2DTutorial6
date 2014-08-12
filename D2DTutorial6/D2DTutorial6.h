#pragma once

#include "resource.h"
ID2D1Factory * pD2DFactory = NULL;
ID2D1HwndRenderTarget* pD2DHRT = NULL;

IWICImagingFactory* pWICImagingFactory = NULL;
IWICBitmapDecoder* pWICBitmapDecoder = NULL;
IWICFormatConverter* pWICFormatConverter = NULL;
IWICBitmapFrameDecode* pWICBitmapFrameDecode = NULL;

ID2D1Bitmap* pD2DBitMap = NULL;
ID2D1Bitmap* pD2DBitMapBackground = NULL;

HRESULT hr;

template<class Interface>
inline void SafeRelease(
	Interface **
	);

void Setup(HWND);
void ReleaseThemAll();
void DrawMe(HWND);

void SetHeroFrameSource(int, int, float&, float&, float&, float&, float&, float&);

#define PIXELS_PER_STEP 20

#define FRAME_RATE 80
#define HERO_WIDTH 120
#define HERO_HEIGHT 120
#define APP_WIDTH 1024
#define APP_HEIGHT 768

DWORD dwCurrentTime{ 0 };
DWORD dwLastUpdateTime{ 0 };
DWORD dwElapsedTime{ 0 };
float globalX{ 100.0f };
float globalY{ 750.0f };
int frameNumber{ 0 };
float srcX1{ 0 }, srcY1{ 0 }, srcX2{ 0 }, srcY2{ 0 };
float heroWidth{ 92 }, heroHeight{ 92 };
int currentHeroAction{ 0 };