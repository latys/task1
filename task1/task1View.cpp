
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
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctask1View

IMPLEMENT_DYNCREATE(Ctask1View, CView)

BEGIN_MESSAGE_MAP(Ctask1View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

	ON_WM_CREATE()

	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// Ctask1View 构造/析构



Ctask1View::Ctask1View()
{
	// TODO: 在此处添加构造代码
	

	

}

Ctask1View::~Ctask1View()
{
	//m_cbmp.ReleaseBmp();
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
	//pDc->FillRect(rect,&cBrush);
	
	if (!pDoc->m_nwheelFlag&&!pDoc->m_nMouseMoveFlag)               //鼠标滚轮未转动实现等比显示
	{
		CRect m_rc;
		GetClientRect(&m_rc);
		float m_fRate=float(pDoc->m_cbmp.m_bmpInfoHeader.biWidth)/float(pDoc->m_cbmp.m_bmpInfoHeader.biHeight);
		
		float m_fRcRate=float(m_rc.Width())/float(m_rc.Height());


		if (m_fRate>=m_fRcRate)                       //判断图像长宽比与窗口长宽比，图片宽度与窗口宽度一样
		{

			pDoc->m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC, m_rc.Width(),int(m_rc.Width()/m_fRate));
			
			//刷新窗口，清除图片上方和下方区域
			CRect m_rcFresh(0,0,m_rc.Width(),(int)pDoc->m_cbmp.get_m_fYPositionInDC());
			//m_rcFresh.SubtractRect(m_rc,m_PicRc);
			pDc->FillRect(m_rcFresh,&cBrush);
			CRect m_rcFresh1(0,(int)pDoc->m_cbmp.get_m_fYPositionInDC()+int(m_rc.Width()/m_fRate),m_rc.Width(),m_rc.Height());
			//m_rcFresh.SubtractRect(m_rc,m_PicRc);
			pDc->FillRect(m_rcFresh1,&cBrush);

			pDoc->m_fZScaling=float(m_rc.Width())/pDoc->m_cbmp.m_bmpInfoHeader.biWidth;
		}
		if (m_fRate<m_fRcRate)                        //图片高度与窗口高度一致
		{

			pDoc->m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC,int(m_rc.Height()*m_fRate),m_rc.Height());
			//刷新窗口，清除图片左边与右边区域
			CRect m_rcFresh(0,0,(int)pDoc->m_cbmp.get_m_fXPositionInDC(),m_rc.Height());
			
			pDc->FillRect(m_rcFresh,&cBrush);

			CRect m_rcFresh1((int)pDoc->m_cbmp.get_m_fXPositionInDC()+int(m_rc.Height()*m_fRate),0,m_rc.Width(),m_rc.Height());

			pDc->FillRect(m_rcFresh1,&cBrush);

			pDoc->m_fZScaling=float(m_rc.Height())/pDoc->m_cbmp.m_bmpInfoHeader.biHeight;
		}
	}
	else  if(pDoc->m_nwheelFlag==1)                         //鼠标滚轮转动实现放大缩小
	{
		int m_nXPosition;    //起始点X轴坐标                                                        
		int m_nYPosition;    //起始点Y轴坐标
		CRect m_rc;
		GetClientRect(&m_rc);
		
		m_nXPosition=(m_rc.Width()-int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling))/2;
		m_nYPosition=(m_rc.Height()-int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling))/2;

		if (m_nXPosition>0)                              //图片起始点横坐标在窗口内，清除图片左右边区域
		{
			CRect m_rcLeft(0,0,m_nXPosition,m_rc.Height());
			pDc->FillRect(&m_rcLeft,&cBrush);
			CRect m_rcRight(m_nXPosition+(int)(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
				            0,m_rc.Width(),m_rc.Height());
			pDc->FillRect(&m_rcRight,&cBrush);
		}
		if (m_nYPosition>0)                             //图片起始点纵坐标在窗口内，清除图片上下方区域
		{
			CRect m_rcTop(0,0,m_rc.Width(),m_nYPosition);
			pDc->FillRect(&m_rcTop,&cBrush);
			CRect m_rcBottom(0,m_nYPosition+(int)(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling),
				             m_rc.Width(),m_rc.Height());
			pDc->FillRect(&m_rcBottom,&cBrush);
		}

		 pDoc->m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC, 
			               int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
			               int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling)
						   );
		 pDoc->m_nwheelFlag=0;
	}
	if (pDoc->m_nMouseMoveFlag==1)                     //实现鼠标拖动
	{
		int m_nXPosition;    //起始点X轴坐标                                                        
		int m_nYPosition;    //起始点Y轴坐标

		CRect m_rc;
		GetClientRect(&m_rc);

		m_nXPosition=pDoc->m_fXPositionInDC+pDoc->m_fXoffset;
		m_nYPosition=pDoc->m_fYPositionInDC+pDoc->m_fYoffset;



		if (m_nXPosition>pDoc->m_cbmp.get_m_fXPositionInDC())                       //向右移动则清除左边区域
		{
			CRect m_rcRefresh((int)pDoc->m_cbmp.get_m_fXPositionInDC(),
				(int)pDoc->m_cbmp.get_m_fYPositionInDC(),
				m_nXPosition,
				(int)pDoc->m_cbmp.get_m_fYPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling));
		
			pDc->FillRect(&m_rcRefresh,&cBrush);
		}
		if (m_nXPosition<pDoc->m_cbmp.get_m_fXPositionInDC())                        //向左移动则清除右边区域
		{
			CRect m_rcRefresh(m_nXPosition+int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
				(int)pDoc->m_cbmp.get_m_fYPositionInDC(),
				(int)pDoc->m_cbmp.get_m_fXPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
				(int)pDoc->m_cbmp.get_m_fYPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling));

			pDc->FillRect(&m_rcRefresh,&cBrush);
			
		}
		if (m_nYPosition>pDoc->m_cbmp.get_m_fYPositionInDC())                      //向下移动则清除上边区域
		{

			CRect m_rcRefresh((int)pDoc->m_cbmp.get_m_fXPositionInDC(),
				(int)pDoc->m_cbmp.get_m_fYPositionInDC(),
				(int)pDoc->m_cbmp.get_m_fXPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
				m_nYPosition);

			pDc->FillRect(&m_rcRefresh,&cBrush);

		}
		if (m_nYPosition<pDoc->m_cbmp.get_m_fYPositionInDC())                       //向上移动则清除下边区域
		{

			CRect m_rcRefresh((int)pDoc->m_cbmp.get_m_fXPositionInDC(),
				m_nYPosition+int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling),
				(int)pDoc->m_cbmp.get_m_fXPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
				(int)pDoc->m_cbmp.get_m_fYPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling));

			pDc->FillRect(&m_rcRefresh,&cBrush);

		}
	
		
		pDoc->m_cbmp.Draw(pDc->m_hDC,
			m_nXPosition,
			m_nYPosition,
			int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling), 
			int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling)
			);

		pDoc->m_nMouseMoveFlag=0;
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





