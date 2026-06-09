//! 
//! \file	Render.cpp
//! \bstar	예제 기본 렌더링/프레임웍 소스 
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

TCHAR* g_WindowName = _T("Yena SW-Renderer : T06 Transform (I) 07 Matrix Composition (행렬결합+이동)(DX.Math)(GDI)(Ready)");


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


//D3DXMATRIX g_mTM;		//'주인공' 모델의 월드 변환행렬.(DX)
B3YXMATRIX g_mTM;		//'주인공' 모델의 월드 변환행렬.(YN)
B3YXMATRIX g_mView;		//뷰 변환행렬.(YN)
B3YXMATRIX g_mProj;		//투영 변환행렬.(YN)

B3YXVECTOR3 g_vPos(0, 0, 0);		//주인공 월드 좌표.★


int  ObjLoad	();
void ObjRelease	();
void ObjUpdate	(float dTime);
//void ObjDraw	(float dTime);




/////////////////////////////////////////////////////////////////////////////
// 
// 렌더링 상태 변수 
//
BOOL g_bWireFrame = FALSE;		//!< 와이어 프레임 출력 플래그.
BOOL g_bCulling   = FALSE;		//!< 뒷면제거 플래그.

BOOL g_bGrid	  = TRUE;		//!< 격자 출력 플래그.
BOOL g_bAxis	  = TRUE;		//!< 방향축 출력 플래그.

B3YXCOLOR	g_ClearColor(0.0f, 0.125f, 0.35f, 1.0f);		//배경색.


//그리드 객체.
ynGrid* g_pGrid = NULL;

//방향축 객체.
ynAxis* g_pAxis = NULL;







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


	//그리드 생성...
	ynGridCreate(&g_pGrid, g_pDevice, 10, 10, 1.0f, COLOR(0.3f, 0.3f, 0.3f, 1));

	//방향축 생성...
	ynAxisCreate(&g_pAxis, g_pDevice, g_Mode.Width, g_Mode.Height);


	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
