//! 
//! \file	ynB3Graphics.h
//! \bstar	Yena 그래픽스 엔진 헤더 파일 
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
//! \date	2025.05.20. Updated. (VS22)(v1.5.x)
// 
#pragma once
//#pragma warning(disable:4996)


/////////////////////////////////////////////////////////////////////////////// 
//
//! 시스템 / 플랫폼 헤더
//
/////////////////////////////////////////////////////////////////////////////// 
//#include "windows.h"
//#include "tchar.h"



/////////////////////////////////////////////////////////////////////////////// 
//
//! Yena 헤더
//
/////////////////////////////////////////////////////////////////////////////// 
// Yena 중요 클래스
//#include "B3Yena.h"
//#include "B3Define.h"		// 기본/공통 선언파일.
//#include "B3X9Types.h"		// DX9 대응 자료형 선언. (v1.5.2)@dev (25.0621)
//#include "B3Graphics.h"	// 그래픽스 엔진. (v1.5.2)@dev (25.0621)

/*
//Yena 확장 클래스
#include "B3Math.h"			// 수학 라이브러리.
#include "B3XSurface.h"		// 서피스 관리.
#include "B3Texture.h"		// 텍스쳐 관리.
#include "B3YDebug.h"		// 디버깅 관련.
#include "B3Light.h"		// 조명 관리 
#include "B3Shader.h"		// 쉐이더 관련.
*/



/////////////////////////////////////////////////////////////////////////////// 
//
//! Yena 클래스 선행 선언
//
/////////////////////////////////////////////////////////////////////////////// 

//.. ynB3Class.h 로 이동.



/////////////////////////////////////////////////////////////////////////////// 
//
//! Yena 그래픽스 상수
//
/////////////////////////////////////////////////////////////////////////////// 

//...

#define _omitted_
#define _기능_생략_	//_omitted_




/////////////////////////////////////////////////////////////////////////////
//
//! 정점 파이프라인(VP, Vertex Pipeline) 출력 구조체.
//
struct VS_OUTPUT
{
	VECTOR4 pos;	//위치 
	COLOR	diff;	//확산광 색상 (Diffuse Color)
};

//typedef _VS_OUTPUT VS_OUTPUT;

//! 각 VP 처리 결과 저장 배열.
typedef std::vector<VS_OUTPUT>  VSOUTS;







/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
// Yena SWR Engine Core  :  SW 렌더링 엔진 기반 클래스
//!
//! Yena SWR 엔진 기반 클래스  
//!  - 공통 데이터 및 인터페이스 제공
//! 
//! \code 
//! <파생 클래스>
//!  - B3YenaGraphics
//!  - B3YenaGraphicsEngine9/10/11/GL/VK    //그래픽스 엔진
//!  - B3YenaCopyEngine						//복사 엔진
//!  - B3YenaComputeEngine					//계산 엔진
//!  - B3YenaCUDAEngine						//쿠다 엔진
//!  - B3YenaRTXEngine						//RTX 엔진
//!  - B3YenaDLSSEngine						//DLSS 엔진
//! \endcode
//! 
//! \b 지원 버전
//!  - Yena SWR v1.5.3 이상
//! 
/////////////////////////////////////////////////////////////////////////////// 

class B3YenaCore							
{
	friend class B3YenaDevice9;
	friend class B3YenaDevice9x;

protected:


public:
	B3YenaCore(void);
	virtual ~B3YenaCore(void);

	//virtual int Draw() pure;
	
};

typedef B3YenaCore*		LPB3YENACORE;
typedef B3YenaCore*		LPB3YENAENGINECORE;
//typedef LPB3GRAPHICSENGINE  LPYENAGE;


// B3Yena9 개체 생성 함수
//B3EngineCore* B3YenaCreate9(DWORD ver);








