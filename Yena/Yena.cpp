
///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 인터페이스 헤더 선언 : ynIUnknown / ynInterface 전용 
//
#include "Yena.h"



///////////////////////////////////////////////////////////////////////////////
//
// Yena SWR 구현 클래스 선언 : B3Class / B3xxx 클래스 전용, 외부 노출 방지용.
// (Yena v1.5.3 이상)
//
#include "ynB3Class.h"
#include "ynB3VertexBuffer.h"
#include "ynB3Graphics.h"




///////////////////////////////////////////////////////////////////////////////
//
// B3YenaGraphicsEngine 데이터 접근 재정의
// - 구형 / 신형 호환성 유지 (v1.2 이전)
// - 가독성 향상.
// 
///////////////////////////////////////////////////////////////////////////////
// B3YenaDevice9 전용
//#define m_pGraphics		this

#define m_pVB				m_pGraphics->m_input.pVB
#define m_FVF				m_pGraphics->m_input.FVF
#define m_Stride			m_pGraphics->m_input.Stride
#define m_TopologyVtxSize	m_pGraphics->m_input.TopologyVtxSize
//#define m_PrimVtxCnt		m_pGraphics->m_input.PrimTypeVtxSize
#define m_mWorld			m_pGraphics->m_tms.mTFM[B3YTS_WORLD]
#define m_mView				m_pGraphics->m_tms.mTFM[B3YTS_VIEW] 
#define m_mProj				m_pGraphics->m_tms.mTFM[B3YTS_PROJECTION]
#define m_mTFM				m_pGraphics->m_tms.mTFM
//#define m_mInvProj		m_pGraphics->m_tms.mInvProj
#define m_RState			m_pGraphics->m_pso.RState
#define m_PresentParam		m_pGraphics->m_om.PresentParam







/////////////////////////////////////////////////////////////////////////////// 
//
//! \class	B3Yena 
//! \brief	Yena 3D 운용 기반, 구현 클래스, IDirect3D9 대응 
//! 
//! <DX>	DX9 3D 렌더링 장치(Device)를 생성하려면 우선 Direct3D9 객체가 필요합니다.  
//!			이 Direct3D9 는 시스템이 보유한 물리적 장치(GPU, Adaptor) 의 지원 성능(Features)을 점검하고 렌더링 장치(Direct3DDevice9) 을 생성합니다.  
//!			Direct3D9 으로 생성되는 모든 장치들은 물리적 자원을 공유하지만, 다중 렌더링 장치 운용시에는 상당한 성능 저하의 요인이 될 수 있습니다.  
//! <Yena>	B3Yena 와 B3YenaDevice9 은 각각 IDirect3D 와 IDirect3DDevice9 을 대응하며 인터페이스를 구현합니다.  
//! 
//! \copydetails yxx_interface_table
//! 
//! \see	IYena, B3Yena
//! \see	IYenaDevice9, B3YenaDevice9
//! \see	[IDirect3D9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nn-d3d9-idirect3d9)
//! \see	[IDirect3DDevice9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9helper/nn-d3d9helper-idirect3ddevice9)
//! \see	[D3D9:Creating Device](https://learn.microsoft.com/en-us/windows/win32/direct3d9/creating-a-device)
//! \see	[IDirect3D9::CreateDevice](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3d9-createdevice)
//! \see	\ref Yena-Class
//! \see	\ref Yena-Interfaces
//! \see	\ref Yena-Math
//! \remarks Yena 는 DX 인터페이스와 메소드의 시그니쳐(Signature)를 가능한 동일하게 준수합니다.  
//!			 구현 클래스 B3Yena 을 참고하십시오.
//! \ingroup Yena-Class
//! 
//
/////////////////////////////////////////////////////////////////////////////// 
//
class B3Yena : public IYena					
{
protected:
	//레퍼런스 참조 카운트 
	ULONG m_ref = 0;

public:
	B3Yena(void);
	virtual ~B3Yena(void);


	//-----------------------------------------------------------------
	// 인터페이스 재정의
	// DX 와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다. 
	//----------------------------------------------------------------

	//! Yena SWR 렌더링 디바이스 개체 생성 : D3D9 대응
	virtual int CreateDevice(HWND hwnd,						//[in]디바이스의 렌더링 목표 윈도우 핸들.
							YENAPRESENT_PARAMETERS* pp,		//[in]디바이스 화면 구성 정보.	
							DWORD vp,						//[in]정점연산 방법 결정 (현재는 SW 만 가능)
							LPYENADEVICE9* pDev				//[out]성공시 리턴받을 디바이스 개체 포인터.
	);


	//참조 카운트 메소드 재정의 
	virtual ULONG AddRef		(void);
	virtual ULONG Release		(void);
	virtual int   QueryInterface(YN_IID riid, _out_ void** ppvObject);

};

//typedef B3Yena* LPB3YENA;



//! Yena 개체 생성 함수 (D3D9 대응)
//! 
//B3Yena* B3YenaCreate9(DWORD ver);






