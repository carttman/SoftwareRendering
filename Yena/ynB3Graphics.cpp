//! 
//! \file	ynB3Graphics.cpp
//! \brief	Yena 그래픽스 엔진 소스 파일 
//!	\version Yena SW Renderer v1.5.x  
//! 
//! \author	김기홍 / Kihong Kim / onlysonim@gmail.com
//! \date	2003.11.4. Kihong Kim / mad_dog@hanmail.net
//! \date	2004.05.07. Updated.
//! \date	2005.09.23. Update (.Net) 
//! \date	2010.07.20. Update
//! \date	2015.11.20. Update
//! \date	2020.08.22. Updated. (DX12.x Windows 10 SDK 10.0.x)
//! \date	2024.04.28. Updated. (VS22)
//! \date	2026.05.20. Updated. (VS22)(v1.5.x)
// 

///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 인터페이스 헤더 선언 : ynIUnknown / ynInterface 전용 
//
#include "Yena.h"



///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 구현 클래스 선언 : B3Class / B3xxx 클래스 전용, 외부 노출 방지용.
//
#include "ynB3Class.h"
#include "ynB3VertexBuffer.h"
#include "ynB3Graphics.h"

#define CHECKSTATE( state, val ) (m_RState[(state)] == (val))	



///////////////////////////////////////////////////////////////////////////////
//
// 외부 라이브러리 포함
//
//#include "D3DX9.h"						//D3DX 라이브러리 를 사용합니다. (수학함수 등)--> DX 를 사용하지 않고 직접 구현합니다.
//#pragma comment(lib, _T("d3dx9.lib")		//임시(비교용). 필요한 대부분의 것을 직접 만들어 사용합니다.




///////////////////////////////////////////////////////////////////////////////
//
// B3YenaGraphicsEngine9 데이터 접근 재정의 
// - 구형 호환성 유지 (v1.2 이전)
// - 가독성 향상.
//
////////////////////////////////////////////////////////////////////////////////
// B3YenaGraphics 전용
//#define m_pGraphics		this

#define m_pVB				m_input.pVB
#define m_FVF				m_input.FVF
#define m_Stride			m_input.Stride 
#define m_StartVertex		m_input.StartVertex
#define m_TopologyVtxSize	m_input.TopologyVtxSize
#define m_PrimitiveType		m_input.PrimitiveType
#define m_PrimitiveCount	m_input.PrimitiveCount
//#define m_PrimVtxCnt	m_input.PrimTypeVtxSize

//구형 호환 (v1.2 이전)
#define m_PrimCnt			m_stm.CurrPrimDrawCnt
#define m_VtxNum			m_stm.CurrVtxIndex
#define m_FaceNum			m_stm.CurrPrimIndex

#define m_CurrPrimDrawCnt	m_stm.CurrPrimDrawCnt
#define m_CurrVtxIndex		m_stm.CurrVtxIndex
#define m_CurrPrimIndex		m_stm.CurrPrimIndex

#define m_mWorld			m_tms.mTFM[B3YTS_WORLD]
#define m_mView				m_tms.mTFM[B3YTS_VIEW] 
#define m_mProj				m_tms.mTFM[B3YTS_PROJECTION]
#define m_mTFM				m_tms.mTFM

#define m_RState			m_pso.RState

#define m_hWnd				m_om.hWnd
#define m_PresentParam		m_om.PresentParam
#define m_hBmpRT			m_om.hBmpRT
#define m_hRT				m_om.hRT
#define m_hSurfaceRT		m_om.hRT


  

/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/*
/////////////////////////////////////////////////////////////////////////////// 
//
// B3GraphicsEngine 개체 생성 최상위 호출 함수
//
/////////////////////////////////////////////////////////////////////////////// 
//
B3GraphicsEngine* B3YenaCreateEngine(DWORD ver)
{
	B3GraphicsEngine* pYena = new B3GraphicsEngine;

	//실패시 예외처리 필요...
	//ASSERT(pYena);

	return pYena;
}

*/




