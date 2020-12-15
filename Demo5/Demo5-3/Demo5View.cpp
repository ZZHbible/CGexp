
// Demo5View.cpp: CDemo5View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Demo5.h"
#endif

#include "Demo5Doc.h"
#include "Demo5View.h"

#include <iostream>
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemo5View

IMPLEMENT_DYNCREATE(CDemo5View, CView)

BEGIN_MESSAGE_MAP(CDemo5View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDemo5View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_HERMITE, &CDemo5View::OnHermite)
	ON_COMMAND(ID_BEZIER, &CDemo5View::OnBezier)
	ON_COMMAND(ID_SPLINE, &CDemo5View::OnSpline)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_BEZIER_SURFACE, &CDemo5View::OnBezierSurface)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CDemo5View 构造/析构


CDemo5View::~CDemo5View()
{
}

BOOL CDemo5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}




// CDemo5View 打印


void CDemo5View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDemo5View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDemo5View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDemo5View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDemo5View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
#if 0
	ClientToScreen(&point);
	OnContextMenu(this, point);
#endif
}

void CDemo5View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDemo5View 诊断

#ifdef _DEBUG
void CDemo5View::AssertValid() const
{
	CView::AssertValid();
}

void CDemo5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo5Doc* CDemo5View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo5Doc)));
	return (CDemo5Doc*)m_pDocument;
}
#endif //_DEBUG


// CDemo5View 消息处理程序

enum DrawMode{Hermite = 0,Bezier,BSpline,BezierSurface};
DrawMode drawMode = Hermite;

vector<CPoint> hermiteCtls;// 0: first point, 1: first tangent, 2: second point, 3: second tangent
vector<CPoint> bezierCtls;
vector<CPoint> splineCtls;// accept any number of ctl pts >= 4

struct Point3{
    float x;
    float y;
    float z;

	Point3() :x(0), y(0), z(0) {};
	Point3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {};

	Point3& operator+=(const Point3& q)
	{
		this->x += q.x;
		this->y += q.y;
		this->z += q.z;

		return *this;
	}

	friend Point3 operator+(const Point3& p, const Point3& q)
	{
		return Point3(p.x + q.x, p.y + q.y, p.z + q.z);
	}

	friend Point3 operator*(float f,const Point3& p)
	{
		return Point3(p.x * f, p.y * f, p.z * f);
	}
	
};
vector<vector<Point3> > bezierSurfaceCtls(4);// bezier surface control points. 4 by 4. actually, surface point should has three elements, not two
// initialize here


CDemo5View::CDemo5View() noexcept
{
	Point3 p1(0, 10, 0); Point3 p2(50, 20, 0); Point3 p3(100, 10, 0); Point3 p4(150, 0, 0);
	vector<Point3> oneRow(4);
	oneRow[0] = p1; oneRow[1] = p2; oneRow[2] = p3; oneRow[3] = p4;
	bezierSurfaceCtls[0] = oneRow;

	p1 = Point3(0, -10, 50); p2 = Point3(50, -20, 50); p3 = Point3(100, -10, 50); p4 = Point3(150, 0, 50);
	oneRow[0] = p1; oneRow[1] = p2; oneRow[2] = p3; oneRow[3] = p4;
	bezierSurfaceCtls[1] = oneRow;

	p1 = Point3(0, 10, 100); p2 = Point3(50, -10, 100); p3 = Point3(100, 10, 100); p4 = Point3(150, -10, 100);
	oneRow[0] = p1; oneRow[1] = p2; oneRow[2] = p3; oneRow[3] = p4;
	bezierSurfaceCtls[2] = oneRow;

	p1 = Point3(0, 0, 150); p2 = Point3(50, 0, 150); p3 = Point3(100, 0, 150); p4 = Point3(150, 0, 150);
	oneRow[0] = p1; oneRow[1] = p2; oneRow[2] = p3; oneRow[3] = p4;
	bezierSurfaceCtls[3] = oneRow;
}


