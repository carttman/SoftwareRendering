//! 
//! \file	Render.cpp
//! \brief	기본 렌더링 프레임워크 소스
//!			DirectX 기준 버전 : DX12
//! 
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com 
//! \date   2003.11.04. Updated. DX9.x 
//! \date	2005.09.23. Updated (.Net)
//! \date   2009.12.01. Updated.
//! \date   2010.12.01. Updated. DX11, Jun.2010
//! \date   2016.12.27. Updated. DX11/12, Window SDK 8.1 / Window 10 SDK 10.0.18362
//! \date   2018.12.30. Updated. DX12.x, Windows 10 SDK 10.0.18362
//! \date   2020.08.22. Updated. DX12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX12.x, Windows 10 SDK 10.0.22621 (VS22)
//!
//
#pragma warning(disable:4996)
#include "Windows.h"
#include "stdio.h"
#include "tchar.h"
#include "Render.h"

#include <cmath>

TCHAR* g_WindowName = _T("Yena::SW Renderer Tutorial 00 BasicFrameworks");



/////////////////////////////////////////////////////////////////////////////
//
//  전역 데이터들.
//  extern으로 헤더에 이름표 달기
extern HWND g_hWnd; 

//출발 좌표, 끝 좌표
POINT g_Sp, g_Ep;

enum 
{
	PT_NONE_ = 0x00, // 점 없음
	PT_V0_ = 0x01, // 첫 번째 점 설정됨
	PT_V1_ = 0x02, // 두 번째 점 설정됨
	PT_V2_ = 0x04, // 세 번째 점 설정됨

	PT_MAX_ = 3,
	PT_1_ = PT_V0_,
	PT_2_ = (PT_V0_ | PT_V1_),
	PT_3_ = (PT_V0_ | PT_V1_ | PT_V2_),
	PT_COMPLETED_ = (PT_V0_ | PT_V1_ | PT_V2_) // 모든 점 설정 됨
};
// =====================================================

POINT my_g_Sp;
POINT my_g_Ep;

enum
{
	MYPT_NONE = 0,
	MYPT_V0,
	MYPT_V1,
	MYPT_ALL
	
};

int my_ptState = MYPT_NONE;

// ========================================================

// 입력된 삼각형 정점
POINT g_Vtx[PT_MAX_];
POINT g_VtxClear = {0, 0};
POINT g_VtxTemp = {0, 0};

DWORD g_PtCheck = PT_NONE_; // 점 몇개 설정됐는지 체크


//출력화면 DC (렌더타겟)
//HDC g_hDC;

// 렌더타겟 백버퍼
HBITMAP g_hBmpRT = nullptr; // 비트맵 리소스를 가리키는 핸들 
HDC g_hSurfaceRT = nullptr;
COLORREF g_BKColor = RGB(0, 0, 255);
#define g_hRT g_hSurfaceRT


//라인 그리기 팬
HPEN g_hPenDash;
HPEN g_hPenWhite;
HPEN g_hPenGreen;

//정점정보 보기
BOOL g_bShowVtxInfo = TRUE;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  
// 렌더링 데이터 불러오기 및 해제.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//! 게임 데이터 및 렌더링 자원을 불러옵니다.
//! 
//! \return 성공시 OK, 실패시 FAIL.
//
int DataLoading()
{ 
	// 데이터 로딩/생성 코드는 여기에...
	// 초기화

	//라인용 팬 생성
	//g_hPenDash = CreatePen(PS_DASH, 1, RGB(64, 64, 64));   
	g_hPenDash = CreatePen(PS_DASH, 1, RGB(255, 255, 255));
	g_hPenWhite = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	g_hPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
//
//! 게임 데이터 및 렌더링 자원을 해제합니다.
//!
//! \param	없음.
//! \return	성공시 OK, 실패시 FAIL.
// 
void DataRelease()
{
	// 데이터 해제 코드는 여기에..
	// ... 

	// 라인용 팬 제거
	DeleteObject(g_hPenDash);
	DeleteObject(g_hPenWhite);
	DeleteObject(g_hPenGreen);
}







//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//
// Rendering Object.
//
////////////////////////////////////////////////////////////////////////////////

//...






//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//
// Engine / System Functions
//
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
//! 타이머의 초당 프레임률(fps)를 출력합니다.
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

	DrawText(x, y, RGB(255, 255, 255), _T("FPS=%.1f/%d      "), fps, time);
}


 