/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
//! \class	B3YenaDevice9 
//! \brief	Yena 렌더링 장치 Device 구현 클래스 
//!			 : IDirect3DDevice9 대응   
//! 
//! 3D 프로그래밍에서 장치(Device)는 시스템에 설치된 그래픽 하드웨어(어뎁터 Adaptor) 를 
//! 의미하며 넓은 의미에서 GPU 자체로 생각할 수 있습니다.   
//! 일반적인 장치들의 중요 기능(Features)은 다음과 같습니다.  
//!  - 렌더링 장치(GPU), 메모리(VRAM) 운용.           
//!  - 그래픽스 파이프라인 (Pipeline) 운용.      
//!  - 렌더링 자원(Resource) 생성 및 관리 : VB, IB, RT, DSB 등.   
//!  - 렌더링 상태(Render State) 조절.      
//!  - 스왑체인(SwapChain) 운용 : 다중 버퍼링, 렌더타겟 개수, 해상도, 색상 포멧 등.    
//!  - 셰이더 (Shader) 파이프라인 : 고수준 GPU 프로그래밍 HLSL, SPIR-V 지원.   
//!  - 계산 파이프라인 (Compute Pipeline) 운용.    
//!  - 영상 미디어 : 비디오 스트리밍 인코딩/디코딩.     
//! 
//! DirectX, OpenGL, Vulkan 같은 전문 저수준 그래픽스 API 들은 장치 운용을 위한 
//! 전용 객체(인터페이스)를 제공하며 사용자는 이를 통해 필요한 자원 생성, 작업 명령 및 
//! 장치 상태를 조작 할 수 있습니다.  
//! Yena 는 DX9 (Direct3D9) 의 장치 운용 방식을 준수하겠습니다.
//! 
//! \par Yena 와 렌더링 장치 
//! Yena 의 SW 렌더링 장치는 HW 렌더링 장치(GPU, DX) 와 최대한 동일 기능을 지원하는 것이 목표입니다.  
//! 각 세부 기능은 B3YenaDevice9 에서 구현되며 사용자 인터페이스를 위해 IYenaDevice9 가 제공됩니다.  
//! 이 인터페이스는 DX 의 IDirect3DDevice9 을 대응합니다.  
//! Yena 렌더링 장치는 렌더링 관련 중요 SW 기능을 수행하며 학과 진행에 맞추어 많은 기능들이 추가될 것입니다.   
//!  
//! <DX>	DX9 3D 렌더링 장치(Device)를 생성하려면 우선 Direct3D9 객체가 필요합니다.  
//!			이 Direct3D9 는 시스템이 보유한 물리적 장치(GPU, Adaptor) 의 지원 성능(Features)을 점검하고 렌더링 장치(Direct3DDevice9) 을 생성합니다.  
//!			Direct3D9 으로 생성되는 모든 장치들은 물리적 자원을 공유합니다. 이는 다중 렌더링 장치 운용시 상당한 성능저하의 요인이 됩니다.   
//! <Yena>	IYena 와 IYenaDevice9 은 각각 IDirect3D 와 IDirect3DDevice9 을 대응하며 가능한 동일한 인터페이스로 구성됩니다.   
//! 
//! \see	IYenaDevice9, B3YenaGraphicsEngine9
//! \see	[IDirect3D9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nn-d3d9-idirect3d9)
//! \see	[IDirect3DDevice9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9helper/nn-d3d9helper-idirect3ddevice9)
//! \see	[D3D9:Creating Device](https://learn.microsoft.com/en-us/windows/win32/direct3d9/creating-a-device)
//! \see	[IDirect3D9::CreateDevice](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3d9-createdevice)
//! \note	<Yena> 주요 렌더링 작업은 Yena GraphicsEngine 에서 처리합니다. (v1.5.3 이상)
//! \remarks Yena 는 DX 인터페이스와 메소드의 시그니쳐(Signature)를 가능한 동일하게 준수합니다.  
//!			 구현 클래스 B3YenaDevice9 을 참고하십시오.
//! \ingroup Yena-Class
//!
//
/////////////////////////////////////////////////////////////////////////////// 
//
class B3YenaDevice9 : public IYenaDevice9			
{
	friend class B3Yena;

protected:
	//레퍼런스 참조 카운트 
	ULONG m_ref = 0;

protected:
	// 장치 운용 기본 정보.
	HWND  m_hWnd;							//!< 출력 윈도우 핸들
	DWORD m_VertexProcessing;				//!< 정점 연산 방식
	//YENADISPLAYMODE  m_Display; 

	// 렌더타겟(Back-Buffer) 구성용 핸들.
	//HBITMAP  m_hBmpRT;
	HDC		 m_hRT;							//!< 렌더타겟 DC 핸들
	COLORREF m_BkColor;						//!< 렌더타겟 지우기 색
	#define m_hSurfaceRT m_hRT				//!< 렌더타겟 DC 핸들 (구형호환 재정의)


	//! 렌더링 그래픽스 엔진
	B3YenaGraphicsEngine9* m_pGraphics;
	#define m_pGX	m_pGraphics

