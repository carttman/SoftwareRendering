//! 
//! \file	ynB3Class.h
//! \brief	Yena 클래스 선행 선언 헤더. 
//!	\version Yena S/W Renderer v1.5.x  
//! 
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net
//! \date   2012.01.17. v0.9   DX9.x, VS08
//! \date   2012.02.01. v1.0   DX9.x, VS10
//! \date   2013.04.10. v1.2.0 DX9.x, VS08
//! \date   2015.10.10. v1.2.1 DX9.x, VS15
//! \date   2020.08.22. Updated. DX12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//! \date   2025.05.01. v1.5.x, v1.9.0   DX12.X, (VS22)
// 
#pragma once
//#pragma warning(disable:4996)


/////////////////////////////////////////////////////////////////////////////// 
//
// Yena B3 클래스 선행 선언
//
/////////////////////////////////////////////////////////////////////////////// 

class	B3YenaDevice9;
//typedef B3YenaDevice9  B3YENADEVICE9;		//DX9 대응.
typedef B3YenaDevice9* LPB3YENADEVICE9;		//DX9 대응.
//typedef LPB3YENADEVICE9  LPDEVICE;

/*
class	B3YenaDevice9x;
typedef B3YenaDevice9x* LPB3YENADEVICE9;	//DX9 대응.
typedef LPB3YENADEVICE9  LPDEVICE;
*/

/*
class	B3YenaSurface;
typedef B3YenaSurface* LPB3YENASURFACE;

class	B3YenaRenderTarget;
typedef B3YenaRenderTarget* LPB3YENARENDERTARGET;

class	B3YenaDepthStencil;
typedef B3YenaDepthStencil* LPB3YENADEPTHSTENCIL;
*/
class	B3YenaVertexBuffer9;
typedef B3YenaVertexBuffer9* LPB3YENAVERTEXBUFFER9;
//typedef LPB3YENAVERTEXBUFFER9 LPVBUFFER;


class	B3YenaGraphicsEngine9;
typedef B3YenaGraphicsEngine9	B3YenaGraphics;				//DX9 대응.
typedef B3YenaGraphicsEngine9*	LPB3YENAGRAPHICSENGINE9;	//DX9 대응.
//typedef LPB3YENAGRAPHICSENGINE9	LPGRAPHICS;



//#define YENA_VERSION  9		//DX9.0




/**************** end of "ynB3Class.h" ***********************************/
