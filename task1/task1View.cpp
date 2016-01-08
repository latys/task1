
// task1View.cpp : Ctask1View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "task1.h"
#endif

#include "task1Doc.h"
#include "task1View.h"
#include "Bmp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctask1View

IMPLEMENT_DYNCREATE(Ctask1View, CView)

BEGIN_MESSAGE_MAP(Ctask1View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &Ctask1View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &Ctask1View::OnFileSave)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_ADAPTSCREEN, &Ctask1View::OnAdaptScreen)
	ON_COMMAND(ID_ORIGINSIZE, &Ctask1View::OnOriginSize)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Ctask1View 构造/析构
CBmp g_cbmp;
float g_fZScaling=1;         //记录鼠标滚轮
CPoint g_ptMousePoint;       //记录鼠标位置
float g_fXPositionInDC;      //鼠标按下时获取当前图片在DC中的位置 
float g_fYPositionInDC;
float g_fXoffset=0;            //鼠标拖动后X轴偏移量
float g_fYoffset=0;            //鼠标拖动后Y轴偏移量
int g_nwheelFlag=0;           //鼠标滚轮转动标志，0：未转动。 1：转动
int g_nLButtonDownFlag=0;     //鼠标左键按下标志位
int g_nMouseMoveFlag=0;       //鼠标拖动标志位


Ctask1View::Ctask1View()
{
	// TODO: 在此处添加构造代码

}

Ctask1View::~Ctask1View()
{
}

BOOL Ctask1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);

}

// Ctask1View 绘制

void Ctask1View::OnDraw(CDC* pDc/*pDC*/)
{
	Ctask1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect rect;
	GetClientRect(&rect);
	CBrush cBrush;
	cBrush.CreateSolidBrush(RGB(255,255,255));
	pDc->FillRect(rect,&cBrush);
	
	if (!g_nwheelFlag&&!g_nMouseMoveFlag)               //鼠标滚轮未转动实现等比显示
	{
		CRect m_rc;
		CRect m_PicRc;             //图片等比缩放区域
		GetClientRect(&m_rc);
		float m_fRate=float(g_cbmp.m_bmpInfoHeader.biWidth)/float(g_cbmp.m_bmpInfoHeader.biHeight);
		
		float m_fRcRate=float(m_rc.Width())/float(m_rc.Height());


		if (m_fRate>=m_fRcRate)
		{

			g_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC, m_rc.Width(),int(m_rc.Width()/m_fRate));
			g_fZScaling=float(m_rc.Width())/g_cbmp.m_bmpInfoHeader.biWidth;
		}
		if (m_fRate<m_fRcRate)
		{

			g_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC,int(m_rc.Height()*m_fRate),m_rc.Height());
			g_fZScaling=float(m_rc.Height())/g_cbmp.m_bmpInfoHeader.biHeight;
		}
	}
	else  if(g_nwheelFlag==1)                         //鼠标滚轮转动实现放大缩小
	{
		 g_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC, 
			               int(g_cbmp.m_bmpInfoHeader.biWidth*g_fZScaling),
			               int(g_cbmp.m_bmpInfoHeader.biHeight*g_fZScaling)
						   );
		 g_nwheelFlag=0;
	}
	if (g_nMouseMoveFlag==1)
	{
		int m_nXPosition;    //起始点X轴坐标                                                        
		int m_nYPosition;    //起始点Y轴坐标

		CRect m_rc;
		GetClientRect(&m_rc);

		m_nXPosition=g_fXPositionInDC+g_fXoffset;
		m_nYPosition=g_fYPositionInDC+g_fYoffset;

		g_cbmp.Draw(pDc->m_hDC,
			        m_nXPosition,
					m_nYPosition,
			        int(g_cbmp.m_bmpInfoHeader.biWidth*g_fZScaling), 
					int(g_cbmp.m_bmpInfoHeader.biHeight*g_fZScaling)
					);
		g_nMouseMoveFlag=0;
	}

	


	// TODO: 在此处为本机数据添加绘制代码
}
void Ctask1View::GetPicRect(CImage img, CRect wdRect, CRect & rect)
{
	if (double(img.GetWidth())/double(img.GetHeight())>=double(wdRect.Width())/double(wdRect.Height()))
	{
//		rect.SetRect(0,0,wdRect.Width(),img.GetHeight()/(img.GetHeight()/))
	}

}
void Ctask1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Ctask1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Ctask1View 诊断

