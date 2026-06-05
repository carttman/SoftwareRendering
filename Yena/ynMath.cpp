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
/*
#include "Windows.h"  
#include "stdio.h"
#include "stdlib.h" 
#include "math.h"

#include "vector"
#include "algorithm"
using namespace std;
*/
#include "Yena.h"		
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



/*
//벡터-더하기
B3YXVECTOR3 B3YXVECTOR3::operator + (B3YXVECTOR3 rhs)
{
	B3YXVECTOR3 v(x+rhs.x, y+rhs.y, z+rhs.z);
	return v;
}

//벡터-빼기
B3YXVECTOR3 B3YXVECTOR3::operator - (B3YXVECTOR3 rhs)
{
	B3YXVECTOR3 v(x-rhs.x, y-rhs.y, z-rhs.z);
	return v;
}

//벡터-스칼라 곱
B3YXVECTOR3 B3YXVECTOR3::operator * (FLOAT rhs)
{
	B3YXVECTOR3 v(x*rhs, y*rhs, z*rhs);
	return v;
}
*/


//! 전역 연산자 오버로딩 : 벡터-빼기
//! 
B3YXVECTOR3 operator - (B3YXVECTOR3 lhs, B3YXVECTOR3 rhs)
{
	B3YXVECTOR3 v(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	return v;
}



//! 벡터 노멀라이즈 : D3DXVec3Normalize 대응.
//
B3YXVECTOR3* B3YXVec3Normalize(B3YXVECTOR3* pOut, CONST B3YXVECTOR3* pV)
{
	B3YXVECTOR3 v;

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★ ^
	
	float length = sqrt((pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z));

	v = {pV->x / length, pV->y / length, pV->z / length};

	if (pOut) *pOut = v;		//결과를 외부로 복사..

	return pOut;
}



//! 벡터곱-내적: D3DXVec3Dot 대응.
//
float B3YXVec3Dot(CONST B3YXVECTOR3* pV1, CONST B3YXVECTOR3* pV2)
{
	float dt = 0;

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★ ^

	dt = (pV1->x * pV2->x) + (pV1->y * pV2->y) + (pV1->z * pV2->z);

	return dt;
}



//! 벡터곱-외적: D3DXVec3Cross 대응.
//
B3YXVECTOR3* B3YXVec3Cross(B3YXVECTOR3* pOut, CONST B3YXVECTOR3* pV1, CONST B3YXVECTOR3* pV2)
{
	B3YXVECTOR3 v;

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★ ^
	

	v = {pV1->y * pV2->z - pV1->z * pV2->x,
		 pV1->z * pV2->x - pV1->x * pV2->z,
		 pV1->x * pV2->y - pV1->y * pV2->x};

	if (pOut) *pOut = v;		//결과를 외부로 복사..

	return pOut;
}




//! 3성분 벡터 변환  
//! 벡터를 지정된 행렬로 변환한다.  D3DXVec3Transform 대응. 
//! \code (ex) v = v' * M
//! \endcode
//! \see	[D3DXVec3Transform](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec3transform)
//! \see	[XMVector3Transform](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector3transform)
//
B3YXVECTOR4* B3YXVec3Transform(B3YXVECTOR4* pOut, CONST B3YXVECTOR3* pV, CONST B3YXMATRIX* pM)
{
	B3YXVECTOR4 v(*pV);					//1x4 벡터로 확장.	v = (x, y, z, 1)
	B3YXVec4Transform(&v, &v, pM);		//v = v * m

	if (pOut) *pOut = v;				//결과를 외부로 복사..
	return pOut;
}




//! 3성분 벡터 변환 ★ 
//! 지정된 행렬로 변환하고, w=1 로 투영한다.  D3DXVec3TransformCoord 대응. 
//! 
//! \see	[D3DXVec3TransformCoord](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec3transformcoord)
//! \see	[XMVector3TransformCoord](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector3transformcoord)
//
B3YXVECTOR3* B3YXVec3TransformCoord(B3YXVECTOR3* pOut, CONST B3YXVECTOR3* pV, CONST B3YXMATRIX* pM)
{
	B3YXVECTOR4 v(*pV);					//1x4 벡터로 확장.	v = (x, y, z, 1)
	B3YXVec4Transform(&v, &v, pM);		//v = v * m

	v = v / v.w;						//w=1 로 투영..

	if (pOut) *pOut = v;				//결과를 외부로 복사..
	return pOut;
}




//! 3성분 벡터 투영 변환 : D3DXVec3Project 대응.
//!
//! \see	[Viewport & Clipping](https://learn.microsoft.com/en-us/windows/win32/direct3d9/viewports-and-clipping?redirectedfrom=MSDN)
//! \see	[D3DXVec3Project](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec3project)
//! \see	[XMVector3Project](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector3project)
//
B3YXVECTOR3* B3YXVec3Project(B3YXVECTOR3* pOut, CONST B3YXVECTOR3* pV,
							CONST B3YVIEWPORT* pViewport, CONST B3YXMATRIX* pProjection,
							CONST B3YXMATRIX* pView, CONST B3YXMATRIX* pWorld
							)
{

	B3YXVECTOR3 v;




	if (pOut) *pOut = v;					//결과를 외부로 복사..

	return pOut;
}
	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	//{
	//	#define vp	(*pViewport)

	//	//<DX> 수학 사용. (과제 제시, 참고용) : 동작 코드는 그려려니 합시다. ★
	//	XMMATRIX mProj, mView, mWorld;
	//	mProj = mView = mWorld = XMMatrixIdentity();
	//	if(pProjection) mProj  = XMLoadFloat4x4(&XMFLOAT4X4((float*)pProjection));
	//	if(pView)		mView  = XMLoadFloat4x4(&XMFLOAT4X4((float*)pView));
	//	if(pWorld)		mWorld = XMLoadFloat4x4(&XMFLOAT4X4((float*)pWorld));
	//	XMVECTOR xv		= XMVectorSet(pV->x, pV->y, pV->z, 0);
	//	xv = XMVector3Project(xv, (float)vp.X, (float)vp.Y, (float)vp.Width, (float)vp.Height, vp.MinZ, vp.MaxZ, mProj, mView, mWorld);
	//	XMFLOAT3 fv;
	//	XMStoreFloat3(&fv, xv);

	//	v = fv;

	//	#undef vp
	//}







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



B3YXVECTOR4 B3YXVECTOR4::operator / (float rhs)	
{
	x = x / rhs;
	y = y / rhs;
	z = z / rhs;
	w = w / rhs;

	return *this;
}



B3YXVECTOR4::operator B3YXVECTOR2 ()
{
	B3YXVECTOR2 v(x, y);
	return v;
}



//! 벡터-행렬 곱 함수. Out = v * M; 
//! 
B3YXVECTOR4* B3YXVec4Transform(B3YXVECTOR4* pOut, CONST B3YXVECTOR4* pV, CONST B3YXMATRIX* pM)
{
	B3YXVECTOR4 v;

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★ ^
	

	// 1 x 4 벡터와 4 x 4 행렬을 곱셈
	v.x = (pV->x * pM->m[0][0]) + (pV->y * pM->m[1][0]) + (pV->z * pM->m[2][0]) + (pV->w * pM->m[3][0]);
	v.y = (pV->x * pM->m[0][1]) + (pV->y * pM->m[1][1]) + (pV->z * pM->m[2][1]) + (pV->w * pM->m[3][1]);
	v.z = (pV->x * pM->m[0][2]) + (pV->y * pM->m[1][2]) + (pV->z * pM->m[2][2]) + (pV->w * pM->m[3][2]);
	v.w = (pV->x * pM->m[0][3]) + (pV->y * pM->m[1][3]) + (pV->z * pM->m[2][3]) + (pV->w * pM->m[3][3]);

	if (pOut) *pOut = v;		//결과를 외부로 복사..

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
//! \brief DX 수학 자료 호환용  \emoji :star:
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
//! \brief	행렬곱 연산자 오버로딩.     
//!			B3YXMatrixMultiply 와 기능적으로는 동일합니다.
//! \code	(ex) m = m1 * m2;
//! \endcode
//! \see	B3YXMatrixMultiplay, D3DXMatrixMultiply, XMMatrixMultiply
//! 
B3YXMATRIX B3YXMATRIX::operator* (CONST B3YXMATRIX& rhs) CONST
{
	B3YXMATRIX m;
	B3YXMatrixMultiply(&m, this, &rhs);

	return m;
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
//! \see [XMMatrixIdentity](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixidentity)
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
//! \brief	행렬 결합 : D3DXMatrixMultiply 대응.    
//! \code	(ex) Out = m1 * m2. 
//! \endcode
//! \param	pOut	결과를 돌려받을 행렬
//! \param	pM1,pM2	결합할 행렬들
//! \return			단위 행렬 리턴, pOut 과 동일.
//! \see	[D3DXMatrixMultiply](https://learn.microsoft.com/en-us/windows/win32/direct3d10/d3d10-d3dxmatrixmultiply), [XMMatrixMultiply](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixmultiply)
//
B3YXMATRIX* B3YXMatrixMultiply(B3YXMATRIX* pOut, CONST B3YXMATRIX* pM1, CONST B3YXMATRIX* pM2)
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★ ^
	
	
	m._11 = (pM1->_11 * pM2->_11) + (pM1->_12 * pM2->_21) + (pM1->_13 * pM2->_31) + (pM1->_14 * pM2->_41);
	m._12 = (pM1->_11 * pM2->_12) + (pM1->_12 * pM2->_22) + (pM1->_13 * pM2->_32) + (pM1->_14 * pM2->_42);
	m._13 = (pM1->_11 * pM2->_13) + (pM1->_12 * pM2->_23) + (pM1->_13 * pM2->_33) + (pM1->_14 * pM2->_43);
	m._14 = (pM1->_11 * pM2->_14) + (pM1->_12 * pM2->_24) + (pM1->_13 * pM2->_34) + (pM1->_14 * pM2->_44);

	m._21 = (pM1->_21 * pM2->_11) + (pM1->_22 * pM2->_21) + (pM1->_23 * pM2->_31) + (pM1->_24 * pM2->_41);
	m._22 = (pM1->_21 * pM2->_12) + (pM1->_22 * pM2->_22) + (pM1->_23 * pM2->_32) + (pM1->_24 * pM2->_42);
	m._23 = (pM1->_21 * pM2->_13) + (pM1->_22 * pM2->_23) + (pM1->_23 * pM2->_33) + (pM1->_24 * pM2->_43);
	m._24 = (pM1->_21 * pM2->_14) + (pM1->_22 * pM2->_24) + (pM1->_23 * pM2->_34) + (pM1->_24 * pM2->_44);

	m._31 = (pM1->_31 * pM2->_11) + (pM1->_32 * pM2->_21) + (pM1->_33 * pM2->_31) + (pM1->_34 * pM2->_41);
	m._32 = (pM1->_31 * pM2->_12) + (pM1->_32 * pM2->_22) + (pM1->_33 * pM2->_32) + (pM1->_34 * pM2->_42);
	m._33 = (pM1->_31 * pM2->_13) + (pM1->_32 * pM2->_23) + (pM1->_33 * pM2->_33) + (pM1->_34 * pM2->_43);
	m._34 = (pM1->_31 * pM2->_14) + (pM1->_32 * pM2->_24) + (pM1->_33 * pM2->_34) + (pM1->_34 * pM2->_44);

	m._41 = (pM1->_41 * pM2->_11) + (pM1->_42 * pM2->_21) + (pM1->_43 * pM2->_31) + (pM1->_44 * pM2->_41);
	m._42 = (pM1->_41 * pM2->_12) + (pM1->_42 * pM2->_22) + (pM1->_43 * pM2->_32) + (pM1->_44 * pM2->_42);
	m._43 = (pM1->_41 * pM2->_13) + (pM1->_42 * pM2->_23) + (pM1->_43 * pM2->_33) + (pM1->_44 * pM2->_43);
	m._44 = (pM1->_41 * pM2->_14) + (pM1->_42 * pM2->_24) + (pM1->_43 * pM2->_34) + (pM1->_44 * pM2->_44);

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}




////////////////////////////////////////////////////////////////////////////////
//
//! \brief	이동 행렬 생성 : D3DXMatrixTranslation 대응.   
//! \code	(ex) Out = treans(x, y, z) 
//! \endcode
//! \see	[D3DXMatrixTranslation](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixtranslation)
//! \see	[XMMatrixTranslation](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixtranslation)
//
B3YXMATRIX* B3YXMatrixTranslation(B3YXMATRIX* pOut, float x, float y, float z)
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);
	
	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★ ^

	m._41 += x;
	m._42 += y;
	m._43 += z;

	if (pOut) *pOut = m;					//결과를 외부로 복사..

	return pOut;
}




