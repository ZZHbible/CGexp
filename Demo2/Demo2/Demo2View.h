
// Demo2View.h: CDemo2View 类的接口
//

#pragma once

#include <vector>
#include <string>

using namespace std;

#define QPolygonF std::vector<CPoint>
#define QPointF CPoint


struct EDGE {//边结构，保存边信息同时作为ET表中和AET表中的吊桶
	int ymin;//最小y值，等同于ET表中登记项的y值
	int ymax;//最大y值，吊桶数据
	double xmin;//小y值端点的x值，吊桶数据
	double fm;//斜率倒数，吊桶数据
	EDGE* next;//连接下一个边
	EDGE* last;//连接到上一个边，为了排序时使用
};


class CDemo2View : public CView
{
protected: // 仅从序列化创建
	CDemo2View() noexcept;
	DECLARE_DYNCREATE(CDemo2View)

// 特性
public:
	CDemo2Doc* GetDocument() const;

// 操作
public:
	//将传入的ET表排序
	void SortET(EDGE* pEDGE);
	//将传入的AET表排序
	void SortAET(EDGE* pEDGE);
	//获得多边形的最初的ET表
	EDGE* GetET(QPolygonF* points);










	vector<vector<int> > imgData;
	void loadImgData(const string& filename);

	std::vector<CPoint> poly;
	void FillPoly(std::vector<CPoint>* _poly, CClientDC* pDC);



















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
	virtual ~CDemo2View();
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
	afx_msg void OnButton32771();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void EdgeMarkFIll(std::vector<CPoint>* points, CClientDC* pDC);
};

#ifndef _DEBUG  // Demo2View.cpp 中的调试版本
inline CDemo2Doc* CDemo2View::GetDocument() const
   { return reinterpret_cast<CDemo2Doc*>(m_pDocument); }
#endif