//
//! 게임 데이터 및 렌더링 자원 해제.
//!
//! \param	없음.
//! \return	없음.
//
void DataRelease()
{
	// 데이터 해제 코드는 여기에..
	// ...  

	//오브젝트 삭제 
	ObjRelease();


	//그리드&방향축 제거.
	SafeDelete(g_pGrid);
	SafeDelete(g_pAxis);

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

		/*//Face 1 : 역삼각형.(CCW) 
		{  -0.5f, 0.0f, 0.0f,  0xffff0000 },		//v3, Red.	
		{   0.0f,-1.0f, 0.0f,  0xff00ff00 },		//v4, Green 
		{   0.5f, 0.0f, 0.0f,  0xff00ffff },		//v5, Light-Blue  
		*/
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
//! 
//! \todo2 "주인공" 움직이기, 방향키 조작				
//! \todo2 "주인공" 변환 행렬 결합 연습 (SRT, STR)		
// 
void ObjUpdate(float dTime)
{

	// 스케일 처리
	//
	B3YXMATRIX mScale;
	B3YXMatrixScaling(&mScale, 3, 3, 3);				//원본 100%
	//B3YXMatrixScaling(&mScale, 2, 1, 1);				//원본 200% (x축)
	//B3YXMatrixScaling(&mScale, 3, 3, 3);				//원본 300%



	//회전 처리 <1>
	static float angle = 0.0f;
	//angle += B3Y_PI * dTime;							//180 (도)/sec  즉, 1회전에 2초 소요.
	angle += B3YXToRadian(180) * dTime;					//위와 동일코드. 매크로 버전.	
	//angle = ynToRadian(0);							//수동 입력.

	B3YXMATRIX mRot;									//회선성분이 들어간 행렬 만들기
	//B3YXMatrixRotationZ(&mRot, angle);				//Z 축 기준 회전:  mR = Z축회전(angle) 
	//B3YXMatrixRotationX(&mRot, angle);				//X 축 기준 회전
	B3YXMatrixRotationY(&mRot, angle);					//Y 축 기준 회전 
	//B3YXMatrixRotationZ(&mRot, B3YXToRadian(0));  
	//B3YXMatrixRotationZ(&mRot, B3YXToRadian(-45));  


	// 이동 처리 ★
	//
	static float mov = 5.0f;
	if (IsKeyDown(VK_LEFT))		g_vPos.x -= mov * dTime;
	if (IsKeyDown(VK_RIGHT))	g_vPos.x += mov * dTime;
	if (IsKeyDown(VK_UP))		g_vPos.z += mov * dTime;
	if (IsKeyDown(VK_DOWN))		g_vPos.z -= mov * dTime;

	// 이동행렬 만들기.	
	B3YXMATRIX mTrans;
	B3YXMatrixTranslation(&mTrans, g_vPos.x, g_vPos.y, g_vPos.z);	 //★
	//B3YXMatrixTranslation(&mTrans, 3, 0, 0);			//이동 테스트1 
	//B3YXMatrixTranslation(&mTrans, 3, 0, 4);			//이동 테스트2 

	//----------------------------
	// 최종 변환 행렬 결합. 
	//----------------------------
	// 렌더링전에 Device 에 설정되어야 합니다.
	//
	g_mTM = mScale * mRot * mTrans;		//행렬 결합 방향 주의!
	//g_mTM = mRot;						//[테스트]변화시킬 단일 정보만 설정하는 것도 가능.

	/*
	//DXUT 함수를 통한 행렬 결합.
	//상기 결과와 동일. 인자의 순서가 곧 결합 방향 .
	B3YXMatrixMultiply(&g_mTM, &mScale, &mRot);		//mTM = mScale * mRot
	B3YXMatrixMultiply(&g_mTM, &g_mTM, &mTrans);	//mTM = mTM * mTrans
	*/

	//----------------------------
	// 행렬 결합 역회전 테스트.
	//----------------------------
	//g_mTM = mScale * mTrans * mRot;		//행렬 결합 방향 주의! 


}




/////////////////////////////////////////////////////////////////////////////// 
//
//! \bstar 오브젝트 그리기
//! 
//! \note	장치는 렌더링 명령을 받은 후 현재 장치에 설정된 자원(기하, 버퍼, 텍스처), 파이프라인 설정, 
//!			변환 행렬, 셰이더 등을 일괄 사용하여 장면을 그려냅니다. 
//!			적절한 장면 연출을 위해서는 그에 맞는 월드-뷰-투영 변환 행렬을 장치에 설정해야 합니다. 
//! 
//! \todo2 \star \star [과제] 주인공 키보드로 이동하기:
//!			여러분의 수학함수로 변환 시킵니다. \emoji :sunglasses:
//
void ObjDraw()
{

	//정점 버퍼 등록(Binding) 
	g_pDevice->SetStreamSource(0, g_pVB, 0, sizeof(COLVTX));

	//정점 형식 설정.
	g_pDevice->SetFVF(FVF_COLVTX);

	//월드 변환 행렬 설정 : 렌더링 전에 설정 되어야 합니다.
	g_pDevice->SetTransform(B3YTS_WORLD, &g_mTM);		// GPU 에 행렬 공급  


	//'기하도형' 렌더링 : 지금부터 모든 그리기가 시작됩니다...
	// DX 메소드와 (가능한)동일한 파이프라인을 구현하는 것이 목표입니다.
	g_pDevice->DrawPrimitive(B3YPT_TRIANGLELIST, 0, 1);	

}



/////////////////////////////////////////////////////////////////////////////// 
//
//! 카메라 갱신 
//! 사용자 지정 카메라 정보를 통해 뷰(View) 및 투영(Projection) 변환 행렬을 생성하고
//! 장치에 설정합니다.. 
//! 
//! \see B3YXMatrixLookAtLH, B3YXMatrixPerspectiveFovLH
//! \see [D3DXMatrixLookAtLH](https://learn.microsoft.com/en-us/windows/win32/direct3d10/d3d10-d3dxmatrixlookatlh), [D3DXMatrixPerspectiveFovLH](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh)
//! \see [XMMatrixLookAtLH](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixlookatlh), [XMMatrixPerspectiveFovLH](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixperspectivefovlh)
//
void CameraUpdate(float dTime)
{
	
	// 뷰 변환 View Transform : 카메라 Body 설정.
	//..
	//B3YXVECTOR3	eye(0.0f, 0.0f, -5.0f);				//카메라 위치.1 
	B3YXVECTOR3		eye(0.0f, 5.0f, -10.0f);			//카메라 위치.2 
	//B3YXVECTOR3	eye(3.0f, 5.0f, -10.0f);			//카메라 위치.3 테스트. 
	B3YXVECTOR3		lookat(0.0f, 0.0f, 0.0f);			//바라보는 곳.위치.
	B3YXVECTOR3		up(0.0f, 1.0f, 0.0f);				//카메라 상방 벡터.
	B3YXMatrixLookAtLH(&g_mView, &eye, &lookat, &up);			

	//Rendering Device 에 뷰 변환 행렬 설정.
	g_pDevice->SetTransform(B3YTS_VIEW, &g_mView);


	// 투영 변환 Projection Transform  : 렌즈 설정.  
	//
	FLOAT fov = B3YXToRadian(45);									//시야각
	FLOAT aspect = g_Mode.Width / (float)g_Mode.Height;				//가로:세로 비율
	FLOAT zn = 1.0f;												//근평면 거리(시야 최소한계)
	FLOAT zf = 100.0f;												//원평면 거리(시야 최대한계)
	B3YXMatrixPerspectiveFovLH(&g_mProj, fov, aspect, zn, zf);		

	//Rendering Device 에 투영 행렬 설정.
	g_pDevice->SetTransform(B3YTS_PROJECTION, &g_mProj);

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


	//격자/방향축 출력.<DX>미지원 <Yena> 전용 
	if (IsKeyUp(VK_F2))		g_bGrid ^= TRUE;
	if (IsKeyUp(VK_F3))		g_bAxis ^= TRUE;
	g_pDevice->SetRenderState(B3YRS_GRIDMODE, g_bGrid);
	g_pDevice->SetRenderState(B3YRS_AXISMODE, g_bAxis);
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
		int x = 200; //g_Mode.Width / 2 - 200;
		int y = 5;
		B3YXCOLOR col(1, 1, 1, 1);
		B3YXCOLOR col2(1, 1, 0, 1);
		B3YXCOLOR col3(0.5f, 0.5f, 0, 1);
		B3YXCOLOR col4(0.5f, 0.5f, 0.5f, 1);
		B3YXCOLOR col5(0.78f, 0.78f, 0.78f, 1);
		B3YXCOLOR col7(0, 1, 1, 1);
		B3YXCOLOR col8(1, 0.5f, 1, 1);

		DrawText(x, y,  col, _T("■ %s"), g_WindowName);
		x = 230; y+=14;
		DrawText(x, y += 14, col,  _T("1. 렌더링 파이프라인(Rendering Pipeline) 의 이해."));
		//DrawText(x, y += 14, col5, _T("2. 3D->2D 변환 과정의 이해 및 연구."));
		//DrawText(x, y += 14, col5, _T("3. Local (Model) Space 의 이해."));
		//DrawText(x, y += 14, col3, _T("2. 월드 변환 (World Transform) : 스케일-회전-이동 변환 구현."));
		//DrawText(x, y += 14, col2, _T("3. 뷰 변환(View Transform) : 카메라 추가"));
		//DrawText(x, y += 14, col2, _T("4. 투영 변환(Projection Transform) : 카메라 렌즈"));
		DrawText(x, y += 14, col2, _T("2. 월드 변환 : 이동/회전/스케일 행렬 결합 연습"));
		DrawText(x, y += 14, col2, _T("3. 뷰 변환 : 다양한 카메라 설정 연습"));
		DrawText(x, y += 14, col2, _T("4. 그리드 및 방향축 추가"));
		//DrawText(x, y += 14, col3, _T("5. Yena-Math : 행렬 클래스 제작 (B3YXMATRIX)"));

		y += 14;
		DrawText(x, y += 14, col,  _T("* 정점 파이프라인 (Vertex Pipeline) 구현."));
		DrawText(x, y += 14, col4, _T("* 기하 파이프라인 (Geometry Pipeline) 구현."));
		DrawText(x, y += 14, col4, _T("* 픽셀 파이프라인 (Pixel Pipeline) 구현."));

		y += 14;
		DrawText(x, y += 14, col2, _T("* 그리드 및 방향축 : class + DX행렬 + GDI그리기"));
		DrawText(x, y += 15, col8, _T("* 키보드로 '주인공' 움직이기"));

		//y += 14;
		y = g_Mode.Height - 100;
		DrawText(x, y += 14, col7, _T("* DirectX Math : 행렬 및 함수 사용 (XMMATRIX)(과제 제작용)"));
		DrawText(x, y += 14, col8, _T("* Yena Math : 행렬 클래스 및 함수 제작 (B3YXMATRIX)(과제)"));
		DrawText(x, y += 14, col7, _T("* _DrawFace (GDI)"));
	}


	//좌측 도움말.
	{
		int x = 1;
		int y = 100;
		B3YXCOLOR col(0, 1, 0, 1);
		B3YXCOLOR colON(0, 1, 0, 1);
		B3YXCOLOR colON2(1, 1, 0, 1);
		B3YXCOLOR colOFF(0.4f, 0.4f, 0.4f, 1);
		DWORD v;

		TCHAR* wmsg[] = { _T("N/A"), _T("POINT"), _T("WIRE"), _T("SOILD") };
		g_pDevice->GetRenderState(B3YRS_FILLMODE, &v);		
		DrawText(x, y += 14, col, _T("Fill: Space (%s)"), wmsg[v]);

		TCHAR* cmsg[] = { _T("N/A"), _T("NONE"), _T("CW"), _T("CCW") };
		g_pDevice->GetRenderState(B3YRS_CULLMODE, &v);						
		DrawText(x, y += 14, col, _T("Cull: F5 (%s)"), cmsg[v]);


		DWORD bOn;
		g_pDevice->GetRenderState(B3YRS_GRIDMODE, &bOn);				
		DrawText(x, y += 14, bOn ? colON : colOFF, _T("Grid : F2 (%s)"), bOn ? _T("ON") : _T("OFF"));
		g_pDevice->GetRenderState(B3YRS_AXISMODE, &bOn);				
		DrawText(x, y += 14, bOn ? colON : colOFF, _T("Axis : F3 (%s)"), bOn ? _T("ON") : _T("OFF"));


		y += 14;
		col = B3YXCOLOR(1, 1, 0, 1);
		DrawText(x, y += 14, col, _T("[오브제]"));
		DrawText(x, y += 14, col, _T("이동: 방향키"));
		DrawText(x, y += 14, col, _T("Pos={%.2f, %.2f, %.2f}"), g_vPos.x, g_vPos.y, g_vPos.z);

	} 

}




