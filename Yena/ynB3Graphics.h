#pragma once

#define _omitted_
#define _기능_생략_	//_omitted_


struct VS_OUTPUT
{
	VECTOR4 pos;	//위치 
	COLOR	diff;	//확산광 색상 (Diffuse Color)
};

//! 각 VP 처리 결과 저장 배열.
typedef std::vector<VS_OUTPUT>  VSOUTS;


class B3YenaCore							
{
	friend class B3YenaDevice9;
	friend class B3YenaDevice9x;

protected:


public:
	B3YenaCore(void);
	virtual ~B3YenaCore(void);

	
};

typedef B3YenaCore*		LPB3YENACORE;
typedef B3YenaCore*		LPB3YENAENGINECORE;



class B3YenaGraphicsEngine9	: public B3YenaCore
{
	friend class B3YenaDevice9;
	friend class B3YenaDevice9x;


public:
	
	struct INPUT_STREAM
	{
		LPYENAVERTEXBUFFER9	pVB[1];			
		DWORD FVF;							
		UINT  Stride;						


		//그리기 옵션.(DrawPrimitive 호출시 입력됨)
		B3YPRIMITIVETYPE PrimitiveType;		
		UINT PrimitiveCount;				
		UINT StartVertex;					
		UINT TopologyVtxSize;				
	};

	struct STREAM_DBG
	{
		UINT  CurrVtxIndex;					
		//UINT  FaceNum;					
		UINT  CurrPrimIndex;				
		UINT  CurrPrimDrawCnt;				
	};

	struct TRANSFORM
	{
		B3YXMATRIX mTFM[B3YTS_MAX_];		
	};


	struct PIPELINE_STATE
	{
		DWORD  RState[B3YRS_MAX_];			
	};

	struct OUTPUT_MERGE
	{
		HWND hWnd;							
		YNPRESENT_PARAM	PresentParam;		
		
		HBITMAP  hBmpRT;					
		HDC		 hRT;						
		#define hSurfaceRT hRT				
	};



protected:

	B3YenaDevice9*	m_pDev;			

	//그래픽 엔진 - 파이프라인 별 운용 데이터
	INPUT_STREAM	m_input;		
	STREAM_DBG		m_stm;			
	TRANSFORM		m_tms;			
	PIPELINE_STATE	m_pso;			
	OUTPUT_MERGE	m_om;				

protected:
	
	
	
	

	
	
	
	int  _RenderTargetCreate	();
	void _RenderTargetRelease	();
	HDC	 _GetDCRT				();
	 
	int _SetVertexBuffer		(IYenaVertexBuffer9* pVB, UINT Stride);
	int _SetFVF					(DWORD fvf); 

	void _TempBufferRelease		();

	
	
	virtual int _BeginScene		();
	virtual int _EndScene		();
	virtual int _Clear			(COLORREF col);
	virtual int _Present		();



	
	virtual int _VertexStreaming	(_out_ B3YXVECTOR3 oPos[3], _out_ B3YXCOLOR oColor[3]);
	virtual int _VertexPipeline		(_in_ B3YXVECTOR3 iPos[3], _out_ B3YXVECTOR4 oPos[3], _inout_ B3YXCOLOR oColor[3]);
	virtual int _GeometryPipeline	(_in_ B3YXVECTOR4 iPos[3], _out_ B3YXVECTOR3 oPos[3], _inout_ B3YXCOLOR oColor[3]);
	virtual int _PixelPipeline		(_in_ B3YXVECTOR3 oPos[3], _in_  B3YXCOLOR oColor[3]);

	virtual int _PrimitiveAssembly	(_in_ VSOUTS& vsouts, _out_ B3YXVECTOR4 oPos[3], _out_ B3YXCOLOR oColor[3]);
	virtual int _PixelPipeline1		();
	virtual int _PixelPipeline2		();
		

	virtual bool _FaceCulling2		(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2);
	virtual bool _FaceCulling3		(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXVECTOR2 v2);
	virtual int  _Viewport			(B3YXVECTOR3 iPos[3], B3YXVECTOR2 oPos[3] );


	
	virtual VS_OUTPUT _TnL_VP	(B3YXVECTOR3 pos, B3YXCOLOR col);
	virtual int		  _TnL_GP	(_in_ B3YXVECTOR4 pos[3], _out_ B3YXVECTOR3 oPos[3], _inout_ B3YXCOLOR col[3]);
	virtual int		  _TnL_PP	(_in_ B3YXVECTOR2 pos[3], _in_ B3YXCOLOR col[3]);
	

	
	virtual void _InitRenderState	();
	virtual int  _SetRenderState	(B3YRENDERSTATETYPE State, DWORD Value);
	virtual int  _GetRenderState	(B3YRENDERSTATETYPE State, DWORD* pValue);

	
	virtual int _SetTransform		(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* mTM);
	virtual int _GetTransform		(B3YTRANSFORMSTATETYPE ts, B3YXMATRIX* mTM);
		
	
	virtual int	 _Line	(B3YXVECTOR2& v0, B3YXVECTOR2& v1, B3YXCOLOR& col);		
	virtual int  _Line	(B3YXVECTOR2 v0, B3YXVECTOR2 v1, B3YXCOLOR sc0, B3YXCOLOR ec0);
	virtual int  _Face	(B3YXVECTOR2 pos[3], B3YXCOLOR  col[3]);
	float _EdgeFunction(const B3YXVECTOR2& v0, const B3YXVECTOR2& v1, const B3YXVECTOR2& p);
	virtual void _HLine	(HDC hdc, int x1, int x2, int y, B3YXCOLOR c0, B3YXCOLOR c1);
  
	
	virtual void _SetPixel(int x, int y, B3YXCOLOR color);

	
	
public:
	B3YenaGraphicsEngine9(void);
	virtual ~B3YenaGraphicsEngine9(void);
		
	virtual int PreDraw	(B3YPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount); 
	virtual int Draw	(); 
	virtual int PostDraw(); 
 
	virtual int Reset	();		//!< 렌더링 엔진 리셋.
	virtual int Release ();		//!< 렌더링 엔진 자원 해제.

	virtual void SetDev	(B3YenaDevice9* pDev) { m_pDev = pDev; }

};

typedef B3YenaGraphicsEngine9		B3YenaGraphics;				
typedef B3YenaGraphicsEngine9*		LPB3YENAGRAPHICSENGINE9;	
typedef LPB3YENAGRAPHICSENGINE9		LPGRAPHICS;