/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
// Yena SWR Engine Core  :  SW 렌더링 엔진 기반 클래스
//
/////////////////////////////////////////////////////////////////////////////// 

B3YenaCore::B3YenaCore(void)
{
	//... 
}

B3YenaCore::~B3YenaCore(void)
{
	//... 
}









/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
// B3GraphcisEngine9 : DX9 대응 렌더링 / 그래픽스 엔진 클래스, 
//
/////////////////////////////////////////////////////////////////////////////// 


B3YenaGraphicsEngine9::B3YenaGraphicsEngine9(void)
{
	Reset();
}


B3YenaGraphicsEngine9::~B3YenaGraphicsEngine9(void)
{ 
	Release();
}




//////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 / 그래픽스 엔진 리셋.
//
int B3YenaGraphicsEngine9::Reset()
{
	m_pDev = NULL;
 
	ZeroMemory(&m_input, sizeof(INPUT_STREAM));
	ZeroMemory(&m_stm,	sizeof(STREAM_DBG));
	ZeroMemory(&m_tms,  sizeof(TRANSFORM));			
	ZeroMemory(&m_pso,	sizeof(PIPELINE_STATE));
	ZeroMemory(&m_om,	sizeof(OUTPUT_MERGE));

	//디버깅 정보 초기화.
	//m_DbgInfo.ClearAll();


	//변환 행렬 클리어.
	for (int i = 0; i < B3YTS_MAX_; i++)
	{
		//D3DXMatrixIdentity(&m_mTFM[i]); 	//ver.DX (임시/비교용)
		B3YXMatrixIdentity(&m_mTFM[i]); 	//ver.YN 
	}


	//렌더상태 초기화.
	_InitRenderState();


	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! 그래픽스 엔진 해제, 내부 객체 제거
//
int B3YenaGraphicsEngine9::Release(void)
{
	//Back-Buffer : Render Target 제거. 
	_RenderTargetRelease();


	//정점 버퍼 자체를 제거하는 것은 사용자가 책임을 집니다.
	//...SafeRelease(m_input.pVB);	

	//각 렌더링 단계별 임시버퍼 제거.
	_TempBufferRelease();

	return YN_OK;
}





///////////////////////////////////////////////////////////////////////////////
//
//! 각 렌더링 파이프라인에서 생성된 단계별 임시버퍼 제거. 
//
void B3YenaGraphicsEngine9::_TempBufferRelease(void)
{
	//... 
}




///////////////////////////////////////////////////////////////////////////////
//
//! 백버퍼용 렌더타겟을 생성.
//
int B3YenaGraphicsEngine9::_RenderTargetCreate()
{
	HDC hdc = GetDC(m_hWnd);

	m_hSurfaceRT = CreateCompatibleDC(hdc);
	m_hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, m_PresentParam.Width, m_PresentParam.Height);
	SelectObject(m_hSurfaceRT, m_hBmpRT);
	
	ReleaseDC(m_hWnd, hdc);

	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
// 
//! 렌더타겟 제거.
//
void B3YenaGraphicsEngine9::_RenderTargetRelease()
{
	DeleteObject(m_hBmpRT);
	DeleteDC(m_hSurfaceRT);
}




////////////////////////////////////////////////////////////////////////////////
//
//! 렌더타겟 (백버퍼) 의 DC 핸들 획득
//! 
//! \return		성공시 DC 핸들 리턴.
//
HDC B3YenaGraphicsEngine9::_GetDCRT()
{
	return m_hSurfaceRT; 
}



///////////////////////////////////////////////////////////////////////////////
//
//! \brief	장면 그리기 시작
//! 
//! : 렌더링에 필요한 (디바이스의) 선위 작업을 수행.
//! \see [IDirect3DDevice9::BeginScene](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3ddevice9-beginscene)
//
int B3YenaGraphicsEngine9::_BeginScene()
{
	//폰트 배경색 기본값 
	//SetBkColor(m_hSurfaceRT, m_BkColor);
	SetBkMode(m_hSurfaceRT, TRANSPARENT);

	//기본 펜 색상 .
	HPEN  hPen = (HPEN)GetStockObject(WHITE_PEN);
	SelectObject(m_hSurfaceRT, hPen);

	return YN_OK;
}



///////////////////////////////////////////////////////////////////////////////
//
//! \brief	장면 그리기 종료
//! 
//! : 렌더링 종료에 필요한 (디바이스의) 후위 작업을 수행.
//! \see [IDirect3DDevice9::EndScene](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3ddevice9-endscene)
//
int B3YenaGraphicsEngine9::_EndScene()
{
	//...

	return YN_OK;
}



/*
////////////////////////////////////////////////////////////////////////////////
//
//! 렌더타겟 클리어.
//! 
//! \param	col		RT 을 지울 지정색.
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaGraphicsEngine9::_Clear(B3YXCOLOR col)
{
	HBRUSH hBrush = CreateSolidBrush(col);
	RECT rc = { 0, 0, (LONG)m_PresentParam.Width,  (LONG)m_PresentParam.Height };
	FillRect(m_hSurfaceRT, &rc, hBrush);
	DeleteObject(hBrush);

	return YN_OK;
}*/


////////////////////////////////////////////////////////////////////////////////
//
//! \brief	렌더타겟 클리어.
//! 
//! 렌더링 파이프라인에 등록(Binding)된 이미지 버퍼(서피스, Surface)를 지웁니다.  
//! 하나 이상의 렌더타겟과 깊이버퍼, 스텐실버퍼를 지울 수 있습니다.
//! 
//! \param	col		RT 을 지울 지정색.
//! \return		성공시 OK, 실패시 FALSE
//! \see [IDirect3DDevice9::Clear](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3ddevice9-clear)
//
int B3YenaGraphicsEngine9::_Clear(COLORREF col)
{
	HBRUSH hBrush = CreateSolidBrush(col);
	RECT rc = { 0, 0, (LONG)m_PresentParam.Width,  (LONG)m_PresentParam.Height };
	FillRect(m_hSurfaceRT, &rc, hBrush);
	DeleteObject(hBrush);

	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! \brief	장면 출력
//! 
//! RT 의 내용(렌더링된 장면)을 Front Buffer 에 출력합니다. "Flipping", "Swapping"
//!	
//! \param   없음
//! \return  오류시 YN_OK 이외의 값.
//! \see [IDirect3DDevice9::Present](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3ddevice9-present)
// 
int B3YenaGraphicsEngine9::_Present()
{
	HDC hdc = GetDC(m_hWnd);
	BitBlt(hdc, 0, 0, m_PresentParam.Width, m_PresentParam.Height, m_hRT, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);

	return YN_OK;
}
int B3YenaGraphicsEngine9::_SetVertexBuffer( IYenaVertexBuffer9* pVB, UINT Stride )
{
	if(YN_INVALIED(pVB)) return YN_FAIL;


	m_pVB[0] = pVB;
	m_Stride = Stride;

	return YN_OK;
}





int B3YenaGraphicsEngine9::_SetFVF(DWORD fvf)
{
	m_FVF = fvf;

	return YN_OK;
}









int B3YenaGraphicsEngine9::_VertexStreaming(
										_out_ B3YXVECTOR3 oPosition[3], 
										_out_ B3YXCOLOR   oColor   [3]
											)
{

	if (YN_INVALIED(m_pVB[0])) return YN_FAIL;
	if (YN_INVALIED(m_Stride)) return YN_FAIL;

	// 정점 버퍼 운용 객체 접근 : VB 0번 고정 
	B3YenaVertexBuffer9* pVB = ynGetClassObject<B3YenaVertexBuffer9>(m_pVB[0]);
	if (YN_INVALIED(pVB))
	{
		//에러 처리...
		return YN_FAIL;
	}


	// 현재 디바이스에 등록된 VB 의 기본정보를 계산. 
	//
	UINT totVtxSize = pVB->_GetVtxCnt();				//(전체) 정점 개수. 
	
	
	
	m_CurrPrimIndex = m_CurrVtxIndex / m_TopologyVtxSize;	

	if (m_CurrVtxIndex + m_TopologyVtxSize > totVtxSize) return YN_FAIL;


	
	for (UINT i = 0; i < m_TopologyVtxSize; i++)		//지정 정점부터, 읽기.
	{
		UINT v = m_CurrVtxIndex + i;					//읽어들일 정점 번호

		//정점 데이터 얻기.
		oPosition[i] = pVB->_GetPos3(v);				//정점 좌표 (3성분)
		oColor   [i] = pVB->_GetDiffuse(v);				
	}


	return YN_OK;
}


int B3YenaGraphicsEngine9::_VertexPipeline(
									_in_	B3YXVECTOR3 iPosition[3],
									_out_   B3YXVECTOR4 oPosition[3], 
									_inout_ B3YXCOLOR	oColor[3]
										  )
{

	std::vector<VS_OUTPUT> vsouts(m_TopologyVtxSize);


	
	for(UINT i=0; i<m_TopologyVtxSize; i++)
	{ 
		B3YXVECTOR3 pos  = iPosition[i];
		B3YXCOLOR	col  = oColor  [i];

		
		
		vsouts[i] = _TnL_VP(pos, col);
	}


	
	_PrimitiveAssembly(vsouts, oPosition, oColor);


	return YN_OK;
}





int B3YenaGraphicsEngine9::_GeometryPipeline(
										_in_    B3YXVECTOR4 iPosition[3],
										_out_   B3YXVECTOR3 oPosition[3],
										_inout_ B3YXCOLOR	oColor   [3]
											)
{
	int res = YN_OK;

	res = _TnL_GP(iPosition, oPosition, oColor);

	
	return res;
}





int B3YenaGraphicsEngine9::_PixelPipeline(
									_in_ B3YXVECTOR3 pos3 [3],
									_in_ B3YXCOLOR	 color[3]
										 )
{

	//이전 스테이지에서 2D 변환된 정점이 공급됩니다. 
	B3YXVECTOR2 pos[3];
	pos[0] = pos3[0];
	pos[1] = pos3[1];
	pos[2] = pos3[2];


	
	_PixelPipeline1( _기능_생략_ );
  
	
	_PixelPipeline2( _기능_생략_ );	


	
	{
		#define v	pos
		#define c	color

		if (m_RState[B3YRS_FILLMODE] == B3YFILL_WIREFRAME)
		{
			//와이어프레임  그리기. 
			_Line(v[0], v[1], c[0], c[1]);
			_Line(v[0], v[2], c[0], c[2]);
			_Line(v[1], v[2], c[1], c[2]);
		}
		else
		{
			//면 채음 그리기 
			_Face(v, c);
			//_Face(v[0], v[1], v[2], c[0], c[1], c[2]);
		}

		#undef v
		#undef c
	}

	return YN_OK;
}





int B3YenaGraphicsEngine9::_PrimitiveAssembly(
										_in_  VSOUTS&		vsouts,  
										_out_ B3YXVECTOR4	oPosition[3],
										_out_ B3YXCOLOR		oColor   [3]
											 )
{
	for (UINT i = 0; i < m_TopologyVtxSize; i++)
	{
		oPosition[i] = vsouts[i].pos;
		oColor   [i] = vsouts[i].diff;
	}

	return YN_OK;
}





int B3YenaGraphicsEngine9::_PixelPipeline1()
{
	
	

	return YN_OK;
}





int B3YenaGraphicsEngine9::_PixelPipeline2()
{
	return YN_OK;
}
 




bool B3YenaGraphicsEngine9::_FaceCulling2(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2)
{
	bool bCull = false;

	switch (m_RState[B3YRS_CULLMODE])
	{
	case B3YCULL_NONE:				//컬링 없음. 무조건 그리기.
		bCull = false;
		break;

	case B3YCULL_CW:				//정점 순서가 시계 방향(CW)이면, 컬링. 그리지 않음.
	{
		B3YXVECTOR2 v01 = v1 - v0;	//방향 계산 (연산자 오버로딩 추가)
		B3YXVECTOR2 v02 = v2 - v0;

		float z = B3YXVec2CCW(&v01, &v02);

		if (z > 0)					//외적의 결과가 양수면, 'CW' 				
			bCull = true;			//곧, 컬링되어야 함.	  
	}
	break;

	case B3YCULL_CCW:				//정점 순서가 반시계 방향(CCW)이면, 컬링. 그리지 않음.
	{
		B3YXVECTOR2 v01 = v1 - v0;
		B3YXVECTOR2 v02 = v2 - v0;

		float z = B3YXVec2CCW(&v01, &v02);

		if (z < 0)					//외적의 결과가 음수면, 'CCW' 
			bCull = true;			//곧, 컬링되어야 함.	
	}
	break;
	}


	return bCull;
}



bool B3YenaGraphicsEngine9::_FaceCulling3(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2)
{
	bool bCull = false;

	switch (m_RState[B3YRS_CULLMODE])
	{
	case B3YCULL_NONE:				//컬링 없음. 무조건 그리기.
		bCull = false;
		break;

	case B3YCULL_CW:				//정점 순서가 시계 방향(CW)이면, 컬링. 그리지 않음.
	{
		B3YXVECTOR2 v01 = v1 - v0;	//방향 계산 (연산자 오버로딩 추가)
		B3YXVECTOR2 v02 = v2 - v0;

		float z = B3YXVec2CCW(&v01, &v02);

		if (z < 0)					//외적의 결과가 음수면, 'CW' 				
			bCull = true;			//곧, 컬링되어야 함.	  
	}
	break;

	case B3YCULL_CCW:				//정점 순서가 반시계 방향(CCW)이면, 컬링. 그리지 않음.
	{
		B3YXVECTOR2 v01 = v1 - v0;
		B3YXVECTOR2 v02 = v2 - v0;

		float z = B3YXVec2CCW(&v01, &v02);

		if (z > 0)					//외적의 결과가 양수면, 'CCW' 
			bCull = true;			//곧, 컬링되어야 함.	
	}
	break;
	}


	return bCull;
}







int B3YenaGraphicsEngine9::_Viewport(
									B3YXVECTOR3 iPos[3], 
									B3YXVECTOR2 oPos[3] 
									)		
{  
	#define vPos  		iPos[i]						//가독성 향상을 위한 메크로 선언.
	#define vScreen 	oPos[i]					

	float w = (float)m_PresentParam.Width;
	float h = (float)m_PresentParam.Height;

	for (UINT i = 0; i < m_TopologyVtxSize; i++)	//기하도형별 정점수 만큼 처리.
	{
		vScreen.x =  vPos.x * (w / 2) + (w / 2);
		vScreen.y = -vPos.y * (h / 2) + (h / 2);
		//vScreen.z =  vPos.z;   <-- 일단 생략..
	}

	#undef vPos
	#undef vScreen

 	return YN_OK;
}




VS_OUTPUT B3YenaGraphicsEngine9::_TnL_VP(
										B3YXVECTOR3 pos,
										B3YXCOLOR	col
										)
{
	VS_OUTPUT output;

	
	B3YXVECTOR4 vPos(pos.x, pos.y, pos.z, 1);				//<Yena>


	B3YXVec4Transform(&vPos, &vPos, &m_mWorld);			//월드 변환 <Yena> ★		

	B3YXVec4Transform(&vPos, &vPos, &m_mView);			//단위 변환.<Yena> ★		


	B3YXVec4Transform(&vPos, &vPos, &m_mProj);			//단위 변환.<Yena> ★


	
	output.pos = vPos;			//4성분 출력, 주의.★	
	output.diff = col;

	return output;
}




int B3YenaGraphicsEngine9::_TnL_GP(
									_in_	B3YXVECTOR4 iPos4 [3],
									_out_	B3YXVECTOR3 oPos  [3],
									_inout_ B3YXCOLOR	oColor[3]
								  )
{
	B3YXVECTOR2 vScreen[3];			//화면(공간)좌표(2D)
	
	
	if (m_TopologyVtxSize >= 3)						//점/선은 뒷면컬링 없음.
	{
		if(_FaceCulling3(iPos4[0], iPos4[1], iPos4[2]))		//3D 버전.
		{
			//true 가 리턴되면, 컬링해야 하는 삼각형으로 판정, 
			//다음 삼각형으로 넘어간다.
			return YN_CULLED;						//조건 충족시, 그리지 않음.
		}
	}

	
	B3YXVECTOR3 vPos3[3];
	vPos3[0] = iPos4[0];
	vPos3[1] = iPos4[1];
	vPos3[2] = iPos4[2];


	
	_Viewport(vPos3, vScreen);



	
	for (UINT i = 0; i < m_TopologyVtxSize; i++)
	{ 											
		oPos[i].x = vScreen[i].x;				
		oPos[i].y = vScreen[i].y;				
		//oPos[i].z = vPos3[i].z;				
		oColor[i] = oColor[i];					
	}


	#undef vPos
	#undef oPos
	return YN_OK;
}



int B3YenaGraphicsEngine9::_TnL_PP(
								_in_ B3YXVECTOR2 Pos  [3],
								_in_ B3YXCOLOR	 Color[3]
								  )
{
	

	return YN_OK;
}









void B3YenaGraphicsEngine9::_InitRenderState()
{
	::ZeroMemory(m_RState, sizeof(DWORD) * B3YRS_MAX_);

	
	m_RState[B3YRS_FILLMODE] = B3YFILL_SOLID;	//단일색 채우기 
	m_RState[B3YRS_CULLMODE] = B3YCULL_CCW;		//반시계 방향(CCW) 컬링 
}





int B3YenaGraphicsEngine9::_SetRenderState(B3YRENDERSTATETYPE state, DWORD value)
{
	m_RState[state] = value;

	return YN_OK;
}




int B3YenaGraphicsEngine9::_GetRenderState(B3YRENDERSTATETYPE state, DWORD* value)
{
	*value = m_RState[state];

	return YN_OK;
}


int B3YenaGraphicsEngine9::_SetTransform(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* m)
{
	m_mTFM[ts] = *m;

	return YN_OK;
}




int B3YenaGraphicsEngine9::_GetTransform(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* m)
{
	*m = m_mTFM[ts];

	return YN_OK;
}









int B3YenaGraphicsEngine9::_Line(
								B3YXVECTOR2 v0, B3YXVECTOR2 v1, 
								B3YXCOLOR   c0, B3YXCOLOR   c1
								)
{
	//! \todo2 <과제> 라인 그리기 ★
	//!	정점 사이의 간격만큼 각 정점색상을 보간하여 라인색을 출력합니다. 
	// ... 
	//
	//! 함수 시그니쳐(Signature : 인자/리턴값/이름) 의 변경없이,
	//! 함수의 Body 를 완성 하십시오.★
	// ...
	const int dx = v0.x - v1.x;
	const int dy = v0.y - v1.y;

	const float length = sqrt(dx * dx + dy * dy);

	const float unitX = dx / length; // 단위벡터
	const float unitY = dy / length;

	float curX = v1.x;
	float curY = v1.y;

	for (int i = 0; i <= (int)length; i++) // 거리만큼 단위벡터를 더해가면서 점을 찍는다
	{
		const float t = i / length;

		B3YXCOLOR color;
		color.r = c0.r + (c1.r - c0.r) * t;
		color.g = c0.g + (c1.g - c0.g) * t;
		color.b = c0.b + (c1.b - c0.b) * t;
		color.a = c0.a + (c1.a - c0.a) * t;

		SetPixel(m_hSurfaceRT, (int)(curX), (int)(curY + 0.5f), color);
		curX += unitX;
		curY += unitY;
	}
	
	return YN_OK;
}


 

///////////////////////////////////////////////////////////////////////////////
//
//! 라인 그리기. (ver.GDI) 
//
int B3YenaGraphicsEngine9::_Line(B3YXVECTOR2& v0, B3YXVECTOR2& v1, B3YXCOLOR& col)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, col);
	HPEN hOldPen = (HPEN)SelectObject(m_om.hRT, hPen);
	MoveToEx(m_om.hRT, (int)v0.x, (int)v0.y, NULL);
	LineTo(m_om.hRT, (int)v1.x, (int)v1.y);
	SelectObject(m_om.hRT, hOldPen);
	DeleteObject(hPen);
	return YN_OK;
}


