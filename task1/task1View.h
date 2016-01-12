
// task1View.h : Ctask1View 类的接口
//

#pragma once
#include "Bmp.h"
#include "task1Doc.h"

class Ctask1View : public CView
{
protected: // 仅从序列化创建
	Ctask1View();
	DECLARE_DYNCREATE(Ctask1View)

// 特性
public:
	Ctask1Doc* GetDocument() const;
	void GetPicRect(CImage img, CRect wdRect, CRect & rect);     //获得等比压缩后图片的区域
	
// 操作
public:
	CBmp m_cbmp;
	float m_fZScaling;         //记录鼠标滚轮
	CPoint m_ptMousePoint;       //记录鼠标位置
	float m_fXPositionInDC;      //鼠标按下时获取当前图片在DC中的位置 
	float m_fYPositionInDC;
	float m_fXoffset;            //鼠标拖动后X轴偏移量
	float m_fYoffset;            //鼠标拖动后Y轴偏移量
	int m_nwheelFlag;           //鼠标滚轮转动标志，0：未转动。 1：转动
	int m_nLButtonDownFlag;     //鼠标左键按下标志位
	int m_nMouseMoveFlag;       //鼠标拖动标志位
	
		 
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~Ctask1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnAdaptScreen();
	afx_msg void OnOriginSize();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
};

#ifndef _DEBUG  // task1View.cpp 中的调试版本
inline Ctask1Doc* Ctask1View::GetDocument() const
   { return reinterpret_cast<Ctask1Doc*>(m_pDocument); }
#endif

