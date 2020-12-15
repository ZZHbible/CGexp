
// Demo3View.cpp : CDemo3View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Demo3.h"
#endif

#include "Demo3Doc.h"
#include "Demo3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Vector4.h"
#include "TransMatrix.h"

#include "SelectRotateAxis.h"
#include "SelectScaleAxis.h"
#include "SetProjCenter.h"


// CDemo3View

IMPLEMENT_DYNCREATE(CDemo3View, CView)

BEGIN_MESSAGE_MAP(CDemo3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDemo3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_PROJ_XIE2, &CDemo3View::OnProjXie2)
	ON_COMMAND(ID_PROJ_PERSPECTIVE, &CDemo3View::OnProjPerspective)
	ON_COMMAND(ID_TRANSLATE, &CDemo3View::OnTranslate)
	ON_COMMAND(ID_ROTATE, &CDemo3View::OnRotate)
	ON_COMMAND(ID_SCALE, &CDemo3View::OnScale)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CDemo3View 构造/析构

float _CubeVerts[8][4] = { {0,0,0,1},{100,0,0,1 },{100,100,0,1 },{0,100,0,1 },
{ 0,0,100,1 },{ 100,0,100 ,1 },{ 100,100,100 ,1 },{ 0,100,100 ,1 } };
Vector4 CubeVerts[8] = { Vector4(_CubeVerts[0]),Vector4(_CubeVerts[1]),Vector4(_CubeVerts[2]), Vector4(_CubeVerts[3]), Vector4(_CubeVerts[4]), Vector4(_CubeVerts[5]), Vector4(_CubeVerts[6]),Vector4(_CubeVerts[7]) };
int CubeFaces[6][4] = { {0,1,2,3}, {4,7,6,5}, {4,0,3,7},{1,5,6,2},{3,2,6,7},{4,5,1,0} };
TransMatrix ScaleMat;
TransMatrix RotateMat;
TransMatrix TransMat;
TransMatrix ProjMat = TransMatrix::parallel_project();

CDemo3View::CDemo3View() :transMode(Translate), transDelta(10),rotateDelta(10),scaleDelta(.2)
{
	// TODO: 在此处添加构造代码
	cout << "Testing vector" << endl;
	Vector4 vec;
	vec.x() = 1.;
	vec.w() = 2.;
	cout << vec << endl;
	vec.dividedByW();
	cout << vec << endl;

	TransMatrix mat, mat2;
	cout << "Testing matrix" << endl;
	mat = TransMatrix::identity();
	mat = TransMatrix::translate(1, 2, 3);
	mat2 = TransMatrix::scale(2, 3, 4);
	//    mat = TransMatrix::rotate(0,10);
	cout << mat * mat2;

	cout << mat;

}

CDemo3View::~CDemo3View()
{
}

BOOL CDemo3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDemo3View 绘制

void CDemo3View::DrawCube(CDC* pDC,POINT projedCubeVerts[], int CubeFaces[][4])
{
	for (int kf = 0; kf < 6; ++kf)
	{
		pDC->MoveTo(projedCubeVerts[CubeFaces[kf][0]]);
		for (int kv = 1; kv < 4; ++kv)
		{
			pDC->LineTo(projedCubeVerts[CubeFaces[kf][kv]]);
		}
		pDC->LineTo(projedCubeVerts[CubeFaces[kf][0]]);
	}
}

void CDemo3View::OnDraw(CDC* pDC)
{
	CDemo3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	POINT projedCubeVerts[8];
	Vector4 _transVerts;
	Vector4 _projVert;
	cout << "--------------------------" << endl;
	for (int kv = 0; kv < 8; ++kv)
	{
		cout << "Before Transform: " << endl;
		cout << CubeVerts[kv] << endl;
		_transVerts = CubeVerts[kv] * ScaleMat * RotateMat * TransMat;
		_projVert = _transVerts * ProjMat;
		cout << "After Transform: " << endl;
		cout << _transVerts << endl ;
		cout << "After Projection: " << endl;
		cout << _projVert << endl << endl;

		_projVert.dividedByW();
		projedCubeVerts[kv].x = (int)(_projVert.x() + 0.5);
		projedCubeVerts[kv].y = (int)(_projVert.y() + 0.5);
	}
	cout << "=====================================" << endl;

	DrawCube(pDC,projedCubeVerts, CubeFaces);

	// TODO: 在此处为本机数据添加绘制代码
}


// CDemo3View 打印


void CDemo3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDemo3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDemo3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDemo3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDemo3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDemo3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDemo3View 诊断