#ifdef _DEBUG
void Ctask1View::AssertValid() const
{
	CView::AssertValid();
}

void Ctask1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ctask1Doc* Ctask1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctask1Doc)));
	return (Ctask1Doc*)m_pDocument;
}
#endif //_DEBUG


// Ctask1View 消息处理程序


void Ctask1View::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	
	HRESULT m_hResult;
	m_lpbImageDataSrc.Destroy();
	CFileDialog m_dlg(0,0,0,0,0,NULL);
	if (m_dlg.DoModal()==IDOK)
	{
		CString m_str=m_dlg.GetPathName();
	
		if (!g_cbmp.IsNull())
		{
			g_cbmp.ReleaseBmp();
		}
		g_cbmp.LoadBmp(m_str);
	
	}
	Invalidate(false); 

	
}


void Ctask1View::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	CString m_filter="*.jpg|*.png|*.bmp";
	CFileDialog m_dlg(0,0,0,0,m_filter,0);
	if(m_dlg.DoModal()==IDOK)
	{
		CString m_str=m_dlg.GetPathName();
		g_cbmp.SaveBmp(m_str);

	}
	
}


BOOL Ctask1View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	 g_nwheelFlag=1;
	 
	 g_fZScaling+= 0.05*(zDelta/120); 

	 if (g_fZScaling>=2)
	 {
		 g_fZScaling=2;
	 }
	 if (g_fZScaling<=0.1)
	 {
		 g_fZScaling=0.1;
	 }


	 {
		 Invalidate(false);
	 }
	 return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void Ctask1View::OnAdaptScreen()
{
	// TODO: 在此添加命令处理程序代码
	
	Invalidate(false);
}


void Ctask1View::OnOriginSize()
{
	// TODO: 在此添加命令处理程序代码
	CClientDC dc(this);
	g_cbmp.DrawCenter(this->m_hWnd,dc.m_hDC,g_cbmp.m_bmpInfoHeader.biWidth, g_cbmp.m_bmpInfoHeader.biHeight);
	g_fZScaling=1;
}


int Ctask1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*RECT rect = {0,0,20,500};
	m_cScrollBar.Create(WS_CHILD | WS_VISIBLE | SB_VERT,rect,this,2);//WS_VISIBLE一定要加上，不然你就看不到m_scrollbar了
	m_cScrollBar.SetScrollRange(0,1);
	m_cScrollBar.SetScrollPos(50);
	// TODO:  在此添加您专用的创建代码*/

	return 0;
}


void Ctask1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (g_nLButtonDownFlag==1)
	{
		g_fXoffset=point.x-g_ptMousePoint.x;
		g_fYoffset=point.y-g_ptMousePoint.y;
		g_nMouseMoveFlag=1;
		Invalidate(false);
	}
	CView::OnMouseMove(nFlags, point);
}


void Ctask1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_nLButtonDownFlag=0;
	CView::OnLButtonUp(nFlags, point);
}


void Ctask1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	g_nLButtonDownFlag=1;
	g_ptMousePoint=point;
	g_fXPositionInDC=g_cbmp.get_m_fXPositionInDC();
	g_fYPositionInDC=g_cbmp.get_m_fYPositionInDC();
	CView::OnLButtonDown(nFlags, point);
}


BOOL Ctask1View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CView::OnEraseBkgnd(pDC);
	//return TRUE;
}
