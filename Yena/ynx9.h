//! 
//! \file	ynx9.h
//! \brief	\emoji :star:
//!			Yena SWR 인터페이스 헤더 : DX9 (d3d9.h) 대응
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
//! \date   2025.05.01. v1.5.x, v1.9.0   DX12.X, (VS22)
//! \addtogroup	Yena-Interfaces
//! \{
//! 
#pragma once


////////////////////////////////////////////////////////////////////////////////
//  
//! Yena 인터페이스/클래스 전방 선언 (Yena 지원, 불요 기능 생략)
//
yninterface IYena;				//IDirect3D9 대응
yninterface IYenaDevice9;		//IDirect3DDevice9 대응

yninterface IYenaDevice9;
typedef IYenaDevice9  YENADEVICE9;			//DX9 대응.
typedef IYenaDevice9* LPYENADEVICE9;		//DX9 대응.
typedef IYenaDevice9* LPDEVICE;				//DX9 대응.

#define YENA_VERSION  9		//DX9.0




////////////////////////////////////////////////////////////////////////////////
//  
// Yena 자료형 선언 
// 
// <주> (이전 예제에서 정의된 자료형 이동...)
//
#include "ynx9types.h"		//!< Yena SWR 자료형 정의 : D3D9 대응 (d3d9types.h) 
//#include "ynx9cpas.h"		//!< Yena SWR 자료형 정의 : D3D9 대응 (d3d9caps.h)






/////////////////////////////////////////////////////////////////////////////// 
//
//! \interface	IYenaResource9
//! \brief		렌더링 자원 기반 인터페이스 
//! 			: IDirect3DResource9 대응 
//!
//! \copydetails B3YenaResource9
//! \remarks	Yena 는 DX 인터페이스와 메소드의 시그니쳐(Signature)를 가능한 동일하게 준수합니다.  
//!				구현 클래스 B3YenaResource9 을 참고하십시오.
// 
////////////////////////////////////////////////////////////////////////////////
//
yninterface IYenaResource9 : public ynIUnknown
{
	/* DX 메소드 구현 생략...
	* 
	// IUnknown methods 
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObj) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	// IDirect3DResource9 methods 
	STDMETHOD(GetDevice)(THIS_ IDirect3DDevice9 * *ppDevice) PURE;
	STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags) PURE;
	STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD * pSizeOfData) PURE;
	STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid) PURE;
	STDMETHOD_(DWORD, SetPriority)(THIS_ DWORD PriorityNew) PURE;
	STDMETHOD_(DWORD, GetPriority)(THIS) PURE;
	STDMETHOD_(void, PreLoad)(THIS) PURE;
	STDMETHOD_(D3DRESOURCETYPE, GetType)(THIS) PURE;
	*/

	//자원 버퍼 획득 : Yena 전용
	virtual void*	GetBuffer		(void) pure;
	virtual int		GetPrivateData	(_out_ void** ppBuffer, _out_ UINT* pSizeData) pure;
};




/////////////////////////////////////////////////////////////////////////////// 
//
//! \interface	IYenaVertexBuffer9
//! \brief		정점 버퍼 운용 인터페이스
//!				: IDirect3DVertexBuffer9 대응  
//! 
//! \copydetails B3YenaVertexBuffer9
//! \see		B3YenaVertexBuffer9
//! \see		[IDirect3DVertexBuffer9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nn-d3d9-idirect3dvertexbuffer9)
//! \remarks	Yena 는 DX 인터페이스와 메소드의 시그니쳐(Signature)를 가능한 동일하게 준수합니다.  
//!				구현 클래스 B3YenaVertexBuffer9 을 참고하십시오.
// 
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYenaVertexBuffer9 : public IYenaResource9		
{
   
	virtual int		Lock			(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags) pure;
	virtual int		Unlock			(void) pure;
	virtual int		GetDesc			(_out_ B3YVERTEXBUFFER_DESC* pDesc) pure;
 
	//자원 버퍼 획득 : Yena 전용 
	virtual void*	GetBuffer		(void) pure;
	virtual UINT	GetVertexCount	(void) pure;
	virtual int		GetPrivateData	(_out_ void** ppBuffer, _out_ UINT* pSizeData) pure;

};

typedef IYenaVertexBuffer9* LPYENAVERTEXBUFFER9;
typedef IYenaVertexBuffer9* LPVBUFFER;
typedef IYenaVertexBuffer9* LPVB;
//typedef IYenaVertexBuffer9* LPB3YENAVERTEXBUFFER9;		//(구형 호환성 유지용)






