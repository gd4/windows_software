#include "stdafx.h"
#include "Plot.h"

////////////////////////////////////////////////////////////////////////
//�ļ���  :MULTICOLORPLOT.CPP
//�ؼ�˵��:ʵʱ������ʾ���ܿؼ�,�ṩLINE��BAR����������ʾ����
//����:    �ݸ�
//�������:2007-01-17
//��Ȩ    :��������ʹ�ô���
///////////////////////////////////////////////////////////////////////

#include "stdafx.h" 
#include "MultiColorPlotCtrl.h"
//#include ".\multicolorplotctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 
/////////////////////////////////////////////////////////////////////////////
// CPlot
CPlot::CPlot(void)
	: nPlotType(LINE) 
	, nGridResolutionX(10)
	, nGridResolutionY(10)
	, nGridScrollSpeedX(-2)
	, nGridScrollSpeedY(0)
	, nPlotGranulatrity(2)
	, nGridLineWidth(1)
	, m_clrDarkBack(RGB(0,0,75))
	, m_clrDarkLine(RGB(32,64,32))
	, nPlotData(0)
	, pfData(NULL)
	, fLow(0.0)
	, fHigh(100.0)
	, fScaleY(1.0)
	, m_clrCyanData(RGB(0,255,255))
	, nGridStarPosX(0)
	, nGridStarPosY(0)
	, bLock(true)
	, m_clrAxisScaleY(RGB(0,255,255))
	, nShowFormatDataText(0)
	, m_clrLinePen(RGB(0,255,0))
	, nLineWidth(1)
{
	// �ؼ�����
	// ��ʼ���ؼ������ C_S �ṹ
	InitializeCriticalSection ( & g_cs ) ;
	
	// ��ʼ������
	sprintf_s ( szTitls , _TEXT ( "%s" ) , _TEXT ( "" ) ) ; 
	// ��ʼ����λ
	sprintf_s ( szUints , _TEXT ( "%s" ) , _TEXT ( "" ) ) ;

	// ���ÿؼ�����ʾ�����ֵ�����ʹ�С
	m_SmallFont.CreateFont( -11 , 0 , 0 , 0 , FW_THIN , false , false , false , DEFAULT_CHARSET ,
		OUT_DEFAULT_PRECIS , CLIP_DEFAULT_PRECIS , DEFAULT_QUALITY , VARIABLE_PITCH , _TEXT( "Times New Roman" ) ) ;
	m_GridPen.CreatePen ( PS_SOLID , nGridLineWidth , m_clrDarkLine ) ;
}
CPlot::~CPlot(void)
{
	if ( pfData )
	{
		delete [] pfData ;
	}
	
	// �ͷŹؼ�����
	DeleteCriticalSection ( & g_cs ) ;
}


BEGIN_MESSAGE_MAP(CPlot, CStatic)
	//{{AFX_MSG_MAP(CPlot)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlot message handlers

void CPlot::OnPaint() 
{
	CPaintDC dc(this); // device context for painting  

	// ��ÿؼ�����
	GetClientRect (&m_rectCtrl);
	// �����ڴ� DC
	CMyMemDC *pMemDC = new CMyMemDC(&dc, &m_rectCtrl);

	CPoint orgBrushOrigin = pMemDC->GetBrushOrg () ;

	if(m_dcBackground.GetSafeHdc() == NULL || m_pBitmapBackground.m_hObject == NULL)
	{
		m_dcBackground.CreateCompatibleDC(&dc);
		m_pBitmapBackground.CreateCompatibleBitmap(&dc, m_rectCtrl.Width(),m_rectCtrl.Height()) ;
		m_pBitmapOldBackground = m_dcBackground.SelectObject(&m_pBitmapBackground) ;
		InitColorPlot(&m_dcBackground);
	}

	pMemDC->BitBlt(0, 0, m_rectCtrl.Width(), m_rectCtrl.Height(), 
		&m_dcBackground, 0, 0, SRCCOPY) ; 
	 
	//�����߱���������
	DrawBackGroundGrid(pMemDC); 
	//����������
	DrawValue(pMemDC);
	//������Y�̶�
	DrawAxisScaleYValue(pMemDC);		

	pMemDC->SetBrushOrg ( orgBrushOrigin.x , orgBrushOrigin.y ) ;  
	delete pMemDC ;
}

