//! 
//! \file	ynUnknownbase.cpp
//! \brief	Yena SWR COM 기반 구현 소스 (v2.x)  
//!	\details (Windows/COM) unknownbase.h 대응 
//!	\version Yena SW Renderer v2.x 
//! 
//! \author	김기홍 / Kihong Kim / onlysonim@gmail.com / mad_dog@hanmail.net
//! \date	2003.11.04. Updated.
//! \date	2004.05.07. Updated.
//! \date	2005.09.23. Updated (.Net)
//! \date	2008.09.23. Updated.
//! \date	2010.07.20. Updated
//! \date	2015.11.20. Updated
//! \date	2020.08.22. Updated. (DX12.x Windows 10 SDK 10.0.x)
//! \date	2025.04.28. Updated. (v2)(VS22)
// 
#pragma once
//#pragma warning(disable:4996)

//시스템 / 플랫폼 헤더
#include "windows.h"

//Yena SWR COM 기반 헤더
#include "ynUnkownbase.h"



/////////////////////////////////////////////////////////////////////////////// 
//
// Yena SWR COM 전역 공통 데이터
// 
/////////////////////////////////////////////////////////////////////////////// 



/////////////////////////////////////////////////////////////////////////////// 
//
// ynIUnKnown : Yena SWR 최상위 인터페이스 <간략>
// 
/////////////////////////////////////////////////////////////////////////////// 

ULONG ynIUnknown::AddRef(void)
{
	//...
	return 1;
)


ULONG ynIUnknown::Release(void)
{
	//...
	return 0;
)






/**************** end of "ynUnknownbase.cpp" ***********************************/
