
// Demo4-CubPolyView.h : CDemo4CubPolyView 类的接口
//

#pragma once


class CDemo4CubPolyView : public CView
{
protected: // 仅从序列化创建
	CDemo4CubPolyView();
	DECLARE_DYNCREATE(CDemo4CubPolyView)

// 特性
public:
	CDemo4CubPolyDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CDemo4CubPolyView();
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // Demo4-CubPolyView.cpp 中的调试版本
inline CDemo4CubPolyDoc* CDemo4CubPolyView::GetDocument() const
   { return reinterpret_cast<CDemo4CubPolyDoc*>(m_pDocument); }
#endif