// ���û��ߵ�����
void CPlot::SetPlotType(int nType)
{
	nPlotType = nType ;	 
}

// ���ݵ������С������һ�����ݵ�ռ�ݵ���������
BOOL CPlot::SetPlotGranulatrity(int nPlotGrltiy)
{
	nPlotGranulatrity = nPlotGrltiy ;
	nPlotData = m_Size.cx / nPlotGranulatrity ;
	pfData = new float [nPlotData] ;
	if ( pfData )
	{
		// ��ʼ������Ϊ 0 
		ZeroMemory ( pfData , sizeof ( pfData ) * nPlotData ) ;			
		return TRUE ;
	}		
	return FALSE ;
}

// ǰ��ɫ
void CPlot::SetGridLineClr(COLORREF clr)
{
	m_clrDarkLine = clr ;
	m_GridPen.CreatePen ( PS_SOLID , nGridLineWidth , m_clrDarkLine ) ;		
	m_clrDarkLine = clr ; 
}

// ���ݷ�Χ
void CPlot::SetRang(float fL, float fH)
{
	fLow = fL ;
	fHigh = fH ;
}

// ��������
void CPlot::SetLinePen(int nWidth, COLORREF clr)
{
	nGridLineWidth = nWidth ;
	m_clrDarkLine = clr ;
	m_GridPen.CreatePen ( PS_SOLID , nGridLineWidth , m_clrDarkLine ) ;
}

// Y��̶���ɫ
void CPlot::SetAxisScaleClrY(COLORREF clr)
{
	m_clrAxisScaleY = clr ; 
}

// ����
void CPlot::SetTitle(LPCTSTR pctTitle)
{
	sprintf_s ( szTitls , _TEXT ( "%s" ) , pctTitle ) ;
}

// ���ݵ�λ����SetUnit( _TEXT ( "(k/sec)" ) ) ;
void CPlot::SetUnit(LPCTSTR pctUint)
{
	sprintf_s ( szUints , _TEXT ( "%s" ) , pctUint ) ;
}

// �Ƿ���ʾ���ݱ��⡢��ɫ����λ
/*
nShow = 0 ; ����ʾ�κ��ı�
nShow = 1 ; ������ʾ����
nShow = 2 ; ������ʾ����������ܼ�
nShow = 3 ; ��ʾ�������Сֵ
nShow = 4 ; ��ʾ���⡢��Сֵ����ֵ
*/
void CPlot::ShowTitle(int nShow)
{
	nShowFormatDataText = nShow ;
}

// BAR ��ɫ
// clrUp -------- ������ɫ
// clrDown -------- �ײ���ɫ
// bfRfastness  -------- ��ɫ�����Ƿ�̶�����  false ---- ����
// bfGfastness  -------- ��ɫ�����Ƿ�̶�����  false ---- ����
// bfBfastness  -------- ��ɫ�����Ƿ�̶�����  false ---- ����
void CPlot::SetBarColor(COLORREF clrUp, COLORREF clrDown, bool bfRfastness, bool bfGfastness, bool bfBfastness)
{

}

// ������ɫ
void CPlot::SetLineColorWidth(COLORREF clrLine, int nWidth)
{
	nLineWidth = nWidth ;
	m_clrLinePen = clrLine ;
}

void CPlot::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	// ��Ӧͨ����Ϣ
	ModifyStyle( 0 , SS_NOTIFY ) ;

	GetClientRect ( & m_rectCtrl ) ;

	// �õ��ÿؼ��Ŀ��͸�
	m_Size.cx = m_rectCtrl.right - m_rectCtrl.left ;
	m_Size.cy = m_rectCtrl.bottom - m_rectCtrl.top ;

	// ����ؼ������ɿɼ������ݵ���
	nPlotData = m_Size.cx / nPlotGranulatrity ;

	
	// ��ʵ�����ݷ����ڴ�
	pfData = new float [ nPlotData ] ;
	if ( pfData )
	{
		// ��ʼ������Ϊ 0 
		ZeroMemory ( pfData , sizeof ( pfData ) * nPlotData ) ;

		CRgn m_Rgn ;
		m_Rgn.CreateRoundRectRgn ( 0 , 0 , m_Size.cx , m_Size.cy , 10 , 10 ) ;

		SetWindowRgn ( ( HRGN ) m_Rgn , true ) ; 

		SetTimer ( GRID_TIMER , GRID_UPDATE_SPEED , NULL ) ;
	}
	CStatic::PreSubclassWindow();
}

