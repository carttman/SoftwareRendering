//! 
//! \file	Render.h
//! \brief	기본 렌더링/프레임웍 헤더 
//!			DirectX 기준 버전 : DX12
//! 
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com 
//! \date   2020.08.22. Updated. DX12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//!
//
#pragma once
 
//  extern으로 헤더에 이름표 달기
// 헤더파일에서 extern을 사용하면 여러 cpp파일에서 쓸수있는 전역 변수가 된다
extern HWND g_hWnd;
//출발 좌표, 끝 좌표
extern POINT g_Sp,g_Ep;
extern POINT g_Vtx[3];
extern POINT g_VtxTemp;
// ====================================================
extern POINT my_g_Sp;
extern POINT my_g_Ep;
extern POINT my_g_Vtx[3];


int		DataLoading();
void	DataRelease();
void	SceneRender();	

//void	DrawText(int x, int y, TCHAR* msg, ...);
void DrawText(int x, int y, COLORREF col, TCHAR* msg, ...);

void BeginScene();
void Clear(COLORREF col);
//void EndScene();
void Present();

void TriangleDraw();
float EdgeFunction(const POINT& sp, const POINT& ep, const POINT& p);
void LineClear();
void LineDraw();
void LineDraw(POINT sp, POINT ep);

void CrossDraw(POINT pt, TCHAR* name, ...);
void LineUpdate(POINT pt);
void FaceDraw();


int RenderTargetCreate(HWND hwnd);
void RenderTargetRelease();
/****************** end of file "Render.h" ***********************************/