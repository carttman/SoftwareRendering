//! 
//! \file	ynAxis.h
//! \brief	공간축 클래스 헤더.
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
//! \class	ynAxis
//! \brief	공간축 표시 클래스	
//
/////////////////////////////////////////////////////////////////////////////
//
class ynAxis		
{
public:

	struct VTX_AXIS 
	{
		VECTOR3 vPos;
		DWORD	Color;
	};

	struct AXISINFO 
	{
		float	length;			//방향축의 길이.
		float	scale;			//비례 스케일
		int		scnX, scnY;		//화면 크기(Viewport 미사용시 적용)
		DWORD	res[20];

		AXISINFO(float len = 1.0f, float s = 1.0f ):length(len), scale(s), scnX(0), scnY(0){} 
	};

	enum 
	{
		FVF_AXIS = B3YFVF_XYZ | B3YFVF_DIFFUSE,
	};	  
	

protected:

	LPDEVICE		m_pDev;
	LPVBUFFER		m_pVB;				//방향축 정점버퍼
	VTX_AXIS*		m_pVtxs;			//방향축 정점들.(로컬)
	VECTOR3			m_vTextPos[3];		//방향축 문자위치(화면)

	MATRIX			m_mTM;				//카메라의 역행로 처리...
	//UINT			m_uLineCnt;
	B3YVIEWPORT		m_ViewPort;
	//BOOL			m_bUseViewPort;
	//MATRIX		m_mOrthoProj;



public:

	ynAxis			(void);
	virtual ~ynAxis	(void);
	 
	virtual int  Init	(LPDEVICE pDev, AXISINFO* pAi);  	 
	virtual void Release();
	virtual int  Update	(float dTime = 0, VECTOR3 vPos = VECTOR3(0,0,0), int width = 0, int height = 0 );		
 	virtual int  Render	(float dTime = 0);

};






/////////////////////////////////////////////////////////////////////////////
//
// 방향축 객체 생성/관리 API 들.
//
int ynAxisCreate(ynAxis** ppAxis, LPDEVICE pDev, int scnX, int scnY, float length = 1.0f, int scale = 1.0f);
int ynAxisCreate(ynAxis** ppAxis, LPDEVICE pDev, ynAxis::AXISINFO* ai);




//! \}	//end of Yena-Helpers
} // end of namespace YENA
/***************** end of file "ynAxis.h" ********************************/
