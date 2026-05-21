//! 
//! \file	Device.h
//! \brief	예제 렌더링 장치 운용 헤더 
//!	\version Yena SW Renderer v1.5.x 
//! 
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net
//! \date   2012.01.17. v0.9   DX9.x, VS08
//! \date   2012.02.01. v1.0   DX9.x, VS10
//! \date   2013.04.10. v1.2.0 DX9.x, VS08
//! \date   2015.10.10. v1.2.1 DX9.x, VS15
//! \date   2020.08.22. Updated. DX12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//! \date   2025.05.01. v1.5.x, v1.9.0   DX12.X, (VS22)
//! \ingroup Demo
//!
#pragma once
#pragma warning(disable:4996)


///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 장치 인터페이스/클래스 정의
//
///////////////////////////////////////////////////////////////////////////////
// Project Property > C/C++ > Additional Include Directories 에 등록..  
#include "Yena.h"		// Yena SWR v1.5.x 
//#include "Yena2.h"	// Yena SW Renderer v2




///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 인터페이스 재정의.											
// 
///////////////////////////////////////////////////////////////////////////////

typedef IYenaDevice9* LPDEVICE;





///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 장치 운용 함수들
//
///////////////////////////////////////////////////////////////////////////////

int		YenaSetup		(HWND hwnd);
void	YenaRelease		();

/* -> YenaDevice 로 이동..
int		RenderTargetCreate	(HWND hwnd);
void	RenderTargetRelease	();

int		BeginScene		();
int		EndScene		();
int		Clear			(COLORREF col);
int		Present			();
*/

void	PutFPS			(int x, int y); 
float	GetEngineTime	();

//void	DrawText	(int x, int y, TCHAR* msg, ...);
void	DrawText	(int x, int y, COLORREF col, TCHAR* msg, ...);



extern BOOL g_ShowFrame;
extern HWND g_hWnd;
extern YENADISPLAYMODE g_Mode;

extern LPYENADEVICE9 g_pDevice;




///////////////////////////////////////////////////////////////////////////////
//
// SWR 시스템/전역/공통 상수 정의
//
///////////////////////////////////////////////////////////////////////////////

#ifndef YN_OK
#define YN_OK	0
#define YN_FAIL	-1
#define YN_FAILED(v)	(V<0)
#endif

//비동기 키보드 입력 처리.
#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

#ifndef SafeRelease
#define SafeRelease(p)  if((p)){ (p)->Release(); (p) = NULL; }
#define SafeDelete(p)   if((p)){ delete (p);	(p) = NULL;	 }
#define SafeDelArry(p)  if((p)){ delete[](p);	(p) = NULL;	 }
#endif






/**************** end of "Device.h" ***********************************/