/////////////////////////////////////////////////////////////////////////////
//
//! 문자열을 출력합니다. (GDI)
//!
//! \param	x, y	출력 화면 좌표.
//! \param	msg		출력 문자열 (형식화 문자열 지원)
//! \return	없음.
//
void DrawText(int x, int y, COLORREF col, TCHAR* msg, ...)
{
	TCHAR buff[128] = _T("");
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);

	RECT rc = { x, y, x + 800, y + 600 };

	//HDC hdc = GetDC(g_hWnd);
	//col = GetSysColor(COLOR_BACKGROUND+1);
	//SetBkColor(hdc, RGB(128, 128, 128));
	//SetBkMode(hdc, TRANSPARENT);
	//SetBkColor(g_hRT, col);
	SetTextColor(g_hRT, col);
	//TextOut(g_hRT, x, y, buff, (int)_tcslen(buff));
	DrawText(g_hRT, buff, (int)_tcslen(buff), &rc, DT_WORDBREAK);
	SetTextColor(g_hRT, RGB(255, 255, 255));
	//ReleaseDC(g_hWnd, hdc);
}

void BeginScene() // 렌더링에 필요한 초기화
{
	//g_hDC = GetDC(g_hWnd); -> 화면 DC 사용하지 않는다. 렌더 타겟을 사용한다
	SetBkMode(g_hRT, TRANSPARENT); // 투명으로 
}

void Clear(COLORREF col) // 렌더타겟 지운다
{
	HBRUSH hBrush = CreateSolidBrush(col); // 단색으로 채워진 브러시 오브젝트 생성
	RECT rc;
	GetClientRect(g_hWnd, &rc); // 윈도우의 그림이 그려지는 영역
	FillRect(g_hRT, &rc, hBrush); // 브러시로 영역 색칠
	DeleteObject(hBrush); // 브러시 삭제
}

//void EndScene() 
//{
//	//ReleaseDC(g_hWnd, g_hDC); // getDC로 얻은 hdc를 시스템에 반환 -> 화면 dc 사용 안함. 렌더타겟을 사용
//}

void Present()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	HDC hdc = GetDC(g_hWnd);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, g_hRT, 0, 0, SRCCOPY);
	ReleaseDC(g_hWnd, hdc);
}

void LineClear() // 모든 정점 초기화
{
	for (int i=0; i<PT_MAX_; i++)
	{
		g_Vtx[i] = g_VtxClear;
	}
	g_VtxTemp = g_VtxClear;

	g_PtCheck = PT_NONE_;
}

