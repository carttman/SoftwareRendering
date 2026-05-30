//! 
//! \file	Render.cpp
//! \brief	예제 기본 렌더링/프레임웍 소스 
//!	\version Yena SWR v1.5.x
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
//! 
//#pragma warning(disable:4267 4996)
#include "Windows.h"
#include "tchar.h"
#include "stdio.h"

#include "Device.h"		
#include "Render.h"

TCHAR* g_WindowName = _T("최형재 : 행렬 회전");


/////////////////////////////////////////////////////////////////////////////
// 
// 전역 데이터
//



///////////////////////////////////////////////////////////////////////////// 
//
// 정점 관련 데이터들.
//

//! 정점 버퍼 : 정점 데이터 관리용 개체. 
LPYENAVERTEXBUFFER9 g_pVB = NULL;


//! 정점 구조체 : 정점 포멧(VERTEX FORMAT) 에 따른 
//!				 정점 1개당 정보단위 처리 구조체 선언.
//! 
//! 3D 용 { Pos(Local Space), Diffuse Color } 으로 구성.
//
struct COLVTX
{
	float x, y, z;		//3D 좌표(Model Space) : '변환' 해야할 원시(원본) 모델 정보.  
	DWORD diff;			//정점색.(DWORD type)
};


//! 정점 포멧의 플래그 조합.. DX9 대응 
//! 
//#define FVF_COLVTX (B3YFVF_XY)
//#define FVF_COLVTX (B3YFVF_XY | B3YFVF_DIFFUSE)		//2D 좌표 + 정점색.
#define FVF_COLVTX (B3YFVF_XYZ | B3YFVF_DIFFUSE)		//3D 좌표 + 정점색.


//D3DXMATRIX g_mTM;		//'주인공' 모델의 월드 변환행렬.(DX)★
B3YXMATRIX g_mTM;		//'주인공' 모델의 월드 변환행렬.(YN)★


int  ObjLoad	();
void ObjRelease	();
void ObjUpdate	(float dTime);
void ObjDraw	(float dTime);




/////////////////////////////////////////////////////////////////////////////
// 
// 렌더링 상태 변수 
//
BOOL g_bWireFrame = FALSE;		//!< 와이어 프레임 출력 플래그.
BOOL g_bCulling   = FALSE;		//!<뒷면제거 플래그.


B3YXCOLOR	g_ClearColor(0.0f, 0.125f, 0.35f, 1.0f);		//배경색.

 