/////////////////////////////////////////////////////////////////////////////// 
//
//! \interface	IYena 
//! \brief		Yena SWR 3D 운용 기반, 인터페이스
//!				: IDirect3D9 대응 
//! 
//! <DX>	DX9 3D 렌더링 장치(Device)를 생성하려면 우선 Direct3D9 객체가 필요합니다.  
//!			이 Direct3D9 는 시스템이 보유한 물리적 장치(GPU, Adaptor) 의 지원 성능(Features)을 점검하고 렌더링 장치(Direct3DDevice9) 을 생성합니다.  
//!			Direct3D9 으로 생성되는 모든 장치들은 물리적 자원을 공유하지만, 다중 렌더링 장치 운용시에는 상당한 성능 저하의 요인이 될 수 있습니다.  
//! <Yena>	IYena 와 IYenaDevice9 는 각각 IDirect3D 와 IDirect3DDevice9 을 대응하며 인터페이스를 구현합니다.  
//! 
//! \copydetails yxx_interface_table
//! 
//! \see	IYena, B3Yena
//! \see	IYenaDevice9, B3YenaDevice9
//! \see	[IDirect3D9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nn-d3d9-idirect3d9)
//! \see	[IDirect3DDevice9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9helper/nn-d3d9helper-idirect3ddevice9)
//! \see	[D3D9:Creating Device](https://learn.microsoft.com/en-us/windows/win32/direct3d9/creating-a-device)
//! \see	[IDirect3D9::CreateDevice](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3d9-createdevice)
//! \remarks Yena 는 DX 인터페이스와 메소드의 시그니쳐(Signature)를 가능한 동일하게 준수합니다.  
//!			 구현 클래스 B3Yena 을 참고하십시오.
//
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYena : public ynIUnknown			
{
	//-----------------------------------------------------------------
	// 인터페이스 선언
	// DX 와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다. 
	//----------------------------------------------------------------
	//! Yena SWR 렌더링 디바이스 개체 생성 : D3D9 대응
	virtual int CreateDevice(HWND hwnd,						//[in] 디바이스의 렌더링 목표 윈도우 핸들.
							 YENAPRESENT_PARAMETERS* pp,	//[in] 디바이스 화면 구성 정보.	
							 DWORD vp,						//[in] 정점연산 방법 결정 (현재는 SW 만 가능)
							 _out_ LPYENADEVICE9* pDev		//[out] 성공시 리턴받을 디바이스 개체 포인터.
							 ) pure;

};

typedef IYena*	LPYENA;
typedef IYena	IYena9;		//DX9 대응 이름 재정의.
typedef IYena9*	LPYENA9;	//DX9 대응 이름 재정의.


//! Yena 개체 생성 함수 (D3D9 대응)
//! 
IYena* YenaCreate9(DWORD ver);







/////////////////////////////////////////////////////////////////////////////// 
//
//! \interface	IYenaDevice9 
//! \bstar		Yena 렌더링 디바이스 운용 인터페이스 
//!				: IDirect3DDevice9 대응 
//!                 
//! DX 는 IDirect3DDevice9 을 통해 3차원 렌더링 장치(Device) 를 접근하며 다음과 같은 작업을 처리합니다.   
//!  - 렌더링 장치(GPU) 운용   
//!  - 그래픽스 파이프라인의 운용 (HWVP 또는 SWVP)   
//!  - 렌더링 자원 생성 및 관리 : VB, IB, RT, DSB 등.  
//!  - 렌더링 상태 조절  
//!  - 스왑체인 운용 : 다중 버퍼링, 렌더타겟 개수, 해상도, 색상 포멧 등.    
//! 
//! Yena 는 IYenaDevice9 인터페이스로 최대한 동일 기능을 지원합니다. 학과 진행에 맞추어 많은 기능들이 추가될 것입니다.  
//! 
//! <DX> DX9 3D 렌더링 장치(Device)를 생성하려면 우선 Direct3D9 객체가 필요합니다.   
//!		 이 Direct3D9 는 시스템이 보유한 물리적 장치(GPU, Adaptor) 의 지원 성능(Features)을 점검하고 렌더링 장치(Direct3DDevice9) 을 생성합니다.   
//!		 Direct3D9 으로 생성되는 모든 장치들은 물리적 자원을 공유합니다. 이는 다중 렌더링 장치 운용시 상당한 성능저하의 요인이 됩니다.   
//! <Yena> IYena 와 IYenaDevice9 은 각각 IDirect3D 와 IDirect3DDevice9 을 대응하며 가능한 동일한 인터페이스로 구성됩니다.
//! 
//! \see	IYena, B3Yena
//! \see	IYenaDevice9, B3YenaDevice9
//! \see	[IDirect3D9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nn-d3d9-idirect3d9)
//! \see	[IDirect3DDevice9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9helper/nn-d3d9helper-idirect3ddevice9)
//! \see	[D3D9:Creating Device](https://learn.microsoft.com/en-us/windows/win32/direct3d9/creating-a-device)
//! \see	[IDirect3D9::CreateDevice](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3d9-createdevice)
//! \remarks Yena 는 DX 인터페이스와 메소드의 시그니쳐(Signature)를 가능한 동일하게 준수합니다.  
//!			 구현 클래스 B3YenaDevice9 을 참고하십시오.
//
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYenaDevice9 : public ynIUnknown		
{
public:
		
	//-----------------------------------------------------------------
	// 인터페이스 선언
	// DX 와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다. 
	//-----------------------------------------------------------------
	//--------------------------------
	// 스왑체인 및 렌더타겟 운용 메소드들
	//--------------------------------
	virtual int BeginScene	() pure;
	virtual int EndScene	() pure;
	virtual int Clear		(COLORREF col) pure;
	virtual int Present		() pure;

	//--------------------------------
	// 기하 버퍼 및 렌더링 메소드들 
	//--------------------------------
	virtual int CreateVertexBuffer	(UINT Length, DWORD Usage, DWORD FVF, B3YENAPOOL Pool, _out_ IYenaVertexBuffer9** ppVertexBuffer, _in_opt_ HANDLE* pSharedHandle) pure;
	virtual int SetStreamSource		(UINT StreamNumber, IYenaVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) pure;
	virtual int SetFVF				(DWORD FVF) pure;
	virtual int DrawPrimitive		(B3YPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) pure;
	
	//--------------------------------
	// 렌더링 상태 조절 메소드들.
	//--------------------------------
	virtual int SetRenderState	(B3YRENDERSTATETYPE	State, DWORD Value)   pure;
	virtual int GetRenderState	(B3YRENDERSTATETYPE State, DWORD* pValue) pure;


	//--------------------------------
	// 멤버데이터 접근자 Accessors : Yena 전용
	//--------------------------------
	virtual HDC		 GetRT		() pure;
	virtual COLORREF GetBkColor	() pure; 

};

