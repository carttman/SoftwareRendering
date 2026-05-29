//! 
//! \file	ynB3VertexBuffer.h
//! \brief	정점 버퍼 운용 클래스 헤더 
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
//
#pragma once
 


/////////////////////////////////////////////////////////////////////////////// 
//
// 시스템 / 플랫폼 헤더
//
#include "windows.h"
#include "tchar.h"
#include "math.h"
#include "stdio.h"

#include "string"
#include "vector"
#include "algorithm"
using namespace std;

/*
/////////////////////////////////////////////////////////////////////////////// 
//
// Yena SW Renderer 헤더
//
// Yena SWR 장치/공통 인터페이스
#include "ynDefine.h"		//!< Yena SWR 기본/공통 상수 정의
#include "ynError.h"		//!< Yena 에러처리 헤더.
#include "ynGUID.h"			//!< Yena SWR COM / GUID 헤더.
#include "ynUnknownbase.h"	//!< Yena SWR COM 헤더, 기반 인터페이스 헤더
#include "ynx9types.h"		//!< Yena SWR 자료형 정의 : D3D9 대응
#include "ynx9.h"			//!< Yena SWR 장치 헤더 : D3D9 대응
//#include "ynGI.h"			//!< Yena GI : DXGI 대응 운용
//#include "ynCommon.h"		//!< Yena SWR 공통 헤더 : DX11 대응
//#include "ynx11.h"		//!< Yena SWR 장치 헤더 : D3D11 대응

// Yena 확장 기능 클래스들
#include "ynMath.h"			// 수학 라이브러리.
//#include "ynXSurface.h"	// 서피스 관리.
//#include "ynTexture.h"	// 텍스쳐 관리.
//#include "ynDebug.h"		// 디버깅 관련.
//#include "ynLight.h"		// 조명 관리 
//#include "ynShader.h"		// 쉐이더 관련.

#include "ynGraphics.h"		//Yena SWR 그래픽스 엔진


// Yena 서비스 클래스들
//#include "ynGrid.h"		//그리드.
//#include "ynAxis.h"		//방향축.

using namespace YENA;
*/



/////////////////////////////////////////////////////////////////////////////// 
//
//! \class	B3YenaVertexBuffer9
//! \brief	정점 버퍼 구현 클래스
//!			: IDirect3DVertexBuffer9 대응  
//! 
//! \par 정점버퍼
//! 정점버퍼는 일련의 기하(Geometry)를 구성하는 정점 데이터가 저장되는 버퍼입니다.  
//! 이 데이터들은 정점 단위규격(Vertex Format 또는 Vertex Declaration) 으로 배열화 저장되며 읽기 전용으로 GPU 에 전달됩니다.  
//! 필요시 동적 정점 버퍼 (Dynamic Vertex Buffer) 를 구성하는 것도 가능합니다.
//! 
//! \par 정점버퍼 렌더링 
//! 렌더링 장치는 렌더링 파이프라인에 설정된 정점버퍼를 읽어오며, 렌더링 기하구성(Primitive Topology) 
//! 주문에 따라 데이터를 처리합니다. (Vertex Streaming)  
//! 렌더링 파이프라인은 하나 이상의 정점버퍼를 연결(Binding) 할 수 있으며 (DX9, max.16)  
//! 정점 조립 스테이지(Vertex Assembly 또는 Input Assembly Stage) 에서 재구성되어 사용됩니다.
//! 
//! <DX9> IDirect3DVertexBuffer9 으로 정점버퍼를 운용합니다.  
//! <Yena> IYenaVertexBuffer9 으로 DX 인터페이스를 대응합니다.  
//! 
//! \note	장치(Device)는 파이프라인에 설정된 "현재" 정점 버퍼를 사용하므로 렌더링 명령
//!			(Draw / DrawPrimitive) 전에 장치에 연결(Binding) 되어야 합니다.  
//! 
//! \see	B3YenaVertexBuffer9
//! \see	IYenaVertexBuffer9
//! \see	[IDirect3DVertexBuffer9](https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nn-d3d9-idirect3dvertexbuffer9)
//! \remarks Yena 는 DX 인터페이스와 메소드의 시그니쳐(Signature)를 가능한 동일하게 준수합니다.
//! \ingroup 	Yena-Class
// 
/////////////////////////////////////////////////////////////////////////////// 
//
class B3YenaVertexBuffer9 : public IYenaVertexBuffer9
{
	friend class B3YenaDevice9;
	friend class B3YenaGraphicsEngine9;


protected:
	//레퍼런스 참조 카운트 
	ULONG m_ref = 0;

protected:
	void* m_pVBuffer;				//!< 정점 버퍼 : 실제 데이터가 저장됨.
	DWORD m_SizeInByte;				//!< 정점 버퍼 크기 (바이트)
	DWORD m_FVF;					//!< 정점 버퍼 규격 조합 플래그.
	UINT  m_Stride;					//!< 정점 버퍼 안의 1마디(정점 구조 하나) 의 크기.
	UINT  m_VtxCnt;					//!< 정점 개수.
	BOOL  m_bLocked;				//!< 버퍼 잠금 상태 (On/Off)


	B3YVERTEXBUFFER_DESC m_Desc;	//!< 정점 버퍼 정보 

protected:
	//(내부) 버퍼 운용 함수들 
	int		_Create			(B3YVERTEXBUFFER_DESC desc);
	int		_Create			(UINT Length, DWORD FVF, B3YPOOL Pool);		//구형 호환.

	void*	_GetVBuffer		() { return m_pVBuffer;		}
	DWORD	_GetSizeInByte	() { return m_SizeInByte;	}
	DWORD	_GetFVF			() { return m_FVF;			}
	DWORD	_GetStride		() { return m_Stride;		}
	UINT	_GetVtxCnt		(); 	

	//(내부) 정점 데이터 접근 메소드들
	B3YVECTOR2	_GetPos2	(int index);	//직접접근용.
	B3YVECTOR3	_GetPos3	(int index);
	DWORD		_GetDiffuse	(int index);
	//B3YXVECTOR2 GetPos2	();				//간접접근용.
	//B3YXVECTOR3 GetPos3	();
	//B3YXCOLOR	  GetDiffuse(); 


public:
	B3YenaVertexBuffer9			(void);
	virtual ~B3YenaVertexBuffer9(void);


	//-----------------------------------------------------------------
	// 인터페이스 재정의
	// DX 와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다. 
	//-----------------------------------------------------------------
	virtual int   Lock				(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
	virtual int   Unlock			(void);
	virtual int   GetDesc			(_out_ B3YVERTEXBUFFER_DESC* pDesc);

	//자원 버퍼 획득 : Yena 전용
	virtual void* GetBuffer			(void);
	virtual UINT  GetVertexCount	(void);	
	virtual int	  GetPrivateData	(_out_ void** ppBuffer, _out_ UINT* pSizeData);

	//--------------------------------
	// 참조 카운트 메소드 재정의 
	//--------------------------------
	virtual ULONG AddRef			(void);
	virtual ULONG Release			(void);
	virtual int   QueryInterface	(YN_IID riid, _out_ void** ppvObject);


};

typedef B3YenaVertexBuffer9* LPB3YENAVERTEXBUFFER9;
//typedef LPB3YENAVERTEXBUFFER9 LPVBUFFER;






/**************** end of "ynB3VertexBuffer.h" ***********************************/
