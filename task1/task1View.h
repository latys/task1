
// task1View.h : Ctask1View ��Ľӿ�
//

#pragma once
#include "Bmp.h"
#include "task1Doc.h"

class Ctask1View : public CView
{
protected: // �������л�����
	Ctask1View();
	DECLARE_DYNCREATE(Ctask1View)

// ����
public:
	Ctask1Doc* GetDocument() const;
	void GetPicRect(CImage img, CRect wdRect, CRect & rect);     //��õȱ�ѹ����ͼƬ������
	
// ����
public:
	
	

	
		 
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~Ctask1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // task1View.cpp �еĵ��԰汾
inline Ctask1Doc* Ctask1View::GetDocument() const
   { return reinterpret_cast<Ctask1Doc*>(m_pDocument); }
#endif

