
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
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// Ctask1View 构造/析构



Ctask1View::Ctask1View()
{
	// TODO: 在此处添加构造代码
	m_fZScaling=1;         //记录鼠标滚轮
	m_ptMousePoint=(0,0);       //记录鼠标位置
	m_fXPositionInDC=0;      //鼠标按下时获取当前图片在DC中的位置 
	m_fYPositionInDC=0;
	m_fXoffset=0;            //鼠标拖动后X轴偏移量
	m_fYoffset=0;            //鼠标拖动后Y轴偏移量
	m_nwheelFlag=0;           //鼠标滚轮转动标志，0：未转动。 1：转动
	m_nLButtonDownFlag=0;     //鼠标左键按下标志位
    m_nMouseMoveFlag=0;       //鼠标拖动标志位
	m_nLButtonUpFlag=0;       //鼠标左键松开标志位
	

}

Ctask1View::~Ctask1View()
{
	m_cbmp.ReleaseBmp();
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
	
	if (!m_nwheelFlag&&!m_nMouseMoveFlag)               //鼠标滚轮未转动实现等比显示
	{
		CRect m_rc;
		GetClientRect(&m_rc);
		float m_fRate=float(m_cbmp.m_bmpInfoHeader.biWidth)/float(m_cbmp.m_bmpInfoHeader.biHeight);
		
		float m_fRcRate=float(m_rc.Width())/float(m_rc.Height());


		if (m_fRate>=m_fRcRate)                       //判断图像长宽比与窗口长宽比，图片宽度与窗口宽度一样
		{

			m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC, m_rc.Width(),int(m_rc.Width()/m_fRate));
			
			//刷新窗口，清除图片上方和下方区域
			CRect m_rcFresh(0,0,m_rc.Width(),(int)m_cbmp.get_m_fYPositionInDC());
			//m_rcFresh.SubtractRect(m_rc,m_PicRc);
			pDc->FillRect(m_rcFresh,&cBrush);
			CRect m_rcFresh1(0,(int)m_cbmp.get_m_fYPositionInDC()+int(m_rc.Width()/m_fRate),m_rc.Width(),m_rc.Height());
			//m_rcFresh.SubtractRect(m_rc,m_PicRc);
			pDc->FillRect(m_rcFresh1,&cBrush);

			m_fZScaling=float(m_rc.Width())/m_cbmp.m_bmpInfoHeader.biWidth;
		}
		if (m_fRate<m_fRcRate)                        //图片高度与窗口高度一致
		{

			m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC,int(m_rc.Height()*m_fRate),m_rc.Height());
			//刷新窗口，清除图片左边与右边区域
			CRect m_rcFresh(0,0,(int)m_cbmp.get_m_fXPositionInDC(),m_rc.Height());
			
			pDc->FillRect(m_rcFresh,&cBrush);

			CRect m_rcFresh1((int)m_cbmp.get_m_fXPositionInDC()+int(m_rc.Height()*m_fRate),0,m_rc.Width(),m_rc.Height());

			pDc->FillRect(m_rcFresh1,&cBrush);

			m_fZScaling=float(m_rc.Height())/m_cbmp.m_bmpInfoHeader.biHeight;
		}
	}
	else  if(m_nwheelFlag==1)                         //鼠标滚轮转动实现放大缩小
	{
		int m_nXPosition;    //起始点X轴坐标                                                        
		int m_nYPosition;    //起始点Y轴坐标
		CRect m_rc;
		GetClientRect(&m_rc);
		
		m_nXPosition=(m_rc.Width()-int(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling))/2;
		m_nYPosition=(m_rc.Height()-int(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling))/2;

		if (m_nXPosition>0)                              //图片起始点横坐标在窗口内，清除图片左右边区域
		{
			CRect m_rcLeft(0,0,m_nXPosition,m_rc.Height());
			pDc->FillRect(&m_rcLeft,&cBrush);
			CRect m_rcRight(m_nXPosition+(int)(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling),0,m_rc.Width(),m_rc.Height());
			pDc->FillRect(&m_rcRight,&cBrush);
		}
		if (m_nYPosition>0)                             //图片起始点纵坐标在窗口内，清除图片上下方区域
		{
			CRect m_rcTop(0,0,m_rc.Width(),m_nYPosition);
			pDc->FillRect(&m_rcTop,&cBrush);
			CRect m_rcBottom(0,m_nYPosition+(int)(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling),m_rc.Width(),m_rc.Height());
			pDc->FillRect(&m_rcBottom,&cBrush);
		}

		 m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC, 
			               int(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling),
			               int(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling)
						   );
		 m_nwheelFlag=0;
	}
	if (m_nMouseMoveFlag==1)                     //实现鼠标拖动
	{
		int m_nXPosition;    //起始点X轴坐标                                                        
		int m_nYPosition;    //起始点Y轴坐标

		CRect m_rc;
		GetClientRect(&m_rc);

		m_nXPosition=m_fXPositionInDC+m_fXoffset;
		m_nYPosition=m_fYPositionInDC+m_fYoffset;



		if (m_nXPosition>m_cbmp.get_m_fXPositionInDC())                       //向右移动则清除左边区域
		{
			CRect m_rcRefresh((int)m_cbmp.get_m_fXPositionInDC(),
				(int)m_cbmp.get_m_fYPositionInDC(),
				m_nXPosition,
				(int)m_cbmp.get_m_fYPositionInDC()+int(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling));
		
			pDc->FillRect(&m_rcRefresh,&cBrush);
		}
		if (m_nXPosition<m_cbmp.get_m_fXPositionInDC())                        //向左移动则清除右边区域
		{
			CRect m_rcRefresh(m_nXPosition+int(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling),
				(int)m_cbmp.get_m_fYPositionInDC(),
				(int)m_cbmp.get_m_fXPositionInDC()+int(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling),
				(int)m_cbmp.get_m_fYPositionInDC()+int(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling));

			pDc->FillRect(&m_rcRefresh,&cBrush);
			
		}
		if (m_nYPosition>m_cbmp.get_m_fYPositionInDC())                      //向下移动则清除上边区域
		{

			CRect m_rcRefresh((int)m_cbmp.get_m_fXPositionInDC(),
				(int)m_cbmp.get_m_fYPositionInDC(),
				(int)m_cbmp.get_m_fXPositionInDC()+int(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling),
				m_nYPosition);

			pDc->FillRect(&m_rcRefresh,&cBrush);

		}
		if (m_nYPosition<m_cbmp.get_m_fYPositionInDC())                       //向上移动则清除下边区域
		{

			CRect m_rcRefresh((int)m_cbmp.get_m_fXPositionInDC(),
				m_nYPosition+int(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling),
				(int)m_cbmp.get_m_fXPositionInDC()+int(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling),
				(int)m_cbmp.get_m_fYPositionInDC()+int(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling));

			pDc->FillRect(&m_rcRefresh,&cBrush);

		}
	
		
		m_cbmp.Draw(pDc->m_hDC,
			m_nXPosition,
			m_nYPosition,
			int(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling), 
			int(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling)
			);

		m_nMouseMoveFlag=0;
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
	//m_lpbImageDataSrc.Destroy();
	CFileDialog m_dlg(0,0,0,0,0,NULL);
	if (m_dlg.DoModal()==IDOK)
	{
		CString m_str=m_dlg.GetPathName();
	
		if (!m_cbmp.IsNull())
		{
			m_cbmp.ReleaseBmp();
		}
		m_cbmp.LoadBmp(m_str);
	
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
		m_cbmp.SaveBmp(m_str);

	}
	
}


