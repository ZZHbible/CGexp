
// Demo3View.cpp: CDemo3View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Demo3.h"
#endif

#include "Demo3Doc.h"
#include "Demo3View.h"

#include "math.h"

#include "SetPerspectiveCenter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemo3View

IMPLEMENT_DYNCREATE(CDemo3View, CView)

BEGIN_MESSAGE_MAP(CDemo3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDemo3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_TRANSLATE, &CDemo3View::OnTranslate)
	ON_COMMAND(ID_ROTATE, &CDemo3View::OnRotate)
	ON_COMMAND(ID_SCALE, &CDemo3View::OnScale)
	ON_COMMAND(ID_PROJ_XIE, &CDemo3View::OnProjXie)
	ON_COMMAND(ID_PROJ_PERSPECTIVE, &CDemo3View::OnProjPerspective)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_RESET_TRANSFORM, &CDemo3View::OnResetTransform)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

const float CubeVerts[8][4] = {
	{0,0,0,1},{100,0,0,1},{100,100,0,1},{0,100,0,1},
{0,0,100,1},{100,0,100,1},{100,100,100,1},{0,100,100,1} 
};

const int CubeFaces[6][4] = {
	{0,1,2,3},{4,7,6,5},{0,3,7,4},{5,6,2,1},{3,2,6,7},{5,1,0,4}
};

const float IdentityMat[4][4] = {
		{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}
};

float TransMat[4][4] = {
	{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}
};

float RotateMat[4][4] = {
	{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}
};

float ScaleMat[4][4] = {
	{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}
};

float ProjMat[4][4] = {
	{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}
};

int TransMode = 1;// 0: translate, 1: rotate, 2:scale, -1: none
int ProjMode = 1;// 0: xieerce, 1: perspective
float Cx = 0., Cy = 0., Cz = -300;

// CDemo3View 构造/析构