////////////////////////////////////////////////////////////////////////////// 
//
//! \bstar 장면 그리기
//
void SceneRender()
{
	
	//-------------------------------
	// 엔진/시스템 갱신.
	//------------------------------- 
	//엔진 경과시간 얻기.
	float dTime = GetEngineTime();  

	//엔진 상태 갱신.
	SystemUpdate();
	

	// 장면 갱신 
	// 기하 데이터: 주인공, 몬스터, 오브제 이동, 충돌검사 등등..
	// ...
	// 게임 로직 이벤트,층돌,점수계산..
	// ...
	ObjUpdate(dTime);			//"주인공" 움직이기..★	

	CameraUpdate(dTime);		//카메라-뷰-투영 적용 


	//그리드 갱신.
	if (g_bGrid) g_pGrid->Update();
	if (g_bAxis) g_pAxis->Update();


	//-------------------------------
	// 장면 그리기 시작.. 
	//-------------------------------
 	 
	//장면 그리기 준비..
	g_pDevice->BeginScene();

		//렌더타겟(배경) 지우기..	
		g_pDevice->Clear(g_ClearColor);

		//축-그리드 그리기..
		if (g_bGrid) g_pGrid->Render(dTime);
		if (g_bAxis) g_pAxis->Render(dTime);
	 
		//..지형, 오브제 그리기..등등..
		//... 

		ObjDraw();				//"주인공" 그리기..


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



 


/****************** end of file "Render.cpp" *********************************/