int B3YenaGraphicsEngine9::_Face(
								B3YXVECTOR2 pos[3],
								B3YXCOLOR   col[3]
								)
{
	//! \todo2 <과제> 삼각형 그리기 ★
	//! 정점 3개와 색상 3개를 이용해 삼각형에 색상을 채웁니다.★
	// ...
	//
	// 함수 시그니쳐(Signature : 인자/리턴값/이름) 의 변경없이,
	// 함수의 Body 를 완성 하십시요.★
	// ...

	
	#define v0 pos[0]
	#define v1 pos[1]
	#define v2 pos[2]
	#define c0 col[0]
	#define c1 col[1]
	#define c2 col[2]

	// Bounding box
	int minX = int(v0.x < v1.x ? v0.x : v1.x);
	minX = int(minX < v2.x ? minX : v2.x);

	int maxX = int(v0.x > v1.x ? v0.x : v1.x);
	maxX = int(maxX > v2.x ? maxX : v2.x);

	int minY = int(v0.y < v1.y ? v0.y : v1.y);
	minY = int(minY < v2.y ? minY : v2.y);

	int maxY = int(v0.y > v1.y ? v0.y : v1.y);
	maxY = int(maxY > v2.y ? maxY : v2.y);

	for (int i = minY; i <= maxY; i++)
	{
		for (int j = minX; j <= maxX; j++)
		{
			B3YXVECTOR2 p = { (float)j, (float)i };

			// 세 직선에 대한 EdgeFunction
			const float area0 = _EdgeFunction(v1, v2, p);
			const float area1 = _EdgeFunction(v2, v0, p);
			const float area2 = _EdgeFunction(v0, v1, p);

			// 무게 중심 좌표
			const float center = area0 + area1 + area2;

			const float w0 = area0 / center;
			const float w1 = area1 / center;
			const float w2 = area2 / center;


			if (area0 >= 0 && area1 >= 0 && area2 >= 0 || area0 <= 0 && area1 <= 0 && area2 <= 0)
			{
				// 색 보간
				B3YXCOLOR color = c0 * w0 + c1 * w1 + c2 * w2;
				SetPixel(m_hSurfaceRT, j, i, color);
			}
		}
	}
	#undef v0
	#undef v1
	#undef v2
	#undef c0
	#undef c1
	#undef c2

	return YN_OK;
}