void LineDraw()
{
	switch (my_ptState)
	{
	case MYPT_NONE:
		break;
	case MYPT_V0:
		SetPixel(g_hRT, my_g_Sp.x, my_g_Sp.y, RGB(255, 0, 0));
		break;
	case MYPT_V1:
		SetPixel(g_hRT, my_g_Sp.x, my_g_Sp.y, RGB(255, 0, 0));
		SetPixel(g_hRT, my_g_Ep.x, my_g_Ep.y, RGB(255, 0, 0));
		break;
	case MYPT_ALL:
		SetPixel(g_hRT, my_g_Sp.x, my_g_Sp.y, RGB(255, 0, 0));
		SetPixel(g_hRT, my_g_Ep.x, my_g_Ep.y, RGB(255, 0, 0));
		LineDraw(my_g_Sp, my_g_Ep);
		break;
	}

	//=========================================================================
	//switch (g_PtCheck)
	//{
	//case PT_NONE_:								//그리기 없음.
	//	break;

	//case PT_1_:
	//	//CrossDraw(g_Vtx[0], _T("v0"));			//첫번째 표시.

	//	//LineDashDraw(g_Vtx[0], g_VtxTemp);		//임시라인 표시..(쇄선)		
	//	break;

	//case PT_2_:
	//	//CrossDraw(g_Vtx[0], _T("v0"));			//첫번째 표시.
	//	//CrossDraw(g_Vtx[1], _T("v1"));			//두번째 표시.

	//	//LineDashDraw(g_Vtx[0], g_VtxTemp);		//임시라인 표시..(쇄선)
	//	//LineDashDraw(g_Vtx[1], g_VtxTemp);

	//	LineDraw(g_Vtx[0], g_Vtx[1]);			//라인 그리기 : V0 -> V1

	//	break;

	//case PT_COMPLETED_:							//완료. 모두 그리기.

	//	if (g_bShowVtxInfo)						//정점 정보 출력..
	//	{
	//		//for (int i = 0; i < PT_MAX_; i++)
	//			//CrossDraw(g_Vtx[i], _T("v%d"), i);
	//	}

	//	//FaceDraw();  							//삼각형 그리기..★

	//	break;
	//}
}

void LineDraw(const POINT sp, const POINT ep)
{
	int dx = ep.x - sp.x;
	int dy = ep.y - sp.y;

	float length = sqrt(dx * dx + dy * dy);

	float unitX = dx / length; // 단위벡터
	float unitY = dy / length;

	float curX = (float)sp.x;
	float curY = (float)sp.y;

	for (int i=0; i<= (int)length; i++) // 거리만큼 단위벡터를 더해가면서 점을 찍는다
	{
		SetPixel(g_hRT, (int)(curX), (int)(curY + 0.5f), RGB(255, 0, 0));
		curX += unitX;
		curY += unitY;
	}

	/*MoveToEx(g_hRT, sp.x, sp.y, nullptr);
	LineTo(g_hRT, ep.x, ep.y);*/



}

// 마우스 위치에 십자선 찍는다
void CrossDraw(POINT pt, TCHAR* name, ...)
{
	HPEN hOldPen = (HPEN)SelectObject(g_hRT, g_hPenGreen);

	MoveToEx(g_hRT, pt.x - 5, pt.y, NULL);
	LineTo(g_hRT, pt.x + 5, pt.y);

	MoveToEx(g_hRT, pt.x, pt.y - 5, NULL);
	LineTo(g_hRT, pt.x, pt.y + 5);

	va_list  vl;
	TCHAR buff[80] = _T("");
	va_start(vl, name);
	_vstprintf(buff, name, vl);
	//va_end(vl);
	TextOut(g_hRT, pt.x, pt.y, buff, _tcslen(buff));

	SelectObject(g_hRT, hOldPen);
}

void LineUpdate(POINT pt)
{
	switch (my_ptState)
	{
	case MYPT_NONE:
	case MYPT_ALL:
		my_g_Sp = {0, 0};
		my_g_Ep = {0, 0};

		my_g_Sp = pt;
		my_ptState = MYPT_V0;
		break;
	case MYPT_V0:
		my_g_Ep = pt;
		my_ptState = MYPT_ALL;
		break;

	}



	//switch (g_PtCheck)
	//{
	//	//초기 새로 시작할 경우..
	//case PT_NONE_:
	//	//입력완료후 새로 입력할 경우..
	//case PT_COMPLETED_:
	//	g_Vtx[0] = pt;
	//	g_Vtx[1] = g_VtxClear;
	//	g_Vtx[2] = g_VtxClear;
	//	g_PtCheck = PT_1_;				//첫번째 점
	//	break;

	//	//2번째 입력시...
	//case PT_1_:
	//	g_Vtx[1] = pt;
	//	g_PtCheck = PT_2_;				//두번째 점
	//	break;

	//	//3번째 입력시..
	//case PT_2_:
	//	g_Vtx[2] = pt;
	//	g_PtCheck = PT_COMPLETED_;		//입력 완료 처리.
	//	break;
	//}
}

