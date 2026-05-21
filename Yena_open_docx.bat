cls
echo off
rem echo  참고 : https://www.doxygen.nl/manual/doxygen_usage.html
echo ===========================================================
echo =  Doxygen Commmandline builder
echo =  2021.08.22 김기홍 / Kihong.Kim / onlysonim@gmail.com
echo ===========================================================

rem echo ----------------------------------------
rem echo 독시젠 실행 
rem echo ----------------------------------------
rem set DOXY_PATH="C:\Program Files\doxygen\bin"
rem set DOXYGEN="C:\Program Files\doxygen\bin\doxygen.exe" 

rem 각 작업 경로 저장  : "...\DirectX 12\10.v.4.5.6@1\00.Shader_00\dox" 기준.
rem set DOX_DIR="%CD%"
rem set OUTPUT="..\doc"
rem set OUTPUT="..\..\doc"

rem <config file> 은 Doxygen Wizard 를 통해 생성함.
rem set DOXY_CFG="Doxyfile_yn_v5" 

rem 현재 폴더 <Project>\doxy 기준 작업 진행...
rem %DOXY_PATH%\doxygen.exe	%DOXY_CFG%



echo ----------------------------------------
echo html help 열기
echo ----------------------------------------
set OUTPUT=".\doc"
set SRC_HTML=%OUTPUT%\html\index.html 
rem set SRC_HTML=%OUTPUT%\html\md__read_me.html 
rem 생성된 html 열기
rem explorer %SRC_HTML%

explorer %SRC_HTML%

 

rem ----------------------------------------
rem chm 만들기 --> 사용않함.
rem ----------------------------------------
rem call doxychm.bat


rem echo ----------------------------------------
rem echo  pdf 만들기 + 복사 + 열기. 
rem echo ----------------------------------------
rem set SRC_PDF=%OUTPUT%\LaTex\refman.pdf
rem set TGT_PDF=%OUTPUT%\GP3D_help.pdf



rem 문서끝