float B3YenaGraphicsEngine9::_EdgeFunction(const B3YXVECTOR2& v0, const B3YXVECTOR2& v1, const B3YXVECTOR2& p)
{
	const B3YXVECTOR2 a = { v1.x - v0.x, v1.y - v0.y };
	const B3YXVECTOR2 b = { p.x - v0.x, p.y - v0.y };

	return((a.x * b.y) - (a.y * b.x)) / 2;
}



void B3YenaGraphicsEngine9::_HLine(HDC hdc, int x1, int x2, int y, B3YXCOLOR c0, B3YXCOLOR c1)
{
	
}


void B3YenaGraphicsEngine9::_SetPixel(int x, int y, B3YXCOLOR color)
{
	::SetPixel(m_hRT, x, y, color);		//GDI 기록하기.
}



int B3YenaGraphicsEngine9::PreDraw(
									B3YPRIMITIVETYPE PrimitiveType,
									UINT StartVertex, 
									UINT PrimitiveCount 
									//B3YDBGINFO* pDbgInfo
								  )
{


	
	m_input.PrimitiveType  = PrimitiveType;
	m_input.PrimitiveCount = PrimitiveCount;
	m_input.StartVertex	   = StartVertex;
	
	
	switch (m_input.PrimitiveType)
	{
	case B3YPT_LINELIST: m_input.TopologyVtxSize = 2;
		break;
	default:
	case B3YPT_TRIANGLELIST: m_input.TopologyVtxSize = 3;
		break;
	}
	 

	
	return YN_OK;
}