BOOL CPlot::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	return TRUE;
}

void CPlot::ReconstructControl(void)
{
}

void CPlot::InitColorPlot(CDC *pDC)
{
	// ���ݲ�ͬ�����߷ֱ����
	if ( nPlotType == BAR ) 
	{
		double factor = 255.0 / ( float ) m_Size.cy ;

		BYTE r , g , b ;

		for ( int x = 0 ; x < m_Size.cy ; x ++ )
		{
			g = ( BYTE ) ( 255 - factor * x ) ;
			r = ( BYTE ) ( factor * x ) ;
			b = ( BYTE ) 64 ;

			pDC->SetPixelV ( 0 , x , RGB ( r , g , b ) ) ;
			pDC->SetPixelV ( 1 , x , RGB ( r , g , b ) ) ;
		}
	}
	else if ( nPlotType == LINE )
	{
	}
	else
	{
	} 

	pDC->SelectObject ( m_pBitmapOldBackground ) ;

	// ����λͼ��ˢ
	m_clrBrush.CreatePatternBrush ( & m_pBitmapBackground ) ;
}

//��������ͱ���
void CPlot::DrawBackGroundGrid(CDC * pDC)
{
	// ��䱳��ɫ
	pDC->FillSolidRect ( & m_rectCtrl , m_clrDarkBack ) ;

	// ������
	int nGridLinesX = m_Size.cx / nGridResolutionX ;
	int nGridLinesY = m_Size.cy / nGridResolutionY ;

	// ѡ�񻭱�
	CPen * pOldPen = pDC->SelectObject ( & m_GridPen ) ;

	// ������ֱ��
	for ( int x = 0 ; x <= nGridLinesX ; x ++ )
	{
		pDC->MoveTo ( x * nGridResolutionX + nGridStarPosX , 0 );
		pDC->LineTo ( x * nGridResolutionX + nGridStarPosX , m_Size.cy );
	}
	// ����ˮƽ��
	for ( int y = 0 ; y <= nGridLinesY ; y ++ )
	{
		pDC->MoveTo ( 0 , nGridStarPosY + m_Size.cy - y * nGridResolutionY - 2 ) ;
		pDC->LineTo ( m_Size.cx , nGridStarPosY + m_Size.cy - y * nGridResolutionY - 2 ) ;
	}
	// ����������ȷ�ƶ�
	
	nGridStarPosX += nGridScrollSpeedX ;
	nGridStarPosY += nGridScrollSpeedY ;

	if ( nGridStarPosX < 0 ) nGridStarPosX = nGridResolutionX ;
	if ( nGridStarPosX > nGridResolutionX ) nGridStarPosX = 0 ;
	if ( nGridStarPosY < 0 ) nGridStarPosY = nGridResolutionY ;
	if ( nGridStarPosY > nGridResolutionY ) nGridStarPosY = 0 ;
	
	// ��ԭ���񻭱�
	pDC->SelectObject ( pOldPen ) ;
}