#ifdef _DEBUG
void CDemo3View::AssertValid() const
{
	CView::AssertValid();
}

void CDemo3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo3Doc* CDemo3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo3Doc)));
	return (CDemo3Doc*)m_pDocument;
}
#endif //_DEBUG


// CDemo3View 消息处理程序


void CDemo3View::OnProjXie2()
{
	ProjMat = TransMatrix::parallel_project();
}


void CDemo3View::OnProjPerspective()
{
	float cx, cy, cz;
	SetProjCenter dlg;
	if (IDOK == dlg.DoModal())
	{
		cx = dlg.cx;
		cy = dlg.cy;
		cz = dlg.cz;
	}
	else
		return;

	ProjMat = TransMatrix::perspective_project(cx, cy, cz);
}


void CDemo3View::OnTranslate()
{
	transMode = Translate;
}


void CDemo3View::OnRotate()
{
	transMode = Rotate;
#if 0
	SelectRotateAxis dlg;
	if (IDOK == dlg.DoModal())
	{
		switch (dlg.whichSelected)
		{
		case 0:
			cout << "Rotate Axis: X" << endl;
			break;
		case 1:
			cout << "Rotate Axis: Y" << endl;
			break;
		case 2:
			cout << "Rotate Axis: Z" << endl;
			break;
		default:
			cerr << "Fatal Error: rotate axis not determined." << endl;
			break;
		}
	}
#endif
}


void CDemo3View::OnScale()
{
	transMode = Scale;
#if 0
	SelectScaleAxis dlg;
	if (IDOK == dlg.DoModal())
	{
		switch (dlg.whichSelected)
		{
		case 0:
			cout << "Scale Axis: X" << endl;
			break;
		case 1:
			cout << "Scale Axis: Y" << endl;
			break;
		case 2:
			cout << "Scale Axis: Z" << endl;
			break;
		default:
			cerr << "Fatal Error: scale axis not determined." << endl;
			break;
		}
	}
#endif
}


void CDemo3View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#if 0
	if (nChar == 'a')
		cout << "a pressed" << endl;
	else if (nChar == 'A')
		cout << "A pressed" << endl;
	
	cout << (char)(nChar);
#endif

	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CDemo3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	TransMatrix transdeltaMat;
	TransMatrix rotatedeltaMat;
	TransMatrix scaledeltaMat;

	switch (nChar)
	{
	case VK_UP:
		transdeltaMat = TransMatrix::translate(0, transDelta, 0);//y+
		rotatedeltaMat = TransMatrix::rotate(0,rotateDelta);// x + 
		scaledeltaMat = TransMatrix::scale(1.,1 + scaleDelta, 1.);// y+
		break;
	case VK_DOWN:
		transdeltaMat = TransMatrix::translate(0, -transDelta, 0);//y-
		rotatedeltaMat = TransMatrix::rotate(0, -rotateDelta);// x - 
		scaledeltaMat = TransMatrix::scale(1., 1 - scaleDelta, 1.);// y-
		break;
	case VK_RIGHT:
		transdeltaMat = TransMatrix::translate(transDelta,0, 0);// x+
		rotatedeltaMat = TransMatrix::rotate(1, rotateDelta);// y + 
		scaledeltaMat = TransMatrix::scale(1 + scaleDelta, 1., 1.);// x+
		break;
	case VK_LEFT:
		transdeltaMat = TransMatrix::translate(-transDelta, 0, 0);// x-
		rotatedeltaMat = TransMatrix::rotate(1, -rotateDelta);// y - 
		scaledeltaMat = TransMatrix::scale(1 - scaleDelta, 1., 1.);// x-
		break;
	case VK_NEXT:
		transdeltaMat = TransMatrix::translate(0, 0, transDelta);// z+
		rotatedeltaMat = TransMatrix::rotate(2, rotateDelta);// z + 
		scaledeltaMat = TransMatrix::scale(1.,1.,1 + scaleDelta);// z+
		break;
	case VK_PRIOR:
		transdeltaMat = TransMatrix::translate(0, 0, -transDelta);// z-
		rotatedeltaMat = TransMatrix::rotate(2, -rotateDelta);// z - 
		scaledeltaMat = TransMatrix::scale(1., 1., 1 - scaleDelta);// z-
		break;
	
	default:
		break;

	}

	switch (transMode)
	{
	case Translate:
		TransMat = TransMat * transdeltaMat;
		break;
	case Rotate:
		RotateMat = RotateMat * rotatedeltaMat;
		break;
	case Scale:
		ScaleMat = ScaleMat * scaledeltaMat;
		break;
	default:
		break;
	}

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