/////////////////////////////////////////////////////////////////////////////
//
//! 게임 데이터 및 렌더링 자원 로딩.
//! 
//! \return 성공시 OK, 실패시 FAIL.
//
int DataLoading()
{   

	//추가 코드는 여기에...
	//...

	//오브젝트 로딩.
	ObjLoad(); 


	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
//
//! 게임 데이터 및 렌더링 자원 해제.
//!
//! \param	없음.
//! \return	성공시 OK, 실패시 FAIL.
//
void DataRelease()
{
	// 데이터 해제 코드는 여기에..
	// ...  

	//오브젝트 삭제 
	ObjRelease();

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
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////
//
//! 오브젝트 : 생성 및 필요 리소스 로딩.
//! 
//! \return		성공시 OK, 실패시 FALSE
//! 
//
int ObjLoad()
{

	//기하 (3D 모델) 데이터 : 외부 저장기기(HDD) 에서 로딩되었다 가정합니다.
	COLVTX	Vertices[] = {

		// 정점 좌표는 모델 공간(Model Space, Local Space) 좌표.
		// 3D 좌표 (x, y, z)   색상( a, r, g, b)  a, 곧 Alpha 는 기본값 255 (1.0f)
		{  -0.5f, 0.0f, 0.0f,  0xffff0000 },		//v0, Red.	
		{   0.0f, 1.0f, 0.0f,  0xff00ff00 },		//v1, Green 
		{   0.5f, 0.0f, 0.0f,  0xff00ffff },		//v2, Light-Blue 

		//Face 1 : 역삼각형.(CCW) 
		{  -0.5f, 0.0f, 0.0f,  0xffff0000 },		//v3, Red.	
		{   0.0f,-1.0f, 0.0f,  0xff00ff00 },		//v4, Green 
		{   0.5f, 0.0f, 0.0f,  0xff00ffff },		//v5, Light-Blue  

	};


	// 정점 버퍼 생성.
	if (YN_FAILED(g_pDevice->CreateVertexBuffer(
							sizeof(Vertices),			// 정점 버퍼 크기 (바이트)
							0,							// 버퍼 사용방식 (기본 0)
							FVF_COLVTX,					// 정점 규격. 
							B3YPOOL_SYSTEMMEM,			// 시스템 메모리 사용. (현재는 이 옵션만 유효) 
							&g_pVB,						// 성공시 리턴되는 버퍼 운용 객체 인터페이스
							NULL						// 예약됨. NULL.
	)))
	{
		//... 에러 처리...
		return YN_FALSE;
	}

	// 버퍼 주소 얻기
	// 실제 DX 의 그것과는 역활이 다르지만, 동작의 결과는 동일합니다.
	COLVTX* pBuff = nullptr;
	if (YN_FAILED(g_pVB->Lock(0, sizeof(Vertices), (void**)&pBuff, 0)))
	{
		return YN_FALSE;
	}

	//버퍼에 정점 데이터 채우기.
	//<DX> CPU -> VB(GPU/VRAM) 정점 데이터 복사
	//<Yena> CPU -> VB(SYSMEM) 정점 데이터 복사.
	memcpy(pBuff, Vertices, sizeof(Vertices));

	//<DX> 작업 완료, 버퍼 사용이 끝나면 버퍼 잠금을 해제(Unlock) 해야합니다.
	//<Yena> 우리는 이 동작을 간략하게 처리하겠습니다.
	g_pVB->Unlock();


	//... 버퍼 생성 완료...
	//... 이제 적절한 때에 VB 를 장치에 등록(Binding) 하고 렌더링을 수행하면 되겠습니다.
	//... 생성된 VB 는 사용자가 제거해야 합니다. (Release 호출) 

	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////// 
//
//! 오브젝트 : 제거 
//
void ObjRelease()
{
	//정점 버퍼 제거..
	SafeRelease(g_pVB);

}




/////////////////////////////////////////////////////////////////////////////// 
//
//! 오브젝트 : 상태 갱신
// 
void ObjUpdate(float dTime)
{

	//스케일 처리.
	//...


	//! \todo2	기하도형 회전 시키기
	//회전 처리 <1>
	static float angle = 0.0f;
	angle += B3Y_PI * 0.5f * dTime;				//90(도)/sec  즉, 1회전에 4초 소요.
	//angle += B3YXToRadian(90) * dTime;		//위와 동일코드. 매크로 버전.	
	
	//회전 처리 <2>
	//angle = ynToRadian(0);					//수동 입력.★

	B3YXMATRIX mR;								//회선성분이 들어간 행렬 만들기★
	//B3YXMatrixRotationZ(&mR, angle);			//Z 축 기준 회전:  mR = Z축회전(angle) ★
	//B3YXMatrixRotationX(&mR, angle);			//X 축 기준 회전
	B3YXMatrixRotationY(&mR, angle);			//Y 축 기준 회전
	//B3YXMatrixRotationZ(&mR, B3YXToRadian(0));  
	//B3YXMatrixRotationZ(&mR, B3YXToRadian(-45));  


	//이동 처리.
	//...


	//최종 TM 계산 완료.
	g_mTM = mR;

}




/////////////////////////////////////////////////////////////////////////////// 
//
//! 오브젝트 그리기
//
void ObjDraw()
{

	//정점 버퍼 등록(Binding) 
	g_pDevice->SetStreamSource(0, g_pVB, 0, sizeof(COLVTX));

	//정점 형식 설정.
	g_pDevice->SetFVF(FVF_COLVTX);

	//월드 변환 행렬 설정 : 렌더링 전에 설정 되어야 합니다.
	g_pDevice->SetTransform(B3YTS_WORLD, &g_mTM);		// GPU 에 행렬 공급★  


	//'기하도형' 렌더링 : 지금부터 모든 그리기가 시작됩니다...
	// DX 메소드와 (가능한)동일한 파이프라인을 구현하는 것이 목표입니다.
	g_pDevice->DrawPrimitive(B3YPT_TRIANGLELIST, 0, 2);

}




/////////////////////////////////////////////////////////////////////////////// 
//
//! 엔진 및 시스템 상태 갱신.
//
void SystemUpdate()
{
	if (IsKeyUp(VK_SPACE))	g_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F5))		g_bCulling ^= TRUE;			

	g_pDevice->SetRenderState(B3YRS_FILLMODE, g_bWireFrame ? B3YFILL_WIREFRAME : B3YFILL_SOLID);		
	g_pDevice->SetRenderState(B3YRS_CULLMODE, g_bCulling ? B3YCULL_CCW : B3YCULL_NONE);	  //반시계방향 컬링 
	//g_pDevice->SetRenderState(B3YRS_CULLMODE,  g_bCulling? B3YCULL_CW : B3YCULL_NONE);  //시계방향 컬링 


	// 렌더링 모드에 따른 배경색 설정.
	if (g_bWireFrame) g_ClearColor = B3YXCOLOR(0.20f, 0.20f, 0.20f, 1.0f);
	else			  g_ClearColor = B3YXCOLOR(0.0f, 0.125f, 0.35f, 1.0f); 
	//else			 g_ClearColor = B3YXCOLOR(0, 0, 1, 1);

}




/////////////////////////////////////////////////////////////////////////////// 
//
//! 도움말 출력
//
void ShowInfo() 
{ 
	static bool bShow = true;
 	if(IsKeyUp(VK_F1)) bShow ^= true;
  
	if(!bShow) 
	{
		//DrawText(1,20, COLOR(1, 1, 0, 1), "[Help] F1"); 
		return;
	}


	// Today's Topic.
	{
		//int x = 350, y = 1;			
		int x = g_Mode.Width / 2 - 150;
		int y = 5;
		B3YXCOLOR col(1, 1, 1, 1);
		B3YXCOLOR col2(1, 1, 0, 1);
		B3YXCOLOR col3(0.5f, 0.5f, 0, 1);
		B3YXCOLOR col4(0.5f, 0.5f, 0.5f, 1);
		B3YXCOLOR col5(0.78f, 0.78f, 0.78f, 1);
		B3YXCOLOR col7(0, 1, 1, 1);
		B3YXCOLOR col8(1, 0.5f, 1, 1);
		B3YXCOLOR col9(1, 0.5f, 0, 1);

		DrawText(x, y,  col, _T("■ %s"), g_WindowName);

	}


	//좌측 도움말.
	{
		int x = 1;
		int y = 100;
		B3YXCOLOR col(0, 1, 0, 1);
		DWORD v;

		TCHAR* wmsg[] = { _T("N/A"), _T("POINT"), _T("WIRE"), _T("SOILD") };
		g_pDevice->GetRenderState(B3YRS_FILLMODE, &v);		
		DrawText(x, y += 14, col, _T("Fill: Space (%s)"), wmsg[v]);

		TCHAR* cmsg[] = { _T("N/A"), _T("NONE"), _T("CW"), _T("CCW") };
		g_pDevice->GetRenderState(B3YRS_CULLMODE, &v);						
		DrawText(x, y += 14, col, _T("Cull: F5 (%s)"), cmsg[v]);
	} 

}




////////////////////////////////////////////////////////////////////////////// 
//
//! 장면 그리기
//
void SceneRender()
{
	
	//-------------------------------
	// 엔진/시스템 갱신.
	//------------------------------- 
	//엔진 경과시간 얻기.
	float dTime = GetEngineTime();  //★

	//엔진 상태 갱신.
	SystemUpdate();
	

	// 장면 갱신 
	// 기하 데이터: 주인공, 몬스터, 오브제 이동, 충돌검사 등등..
	// ...
	// 게임 로직 이벤트,층돌,점수계산..
	// ...
	ObjUpdate(dTime);	      //★


	//-------------------------------
	// 장면 그리기 시작.. 
	//-------------------------------
 	 
	//장면 그리기 준비..
	g_pDevice->BeginScene();

		//렌더타겟(배경) 지우기..	
		g_pDevice->Clear(g_ClearColor);
	 
		//..지형, 오브제 그리기..등등..
		//... 

		//오브제 그리기.
		ObjDraw();

		//도움말 및 기타 렌더링 정보 출력.
		PutFPS(1, 1);
		ShowInfo();

	//장면 그리기 종료...
	g_pDevice->EndScene();

 
	//-------------------------------
	// 장면 그리기 종료.
	// 1회의 게임 루프 종료.
	//------------------------------- 
	g_pDevice->Present();			//화면출력 : Flip! 


}