void CPlot::DrawValue(CDC * pDC)
{
	float fx , fy ;

	// �ùؼ�����ͬ����SetData
	EnterCriticalSection ( & g_cs ) ;

	if ( nPlotType == BAR )
	{
		RECT rFill ;

		for ( int nData = 0 ; nData < nPlotData ; nData ++ )
		{
			fx = ( float ) ( m_rectCtrl.left + nData * nPlotGranulatrity ) ;
			fy = ( float ) fabs ( ( float ) ( m_rectCtrl.bottom - ( ( ( ( pfData[nData] - fLow ) / ( fHigh - fLow ) ) * m_Size.cy ) ) ) ) ;

			rFill.bottom = ( LONG ) m_rectCtrl.bottom ;
			rFill.top = ( LONG ) fy ;
			rFill.left = ( LONG ) fx ;
			rFill.right = ( LONG ) ( fx + nPlotGranulatrity ) ;

			pDC->SetBrushOrg ( ( int ) fx , m_rectCtrl.bottom ) ;

			// �ó�ʼ����ˢʱ���ɵĽ���λͼ��ˢ������
			pDC->FillRect ( & rFill , & m_clrBrush ) ;
			// �����ݵ㴦����ɫ
			pDC->SetPixelV ( ( int ) fx , ( int ) fy , m_clrCyanData ) ;
		}
	}
	else if ( nPlotType == LINE )
	{
		CPoint * g_DataPoint = new CPoint [nPlotData] ;

		// �������߻���
		CPen m_Pen ;
		m_Pen.CreatePen ( PS_SOLID , nLineWidth , m_clrLinePen ) ;
		CPen * m_pOldPen = pDC->SelectObject ( & m_Pen ) ;

		for ( int nData = 0 ; nData < nPlotData ; nData ++ )
		{
			g_DataPoint[nData].x  =  ( LONG ) ( m_rectCtrl.left + nData * nPlotGranulatrity ) ;
			g_DataPoint[nData].y  =  ( LONG ) fabs ( ( float ) ( m_rectCtrl.bottom - ( ( ( ( pfData[nData] - fLow ) / ( fHigh - fLow ) ) * m_Size.cy ) ) ) ) ;
		}

		pDC->Polyline ( g_DataPoint , nPlotData ) ;

		pDC->SelectObject ( m_pOldPen ) ;

		delete [] g_DataPoint ;
	}
	else
	{
	}
}

void CPlot::DrawAxisScaleYValue(CDC * pDC)
{
	CFont * pOldFont ;
	// ����Y��̶�
	TCHAR szAxisScaleYMax [MAX_PATH * sizeof ( TCHAR ) + 1] ;
	TCHAR szAxisScaleYMin [MAX_PATH * sizeof ( TCHAR ) + 1] ;
	TCHAR szAxisScaleY  [MAX_PATH * sizeof ( TCHAR ) + 1] ;

	ZeroMemory ( & szAxisScaleYMax , sizeof ( szAxisScaleYMax ) ) ;
	ZeroMemory ( & szAxisScaleYMin , sizeof ( szAxisScaleYMin ) ) ;
	ZeroMemory ( & szAxisScaleY , sizeof ( szAxisScaleY ) ) ;

	COLORREF clrText = pDC->GetTextColor () ;
	int nBkMode = pDC->GetBkMode () ;
	pDC->SetTextColor ( m_clrAxisScaleY ) ;
	pDC->SetBkMode ( TRANSPARENT ) ;

	pOldFont = pDC->SelectObject ( & m_SmallFont ) ;

	// ���Ҫ����ʵY��̶�����
	switch ( nShowFormatDataText )
	{
	case 0 : // ����ʾ
		{
		} 
		break ;
	case 1 : // ����ʾ����
		{
			sprintf_s ( szAxisScaleYMax , _TEXT ( "%s" ) , szTitls ) ;

			pDC->TextOut ( 0 ,  0 , szAxisScaleYMax ) ;
		} 
		break ;
	case 2 : // ��ʾ�������������
		{
			sprintf_s ( szAxisScaleYMax , _TEXT ( "%.3f    %s %.3f %s" ) ,fHigh,szTitls , pfData [ nPlotData - 1 ] , szUints ) ;

			pDC->TextOut ( 0 ,  0 , szAxisScaleYMax ) ;
		} 
		break ;
	case 3 : // ��ʾ�����Сֵ
		{
			// ��ʽ�����ֵ�ͱ��⼰��λ
			sprintf_s ( szAxisScaleYMax , _TEXT ( "%.3f    %s %8.3f %s" )  ,fHigh,szTitls,pfData [ nPlotData - 1 ] , szUints ) ;
			// ��ʽ����Сֵ
			sprintf_s ( szAxisScaleYMin , _TEXT ( "%.3f" ) , fLow ) ;

			pDC->TextOut ( 0 ,  0 , szAxisScaleYMax ) ;
			pDC->TextOut ( 0 , m_Size.cy - 15 , szAxisScaleYMin );
		} 
		break ;
	case 4 : // ��ʾȫ��
		{
			// ��ʽ�����ֵ�ͱ��⼰��λ
			sprintf_s ( szAxisScaleYMax , _TEXT ( "%.3f    %s: %.3f %s" ) , fHigh,szTitls,pfData [ nPlotData - 1 ] , szUints ) ;
			// ��ʽ����Сֵ
			sprintf_s ( szAxisScaleYMin , _TEXT ( "%.3f" ) , fLow ) ;
			// ��ʽ����ֵ
			sprintf_s ( szAxisScaleY , _TEXT ( "%.3f" ) , ( ( fHigh - fLow ) / 2.0 + fLow ) ) ;


			// ����Y��̶�
			pDC->TextOut ( 0 ,  0 , szAxisScaleYMax ) ;
			pDC->TextOut ( 0 , m_Size.cy - 15 , szAxisScaleYMin ) ;
			pDC->TextOut ( 0 , m_Size.cy /  2 , szAxisScaleY ) ;
		} break ;

	}


	pDC->SetTextColor ( clrText ) ;
	pDC->SetBkMode ( nBkMode ) ;
	pDC->SelectObject ( pOldFont ) ;

	// �뿪�ؼ�����
	LeaveCriticalSection ( & g_cs ) ;
}