////////////////////////////////////////////////////////////////////////////////
//
//! \brief	스케일 행렬 생성 : D3DXMatrixScaling 대응.    
//! \code	(ex) Out = scaling(sx, sy, sz) 
//! \endcode
//! \see	[D3DXMatrixScaling](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixscaling)
//! \see	[XMMatrixScaling](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixscaling)
//
B3YXMATRIX* B3YXMatrixScaling(B3YXMATRIX* pOut, float sx, float sy, float sz)
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);
	
	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	//{
	//	//<DX> 수학 사용. (과제 제시, 참고용) : 동작 코드는 그려려니 합시다. ★
	//	XMFLOAT4X4  xm;
	//	XMStoreFloat4x4(&xm, XMMatrixScaling(sx, sy, sz));
	//	m = xm;
	//}

	if (pOut) *pOut = m;					//결과를 외부로 복사..

	return pOut;
}




////////////////////////////////////////////////////////////////////////////////
//
//! \brief	X 축 기준 회전행렬 생성함수 (YN) 
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
//
B3YXMATRIX* B3YXMatrixRotationX(B3YXMATRIX* pOut, float angle)
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★ ^
	const float cos = cosf(angle);
	const float sin = sinf(angle);

	m._22 = cos;
	m._23 = sin;
	m._32 = -sin;
	m._33 = cos;

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}



