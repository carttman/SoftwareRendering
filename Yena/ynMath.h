//! 
//! \file	ynMath.h
//! \bstar	Yena 수학 라이브러리
//!			Yena Math Library for DirectX / OpenGL 
//!	\version Yena SW Renderer v2.x
//! 
//! \author	Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net
//! \date	2004.05.07. Updated.
//! \date	2010.07.20. Updatee.
//! \date	2015.11.20. Updated.
//! \date	2025.04.28. Updated. (v1.x)(VS22)
//! \addtogroup Yena-Math
//! \{
//
#pragma once



/////////////////////////////////////////////////////////////////////////////// 
//
#include "ynDefine.h"

//DX / XMMath 사용, (과제 제작용) ★
//DirectXMath 는 Windows SDK 에 이미 포함되어 있음.
#include "DirectXmath.h"
using namespace DirectX;



//OpenGL 표준 헤더파일. 필수요소.
//#include <GL/gl.h>
//#include <GL/glu.h>

#define YN_PI   3.141592f
#define ynToRadian(a)  (a/180.0f * YN_PI)
#define ynToDegree(r)  (r * 180.0f / YN_PI)
#define ynToAngle(r)	ynToDegree(r)

//구형 호환성 재정의
#define YNToRadian	ynToRadian
#define YNToAngle	ynToAngle
#define YNToDegree	ynToDegree

#define B3YXToRadian  YNToRadian			//D3DXToRadian 대응.
#define B3YXToDegree  YNToDegree			//D3DXToDegreee 대응.






/////////////////////////////////////////////////////////////////////////////// 
//
//! 벡터 데이터.
//!
//! 멤버데이터를 '수평:행'으로 나열했지만,
//! 벡터/행렬 연산시에는 '수직:열'기준 처리됨을 기억합시다.
//! 아래의 벡터 표현식 참조.
//!
//!  x   or   x
//!  y        y
//!  z        z
//!           w
//
/////////////////////////////////////////////////////////////////////////////// 

//! \struct B3YVECTOR2
//! 2성분 벡터 구조체.
//
struct B3YVECTOR2 {
	float x, y;  
};


//! \struct B3YVECTOR3
//! 3성분 벡터 구조체.
//
struct B3YVECTOR3 {
	float x, y, z;  
};


//! \struct B3YVECTOR4
//! 4성분 벡터.구조체.
//
struct B3YVECTOR4 {
	float x, y, z, w; 
};



////////////////////////////////////////////////////////////////////////////////
//
// 클래스 선행 선언 
//
//class B3YXVECTOR2;
class B3YXVECTOR3;
class B3YXVECTOR4;
class B3YXMATRIX;		
struct B3YVIEWPORT;			




////////////////////////////////////////////////////////////////////////////////
// 
//! \class	B3YXVECTOR2
//! \brief	벡터 클래스 (2성분)  
//! \details VECTOR2 일반 구조체 + 생성자/연산자 오버로딩. 
//
class B3YXVECTOR2 : public B3YVECTOR2				 
{
public:
	//float x, y; 
public:
	B3YXVECTOR2()					 { x =  0;	 y =  0;  }
	B3YXVECTOR2(float _x, float _y)  { x = _x;	 y = _y;  }
	B3YXVECTOR2(const B3YVECTOR2& v) { x = v.x;  y = v.y; }

	//연산자 오버로딩.
	B3YXVECTOR2 operator + (const B3YXVECTOR2& rhs);
	B3YXVECTOR2 operator - (const B3YXVECTOR2& rhs);
	//B3YVECTOR2	operator = (B3YXVECTOR2 rhs);
	operator B3YVECTOR2* ();


	operator POINT() { return POINT{ (LONG)x, (LONG)y }; }
};
typedef B3YXVECTOR2 VECTOR2;