typedef IYenaDevice9* LPYENADEVICE9;
//typedef B3YenaDevice9* LPB3YENADEVICE9;	//DX9 대응.






/////////////////////////////////////////////////////////////////////////////// 
//
// 기타 디바이스 대응 클래스의 확장 선언 예시는 아래와 같습니다.
// 확장성을 고려한 클래스 설계 베이스 입니다만, 이번 프로젝트에서는 
// 제작/사용하지 않습니다.  "이런식으로 쓸 수 있겠군.." 정도로 참고해 주십시오.
//
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
//! \interface	IYenaDevice10 
//! \noimp		IYenaDevice10 : DX10 대응 렌더링 "디바이스" 인터페이스 <생략>  
//!              : ID3D10Device 대응 
//! 
//! \note \noimp
//
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYenaDevice10 : public ynIUnknown
{
	YN_NOT_IMPLEMENTED
	YN_미구현_미사용

public:

	//...DX10 대응 Device ...
};



/////////////////////////////////////////////////////////////////////////////// 
//
//! \interface	IYenaDevice11 
//! \noimp		IYenaDevice11
//!				 : DX11 대응 렌더링 "디바이스" 인터페이스 <생략>  
//!              : ID3D11Device 대응 
//! 
//! \note \noimp
//
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYenaDevice11 : public ynIUnknown
{
	YN_NOT_IMPLEMENTED
	YN_미구현_미사용

public:

	//...DX11 대응 Device ...
};




/////////////////////////////////////////////////////////////////////////////// 
//
//! \interface	IYenaDevice12 
//! \noimp		IYenaDevice12 
//!				 : DX12 대응 렌더링 "디바이스" 인터페이스 <생략>  
//!              : ID3D12Device 대응 
//! 
//! \note \noimp
//
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYenaDevice12 : public ynIUnknown
{
	YN_NOT_IMPLEMENTED
	YN_미구현_미사용

public:

	//...DX12 대응 Device ...
};





/////////////////////////////////////////////////////////////////////////////// 
//
//! \interface	IYenaDeviceGL 
//! \noimp		IYenaDeviceGL 
//!				 : OpenGL 대응 렌더링 "디바이스" 인터페이스 <생략>  
//! 
//! \note \noimp
//
/////////////////////////////////////////////////////////////////////////////// 
//
yninterface IYenaDeviceGL : public ynIUnknown
{
	YN_NOT_IMPLEMENTED
	YN_미구현_미사용

public:

	//...OpenGL 대응 Device ...
};



/////////////////////////////////////////////////////////////////////////////// 
//
//! \interface	IYenaDeviceVK 
//! \noimp		IYenaDeviceVK
//!				 : Vulkan 대응 렌더링 "디바이스" 인터페이스 
//!				 : VkPhysicalDevice 대응.
//! 
//! \note \noimp
//! \see	[VK Device Creation](https://docs.vulkan.org/spec/latest/chapters/devsandqueues.html#devsandqueues-device-creation)
//
/////////////////////////////////////////////////////////////////////////////// 
// 
yninterface IYenaDeviceVK : public ynIUnknown
{
	YN_NOT_IMPLEMENTED
	YN_미구현_미사용

public:

	//...Vulkan 대응 Device ...
};





//!	\}	end of group IYena-Interfaces
/**************** end of "ynx9.h" ***********************************/