////////////////////////////////////////////////////////////////////////////////
//
//! \brief Y 축 기준 회전행렬 생성함수 (ver.YN) 
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
//
B3YXMATRIX* B3YXMatrixRotationY(B3YXMATRIX* pOut, float angle)
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★ ^
	const float cos = cosf(angle);
	const float sin = sinf(angle);


	m._11 = cos;
	m._13 = -sin;
	m._31 = sin;
	m._33 = cos;


	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}



////////////////////////////////////////////////////////////////////////////////
//
//! \brief	Z 축 기준 회전행렬 생성함수 (ver.YN) 
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
B3YXMATRIX* B3YXMatrixRotationZ(B3YXMATRIX* pOut, float angle)
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);
	
	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★ ^
	// cos, sin 값을 구한다
	const float cos = cosf(angle);
	const float sin = sinf(angle);

	m._11 = cos;
	m._12 = sin;
	m._21 = -sin;
	m._22 = cos;

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}







////////////////////////////////////////////////////////////////////////////////
//
// 뷰-투영 행렬 함수 (YN)
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// B3YXMatrixLookAtLH
// 
//! \brief 뷰 변환 행렬 생성 : D3DXMatrixLookAtLH 대응. 
//! 
//! \code
//!  mV = W-1 = (RT)-1 = (T-1)(R-1)  (-1=역행렬)
//!  zaxis = normal(At - Eye)
//!  xaxis = normal(cross(Up, zaxis))
//!  yaxis = cross(zaxis, xaxis)
//! 
//!  행렬구성 m =   {  xaxis.x			yaxis.x			zaxis.x          0
//!	 			      xaxis.y			yaxis.y			zaxis.y          0
//!				      xaxis.z			yaxis.z			zaxis.z          0
//!			       -dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1  } 
//! \endcode
//! \param	pOut	변환 행렬, 출력
//! \param	pEye	카메라 위치
//! \param	pLookAt 카메라 시점
//! \param	pUp		카메라 상방 벡터
//! \return	변환 행렬, pOut 과 동일
//! 
//! \see B3YXMatrixLookAtLH, B3YXMatrixPerspectiveFovLH
//! \see [D3DXMatrixLookAtLH](https://learn.microsoft.com/en-us/windows/win32/direct3d10/d3d10-d3dxmatrixlookatlh), [D3DXMatrixPerspectiveFovLH](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh)
//! \see [XMMatrixLookAtLH](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixlookatlh), [XMMatrixPerspectiveFovLH](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixperspectivefovlh)
//
B3YXMATRIX* B3YXMatrixLookAtLH(B3YXMATRIX* pOut, CONST B3YXVECTOR3* pEye, CONST B3YXVECTOR3* pLookAt, CONST B3YXVECTOR3* pUp)
{
	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	{
		//<DX> 수학 사용. (과제 제시, 참고용) : 동작 코드는 그려려니 합시다. ★
		XMFLOAT4X4 xm;
		XMVECTOR eye, lookat, up;		 
		eye    = XMVectorSet(pEye->x, pEye->y, pEye->z, 1);
		lookat = XMVectorSet(pLookAt->x, pLookAt->y, pLookAt->z, 1);
		up     = XMVectorSet(pUp->x, pUp->y, pUp->z, 0);
		XMStoreFloat4x4(&xm, XMMatrixLookAtLH(eye, lookat, up));
		m = xm;
	}

	if (pOut) *pOut = m;		//결과를 외부로 복사..

	return pOut;
}




