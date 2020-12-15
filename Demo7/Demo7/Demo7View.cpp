
// Demo7View.cpp: CDemo7View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Demo7.h"
#endif

#include "Demo7Doc.h"
#include "Demo7View.h"

#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemo7View

IMPLEMENT_DYNCREATE(CDemo7View, CView)

BEGIN_MESSAGE_MAP(CDemo7View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDemo7View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_1, &CDemo7View::On1)
	ON_COMMAND(ID_2, &CDemo7View::On2)
	ON_COMMAND(ID_3, &CDemo7View::On3)
END_MESSAGE_MAP()

vector<CPoint> Poly;
bool inputFinished = false;
int mode = 0;// 1, 2, 3 for the requriment

// CDemo7View 构造/析构

CDemo7View::CDemo7View() noexcept
{
	// TODO: 在此处添加构造代码

}

CDemo7View::~CDemo7View()
{
}

BOOL CDemo7View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDemo7View 绘图

void vectorMatrixProduct(const vector<float>& vec, const vector<vector<float> > & mat, vector<float>& rst)
{
	for (int k = 0; k < rst.size(); ++k)
	{
		rst[k] = 0;
		for (int m = 0; m < vec.size(); ++m)
			rst[k] += vec[m] * mat[m][k];
	}
}

void DrawBSpline(const vector<CPoint>& splineCtls, CDC* pDC)
{
	if (splineCtls.empty())
		return;

	CPen Pens[3];
	Pens[0].CreatePen(PS_SOLID, 2, RGB(200, 0, 0));
	Pens[1].CreatePen(PS_SOLID, 2, RGB(0, 200, 0));
	Pens[2].CreatePen(PS_SOLID, 2, RGB(0, 0, 200));
	CPen* oldPen = nullptr;
	

	vector<CPoint> ctls(splineCtls.size() + 6);
	ctls[0] = ctls[1] = ctls[2] = ctls[3] = splineCtls[0];
	int k1 = 1;
	int k2 = 4;
	for (int k = 1; k < splineCtls.size(); ++k, ++ k2, ++ k1)
	{
		ctls[k2] = splineCtls[k];
	}
	for (int k = 0; k < 3; ++k)
	{
		ctls[k2++] = splineCtls[k1 - 1];
	}

	vector<vector<float> > mat(4);
	vector<float> mat0(4);
	mat0[0] = -1; mat0[1] = 3; mat0[2] = -3; mat0[3] = 1;
	mat[0] = mat0;

	vector<float> mat1(4);
	mat1[0] = 3; mat1[1] = -6; mat1[2] = 3; mat1[3] = 0;
	mat[1] = mat1;

	vector<float> mat2(4);
	mat2[0] = -3; mat2[1] = 0; mat2[2] = 3; mat2[3] = 0;
	mat[2] = mat2;

	vector<float> mat3(4);
	mat3[0] = 1; mat3[1] = 4; mat3[2] = 1; mat3[3] = 0;
	mat[3] = mat3;
	for (int k = 0; k < 4; ++k)
		for (int m = 0; m < 4; ++m)
			mat[k][m] /= 6.;

	CPen penCurve(PS_SOLID, 2, RGB(128, 10, 10));
	oldPen = pDC->SelectObject(&penCurve);
	// draw curve

	const int numSegs = 20;
	// to get more beautiful effect, use other method for computing the points on the curve !!
	for (int kc = 0; kc < ctls.size() - 4 + 1; ++kc)
	{
		pDC->SelectObject(Pens + (kc % 3));
		vector<float> uvec(4);
		float u = 0;
		vector<float> umat(4);
		float curPt[2]{ 0,0 };

		vector<CPoint> curCtls(4);
		for (int k = 0; k < 4; ++k)
			curCtls[k] = ctls[kc + k];

		for (int k = 0; k <= numSegs; ++k)
		{
			u = 1. / numSegs * k;
			uvec[0] = u * u * u;
			uvec[1] = u * u;
			uvec[2] = u;
			uvec[3] = 1.;

			vectorMatrixProduct(uvec, mat, umat);

			curPt[0] = 0;
			curPt[1] = 0;

			for (int m = 0; m < 4; ++m)
			{
				curPt[0] += curCtls[m].x * umat[m];
				curPt[1] += curCtls[m].y * umat[m];
			}

			if (k == 0)
				pDC->MoveTo(curPt[0] + .5,curPt[1] + .5);
			else
				pDC->LineTo(curPt[0] + .5,curPt[1] + .5);
		}
	}



	pDC->SelectObject(oldPen);
}

void Draw(const vector<CPoint>& Poly, CDC* pDC)
{
	if (Poly.empty())
		return;

	CPen penPoly(PS_SOLID, 2, RGB(0, 0, 128));
	CPen penCurve(PS_SOLID, 2, RGB(128, 0, 0));

	pDC->SelectObject(&penPoly);
	pDC->MoveTo(Poly[0]);
	for (int k = 0; k < Poly.size(); ++k)
		pDC->LineTo(Poly[k]);

	if (Poly.size() < 4)
		return;

	switch (mode)
	{
	case 1:
		DrawBSpline(Poly, pDC);
		break;

	case 2:

		break;

	case 3:

		break;

	default:
		return;
		break;
	}
}

void CDemo7View::OnDraw(CDC* pDC)
{
	CDemo7Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Draw(Poly, pDC);

	// TODO: 在此处为本机数据添加绘制代码
}


// CDemo7View 打印


void CDemo7View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDemo7View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDemo7View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDemo7View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDemo7View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDemo7View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDemo7View 诊断

#ifdef _DEBUG
void CDemo7View::AssertValid() const
{
	CView::AssertValid();
}

void CDemo7View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo7Doc* CDemo7View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo7Doc)));
	return (CDemo7Doc*)m_pDocument;
}
#endif //_DEBUG


// CDemo7View 消息处理程序


void CDemo7View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (inputFinished)
		return;

	Poly.push_back(point);

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}


void CDemo7View::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	inputFinished = true;

	Invalidate();

	CView::OnLButtonDblClk(nFlags, point);
}


void CDemo7View::On1()
{
	mode = 1;

	Invalidate();
}


void CDemo7View::On2()
{
	mode = 2;

	Invalidate();
}


void CDemo7View::On3()
{
	mode = 3;

	Invalidate();
}
