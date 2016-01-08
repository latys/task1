#pragma once

class CBmp
{
public:
	BITMAPFILEHEADER m_bmpFileHeader;
	BITMAPINFOHEADER m_bmpInfoHeader;
	RGBQUAD *m_rgbQuad;

	BYTE *m_bmpData;  //图像数据

	float fWidthHeightRate;
private:
	float m_fXPositionInDC;       //记录图像在DC中的起始位置，调用Draw函数后获得
	float m_fYPositionInDC;
public:
	CBmp(void);
	CBmp(int x, int y, int n);
	~CBmp(void);


	void set_m_fXPositionInDC(float fXPosition);
	float get_m_fXPositionInDC();
	void set_m_fYPositionInDC(float fXPosition);
	float get_m_fYPositionInDC();
	
	void LoadBmp(CString m_strFileName);
	void SaveBmp(CString m_strFileName);
	bool IsNull();
	void ReleaseBmp();
	void Draw(HDC hdc, int x, int y);
	void Draw(HDC hdc, int x, int y, int nwidth, int nheight);
	void DrawCenter(HWND hWnd, HDC hdc, int nwidth, int nheight);
	void Resize(CBmp *dstBmp,int nWidth, int nHeight);
	void Resize(CBmp *dstBmp,float nRate);
		 
};

