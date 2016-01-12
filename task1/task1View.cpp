
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

// Ctask1View ����/����



Ctask1View::Ctask1View()
{
	// TODO: �ڴ˴���ӹ������
	m_fZScaling=1;         //��¼������
	m_ptMousePoint=(0,0);       //��¼���λ��
	m_fXPositionInDC=0;      //��갴��ʱ��ȡ��ǰͼƬ��DC�е�λ�� 
	m_fYPositionInDC=0;
	m_fXoffset=0;            //����϶���X��ƫ����
	m_fYoffset=0;            //����϶���Y��ƫ����
	m_nwheelFlag=0;           //������ת����־��0��δת���� 1��ת��
	m_nLButtonDownFlag=0;     //���������±�־λ
    m_nMouseMoveFlag=0;       //����϶���־λ
	

}

Ctask1View::~Ctask1View()
{
	m_cbmp.ReleaseBmp();
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
	pDc->FillRect(rect,&cBrush);
	
	if (!m_nwheelFlag&&!m_nMouseMoveFlag)               //������δת��ʵ�ֵȱ���ʾ
	{
		CRect m_rc;
		CRect m_PicRc;             //ͼƬ�ȱ���������
		GetClientRect(&m_rc);
		float m_fRate=float(m_cbmp.m_bmpInfoHeader.biWidth)/float(m_cbmp.m_bmpInfoHeader.biHeight);
		
		float m_fRcRate=float(m_rc.Width())/float(m_rc.Height());


		if (m_fRate>=m_fRcRate)
		{

			m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC, m_rc.Width(),int(m_rc.Width()/m_fRate));
			m_fZScaling=float(m_rc.Width())/m_cbmp.m_bmpInfoHeader.biWidth;
		}
		if (m_fRate<m_fRcRate)
		{

			m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC,int(m_rc.Height()*m_fRate),m_rc.Height());
			m_fZScaling=float(m_rc.Height())/m_cbmp.m_bmpInfoHeader.biHeight;
		}
	}
	else  if(m_nwheelFlag==1)                         //������ת��ʵ�ַŴ���С
	{
		 m_cbmp.DrawCenter(this->m_hWnd, pDc->m_hDC, 
			               int(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling),
			               int(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling)
						   );
		 m_nwheelFlag=0;
	}
	if (m_nMouseMoveFlag==1)
	{
		int m_nXPosition;    //��ʼ��X������                                                        
		int m_nYPosition;    //��ʼ��Y������

		CRect m_rc;
		GetClientRect(&m_rc);

		m_nXPosition=m_fXPositionInDC+m_fXoffset;
		m_nYPosition=m_fYPositionInDC+m_fYoffset;

		m_cbmp.Draw(pDc->m_hDC,
			        m_nXPosition,
					m_nYPosition,
			        int(m_cbmp.m_bmpInfoHeader.biWidth*m_fZScaling), 
					int(m_cbmp.m_bmpInfoHeader.biHeight*m_fZScaling)
					);
		m_nMouseMoveFlag=0;
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


void Ctask1View::OnFileOpen()
{
	// TODO: �ڴ���������������
	
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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


	 {
		 Invalidate(false);
	 }
	 return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void Ctask1View::OnAdaptScreen()
{
	// TODO: �ڴ���������������
	
	Invalidate(false);
}


void Ctask1View::OnOriginSize()
{
	// TODO: �ڴ���������������
	CClientDC dc(this);
	m_cbmp.DrawCenter(this->m_hWnd,dc.m_hDC,m_cbmp.m_bmpInfoHeader.biWidth, m_cbmp.m_bmpInfoHeader.biHeight);
	m_fZScaling=1;
}


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


void Ctask1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_nLButtonDownFlag==1)
	{
		m_fXoffset=point.x-m_ptMousePoint.x;
		m_fYoffset=point.y-m_ptMousePoint.y;
		m_nMouseMoveFlag=1;
		Invalidate(false);
	}
	CView::OnMouseMove(nFlags, point);
}


void Ctask1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_nLButtonDownFlag=0;
	CView::OnLButtonUp(nFlags, point);
}


void Ctask1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_nLButtonDownFlag=1;
	m_ptMousePoint=point;
	m_fXPositionInDC=m_cbmp.get_m_fXPositionInDC();
	m_fYPositionInDC=m_cbmp.get_m_fYPositionInDC();
	CView::OnLButtonDown(nFlags, point);
}


BOOL Ctask1View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CView::OnEraseBkgnd(pDC);
	//return TRUE;
}


void Ctask1View::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_cbmp.IsNull())
	{
		m_cbmp.ReleaseBmp();
	}
	CView::OnClose();
}
