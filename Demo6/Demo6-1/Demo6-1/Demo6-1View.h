
// Demo6-1View.h: CDemo61View 类的接口
//

#pragma once

#include<vector>
using namespace std;


class CDemo61View : public CView
{
protected: // 仅从序列化创建
	CDemo61View() noexcept;
	DECLARE_DYNCREATE(CDemo61View)

// 特性
public:
	CDemo61Doc* GetDocument() const;

private:
	void UpdatePoly(vector<CPoint>& poly,vector<int>& dir);

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
	virtual ~CDemo61View();
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
	afx_msg void OnEndTimer();
	afx_msg void OnStartTimer();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // Demo6-1View.cpp 中的调试版本
inline CDemo61Doc* CDemo61View::GetDocument() const
   { return reinterpret_cast<CDemo61Doc*>(m_pDocument); }
#endif

