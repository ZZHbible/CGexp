
// Demo3View.h : CDemo3View ��Ľӿ�
//

#pragma once


class CDemo3View : public CView
{
protected: // �������л�����
	CDemo3View();
	DECLARE_DYNCREATE(CDemo3View)

// ����
public:
	CDemo3Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CDemo3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	enum TransMode{Translate,Rotate,Scale};
	TransMode transMode;
	float transDelta;
	float rotateDelta;
	float scaleDelta;
	void DrawCube(CDC* pDC, POINT projedCubeVerts[], int CubeFaces[][4]);

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnProjXie2();
	afx_msg void OnProjPerspective();
	afx_msg void OnTranslate();
	afx_msg void OnRotate();
	afx_msg void OnScale();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // Demo3View.cpp �еĵ��԰汾
inline CDemo3Doc* CDemo3View::GetDocument() const
   { return reinterpret_cast<CDemo3Doc*>(m_pDocument); }
#endif

