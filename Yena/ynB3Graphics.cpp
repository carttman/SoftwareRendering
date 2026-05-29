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




////////////////////////////////////////////////////////////////////////////////
//
//! \brief	정점 버퍼 등록.
//! 
//! 정점 버퍼를 디바이스에 등록합니다. 
//! 
//! <DX> 복수의 정점 버퍼로 동시 '기하 스트리밍'을 지원합니다. (DX9, 최대 16)
//! 또는 지정 VB 내의 데이터중 부분 렌더링(OffsetBytes)도 가능합니다.  
//! \code
//!      GraphicsEngine.pVB[StreamNumber] = pVB; 
//! \endcode
//! <Yena> 1개의 정점 버퍼만 사용합니다.
//!
//! \param	pVB		렌더링 할 정점 버퍼 포인터.
//! \param	Stride	렌더링 할 버퍼의 1마디(정점 데이터) 크기
//! \return		성공시 OK, 실패시 FALSE
//! \see	B3YenaDevice9::SetStreamSource
//
int B3YenaGraphicsEngine9::_SetVertexBuffer( IYenaVertexBuffer9* pVB, UINT Stride )
{
	if(YN_INVALIED(pVB)) return YN_FAIL;


	// <DX> 복수의 정점 버퍼로 동시 '기하 스트리밍'을 지원합니다. (DX9, 최대 16)
	// 또는 지정 VB 내의 데이터중 부분 렌더링(OffsetBytes)도 가능합니다.
	// m_pVB[StreamNumber] = pVB;
	// 
	// <Yena> 1개의 정점 버퍼만 사용합니다.
	m_pVB[0] = pVB;
	m_Stride = Stride;

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! \brief	정점 규격 설정.
//! 
//! 정점 규격을 설정합니다. 
//! 렌더링시 엔진에 등록된 정점과 동일한 규격을 명시해야 합니다.
//! 그렇지 않다면 정상적인 렌더링 결과를 기대할 수 없습니다.
//! 
//! \param	fvf	정점규격 (FVF)
//! \return		성공시 OK, 실패시 FALSE
//! \see  B3YenaDevice9::SetFVF
//
int B3YenaGraphicsEngine9::_SetFVF(DWORD fvf)
{
	m_FVF = fvf;

	return YN_OK;
}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// Yena Graphics Engine : Pipeline API
//
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//! 정점 버퍼에서 데이터 읽기 "Vertex Streaming"
//! "Triangle-List" 렌더링 모드로 동작시  VB 에서 1개의 페이스(3개의 정점)씩 처리합니다. 
//!  - Triangle-List = 1 Face, 3 Vertices
//!  - Line-List = 1 Line, 2 Vertices
//! 
//! \param[out]	oPosition	(처리할) 정점 위치
//! \param[out]	oColor0		(처리할) 정점 색상 (Diffuse Color)
//! \return		성공시 OK, 실패시 FALSE
//
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
	//m_CurrVtxNum = -1;								//현재 처리중인 정점번호.디버깅용
	//m_CurrPrimNum = -1;
	
	
	//(현재 작업할) 기하도형 색인(번호) 계산
	m_CurrPrimIndex = m_CurrVtxIndex / m_TopologyVtxSize;	

	//정점 버퍼 범위 검사, 범위를 벗어나면 작업 종료..
	if (m_CurrVtxIndex + m_TopologyVtxSize > totVtxSize) return YN_FAIL;


	// 정점 버퍼에서 기하 도형별 정점 정보 읽기. "Vertex Streaming"
	// "Triangle-List" 렌더링 모드로 동작시  VB 에서 1개의 페이스(3개의 정점)씩 처리합니다. 
	//  - Triangle-List = 1 Face, 3 Vertices
	//  - Line-List = 1 Line, 2 Vertices
	for (UINT i = 0; i < m_TopologyVtxSize; i++)		//지정 정점부터, 읽기.
	{
		UINT v = m_CurrVtxIndex + i;					//읽어들일 정점 번호
		//m_dbg.CurrVtxIndex = v;						//읽어들일 정점 번호.디버깅용

		//정점 데이터 얻기.
		oPosition[i] = pVB->_GetPos3(v);				//정점 좌표 (3성분)
		oColor   [i] = pVB->_GetDiffuse(v);				//정점 색상 (4성분) 원본 정점버퍼에 DWORD 형으로 입력되더라도, XCOLOR 타입으로 처리합니다.(편의성 증대)
	}

	//3. 외부로 정점 리턴
	//...

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! \bstar 정점 파이프 라인 연산을 수행. 
//! 
//! 각 Vertex 별 연산 Operation - 정점별 변환, 조명, 안개 등의 연산을 처리합니다.  
//!   - 정점 변환 (World Transform)  
//!   - 정점 블랜딩 (Vertex Blending)  
//!   - 정점 변환 (View Transform)  
//!   - 정점 안개 (Vertex Fog)  
//!   - 정점 조명 (Vertex Lighting)  
//!   - 정점 변환 (Projection Transform)  
//! 출력결과는 기하 파이프라인 (Geometry Pipeline, GP) 으로 넘어감니다.  
//! 
//! <DX> 정점 셰이더(VS, Vertex Shader)를 통한 사용자 연산을 처리할 수 있습니다.  
//! <Yena> TnL 파이프라인을 우선 제작하고, 추후 셰이더 파이프라인을 구축합니다.  
//!        더하여  VS 및 HLSL 형식의 개발환경도 함께 제작합니다.
//! 
//! \param[int]		iPosition	(처리할) 현재 정점 좌표, 3성분, 배열
//! \param[out]		oPosition	(처리된) 현재 정점 좌표, 4성분, 배열
//! \param[in, out]	oColor		(처리할) 현재 정점 색상 (Diffuse Color), 배열
//! \return		성공시 OK, 실패시 FALSE
//! \see _GeometryPipeline, _PixelPipeline
//! \see _TnL_VP, _VS_Main
//
int B3YenaGraphicsEngine9::_VertexPipeline(
									_in_	B3YXVECTOR3 iPosition[3],
									_out_   B3YXVECTOR4 oPosition[3], 
									_inout_ B3YXCOLOR	oColor[3]
										  )
{

	//각 VP 처리 결과 저장 배열.
	std::vector<VS_OUTPUT> vsouts(m_TopologyVtxSize);


	//현재 렌더링 기하 형식 (Primitive Topology) 맞추어 정점별 연산을 수행...
	//...
	for(UINT i=0; i<m_TopologyVtxSize; i++)
	{ 
		B3YXVECTOR3 pos  = iPosition[i];
		B3YXCOLOR	col  = oColor  [i];

		
		//정점 셰이더 스테이지 (Vertex Shader Stage)
		//...
		//vsouts[i] = _VS_Main(pos, col);
		 
		
		//정점 고정함수 파이프라인 (Fixed Function Pipeline)
		vsouts[i] = _TnL_VP(pos, col);
	}


	//전체 VP 연산 결과 조합, 다음 파이프라인으로 출력...
	//...
	_PrimitiveAssembly(vsouts, oPosition, oColor);


	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! \bstar 기하 파이프라인 연산을 수행.
//! 
//! 각 Face 별 연산 Operation 을 처리합니다.  
//!   - 삼각형 컬링 (Face Culling)  
//!   - 사용자 평면 클립핑 (User Plane Clipping)   
//!   - 시야 공간 클립핑 (Viewing Frustum Clipping)   
//!   - 동차 나누기 (Homogeneous Divide)  
//!   - 뷰포팅 (View Port) (3D->2D)  
//! 
//! 출력결과는 픽셀 파이프라인 (Pixel Pipeline, PP) 으로 넘어감니다.  
//! 
//! <DX9> 고정함수 파이프라인만 지원. DX10 이상, 기하 셰이더 (Geometry Shader) 지원  
//! <Yena> 고정함수 파이프라인만 지원.  
//! 
//! \param[in]	iPosition	(입력) 정점 좌표, 4성분 
//! \param[out]	oPosition	(출력) 정점 좌표, 3성분 
//! \param[in, out]	oColor	(출력) 정점색 (Diffuse Color)
//! \return		성공시 OK, 실패시 FALSE
//! \see _VertexPipeline, _PixelPipeline
//! \see _TnL_GP, _GS_Main
//
int B3YenaGraphicsEngine9::_GeometryPipeline(
										_in_    B3YXVECTOR4 iPosition[3],
										_out_   B3YXVECTOR3 oPosition[3],
										_inout_ B3YXCOLOR	oColor   [3]
											)
{
	int res = YN_OK;

	//기하 셰이더 스테이지 (Geometry Shader Stage)
	//<DX10> 이상 지원, <Yena> 미지원
	//...
	//_GS_Main(oPosition, oColor, oNormal, oTexCoord);
	
	 
	//기하 고정함수 파이프라인 (Fixed Function Pipeline)
	res = _TnL_GP(iPosition, oPosition, oColor);

	
	return res;
}




////////////////////////////////////////////////////////////////////////////////
//
//! \bstar 픽셀 파이프 라인 연산을 수행.
//! 
//! 픽셀 Pixel 별 연산 Operation 을 처리하는 중요 스테이지(함수) 중 하나입니다.  
//! 다음의 연산이 순차적으로 처리됩니다.  
//! 
//!   * Pixel Pipeline #1
//!     - 텍스처 혼합 (Texture Stage)  
//!     - 조명 혼합 (Specular Light Blending)  
//!     - 픽셀 안개 혼합 (Pixel Fog Blending)  
//!   * Pixel Pipeline #2
//!     - 안개 혼합 (Fog Blending)  
//!		- 가위 테스트 (Scissor Test)   
//!		- 알파 테스트 (Alpha Test)  
//!		- 깊이/스텐실 테스트 (Depth/Stencil Test)  
//!		- 알파 블렌딩 (Alpha Blending)  
//!		- 디더링 (Dithering)  
//!		- 렌더타겟 쓰기 마스크 (Rendering Channel Mask)  
//!		- 렌더타겟에 색상 기록 (Present, Write to Render Target)  
//! 
//! 색상 출력은 필수이며 연산 결과는 렌더타겟(RenderTarget, RT) 에 기록됩니다.   
//! 
//! <DX> 셰이더는 레거시 고정 함수 파이프라인(Fixed Function Pipeline) 의 기능을 사용자의 
//! 선택에 따라 조정하거나 변경할 수 있습니다. 또한 픽셀 셰이더(Pixel Shader, PS) 를 통해 
//! 여러분만의 기능으로 독자적인 파이프라인 구축이 가능합니다.  
//! 픽셀 셰이더(Pixel Shader, PS) 는 GPU 프로그래밍 언어(HLSL) 로 개발되며 사용자 연산을 처리할 수 있습니다.
//! 
//! <DX9> 고정 함수 파이프라인 및 픽셀 셰이더 지원  
//! <Yena> TnL 파이프라인을 우선 제작하고, 추후 셰이더 파이프라인을 구축합니다.
//!        더하여  PS 및 HLSL 형식의 개발환경도 함께 제작합니다.  
//! 
//! \param	pos3	픽셀 좌표, 배열
//! \param	color	픽셀 색상, 배열
//! \return		성공시 OK, 실패시 FALSE
//! \see _VertexPipeline, _GeometryPipeline
//! \see _TnL_PP, _PS_Main
//
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


	//------------------------------------------
	// 1. 레스터라이저 : 2D 픽셀 정보 산출 (보간)
	//------------------------------------------
	// 각 Raster 함수 -> 보간 -> Pixel 산출, 출력 
	//_Rasterization(oPosition, oColor);		// --> 추후 개량...
	 

	//------------------------------------------
	// 2. 픽셀 파이프라인 (1) 
	//------------------------------------------
	// 픽셀 색상 및 텍스처 혼합
	_PixelPipeline1( _기능_생략_ );
  
	//------------------------------------------
	// 3. 픽셀 파이프라인 (2) 
	//------------------------------------------
	// 렌더타겟에 출력전, PS_Main 에서 계산된 색상으로
	// 여러 테스트를 수행하여  최종 출력  픽셀(색상)을 결정합니다.
	// 아래의 각 단계별 수행결과에 따라 , 테스트를 통과하지 못한 픽셀은
	// 전체 파이프라인에서 제외(버려짐) 됩니다.
	//------------------------------------------	 
	// 픽셀 출력 테스트 (AB, AT, DST) 및 최종색 출력(RT)
	_PixelPipeline2( _기능_생략_ );	


	// 이하, 파이프 라인 진행..(대부분 드라이버 상에서 처리)
	// ...


	//임시 그리기.. (여러분의 그리기 함수로 변경) 
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




////////////////////////////////////////////////////////////////////////////////
//
//! 기하 도형 재구성.  
//! 
//! 이전 스테이지에서 각 (변환된) 정점 데이터를 
//!	사용자 지정 기하 형식 (Primitive Topology) 맞추어  "삼각형" 을 재구성합니다.
//! 각 VP 및 VS 연산 결과를 조합하여 다음 파이프라인으로 출력합니다.
//! 
//! \param	vsouts			기하도형으로 재구성할 (변환된)정점 정보, 배열
//! \param	oPosition		기하도형으로 재구성된 (변환된)정점 위치, 4성분, 배열
//! \param	oColor			기하도형으로 재구성된 정점 색상 (Diffuse Color), 배열
//! \return	성공시 OK, 실패시 FALSE
//
int B3YenaGraphicsEngine9::_PrimitiveAssembly(
										_in_  VSOUTS&		vsouts,  
										_out_ B3YXVECTOR4	oPosition[3],
										_out_ B3YXCOLOR		oColor   [3]
											 )
{

	// 이전 스테이지에서 각 (변환된) 정점 데이터를 
	// 사용자 지정 기하 형식 (Primitive Topology) 맞추어  "삼각형" 을 재구성합니다.
	for (UINT i = 0; i < m_TopologyVtxSize; i++)
	{
		oPosition[i] = vsouts[i].pos;
		oColor   [i] = vsouts[i].diff;
	}

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! 픽셀 파이프라인 (#1) : 픽셀 색상 및 텍스처 혼합
//!
//! \see _PixelPipeline
//
int B3YenaGraphicsEngine9::_PixelPipeline1()
{
	
	// 픽셀 셰이더 스테이지 (Pixel Shader Stage)
	// _GS_Main(oPosition, oColor);
	//
	// 또는 ...
	// 
	// 픽셀 고정함수 파이프라인 (Fixed Function Pipeline)
	// _TnL_PP1(oPosition, oColor);
	

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! 픽셀 파이프라인 (#2) : 픽셀 출력 테스트 (AB, AT, DST) 및 최종색 출력(RT)
//! 
//! \see _PixelPipeline
// 
int B3YenaGraphicsEngine9::_PixelPipeline2()
{
	// 깊이 테스트 'Depth-Test' 등의 픽셀 '유효성(Test) 연산... 
	//...SetPixel 에서 처리함.


	// 2.안개 혼합 Fog Blending. 
	// ...

	// 3.가위 테스트 Scissor Test. 
	// ...

	// 4.알파 테스트 Alpha Test. 
	// ... 

	// 5.깊이/스텐실 테스트 Depth/Stencil Test.  
	// ...

	// 6.알파 블렌딩 Alpha Blending. 
	// ...

	// 7.디더링 Dithering. 
	// ...

	// 8.렌더타겟 쓰기 마스크 Rendering Channel Mask  
	// ..

	// 9.렌더타겟에  색상 기록 Write to Render Target.
	//
	//_SetPixel(x, y, oColor);


	// 10.화면 전송 Presentation 
	// ...

	return YN_OK;
}
 



///////////////////////////////////////////////////////////////////////////////
//
//! 삼각형 컬링, 2D (Screen) 좌표 기준.   
//! 
//! 2D Screen 좌표계 정점으로 컬링판정을 수행합니다.
//!	기본 컬링 모드는 반시계방향(CCW)으로 이를 만족하면 true 를 리턴합니다. 
//! 이 결과는 렌더링 파이프라인에서 해당 기하(삼각형)을 제외하는 조건으로 사용됩니다. 
//!				
//! \warning 2D(Screen) 좌표계는 +Y 축이 화면아랫 방향으로 3D 좌표계와 다름에 주의!
//! 
//! \param	v0, v1, v2	삼각형 구성 정점 좌표 (2D)
//! \return	컬링 조건 만족시 TRUE, 아니면 FALSE 리턴.
//! \see _FaceCulling3
// 
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




////////////////////////////////////////////////////////////////////////////////
//
//! 삼각형 컬링, 3D (Device / NDC Space) 좌표 기준.  
//! 
//! 3D 좌표계 정점으로 컬링판정을 수행합니다.
//!	기본 컬링 모드는 반시계방향(CCW)으로 이를 만족하면 true 를 리턴합니다. 
//! 이 결과는 렌더링 파이프라인에서 해당 기하(삼각형)을 제외하는 조건으로 사용됩니다. 
//! 
//! \warning DX 의 3D 좌표계는 (특별한 조건이 없다면) +Y 축이 화면 윗 방향입니다.
//!			 2D 화면 좌표계와 다름에 주의 하십시오!
//! 
//! \param	v0, v1, v2	삼각형 구성 정점 좌표 (3D)
//! \return	컬링 조건 만족시 TRUE, 아니면 FALSE 리턴.
//! \see _FaceCulling2
//
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






////////////////////////////////////////////////////////////////////////////////
//
//! \bstar 뷰포트 멥핑 (Viewport) 
//! 
//! 정규장치공간(NDC) 의 정점을 출력 화면 좌표(Screen) 로 변환합니다.  
//! 입력된 좌표는 해상도에 맞게 보정(Scale & Bias) 되어 다음 파이프라인으로 전달됩니다. 
//! 
//! \code
//! 화면 좌표로 변환 : 해상도 (800x600) 기준
//!  pos.x = -1 ~ 1 -->  0 ~ 799
//!	 pos.y = -1 ~ 1 -->  0 ~ 599
//!
//! \endcode
//! \param	iPos	좌표 입력. 3성분.
//! \param	oPos	좌표 출력. 2성분.
//! \return	성공시 OK, 실패시 FALSE
//
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




////////////////////////////////////////////////////////////////////////////////
//
//! 정점 처리 파이프라인 \b [고정 함수][TnL]  
//! 
//! 각 Vertex 별 연산 Operation - 정점별 변환, 조명, 안개 등의 연산을 처리합니다.  
//!   - 정점 변환 (World Transform)  
//!   - 정점 블랜딩 (Vertex Blending)  
//!   - 정점 변환 (View Transform)  
//!   - 정점 안개 (Vertex Fog)  
//!   - 정점 조명 (Vertex Lighting)  
//!   - 정점 변환 (Projection Transform)  
//!    
//! 출력결과는 기하 파이프라인 (Geometry Pipeline, GP) 으로 넘어감니다.  
//! 
//! \param	pos	(처리할) 현재 정점 좌표, 3성분 
//! \param	col	(처리할) 현재 정점 색상 (Diffuse Color)
//! \return	성공시 OK, 실패시 FALSE
//
VS_OUTPUT B3YenaGraphicsEngine9::_TnL_VP(
										B3YXVECTOR3 pos,
										B3YXCOLOR	col
										)
{
	VS_OUTPUT output;

	//
	// VS 내부 연산은 자체 수학라이브러리를 활용하겠습니다.
	//
	// Yena.Pos -> D3D.Pos 로 변환. 
	// D3DXVECTOR4 vPos( pos.x, pos.y, pos.z, 1);			//<DX>
	// B3Yena 벡터 변수 사용.
	B3YXVECTOR4 vPos(pos.x, pos.y, pos.z, 1);				//<Yena>


	//1. 정점, 월드 변환 (World Transform)
	//   vPos = vPos * m_mWorld;
	//D3DXVec4Transform(&vPos, &vPos, &m_mWorld);		//월드 변환 <DX>		
	B3YXVec4Transform(&vPos, &vPos, &m_mWorld);			//월드 변환 <Yena> ★		


	//* 정점 블랜딩 (Vertex Blending)
	//...

	//2. 정점, 뷰 변환 (View Transform)
	//   vPos = vPos * m_mView;
	//D3DXVec4Transform(&vPos, &vPos, &m_mView);		//단위 변환.<DX>			
	B3YXVec4Transform(&vPos, &vPos, &m_mView);			//단위 변환.<Yena> ★		


	//3. 정점 안개 (Vertex Fog)
	//...

	//4. 정점 조명 (Vertex Lighting)
	//...

	//5. 정점, 투영 변환 (Projection Transform)
	//   vPos = vPos * m_mProj;
	//D3DXVec4Transform(&vPos, &vPos, &m_mProj);		//단위 변환.<DX>				
	B3YXVec4Transform(&vPos, &vPos, &m_mProj);			//단위 변환.<Yena> ★


	//외부로 출력
	//...
	output.pos = vPos;			//4성분 출력, 주의.★	
	output.diff = col;

	return output;
}



////////////////////////////////////////////////////////////////////////////////
//
//! 기하 파이프라인 \b [고정 함수][TnL]  
//! 
//! 각 Face 별 연산 Operation 을 처리합니다.  
//!   - 삼각형 컬링 (Face Culling)  
//!   - 사용자 평면 클립핑 (User Plane Clipping)   
//!   - 시야 공간 클립핑 (Viewing Frustum Clipping)   
//!   - 동차 나누기 (Homogeneous Divide)  
//!   - 뷰포팅 (View Port) (3D->2D)   
//! 
//! 출력결과는 픽셀 파이프라인 (Pixel Pipeline, PP) 으로 넘어감니다.  
//! 
//! <DX9> 고정함수 파이프라인만 지원. DX10 이상, 기하 셰이더 (Geometry Shader) 지원  
//! <Yena> 고정함수 파이프라인만 지원.  
//! 
//! \param[in]	vPos4		(입력) 정점 좌표, 4성분 
//! \param[out]	oPos		(출력) 정점 좌표, 3성분 
//! \param[in, out]	oColor	(출력) 정점색 (Diffuse Color)
//! \return	성공시 OK, 실패시 FALSE
//
int B3YenaGraphicsEngine9::_TnL_GP(
									_in_	B3YXVECTOR4 iPos4 [3],
									_out_	B3YXVECTOR3 oPos  [3],
									_inout_ B3YXCOLOR	oColor[3]
								  )
{
	B3YXVECTOR2 vScreen[3];			//화면(공간)좌표(2D)
	//#define vPos vPos4
	//#define oPos oPos

	//1. 삼각형 컬링 (Face Culling)
	if (m_TopologyVtxSize >= 3)						//점/선은 뒷면컬링 없음.
	{
		if(_FaceCulling3(iPos4[0], iPos4[1], iPos4[2]))		//3D 버전.
		{
			//true 가 리턴되면, 컬링해야 하는 삼각형으로 판정, 
			//다음 삼각형으로 넘어간다.
			return YN_CULLED;						//조건 충족시, 그리지 않음.
		}
	}

	// 2.사용자 평면 클립핑 (User Plane Clipping)  
	//...생략...
	// return YN_CLIPPED;							//조건 충족시, 그리지 않음.


	// 3.시야 공간 클립핑 (Viewing Frustum Clipping) 
	//...생략...
	// return YN_CILPPED;							//조건 충족시, 그리지 않음.


	// 4. 동차 나누기 (Homogeneous Divide)
	//...

	//동차 변환 후 3D 가 되었다 가정...(벡터 형변환 연산자 오버로딩 필요) 
	B3YXVECTOR3 vPos3[3];
	vPos3[0] = iPos4[0];
	vPos3[1] = iPos4[1];
	vPos3[2] = iPos4[2];


	// 5. 뷰포팅 (View Port) (3D->2D) 
	//
	_Viewport(vPos3, vScreen);



	// 6. 최종 결과 출력 : PS 로 출력.
	//
	for (UINT i = 0; i < m_TopologyVtxSize; i++)	//기하도형별 정점개수만큼 처리.
	{ 												//해상도 800x600 기준.
		oPos[i].x = vScreen[i].x;					//화면좌표.. ( x: 0 ~ 799)
		oPos[i].y = vScreen[i].y;					//           ( y: 0 ~ 599)
		//oPos[i].z = vPos3[i].z;					//깊이값 Depth ( z : 0 ~ 1.0f) : ..생략...
		oColor[i] = oColor[i];						//색상은 변환없이 그대로..
	}


	#undef vPos
	#undef oPos
	return YN_OK;
}



////////////////////////////////////////////////////////////////////////////////
//
//! 픽셀 처리 파이프라인 \b [고정함수][TnL]  
//! 
//! 픽셀 Pixel 별 연산 Operation 을 처리하는 중요 스테이지(함수) 중 하나입니다.  
//!   - 텍스처 혼합 (Texture Stage)  
//!   - 조명 혼합 (Specular Light Blending)  
//!   - 픽셀 안개 혼합 (Pixel Fog Blending)  
//! 
//! 색상 출력은 필수이며 연산 결과는 렌더타겟(RenderTarget, RT) 에 기록됩니다.   
//! 
//! <DX> 셰이더는 레거시 고정 함수 파이프라인(Fixed Function Pipeline) 의 기능을 사용자의 
//! 선택에 따라 조정하거나 변경할 수 있습니다. 또한 픽셀 셰이더(Pixel Shader, PS) 를 통해 
//! 여러분만의 기능으로 독자적인 파이프라인 구축이 가능합니다. 
//! 픽셀 셰이더(Pixel Shader, PS) 는 GPU 프로그래밍 언어(HLSL) 로 개발되며 사용자 연산을 처리할 수 있습니다.  
//! 
//! <DX9> 고정 함수 파이프라인 및 픽셀 셰이더 지원  
//! <Yena> TnL 파이프라인을 우선 제작하고, 추후 셰이더 파이프라인을 구축합니다.
//!        더하여  PS 및 HLSL 형식의 개발환경도 함께 제작합니다.
//! 
//! \param	pos		픽셀 좌표, 배열
//! \param	color	픽셀 색상, 배열
//! \return	성공시 OK, 실패시 FALSE
//
int B3YenaGraphicsEngine9::_TnL_PP(
								_in_ B3YXVECTOR2 Pos  [3],
								_in_ B3YXCOLOR	 Color[3]
								  )
{
	//1. 텍스처 혼합 (Texture Stages)
	//...

	//2. 정반사광 혼합 (Add Specular)
	//...

	//3. 픽셀 안개 혼합 (Pixel Fog Blending)
	//...	 

	return YN_OK;
}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// Yena Graphics Engine : Rendering State API
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 상태 초기화. 
//
void B3YenaGraphicsEngine9::_InitRenderState()
{
	::ZeroMemory(m_RState, sizeof(DWORD) * B3YRS_MAX_);

	// 렌더링 장치, 상태 기본값 설정
	//
	m_RState[B3YRS_FILLMODE] = B3YFILL_SOLID;	//단일색 채우기 
	m_RState[B3YRS_CULLMODE] = B3YCULL_CCW;		//반시계 방향(CCW) 컬링 
}




///////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 상태 조절.
//
int B3YenaGraphicsEngine9::_SetRenderState(B3YRENDERSTATETYPE state, DWORD value)
{
	m_RState[state] = value;

	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 상태 얻기   
//
int B3YenaGraphicsEngine9::_GetRenderState(B3YRENDERSTATETYPE state, DWORD* value)
{
	*value = m_RState[state];

	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 변환 행렬 설정  
//
int B3YenaGraphicsEngine9::_SetTransform(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* m)
{
	m_mTFM[ts] = *m;

	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 변환 행렬 얻기 
//
int B3YenaGraphicsEngine9::_GetTransform(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* m)
{
	*m = m_mTFM[ts];

	return YN_OK;
}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// Yena Graphics Engine : Drawing API
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//! 라인 그리기. (ver.GDI)  
//! 직접 그리기.[5] (세로방향기준 기울기+오류수정+강제정수화)  
//!	직접 그리기.[2] 방식의 오류 수정 버전. 2015.11.13.   
//! 
//! \param	v0, v1	라인 구성 정점 좌표
//! \param	c0, c1	라인 구성 정점 색상 
//! \return	작업 완료시 YN_OK, 오류시 YN_FAIL 리턴.
//! 
//! \date	Updated. 25.0623. Pixel Buffer 에 기록, 이후 일괄 RT 에 출력. 
//! \remarks <주> 정점 버퍼 내의 색상은 여전히 DWORD 형임에 주의, 클래스 연산자 오버로딩으로 호환성 유지.
//! 
//
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
	//임시 그리기..(삭제후 과제 수행)★
	/*MoveToEx(m_hSurfaceRT, (int)v0.x, (int)v0.y, NULL);
	LineTo(m_hSurfaceRT, (int)v1.x, (int)v1.y);*/

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



/*
///////////////////////////////////////////////////////////////////////////////
//
//! 라인 그리기. (ver.GDI)
//
int B3YenaGraphicsEngine9::_Line(B3YVECTOR2& v0, B3YVECTOR2& v1)
{
	MoveToEx(m_om.hRT, (int)v0.x, (int)v0.y, NULL);
	LineTo(m_om.hRT, (int)v1.x, (int)v1.y);

	return YN_OK;
}
*/



///////////////////////////////////////////////////////////////////////////////
//
//! 삼각형 그리기 : 정점색 추가.(B3YXCOLOR 타입)(ver.GDI) 
//! 
//! \remarks <주> 정점 버퍼 내의 색상은 여전히 DWORD 형임에 주의, 클래스 연산자 오버로딩으로 호환성 유지.
//! 
//! \param	v0, v1, v2	삼각형 구성 정점 좌표
//! \param	c0, c1, c2	삼각형 구성 정점 색상
//! \return	작업 완료시 YN_OK, 오류시 YN_FAIL 리턴.
//! 
//
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

	// <GDI 버전> 임시 그리기.. (삭제후 과제 수행)★
	// ...
	#define v0 pos[0]
	#define v1 pos[1]
	#define v2 pos[2]
	#define c0 col[0]
	#define c1 col[1]
	#define c2 col[2]

	//POINT pt[3] = { v0, v1, v2 };	//연산자 오버로딩.
	//Polygon(m_hSurfaceRT, pt, 3);
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


///////////////////////////////////////////////////////////////////////////////
//
//! 수평선 그리기   
//! 지정 위치와 색상으로 수평선을 출력합니다  
//! (ex) v0(x1, y) --> v1(x2, y)
//! 
//! \remarks 픽셀 출력에 사용되는  SetPixel 로 인해 상당한 성능 저하가 발생됨.
//!			 다음 버전에서 개량 예정. 
//! 
//! \param	hdc	출력 화면 DC (렌더타겟)
//! \param	x1	수평선 시작 점, x 좌표
//! \param	x2	수평선 끝 점, x 좌표
//! \param	y	수평선, y 좌표
//! \param	c0	수평선 시작 점 색상
//! \param	c1	수평선 끝 점 색상
//! \return	작업 완료시 YN_OK, 오류시 YN_FAIL 리턴.
//
void B3YenaGraphicsEngine9::_HLine(HDC hdc, int x1, int x2, int y, B3YXCOLOR c0, B3YXCOLOR c1)
{
	//...
}

 

/*
//Test 2 (ver. GDI) 오우~~ 속도~~ ㅠ_ㅠ
void B3YenaDevice9::_HLine(HDC hdc, int x1, int x2, int y )
{
	for(int i=x1; i<=x2; i++)
	{
		_SetPixel(hdc, i, y, RGB(255, 255, 255));
	}
}


/*
//Test 1 (ver. GDI)
void B3YenaDevice9::_HLine( HDC hdc, int x1, int x2, int y )
{
	MoveToEx(hdc, x1, y, NULL);
	LineTo(hdc,x2, y);
}
*/




////////////////////////////////////////////////////////////////////////////////
//
//! 렌더 타겟에 픽셀 출력  
//! 
//! 우리는 직접 읽기/쓰기 가능한 메모리-비트멥(DIB)을 사용하고 있습니다.  
//! DIB 의 컬러비트가 일반적인 GDI 의 것과 반대인 것에 주의하십시오.  
//!  - DDI : color = 0x00 BB GG RR  
//!  - DIB : color = 0x00 RR GG BB   
//! 
//! 장치 독립 비트멥(Device-Independent Bitmap) 도움말은 아래의 문서를 참고하십시오.  
//! \see [참고1] https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd183562(v=vs.85).aspx  
//! \see [참고2] https://msdn.microsoft.com/ko-kr/library/ff566496(v=vs.85).aspx  
//! 
//! \param	x		출력 화면 좌표(x)
//! \param	y		출력 화면 좌표(y)
//! \param	color	출력 색상
//
void B3YenaGraphicsEngine9::_SetPixel(int x, int y, B3YXCOLOR color)
{
	::SetPixel(m_hRT, x, y, color);		//GDI 기록하기.
}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// Yena Graphics Engine : Drawing API (외부)
//
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//! 그래픽 엔진, 그리기 선행 준비.  
//! 
//! 렌더링 전 디바이스에 공급된 개별 데이터를 재구성합니다.	
//! 렌더링 각 단계별 반복적으로 계산되는 상수 데이터를 미리 계산하여 파이프라인의 연산 속도 개선이 목적.
//! 
//! \param	PrimitiveType	렌더링 기하 타입.
//! \param	StartVertex		렌더링할 정점 시작번호 (정점버퍼 안)
//! \param	PrimitiveCount	렌더링할 기하 개수.
//! \return	작업 완료시 YN_OK, 오류시 YN_FAIL 리턴.
//
int B3YenaGraphicsEngine9::PreDraw(
									B3YPRIMITIVETYPE PrimitiveType,
									UINT StartVertex, 
									UINT PrimitiveCount 
									//B3YDBGINFO* pDbgInfo
								  )
{


	//------------------------------------------------------------
	// 그래픽 엔진 리셋
	//------------------------------------------------------------
	//Reset();

	//------------------------------------------------------------
	// 장치 정보 연결 및 필요 정보 복사.
	//------------------------------------------------------------
	//m_pDev = pDev;
	m_input.PrimitiveType  = PrimitiveType;
	m_input.PrimitiveCount = PrimitiveCount;
	m_input.StartVertex	   = StartVertex;
	
	//m_pDbgInfo = pDbgInfo;



	//------------------------------------------------------------
	// 렌더링 전 디바이스에 공급된 개별 데이터를 재구성하여
	// 렌더링 각 단계별 반복적으로 계산되는 상수 데이터를 
	// 미리 계산합니다.  파이프라인의 연산 속도 개선이 목적.
	//------------------------------------------------------------
	//조명, 행렬, 기타 상수들 사전 계산. 
	//_PreCompute();		



	//------------------------------------------------------------
	// 한번에 처리할 정점개수 지정 
	// 기하도형의 형태 PrimitiveType 에 따라 다르게 처리.
	// Triangle-List = 3 개.
	// Line-List = 2개 
	//------------------------------------------------------------
	switch (m_input.PrimitiveType)
	{
	case B3YPT_LINELIST: m_input.TopologyVtxSize = 2;
		break;
	default:
	case B3YPT_TRIANGLELIST: m_input.TopologyVtxSize = 3;
		break;
	}
	 

	//------------------------------------------------------------
	// 디버깅 정보 초기화. 
	//------------------------------------------------------------
	///m_DbgInfo.Clear();

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! \bstar 그래픽 엔진, 렌더링 시작.
//! 
//! 렌더링 파이프라인의 각 단계별 연산(VP/GP/PP)이 처리되며, 최종 결과는 렌더 타겟에 출력됩니다.  
//!   - _VertexPipeline  
//!   - _GeometryPipeline  
//!   - _PixelPipeline  
//! 
//! 이 메소드는 그래픽 엔진의 핵심 기능을 수행합니다. 함수 호출 전에 정점 버퍼, FVF, 정점 크기(Stride) 같은
//! 렌더링에 필요한 모든 정보가 디바이스에 등록되어 있어야 합니다.
//!
//! \par 렌더링 장치와 자원
//!		일반적인 하드웨어 렌더링 장치(Device) 는 다양한 렌더링 객체와 자원을 사용합니다.  
//!   	- 기하 버퍼 : VBs, IB, UAVs,...  
//!   	- 변환 정보 : Matrices, CBs  
//!   	- 조명, 텍스처 : Lights, Materials, Textures, Samplers  
//!   	- 파이프라인 상태 객체 : PSOs   
//!		- 셰이더 : Shader
//! 	렌더링 장치(Device)는 그리기가 시작되면 "현재" 등록(설정)된 자원과 객체들로 연산을 수행합니다.  
//! 	따라서 필요 자원들은 렌더링 시작 전 장치에 등록되어야 합니다.  
//! 
//! \par DX9 렌더링 파이프라인
//!		DX9 은 두가지 렌더링 파이프라인을 제공합니다.
//!		- 고정 함수 파이프라인 (Fixed Function Pipeline 또는 TnL) 
//!		- 셰이더 파이프라인 (Shader 또는 Programmable Function Pipeline) 
//! 
//!		사용자는 렌더링 목적과 성능에 맞추어 이들을 조정하거나 변경할 수 있습니다. \n
//!		셰이더 파이프라인은 셰이더(Shader) 를 통해 여러분만의 기능으로 독자적인 파이프라인 구축도 가능합니다.  \n
//!		셰이더(Shader) 는 GPU 사용자 연산 프로그램이며 GPU 프로그래밍 언어(HLSL, GLSL, DXIL, SPIR-V) 로 개발됩니다.
//! 
//! \note <Yena> TnL 파이프라인을 우선 제작하고, 추후 셰이더 파이프라인을 구축합니다.  
//!				 더하여 VS, PS 및 HLSL 형식의 개발환경도 함께 제작합니다.
//! 
//! \return	성공시 OK, 실패시 FALSE
//! \see	_VertexPipeline, _GeometryPipeline, _PixelPipeline
//! \see	_TnL_VP, _TnL_GP, _TnL_PP
//! \see	_VS_Main, _GS_Main, _PS_Main
//! \see	B3YenaDevice9::DrawPrimitive
//! 
//! \todo [과제] 렌더링 파이프라인 기본 골격 구축하기. \emoji :star:
//! \todo [과제] 정점 파이프라인 : 월드 (회전) 변환하기. \emoji :star: 
//! \todo [연구] 정점 파이프라인 : 뷰-투영 단위변환 결과 연구. \emoji :star: 
//
int B3YenaGraphicsEngine9::Draw()
{
	if (YN_INVALIED(m_pVB[0])) return YN_FAIL;
	if (YN_INVALIED(m_Stride)) return YN_FAIL;


	//연산결과 입/출력용 변수들.
	B3YXVECTOR3 vPos[3];					//정점 좌표 (3성분)
	B3YXVECTOR3 oPos3[3];					//정점 좌표 (3성분)
	B3YXVECTOR4 oPos4[3];					//정점 좌표 (4성분)
	B3YXCOLOR	oColor[3];					//색상 출력.(Diffuse Color)
	

	//현재 처리중인 기하정보 (디버깅용)
	m_CurrVtxIndex = m_StartVertex;			//시작, 정점 번호 설정 (외부 지정)
	m_CurrPrimIndex = -1;					//기하 도형 번호 (Line, Face) (초기화)
	m_CurrPrimDrawCnt = 0;					//처리한 기하도형 개수(Line, Face) (초기화) (bug@fix.21.0822)★

	int res = YN_OK;

	//------------------------------------------------------------
	// 각 렌더링 파이프라인별로 적절한 연산을 수행한 후, 
	// 그 결과를 다음 파이프라인으로 공급합니다.
	// 
	// 각 변환에 필요한 데이터는 DrawPrimitive 함수가 호출되기 전에
	// Device 에 공급되어야 합니다. 
	//------------------------------------------------------------
	while (1)
	{
		//1. 정점 읽기 : "Vertex Streaming" 
		res = _VertexStreaming(vPos, oColor);
		if (YN_FAILED(res))
		{
			//작업 완료, 또는 에러..
			break;
		}

		//2. 정점 조립 : 다중 정점 버퍼 조합 운용 (생략) 
		//_VertexAssembly(_기능_생략_);		


		//3. 정점 파이프라인 처리 : 월드+뷰+투영 변환.★ 
		_VertexPipeline(vPos, oPos4, oColor);


		//4. 기하 파이프라인 처리...
		res = _GeometryPipeline(oPos4, oPos3, oColor);
		if(YN_FAILED(res)) 
		{
			//특정 조건 부합시, 그리지 않음. 
			//... 컬링, 클립핑 등등...
		}
		else
		{
			//5. 픽셀 파이프라인 처리...
			_PixelPipeline(oPos3, oColor);

			//기하도형(Line, Face) 1개 출력 완료.
			//다음 기하 준비...
		
			//디버깅 정보 갱신 : 그리기 성공한 Primtivie 정보 수집...
			//m_DbgInfo.Update(m_input.PrimTypeVtxSize, 1);
		}


		//기하도형(Line, Face) 1개 출력 완료.
		//다음 기하도형 처리..
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

	 
	//디버깅 정보 갱신 : 전체장면(Scene) 정보들. 
	//m_DbgInfo.Update();


	//임시버퍼 제거.
	//_TempBufferRelease();

	return YN_OK;
}



//////////////////////////////////////////////////////////////////////////////
//
//! 그래픽 엔진, 그리기 후위 작업, 임시 버퍼 제거 등.
//
int B3YenaGraphicsEngine9::PostDraw()
{
	//...

	return YN_OK;
}



/*
//////////////////////////////////////////////////////////////////////////////
//
//! 디버깅 정보 획득
//
B3YDBGINFO* B3YenaGraphicsEngine9::_GetDbgInfo()
{
	return &m_DbgInfo;
}
*/







///////////////////////////////////////////////////////////////////////////////
//
// B3YenaGraphicsEngine9 의 정의와 구별용 정의 해제.
// B3YenaGraphicsEngine9 전용
//
///////////////////////////////////////////////////////////////////////////////
#undef m_pGraphics

#undef m_pVB 
#undef m_FVF	
#undef m_Stride 
#undef m_StartVertex
#undef m_TopologyVtxSize
#undef m_PrimitiveType
#undef m_PrimitiveCount
//#undef m_PrimVtxCnt

//구형 호환 (v1.2 이전)
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




/**************** end of "ynB3Graphics.cpp" ***********************************/