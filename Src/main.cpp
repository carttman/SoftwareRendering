
#include "windows.h"
#include "tchar.h"
#include "Device.h"		
#include "Render.h"
 

//////////////////////////////////////////////////////////////////////////////
//
// 전역 데이터들.
//
HWND g_hWnd = NULL;
BOOL g_bLoop = TRUE;
HINSTANCE g_hInstance = NULL;

TCHAR* g_ClassName = _T("Yena");
extern TCHAR* g_WindowName;


//윈도 설정 관련 함수들.
BOOL InitWindow		(int width, int height);
void ResizeWindow	(HWND hWnd, UINT NewWidth, UINT NewHeight);
BOOL MessagePump	();
void MoveToCenter	(HWND hwnd, int width, int height);

LRESULT CALLBACK MsgProc(HWND, UINT, WPARAM, LPARAM);



 
/////////////////////////////////////////////////////////////////////////////
//
//! 애플리케이션 메인 함수.
//
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	//------------------------------------------------------------------//
	// 기본적인 윈도우 프레임 생성..										//
	//------------------------------------------------------------------//
	if(!InitWindow(g_Mode.Width, g_Mode.Height)) return 0;

	//------------------------------------------------------------------//
	// Yena 및 관련 객체 초기화.											//
	//------------------------------------------------------------------//
	if(FAILED(YenaSetup(g_hWnd)))
		return 0;

	//------------------------------------------------------------------//
	// 데이터 로딩														//
	//------------------------------------------------------------------//
	if(!DataLoading())
	{
		g_bLoop = FALSE;  
	}
 
	//------------------------------------------------------------------//
	// 메인  루프														//
	//------------------------------------------------------------------//
	while(g_bLoop) 
	{
		if(!MessagePump())		// 메세지 펌프.
			break;
 
		SceneRender();			//렌더링.	 
	}

	//------------------------------------------------------------------//
	// 어플리케이션 종료													// 
	//------------------------------------------------------------------//
	DataRelease();				//데이터 제거.
	YenaRelease();				//예나 제거.

	return 0;
}


 

////////////////////////////////////////////////////////////////////////////////
//
//! 윈도 메시지 콜백 함수.  
//! 윈도 메세지 처리.
//
LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{ 
	case WM_KEYDOWN:
		switch(wParam)
		{
 		case VK_ESCAPE:
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);

}




/////////////////////////////////////////////////////////////////////////////
//
//! 윈도 생성 함수.
//! 입력받은 크기에 맞추어 렌더링 영역을 생성하기 위해서 
//! 클라이언트 영역을 포함 윈도 전체 크기를 재조정합니다.
//! 
//! \param width	생성할 윈도의 가로 크기.
//! \param height	생성할 윈도의 세로 크기.
//! \return			성공시 OK, 실패시 FAIL.
//
BOOL InitWindow(int width, int height)
{
	WNDCLASSEX wc = { 
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0, 0,
			::GetModuleHandle(NULL), NULL, NULL, 
			(HBRUSH)GetStockObject(DKGRAY_BRUSH), NULL, 
			g_ClassName, NULL 
	};
	RegisterClassEx(&wc);
 
	HWND hWnd = ::CreateWindow( g_ClassName, g_WindowName, 
								WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
								//WS_OVERLAPPEDWINDOW, 
								0, 0, 
								width, height, 
								GetDesktopWindow(), NULL, 
								wc.hInstance, NULL
								);
	if(hWnd == NULL) return FALSE; 

	//윈도 크기 조정 (필수) 
	ResizeWindow(hWnd, width, height);

	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
	::SetCursor(LoadCursor(NULL, IDC_ARROW));

	//윈도우 핸들 저장.
	g_hWnd = hWnd;

	return TRUE;
}




/////////////////////////////////////////////////////////////////////////////
// 
//! 메세지 펌핑 함수.  \n
//! 메인 루프 매 프레임마다 호출 되어야 하며 종료 이벤트(ESC 키 눌림) 발생시 
//! 메인 루프를 빠져나와 전체 프로세스를 종료시킨다.
//! 
//! \return	애플리케이션의 현재 동작상태
// 
int MessagePump()
{
	MSG msg;	::ZeroMemory(&msg, sizeof(msg));

	while(1)
	{
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				return FALSE;		

			//메세지 처리...
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else  
		{
			//특별한 메세지가 없다면 리턴후 게임(렌더링)을 진행합니다..
			return TRUE;		
		}
	}

	return FALSE;
}




//////////////////////////////////////////////////////////////////////////////
//
//! 윈도 크기를 조정합니다. \n
//! 입력된 크기는 클라이언트 영역의 새 크기로 사용되며 윈도 크기도 함께 조정됩니다.
//! 
//! \param hWnd		조정할 윈도 핸들
//! \param width	변경할 새 가로 크기
//! \param height	변경할 새 세로 크기
//! \return			없음
//
//! \date 2004.05.28. 버전업.
//
void ResizeWindow(HWND hWnd, UINT width, UINT height)
{	
	//현재 윈도 정보 획득. 
	RECT oldrc;	::GetWindowRect(hWnd, &oldrc);
	DWORD sytle   = (DWORD) ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD) ::GetWindowLong(hWnd, GWL_EXSTYLE);
	
	//새로 생성될 윈도우의 '클라이언트' 영역.
	RECT newrc = { 0, 0, (LONG)width, (LONG)height };
	
	//newrc 만큼의 클라이언트 영역을 포함하는 윈도 '전체' 크기를 구합니다.
	//현재 메뉴는 없다는 가정하에 처리되고 있습니다.
	//계산된 결과는 newrc 에 다시 들어옵니다. (스크린 좌표)
	//exstyle &= ~WS_EX_TOPMOST;
	::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);

	//보정된 윈도우의 너비와 폭을 구합니다. 
	int newWidth = (newrc.right - newrc.left);
	int newHeight = (newrc.bottom - newrc.top);

	//새로운 크기를 윈도우에 설정합니다.
	::SetWindowPos(hWnd, HWND_NOTOPMOST, oldrc.left, oldrc.top, 
 					newWidth, newHeight, SWP_SHOWWINDOW);

	//화면 중앙으로 이동.
	MoveToCenter(hWnd, newWidth, newHeight);

}




////////////////////////////////////////////////////////////////////////////////
//
//! \fn		MoveToCenter 
//! \brief	윈도우를 화면 중앙으로 이동시킵니다.
//! 
//! \param	hwnd	이동할 윈도우 핸들
//! \param	width	윈도우 창 가로 크기
//! \param	height	윈도우 창 세로 크기
//! \return			없음
//
void MoveToCenter(HWND hwnd, int width, int height)
{
	RECT rc = {};
	::GetClientRect(GetDesktopWindow(), &rc);
	rc.left = rc.right/2 - width/2;
	rc.top  = rc.bottom/2 - height/2;
		
	::MoveWindow(hwnd, rc.left, rc.top, width, height, TRUE);
	//::SetWindowPos(hwnd, HWND_NOTOPMOST, rc.left, rc.top, width, height, SWP_SHOWWINDOW);
}






/*************** end of "main.cpp" *****************************************/