int capturedAxis = -1;// x: 0, y: 1, z: 2

vector<int> capturedPtId(2, -1);// for hermite, bezier and spline curves, only the first element is use. for bezier surface, the second is used
const int ctlPtRadius = 3;

void CDemo5View::OnHermite()
{
	drawMode = Hermite;

	hermiteCtls.clear();

    capturedPtId[0] = -1;

	Invalidate();
}


void CDemo5View::OnBezier()
{
	// TODO: 在此添加命令处理程序代码
	drawMode = Bezier;

	bezierCtls.clear();

    capturedPtId[0] = -1;

	Invalidate();
}


void CDemo5View::OnSpline()
{
	// TODO: 在此添加命令处理程序代码
	drawMode = BSpline;

	splineCtls.clear();

    capturedPtId[0] = -1;

	Invalidate();
}

float ProjMat[4][4] = {
	{ 1,0,0,0 },{ 0,1,0,0 },{ 0,0,1,0 },{ 0,0,0,1 }
};

void VectorMatrixProduct(const float CubeVert[4], float CubeVert_Transformed[4], float Matrix[][4])
{
	for (int k = 0; k < 4; ++k)
	{
		CubeVert_Transformed[k] = 0;
		for (int kk = 0; kk < 4; ++kk)
		{
			CubeVert_Transformed[k] += CubeVert[kk] * Matrix[kk][k];
		}
	}
}

void SetUpProj_Perspective(float cx, float cy, float cz)
{
//	memcpy(ProjMat, IdentityMat, sizeof(float) * 16);

	const float d = -cz;

	ProjMat[2][0] = cx / d;
	ProjMat[2][1] = cy / d;
	ProjMat[2][2] = 0;
	ProjMat[2][3] = 1 / d;
}

CPoint proj(Point3 vert)
{
	float vert4[4] = { vert.x,vert.y,vert.z,1. };
	float tmpVec[4];
	VectorMatrixProduct(vert4, tmpVec, ProjMat);

	tmpVec[0] /= tmpVec[3];
	tmpVec[1] /= tmpVec[3];
	tmpVec[2] /= tmpVec[3];

	POINT vert2D;
	vert2D.x = (int)(tmpVec[0] + 0.5);
	vert2D.y = (int)(tmpVec[1] + .5);

	return vert2D;
}


const int numSegs = 20;


void CDemo5View::OnBezierSurface()
{
	drawMode = BezierSurface;

	// each time switch back to bezier surface mode, show the surface with last status
/*	bezierSurfaceCtls.clear();*/

	capturedAxis = -1;
	capturedPtId[0] = -1;
	capturedPtId[1] = -1;

	Invalidate();
}

bool withinDist(const CPoint& p, const CPoint& q, int dist)
{
	if (abs(p.x - q.x) < dist && abs(p.y - q.y) < dist)
		return true;
	else
		return false;
}

void updateXYZAxis(const vector<vector<Point3> >& bezierSurfaceCtls, const vector<int>& capturedPtId, vector<Point3>& axis)
{
	// compute the normal of control points
	// now, just let the xyz axis of world to be the local one

	axis[0] = Point3(1, 0, 0);
	axis[1] = Point3(0, 1, 0);
	axis[2] = Point3(0, 0, 1);
}

