
\mainpage

# T06 Transform (I) 03 World + Rotation (DX.Math)(GDI)
### Yena SW Renderer Tutorials v1.5.x

* 김기홍 Kihong.Kim / onlysonim@gmail.com
* Updated. 2025.04.24. (VS22)

---------------------------------------------------------------------------

<img src="T06_Transform_I_03_World+Rot_DX.Math.GDI.135.png"  style="max-width: 100%; height:auto" />


### 예제 내용 

### 렌더링 파이프라인 구축

* <GDI> 사용 버전 ★
  - 과제 제작용
   
* <DX> 수학 라이브러리 사용 버전 ★
  - 과제 제작용

* <Yena> 수학 라이브러리 제작 및 적용.
 
* 정점 파이프라인 Vertex-PipeLine
  - 월드변환  : 회전 시키기 ★
  - 뷰 변환   : 변환 없음 - 단위 행렬
  - 투영 변환 : 변환 없음 - 단위 행렬
  


### 목표 및 과제

* 변환 파이프라인의 이해	
  - 변환 행렬 적용
  - 정점 투영의 이해

*todo
\todo2 	기하도형 변환
  		- World + Rotation 변환
  		- 변환 행렬 적용
  		- 정점 투영의 이해

\todo2 	수학 라이브러리 클래스 갱신
  		- 벡터/행렬 클래스형 화
  		- 행렬 클래스 완성 ★
    		+ B3XYMATRIX : ( D3DXMATRIX 대응 )
  		- 행렬 함수 추가 제작 ★
    		+ B3YXMatrixRotationX
    		+ B3YXMatrixRotationY
    		+ B3YXMatrixRotationZ  
  		- 기타 필요 기능 확장 하기

\todo2 	DirectX 수학 클래스 사용하지 않기.★
  		- 직접 만들기 (과제)


---------------------------------------------------------------------------

### 기본 제공

* 애플리케이션 프레임워크 생성. 
* 메세지 펌핑 처리 개선.
* 32/64비트 지원
* 작업영역 색상변경 (DarkGray) 
* 윈도 중앙 이동


---------------------------------------------------------------------------

### 프로젝트 개요

* 기본 구성  
  - Windows Desktop Application
  - 32/64비트 지원
  - 유니코드 버전 (UTF-8)  

* 예제 소스 구성 
  - [Files](./files.html) 참조   

* 상세내역
  - 작업내용.md 참조

 

---------------------------------------------------------------------------


 
 
-문서 끝-
2025 ⓒ Kihong.Kim / onlysonim@gmail.com




