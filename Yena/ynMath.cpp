
#include "Windows.h"  
#include "stdio.h"
#include "stdlib.h" 
#include "math.h"

#include "vector"
#include "algorithm"
using namespace std;

#include "ynMath.h"

#define GLYMATH_VERSION  "2010.10.10"




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




B3YXVECTOR2::operator B3YVECTOR2* ()
{
	return (B3YVECTOR2*)this;
}

float B3YXVec2CCW(B3YXVECTOR2* v0, B3YXVECTOR2* v1)	
{
	float z = v0->x * v1->y - v0->y * v1->x;
	return z;
}


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
	
	// 1 x 4 벡터와 4 x 4 행렬을 곱셈
	v.x = (pV->x * pM->m[0][0]) + (pV->y * pM->m[1][0]) + (pV->z * pM->m[2][0]) + (pV->w * pM->m[3][0]);
	v.y = (pV->x * pM->m[0][1]) + (pV->y * pM->m[1][1]) + (pV->z * pM->m[2][1]) + (pV->w * pM->m[3][1]);
	v.z = (pV->x * pM->m[0][2]) + (pV->y * pM->m[1][2]) + (pV->z * pM->m[2][2]) + (pV->w * pM->m[3][2]);
	v.w = (pV->x * pM->m[0][3]) + (pV->y * pM->m[1][3]) + (pV->z * pM->m[2][3]) + (pV->w * pM->m[3][3]);

	if (pOut)
		*pOut = v;		//결과를 외부로 복사..

	return pOut;
}



B3YXMATRIX::B3YXMATRIX(const XMFLOAT4X4& rm)		//DX 호환용.★
{
	memcpy(m, rm.m, sizeof(B3YMATRIX));
}

B3YXMATRIX* B3YXMatrixIdentity(B3YXMATRIX* pOut)
{
	B3YXMATRIX m;
	memset(m.m, 0, sizeof(B3YMATRIX));
	
	m._12 = m._13 = m._14 =
	m._21 = m._23 = m._24 =
	m._31 = m._32 = m._34 =
	m._41 = m._42 = m._43 = 0;

	m._11 = m._22 = m._33 = m._44 = 1;

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}

B3YXMATRIX* B3YXMatrixRotationX(B3YXMATRIX* pOut, float angle)
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	const float cos = cosf(angle);
	const float sin = sinf(angle);

	m._22 = cos;
	m._23 = sin;
	m._32 = -sin;
	m._33 = cos;


	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}


B3YXMATRIX* B3YXMatrixRotationY(B3YXMATRIX* pOut, float angle) //★
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	const float cos = cosf(angle);
	const float sin = sinf(angle);


	m._11 = cos;
	m._13 = -sin;
	m._31 = sin;
	m._33 = cos;

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}


B3YXMATRIX* B3YXMatrixRotationZ(B3YXMATRIX* pOut, float angle) //★
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);
	
	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★

	// cos, sin 값을 구한다
	const float cos = cosf(angle);
	const float sin = sinf(angle);

	m._11 = cos;
	m._12 = sin;
	m._21 = -sin;
	m._22 = cos;

	if (pOut) *pOut = m;		

	return pOut;
}


B3YXCOLOR::B3YXCOLOR(DWORD col)
{
	//<Blue> 변환.
	b = (float)(col & 0x000000ff);		
	b = b / 255.0f;						

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

B3YXCOLOR B3YXCOLOR::operator * (const B3YXCOLOR& rhs)
{
	B3YXCOLOR v;

	v.r = r * rhs.r;	if (v.r > 1) v.r = 1;   if (v.r < 0) v.r = 0;
	v.g = g * rhs.g;	if (v.g > 1) v.g = 1;   if (v.g < 0) v.g = 0;
	v.b = b * rhs.b;	if (v.b > 1) v.b = 1;   if (v.b < 0) v.b = 0;
	v.a = a * rhs.a;	if (v.a > 1) v.a = 1;   if (v.a < 0) v.a = 0;

	return v;
}


B3YXCOLOR B3YXCOLOR::operator * (float rhs)
{
	B3YXCOLOR v;

	v.r = r * rhs;	if (v.r > 1) v.r = 1;   if (v.r < 0) v.r = 0;
	v.g = g * rhs;	if (v.g > 1) v.g = 1;   if (v.g < 0) v.g = 0;
	v.b = b * rhs;	if (v.b > 1) v.b = 1;   if (v.b < 0) v.b = 0;
	v.a = a * rhs;	if (v.a > 1) v.a = 1;   if (v.a < 0) v.a = 0;

	return v;
}

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


B3YXCOLOR::operator DWORD ()
{
	COLORREF col;

	col = RGB(r * 255, g * 255, b * 255);

	return col;
}


void ynLerp( _out_ int* v, int v0, int v1, float a)
{
	a = min(max(a, 0), 1);					//0~1 제한

	*v = (int)((float)v0 * (1 - a) + (float)v1 * (a));
}



void ynLerp( _out_ B3YXCOLOR* c, B3YXCOLOR c0, B3YXCOLOR c1, float a)  
{
	a = min(max(a, 0), 1);					//0~1 제한

	*c = c0 * (1 - a) + c1 * (a);			//연산자(*-+)를 재정의(overloading).
}




 



/************* end of file "ynMath.cpp" *******************************************/

 