CPoint prePt;
vector<Point3> axis(3);
const CPoint screenOffset(150, 150);
const float axisLen = 50;
void CDemo5View::OnLButtonDown(UINT nFlags, CPoint point)
{
    switch (drawMode)
    {
    case Hermite:
        if (hermiteCtls.size() < 4)
            hermiteCtls.push_back(point);
        for (int k = 0; k < hermiteCtls.size(); ++k)
            if (withinDist(hermiteCtls[k], point, ctlPtRadius))
            {
                capturedPtId[0] = k;
                break;
            }
        break;
    case Bezier:
        if (bezierCtls.size() < 4)
            bezierCtls.push_back(point);
        for (int k = 0; k < bezierCtls.size(); ++k)
            if (withinDist(bezierCtls[k], point, ctlPtRadius))
            {
                capturedPtId[0] = k;
                break;
            }
        break;
    case BSpline:
		for (int k = 0; k < splineCtls.size(); ++k)
			if (withinDist(splineCtls[k], point, ctlPtRadius))
			{
				capturedPtId[0] = k;
				break;
			}

		if (capturedPtId[0] == -1)
			splineCtls.push_back(point);
        
        break;
    case BezierSurface:
		// the processing code is below
        break;
    default:
        break;
    }


	if (drawMode != BezierSurface)
    {
		Invalidate();
        CView::OnLButtonDown(nFlags,point);
        return;
    }

    // for bezierSurface mode
    // for both captured a point or not, consider if a target is captured in this cycle. if captured, try to move it, else hide the local axis

	bool getAxis = false;
	// try to see if one of the xyz local axis is captured.
	const int kr = capturedPtId[0];
	const int kc = capturedPtId[1];

	if (kr != -1 && kc != -1)
	{
		vector <CPoint> axis2D(3);
		Point3 capturePt = bezierSurfaceCtls[kr][kc];
		for (int k = 0; k < 3; ++k)
		{
			axis2D[k] = proj(capturePt + axisLen * axis[k]) + screenOffset;
		}

		for (int k = 0; k < 3; ++k) {
			if (withinDist(axis2D[k], point, ctlPtRadius)) {
				capturedAxis = k;
				prePt = point;

				getAxis = true;
				break;
			}
		}
	}

	if (getAxis)
	{
		Invalidate();
		CView::OnLButtonDown(nFlags, point);

		return;
	}
	else
	{
		bool getPoint = false;
		vector <vector<CPoint>> surfaceCtls2D(4);

		for (int k = 0; k < 4; ++k) {
			vector <CPoint> oneRow(4);
			for (int m = 0; m < 4; ++m) {
				oneRow[m] = proj(bezierSurfaceCtls[k][m]) + screenOffset;
			}

			surfaceCtls2D[k] = oneRow;
		}

		for (int k = 0; k < 4; ++k) {
			for (int m = 0; m < 4; ++m) {
				if (withinDist(surfaceCtls2D[k][m], point, ctlPtRadius)) {
					capturedPtId[0] = k;
					capturedPtId[1] = m;

					getPoint = true;
					break;
				}
			}

			if (getPoint)
				break;
		}
		if (getPoint)
		{
			updateXYZAxis(bezierSurfaceCtls, capturedPtId, axis);
		}
		else
		{
			capturedPtId[0] = -1;
			capturedPtId[1] = -1;
		}
	}

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

void CDemo5View::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (drawMode != BezierSurface) {
        capturedPtId[0] = -1;
        capturedPtId[1] = -1;
    }
    else
    {
        capturedAxis = -1;
    }


	CView::OnLButtonUp(nFlags, point);
}



void CDemo5View::OnMouseMove(UINT nFlags, CPoint point)
{
	if (-1 == capturedPtId[0])
		return;
	if (drawMode == BezierSurface && (-1 == capturedPtId[1] || -1 == capturedAxis))
	    return;

	switch (drawMode)
	{
	case Hermite:
		hermiteCtls[capturedPtId[0]] = point;
		break;
	case Bezier:
		bezierCtls[capturedPtId[0]] = point;
		break;
	case BSpline:
		splineCtls[capturedPtId[0]] = point;
		break;
	case BezierSurface:
		// process below
		break;
	default:
		break;
	}

	Invalidate();
	if (drawMode != BezierSurface)
    {
        CView::OnMouseMove(nFlags,point);
        return;
    }

    float delta = capturedAxis == 0 ? point.x - prePt.x : point.y - prePt.y;
	prePt = point;

	bezierSurfaceCtls[capturedPtId[0]][capturedPtId[1]] += delta * axis[capturedAxis];

	Invalidate();

	CView::OnMouseMove(nFlags, point);
}


