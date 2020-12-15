
// Demo6-1View.cpp: CDemo61View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Demo6-1.h"
#endif

#include "Demo6-1Doc.h"
#include "Demo6-1View.h"

#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemo61View

IMPLEMENT_DYNCREATE(CDemo61View, CView)

BEGIN_MESSAGE_MAP(CDemo61View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDemo61View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_END_TIMER, &CDemo61View::OnEndTimer)
	ON_COMMAND(ID_START_TIMER, &CDemo61View::OnStartTimer)
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CDemo61View 构造/析构

CDemo61View::CDemo61View() noexcept
{
	// TODO: 在此处添加构造代码

}

CDemo61View::~CDemo61View()
{
}

BOOL CDemo61View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDemo61View 绘图

vector<CPoint> Poly;

bool inputFinished = false;

void vectorMatrixProduct(const vector<float>& vec, const vector<vector<float> > & mat, vector<float>& rst)
{
	for (int k = 0; k < rst.size(); ++k)
	{
		rst[k] = 0;
		for (int m = 0; m < vec.size(); ++m)
			rst[k] += vec[m] * mat[m][k];
	}
}
void DrawBezier(const vector<CPoint>& poly,CDC* pDC)
{
	if (poly.empty())
		return;

	CPen penPoly(PS_SOLID, 2, RGB(10, 10, 128));
	CPen* oldPen = nullptr;
	// draw poly
	pDC->MoveTo(poly[0]);
	for (int k = 1; k < poly.size(); ++k)
		pDC->LineTo(poly[k]);

	if (inputFinished)
		pDC->LineTo(poly[0]);

	if (poly.size() < 3)
		return;

	vector<vector<float> > mat(3);
	vector<float> mat0(3);
	mat0[0] = 1; mat0[1] = -2; mat0[2] = 1;
	mat[0] = mat0;

	vector<float> mat1(3);
	mat1[0] = -2; mat1[1] = 2; mat1[2] = 0;
	mat[1] = mat1;

	vector<float> mat2(3);
	mat2[0] = 1; mat2[1] = 0; mat2[2] = 0;
	mat[2] = mat2;

	CPen penCurve(PS_SOLID, 2, RGB(128, 10, 10));
	oldPen = pDC->SelectObject(&penCurve);
	// draw curve
	for (int kc = 0; kc < poly.size(); ++kc)
	{
		vector<CPoint> curCtls(3);
		const int KC = kc;
		const int KC1 = (kc + 1) % poly.size();
		const int KC2 = (kc + 2) % poly.size();
		curCtls[0] = (poly[KC] + poly[KC1]);
		curCtls[0].x /= 2;
		curCtls[0].y /= 2;
		curCtls[1] = (poly[KC1]);
		curCtls[2] = poly[KC2];
		curCtls[2] = (poly[KC1] + poly[KC2]);
		curCtls[2].x /= 2;
		curCtls[2].y /= 2;

		vector<float> uvec(3);
		float u = 0;
		vector<float> umat(3);
		float curPtFloat[2]{ 0,0 };
		const int numSegs = 20;
		// to get more beautiful effect, use other method for computing the points on the curve !!
		for (int k = 0; k <= numSegs; ++k)
		{
			u = 1. / numSegs * k;
			uvec[0] = u * u;
			uvec[1] = u;
			uvec[2] = 1;

			vectorMatrixProduct(uvec, mat, umat);

			curPtFloat[0] = 0;
			curPtFloat[1] = 0;

			for (int m = 0; m < 3; ++m)
			{
				curPtFloat[0] += curCtls[m].x * umat[m];
				curPtFloat[1] += curCtls[m].y * umat[m];
			}

			if (k == 0)
				pDC->MoveTo(curPtFloat[0] + 0.5, curPtFloat[1] + 0.5);
			else
				pDC->LineTo(curPtFloat[0] + .5, curPtFloat[1] + .5);
		}
	}

	pDC->SelectObject(oldPen);
}


void CDemo61View::OnDraw(CDC* pDC)
{
	CDemo61Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawBezier(Poly,pDC);

	// TODO: 在此处为本机数据添加绘制代码
}


// CDemo61View 打印


void CDemo61View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDemo61View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDemo61View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDemo61View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDemo61View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
#if 0
	ClientToScreen(&point);
	OnContextMenu(this, point);
#endif
}

void CDemo61View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDemo61View 诊断

#ifdef _DEBUG
void CDemo61View::AssertValid() const
{
	CView::AssertValid();
}

void CDemo61View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo61Doc* CDemo61View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo61Doc)));
	return (CDemo61Doc*)m_pDocument;
}
#endif //_DEBUG


// CDemo61View 消息处理程序


void CDemo61View::OnEndTimer()
{
	KillTimer(1);
}


void CDemo61View::OnStartTimer()
{
	if (!inputFinished)
		return;

	SetTimer(1, 50, NULL);
}

vector<int> Dir;

void CDemo61View::UpdatePoly(vector<CPoint>& poly,vector<int>& dir)
{
	CRect rect;
	GetClientRect(rect);

	for (int k = 0; k < poly.size(); ++k)
	{
		const int speed = (k + 1) * 3 * dir[k];
		int curPos = 0;
		if (k % 2 == 0)// odd
		{
			curPos = poly[k].x + speed;
			if (curPos > rect.right)
			{
				curPos = rect.right;// any problem???


				Dir[k] = -Dir[k];
			}
			else if (curPos < rect.left)
			{
				curPos = rect.left;

				Dir[k] = -Dir[k];
			}

			poly[k].x = curPos;

		}
		else// even
		{
			curPos = poly[k].y + speed;
			if (curPos > rect.bottom)
			{
				curPos = rect.bottom;

				Dir[k] = -Dir[k];
			}
			else if (curPos < rect.top)
			{
				curPos = rect.top;

				Dir[k] = -Dir[k];
			}
			poly[k].y = curPos;
			
		}
	}
}

void CDemo61View::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
		UpdatePoly(Poly,Dir);
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnTimer(nIDEvent);
}



void CDemo61View::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	inputFinished = true;

	Invalidate();

	CView::OnLButtonDblClk(nFlags, point);
}


void CDemo61View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (inputFinished)
		return;

	Poly.push_back(point);
	Dir.push_back(1);

	Invalidate();



	CView::OnLButtonDown(nFlags, point);
}


void CDemo61View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_RETURN:
		UpdatePoly(Poly, Dir);
		Invalidate();
		break;
	default:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