	//카피 엔진 
	//B3YenaCopyEngine9* m_pCopy;

	//컴퓨트 엔진
	//B3YenaComputeEngine9* m_pCompute;


protected:
	// 내부 메소드들. '밑줄 _ '을 접두어로 표시합니다.
	//
	int _Create(HWND hwnd, YENAPRESENT_PARAMETERS* pp, DWORD vp);


	//----------------------------------------
	// 렌더링 파이프 라인 : 스테이지 메소드.
	//----------------------------------------
	// --> Graphics Engine 으로 이동 ...

	//----------------------------------------
	// 스왑체인 : 렌더타겟 메소드
	//----------------------------------------
	// --> Graphics Engine 으로 이동 ...

	//----------------------------------------
	// 그리기 함수
	//----------------------------------------
	// --> 클래스 B3XVECTOR 타입으로 갱신. 
	// --> Graphics Engine 으로 이동 ...

	//----------------------------------------
	// 렌더링 상태 함수.
	//----------------------------------------
	// --> Graphics Engine 으로 이동 ...


public:
	B3YenaDevice9(void);
	virtual ~B3YenaDevice9(void);
	
	
	//-----------------------------------------------------------------
	// 인터페이스 재정의
	// DX 와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다. 
	//-----------------------------------------------------------------
	//--------------------------------
	// 스왑체인 및 렌더타겟 운용 메소드들
	//--------------------------------
	virtual int BeginScene	();
	virtual int EndScene	();
	virtual int Clear		(COLORREF col);
	virtual int Present		();

