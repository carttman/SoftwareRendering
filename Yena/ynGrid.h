//! 
//! \file	ynGrid.h
//! \brief	그리드 클래스 헤더.
//! 
//! \author	김기홍 / Kihong Kim / onlysonim@gmail.com
//! \date	2004.06. Kihong Kim / mad_dog@hanmail.net / Blue3D / Hero3D
//! \date	2004.10. update.
//! \date	2010.02. Update. B3Yena / Zero-G Interactive
//! \date	2015.01. Update. for SWRenderer (v1.2)
//! \date	2018.03.05. Updated. C++17  / GCC11 / VS17
//! \date	2020.08.22. Updated. Windows 10 SDK 10.0.x (DX12.x)
//! \date	2024.04.25. Updated. DX12.X / VS22
//!	\date	2025.04.20. Yena SWR 용으로 발췌 및 개정. 
//
#pragma once

#include "Yena.h"
//#include "stdio.h"
#include "assert.h"


//DX 임시사용.
//#include "d3dx9.h" --> DX 를 사용하지 않고 직접 구현합니다.
//#include "dxerr.h"
//typedef D3DXMATRIX	MATRIX;


namespace YENA {
//! \addtogroup Yena-Helpers
//! \{


 

/////////////////////////////////////////////////////////////////////////////
//
//! \class	ynGrid
//! \brief	그리드 : 공간 격자 클래스		
//
/////////////////////////////////////////////////////////////////////////////
//
class ynGrid	
{
public:

	struct VTX_GRID 
	{
		VECTOR3 vPos;
		DWORD	Color;
	};

	struct GRIDINFO 
	{
		int		width; 
		int		height;
		float	scale;
		COLOR	color;
		DWORD	res[20];

		GRIDINFO(int w = 100, int h = 100, float s = 1.0f, 
				COLOR c = COLOR(0.3f, 0.3f, 0.3f, 1.0f))
				:width(w), height(h), scale(s), color(c){}				
	};

	enum 
	{
		FVF_GRID = B3YFVF_XYZ | B3YFVF_DIFFUSE,
	}; 
	

protected:

	LPDEVICE		m_pDev;
	LPVBUFFER		m_pVB;				//그리드 정점버퍼
	VTX_GRID*		m_pGrids;			//그리드 정점들

	MATRIX			m_mTM;
	UINT			m_uLineCnt;
	
	GRIDINFO		m_Info;				//그리드 운용정보



public:

	ynGrid			(void);
	virtual ~ynGrid	(void);

	virtual int  Init	(LPDEVICE pDev, GRIDINFO* pGrid);
	virtual void Release();
	
	virtual int  Update	( float dTime = 0, GRIDINFO* pGrid  = NULL );
	virtual int  Render	( float dTime = 0, float sx = 0.0f, float sy = 0.0f );

	const GRIDINFO& GetInfo() const { return m_Info; }


};





/////////////////////////////////////////////////////////////////////////////
//
// 격자 객체 생성/관리 API 들.
//
int ynGridCreate(ynGrid** ppGrid, LPDEVICE pDev, int width = 100, int height = 100, 
				 float scale = 10.0f, COLOR col = COLOR(0.4f, 0.4f, 0.4f, 1.0f));
int ynGridCreate(ynGrid** ppGrid, LPDEVICE pDev, ynGrid::GRIDINFO* gi);





//! \}	//end of Yena-Helpers
} // end of namespace YENA
/***************** end of file "ynGrid.h" ********************************/
