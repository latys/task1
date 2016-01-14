
// task1Doc.h : Ctask1Doc 类的接口
//


#pragma once
#include "Bmp.h"

class Ctask1Doc : public CDocument
{
protected: // 仅从序列化创建
	Ctask1Doc();
	DECLARE_DYNCREATE(Ctask1Doc)

// 特性
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
	int m_nLButtonUpFlag;        //鼠标左键松开标志位
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~Ctask1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
