
// task1View.cpp : Ctask1View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// Ctask1View ����/����



Ctask1View::Ctask1View()
{
	// TODO: �ڴ˴���ӹ������
	

	

}

Ctask1View::~Ctask1View()
{
	//m_cbmp.ReleaseBmp();
}

BOOL Ctask1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);

}

// Ctask1View ����

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
	
	if (!pDoc->m_nwheelFlag&&!pDoc->m_nMouseMoveFlag)               //������δת��ʵ�ֵȱ���ʾ
	{
		CRect m_rc;
		GetClientRect(&m_rc);
		float m_fRate=float(pDoc->m_cbmp.m_bmpInfoHeader.biWidth)/float(pDoc->m_cbmp.m_bmpInfoHeader.biHeight);
		
		float m_fRcRate=float(m_rc.Width())/float(m_rc.Height());


		if (m_fRate>=m_fRcRate)                       //�ж�ͼ�񳤿���봰�ڳ���ȣ�ͼƬ����봰�ڿ��һ��
		{

			pDoc->m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC, m_rc.Width(),int(m_rc.Width()/m_fRate));
			
			//ˢ�´��ڣ����ͼƬ�Ϸ����·�����
			CRect m_rcFresh(0,0,m_rc.Width(),(int)pDoc->m_cbmp.get_m_fYPositionInDC());
			//m_rcFresh.SubtractRect(m_rc,m_PicRc);
			pDc->FillRect(m_rcFresh,&cBrush);
			CRect m_rcFresh1(0,(int)pDoc->m_cbmp.get_m_fYPositionInDC()+int(m_rc.Width()/m_fRate),m_rc.Width(),m_rc.Height());
			//m_rcFresh.SubtractRect(m_rc,m_PicRc);
			pDc->FillRect(m_rcFresh1,&cBrush);

			pDoc->m_fZScaling=float(m_rc.Width())/pDoc->m_cbmp.m_bmpInfoHeader.biWidth;
		}
		if (m_fRate<m_fRcRate)                        //ͼƬ�߶��봰�ڸ߶�һ��
		{

			pDoc->m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC,int(m_rc.Height()*m_fRate),m_rc.Height());
			//ˢ�´��ڣ����ͼƬ������ұ�����
			CRect m_rcFresh(0,0,(int)pDoc->m_cbmp.get_m_fXPositionInDC(),m_rc.Height());
			
			pDc->FillRect(m_rcFresh,&cBrush);

			CRect m_rcFresh1((int)pDoc->m_cbmp.get_m_fXPositionInDC()+int(m_rc.Height()*m_fRate),0,m_rc.Width(),m_rc.Height());

			pDc->FillRect(m_rcFresh1,&cBrush);

			pDoc->m_fZScaling=float(m_rc.Height())/pDoc->m_cbmp.m_bmpInfoHeader.biHeight;
		}
	}
	else  if(pDoc->m_nwheelFlag==1)                         //������ת��ʵ�ַŴ���С
	{
		int m_nXPosition;    //��ʼ��X������                                                        
		int m_nYPosition;    //��ʼ��Y������
		CRect m_rc;
		GetClientRect(&m_rc);
		
		m_nXPosition=(m_rc.Width()-int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling))/2;
		m_nYPosition=(m_rc.Height()-int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling))/2;

		if (m_nXPosition>0)                              //ͼƬ��ʼ��������ڴ����ڣ����ͼƬ���ұ�����
		{
			CRect m_rcLeft(0,0,m_nXPosition,m_rc.Height());
			pDc->FillRect(&m_rcLeft,&cBrush);
			CRect m_rcRight(m_nXPosition+(int)(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
				            0,m_rc.Width(),m_rc.Height());
			pDc->FillRect(&m_rcRight,&cBrush);
		}
		if (m_nYPosition>0)                             //ͼƬ��ʼ���������ڴ����ڣ����ͼƬ���·�����
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
	if (pDoc->m_nMouseMoveFlag==1)                     //ʵ������϶�
	{
		int m_nXPosition;    //��ʼ��X������                                                        
		int m_nYPosition;    //��ʼ��Y������

		CRect m_rc;
		GetClientRect(&m_rc);

		m_nXPosition=pDoc->m_fXPositionInDC+pDoc->m_fXoffset;
		m_nYPosition=pDoc->m_fYPositionInDC+pDoc->m_fYoffset;



		if (m_nXPosition>pDoc->m_cbmp.get_m_fXPositionInDC())                       //�����ƶ�������������
		{
			CRect m_rcRefresh((int)pDoc->m_cbmp.get_m_fXPositionInDC(),
				(int)pDoc->m_cbmp.get_m_fYPositionInDC(),
				m_nXPosition,
				(int)pDoc->m_cbmp.get_m_fYPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling));
		
			pDc->FillRect(&m_rcRefresh,&cBrush);
		}
		if (m_nXPosition<pDoc->m_cbmp.get_m_fXPositionInDC())                        //�����ƶ�������ұ�����
		{
			CRect m_rcRefresh(m_nXPosition+int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
				(int)pDoc->m_cbmp.get_m_fYPositionInDC(),
				(int)pDoc->m_cbmp.get_m_fXPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
				(int)pDoc->m_cbmp.get_m_fYPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biHeight*pDoc->m_fZScaling));

			pDc->FillRect(&m_rcRefresh,&cBrush);
			
		}
		if (m_nYPosition>pDoc->m_cbmp.get_m_fYPositionInDC())                      //�����ƶ�������ϱ�����
		{

			CRect m_rcRefresh((int)pDoc->m_cbmp.get_m_fXPositionInDC(),
				(int)pDoc->m_cbmp.get_m_fYPositionInDC(),
				(int)pDoc->m_cbmp.get_m_fXPositionInDC()+int(pDoc->m_cbmp.m_bmpInfoHeader.biWidth*pDoc->m_fZScaling),
				m_nYPosition);

			pDc->FillRect(&m_rcRefresh,&cBrush);

		}
		if (m_nYPosition<pDoc->m_cbmp.get_m_fYPositionInDC())                       //�����ƶ�������±�����
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

	




	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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


