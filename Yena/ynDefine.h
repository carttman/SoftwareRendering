//! 
//! \file	ynDefine.h
//! \brief	Yena SW 엔진 기본 정의 문서. 
//!	\version Yena S/W Renderer v1.x  
//! 
//! \author	김기홍 / Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net
//! \date	2003.11.04. Updated.
//! \date	2004.05.07. Updated
//! \date	2005.09.23. Updated (.Net)
//! \date	2008.09.23. Updated.
//! \date	2010.07.20. Updated
//! \date	2015.11.20. Updated
//! \date	2020.08.22. Updated. (DX12.x Windows 10 SDK 10.0.x)
//! \date	2025.04.28. Updated. (v2)(VS22)
//! \ingroup	Yena-Class
//
#pragma once


/////////////////////////////////////////////////////////////////////////////// 
//
// 시스템 / 플랫폼 헤더
//
//#include "B3API.h"		//2005.04.07. DLL Export.
#include "windows.h" 

#pragma warning(disable : 4305)
//#pragma warning(disable : 5906)

 
/////////////////////////////////////////////////////////////////////////////
//
#define YES_ 1
#define NO_	 0



/////////////////////////////////////////////////////////////////////////////
//
// 메소드/함수 팔라미터 속성 표시용 (가독성 향상)
//
#ifndef _in_
#define _in_            //input
#define _in_out_        //input & output
#define _inout_         //input & output
#define _out_           //output
#define _out_opt_       //output, option
#define _opt_           //option
#define _in_opt_        //input, option
#endif


//폐기 경고 : C4996 에러 적용됨.
#ifndef _deprecated_
#define _not_implemented_	[[deprecated("> 미구현 - 사용 중지")]]
#define _deprecated_		[[deprecated("> 폐기됨 - 사용 중지")]]
#define YN_NOT_IMPLEMENTED	_not_implemented_;
//#define YN_미구현_미사용		_not_implemented_
#define YN_미구현_미사용		//! \brief \emoji :construction: \emoji :no_entry: 미구현, 미사용
#define YN_폐기됨_미사용		//! \brief \emoji :construction: \emoji :no_entry: 폐기됨, 사용중지
#define YN_DEPRECATED		_deprecated_ 
#define YN_DEPRECATED_(s)	YN_폐기됨_미사용##(s)
#define ynDeprecatedMsg(s)	ynBuildMsg("폐기됨 - 사용중지 "s)
//#define ynDeprecatedMsg(s)	ynBuildMsg("폐기됨 - 사용중지 : "##s); YN_DEPRECATED_(s) 
#endif

#ifndef _omitted_
#define _omitted_
#define _기능_생략_		//_omitted_
#define YN_기능_생략_	//_omitted_
#endif



/////////////////////////////////////////////////////////////////////////////
//
#include "assert.h"
#define ASSERT(Val) assert((Val))

#define YN_INVALIED(res) ((res) == NULL)
#define YN_VALIED(res)	 ((res) != NULL)
#define YN_FAILED(res)	 ((res) < 0 )
#define YN_SUCCEEDED(res) ((res) == YN_OK )


#define YN_ENABLED(res)	 ((res) == TRUE )
#define YN_DISABLED(res) ((res) == FALSE )

//#define YN_CHECK	YN_ENABLED

#define YN_OK       0 
#define YN_FALSE   -1 
#define YN_FAIL    YN_FALSE
#define YN_NULL		nullptr



/////////////////////////////////////////////////////////////////////////////
//
#define _LOGFILE_ON_			//로그 파일 생성하기..



/////////////////////////////////////////////////////////////////////////////
//
// DXUtil.h 의 것을 사용함. 2004.06.07. 
//
#ifndef SafeDelete
#define SafeDelete(pBuff)	if((pBuff)){ delete (pBuff);	 (pBuff) = NULL; }
#define SafeDelArry(pBuff)	if((pBuff)){ delete [] (pBuff);  (pBuff) = NULL; }
#define SafeRelease(pBuff)	if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#endif



/////////////////////////////////////////////////////////////////////////////
//
// 각도-호도/라디안 변환 매크로
// pi : 180 = r : 1 
// r = pi / 180      즉  1 라디안 : 0.017444444444444444
// 
#define YN_PI	3.141592f 
#define ynConvertToRadian(deg)	((float)(YN_PI/180.0f)*((float)(deg)))	//!< 각도-라디안 변환 매크로
#define ynConvertToDegree(rad)	((rad) * 180.0f / YN_PI)				//!< 라디안-각도 변환 매크로
//#define ynConvertToAngle(r)  
 
//구형 호환성 유지.
#define B3Y_PI	YN_PI				
#define MAKERADIAN	ynConvertToRadian




/*********** end of file "ynDefine.h" *******************************/