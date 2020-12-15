
// Demo7View.h: CDemo7View 类的接口
//

#pragma once


class CDemo7View : public CView
{
protected: // 仅从序列化创建
	CDemo7View() noexcept;
	DECLARE_DYNCREATE(CDemo7View)

// 特性
public:
	CDemo7Doc* GetDocument() const;

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
	virtual ~CDemo7View();
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
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void On1();
	afx_msg void On2();
	afx_msg void On3();
};

#ifndef _DEBUG  // Demo7View.cpp 中的调试版本
inline CDemo7Doc* CDemo7View::GetDocument() const
   { return reinterpret_cast<CDemo7Doc*>(m_pDocument); }
#endif

