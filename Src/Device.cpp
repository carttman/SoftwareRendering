//! 
//! \file	Device.cpp
//! \brief	예제 렌더링 장치 운용 소스. 
//!	\version Yena SW Renderer v1.5.x 
//! 
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net
//! \date   2012.01.17. v0.9   DX9.x, VS08
//! \date   2012.02.01. v1.0   DX9.x, VS10
//! \date   2013.04.10. v1.2.0 DX9.x, VS08
//! \date   2015.10.10. v1.2.1 DX9.x, VS15
//! \date   2020.08.22. Updated. DX12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//! \date   2025.05.01. v1.5.x, v1.9.0   DX12.X, (VS22)
//! \ingroup Demo
//
#include "windows.h"
#include "tchar.h"
#include "mmsystem.h"
#include "stdio.h"

#include "Device.h"



/////////////////////////////////////////////////////////////////////////////
//
// 디바이스 관련 전역 데이터들. 
//

// Yena SWR 디바이스 (D3D9 인터페이스 대응)
LPYENA			g_pYena = NULL;			
LPYENADEVICE9	g_pDevice = NULL;		


//백버퍼 접근...
#define  hRT   g_pDevice->GetRT()

 
// 화면 모드 설정 전역 변수.  
//
YENADISPLAYMODE g_Mode = { 800, 600 };		//4:3 
//YENADISPLAYMODE g_Mode = { 960, 600 };	//16:10


// 시스템 폰트.
HFONT		g_hSysFont	 = NULL;
COLORREF	g_SysFnColor = RGB(0, 255, 0);	//기본 시스템 폰트 색상.



/////////////////////////////////////////////////////////////////////////////
//
// 기타 상태 변수들. 
//
BOOL g_ShowFrame = FALSE;





/////////////////////////////////////////////////////////////////////////////
//
//! 예나 SWR 엔진 구성 : 초기 1회 호출
//! 
//               
int YenaSetup(HWND hwnd)
{
	// 예나 생성.(IDirect3D 대응)
	//
	g_pYena = YenaCreate9(YENA_VERSION);
	if(YN_INVALIED(g_pYena))
	{
		//Error...
		return YN_FAIL; 
	}

 
	// Yena SWR 장치 정보 구성 
	// <주> D3D9 운용 필요 데이터 중 Yena 에 필요한 최소한의 정보만 구성)
	YENAPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.Width  = g_Mode.Width;		//해상도 결정.(백버퍼 크기)
	pp.Height = g_Mode.Height;
	pp.BackBuffercnt = 1;			//백버퍼 개수. (현재는 1개만 가능)
	pp.Windowed = TRUE;				//풀스크린, 창모드 선택.(현재는 창모드만 가능)

	
	// Yena SWR 장치 생성 : ID3DDevice9 대응	
	//
	g_pYena->CreateDevice(g_hWnd,								//디바이스의 렌더링 목표 윈도우 핸들.
						&pp,									//디바이스 화면 구성 정보.
						YENACREATE_SOFTWARE_VERTEXPROCESSING,   //정점연산 방법 결정 (현재는 SW 만 가능) <DX> HW <Yena> SW 설정. 
						&g_pDevice								//성공시 리턴받을 디바이스 개체 포인터.
						);							
	if(YN_INVALIED(g_pDevice))
	{
		//Error..
		return YN_FAIL;
	}

 
	// 디바이스 기본 상태 옵션 결정.
	// ...
	// ...


	// 시스템 폰트 생성.
	//
	g_hSysFont = CreateFont(
							12, 6,
							0, 0, 1, 0, 0, 0,   
							DEFAULT_CHARSET,	//HANGUL_CHARSET  
							OUT_DEFAULT_PRECIS,		 
							CLIP_DEFAULT_PRECIS,  
							DEFAULT_QUALITY,
							FF_DONTCARE, 
							_T("굴림") 
							);
	if(g_hSysFont==NULL)
	{
		// error..! 
		return YN_FAIL;	 
	}
 	SelectObject(hRT, g_hSysFont);


	return YN_OK;

}




/////////////////////////////////////////////////////////////////////////////
//
//! 예나 SWR 엔진 제거 : 종료시 1회 호출
//
void YenaRelease()
{ 	
	// 폰트 제거.
	DeleteObject(g_hSysFont);


	// 렌더링 객체 제거. 
	SafeRelease(g_pDevice);		//디바이스 제거 
	SafeRelease(g_pYena);		//예나 제거 

} 



 
/////////////////////////////////////////////////////////////////////////////
//
//! 엔진 타이머 : 초당 프레임(fps) 출력
//! 
//! \param	x, y  출력할 화면좌표(2D)
//! \return	없음.
//
void PutFPS(int x, int y)
{
	static UINT  frm = 0;
	static float fps = 0.0f;
	++frm;
	static ULONGLONG oldtime = GetTickCount64();
	ULONGLONG		 nowtime = GetTickCount64();

	UINT time = (UINT)(nowtime - oldtime);
	if (time >= 1000)
	{
		fps = (float)(frm * 1000) / (float)time;
		frm = 0;
		oldtime = nowtime;
	}

	DrawText(x, y, RGB(0, 255, 0), _T("FPS=%.1f/%d"), fps, time);
}





/////////////////////////////////////////////////////////////////////////////
//
//! 엔진 타이머 : 프레임 시간 획득.
//
float GetEngineTime()
{	 
	static ULONGLONG oldtime = GetTickCount64();
	ULONGLONG nowtime = GetTickCount64();
	float dTime = (nowtime - oldtime) * 0.001f;
	oldtime = nowtime;

 	return dTime; 
}




/////////////////////////////////////////////////////////////////////////////
//
//! 문자열 출력 (GDI)
//!
//! \param	x, y	출력 화면 좌표.
//! \param	col		출력 색상
//! \param	msg		출력 문자열 (형식화 문자열 지원)
//! \return	없음.
//
void DrawText(int x, int y, COLORREF col, TCHAR* msg, ...)
{
	TCHAR buff[2048]=_T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);
	RECT rc = { x, y, (LONG)(x + g_Mode.Width), (LONG)(y + g_Mode.Height) };

 	SetTextColor(hRT, col);
	DrawText(hRT, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(hRT, g_SysFnColor);
	
	//return res;
}





/*********************** end of file "Device.cpp" *********************/