//! 2성분 외적 : 외적의 결과가 양수면, 'CCW' .Back-Face Culling 에 사용.
//!			   : D3DXVec2CCW 대응 (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec2ccw)
//! 
float B3YXVec2CCW(B3YXVECTOR2* v0, B3YXVECTOR2* v1);	

//! 2성분 벡터의 길이 구하기.
float B3YXVec2Length(B3YXVECTOR2* v);			
float B3YXVec2Length(float x, float y);			






////////////////////////////////////////////////////////////////////////////////
// 
//! \class	B3YXVECTOR3
//! \brief	벡터 클래스 (3성분)    
//! \details VECTOR3 일반 구조체 + 생성자/연산자 오버로딩.  
//
class B3YXVECTOR3 : public B3YVECTOR3			
{
public:
	//float x, y, z; 
public:
	B3YXVECTOR3() 							 { x = y = z = 0; 				}			
	B3YXVECTOR3(float _x, float _y, float _z){ x = _x;  y = _y;   z = _z; 	}
	B3YXVECTOR3(const B3YVECTOR3&  v)		 { x = v.x; y = v.y;  z = v.z; 	}  
	B3YXVECTOR3(const B3YXVECTOR4& v);
	//B3YXVECTOR4(const D3DXVECTOR4& v) 	 { x = v.x;  y = v.y;  z = v.z; }		//D3DX 행렬 대응 
	B3YXVECTOR3(const XMFLOAT3& v)			 { x = v.x;  y = v.y;  z = v.z; }		//XMMath 행렬 대응 ★
	B3YXVECTOR3(const XMFLOAT4& v)			 { x = v.x;  y = v.y;  z = v.z; }		//XMMath 행렬 대응 ★

	//연산자 오버로딩.
	operator B3YXVECTOR2 ();					

};
typedef B3YXVECTOR3 VECTOR3;

//전역 연산자 오버로딩
B3YXVECTOR3  operator - (B3YXVECTOR3 rhs);

//벡터 노멀라이즈 : D3DXVec3Normalize 대응.
B3YXVECTOR3* B3YXVec3Normalize	(B3YXVECTOR3* pOut, CONST B3YXVECTOR3* pV);
//벡터곱-내적: D3DXVec3Dot 대응.
float		 B3YXVec3Dot		(CONST B3YXVECTOR3* pV1, CONST B3YXVECTOR3* pV2);
//벡터곱-외적: D3DXVec3Cross 대응.
B3YXVECTOR3* B3YXVec3Cross		(B3YXVECTOR3* pOut, CONST B3YXVECTOR3* pV1, CONST B3YXVECTOR3* pV2);
//3성분 벡터와 행렬 곱 : D3DXVec3Transform 대응. 
B3YXVECTOR4* B3YXVec3Transform	(B3YXVECTOR4 *pOut, CONST B3YXVECTOR3 *pV, CONST B3YXMATRIX *pM );
//3성분 벡터와 행렬 곱 : D3DXVec3TransformCoord 대응. 
B3YXVECTOR3* B3YXVec3TransformCoord	(B3YXVECTOR3* pOut, CONST B3YXVECTOR3* pV, CONST B3YXMATRIX* pM);

//3성분투영 : D3DXVec3Project 대응.
B3YXVECTOR3* B3YXVec3Project	(B3YXVECTOR3 *pOut, CONST B3YXVECTOR3 *pV, 
								CONST B3YVIEWPORT *pViewport, CONST B3YXMATRIX *pProjection, 
								CONST B3YXMATRIX *pView, CONST B3YXMATRIX *pWorld);