/////////////////////////////////////////////////////////////////////////////// 
//
//! \class	B3YenaGraphicsEngine9  
//! \bstar	Yena 그래픽스 엔진 클래스
//! 
//!	렌더링 파이프라인 운용 및 그래픽스 API 구현 (D3D9 대응)
//!
//! \par [목적]
//!  - 렌더링 장치 Device 와 렌더링/그래픽스 Pipeline 기능 분리.
//!  - 신규 렌더링 엔진 개량, 배포 및 재사용성 증대
//! 
//! \par [운용]
//! 모든 렌더링(에 사용되는) 객체의 생성과 운용을 담당합니다. 일부 객체는 외부 참조됩니다.
//!  - RTs, DS, VBs, IB, CBs, UAVs
//!  - Render States, Pipeline State Objects (PSOs)
//!  - Lights, Materials,Textures, Samplers
//!  - Shaders
//!  - Transform Matrices, Viewports   
//! 공급된 데이터를 기반으로 그래픽스 연산 - 파이프라인을  진행하며 연산 결과는 
//! (외부 지정) 렌더타겟에 출력됩니다. 더하여 스테이지 별 디버깅 정보 생산 및 관리도 지원합니다.
//! 
//! \par [개발 테스트 베드]
//!  - Yena SWR v1.5.3 (25.0522)
//!  - 99_SpecularMap+3_(5box)_(vs22)+(v1.5.3)@dev3
//! 
//! \par [추후 개정]
//!  - 렌더링 파이프라인 병렬화
//!  - 정점/기하/픽셀 파이프라인 병렬화
//!  - Streaming Processor / CUDA Unit 구현
//! 
//! \note <Yena> 주요 렌더링 작업은 Yena GraphicsEngine 에서 처리합니다. (v1.5.3 이상)
//! \see B3YenaDevice9, IYenaDevice9
//! \ingroup 	Yena-Class
//
/////////////////////////////////////////////////////////////////////////////// 
//
class B3YenaGraphicsEngine9	: public B3YenaCore
{
	friend class B3YenaDevice9;
	friend class B3YenaDevice9x;


public:

	//! 파이프라인 입력 정보 및 객체 운용 : VB, IB, FVF 등...
	//! 
	struct INPUT_STREAM
	{
		//LPYENAVERTEXBUFFER9 pVB[16];		//!< 렌더링용 등록된 정점 버퍼.<DX9>
		LPYENAVERTEXBUFFER9	pVB[1];			//!< 렌더링용 등록된 정점 버퍼.<Yena>
		DWORD FVF;							//!< 렌더링용 등록된 정점 규격
		UINT  Stride;						//!< 렌더링용 등록된 정점 1마디의 크기


		//그리기 옵션.(DrawPrimitive 호출시 입력됨)
		B3YPRIMITIVETYPE PrimitiveType;		//!< 이번에 그려질 기하도형 타입. (디버깅용)
		UINT PrimitiveCount;				//!< 이번에 그려질 기하도형 개수. (Line, Face)
		UINT StartVertex;					//!< 이번에 그려질 (시작) 정점 번호.
		UINT TopologyVtxSize;				//!< 이번에 그려질 기하도형 구성 정점 개수 : Triangle-List = 1 Face, 3 Vertices, Line-List = 1 Line, 2 Vertices
	};


	//! 렌더링 - 스트리밍 실행중 데이터 (디버깅용)
	//! 
	struct STREAM_DBG
	{
		UINT  CurrVtxIndex;					//!< 현재 작업중인 정점 번호. 디버깅용.
		//UINT  FaceNum;					//!< 현재 작업중인 페이스 번호. 디버깅용.
		UINT  CurrPrimIndex;				//!< 현재 작업중인 기하도형 번호. 디버깅용.
		UINT  CurrPrimDrawCnt;				//!< 현재 작업중인 기하도형 개수. 
	};

	//! 변환용 데이터 운용
	//!
	struct TRANSFORM
	{
		B3YXMATRIX mTFM[B3YTS_MAX_];		//!< 변환 행렬들, 배열  
		//#define m_mWorld  mTFM[B3YTS_WORLD]
		//#define m_mView	mTFM[B3YTS_VIEW] 
		//#define m_mProj   mTFM[B3YTS_PROJECTION]

		//B3YXMATRIX mInvProj;				//!< 원근 보정용 행렬.
	};


	//! 파이프라인 상태 객체 운용
	//! 
	struct PIPELINE_STATE
	{
		DWORD  RState[B3YRS_MAX_];			//!< 렌더링 상태 값 
	};


	//! 출력/혼합 단계 정보 및 객체 운용 : 렌더타겟, 뷰포트 등..
	//!
	struct OUTPUT_MERGE
	{
		HWND hWnd;							//!< 출력 윈도 핸들
		YNPRESENT_PARAM	PresentParam;		//!< 출력 정보 (해상도, 포멧 등)
		
