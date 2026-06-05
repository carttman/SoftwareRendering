//! 
//! \file	ynGrid.cpp
//! \brief	그리드 클래스 소스 파일.
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
#include "Yena.h"
#include "ynGrid.h"

namespace YENA {




/////////////////////////////////////////////////////////////////////////////
//
ynGrid::ynGrid(void)
{
	m_pDev	 = NULL;
	m_pVB	 = NULL;
	m_pGrids = NULL;

	m_uLineCnt = 0;
	B3YXMatrixIdentity(&m_mTM); 

}


ynGrid::~ynGrid(void)
{
	Release();
}




/////////////////////////////////////////////////////////////////////////////
//
void ynGrid::Release()
{
	SafeRelease(m_pVB);
	SafeDelete(m_pGrids);	
}




/////////////////////////////////////////////////////////////////////////////
//
//! 그리드 객체 초기화   
//! 외부에서 지정된 정보를 기준, 격자와 기타 정보를 구성한다.  
//
int ynGrid::Init(LPDEVICE pDev, GRIDINFO* pGrid)
{
	assert(pDev != NULL);
	assert(pGrid != NULL);

	//외부 지정 정보 저장. 
	m_Info = *pGrid;
	m_pDev = pDev;


	//정적 버퍼 생성.
	int xcnt = (pGrid->width / (int)pGrid->scale) + 1;				//가로/세로별 라인 개수.
	int ycnt = (pGrid->width / (int)pGrid->scale) + 1;
	int vtxcnt = (xcnt + ycnt) * 2;									//정점개수.

	m_pGrids = new VTX_GRID[vtxcnt];
	assert(m_pGrids);
	ZeroMemory(m_pGrids, sizeof(VTX_GRID) * vtxcnt);


	//그리드 시작 위치. (기본. 원점(0,0,0))
	float hx = (pGrid->width * 0.5f);
	float hy = (pGrid->height * 0.5f);

	//그리드 중앙색 (DX 형식 0xAARRGGBB)  
	//우리가 만든 DWORD 형변환 연산자는 DWORD(dx) --> B3YXCOLOR 임을 기억할 것.
	//COLOR col = COLOR(0, 0, 0, 1); 
	DWORD col = 0xff000000;				
										
 
	//x 축 라인 생성.
	int k=0;
	for(int i=0; i<xcnt; i++, k+=2)
	{ 
		float sx = -hx;
		float sz = hy -  i * pGrid->scale;		//위에서 아래로 내려옵니다.(+Z --> -Z)

		m_pGrids[k].vPos   = VECTOR3(sx, 0.0f, sz);
		m_pGrids[k+1].vPos = VECTOR3(sx + pGrid->width, 0.0f,  sz);

		m_pGrids[k].Color   = (xcnt/2 == i)?col:pGrid->color;
		m_pGrids[k+1].Color = (xcnt/2 == i)?col:pGrid->color;
	}
	 
	//z 축 라인 생성
	for(int j=0; j<ycnt; j++, k+=2)
	{ 
		float sx = -hx + j * pGrid->scale;		//왼쪽에서 오른쪽으로..(-X --> +X)
		float sz = hy;		

		m_pGrids[k].vPos   = VECTOR3(sx, 0.0f, sz);
		m_pGrids[k+1].vPos = VECTOR3(sx, 0.0f,  sz - pGrid->height);

		m_pGrids[k].Color   = (ycnt/2 == j)?col:pGrid->color;
		m_pGrids[k+1].Color = (ycnt/2 == j)?col:pGrid->color;
	}
 
	

	//정점 버퍼 구성.
 	int res = YN_OK;
	int vbsize = vtxcnt * sizeof(VTX_GRID);
	res = m_pDev->CreateVertexBuffer(vbsize, 0, FVF_GRID, B3YPOOL_SYSTEMMEM, &m_pVB, NULL );
	if (YN_FAILED(res))
	{
		//B3D_Error(NO_, "B3Grid::Init() 실패. VB 생성 실패.");
		return res;
	}

	VTX_GRID* pVB = NULL;
	res = m_pVB->Lock(0, vbsize, (void**)&pVB, 0);
	if(YN_FAILED(res))
	{ 	
		//.. 에러...
		return res;
	}
	::memcpy(pVB, m_pGrids, vbsize);
	m_pVB->Unlock();


	//전체 라인개수.
	m_uLineCnt = vtxcnt / 2; 


	//완료.
	//B3D_Error(NO_, "B3Grid::Init() 성공. 그리드 로딩 완료.");


	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
//
int ynGrid::Update( float dTime/*=0*/, GRIDINFO* pGrid /* = NULL */ )
{
	//if(pDev != NULL)	m_pDev = pDev;

	if(pGrid != NULL)
	{
		//...그리드 정보 리셋. 정점버퍼 재구성...
	}

	//..실시간 그리드 크기/개수 변경.
	//..지금은 작업없음.... 
	B3YXMatrixIdentity(&m_mTM); 

	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
//
int ynGrid::Render(float dTime/*=0*/, float sx /* = 0.0f */, float sy /* = 0.0f */) 
{
 	m_pDev->SetTransform(B3YTS_WORLD, &m_mTM);
	m_pDev->SetStreamSource(0, m_pVB, 0, sizeof(VTX_GRID)); 
	m_pDev->SetFVF(FVF_GRID);					 

	m_pDev->DrawPrimitive(B3YPT_LINELIST, 0, m_uLineCnt);		//"Line-List" 주의.
 
	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
//
int ynGridCreate(ynGrid** ppGrid, LPDEVICE pDev, int width /* = 100 */, int height /* = 100 */, 
				  float scale /* = 10.0f */, COLOR col /* = COLOR */)

{
	if(YN_VALIED(*ppGrid)) return YN_FAIL;
	if(YN_INVALIED(pDev))	return YN_FAIL;

	ynGrid::GRIDINFO gi;
	gi.width  = width;
	gi.height = height;
	gi.scale  = scale; 
	gi.color  = col;

	ynGrid* pGrid = new ynGrid;
	assert(pGrid);
	pGrid->Init(pDev, &gi);
	
	*ppGrid = pGrid;		//외부로 복사..
 
	return YN_OK;
}




/////////////////////////////////////////////////////////////////////////////
//
int ynGridCreate(ynGrid** ppGrid, LPDEVICE pDev, ynGrid::GRIDINFO* gi)
{
	if(YN_VALIED(*ppGrid))	return YN_FAIL;
	if(YN_INVALIED(pDev))	return YN_FAIL;
	if(YN_INVALIED(gi))		return YN_FAIL;

	ynGrid* pGrid = new ynGrid;
	assert(pGrid);
	pGrid->Init(pDev, gi);

	*ppGrid = pGrid;		//외부로 복사..

	return YN_OK;
}






} // end of namespace YENA

/***************** end of file "ynGrid.cpp" ********************************/