void CPlot::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	switch ( nIDEvent )
	{
	case GRID_TIMER :
		{
		} 
		break ;
	}
	Invalidate ( false ) ;
	CStatic::OnTimer(nIDEvent);
}

// ����������
void CPlot::SetGridResolutionX(int nGridReluX)
{
	nGridResolutionX = nGridReluX ;
}

// ����������
void CPlot::SetGridResolutionY(int nGridReluY)
{
	nGridResolutionY = nGridReluY ;
}

// ������������ٶ�,��ֵΪ�������ҹ���,0����
void CPlot::SetGridScrollSpeedX(int nSpeedX)
{
	nGridScrollSpeedX = nSpeedX ; 
}

// ��ֵΪ���ϵ��¹���,0����
void CPlot::SetGridScrollSpeedY(int nSpeedY)
{
	nGridScrollSpeedY = nSpeedY ;
}

// �����߿���
void CPlot::SetGridLineWidth(int nWidth)
{
	nGridLineWidth = nWidth ;
}

// ����ɫ
void CPlot::SetGridBackClr(COLORREF clr)
{
	m_clrDarkBack = clr ;
}

// // ����������ʾ��Χ
void CPlot::LockRang(bool bfLock)
{
	bLock = bfLock ;
}

// ����������ʾ��Χ
void CPlot::LockRang(float fMin, float fMax)
{
	fLow = fMin ;
	fHigh = fMax ;	
	LockRang ( true ) ;
}

// ��������
void CPlot::SetData(float fData)
{
	// �ùؼ�����ͬ��
	EnterCriticalSection ( & g_cs ) ;
	
	for ( int n = 0 ; n < nPlotData - 1 ; n ++ )
	{
		pfData [ n ] = pfData [ n + 1 ] ;
	}
	
	pfData [ nPlotData - 1 ] = fData ;
	
	if ( bLock ) // ����������Χ
	{
	}
	else
	{
		// ������Сֵ
		if ( fLow > fData )
		{
			fLow = fData ;
		}
		// �������ֵ
		if ( fHigh < fData )
		{
			fHigh = fData ;
		}
	}
	
	// �뿪�ؼ�����
	LeaveCriticalSection ( & g_cs ) ;
}

// ������ɫ
void CPlot::SetLineColor(COLORREF clrLine)
{
	m_clrLinePen = clrLine ;
}

void CPlot::SetLineWidth(int nWidth)
{
	nLineWidth = nWidth ;
}

const COLORREF CPlot::GetLineColor(void)
{
	return m_clrLinePen ;
}

const int CPlot::GetLineWidth(void)
{
	return nLineWidth ;
}