		HBITMAP  hBmpRT;					//!< 렌더타겟(Back-Buffer) 구성용 핸들.		
		HDC		 hRT;						//!< 렌더타겟 DC 핸들
		#define hSurfaceRT hRT				//!< 렌더타겟 DC 핸들.(구형호환성 유지용)		
	};



protected:

	B3YenaDevice9*	m_pDev;			//!< 렌더링 장치 (외부 참조)

	//그래픽 엔진 - 파이프라인 별 운용 데이터
	INPUT_STREAM	m_input;		//!< 파이프라인 입력 정보 및 객체 운용 : VB, IB, FVF 등...
	STREAM_DBG		m_stm;			//!< 렌더링 - 스트리밍 실행중 데이터 (디버깅용)
	TRANSFORM		m_tms;			//!< 변환용 데이터 운용
	PIPELINE_STATE	m_pso;			//!< 파이프라인 상태 객체 운용
	OUTPUT_MERGE	m_om;			//!< 출력/혼합 단계 정보 및 객체 운용 : 렌더타겟, 뷰포트 등..	

	//SHADER_STAGE	m_ss;

	//디버깅 정보 : BegineScene 부터 EndScene 까지 사용된 정보 누계. 이후 리셋됨. 
	//B3YDBGINFO	m_DbgInfo;


protected:
	//---------------------------
	// 내부 메소드들 : '밑줄 _ '접두어 표시.
	//---------------------------
	//...

	//---------------------------
	// 자원 - 버퍼 운용
	//---------------------------
	int  _RenderTargetCreate	();
	void _RenderTargetRelease	();
	HDC	 _GetDCRT				();
	 
	int _SetVertexBuffer		(IYenaVertexBuffer9* pVB, UINT Stride);
	int _SetFVF					(DWORD fvf); 

	void _TempBufferRelease		();

	
	//---------------------------
	// 장면 관리
	//---------------------------
	virtual int _BeginScene		();
	virtual int _EndScene		();
	virtual int _Clear			(COLORREF col);
	virtual int _Present		();



	//---------------------------
	// 파이프라인 스테이지
	//---------------------------
	virtual int _VertexStreaming	(_out_ B3YXVECTOR3 oPos[3], _out_ B3YXCOLOR oColor[3]);
	//virtual int _VertexAssembly	();
	virtual int _VertexPipeline		(_in_ B3YXVECTOR3 iPos[3], _out_ B3YXVECTOR4 oPos[3], _inout_ B3YXCOLOR oColor[3]);
	virtual int _GeometryPipeline	(_in_ B3YXVECTOR4 iPos[3], _out_ B3YXVECTOR3 oPos[3], _inout_ B3YXCOLOR oColor[3]);
	virtual int _PixelPipeline		(_in_ B3YXVECTOR3 oPos[3], _in_  B3YXCOLOR oColor[3]);

	virtual int _PrimitiveAssembly	(_in_ VSOUTS& vsouts, _out_ B3YXVECTOR4 oPos[3], _out_ B3YXCOLOR oColor[3]);
	//virtual int _Rasterization		(_inout_ B3YXVECTOR4 oPos[3], _inout_ B3YXCOLOR oColor[3]);
	virtual int _PixelPipeline1		();
	virtual int _PixelPipeline2		();
		

	virtual bool _FaceCulling2		(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2);
	virtual bool _FaceCulling3		(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2);
	virtual int  _Viewport			(B3YXVECTOR3 iPos[3], B3YXVECTOR2 oPos[3] );


	//---------------------------
	// 고정함수 스테이지
	//---------------------------
	//virtual VS_OUTPUT _TnL_VP(_inout_ B3YXVECTOR3 oPos[3], _inout_ B3YXCOLOR oColor0[3], _inout_ B3YXVECTOR3 oNormal[3], _inout_ B3YXVECTOR2 oTexCoord[3]);
	virtual VS_OUTPUT _TnL_VP	(B3YXVECTOR3 pos, B3YXCOLOR col);
	virtual int		  _TnL_GP	(_in_ B3YXVECTOR4 pos[3], _out_ B3YXVECTOR3 oPos[3], _inout_ B3YXCOLOR col[3]);
	virtual int		  _TnL_PP	(_in_ B3YXVECTOR2 pos[3], _in_ B3YXCOLOR col[3]);
	