CDemo3View::CDemo3View() noexcept
{
#define PI 3.1415926
	float l = .5;
	float alpha = PI / 4;
	ProjMat[2][2] = 0;
	ProjMat[2][0] = l * cos(alpha);
	ProjMat[2][1] = l * sin(alpha);
#undef PI

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

// CDemo3View 绘图

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


POINT Proj(float vert[4])
{
	float tmpVec[4];
	VectorMatrixProduct(vert, tmpVec, ProjMat);

	tmpVec[0] /= tmpVec[3];
	tmpVec[1] /= tmpVec[3];
	tmpVec[2] /= tmpVec[3];

	POINT vert2D;
	vert2D.x = (int)(tmpVec[0] + 0.5);
	vert2D.y = (int)(tmpVec[1] + .5);

	return vert2D;
}

void DrawCube(CDC* pDC, POINT CubeVerts2D[], const int CubeFaces[][4])
{
	for (int kf = 0; kf < 6; ++kf)
	{
		pDC->MoveTo(CubeVerts2D[CubeFaces[kf][0]]);
		for (int kv = 1; kv < 4; ++kv)
		{
			pDC->LineTo(CubeVerts2D[CubeFaces[kf][kv]]);
		}
		pDC->LineTo(CubeVerts2D[CubeFaces[kf][0]]);
	}

	pDC->Ellipse(CubeVerts2D[0].x - 5, CubeVerts2D[0].y - 5, CubeVerts2D[0].x + 5, CubeVerts2D[0].y + 5);
	pDC->Rectangle(CubeVerts2D[7].x - 5, CubeVerts2D[7].y - 5, CubeVerts2D[7].x + 5, CubeVerts2D[7].y + 5);
}

void DrawCube(CDC* pDC, POINT CubeVerts2D[], const int CubeFaces[][4],const bool shouldDraw[6])
{
	CPen newPen(PS_SOLID, 3, RGB(0, 0, 128));
	CBrush newBrush(HS_DIAGCROSS, RGB(128, 128, 0));

	CPen* oldPen = pDC->SelectObject(&newPen);
	CBrush* oldBrush = pDC->SelectObject(&newBrush);

	for (int kf = 0; kf < 6; ++kf)
	{
		if (!shouldDraw[kf])
			continue;

		POINT tmpPoly[4];

		for (int kv = 0; kv < 4; ++kv)
		{
			tmpPoly[kv] = CubeVerts2D[CubeFaces[kf][kv]];
		}
				
		pDC->Polygon(tmpPoly, 4);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void TransformVerts(const float CubeVert[4], float CubeVert_Transformed[4], float ScaleMat[][4],
	float RotateMat[][4], float TransMat [][4])
{
	
	float tmpVec[4];
	VectorMatrixProduct(CubeVert, tmpVec,ScaleMat);
	float tmpVec2[4];
	VectorMatrixProduct(tmpVec, tmpVec2, RotateMat);
	VectorMatrixProduct(tmpVec2, CubeVert_Transformed, TransMat);
}

float DotProduct(const float U[3], const float V[3])
{
	return U[0] * V[0] + U[1] * V[1] + U[2] * V[2];
}

void CrossPruduct(const float U[3], const float V[3], float result[3]) {
	result[0] = U[1] * V[2] - U[2] * V[1];
	result[1] = U[2] * V[0] - U[0] * V[2];
	result[2] = U[0] * V[1] - U[1] * V[0];
}

void ComputeCenter(float empPoly[4][3], float center[3])
{
	center[0] = center[1] = center[2] = 0.;
	for (int k = 0; k < 4; ++k)
	{
		center[0] += empPoly[k][0];
		center[1] += empPoly[k][1];
		center[2] += empPoly[k][2];
	}

	center[0] /= 4.;
	center[1] /= 4.;
	center[2] /= 4.;
}

void ComputeNormal(float tmpPoly[4][3],float normal[3])
{
	const float P12[3] = { tmpPoly[2][0] - tmpPoly[1][0],tmpPoly[2][1] - tmpPoly[1][1], tmpPoly[2][2] - tmpPoly[1][2] };
	const float P10[3] = { tmpPoly[0][0] - tmpPoly[1][0],tmpPoly[0][1] - tmpPoly[1][1], tmpPoly[0][2] - tmpPoly[1][2] };
	
	CrossPruduct(P12, P10, normal);

	float len = DotProduct(normal, normal);
	normal[0] /= len;
	normal[1] /= len;
	normal[2] /= len;
}

void DetermineVisibility(float CubeVerts_Transformed[][4],const int CubeFaces[][4], bool shouldDraw[])
{
	for (int kf = 0; kf < 6; ++kf)
	{
		float tmpPoly[4][3];

		for (int kv = 0; kv < 4; ++kv)
		{
			for (int k = 0; k < 3; ++k)
				tmpPoly[kv][k] = CubeVerts_Transformed[CubeFaces[kf][kv]][k];
		}

		float normal[3];
		ComputeNormal(tmpPoly, normal);

		
		float viewDir3[3];

		if (ProjMode == 0)// xieerce
		{
// 			float viewDir[4] = { 0,0,1,1 };
 			float viewDir2[4];
// 			VectorMatrixProduct(viewDir, viewDir2, ProjMat);
			// 只是大概的方向。实际上应该是斜二测的投影方向
			viewDir2[0] = -0.5;
			viewDir2[1] = -0.5;
			viewDir2[2] = 1.;
			viewDir2[3] = 1.;
			viewDir3[0] = viewDir2[0] / viewDir2[3];
			viewDir3[1] = viewDir2[1] / viewDir2[3];
			viewDir3[2] = viewDir2[2] / viewDir2[3];
		}
		else// perspective
		{
			float polyCenter[3];
			ComputeCenter(tmpPoly, polyCenter);

			viewDir3[0] = polyCenter[0] - Cx;
			viewDir3[1] = polyCenter[1] - Cy;
			viewDir3[2] = polyCenter[2] - Cz;
		}
		float len = DotProduct(viewDir3, viewDir3);
		viewDir3[0] /= len;
		viewDir3[1] /= len;
		viewDir3[2] /= len;

		float dot = DotProduct(viewDir3, normal);

		if (dot > 0)
			shouldDraw[kf] = true;
		else
			shouldDraw[kf] = false;

	}
}


void SetUpProj_Xieerce()
{
#define PI 3.1415926
	float l = .5;
	float alpha = PI / 4;
	memcpy(ProjMat, IdentityMat, sizeof(float) * 16);
	ProjMat[2][2] = 0;
	ProjMat[2][0] = l * cos(alpha);
	ProjMat[2][1] = l * sin(alpha);
#undef PI
}


void SetUpProj_Perspective(float cx, float cy, float cz)
{
	memcpy(ProjMat, IdentityMat, sizeof(float) * 16);

	const float d = -cz;

	ProjMat[2][0] = cx / d;
	ProjMat[2][1] = cy / d;
	ProjMat[2][2] = 0;
	ProjMat[2][3] = 1 / d;
}

void CDemo3View::OnDraw(CDC* pDC)
{
	CDemo3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->TextOutW(0,0,CString("使用方法：鼠标和键盘（上下左右+PageUp+PageDown)"));

 


	float CubeVerts_Transformed[8][4];
	for (int k = 0; k < 8; ++k)
	{
		//通过读入三个矩阵的值来确定当前三维立方体的位点
		TransformVerts(CubeVerts[k], CubeVerts_Transformed[k],ScaleMat,RotateMat,TransMat);
	}

	bool shouldDraw[6] = { true,true,true,true,true,true };
	//判断哪些面应该被画出来
	DetermineVisibility(CubeVerts_Transformed, CubeFaces, shouldDraw);

	POINT CubeVerts2D[8];
	for (int k = 0; k < 8; ++k)
	{
		CubeVerts2D[k] = Proj(CubeVerts_Transformed[k]);

		CubeVerts2D[k].x += 300;
		CubeVerts2D[k].y += 300;
	}
	//根据位点画出图像
	DrawCube(pDC,CubeVerts2D, CubeFaces);

	for (int k = 0; k < 8; ++k)
	{
		CubeVerts2D[k].x += 200;
	}
	DrawCube(pDC, CubeVerts2D, CubeFaces,shouldDraw);
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


void CDemo3View::OnTranslate()
{
	TransMode = 0;
}


void CDemo3View::OnRotate()
{
	TransMode = 1;
}


void CDemo3View::OnScale()
{
	TransMode = 2;
}


void CDemo3View::OnProjXie()
{
	ProjMode = 0;
//	SetUpProj_Xieerce();
	Invalidate();
}


void CDemo3View::OnProjPerspective()
{
	SetPerspectiveCenter dlg;

	dlg.CX = Cx;
	dlg.CY = Cy;
	dlg.CZ = Cz;
	if (IDOK == dlg.DoModal())
	{
		ProjMode = 1;

		Cx = dlg.CX;
		Cy = dlg.CY;
		Cz = dlg.CZ;

//		SetUpProj_Perspective(cx, cy, cz);

		Invalidate();
	}
}

void MatrixMatrixProduct(const float tmpRotateMat[][4], const float tmpMat[][4], float RotateMat[][4])
{
	for (int kr = 0; kr < 4; ++kr)
	{
		for (int kc = 0; kc < 4; ++kc)
		{
			RotateMat[kr][kc] = 0;
			for (int k = 0; k < 4; ++k)
			{
				RotateMat[kr][kc] += tmpRotateMat[kr][k] * tmpMat[k][kc];
			}
		}
	}
}


void CDemo3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#define PI 3.1415926
	const float theta = 2. / 180. * PI;
	switch (nChar)
	{
	case VK_UP:
		switch (TransMode)
		{
		case 0:
			TransMat[3][1] -= 10.;
			break;
		case 1:
		{
			float tmpMat[4][4] = {
				{1,0,0,0},{0,cos(theta),sin(theta),0},{0,-sin(theta),cos(theta),0},{0,0,0,1}
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat,RotateMat);
		}
			break;
		case 2:
			ScaleMat[1][1] *= 1.1;
			break;
		default:
			break;
		}
		
		break;

	case VK_DOWN:
		switch (TransMode)
		{
		case 0:
			TransMat[3][1] += 10.;
			break;
		case 1:
		{
			float tmpMat[4][4] = {
				{1,0,0,0},{0,cos(-theta),sin(-theta),0},{0,-sin(-theta),cos(-theta),0},{0,0,0,1}
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}
			break;
		case 2:
//			if (ScaleMat[1][1] > 0.1)
				ScaleMat[1][1] *= 0.9;
			break;
		default:
			break;
		}
		
		break;

	case VK_RIGHT:
		switch (TransMode)
		{
		case 0:
			TransMat[3][0] += 10.;
			break;
		case 1:
		{
			float tmpMat[4][4] = {
				{cos(theta),0,-sin(theta),0},{0,1,0,0},{sin(theta),0,cos(theta),0},{0,0,0,1}
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}
			break;
		case 2:
			ScaleMat[0][0] *= 1.1;
			break;
		default:
			break;
		}
		
		break;

	case VK_LEFT:
		switch (TransMode)
		{
		case 0:
			TransMat[3][0] -= 10.;
			break;
		case 1:
		{
			float tmpMat[4][4] = {
				{cos(-theta),0,-sin(-theta),0},{0,1,0,0},{sin(-theta),0,cos(-theta),0},{0,0,0,1}
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}
			break;
		case 2:
//			if (ScaleMat[0][0] > 0.1)
			ScaleMat[0][0] *= 0.9;
			break;
		default:
			break;
		}
		
		break;

	case VK_PRIOR:
		switch (TransMode)
		{
		case 0:
			TransMat[3][2] -= 10.;
			break;
		case 1:
		{
			float tmpMat[4][4] = {
				{cos(theta),sin(theta),0,0},{-sin(theta),cos(theta),0,0},{0,0,1,0},{0,0,0,1}
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}
			break;
		case 2:
			ScaleMat[2][2] *= 1.1;
			break;
		default:
			break;
		}
		
		break;

	case VK_NEXT:
		switch (TransMode)
		{
		case 0:
			TransMat[3][2] += 10.;
			break;
		case 1:
		{
			float tmpMat[4][4] = {
				{cos(-theta),sin(-theta),0,0},{-sin(-theta),cos(-theta),0,0},{0,0,1,0},{0,0,0,1}
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}
			break;
		case 2:
//			if (ScaleMat[2][2] > 0.1)
			ScaleMat[2][2] *= 0.9;
			break;
		default:
			break;
		}
		
		break;
	}

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDemo3View::OnResetTransform()
{
	memcpy(TransMat, IdentityMat, sizeof(float) * 16);
	memcpy(RotateMat, IdentityMat,sizeof(float) * 16);
	memcpy(ScaleMat, IdentityMat, sizeof(float) * 16);

	Invalidate();
}


BOOL CDemo3View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0)
	{
		ScaleMat[0][0] *= 1.1;
		ScaleMat[1][1] *= 1.1;
		ScaleMat[2][2] *= 1.1;
	}
	else if (zDelta < 0)
	{
		ScaleMat[0][0] *= 0.9;
		ScaleMat[1][1] *= 0.9;
		ScaleMat[2][2] *= 0.9;
	}

	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


CPoint prePoint;

void CDemo3View::OnMouseMove(UINT nFlags, CPoint point)
{
	float theta = 1. * 3.1415926 / 180.;
	const int xdelta = point.x - prePoint.x;
	const int ydelta = point.y - prePoint.y;
	if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		if (xdelta > 0)
		{
			float tmpMat[4][4] = {
				{ cos(-theta),0,-sin(-theta),0 },{ 0,1,0,0 },{ sin(-theta),0,cos(-theta),0 },{ 0,0,0,1 }
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}
		else if (xdelta < 0)
		{
			float tmpMat[4][4] = {
				{ cos(theta),0,-sin(theta),0 },{ 0,1,0,0 },{ sin(theta),0,cos(theta),0 },{ 0,0,0,1 }
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}

		if (ydelta > 0)
		{
			float tmpMat[4][4] = {
				{ 1,0,0,0 },{ 0,cos(theta),sin(theta),0 },{ 0,-sin(theta),cos(theta),0 },{ 0,0,0,1 }
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}
		else if (ydelta < 0)
		{
			float tmpMat[4][4] = {
				{ 1,0,0,0 },{ 0,cos(-theta),sin(-theta),0 },{ 0,-sin(-theta),cos(-theta),0 },{ 0,0,0,1 }
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}

		prePoint = point;
		Invalidate();
	}
	else if ((nFlags & MK_MBUTTON) == MK_MBUTTON)
	{
		if (xdelta > 0)
		{
			TransMat[3][0] += 8;
		}
		else if (xdelta < 0)
		{
			TransMat[3][0] -= 8;
		}

		if (ydelta > 0)
		{
			TransMat[3][1] += 8;
		}
		else if (ydelta < 0)
		{
			TransMat[3][1] -= 8;
		}

		prePoint = point;
		Invalidate();
	}
	else if ((nFlags & MK_RBUTTON) == MK_RBUTTON)
	{
		int trueDelta = abs(xdelta) > abs(ydelta) ? -xdelta : ydelta;
		theta *= 2.;
		if (trueDelta > 0)
		{
			float tmpMat[4][4] = {
				{ cos(theta),sin(theta),0,0 },{ -sin(theta),cos(theta),0,0 },{ 0,0,1,0 },{ 0,0,0,1 }
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}
		else if (trueDelta < 0)
		{
			float tmpMat[4][4] = {
				{ cos(-theta),sin(-theta),0,0 },{ -sin(-theta),cos(-theta),0,0 },{ 0,0,1,0 },{ 0,0,0,1 }
			};
			float tmpRotateMat[4][4];
			memcpy(tmpRotateMat, RotateMat, sizeof(float) * 16);
			MatrixMatrixProduct(tmpRotateMat, tmpMat, RotateMat);
		}
		prePoint = point;
		Invalidate();
	}

	

	CView::OnMouseMove(nFlags, point);
}


void CDemo3View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	prePoint = point;


	CView::OnLButtonDown(nFlags, point);
}


void CDemo3View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnLButtonUp(nFlags, point);
}


void CDemo3View::OnMButtonDown(UINT nFlags, CPoint point)
{
	prePoint = point;

	CView::OnMButtonDown(nFlags, point);
}


void CDemo3View::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnMButtonUp(nFlags, point);
}


void CDemo3View::OnRButtonDown(UINT nFlags, CPoint point)
{
	prePoint = point;

	CView::OnRButtonDown(nFlags, point);
}

void CDemo3View::OnRButtonUp(UINT  nFlags, CPoint point)
{

#if 0
	ClientToScreen(&point);
	OnContextMenu(this, point);
#elif 0
	CView::OnRButtonUp(nFlags, point);
#endif
}