BOOL Ctask1View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	 m_nwheelFlag=1;
	 
	 m_fZScaling+= 0.05*(zDelta/120); 

	 if (m_fZScaling>=2)
	 {
		 m_fZScaling=2;
	 }
	 if (m_fZScaling<=0.1)
	 {
		 m_fZScaling=0.1;
	 }


	 
	 
	Invalidate(FALSE);
	 
	 return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void Ctask1View::OnAdaptScreen()
{
	// TODO: 在此添加命令处理程序代码
	
	Invalidate(FALSE);
}


void Ctask1View::OnOriginSize()
{
	// TODO: 在此添加命令处理程序代码
	CClientDC dc(this);
	m_cbmp.DrawCenter(this->m_hWnd,dc.m_hDC,m_cbmp.m_bmpInfoHeader.biWidth, m_cbmp.m_bmpInfoHeader.biHeight);
	m_fZScaling=1;
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
	if (m_nLButtonDownFlag==1)
	{
		m_fXoffset=point.x-m_ptMousePoint.x;
		m_fYoffset=point.y-m_ptMousePoint.y;
		m_nMouseMoveFlag=1;
		Invalidate(false);
		UpdateWindow();
	}
	CView::OnMouseMove(nFlags, point);
}


void Ctask1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nLButtonDownFlag=0;
	m_nLButtonUpFlag=1;
	
	CView::OnLButtonUp(nFlags, point);
}


void Ctask1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nLButtonDownFlag=1;
	m_ptMousePoint=point;
	m_fXPositionInDC=m_cbmp.get_m_fXPositionInDC();
	m_fYPositionInDC=m_cbmp.get_m_fYPositionInDC();
	
	CView::OnLButtonDown(nFlags, point);
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
	if (!m_cbmp.IsNull())
	{
		m_cbmp.ReleaseBmp();
	}
	CView::OnClose();
}
