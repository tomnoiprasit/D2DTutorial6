// D2DTutorial6.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <d2d1.h>
#include <wincodec.h>
#include <wincodecsdk.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "Windowscodecs")
#include "D2DTutorial6.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
	)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

void SetHeroFrameSource(int action, int frame, float& left, float& top, float& right, float& bottom, float& heroWidth, float& heroHeight) {
	switch (action) {
	case 1:
		switch (frame) {
		case 0:
			left = 200.0f; top = 12.0f; right = 252.0f; bottom = 104.0f;
			break;
		case 1:
			left = 249.0f; top = 12.0f; right = 301.0f; bottom = 104.0f;		
			break;
		case 2:
			left = 298.0f; top = 12.0f; right = 350.0f; bottom = 104.0f;
			break;
		case 3:
			left = 347.0f; top = 12.0f; right = 399.0f; bottom = 104.0f;
			break;
		case 4:
			left = 396.0f; top = 12.0f; right = 448.0f; bottom = 104.0f;
			break;
		}
		break;
	default:
		switch (frame) {
		case 0:
			left = 1.0f; top = 12.0f; right = 53.0f; bottom = 104.0f;
			break;
		case 1:
			left = 50.0f; top = 12.0f; right = 102.0f; bottom = 104.0f;		
			break;
		case 2:
			left = 99.0f; top = 12.0f; right = 151.0f; bottom = 104.0f;
			break;
		case 3:
			left = 148.0f; top = 12.0f; right = 200.0f; bottom = 104.0f;
			break;
		}
	}
	heroWidth = right - left + 1;
	heroHeight = top - bottom + 1;
}

void SetUp(HWND hWnd) {
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);

	RECT rc;
	GetClientRect(hWnd, &rc);

	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
		hWnd,
		D2D1::SizeU(
		rc.right - rc.left,
		rc.bottom - rc.top)
		),
		&pD2DHRT
		);

	if (SUCCEEDED(hr)) {
		hr = CoCreateInstance(CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory),
			(void**)(&pWICImagingFactory)
			);

		// Create background bitmap
		hr = pWICImagingFactory->CreateDecoderFromFilename(
			L"D:\\images\\sf2-bg.jpg",
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pWICBitmapDecoder);

		hr = pWICImagingFactory->CreateFormatConverter(&pWICFormatConverter);

		hr = pWICBitmapDecoder->GetFrame(0, &pWICBitmapFrameDecode);

		hr = pWICFormatConverter->Initialize(
			pWICBitmapFrameDecode,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL, 0.f,
			WICBitmapPaletteTypeMedianCut
			);

		hr = pD2DHRT->CreateBitmapFromWicBitmap(
			pWICFormatConverter,
			NULL,
			&pD2DBitMapBackground
			);

		// Create hero bitmap
		hr = pWICImagingFactory->CreateDecoderFromFilename(
			L"D:\\images\\5556.gif",
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pWICBitmapDecoder);

		hr = pWICImagingFactory->CreateFormatConverter(&pWICFormatConverter);

		hr = pWICBitmapDecoder->GetFrame(0, &pWICBitmapFrameDecode);

		hr = pWICFormatConverter->Initialize(
			pWICBitmapFrameDecode,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL, 0.f,
			WICBitmapPaletteTypeMedianCut
			);

		hr = pD2DHRT->CreateBitmapFromWicBitmap(
			pWICFormatConverter,
			NULL,
			&pD2DBitMap
			);
	}
}

void DrawMe(HWND hWnd) {
	SetUp(hWnd);
	D2D_SIZE_F rtSize = pD2DBitMapBackground->GetSize();

	// Calculate source tile location for each frame
	// Handle the animation frame, here	
	dwCurrentTime = GetTickCount();
	dwElapsedTime = dwCurrentTime - dwLastUpdateTime;
	if (dwElapsedTime > FRAME_RATE) {
		dwLastUpdateTime = dwCurrentTime;
		frameNumber++;
		switch (currentHeroAction) {
		case 1:
			globalX += 5;
			if (frameNumber > 4) {
				frameNumber = 0;
				currentHeroAction = 0;
			}
			break;
		default:
			if (frameNumber > 3) frameNumber = 0;
		}
		SetHeroFrameSource(currentHeroAction, frameNumber, srcX1, srcY1, srcX2, srcY2, heroWidth, heroHeight);
	}

	pD2DHRT->BeginDraw();

	pD2DHRT->SetTransform(D2D1::Matrix3x2F::Identity());
	pD2DHRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

	pD2DHRT->DrawBitmap(pD2DBitMapBackground,
		D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height),
		1.0,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height)
		);

	pD2DHRT->DrawBitmap(pD2DBitMap,
		D2D1::RectF(globalX, globalY, globalX + heroWidth*2, globalY + heroHeight*2),
		1.0,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1::RectF(srcX1, srcY1, srcX2, srcY2)
		);

	hr = pD2DHRT->EndDraw();

	ReleaseThemAll();

}

void ReleaseThemAll() {
	SafeRelease(&pD2DBitMapBackground);
	SafeRelease(&pD2DBitMap);
	SafeRelease(&pWICBitmapFrameDecode);
	SafeRelease(&pWICBitmapDecoder);
	SafeRelease(&pWICFormatConverter);
	SafeRelease(&pWICImagingFactory);
	SafeRelease(&pD2DHRT);
	SafeRelease(&pD2DFactory);
}

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_D2DTUTORIAL6, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D2DTUTORIAL6));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2DTUTORIAL6));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_D2DTUTORIAL6);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   SetMenu(hWnd, NULL);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			globalX -= PIXELS_PER_STEP;
			if (globalX < 0) globalX = 0;
			break;
		case VK_RIGHT:
			currentHeroAction = 1;
			frameNumber = 0;
			break;
		case VK_UP:
			globalY -= PIXELS_PER_STEP;
			if (globalY < 0) globalY = 0;
			break;
		case VK_DOWN:
			globalY += PIXELS_PER_STEP;
			if (globalY > APP_HEIGHT) globalY = APP_HEIGHT - HERO_HEIGHT;
			break;
		}
	case WM_DISPLAYCHANGE:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		DrawMe(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
