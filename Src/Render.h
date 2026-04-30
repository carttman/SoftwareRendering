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
 
int		DataLoading();
void	DataRelease();
void	SceneRender();	

void	DrawText(int x, int y, TCHAR* msg, ...);

void BeginScene();
void Clear(COLORREF col);
void EndScene();
void Present();


void SetLineStartPos(POINT pt);
void LineDraw();
void CrossDraw(POINT pt);
void LineUpdate(POINT pt);
/****************** end of file "Render.h" ***********************************/