void CDemo5View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDemo5View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{


	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}



// CDemo5View 绘图
// vec: 4, matrix: 4 by 4
// memory allocated beforehand
void vectorMatrixProduct(const vector<float>& vec, const vector<vector<float> > & mat, vector<float>& rst)
{
	for (int k = 0; k < 4; ++k)
	{
		rst[k] = 0;
		for (int m = 0; m < 4; ++m)
			rst[k] += vec[m] * mat[m][k];
	}
}

// For all kinds of curves and surface: to get better curve, use some comple alsorithm for drawing,
// not just the equally segment one.

void DrawHermite(const vector<CPoint>& hermiteCtls,CDC* pDC)
{
	if (hermiteCtls.empty())
		return;

	

	CBrush brush(RGB(128, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	CPen penPoly(PS_SOLID, 2, RGB(10, 10, 128));
	CPen* oldPen = nullptr;
	// draw poly
	if (hermiteCtls.size() > 0)
	{
		pDC->Ellipse(hermiteCtls[0].x - ctlPtRadius, hermiteCtls[0].y - ctlPtRadius, hermiteCtls[0].x + ctlPtRadius, hermiteCtls[0].y + ctlPtRadius);
	}
	if (hermiteCtls.size() > 1)
	{
		pDC->Ellipse(hermiteCtls[1].x - ctlPtRadius, hermiteCtls[1].y - ctlPtRadius, hermiteCtls[1].x + ctlPtRadius, hermiteCtls[1].y + ctlPtRadius);

		pDC->MoveTo(hermiteCtls[0]);
		pDC->LineTo(hermiteCtls[1]);
	}
	if (hermiteCtls.size() > 2)
	{
		pDC->Ellipse(hermiteCtls[2].x - ctlPtRadius, hermiteCtls[2].y - ctlPtRadius, hermiteCtls[2].x + ctlPtRadius, hermiteCtls[2].y + ctlPtRadius);
	}
	if (hermiteCtls.size() > 3)
	{
		pDC->Ellipse(hermiteCtls[3].x - ctlPtRadius, hermiteCtls[3].y - ctlPtRadius, hermiteCtls[3].x + ctlPtRadius, hermiteCtls[3].y + ctlPtRadius);

		pDC->MoveTo(hermiteCtls[2]);
		pDC->LineTo(hermiteCtls[3]);
	}

	pDC->SelectObject(oldPen);

	if (hermiteCtls.size() != 4)
		return;

	const float factor = 1.;
	vector<CPoint> realCtls(4);
	realCtls[0] = hermiteCtls[0];
	realCtls[1] = hermiteCtls[1] - hermiteCtls[0];
	realCtls[2] = hermiteCtls[2];
	realCtls[3] = hermiteCtls[3] - hermiteCtls[2];
		
	vector<vector<float> > mat(4);
	vector<float> mat0(4);
	mat0[0] = 2; mat0[1] = -2; mat0[2] = 1; mat0[3] = 1;
	mat[0] = mat0;

	vector<float> mat1(4);
	mat1[0] = -3; mat1[1] = 3; mat1[2] = -2; mat1[3] = -1;
	mat[1] = mat1;

	vector<float> mat2(4);
	mat2[0] = 0; mat2[1] = 0; mat2[2] = 1; mat2[3] = 0;
	mat[2] = mat2;

	vector<float> mat3(4);
	mat3[0] = 1; mat3[1] = 0; mat3[2] = 0; mat3[3] = 0;
	mat[3] = mat3;

	CPen penCurve(PS_SOLID, 2, RGB(128, 10, 10));
	oldPen = pDC->SelectObject(&penCurve);
	// draw curve
	vector<float> uvec(4);
	float u = 0;
	vector<float> umat(4);
	float curPt[2]{ 0,0 };
	// to get more beautiful effect, use other method for computing the points on the curve !!
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
		
		curPt[0] += realCtls[0].x * umat[0];
		curPt[0] += realCtls[2].x * umat[1];
		curPt[0] += realCtls[1].x * umat[2];
		curPt[0] += realCtls[3].x * umat[3];

		curPt[1] += realCtls[0].y * umat[0];
		curPt[1] += realCtls[2].y * umat[1];
		curPt[1] += realCtls[1].y * umat[2];
		curPt[1] += realCtls[3].y * umat[3];
		
		if (k == 0)
			pDC->MoveTo(curPt[0] + .5,curPt[1] + .5);
		else
			pDC->LineTo(curPt[0] + .5,curPt[1] + .5);
	}



	pDC->SelectObject(oldPen);
}

void DrawBezierCurve(const vector<CPoint>& bezierCtls, CDC* pDC)
{
	if (bezierCtls.empty())
		return;

	CBrush brush(RGB(128, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	CPen penPoly(PS_SOLID, 2, RGB(10, 10, 128));
	CPen* oldPen = nullptr;
	// draw poly
	for (int k = 0; k < bezierCtls.size(); ++k)
	{
		pDC->Ellipse(bezierCtls[k].x - ctlPtRadius, bezierCtls[k].y - ctlPtRadius, bezierCtls[k].x + ctlPtRadius, bezierCtls[k].y + ctlPtRadius);
	}
	pDC->MoveTo(bezierCtls[0]);
	for (int k = 1; k < bezierCtls.size(); ++k)
		pDC->LineTo(bezierCtls[k]);

	pDC->SelectObject(oldPen);

	if (bezierCtls.size() != 4)
		return;

	vector<vector<float> > mat(4);
	vector<float> mat0(4);
	mat0[0] = -1; mat0[1] = 3; mat0[2] = -3; mat0[3] = 1;
	mat[0] = mat0;

	vector<float> mat1(4);
	mat1[0] = 3; mat1[1] = -6; mat1[2] = 3; mat1[3] = 0;
	mat[1] = mat1;

	vector<float> mat2(4);
	mat2[0] = -3; mat2[1] = 3; mat2[2] = 0; mat2[3] = 0;
	mat[2] = mat2;

	vector<float> mat3(4);
	mat3[0] = 1; mat3[1] = 0; mat3[2] = 0; mat3[3] = 0;
	mat[3] = mat3;

	CPen penCurve(PS_SOLID, 2, RGB(128, 10, 10));
	oldPen = pDC->SelectObject(&penCurve);
	// draw curve
	vector<float> uvec(4);
	float u = 0;
	vector<float> umat(4);
	float curPt[2]{ 0,0 };
	// to get more beautiful effect, use other method for computing the points on the curve !!
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
			curPt[0] += bezierCtls[m].x * umat[m];
			curPt[1] += bezierCtls[m].y * umat[m];
		}

		if (k == 0)
			pDC->MoveTo(curPt[0] + .5,curPt[1] + .5);
		else
			pDC->LineTo(curPt[0] + .5,curPt[1] + .5);
	}



	pDC->SelectObject(oldPen);
}

void DrawBSpline(const vector<CPoint>& splineCtls, CDC* pDC)
{
	if (splineCtls.empty())
		return;

	CBrush brush(RGB(128, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	CPen penPoly(PS_SOLID, 2, RGB(10, 10, 128));
	CPen* oldPen = nullptr;
	// draw poly
	for (int k = 0; k < splineCtls.size(); ++k)
	{
		pDC->Ellipse(splineCtls[k].x - ctlPtRadius, splineCtls[k].y - ctlPtRadius, splineCtls[k].x + ctlPtRadius, splineCtls[k].y + ctlPtRadius);
	}
	pDC->MoveTo(splineCtls[0]);
	for (int k = 1; k < splineCtls.size(); ++k)
		pDC->LineTo(splineCtls[k]);

	pDC->SelectObject(oldPen);

	if (splineCtls.size() < 4)
		return;

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
	
	// to get more beautiful effect, use other method for computing the points on the curve !!
	for (int kc = 0; kc < splineCtls.size() - 4 + 1; ++kc)
	{
		vector<float> uvec(4);
		float u = 0;
		vector<float> umat(4);
		float curPt[2]{ 0,0 };

		vector<CPoint> curCtls(4);
		for (int k = 0; k < 4; ++k)
			curCtls[k] = splineCtls[kc + k];

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

void DrawBezierCurve(const vector<Point3>& bezierCtls, CDC* pDC)
{
	if (bezierCtls.empty())
		return;

	CBrush brush(RGB(128, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	CPen penPoly(PS_SOLID, 2, RGB(10, 10, 128));
	CPen* oldPen = nullptr;
	// draw poly
	

	if (bezierCtls.size() != 4)
		return;

	vector<vector<float> > mat(4);
	vector<float> mat0(4);
	mat0[0] = -1; mat0[1] = 3; mat0[2] = -3; mat0[3] = 1;
	mat[0] = mat0;

	vector<float> mat1(4);
	mat1[0] = 3; mat1[1] = -6; mat1[2] = 3; mat1[3] = 0;
	mat[1] = mat1;

	vector<float> mat2(4);
	mat2[0] = -3; mat2[1] = 3; mat2[2] = 0; mat2[3] = 0;
	mat[2] = mat2;

	vector<float> mat3(4);
	mat3[0] = 1; mat3[1] = 0; mat3[2] = 0; mat3[3] = 0;
	mat[3] = mat3;

	CPen penCurve(PS_SOLID, 2, RGB(128, 10, 10));
	oldPen = pDC->SelectObject(&penCurve);
	// draw curve
	vector<float> uvec(4);
	float u = 0;
	vector<float> umat(4);
	Point3 curPt;
	// to get more beautiful effect, use other method for computing the points on the curve !!
	for (int k = 0; k <= numSegs; ++k)
	{
		u = 1. / numSegs * k;
		uvec[0] = u * u * u;
		uvec[1] = u * u;
		uvec[2] = u;
		uvec[3] = 1.;

		vectorMatrixProduct(uvec, mat, umat);

		curPt.x = 0;
		curPt.y = 0;
		curPt.z = 0;

		for (int m = 0; m < 4; ++m)
		{
			curPt.x += bezierCtls[m].x * umat[m];
			curPt.y += bezierCtls[m].y * umat[m];
			curPt.z += bezierCtls[m].z * umat[m];
		}


		CPoint curPt2D = proj(curPt);
		curPt2D.x += screenOffset.x;
		curPt2D.y += screenOffset.y;
		if (k == 0)
			pDC->MoveTo(curPt2D);
		else
			pDC->LineTo(curPt2D);
	}



	pDC->SelectObject(oldPen);
}

void DrawBezierSurface(const vector<vector<Point3> >& bezierSurfaceCtls, CDC* pDC)
{
	CBrush brush(RGB(128, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);

	CPen penCtlPoly(PS_SOLID, 2, RGB(10, 10, 128));
	CPen penSurfPoly(PS_SOLID, 2, RGB(10, 128, 10));
	CPen* oldPen = nullptr;

	pDC->SelectObject(penCtlPoly);
	
	for (int k = 0; k < 4; ++k)
	{
		for (int m = 0; m < 4; ++m)
		{
			CPoint curPt = proj(bezierSurfaceCtls[k][m]);
			curPt.x += screenOffset.x;
			curPt.y += screenOffset.y;
			pDC->Ellipse(curPt.x - ctlPtRadius, curPt.y - ctlPtRadius, curPt.x + ctlPtRadius, curPt.y + ctlPtRadius);
		}
	}

	for (int k = 0; k < 4; ++k)
	{
		CPoint curPt = proj(bezierSurfaceCtls[k][0]);
		curPt.x += screenOffset.x;
		curPt.y += screenOffset.y;
		pDC->MoveTo(curPt);
		for (int m = 1; m < 4; ++m)
		{
			curPt = proj(bezierSurfaceCtls[k][m]);
			curPt.x += screenOffset.x;
			curPt.y += screenOffset.y;

			pDC->LineTo(curPt);
		}
	}

	for (int m = 0; m < 4; ++m)
	{
		CPoint curPt = proj(bezierSurfaceCtls[0][m]);
		curPt.x += screenOffset.x;
		curPt.y += screenOffset.y;
		pDC->MoveTo(curPt);
		for (int k = 1; k < 4; ++k)
		{
			curPt = proj(bezierSurfaceCtls[k][m]);
			curPt.x += screenOffset.x;
			curPt.y += screenOffset.y;

			pDC->LineTo(curPt);
		}
	}

	// here, we should use formula of surface, not curve
	for (int k = 0; k < 4; ++k)
	{
		DrawBezierCurve(bezierSurfaceCtls[k], pDC);
	}
	for (int m = 0; m < 4; ++m)
	{
		vector<Point3> ctls(4);
		for (int k = 0; k < 4; ++k)
		{
			ctls[k] = bezierSurfaceCtls[k][m];
		}

		DrawBezierCurve(ctls, pDC);
	}
}

void DrawAxis(const vector<vector<Point3> >& bezierSurfaceCtls, const vector<int>& capturedPtId, const vector<Point3>& axis, CDC* pDC)
{
	CBrush brush(RGB(128, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);

	CPen penX(PS_SOLID, 2, RGB(255, 0, 0));
	CPen penY(PS_SOLID, 2, RGB(0, 255, 0));
	CPen penZ(PS_SOLID, 2, RGB(0, 0, 255));

	CPen* oldPen = nullptr;
	
	Point3 center3 = bezierSurfaceCtls[capturedPtId[0]][capturedPtId[1]];
	Point3 x3 = center3 + axisLen * axis[0];
	Point3 y3 = center3 + axisLen * axis[1];
	Point3 z3 = center3 + axisLen * axis[2];

	CPoint center2 = proj(center3) + screenOffset;
	CPoint x2 = proj(x3) + screenOffset;
	CPoint y2 = proj(y3) + screenOffset;
	CPoint z2 = proj(z3) + screenOffset;

	// draw axis
	pDC->SelectObject(penX);
	pDC->MoveTo(center2);
	pDC->LineTo(x2);

	pDC->SelectObject(penY);
	pDC->MoveTo(center2);
	pDC->LineTo(y2);

	pDC->SelectObject(penZ);
	pDC->MoveTo(center2);
	pDC->LineTo(z2);

	// draw the arrows
	const CPoint rectLen(ctlPtRadius, ctlPtRadius);

	pDC->SelectObject(penX);
	pDC->Rectangle(CRect(CPoint(x2 - rectLen),CPoint( x2 + rectLen)));

	pDC->SelectObject(penY);
	pDC->Rectangle(CRect(CPoint(y2 - rectLen), CPoint(y2 + rectLen)));

	pDC->SelectObject(penZ);
	pDC->Rectangle(CRect(CPoint(z2 - rectLen), CPoint(z2 + rectLen)));

}

void CDemo5View::OnDraw(CDC* pDC)
{
	CDemo5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	switch (drawMode)
	{
	case Hermite:
		DrawHermite(hermiteCtls,pDC);
		break;
	case Bezier:
		DrawBezierCurve(bezierCtls, pDC);
		break;
	case BSpline:
		DrawBSpline(splineCtls, pDC);
		break;
	case BezierSurface:
		SetUpProj_Perspective(0, 0, -300);
		DrawBezierSurface(bezierSurfaceCtls, pDC);
		if (capturedPtId[0] != -1 && -1 != capturedPtId[1])
		{
			DrawAxis(bezierSurfaceCtls, capturedPtId,axis, pDC);
		}
		
		break;
	default:

		break;

	}

	// TODO: 在此处为本机数据添加绘制代码
}