	//--------------------------------
	// 기하 버퍼 및 렌더링 메소드들 
	//--------------------------------
	virtual int CreateVertexBuffer	(UINT Length, DWORD Usage, DWORD FVF, B3YENAPOOL Pool, _out_ IYenaVertexBuffer9** ppVB, _in_opt_ HANDLE* pSharedHandle);
	virtual int SetStreamSource		(UINT SteamNumber, IYenaVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride);
	virtual int SetFVF				(DWORD FVF);
	virtual int DrawPrimitive		(B3YPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
	
	//--------------------------------
	// 렌더링 상태 조절 메소드들.
	//--------------------------------
	virtual int SetRenderState	(B3YRENDERSTATETYPE	State, DWORD Value);
	virtual int GetRenderState	(B3YRENDERSTATETYPE State, DWORD* pValue);

	//--------------------------------
	// 변환 행렬 설정.
	//--------------------------------
	virtual int SetTransform	(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* mTM);
	virtual int GetTransform	(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* mTM);

		
	//--------------------------------
	// 멤버데이터 접근자 Accessors : Yena 전용
	//--------------------------------
	virtual HDC		 GetRT		();
	virtual COLORREF GetBkColor	();


	//---------------------------
	// 공간 가시화 장치 (Gizmos) :  <DX> 미지원. <Yena> 전용 "친절한 예나씨" 
	//---------------------------
	virtual void  DrawGrid		();		//그리드 출력.
	virtual void  DrawAxis		();		//방향축 출력.


	//--------------------------------
	// 참조 카운트 메소드 재정의 
	//--------------------------------
	virtual ULONG AddRef		(void);
	virtual ULONG Release		(void);
	virtual int   QueryInterface(YN_IID riid, _out_ void** ppvObject);

};

//typedef B3YenaDevice9* LPB3YENADEVICE9;		//DX9 대응.

//#define CHECKSTATE( state, val ) (m_RState[(state)] == (val))	









/////////////////////////////////////////////////////////////////////////////// 
//
//! class	B3YenaVertexBuffer9
//! brief	정점 버퍼 구현 클래스
//!			: IDirect3DVertexBuffer9 대응  
// 
/////////////////////////////////////////////////////////////////////////////// 
/*
*   B3YenaVertexBuffer9 : 클래스 선언 
*
*		-> ynB3YenaVertexBuffer.h 로 이동
*
*/








/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

////////////////////////////////////////////////////////////////////////////////
//
//! YenaCreate9 : B3Yena 최상위 인터페이스 생성 함수
//! 
//! \param	ver		Yena 버전
//! \return		성공시 IYena 인터페이스를 리턴.
//
IYena* YenaCreate9(DWORD ver)
{
	//COM 객체 생성.
	B3Yena* pB3Yena = new B3Yena;
	assert(pB3Yena != NULL);

	/*//인터페이스 생성 "RTTI" 
	//QueryInterface 등의 메소드 사용이 기본이지만, Yena 에서는 직접 형변환 하겠습니다.
	//이쪽이 COM Interface 의 구조와 동작의 이해에 도움이 될 것입니다.
	IYena* pYena = dynamic_cast<IYena*>(pB3Yena);
	assert(pYena != NULL);
	//객체 참조카운트 증가.
	pB3Yena->AddRef();					
	*/

	//인터페이스 질의 및 획득.(GUID 및 Query) 
	//GUID 와 Query 를 사용하는 것이 보다 DX/COM 표준 운용에 가깝습니다.
	//획득한 인터페이스는 사용후 Release 를 호출해야 합니다. (안전한 메모리 해제)
	IYena* pYena = nullptr;
	if (YN_FAILED(pB3Yena->QueryInterface(IID_IYena, (void**)&pYena)))
	{
		//...ERROR...
	}


	//외부로 리턴.
	return pYena;
}



/*
////////////////////////////////////////////////////////////////////////////////
//
//! ynGetObject 
//! Yena 인터페이스와 연결된 실제 구현 클래스 객체를 획득합니다.
//! B3Yena 및 ynIUnknown 을 상속한 구현 클래스만 획득이 가능합니다.
//! 
//! Windows/COM 의 GetClassObject 함수는 CLSID 의 객체 인터페이스를 리턴합니다.
//! Yena 는 Windows/COM 과는 다르게 실제 구현 클래스 객체를 리턴합니다.
//! \see [참고] DLLGetClassObject (https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-dllgetclassobject)
//!
//! \remarks 이 함수는 Yena 개발팀 내부 전용으로 외부 사용자에게는 노출되지 않습니다.
//! \param	 ypInterface	구현 클래스 인터페이스 포인터
//! \return	 성공시 구현 클래스 포인터, 실패시 NULL.
//
template<class T> 
T* ynGetClassObject(ynIUnknown* ypInterface)
{
	T* pObj = dynamic_cast<T*>(ypInterface);
	if (YN_INVALIED(pObj))
	{
		//에러 처리...
		return YN_NULL;
	}

	//참조 카운트 증가
	//...

	return pObj;
}
*/


////////////////////////////////////////////////////////////////////////////////
//
//! ynGetObject 
//! Yena 인터페이스로 실제 구현 클래스 객체를 획득합니다.
//! B3Yena 및 ynIUnknown 을 상속한 구현 클래스만 획득이 가능합니다.
//! \remarks 이 함수는 Yena 개발팀 내부 전용으로 외부 사용자에게는 노출되지 않습니다.
//! 
//! Windows/COM 의 GetClassObject 함수는 CLSID 의 객체 인터페이스를 리턴합니다.
//! Yena 는 Windows/COM 과는 살짝 다르게 만들겠습니다.
//! \see [참고] DLLGetClassObject (https://learn.microsoft.com/en-us/windows/win32/api/combaseapi/nf-combaseapi-dllgetclassobject)
//!  
//! \param	ypInterface	구현 클래스 인터페이스 포인터
//! \return	성공시 구현 클래스 포인터, 실패시 NULL.
//
/*int ynGetClassObject(YNCLSID clsid, void* ypInterface)
{
	T* pObj = dynamic_cast<T*>(ypInterface);
	if (YN_INVALIED(pObj))
	{
		//에러 처리...
		return YN_FALSE;
	}

	//참조 카운트 증가
	//...

	return YN_OK;
}*/






/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
// class B3Yena : 정의 와 구현
//	            : IDirect3D9 대응
//
/////////////////////////////////////////////////////////////////////////////// 


///////////////////////////////////////////////////////////////////////////////
//
B3Yena::B3Yena(void)
{
#ifdef _DEBUG
	ynLog(_T("B3Yena 생성됨..."));
#endif
}



///////////////////////////////////////////////////////////////////////////////
//
B3Yena::~B3Yena(void)
{
#ifdef _DEBUG
	ynLog(_T("B3Yena 제거됨..."));
#endif
}




/////////////////////////////////////////////////////////////////////////////// 
//
//! Yena SWR 렌더링 디바이스 개체 생성 : D3D9 대응
//! 
//! \param	hwnd		디바이스의 렌더링 목표 윈도우 핸들.
//! \param	pp			디바이스 화면 구성 정보.	
//! \param	vp			정점연산 방법 결정 (현재는 SW 만 가능)
//! \param	ppDevice	성공시 리턴받을 장치(인터페이스) 포인터.
//! \return		성공시 OK, 실패시 FALSE
//
int B3Yena::CreateDevice(HWND hwnd, 
						 YENAPRESENT_PARAMETERS* pp, 
						 DWORD vp, 
						 LPYENADEVICE9* ppDevice
						 )
{
	//입력 정보 확인 : 오류별로 리턴값을 다르게 하는 것을 추천.
	if(hwnd == NULL) return YN_FAIL;
	if(pp == NULL) return YN_FAIL;
	if(ppDevice == NULL) return YN_FAIL;


	//디바이스 개체 생성. : DX9 대응
	B3YenaDevice9* pB3Dev = new B3YenaDevice9;
	ASSERT(pB3Dev);
	pB3Dev->_Create(hwnd, pp, vp);


	/*//인터페이스 생성 "RTTI" 
	//QueryInterface 등의 메소드 사용이 기본이지만, Yena 에서는 직접 형변환 하겠습니다.
	//이쪽이 COM Interface 의 구조와 동작의 이해에 도움이 될 것입니다.
	IYenaDevice9* pDev = dynamic_cast<IYenaDevice9*>(pB3Dev);
	assert(pDev != NULL);
	//객체 참조카운트 증가.
	pB3Dev->AddRef();
	*/

	//인터페이스 질의 및 획득.(GUID 및 Query) 
	//GUID 와 Query 를 사용하는 것이 보다 DX/COM 표준 운용에 가깝습니다.
	//획득한 인터페이스는 사용후 Release 를 호출해야 합니다. (안전한 메모리 해제)
	IYenaDevice9* pDev = nullptr;
	if (YN_FAILED(pB3Dev->QueryInterface(IID_IYenaDevice9, (void**)&pDev)))
	{
		//...ERROR...
	}


	//외부에 리턴..W
	*ppDevice = pDev;

	return YN_OK;  
}




////////////////////////////////////////////////////////////////////////////////
//
//! Yena COM 참조 카운트 증가 
//! 
//! \return	현재 참조 카운트
//
ULONG B3Yena::AddRef(void)
{ 
	 return ++m_ref;
};




////////////////////////////////////////////////////////////////////////////////
//
//! Yena COM 참조 카운트 감소, 0 이하면 객체 제거. 
//! 
//! \return	현재 참조 카운트
//
ULONG B3Yena::Release(void)
{
	if (--m_ref <= 0) delete this; 

	return m_ref;
};




////////////////////////////////////////////////////////////////////////////////
//
//! Yena COM 인터페이스 획득.
//! 
//! <COM> 객체에 인터페이스를 질의하고 유효한 경우 해당 인터페이스가 리턴됩니다. 
//!	지정 인터페이스 ID 로 참조되는 (원본) 객체는 참조 카운트가 증가되므로 
//! 인터페이스 사용 후 안전한 메모리 해제를 위해 Release 를 호출해야 합니다.
//! 
//! <Yena> Yena 전용 GUID 를 통해 해당 ynInterface 를 획득하며 
//! 기본적인 기능은 Windows / COM 과 동일합니다.
//! 
//! \param	riid		Yena 인터페이스 ID
//! \param	ppvObject	(생성된) Yena 인터페이스 포인터.
//! \return		성공시 OK, 실패시 FALSE
//
int B3Yena::QueryInterface(YN_IID riid, _out_ void** ppvObject)
{
	//void* pInterface = nullptr;

	//지정 ID 의 유효성 검사..(간략)
	//if(riid == IID_IYena)
	//if (IsEqualGUID((GUID&)riid, (GUID&)IID_IYena))	//GUID 동일 검사
	//if (IsEqualIID((IID&)riid, (IID&)IID_IYena))		//IID 동일 검사 <상동>
	if (ynIsEqualIID(riid, IID_IYena))					//IID 동일 검사 <상동>
	{
		AddRef();										//참조 카운트 증가.
		*ppvObject = dynamic_cast<IYena*>(this);		//지정 인터페이스 리턴.
	}

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
// class B3YenaDevice9 : 정의 와 구현
//					   : IDirect3DDevice9대응
//
/////////////////////////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////////////////////////////////
//
B3YenaDevice9::B3YenaDevice9(void)
{
	m_hWnd = NULL;
	m_VertexProcessing = 0;

	m_hRT = NULL;		 
	m_BkColor = RGB(0, 0, 255);	

	m_pGraphics = nullptr;

#ifdef _DEBUG
	ynLog(_T("B3YenaDevice9 생성됨..."));
#endif

}



///////////////////////////////////////////////////////////////////////////////
//
B3YenaDevice9::~B3YenaDevice9(void)
{ 

	SafeDelete(m_pGraphics);	

#ifdef _DEBUG
	ynLog(_T("B3YenaDevice9 제거됨..."));
#endif
}




/////////////////////////////////////////////////////////////////////////////// 
//
//! 지정 렌더링 디바이스 개체 생성 메소드.
//
int B3YenaDevice9::_Create(	HWND hwnd,
							YENAPRESENT_PARAMETERS* pp,
							DWORD vp
						)
{
	//입력 정보 확인 : 오류별로 리턴값을 다르게 하는 것을 추천.
	if (YN_INVALIED(hwnd)) return YN_FAIL;
	if (YN_INVALIED(pp))   return YN_FAIL;


	//그래픽스 엔진 생성 (v1.5.3) 
	LPB3YENAGRAPHICSENGINE9 pGraphics = new B3YenaGraphicsEngine9;
	ASSERT(pGraphics);

	//디바이스/그래픽스 엔진 초기화.
	B3YenaGraphicsEngine9::OUTPUT_MERGE om = {};
	om.hWnd = hwnd;
	om.PresentParam = *pp;
	pGraphics->m_om = om;
	//pGraphics->m_om.VertexProcessing = vp;
	pGraphics->_RenderTargetCreate();			//렌더타겟(백버퍼) 생성 <Yena> 1개만 지원.


	//디바이스 - 그래픽스 엔진 바인딩.
	pGraphics->SetDev(this);

	//디바이스 - 그래픽스 엔진 정보 참조.
	m_hWnd = hwnd;
	m_VertexProcessing = vp;
	m_pGraphics = pGraphics;
	m_hSurfaceRT= pGraphics->_GetDCRT();
	m_BkColor	= B3YXCOLOR(0.25f, 0.25f, 0.25f, 1);
	//m_pDbgInfo = pGraphics->_GetDbgInfo();

	//외부에 리턴..
	//*ppDevice = pDev;

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! 렌더타겟 DC 핸들 획득
//! 
//! \return	렌더타겟 DC 핸들
//
HDC B3YenaDevice9::GetRT()
{ 
	m_hSurfaceRT = m_pGraphics->_GetDCRT(); 
	return m_hSurfaceRT;

}

 


///////////////////////////////////////////////////////////////////////////////
//
//! 렌더타겟 배경색 획득
//
COLORREF B3YenaDevice9::GetBkColor()
{ 
	return m_BkColor; 
}




///////////////////////////////////////////////////////////////////////////////
//
//! 장면 그리기 시작 : 렌더링에 필요한 (디바이스의) 선위 작업을 수행.
//
int B3YenaDevice9::BeginScene()
{ 
	m_pGraphics->_BeginScene();

	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 장면 그리기 종료 : 렌더링 종료에 필요한 (디바이스의) 후위 작업을 수행.
//
int B3YenaDevice9::EndScene()
{
	m_pGraphics->_EndScene();

	return YN_OK;  
}




///////////////////////////////////////////////////////////////////////////////
//
//! 렌더타겟 클리어.
//!
//! \param  col	RT 을 지울 지정색.
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaDevice9::Clear(COLORREF col)
{ 	
	m_BkColor = col;
	m_pGraphics->_Clear(col);

	return YN_OK;  
}




///////////////////////////////////////////////////////////////////////////////
//
//! RT 의 내용(렌더링된 장면)을 Front Buffer 에 출력합니다. "Flipping", "Swapping"
//!
//! \param   없음
//! \return  오류시 YN_OK 이외의 값.
//! \note <Yena> 주요 렌더링 작업은 Yena GraphicsEngine 에서 처리합니다. (v1.5.3 이상)
//! \see	B3YenaGraphicsEngine9::_Present
//
int B3YenaDevice9::Present() 
{
	return m_pGraphics->_Present();
}




////////////////////////////////////////////////////////////////////////////////
//
//! 정점 버퍼 생성.
//! \note 생성된 정점 버퍼는 렌더링 전, 장치(Device)에 설정(Binding) 되어야 합니다.
//! 
//! \param	Length			정점 버퍼의 전체 크기 (바이트)	
//! \param	Usage			버퍼 용도 (미지정은 0)
//! \param	FVF				정점 규격
//! \param	Pool			버퍼-메모리 형식 : 시스템메모리 사용. (Yena 유일 옵션) 
//! \param	ppVertexBuffer	성공시 리턴되는 정점 버퍼 인터페이스 포인터
//! \param	pSharedHandle	공유 헨들 (기본 NULL)
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaDevice9::CreateVertexBuffer( UINT Length, 
									   DWORD Usage,
									   DWORD FVF, 
									   B3YENAPOOL Pool, 
									   _out_ IYenaVertexBuffer9** ppVertexBuffer,
									   _in_opt_ HANDLE* pSharedHandle
									  )
{

	//정점 버퍼 운용 개체 생성.
	LPB3YENAVERTEXBUFFER9 pB3VB = new B3YenaVertexBuffer9;		
	if(YN_INVALIED(pB3VB)) return YN_FAIL;
	 
	//생성할 정점 버퍼 정보 기술
	//<Yena> 시스템 메모리를 사용하므로 필수 동작은 아니지만, 구조의 일관성을 위해 사용.
	//GPU 메모리(VRAM) 확보를 위해서 이 정도의 수고는 필요하다...정도로 이해해 봅시다.
	B3YVERTEXBUFFER_DESC desc = {};
	desc.Format = B3YFMT_VERTEXDATA;		//버퍼 포멧 : 정점 데이터용
	desc.Type	= B3YRTYPE_VERTEXBUFFER;	//버펴 형식 : 정점 버퍼
	desc.Usage	= B3YUSAGE_WRITEONLY;		//버퍼 사용방식 : 읽기 전용, <주> 정점버퍼는 CPU "쓰기전용", GPU "읽기 전용" 임을 상기합시다.
	desc.Pool	= Pool;						//버퍼 메모리 풀 형식
	desc.Size	= Length;					//버퍼 크기
	desc.FVF	= FVF;						//버퍼에 저장될 정점 규격
	
	//정점 버퍼 생성.
	if (YN_FAILED(pB3VB->_Create(desc)))	
	{
		//Error...
		return YN_FAIL;
	}

	/* //정점 버퍼 생성 (구형)
	if (YN_FAILED(pB3VB->_Create(Length, FVF, Pool)))	 
	{
		//Error...
		return YN_FAIL;
	}*/


	/*//인터페이스 생성 "RTTI" 
	//QueryInterface 등의 메소드 사용이 기본이지만, Yena 에서는 직접 형변환 하겠습니다.
	//이쪽이 COM Interface 의 구조와 동작의 이해에 도움이 될 것입니다.
	IYenaVertexBuffer9* pVB = dynamic_cast<IYenaVertexBuffer9*>(pB3VB);
	assert(pVB != NULL);
	//객체 참조카운트 증가.
	pVB->AddRef();
	*/

	//인터페이스 질의 및 획득.(GUID 및 Query) 
	//GUID 와 Query 를 사용하는 것이 보다 DX/COM 표준 운용에 가깝습니다.
	//획득한 인터페이스는 사용후 Release 를 호출해야 합니다. (안전한 메모리 해제)
	IYenaVertexBuffer9* pVB = nullptr;
	if (YN_FAILED(pB3VB->QueryInterface(IID_IYenaVertexBuffer9, (void**)&pVB)))
	{
		//...ERROR...
	}

	//외부로 리턴..
	*ppVertexBuffer = pVB;

	return YN_OK;  
}




////////////////////////////////////////////////////////////////////////////////
//
//! 정점 버퍼를 디바이스에 등록.  
//! 
//! <DX> 복수의 정점 버퍼로 동시 '기하 스트리밍'을 지원합니다. (DX9, 최대 16)
//! 또는 지정 VB 내의 데이터중 부분 렌더링(OffsetBytes)도 가능합니다.  
//! \code
//!      GraphicsEngine.pVB[StreamNumber] = pVB; 
//! \endcode
//! <Yena> 1개의 정점 버퍼만 사용합니다. 
//! 
//! 
//! \param	StreamNumber	(기하, 정점)스트리밍 색인 (Yena 기본값은 0)
//! \param	pVB				렌더링 할 정점 버퍼 포인터.
//! \param	OffsetBytes		(기하,정점) 스트리밍 시작 색인 (Yena 기본값은 0)
//! \param	Stride			렌더링 할 버퍼의 1마디(정점 데이터) 크기
//! \return		성공시 OK, 실패시 FALSE
//! \note <Yena> 주요 렌더링 작업은 Yena GraphicsEngine 에서 처리합니다. (v1.5.3 이상)
//! \see	B3YenaGraphicsEngine9::_SetVertexBuffer
//
int B3YenaDevice9::SetStreamSource(UINT StreamNumber, IYenaVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride)
{
	if (YN_INVALIED(pVB)) return YN_FAIL;

	// <Yena> 1개의 정점 버퍼만 사용합니다. 
	// 그래픽엔진에 설정 : 바인딩 Binding 
	m_pGraphics->_SetVertexBuffer(pVB, Stride);


	// 이하 생략.. <Yena> 미지원
	// ... = StreamNumber
	// ... = OffsetBytes

	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 정점 규격을 설정합니다. 
//! 
//! \note 렌더링시 장치에 설정된 정점버퍼 내 정점과 동일 규격을 명시해야 합니다.
//!		  그렇지 않다면 정상적인 렌더링 결과를 기대할 수 없습니다.
//! \see  B3YenaGraphicsEngine9::_SetFVF
//
int B3YenaDevice9::SetFVF(DWORD fvf)
{	
	m_pGraphics->_SetFVF(fvf);

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 작업을 시작합니다.  
//! 
//! 이 메소드의 호출 이전에 정점버퍼, FVF, 마디 크기(Stride) 등 렌더링에 필요한
//! 모든 정보가 디바이스에 등록되어 있어야 합니다.  
//! 
//! \param	PrimitiveType	렌더링 기하 타입.
//! \param	StartVertex		렌더링 정점 시작번호 (정점 버퍼 안)
//! \param	PrimitiveCount	렌더링 기하 개수.
//! \return		성공시 OK, 실패시 FALSE
//! \note <Yena> 주요 렌더링 작업은 Yena GraphicsEngine 에서 처리합니다. (v1.5.3 이상)
//! \see	B3YenaGraphicsEngine9::Draw
//
int B3YenaDevice9::DrawPrimitive(B3YPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	if (YN_INVALIED(m_pVB)) return YN_FAIL;
	if (YN_INVALIED(m_Stride)) return YN_FAIL;

	//------------------------------------------------------------
	// 렌더링 준비
	// 렌더링 전 디바이스에 공급된 개별 데이터를 재구성하여
	// 렌더링 각 단계별 반복적으로 계산되는 상수 데이터를 
	// 미리 계산합니다.  파이프라인의 연산 속도 개선이 목적.
	//------------------------------------------------------------
	//------------------------------------------------------------
	// 그래픽 엔진 정보 구성
	//------------------------------------------------------------ 
	m_pGraphics->PreDraw(PrimitiveType, StartVertex, PrimitiveCount);


	//------------------------------------------------------------
	// 렌더링 시작
	// 각 렌더링 파이프라인 단계별 적절한 연산이 수행되며
	// 그 결과를 다음 파이프라인으로 공급합니다.
	// Yena 는 각 기능을 가능한 상세하게 구현하겠습니다.
	// 
	// 각 변환에 필요한 데이터는 DrawPrimitive 함수가 호출되기 전에
	// Device 에 공급되어야 합니다. 
	//------------------------------------------------------------
	m_pGraphics->Draw();


	//------------------------------------------------------------
	// 렌더링 종료 - 파이프라인 종료 후처리 
	// 임시 버퍼 해제, 디버깅 정보 정리 등...
	//------------------------------------------------------------
	m_pGraphics->PostDraw();


	return YN_OK;
}


 

///////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 상태 조절.
//
int B3YenaDevice9::SetRenderState(B3YRENDERSTATETYPE state, DWORD value)
{
	m_pGraphics->_SetRenderState(state, value);
	return YN_OK;
}



///////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 상태 얻기   
//
int B3YenaDevice9::GetRenderState(B3YRENDERSTATETYPE state, DWORD* value)
{
	m_pGraphics->_GetRenderState(state, value);
	 
	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 변환 행렬 설정   
//
int B3YenaDevice9::SetTransform(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* m)
{
	m_pGraphics->_SetTransform(ts, m);
	//m_mTFM[ts] = *m;

	return YN_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
//! 변환 행렬 얻기  
//
int B3YenaDevice9::GetTransform(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* m)
{
	m_pGraphics->_GetTransform(ts, m);
	//*m = m_mTFM[ts];

	return YN_OK;
}



///////////////////////////////////////////////////////////////////////////////
//
//! 공간 가시화 : 격자(Grid) 그리기
//! 
//! <DX> 미지원. "친절한 예나씨" , 내부의 코드는 '그려려니' 합시다.
//
void B3YenaDevice9::DrawGrid()
{
	m_pGraphics->_DrawGrid();
}




///////////////////////////////////////////////////////////////////////////////
//
//! 공간 가시화 : 방향축(Axis) 그리기
//! 
//! <DX> 미지원. "친절한 예나씨" , 내부의 코드는 '그려려니' 합시다.
//
void B3YenaDevice9::DrawAxis()
{
	m_pGraphics->_DrawAxis();
}




 

////////////////////////////////////////////////////////////////////////////////
//
//! AddRef : Yena COM 참조 카운트 증가 
//! 
//! \return	현재 참조 카운트
//
ULONG B3YenaDevice9::AddRef(void)
{
	return ++m_ref;
};




////////////////////////////////////////////////////////////////////////////////
//
//! Release : Yena COM 참조 카운트 감소, 0 이하면 객체 제거.
//! 
//! \return	현재 참조 카운트
//
ULONG B3YenaDevice9::Release(void)
{
	if (--m_ref <= 0) delete this;

	return m_ref;
};




////////////////////////////////////////////////////////////////////////////////
//
//! QueryInterface : Yena COM 인터페이스 획득.
//! 
//! <COM> 객체에 인터페이스를 질의하고 유효한 경우 해당 인터페이스가 리턴됩니다. 
//!	지정 인터페이스 ID 로 참조되는 (원본) 객체는 참조 카운트가 증가되므로 
//! 인터페이스 사용 후 안전한 메모리 해제를 위해 Release 를 호출해야 합니다.
//! 
//! <Yena> Yena 전용 GUID 를 통해 해당 ynInterface 를 획득하며 
//! 기본적인 기능은 Windows / COM 과 동일합니다.
//! 
//! \param	riid		Yena 인터페이스 ID
//! \param	ppvObject	(생성된) Yena 인터페이스 포인터.
//! \return		성공시 OK, 실패시 FALSE
//
int B3YenaDevice9::QueryInterface(YN_IID riid, _out_ void** ppvObject)
{
	//void* pInterface = nullptr;

	//지정 ID 의 유효성 검사..(간략)
	//if(riid == IID_IYena)
	//if (IsEqualGUID((GUID&)riid, (GUID&)IID_IYena))	//GUID 동일 검사
	//if (IsEqualIID((IID&)riid, (IID&)IID_IYena))		//IID 동일 검사 <상동>
	if (ynIsEqualIID(riid, IID_IYenaDevice9))			//IID 동일 검사 <상동>
	{
		AddRef();										//참조 카운트 증가.
		*ppvObject = dynamic_cast<IYenaDevice9*>(this);	//지정 인터페이스 리턴.
	}

	return YN_OK;
}





////////////////////////////////////////////////////////////////////////////////
//


/*  
*   B3YenaVertexBuffer9 : 인터페이스 구현
* 
*		-> B3YenaVertexBuffer9.cpp 로 이동
* 
*/




///////////////////////////////////////////////////////////////////////////////
//
// B3YenaGraphicsEngine 의 정의와 구별용 정의 해제
// B3YenaDevice9 전용
// 
///////////////////////////////////////////////////////////////////////////////
//#undef m_pGraphics	this

#undef m_pVB 
#undef m_FVF	
#undef m_Stride 
#undef m_TopologyVtxSize
//#undef m_PrimVtxCnt
#undef m_mWorld 
#undef m_mView 
#undef m_mProj 
#undef m_mTFM 
#undef m_RState
#undef m_PresentParam 





/**************** end of "Yena.h" *********************************************/

