//! 
//! \file	ynMath.cpp
//! \bstar	Yena 수학 라이브러리
//!			Yena Math Library for DirectX / OpenGL 
//!	\version Yena SW Renderer v2.x
//! 
//! \author	Kihong Kim / mad_dog@hanmail.net
//! \date	2004.05.07. Updated.
//! \date	2010.07.20. Updatee.
//! \date	2010.10.10. Updated. glyVec3Add, glyVec3Sub
//! \date	2015.11.20. Updated.
//! \date	2025.04.28. Updated. (v1.x)(VS22)
//
#include "Windows.h"  
#include "stdio.h"
#include "stdlib.h" 
#include "math.h"

#include "vector"
#include "algorithm"
using namespace std;

#include "ynMath.h"

#define GLYMATH_VERSION  "2010.10.10"




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// VECTOR2 :  2성분 벡터 연산자 오버로딩  
//
///////////////////////////////////////////////////////////////////////////////

B3YXVECTOR2 B3YXVECTOR2::operator + (const B3YXVECTOR2& rhs)
{
	B3YXVECTOR2 v;
	v.x = x + rhs.x;
	v.y = y + rhs.y;

	return v;
}


B3YXVECTOR2 B3YXVECTOR2::operator - (const B3YXVECTOR2& rhs)
{
	B3YXVECTOR2 v;
	v.x = x - rhs.x;
	v.y = y - rhs.y;

	return v;
}


/* -> B3YXVECTOR2 로 통합, 폐기.
B3YVECTOR2 B3YXVECTOR2::operator = (B3YXVECTOR2 rhs)
{
	B3YVECTOR2 v = { x, y };
	return v;
}*/



B3YXVECTOR2::operator B3YVECTOR2* ()
{
	return (B3YVECTOR2*)this;
}



///////////////////////////////////////////////////////////////////////////////
//
//! B3YXVec2CCW : 2성분 외적, 외적의 결과가 양수면, 'CCW' .Back-Face Culling 에 사용.
//!			    : D3DXVec2CCW 대응 (https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec2ccw)
//!  
//! 2성분으로는 구조적으로 외적이 불가능하므로 3차원으로 확장 사용합니다. 
//!		v0=(x1,y1,0) cross  v1=(x2,y2,0). 
//! 또는 아래 수식을 참조하십시오.
//! \code  
//! [3성분 외적] 
//! VECTOR3 v;
//! v.x = v0.y * v1.z - v0.z * v1.y;
//! v.y = v0.z * v1.x - v0.x * v1.z;
//! v.z = v0.x * v1.y - v0.y * v1.x;
//! \endcode
//! 
//
float B3YXVec2CCW(B3YXVECTOR2* v0, B3YXVECTOR2* v1)	
{
	float z = v0->x * v1->y - v0->y * v1->x;
	return z;
}



///////////////////////////////////////////////////////////////////////////////
//
//!  2성분 벡터 : 길이 구하기. 
//
float B3YXVec2Length(B3YXVECTOR2* v)
{
	float len = sqrtf((v->x * v->x) + (v->y * v->y));

	return len;
}


float B3YXVec2Length(float x, float y)
{
	float len = sqrtf((x * x) + (y * y));

	return len;
}






///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// class B3YXVECTOR3
//
///////////////////////////////////////////////////////////////////////////////

B3YXVECTOR3::B3YXVECTOR3(const B3YXVECTOR4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}



B3YXVECTOR3::operator B3YXVECTOR2 ()         
{
	//return (B3YXVECTOR3)*this;
	B3YXVECTOR2 v(x, y);
	return v;
}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// class B3YXVECTOR4  
//
///////////////////////////////////////////////////////////////////////////////

B3YXVECTOR4 B3YXVECTOR4::operator=(const B3YXVECTOR3& rhs) 
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = 1.0f;

	return *this;
}


B3YXVECTOR4::operator B3YXVECTOR2 ()
{
	B3YXVECTOR2 v(x, y);
	return v;
}