BOOL Ctask1View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}


void Ctask1View::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	
	CView::OnClose();
}


void Ctask1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Ctask1Doc *pDoc=(Ctask1Doc *)(GetDocument());
	pDoc->m_nLButtonDownFlag=1;
	pDoc->m_ptMousePoint=point;
	pDoc->m_fXPositionInDC=pDoc->m_cbmp.get_m_fXPositionInDC();
	pDoc->m_fYPositionInDC=pDoc->m_cbmp.get_m_fYPositionInDC();

	CView::OnLButtonDown(nFlags, point);
}


void Ctask1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Ctask1Doc *pDoc=(Ctask1Doc *)(GetDocument());
	pDoc->m_nLButtonDownFlag=0;
	pDoc->m_nLButtonUpFlag=1;
	CView::OnLButtonUp(nFlags, point);
}


void Ctask1View::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	CView::OnMouseHWheel(nFlags, zDelta, pt);
}


void Ctask1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Ctask1Doc *pDoc=(Ctask1Doc *)(GetDocument());
	if (pDoc->m_nLButtonDownFlag==1)
	{
		pDoc->m_fXoffset=point.x-pDoc->m_ptMousePoint.x;
		pDoc->m_fYoffset=point.y-pDoc->m_ptMousePoint.y;
		pDoc->m_nMouseMoveFlag=1;
		Invalidate(false);
		UpdateWindow();
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL Ctask1View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Ctask1Doc *pDoc=(Ctask1Doc *)(GetDocument());

	pDoc->m_nwheelFlag=1;

	pDoc->m_fZScaling+= 0.05*(zDelta/120); 

	if (pDoc->m_fZScaling>=2)
	{
		pDoc->m_fZScaling=2;
	}
	if (pDoc->m_fZScaling<=0.1)
	{
		pDoc->m_fZScaling=0.1;
	}

	Invalidate();
	UpdateWindow();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