	//---------------------------
	// 셰이더 스테이지
	//---------------------------
	//virtual VS_OUTPUT _VS_Main (B3YXVECTOR3 pos, B3YXCOLOR col);
	//virtual int		_GS_Main ();
	//virtual int		_PS_Main (B3YXCOLOR color, _out_ B3YXCOLOR& oColor);



	//---------------------------
	// 파이프라인 상태 조절
	//---------------------------
	virtual void _InitRenderState	();
	virtual int  _SetRenderState	(B3YRENDERSTATETYPE State, DWORD Value);
	virtual int  _GetRenderState	(B3YRENDERSTATETYPE State, DWORD* pValue);

	//--------------------------------
	// 변환 행렬 설정.
	//--------------------------------
	virtual int _SetTransform		(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* mTM);
	virtual int _GetTransform		(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* mTM);
		
	//---------------------------
	// 그래픽스 API
	//---------------------------
	//그리기 함수.: 클래스 B3XVECTOR 타입으로 갱신.
	virtual int	 _Line	(B3YXVECTOR2& v0, B3YXVECTOR2& v1, B3YXCOLOR& col);		
	virtual int  _Line	(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXCOLOR sc0, B3YXCOLOR ec0);
	virtual int  _Face	(B3YXVECTOR2 pos[3], B3YXCOLOR  col[3]);
	float _EdgeFunction(const B3YXVECTOR2& v0, const B3YXVECTOR2& v1, const B3YXVECTOR2& p);
	virtual void _HLine	(HDC hdc, int x1, int x2, int y, B3YXCOLOR c0, B3YXCOLOR c1);
  
	// 단순 색상 출력. 대부분의 기능은 _Rasterizer 로 이전.
	virtual void _SetPixel(int x, int y, B3YXCOLOR color);

	//레스터 라이저
	// Rasterizer : 픽셀 혼합 및 출력 관련 중요 스테이지.
	//            : 기존의 _SetPixel 의 기능 확장. Rasterizer Stage (DX10/11) 대응. 
	//void _Rasterizer(HDC hDC, int x, int y, B3YXCOLOR color0, B3YXCOLOR color1, float z, B3YXVECTOR2 tex);


	//---------------------------
	// 디버깅 정보 획득
	//---------------------------
	//B3YDBGINFO* _GetDbgInfo();

	
public:
	B3YenaGraphicsEngine9(void);
	virtual ~B3YenaGraphicsEngine9(void);
		
	virtual int PreDraw	(B3YPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount); 
	virtual int Draw	(); 
	virtual int PostDraw(); 
 
	virtual int Reset	();		//!< 렌더링 엔진 리셋.
	virtual int Release ();		//!< 렌더링 엔진 자원 해제.

	virtual void SetDev	(B3YenaDevice9* pDev) { m_pDev = pDev; }

};

typedef B3YenaGraphicsEngine9		B3YenaGraphics;				//DX9 대응.
typedef B3YenaGraphicsEngine9*		LPB3YENAGRAPHICSENGINE9;	//DX9 대응.
typedef LPB3YENAGRAPHICSENGINE9		LPGRAPHICS;





/*
/////////////////////////////////////////////////////////////////////////////// 
//
// B3GraphicsEngine10 : DX10 대응 렌더링 / 그래픽스 엔진 클래스, 
//					  : ID3D10Device 대응 
//
/////////////////////////////////////////////////////////////////////////////// 

class B3GraphicsEngine10 : public B3EngineCore
{
protected:

	//...DX10 대응 Device ...
};



/////////////////////////////////////////////////////////////////////////////// 
//
// B3GraphicsEngine11 : DX11 대응 렌더링 / 그래픽스 엔진 클래스,  
//                : ID3D11Device 대응 
//
//
/////////////////////////////////////////////////////////////////////////////// 

class B3GraphicsEngine11 : public B3EngineCore
{
protected:

	//...DX11 대응 Device ...
};



/////////////////////////////////////////////////////////////////////////////// 
//
// B3GraphicsEngineGL : OpenGL 대응 렌더링 / 그래픽스 엔진 클래스, 
//
//
/////////////////////////////////////////////////////////////////////////////// 

class B3GraphicsEngineGL : public B3EngineCore
{
protected:

	//...OpenGL 대응 Device ...
};



/////////////////////////////////////////////////////////////////////////////// 
//
//  B3EngineCore 시스템 서비스/공통 클래스들.. 
//
/////////////////////////////////////////////////////////////////////////////// 

//...

*/


/**************** end of "ynB3Graphics.h" ***********************************/