//! \brief	벡터-행렬 곱 함수. 
//!			Out = v * M;
//! \todo2	[과제] 벡터-행렬 곱 함수 만들기
//! 
B3YXVECTOR4* B3YXVec4Transform(B3YXVECTOR4* pOut, CONST B3YXVECTOR4* pV, CONST B3YXMATRIX* pM)
{
	B3YXVECTOR4 v{0, 0, 0, 0};

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	//{
	//	//<DX> 수학 사용. (과제 제시, 참고용) : 동작 코드는 그려려니 합시다. ★
	//	XMVECTOR xv = XMVectorSet(pV->x, pV->y, pV->z, pV->w);
	//	XMMATRIX xm((const float*)(pM->m));
	//	XMFLOAT4 fv;
	//	XMStoreFloat4(&fv, XMVector4Transform(xv, xm));
	//	v = fv;
	//}
	
	// 1 x 4 벡터와 4 x 4 행렬을 곱셈
	v.x = (pV->x * pM->m[0][0]) + (pV->y * pM->m[1][0]) + (pV->z * pM->m[2][0]) + (pV->w * pM->m[3][0]);
	v.y = (pV->x * pM->m[0][1]) + (pV->y * pM->m[1][1]) + (pV->z * pM->m[2][1]) + (pV->w * pM->m[3][1]);
	v.z = (pV->x * pM->m[0][2]) + (pV->y * pM->m[1][2]) + (pV->z * pM->m[2][2]) + (pV->w * pM->m[3][2]);
	v.w = (pV->x * pM->m[0][3]) + (pV->y * pM->m[1][3]) + (pV->z * pM->m[2][3]) + (pV->w * pM->m[3][3]);

	if (pOut)
		*pOut = v;		//결과를 외부로 복사..

	return pOut;
}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// class B3YXMATRIX  
//
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//! \bstar DX 수학 자료 호환용
//! XMFLOAT4X4 형을 B3YXMATRIX 형으로 변환합니다.
//! 
//! \param	rm DX 행렬
//
B3YXMATRIX::B3YXMATRIX(const XMFLOAT4X4& rm)		//DX 호환용.★
{
	memcpy(m, rm.m, sizeof(B3YMATRIX));
	//memcpy(m, rm.m, sizeof(float) * 4 * 4);		//상동.
}
 



////////////////////////////////////////////////////////////////////////////////
//
//! \brief	행렬 단위화(초기화) 함수   
//! \code	<Yena> Out = I. 
//! \endcode
//! \param	pOut	결과를 돌려받을 행렬
//! \return			단위 행렬 리턴, pOut 과 동일.
//! 
//! \see [D3DXMatrixIdentity](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixidentity)
//! 
B3YXMATRIX* B3YXMatrixIdentity(B3YXMATRIX* pOut)
{
	B3YXMATRIX m;
	memset(m.m, 0, sizeof(B3YMATRIX));
	/*
	m._12 = m._13 = m._14 =
	m._21 = m._23 = m._24 =
	m._31 = m._32 = m._34 =
	m._41 = m._42 = m._43 = 0;
	*/

	m._11 = m._22 = m._33 = m._44 = 1;

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}





////////////////////////////////////////////////////////////////////////////////
//
//! B3YXMatrixRotationX
//! \bstar	X 축 기준 회전행렬 생성함수 (YN) 
//!			단 회전량은 라디안. 
//! \code
//! 행렬구성 m = { 1,   0,     0,   0
//!                0, cosθ, sinθ,   0
//!                0,-sinθ, cosθ,   0
//!                0,   0,     0,   1 }
//! \endcode
//! \param	pOut	회전 행렬
//! \param	angle	회전 각 (라디안)
//! \return	회전 행렬, pOut 리턴
//! \see	[D3DXMatrixRotationX](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixrotationx)
//! \see	[XMMatrixRotationX](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixrotationx)
//!
//! \todo2 [과제] 회전행렬(X축) 함수 만들기 
//
B3YXMATRIX* B3YXMatrixRotationX(B3YXMATRIX* pOut, float angle)
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	{
		//<DX> 수학 사용. (과제 제시, 참고용) : 동작 코드는 그려려니 합시다. ★
		XMFLOAT4X4 xm; 
		XMStoreFloat4x4(&xm, XMMatrixRotationX(angle));
		m = xm;
	}

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}



////////////////////////////////////////////////////////////////////////////////
//
//! \bstar Y 축 기준 회전행렬 생성함수 (ver.YN) 
//!			단 회전량은 라디안.
//! \code
//! 행렬구성 m = { cosθ,   0,-sinθ,   0
//!                   0,   1,    0,   0
//!                sinθ,   0, cosθ,   0
//!                   0,   0,    0,   1 }
//! \endcode
//! \param	pOut	회전 행렬
//! \param	angle	회전 각 (라디안)
//! \return	회전 행렬, pOut 리턴
//! \see	[D3DXMatrixRotationY](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixrotationy)
//! \see	[XMMatrixRotationY](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixrotationy)
//!
//! \todo2 [과제] 회전행렬(Y축) 함수 만들기
//
B3YXMATRIX* B3YXMatrixRotationY(B3YXMATRIX* pOut, float angle) //★
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	{
		//<DX> 수학 사용. (과제 제시, 참고용) : 동작 코드는 그려려니 합시다. ★
		XMFLOAT4X4 xm; 
		XMStoreFloat4x4(&xm, XMMatrixRotationY(angle));
		m = xm;
	}


	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}



