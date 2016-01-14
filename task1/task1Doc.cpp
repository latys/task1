
// task1Doc.cpp : Ctask1Doc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "task1.h"
#endif

#include "task1Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ctask1Doc

IMPLEMENT_DYNCREATE(Ctask1Doc, CDocument)

BEGIN_MESSAGE_MAP(Ctask1Doc, CDocument)
END_MESSAGE_MAP()


// Ctask1Doc 构造/析构

Ctask1Doc::Ctask1Doc()
{
	// TODO: 在此添加一次性构造代码
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

Ctask1Doc::~Ctask1Doc()
{
	m_cbmp.ReleaseBmp();
}

BOOL Ctask1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// Ctask1Doc 序列化

void Ctask1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void Ctask1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void Ctask1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void Ctask1Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Ctask1Doc 诊断

#ifdef _DEBUG
void Ctask1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Ctask1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Ctask1Doc 命令