void FaceDraw()
{
	//라인 그리기.. : v0->v1
	LineDraw(g_Vtx[0], g_Vtx[1]);

	//라인 그리기.. : v0->v2
	LineDraw(g_Vtx[0], g_Vtx[2]);

	//라인 그리기.. : v1->v2
	LineDraw(g_Vtx[1], g_Vtx[2]);
}

int RenderTargetCreate(HWND hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	HDC hdc = GetDC(hwnd);
	g_hRT = CreateCompatibleDC(hdc); // 렌더타겟, DC 핸들 생성
	g_hBmpRT = CreateCompatibleBitmap(hdc, rc.right, rc.bottom); // 렌더타겟, 비트맵 생성
	SelectObject(g_hRT, g_hBmpRT);
	ReleaseDC(hwnd, hdc);

	return TRUE;
}

void RenderTargetRelease() // 렌더타겟 제거
{
	DeleteObject(g_hBmpRT);
	DeleteDC(g_hRT);
}


/////////////////////////////////////////////////////////////////////////////// 
//
//! 도움말을 출력합니다.
//!
//! \param	없음.
//! \return 없음.
//
void ShowInfo()
{
	static bool bShow = true;
	//if (IsKeyUp(VK_F1)) bShow ^= true;

	PutFPS(1, 1);

	if (!bShow) return;

	// Today's Topic.	 
	int x = 300, y = 1;
	COLORREF col = RGB(255, 255, 255);
	COLORREF col2 = RGB(255, 255, 0);
	DrawText(x, y, col, _T("■ %s"), g_WindowName);
	DrawText(x, y += 16, col, _T("1. 점 2개를 이용한 직선 그리기"));
	DrawText(x, y += 16, col2, _T("2. 더블버퍼링 : \"BackBuffer\" 추가."));
	DrawText(x, y += 16, col2, _T("   + 속도/깜박거림 비교."));

	x = 1, y = 1;
	DrawText(x, y += 16, col, _T("도움말: F1"));

	y += 16;
	DrawText(x, y += 16, col, _T("[Line]"));
	DrawText(x, y += 16, col, _T("좌표입력 : L-Button"));
	DrawText(x, y += 16, col, _T("Sp = {%d, %d}"), g_Sp.x, g_Sp.y);
	DrawText(x, y += 16, col, _T("Ep = {%d, %d}"), g_Ep.x, g_Ep.y);
}

 


////////////////////////////////////////////////////////////////////////////////
//
//! \brief	게임 장면을 렌더링합니다.
//! \details 매 게임루프마다 게임(예제)의 장면정보를 갱신하고 출력합니다. 
//!			 게임(렌더링) 객체들은 렌더링 전에 정보를 갱신해야 합니다.
//! \note	 예제를 진행하면서 다양한 장면 객체들을 이곳에 추가할 것입니다.
//
void SceneRender()
{
	//-------------------------------
	// 엔진/시스템 갱신
	//------------------------------- 

	//렌더링 상태, 조명, 카메라 등 조절..
	//...

	//장면 갱신 : 주인공, 몬스터, 오브제 이동, 충돌검사 등등..
	//...

	//-------------------------------
	// 장면 그리기 시작
	//-------------------------------
	//장면 렌더링 : 주인공, 몬스터, 지형.. 
	//... 
	BeginScene(); 
	Clear(RGB(128, 128, 128)); // 회색으로 배경 채운다

	LineDraw();

	//도움말 출력.
	ShowInfo();
	//-------------------------------
	// 장면 그리기 종료.
	//------------------------------- 
	//... 
	//EndScene();

	Present(); 
}//end of void SceneRender()






/****************** end of file "Render.cpp" *********************************/