
// task1Doc.h : Ctask1Doc ��Ľӿ�
//


#pragma once
#include "Bmp.h"

class Ctask1Doc : public CDocument
{
protected: // �������л�����
	Ctask1Doc();
	DECLARE_DYNCREATE(Ctask1Doc)

// ����
public:
	CBmp m_cbmp;
	float m_fZScaling;         //��¼������
	CPoint m_ptMousePoint;       //��¼���λ��
	float m_fXPositionInDC;      //��갴��ʱ��ȡ��ǰͼƬ��DC�е�λ�� 
	float m_fYPositionInDC;
	float m_fXoffset;            //����϶���X��ƫ����
	float m_fYoffset;            //����϶���Y��ƫ����
	int m_nwheelFlag;           //������ת����־��0��δת���� 1��ת��
	int m_nLButtonDownFlag;     //���������±�־λ
	int m_nMouseMoveFlag;       //����϶���־λ
	int m_nLButtonUpFlag;        //�������ɿ���־λ
// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~Ctask1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