// Ctask1View ���

#ifdef _DEBUG
void Ctask1View::AssertValid() const
{
	CView::AssertValid();
}

void Ctask1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ctask1Doc* Ctask1View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctask1Doc)));
	return (Ctask1Doc*)m_pDocument;
}
#endif //_DEBUG


// Ctask1View ��Ϣ�������













int Ctask1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*RECT rect = {0,0,20,500};
	m_cScrollBar.Create(WS_CHILD | WS_VISIBLE | SB_VERT,rect,this,2);//WS_VISIBLEһ��Ҫ���ϣ���Ȼ��Ϳ�����m_scrollbar��
	m_cScrollBar.SetScrollRange(0,1);
	m_cScrollBar.SetScrollPos(50);
	// TODO:  �ڴ������ר�õĴ�������*/

	return 0;
}





BOOL Ctask1View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}


void Ctask1View::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	
	CView::OnClose();
}


void Ctask1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Ctask1Doc *pDoc=(Ctask1Doc *)(GetDocument());
	pDoc->m_nLButtonDownFlag=1;
	pDoc->m_ptMousePoint=point;
	pDoc->m_fXPositionInDC=pDoc->m_cbmp.get_m_fXPositionInDC();
	pDoc->m_fYPositionInDC=pDoc->m_cbmp.get_m_fYPositionInDC();

	CView::OnLButtonDown(nFlags, point);
}


void Ctask1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	Ctask1Doc *pDoc=(Ctask1Doc *)(GetDocument());
	pDoc->m_nLButtonDownFlag=0;
	pDoc->m_nLButtonUpFlag=1;
	CView::OnLButtonUp(nFlags, point);
}


void Ctask1View::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// �˹���Ҫ�� Windows Vista ����߰汾��
	// _WIN32_WINNT ���ű��� >= 0x0600��
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	CView::OnMouseHWheel(nFlags, zDelta, pt);
}


void Ctask1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