int B3YenaGraphicsEngine9::Draw()
{
	if (YN_INVALIED(m_pVB[0])) return YN_FAIL;
	if (YN_INVALIED(m_Stride)) return YN_FAIL;


	B3YXVECTOR3 vPos[3];					
	B3YXVECTOR3 oPos3[3];					
	B3YXVECTOR4 oPos4[3];					
	B3YXCOLOR	oColor[3];					
	

	m_CurrVtxIndex = m_StartVertex;			
	m_CurrPrimIndex = -1;					
	m_CurrPrimDrawCnt = 0;					

	int res = YN_OK;

	while (1)
	{
		//1. 정점 읽기 : "Vertex Streaming" 
		res = _VertexStreaming(vPos, oColor);
		if (YN_FAILED(res))
		{
			//작업 완료, 또는 에러..
			break;
		}

		

		//3. 정점 파이프라인 처리 : 월드+뷰+투영 변환.★ 
		_VertexPipeline(vPos, oPos4, oColor);


		
		res = _GeometryPipeline(oPos4, oPos3, oColor);
		if(YN_FAILED(res)) 
		{
		}
		else
		{
			
			_PixelPipeline(oPos3, oColor);
		}


		{
			m_CurrVtxIndex += m_TopologyVtxSize;					//다음 정점 색인 계산.
			m_CurrPrimIndex = m_CurrVtxIndex / m_TopologyVtxSize;	//다음 기하 색인 계산. 

			//지정 개수 이상의 삼각형이 그려지면, 작업 종료... 			
			if (++m_CurrPrimDrawCnt >= m_PrimitiveCount)
				break;

			//임시 버퍼 비우기
			ZeroMemory(vPos, sizeof(B3YXVECTOR3) * 3);
			ZeroMemory(oPos3, sizeof(B3YXVECTOR3) * 3);
			ZeroMemory(oPos4, sizeof(B3YXVECTOR4) * 3);
			ZeroMemory(oColor, sizeof(B3YXCOLOR) * 3);
		}
	}

	return YN_OK;
}


int B3YenaGraphicsEngine9::PostDraw()
{
	//...

	return YN_OK;
}


#undef m_pGraphics

#undef m_pVB 
#undef m_FVF	
#undef m_Stride 
#undef m_StartVertex
#undef m_TopologyVtxSize
#undef m_PrimitiveType
#undef m_PrimitiveCount

#undef m_PrimCnt 
#undef m_VtxNum 
#undef m_FaceNum 

#undef m_CurrPrimDrawCnt 
#undef m_CurrVtxIndex 
#undef m_CurrPrimIndex 


#undef m_mWorld 
#undef m_mView 
#undef m_mProj 
#undef m_mTFM 

#undef m_RState

#undef m_hWnd
#undef m_PresentParam 
#undef m_hBmpRT
#undef m_hRT 
#undef m_hSurfaceRT 