////////////////////////////////////////////////////////////////////////////////
//
//! \bstar	Z 축 기준 회전행렬 생성함수 (ver.YN) 
//!			단 회전량은 라디안.
//! \code
//! 행렬구성 m = {  cosθ, sinθ,   0,   0
//!                -sinθ, cosθ,   0,   0
//!                    0,    0,   1,   0
//!                    0,    0,   0,   1 }
//! \endcode
//! \param	pOut	회전 행렬
//! \param	angle	회전 각 (라디안)
//! \return	회전 행렬, pOut 리턴
//! \see	[D3DXMatrixRotationZ](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixrotationz)
//! \see	[XMMatrixRotationZ](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixrotationz)
//!
//! \todo2 [과제] 회전행렬(Z축) 함수 만들기
// 
B3YXMATRIX* B3YXMatrixRotationZ(B3YXMATRIX* pOut, float angle) //★
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);
	
	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	{
		//<DX> 수학 사용. (과제 제시, 참고용) : 동작 코드는 그려려니 합시다. ★
		XMFLOAT4X4 xm;
		XMStoreFloat4x4(&xm, XMMatrixRotationZ(angle));
		m = xm;
	}

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}






 

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// struct COLOR
//
///////////////////////////////////////////////////////////////////////////////

//
//! 생성자 오버로딩.
//!
//! BYTE(r, g, b, a) -> float (r, g, b, a)
// 
/*B3YXCOLOR::B3YXCOLOR(BYTE _r, BYTE _g, BYTE _b, BYTE _a)
{
	b = _b/255.0f;					//실수형으로 전환.  0~ 1.0f
	g = _g/255.0f;
	r = _r/255.0f;
	a = _a/255.0f;
}
*/

//! DWORD 형 색상에서 float 타입으로 색상정보를 전환합니다.
//! DWORD(a, r, g, b)(DX) --> float(r, g, b, a)(Yena) 로 전환.
//!   0x AA RR GG BB       
//! 각 채널별 시작 비트..
//! Blue 채널: 0번비트  
//! Gree 채널: 8번비트 
//! Red  채널: 16번비트 
//! Alpha 채널: 24번비트
//
B3YXCOLOR::B3YXCOLOR(DWORD col)
{
	//<Blue> 변환.
	b = (float)(col & 0x000000ff);		//정수형.blue 채널값 얻기.  0 ~ 255
	b = b / 255.0f;						//여기서 다시 실수형으로 전환.  0~ 1.0f 

	//<Green> 변환.
	g = (float)((col & 0x0000ff00) >> 8);
	g = g / 255.0f;

	//<Red> 변환.
	r = (float)((col & 0x00ff0000) >> 16);
	r = r / 255.0f;

	//<Alpha> 변환.
	a = (float)((col & 0xff000000) >> 24);
	a = a / 255.0f;

}




// 
//! 색상 연산 오버로딩..
//!
//! 모든 연산에 색상 포화도(Saturation) 제한을 처리합니다.
//! 0 ~ 1.0f ( 0~ 255)
//

//! 색상 채널별 곱셈.  
B3YXCOLOR B3YXCOLOR::operator * (const B3YXCOLOR& rhs)
{
	B3YXCOLOR v;

	v.r = r * rhs.r;	if (v.r > 1) v.r = 1;   if (v.r < 0) v.r = 0;
	v.g = g * rhs.g;	if (v.g > 1) v.g = 1;   if (v.g < 0) v.g = 0;
	v.b = b * rhs.b;	if (v.b > 1) v.b = 1;   if (v.b < 0) v.b = 0;
	v.a = a * rhs.a;	if (v.a > 1) v.a = 1;   if (v.a < 0) v.a = 0;

	return v;
}