////////////////////////////////////////////////////////////////////////////////
//
// B3YXMatrixPerspectiveFovLH
//! 
//! \brief 투영 변환 행렬 생성 : D3DXMatrixPerspectiveFovLH 대응. 
//!
//! 투영변환은 zn=1 인 평면에 시야볼륨에 해당하는 정점들을 (축소)투영하는 것으로
//! 각 성분의 최종 변환은 다음과 같습니다.
//!
//! \code
//!
//! m = {  X    0     0     0
//!        0    Y     0     0
//!        0    0     A     C
//!        0    0     B     D   }
//!
//!  X = 1/r.tan(fovy/2)  : X 축 스케일 (Zn=1 인 평면에 (축소)투영하기 위한 스케일 성분)
//!  Y = 1/tan(fovy/2)    : Y 축 스케일 (r : Aspect-ratio)
//!  A = zf/(zf-zn)       : Z 축 정규화 순서보존 함수  g(z) = A + B/z
//!  B = -(zf*zn)/(zf-zn) : Z 축 정규화 순서보전 함수
//!  C = 1				  : 투영변환 중 비선형 연산 Z 축(원근) 나누기를 위한 원본 z값 저장용(행렬적용후 별도 사용)
//!  D = 0				  : 투영행렬은 이동 변환 없음.
//!
//!  *순서보존함수 : 일정연산후에도 결과가 비례하여 순서를 보존시키는 함수.
//! 
//! \endcode
//! \param	pOut	변환행렬, 출력
//! \param	fovy	시야 볼륨 수평각도(y)
//! \param	aspect	시야 볼륨 가로 대 세로 비율
//! \param	zn		근평면 거리 (z)
//! \param	zf		원평면 거리 (z)
//! \return	변환 행렬, pOut 과 동일
//! 
//! \see B3YXMatrixLookAtLH, B3YXMatrixPerspectiveFovLH
//! \see [D3DXMatrixLookAtLH](https://learn.microsoft.com/en-us/windows/win32/direct3d10/d3d10-d3dxmatrixlookatlh), [D3DXMatrixPerspectiveFovLH](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh)
//! \see [XMMatrixLookAtLH](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixlookatlh), [XMMatrixPerspectiveFovLH](https://learn.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmmatrixperspectivefovlh)
//
B3YXMATRIX* B3YXMatrixPerspectiveFovLH(B3YXMATRIX* pOut, FLOAT fovy, FLOAT aspect, FLOAT zn, FLOAT zf)
{

	B3YXMATRIX m;
	B3YXMatrixIdentity(&m);

	//<Yena> 다음의 코드 블럭을 제거하고, 수학 기능을 직접 제작 하십시오. (과제) ★
	{
		//<DX> 수학 사용. (과제 제시, 참고용) : 동작 코드는 그려려니 합시다. ★
		XMFLOAT4X4 xm;
		XMStoreFloat4x4(&xm, XMMatrixPerspectiveFovLH(fovy, aspect, zn, zf));
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

 