////////////////////////////////////////////////////////////////////////////////
// 
//! \class	B3YXVECTOR4
//! \brief	벡터 클래스 (4성분)    
//! \details VECTOR4 일반 구조체 + 생성자/연산자 오버로딩.  
//
class B3YXVECTOR4 : public B3YVECTOR4				 
{
public:
	//float x, y, z, w; 
public:
	B3YXVECTOR4() 										{ x = y = z = w = 0; 					}
	B3YXVECTOR4(float _x, float _y, float _z, float _w) { x = _x;  y = _y;   z = _z;  w = _w;	}
	B3YXVECTOR4(const B3YVECTOR4& v)					{ x = v.x; y = v.y;  z = v.z; w = v.w;  }  
	B3YXVECTOR4(const B3YXVECTOR3& v) 					{ x = v.x;  y = v.y;  z = v.z; w = 1; 	}
	//B3YXVECTOR4(const D3DXVECTOR4& v) 				{ x = v.x;  y = v.y;  z = v.z; w = v.w; }		//D3DX 행렬 대응 
	B3YXVECTOR4(const XMFLOAT4& v)						{ x = v.x;	y = v.y;  z = v.z; w = v.w; }		//XMMath 행렬 대응 ★

	//연산자 오버로딩.
	B3YXVECTOR4 operator = (const B3YXVECTOR3& rhs);		
	B3YXVECTOR4 operator / (float rhs);				
	operator B3YXVECTOR2 ();						

};
typedef B3YXVECTOR4 VECTOR4;


// 벡터-행렬 곱 함수. <Yena> Out = v * M; 
B3YXVECTOR4* B3YXVec4Transform(B3YXVECTOR4* pOut, CONST B3YXVECTOR4* pV, CONST B3YXMATRIX* pM);

// 벡터-행렬 곱 함수. <DX> Out = v * M;
//D3DXVECTOR4* D3DXVec4Transform ( D3DXVECTOR4* pOut, CONST D3DXVECTOR4 *pV, CONST D3DXMATRIX *pM ); 





////////////////////////////////////////////////////////////////////////////////
// 
//! \struct B3YMATRIX 
//! \bstar  행렬 구조체
//! 
//! \copydoc struct_B3YMATRIX
//! 
struct B3YMATRIX
{
	//4X4 행렬 성분 추가....(과제)  
	union
	{
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
	
};



////////////////////////////////////////////////////////////////////////////////
// 
//! \class	B3YXMATRIX 
//! \bstar	행렬 클래스
//! 
//! \todo2	[과제] 행렬 클래스 만들기 \n
//!			DX 행렬 클래스를 참고하여 필요한 기능을 구현하십시오.
//! \see	[D3DXMATRIX](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrix)
//! \see	[XMMATRIX](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/ns-directxmath-xmmatrix)
//! \see	B3YMATRIX
//
class B3YXMATRIX : public B3YMATRIX
{
public:
	//4X4 행렬 기능 추가....(필요시)(과제) ★ 

public:
	B3YXMATRIX() {}
	B3YXMATRIX(const XMFLOAT4X4& m);		//DX 호환용.

