
// Demo4-CubPolyView.h : CDemo4CubPolyView ��Ľӿ�
//

#pragma once


class CDemo4CubPolyView : public CView
{
protected: // �������л�����
	CDemo4CubPolyView();
	DECLARE_DYNCREATE(CDemo4CubPolyView)

// ����
public:
	CDemo4CubPolyDoc* GetDocument() const;

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
	virtual ~CDemo4CubPolyView();
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // Demo4-CubPolyView.cpp �еĵ��԰汾
inline CDemo4CubPolyDoc* CDemo4CubPolyView::GetDocument() const
   { return reinterpret_cast<CDemo4CubPolyDoc*>(m_pDocument); }
#endif