//! 색상 스칼라 곱.:  
B3YXCOLOR B3YXCOLOR::operator * (float rhs)
{
	B3YXCOLOR v;

	v.r = r * rhs;	if (v.r > 1) v.r = 1;   if (v.r < 0) v.r = 0;
	v.g = g * rhs;	if (v.g > 1) v.g = 1;   if (v.g < 0) v.g = 0;
	v.b = b * rhs;	if (v.b > 1) v.b = 1;   if (v.b < 0) v.b = 0;
	v.a = a * rhs;	if (v.a > 1) v.a = 1;   if (v.a < 0) v.a = 0;

	return v;
}

//! 색상 혼합.
B3YXCOLOR B3YXCOLOR::operator + (const B3YXCOLOR& rhs)
{
	B3YXCOLOR v;
	v.r = r + rhs.r;	if (v.r > 1) v.r = 1;
	v.g = g + rhs.g;	if (v.g > 1) v.g = 1;
	v.b = b + rhs.b;	if (v.b > 1) v.b = 1;
	v.a = a + rhs.a;	if (v.a > 1) v.a = 1;

	return v;
}

//! 색상 뺄셈.
B3YXCOLOR B3YXCOLOR::operator - (const B3YXCOLOR& rhs)
{
	B3YXCOLOR v;

	v.r = r - rhs.r;	if (v.r < 0) v.r = 0;
	v.g = g - rhs.g;	if (v.g < 0) v.g = 0;
	v.b = b - rhs.b;	if (v.b < 0) v.b = 0;
	v.a = a - rhs.a;	if (v.a < 0) v.a = 0;

	return v;
}


//! [ GDI 대응 형변환 ]
//! float(r, g, b, a) -> DWORD(a, b, g, r) 로 전환. (GDI용) 
//! Blue 채널이 16번비트..주의.
//
B3YXCOLOR::operator DWORD ()
{
	COLORREF col;

	col = RGB(r * 255, g * 255, b * 255);

	return col;
}



/* [ DX 대응 형변환] <예시>
// float(r, g, b, a) -> DWORD(a, r, g, b) 로 전환. (DX용)
// Blue 채널이 0번비트..임에 주의.
//
B3YXCOLOR::operator DWORD ()
{
	DWORD col;
	DWORD B = (DWORD)(b*255.0f);		//여기서 다시 실수형으로 전환.  0~1.0f -> 0~255.0f;
	col =  B ;							//정수형.blue 넣기.  0 ~ 255

	DWORD G = (DWORD)(g*255.0f);
	col |= (G <<8);

	DWORD R = (DWORD)(r*255.0f);
	col |= (R<<16);

	DWORD A = (DWORD)(a/255.0f);
	col |= (A<<24);

	return col;
}
*/









///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
//! ynLerp : 선형보간 : 정수형 
//! 
//! \param[out]	v	보간된 값
//! \param		v0	시작 값
//! \param		v1	끝 값
//! \param		a	보간율 (alpha : 0.0~1.0)
//
void ynLerp( _out_ int* v, int v0, int v1, float a)
{
	//if (a < 0) a = 0;	if (a > 1) a = 1;
	a = min(max(a, 0), 1);					//0~1 제한

	*v = (int)((float)v0 * (1 - a) + (float)v1 * (a));
}


/*
///////////////////////////////////////////////////////////////////////////////
//
//! ynLerp 색상 선형보간.
//! DWORD 타입은 연산자 오버로딩 필요.
//!
//! \param[out]	c	보간된 색상
//! \param		c0	시작 색
//! \param		c1	끝 색
//! \param		a	보간율 (alpha : 0.0~1.0)
//
void ynLerp(DWORD* c, DWORD c0, DWORD c1, float a)
{
	//if (a < 0)  a = 0;	if (a > 1) a = 1;
	a = min(max(a, 0), 1);					//0~1 제한

	*c = c0 * (1 - a) + c1 * (a);			//연산자(*-+)를 재정의(overloading).
}
*/



////////////////////////////////////////////////////////////////////////////////
//
//! ynLerp 색상 선형보간. 
//! B3YXCOLOR 타입은 연산자 오버로딩 필요.
//! 
//! \param[out]	c	보간된 색상
//! \param		c0	시작 색
//! \param		c1	끝 색
//! \param		a	보간율 (alpha : 0.0~1.0)
//
void ynLerp( _out_ B3YXCOLOR* c, B3YXCOLOR c0, B3YXCOLOR c1, float a)  
{
	//if (a < 0) a = 0;	if (a > 1) a = 1;
	a = min(max(a, 0), 1);					//0~1 제한

	*c = c0 * (1 - a) + c1 * (a);			//연산자(*-+)를 재정의(overloading).
}




 



/************* end of file "ynMath.cpp" *******************************************/

 