	//연산자 오버로딩도 필요합니다.(필요시)(과제) 
	B3YXMATRIX operator* (CONST B3YXMATRIX& rhs) CONST;	

};
typedef B3YXMATRIX MATRIX;


//행렬 단위화(초기화) 함수 <DX> Out = I. 
//D3DXMATRIX* D3DXMatrixIdentity ( D3DXMATRIX *pOut );
//행렬 단위화(초기화) 함수 <Yena> Out = I. 
B3YXMATRIX* B3YXMatrixIdentity		(B3YXMATRIX* pOut);
//행렬 결합 : D3DXMatrixMultiply 대응. Out = m1 * m2. 
B3YXMATRIX* B3YXMatrixMultiply		(B3YXMATRIX* pOut, CONST B3YXMATRIX* pM1, CONST B3YXMATRIX* pM2);
//이동 행렬 생성 : D3DXMatrixTranslation 대응. Out = treans(x, y, z) 
B3YXMATRIX* B3YXMatrixTranslation	(B3YXMATRIX* pOut, float x, float y, float z);
//스케일 행렬 생성 : D3DXMatrixScaling 대응. Out = scaling(sx, sy, sz) 
B3YXMATRIX* B3YXMatrixScaling		(B3YXMATRIX* pOut, float sx, float sy, float sz);

//회전축 기준 회전행렬 생성 함수, 회전량은 라디안. 
B3YXMATRIX* B3YXMatrixRotationX		(B3YXMATRIX* pOut, float angle);		// X 축 회전 행렬 계산 
B3YXMATRIX* B3YXMatrixRotationY		(B3YXMATRIX* pOut, float angle);		// Y 축 회전 행렬 계산 
B3YXMATRIX* B3YXMatrixRotationZ		(B3YXMATRIX* pOut, float angle);		// Z 축 회전 행렬 계산 

//뷰 변환 행렬 생성 : D3DXMatrixLookAtLH 대응.(과제)★
B3YXMATRIX* B3YXMatrixLookAtLH			(B3YXMATRIX* pOut, CONST B3YXVECTOR3* pEye, CONST B3YXVECTOR3* pAt, CONST B3YXVECTOR3* pUp);
//투영 변환 행렬 생성 : D3DXMatrixPerspectiveFovLH 대응.(과제)★
B3YXMATRIX* B3YXMatrixPerspectiveFovLH	(B3YXMATRIX* pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf);






////////////////////////////////////////////////////////////////////////////////
//  
//! \struct B3YCOLOR
//! 컬러 구조체
//! 
//! \copydoc struct_B3YCOLOR
//! 
struct B3YCOLOR 
{
	float r, g, b, a;
};
//typedef B3YCOLOR COLOR;



////////////////////////////////////////////////////////////////////////////////
//
//! \class	B3YXCOLOR
//! \brief	컬러 클래스 : B3YCOLOR 확장 클래스   
//!         
//! \note	필요한 메소드/연산자를 완성할 것.
//! 
class B3YXCOLOR : public B3YCOLOR			
{
public:
	//float r, g, b, a;

public:
	B3YXCOLOR ()											{ r = g = b = 0; a = 1.0f;		  }	//기본색 (0,0,0,a=1) 
	B3YXCOLOR (float _r, float _g, float _b, float _a = 1)	{ r = _r; g = _g; b = _b; a = _a; }
	//B3YXCOLOR(BYTE _r, BYTE _g, BYTE _b, BYTE _a);	
	//B3YXCOLOR(COLORREF col);						
	B3YXCOLOR(DWORD col);						

	B3YXCOLOR operator* (const B3YXCOLOR& rhs);		//색상 채널별 곱.
	B3YXCOLOR operator* (float rhs);				//색상 스칼라 곱.
	B3YXCOLOR operator+ (const B3YXCOLOR& rhs);		//색상 덧셈 (채널별)
	B3YXCOLOR operator- (const B3YXCOLOR& rhs);		//색상 뺄셈 (채널별)
	operator DWORD	();								//DWORD 형변환.

};

typedef B3YXCOLOR COLOR;


//사용 안함.
//#define TOCOLOR(col)  (*(COLOR*)&(col))
//COLOR   B3YXColor(float r, float g, float b, float a);



 


////////////////////////////////////////////////////////////////////////////////
//  
//! 헬퍼 함수 및 지원 라이브러리 : B3YXVECTOR 클래스 타입으로 갱신.
//

//! 선형보간 (Linear Interpolation) 함수 
void  ynLerp	(_out_ int* v, int v0, int v1, float a);
//void ynLerp	(_out_ DWORD* c, DWORD c0, DWORD c1, float a); 		//색상(DWORD)
void  ynLerp	(_out_ B3YXCOLOR* c, B3YXCOLOR c0, B3YXCOLOR c1, float a); //색상(B3YXCOLOR)


//! 구형 호환성 유지용 재정의
//! 
#define YnLerp		ynLerp






//! \}	//end of Yena-Math
/************* end of file "ynMath.h" *******************************************/



