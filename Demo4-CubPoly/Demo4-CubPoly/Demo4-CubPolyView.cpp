
// Demo4-CubPolyView.cpp : CDemo4CubPolyView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Demo4-CubPoly.h"
#endif

#include "Demo4-CubPolyDoc.h"
#include "Demo4-CubPolyView.h"

#include <iostream>

#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemo4CubPolyView

IMPLEMENT_DYNCREATE(CDemo4CubPolyView, CView)

BEGIN_MESSAGE_MAP(CDemo4CubPolyView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDemo4CubPolyView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()

// CDemo4CubPolyView 构造/析构

CDemo4CubPolyView::CDemo4CubPolyView()
{
	// TODO: 在此处添加构造代码

}

CDemo4CubPolyView::~CDemo4CubPolyView()
{
}

BOOL CDemo4CubPolyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDemo4CubPolyView 绘制

vector<CPoint> rectPts;// 0 should be the left top point, and 1 the right bottom point
vector<CPoint> polyPts;// the points of the polygon
bool polyFinished = false;// if double clicked is triggered.

void DrawRect(const vector<CPoint>& rectPts,CDC* pDC)
{
	if (rectPts.size() != 2)
	{
//		cout << "矩形顶点数目不对" << rectPts.size() << endl;
		return;
	}
	
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* oldPen = pDC->SelectObject(&pen);

	pDC->Rectangle(CRect(rectPts[0], rectPts[1]));

	pDC->SelectObject(oldPen);
}
void DrawPoly(const vector<CPoint>& polyPts,CDC* pDC)
{
	if (polyPts.size() < 2)
		return;

	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(&pen);

	const int ptcount = polyPts.size();
	pDC->MoveTo(polyPts[0]);
	for (int k = 0; k < ptcount; ++k)
	{
		pDC->LineTo(polyPts[k]);
	}
	if (polyFinished)
		pDC->LineTo(polyPts[0]);

	pDC->SelectObject(oldPen);
}



vector<CPoint> cut_top(const vector<CPoint>& rectPts,const vector<CPoint>& pt)
{
	vector<CPoint> cp;

	CPoint f, p, s;// f: first point, p: current point, s: last point
	int c1, c2;
	int juy2 = max(rectPts[1].y, rectPts[0].y);
	const int count = pt.size() - 1;
	// the last one is just the first one. count is the real number of DIFFERENT points in the poly.
	for (int i = 0; i< count + 1; i++) 
	{
		p = pt[i];// current point

		if (i != 0) 
		{
			if (p.y>juy2)// p is outside the rectangle
				c2 = -1;
			else
				c2 = 1;

			// c1 + c2 means one is -1 and the other is 1, so one is inside and the other is outside. so there is one intersection.
			if (c1 + c2 == 0) 
			{
				CPoint I;// intersection point
				I.y = juy2;
				I.x = (p.x - s.x)*(I.y - s.y) / (p.y - s.y) + s.x;
				cp.push_back(I);
			}
		}
		else
		{
			f = p;
		}

		s = p;
		if (s.y>juy2)
			c1 = -1;
		else 
		{
			c1 = 1;
			cp.push_back(s);
		}
	}
	if (f.y>juy2)
		c2 = -1;
	else
		c2 = 1;
	if (c1 + c2 == 0) {
		CPoint I;
		I.y = juy2;
		I.x = (f.x - s.x)*(I.y - s.y) / (f.y - s.y) + s.x;
		cp.push_back(I);
	}
	return cp;
}

vector<CPoint> cut_right(const vector<CPoint>& rectPts, const vector<CPoint>& pt)
{
	vector<CPoint> cp;
	CPoint f, p, s;
	int c1, c2;
	const int count = pt.size() - 1;
	int jux2 = max(rectPts[0].x,rectPts[1].x);
	for (int i = 0; i<count + 1; i++) 
	{
		p = pt[i];
		if (i != 0) {
			if (p.x>jux2)
				c2 = -1;
			else 
				c2 = 1;
			if (c1 + c2 == 0) 
			{
				CPoint I;
				I.x = jux2;
				I.y = (p.y - s.y)*(I.x - s.x) / (p.x - s.x) + s.y;
				cp.push_back(I);
			}
		}
		else
			f = p;
		s = p;
		if (s.x>jux2)
			c1 = -1;
		else 
		{
			c1 = 1;
			cp.push_back(s);
		}
	}
	if (f.x>jux2)
		c2 = -1;
	else
		c2 = 1;
	if (c1 + c2 == 0)
	{
		CPoint I;
		I.x = jux2;
		I.y = (f.y - s.y)*(I.x - s.x) / (f.x - s.x) + s.y;
		cp.push_back(I);
	}

	return cp;
}

vector<CPoint> cut_bottem(const vector<CPoint>& rectPts, const vector<CPoint>& pt)
{
	vector<CPoint> cp;// cut poly points
	CPoint f, p, s;
	int c1, c2;
	const int count = pt.size() - 1;// cause the last pt is just the first pt, the real DIFFERENT points in pt are from 0 to pt.size() - 1.
	int juy1 = min(rectPts[1].y,rectPts[0].y);
	for (int i = 0; i<count + 1; i++) 
	{
		p = pt[i];
		if (i != 0)
		{
			if (p.y<juy1)
				c2 = -1;
			else 
				c2 = 1;
			if (c1 + c2 == 0) 
			{
				CPoint I;
				I.y = juy1;
				I.x = (p.x - s.x)*(I.y - s.y) / (p.y - s.y) + s.x;
				cp.push_back(I);
			}
		}
		else
			f = p;
		s = p;
		if (s.y<juy1)
			c1 = -1;
		else 
		{
			c1 = 1;
			cp.push_back(s);
		}
	}
	if (f.y<juy1)
		c2 = -1;
	else
		c2 = 1;
	if (c1 + c2 == 0)
	{
		CPoint I;
		I.y = juy1;
		I.x = (f.x - s.x)*(I.y - s.y) / (f.y - s.y) + s.x;
		cp.push_back(I);
	}

	return cp;
}

vector<CPoint> cut_left(const vector<CPoint>& rectPts, const vector<CPoint>& pt)
{
	vector<CPoint> cp;

	CPoint f, p, s;
	int c1, c2;
	const int count = pt.size() - 1;
	int jux1 = min(rectPts[1].x,rectPts[0].x);
	for (int i = 0; i<count + 1; i++)
	{
		p = pt[i];
		if (i != 0) 
		{
			if (p.x<jux1)
				c2 = -1;
			else
				c2 = 1;
			if (c1 + c2 == 0) 
			{
				CPoint I;
				I.x = jux1;
				I.y = (p.y - s.y)*(I.x - s.x) / (p.x - s.x) + s.y;
				cp.push_back(I);
			}
		}
		else
			f = p;
		s = p;
		if (s.x<jux1)
			c1 = -1;
		else 
		{
			c1 = 1;
			cp.push_back(s);
		}
	}
	if (f.x<jux1)
		c2 = -1;
	else
		c2 = 1;
	if (c1 + c2 == 0) {
		CPoint I;
		I.x = jux1;
		I.y = (f.y - s.y)*(I.x - s.x) / (f.x - s.x) + s.y;
		cp.push_back(I);
	}

	return cp;
}

void CutPoly(const vector<CPoint>& rectPts, vector<CPoint>& pt,CDC*  pDC)
{
	if (rectPts.size() != 2)
	{
//		cout << "矩形顶点数目不对" << rectPts.size() << endl;
		return;
	}

	if (pt.size() < 3)
		return;

	pt.push_back(pt[0]);
	pt = cut_top(rectPts,pt);

#if 1
	pt.push_back(pt[0]);
	pt = cut_right(rectPts, pt);

	pt.push_back(pt[0]);
	pt = cut_bottem(rectPts, pt);

	pt.push_back(pt[0]);
	pt = cut_left(rectPts, pt);

#endif
	CPen pen(PS_SOLID, 3, RGB(0, 255, 0));
	CPen* oldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(pt[0]);
	for (int i = 1; i< pt.size(); i++) {
		pDC->LineTo(pt[i]);
	}
//	pDC->LineTo(pt[0]);

	pDC->SelectObject(oldPen);
}

void CDemo4CubPolyView::OnDraw(CDC* pDC)
{
	CDemo4CubPolyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawRect(rectPts, pDC);
	DrawPoly(polyPts, pDC);

	if (polyFinished)
		CutPoly(rectPts, polyPts,pDC);

	// TODO: 在此处为本机数据添加绘制代码
}


// CDemo4CubPolyView 打印


void CDemo4CubPolyView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDemo4CubPolyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDemo4CubPolyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDemo4CubPolyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDemo4CubPolyView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
#if 0
	ClientToScreen(&point);
	OnContextMenu(this, point);
#endif
}

void CDemo4CubPolyView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDemo4CubPolyView 诊断

#ifdef _DEBUG
void CDemo4CubPolyView::AssertValid() const
{
	CView::AssertValid();
}

void CDemo4CubPolyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo4CubPolyDoc* CDemo4CubPolyView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo4CubPolyDoc)));
	return (CDemo4CubPolyDoc*)m_pDocument;
}
#endif //_DEBUG


// CDemo4CubPolyView 消息处理程序


void CDemo4CubPolyView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (rectPts.size() == 2)
		rectPts.clear();

	rectPts.push_back(point);

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}


void CDemo4CubPolyView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (polyFinished)
	{
		polyFinished = false;
		polyPts.clear();
	}

	polyPts.push_back(point);

	Invalidate();

	CView::OnRButtonDown(nFlags, point);
}


void CDemo4CubPolyView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	polyFinished = true;

	Invalidate();

	CView::OnRButtonDblClk(nFlags, point);
}
