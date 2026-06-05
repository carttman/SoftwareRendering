//! 
//! \file	ynAxis.cpp
//! \brief	공간축 클래스 소스 파일.
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
//#pragma warning(disable:4996)
#include "Yena.h"
#include "ynAxis.h"

#define VIEWPORT_SCALE_  1.0f 

// 문자 출력용 외부 API 들. <Device.h>
void DrawText(int x, int y, COLORREF col, TCHAR* msg, ...);
#define B3DrawText  DrawText

namespace YENA {




/////////////////////////////////////////////////////////////////////////////
//
ynAxis::ynAxis(void)
{
	m_pDev  = NULL;
	m_pVB   = NULL;
	m_pVtxs = NULL;

	//m_bUseViewPort = YES_;
	 
}

ynAxis::~ynAxis(void)
{
	Release();
}




/////////////////////////////////////////////////////////////////////////////
//
void ynAxis::Release()
{
	SafeRelease(m_pVB);
	SafeDelete(m_pVtxs);	
}




/////////////////////////////////////////////////////////////////////////////
//
//! 방향축 객체 초기화   
//! 외부에서 지정된 정보를 기준, 방향축 출력 정보를 구성한다. 
//! 
int ynAxis::Init(LPDEVICE pDev, AXISINFO* pAi)
{
	assert(pDev != NULL);
	assert(pAi != NULL); 
	
	//디바이스 저장.
	m_pDev = pDev;


	//방향축 색상 (DX 형식 0xAARRGGBB)  
	//우리가 만든 DWORD 형변환 연산자는 DWORD(dx) --> B3YXCOLOR 임을 기억할 것.
	//COLOR col = COLOR(0, 0, 0, 1); 
 	#define RED   0xffff0000			//COLOR(1, 0, 0, 1);
	#define GREEN 0xff00ff00			//COLOR(0, 1, 0, 1);
	#define BLUE  0xff0080ff			//COLOR(0, 0.3f, 1, 1);	
	
	//정점개수. (직선x3 + 문자위치3)
	int vtxcnt = 9;	 
	
	//데이터 구성.
	m_pVtxs = new VTX_AXIS[vtxcnt];
	assert(m_pVtxs);
	m_pVtxs[0].vPos  = VECTOR3(0, 0, 0);				//x 축. 라인.
	m_pVtxs[1].vPos  = VECTOR3(pAi->length, 0, 0);
	m_pVtxs[0].Color = RED;		 
	m_pVtxs[1].Color = RED;		 
	 
	m_pVtxs[2].vPos  = VECTOR3(0, 0, 0);				//y 축. 라인.
	m_pVtxs[3].vPos  = VECTOR3(0, pAi->length, 0);
	m_pVtxs[2].Color = GREEN;	 
	m_pVtxs[3].Color = GREEN;	 

	m_pVtxs[4].vPos  = VECTOR3(0, 0, 0);				//z 축. 라인.
	m_pVtxs[5].vPos  = VECTOR3(0, 0, pAi->length);
	m_pVtxs[4].Color = BLUE;	 
	m_pVtxs[5].Color = BLUE;	 

	m_pVtxs[6].vPos  = VECTOR3(pAi->length+0.1f, 0, 0);	//"X"
	m_pVtxs[6].Color = RED;		 
	m_pVtxs[7].vPos  = VECTOR3(0, pAi->length+0.1f, 0);	//"Y"
	m_pVtxs[7].Color = GREEN;	 
	m_pVtxs[8].vPos  = VECTOR3(0, 0, pAi->length+0.1f);	//"Z"
	m_pVtxs[8].Color = BLUE;	 
	 
					
 	

	//정점 버퍼 구성.
	int res = YN_OK;
	int vbsize = vtxcnt * sizeof(VTX_AXIS);
	res = m_pDev->CreateVertexBuffer(vbsize, 0, FVF_AXIS, B3YPOOL_SYSTEMMEM, &m_pVB, 0);
	if(YN_FAILED(res))
	{
		//B3D_Error(NO_, "B3Axis::Init() 실패. VB 생성 실패.");
		return res;
	}

	VTX_AXIS* pVB = NULL;
	res = m_pVB->Lock(0, vbsize, (VOID**)&pVB, 0);
	if(YN_FAILED(res))
	{ 		
		//.. 에러...
		return res;
	}
	::memcpy(pVB, m_pVtxs, vbsize);
	m_pVB->Unlock();


	//전체 라인개수.
	//m_uLineCnt = vtxcnt / 2; 
 
	//뷰포트 구성..
	m_ViewPort.X = 0;
	m_ViewPort.Y = 0;
	m_ViewPort.Width  = (DWORD)(pAi->scnX * VIEWPORT_SCALE_);
	m_ViewPort.Height = (DWORD)(pAi->scnY * VIEWPORT_SCALE_); 
	m_ViewPort.MinZ = 0.0f;
	m_ViewPort.MaxZ = 1.0f;  


	//완료.
	//B3D_Error(NO_, "B3Axis::Init() 성공. 공간축 구성 완료.");

	return TRUE;
}

 



/////////////////////////////////////////////////////////////////////////////
//
//! 월드 좌표 공간에 정렬하여 축 선 보여주기.
//
int ynAxis::Update( 
				  float dTime /*= 0*/,
				  VECTOR3 vPos /*=VECTOR3(0,0,0)*/, 	//출력좌표. (월드) 
				  int width /*=0*/, int height /*=0*/
				  ) 
{
	//if(pDev != NULL)m_pDev = pDev;
	//B3YXMatrixIdentity(&m_mTM); 	 

	//화면크기 산출 : 뷰포트 미적용시...사용.
	if(width != 0)
	{
		m_ViewPort.X = 0;
		m_ViewPort.Y = 0;
		m_ViewPort.Width  = width;
		m_ViewPort.Height = height;
		m_ViewPort.MinZ = 0.0f;
		m_ViewPort.MaxZ = 1.0f; 
	}
	 
 
	//월드 변환 행렬. 
	MATRIX mTM;			B3YXMatrixIdentity(&mTM);
	mTM._41 = vPos.x; 	mTM._42 = vPos.y;	mTM._43 = vPos.z;	
	m_mTM  =  mTM;		
	

	//축방향 문자 출력좌표 계산용 행렬 준비..
	MATRIX mView;	m_pDev->GetTransform(B3YTS_VIEW, &mView);
	MATRIX mProj;	m_pDev->GetTransform(B3YTS_PROJECTION, &mProj);
  	MATRIX mVP; 	B3YXMatrixIdentity(&mVP);
	mVP._11 = m_ViewPort.Width * 0.5f;				 
	mVP._22 = -(m_ViewPort.Height * 0.5f);			 
	mVP._41 = m_ViewPort.X + m_ViewPort.Width * 0.5f;				 
	mVP._42 = m_ViewPort.Y + m_ViewPort.Height * 0.5f;	 

	//축방향 문자 출력좌표 계산용 행렬 누계..
	mTM = mTM *  mView * mProj * mVP;  

	//축방향 문자 출력좌표 최종 계산. 
	for(int i=6, k=0; i<9; i++ , k++) 
		B3YXVec3TransformCoord(&m_vTextPos[k], &m_pVtxs[i].vPos, &mTM);   

	return TRUE;
}

 


/////////////////////////////////////////////////////////////////////////////
//
int ynAxis::Render( float dTime /*= 0*/ )
{ 

	//방향축 문자 출력.
	B3DrawText((int)m_vTextPos[0].x, (int)m_vTextPos[0].y, COLOR(m_pVtxs[6].Color), _T("X"));
	B3DrawText((int)m_vTextPos[1].x, (int)m_vTextPos[1].y, COLOR(m_pVtxs[7].Color), _T("Y"));
	B3DrawText((int)m_vTextPos[2].x, (int)m_vTextPos[2].y, COLOR(m_pVtxs[8].Color), _T("Z"));

  
	//방향축 라인 그리기.
	m_pDev->SetTransform(B3YTS_WORLD, &m_mTM);
	m_pDev->SetStreamSource(0, m_pVB, 0, sizeof(VTX_AXIS)); 
	m_pDev->SetFVF(FVF_AXIS);					 
	m_pDev->DrawPrimitive(B3YPT_LINELIST, 0, 3);  
 	//m_pDev->DrawPrimitiveUP(B3YPT_LINELIST, 3, m_pVtxs, sizeof(VTX_AXIS)); 
  
 	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
//
int ynAxisCreate(ynAxis** ppAxis, LPDEVICE pDev, int scnX, int scnY, float length /* = 1.0f */, int scale /* = 1.0f */)
{
	if(YN_VALIED(*ppAxis))	return YN_FAIL;
	if(YN_INVALIED(pDev))	return YN_FAIL;

	ynAxis::AXISINFO ai;
	ai.length = 1.0f;				//방향축 길이.
	ai.scnX   = scnX;				//방향축 표시 화면 영역.(Width, Pixel)
	ai.scnY   = scnY;				//방향축 표시 화면 영역.(Height, Pixel)	

	ynAxis* pAxis = new ynAxis;
	assert(pAxis);
	pAxis->Init(pDev, &ai);

	*ppAxis = pAxis;				//외부에 복사.

	return YN_OK;
}



/////////////////////////////////////////////////////////////////////////////
//
int ynAxisCreate(ynAxis** ppAxis, LPDEVICE pDev, ynAxis::AXISINFO* ai)
{
	if(YN_VALIED(*ppAxis))	return YN_FAIL;
	if(YN_INVALIED(pDev))	return YN_FAIL;
	if(YN_INVALIED(ai))		return YN_FAIL;

 	ynAxis* pAxis = new ynAxis;
	assert(pAxis);
	pAxis->Init(pDev, ai);

	*ppAxis = pAxis;				//외부에 복사.

	return YN_OK;
}






} // end of namespace YENA

/***************** end of file "ynAxis.cpp" ********************************/













