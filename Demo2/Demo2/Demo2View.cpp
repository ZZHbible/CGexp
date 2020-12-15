
// Demo2View.cpp: CDemo2View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Demo2.h"
#endif

#include "Demo2Doc.h"
#include "Demo2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <iostream>
#include <fstream>
#include <sstream>


// CDemo2View

IMPLEMENT_DYNCREATE(CDemo2View, CView)

BEGIN_MESSAGE_MAP(CDemo2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDemo2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON32771, &CDemo2View::OnButton32771)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

// CDemo2View 构造/析构

CDemo2View::CDemo2View() noexcept
{
	// TODO: 在此处添加构造代码
	imgData.clear();
	loadImgData("0622.txt");
}

CDemo2View::~CDemo2View()
{
}

void CDemo2View::loadImgData(const string& filename)
{
#if 1
	ifstream fin(filename.c_str());
	if (!fin.is_open())
	{
		cout << "image data not opened: " << filename.c_str();
		return;
	}

	std::string oneline;
	while (!fin.eof())
	{
		std::getline(fin, oneline);
		vector<int> onelineData;

		stringstream ss(oneline);
		int data;
		while (!ss.eof())
		{
			ss >> data;
			onelineData.push_back(data);
		}

		imgData.push_back(onelineData);
	}

	fin.close();
	imgData.pop_back();// the last one is an empty line.
#else
	int tmpImgData[10][20] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,0,
		0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,0,
		0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,
		0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,
		0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,
		0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,
		0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,0,
		0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	};
	for (int r = 0; r < 10; ++ r)
	{
		vector<int> onelineData;

		int data;
		for (int c = 0; c < 20; ++ c)
		{
			data = tmpImgData[r][c];
			onelineData.push_back(data);
		}

		imgData.push_back(onelineData);
	}
#endif

	cout << "Finish loading image. size: " << imgData[0].size() << " "  << imgData.size();
}


void CDemo2View::SortET(EDGE *pEDGE)
{
	//要排序的边链表中第一个边
	EDGE* p1 = pEDGE;
	//第一个边的下一个边
	EDGE* p2 = NULL;
	while (p1 != NULL)
	{
		p2 = p1->next;//获得下一条边
		while (p2 != NULL)
		{
			//如果前一条边的ymin大于后一条边
			//或者ymin相等时xmin大于后一条边
			if (p1->ymin > p2->ymin ||
				(p1->ymin == p2->ymin && p1->xmin > p2->xmin))
			{
				//交换数据
				int cid; double cdd;
				cid = p1->ymax; p1->ymax = p2->ymax; p2->ymax = cid;
				cid = p1->ymin; p1->ymin = p2->ymin; p2->ymin = cid;
				cdd = p1->xmin; p1->xmin = p2->xmin; p2->xmin = cdd;
				cdd = p1->fm; p1->fm = p2->fm; p2->fm = cdd;
			}
			//下一条边
			p2 = p2->next;
		}
		//下一条边
		p1 = p1->next;
	}
}
void CDemo2View::EdgeMarkFIll(std::vector<CPoint>* points, CClientDC* pDC)
{
	// TODO: 在此处添加实现代码.
	COLORREF fillColor = RGB(255, 0, 0);
	//先确定多边形所存在矩阵的边界
	int xmin = points->at(0).x;
	int xmax = points->at(0).x;
	int ymin = points->at(0).y;
	int ymax = points->at(0).y;
	for (int i = 1; i < points->size(); i++)
	{
		if (points->at(i).x > xmax) xmax = points->at(i).x;
		if (points->at(i).x < xmin) xmin = points->at(i).x;
		if (points->at(i).y > ymax) ymax = points->at(i).y;
		if (points->at(i).y < ymin) ymin = points->at(i).y;
	}
	int** MASK = new int*[ymax - ymin + 1];
	for (int i = 0; i < ymax - ymin+1; i++)
	{
		MASK[i] = new int[xmax - xmin + 1];
	}
	for (int i = 0; i < ymax - ymin+1; i++)
	{
		for (int j= 0; j < xmax - xmin+1; j++)
		{
			MASK[i][j] = 0;
		}
	}
	int ixs;
	float xs, dxs, dys;
	//确认图像的大小，进行图案填充
	const int heightImg = imgData.size();
	const int widthImg = imgData[0].size();

	//形成边界
	for (int i = 0; i < points->size(); i++)
	{
		QPointF point1 = (QPointF)points->at(i);
		QPointF point2;
		//如果当前点已经是最后一个点，则和第一点构成一条边
		if (i == points->size() - 1)
			point2 = (QPointF)points->at(0);
		else
		{
			point2 = (QPointF)points->at(i + 1);
		}
		//边平行于x轴，舍弃
		if (point1.y == point2.y)
			continue;
		xs = point1.x; 
		dxs = (point2.x - point1.x) / (float)(point2.y - point1.y);
		dys = abs(point2.y - point1.y) /(float)( point2.y - point1.y);
		for (int ys = point1.y; ys!=point2.y; ys += dys)
		{
			ixs = int(xs + 0.5);
			xs = xs + dys * dxs;
			MASK[ys - ymin][ixs - xmin] = !MASK[ys - ymin][ixs - xmin];
		}
	}
	for (int y = ymin; y < ymax; y++)
	{
		int inside = false;
		int flag = false;
		int begin = xmin;
		int end = xmax;
		for (int x = xmin; x <= xmax; x++)
		{
			if (MASK[y-ymin][x-xmin]==1&&flag==false)
			{
				inside = !inside;
				begin = x;
				flag = true;
				continue;
			}
			if (MASK[y - ymin][x - xmin] == 1 && flag == true)
			{
				inside = !inside;
				end = x;
				flag = false;
				for (int n = begin; n <= end; n++)
				{
					if (imgData[y % heightImg][n % widthImg] == 1)
						pDC->SetPixel(n, y, fillColor);
				}
			}
			
		}
		//for (int x = xmin; x <= xmax; x++)
		//{
		//	if (MASK[y - ymin][x - xmin])
		//	{
		//		inside = !inside;
		//	}
		//	if (inside)
		//	{
		//		if (imgData[y % heightImg][x % widthImg] == 1)
		//			pDC->SetPixel(x, y, fillColor);
		//	}
		//}
	}

	points->clear();

}
EDGE* CDemo2View::GetET(QPolygonF* points)
{
	EDGE* pHead = NULL;
	EDGE* p = NULL;
	//循环点列表构造最初的边链表
	for (int i = 0; i < points->size(); i++)
	{
		//获得当前点和下一点，构成一条边
		QPointF point1 = (QPointF)points->at(i);
		QPointF point2;
		//如果当前点已经是最后一个点，则和第一点构成一条边
		if (i == points->size() - 1)
			point2 = (QPointF)points->at(0);
		else
			point2 = (QPointF)points->at(i + 1);
		//边平行于x轴，舍弃
		if (point1.y == point2.y)
			continue;
		//构造边结构
		EDGE* edge = new EDGE();
		//计算斜率倒数
		edge->fm = (double)(point2.x - point1.x) / (point2.y - point1.y);
		//设置ymin，ymax和xmin
		if (point1.y > point2.y)
		{
			edge->ymax = point1.y; edge->ymin = point2.y;
			edge->xmin = point2.x;
		}
		else
		{
			edge->ymax = point2.y; edge->ymin = point1.y;
			edge->xmin = point1.x;
		}
		edge->next = NULL; edge->last = NULL;
		//构造链表
		if (p == NULL)
		{
			pHead = edge; p = edge;
		}
		else
		{
			p->next = edge;
			edge->last = p;
			p = edge;
		}
	}
	//下面判断局部极大极小并缩短相应的边
	p = pHead;
	EDGE* pNext = NULL;
	while (p != NULL)
	{
		//如果没有下一个条边，则和第一条边比较
		if (p->next == NULL)
			pNext = pHead;
		else
			pNext = p->next;
		//如果连续的两条边的ymin值和ymax值都不同
		//表示连接两边的节点不为局部极大或极小
		if (p->ymin != pNext->ymin && p->ymax != pNext->ymax)
		{
			//缩短ymax值较大的边
			if (p->ymax > pNext->ymax)
			{
				p->ymin++;
				p->xmin += p->fm;
			}
			else
			{
				pNext->ymin++;
				pNext->xmin += pNext->fm;
			}
		}
		p = p->next;
	}
	//将边链表排序
	SortET(pHead);
	//返回排序后的链表
	return pHead;
}

void CDemo2View::SortAET(EDGE *pEDGE)
{
	//要排序的边链表中第一个边
	EDGE* p1 = pEDGE;
	//第一个边的下一个边
	EDGE* p2 = NULL;
	while (p1 != NULL)
	{
		p2 = p1->next;//获得下一条边
		while (p2 != NULL)
		{
			//如果前一条边的xmin大于后一条边
			if (p1->xmin > p2->xmin)
			{
				//交换数据
				int cid; double cdd;
				cid = p1->ymax; p1->ymax = p2->ymax; p2->ymax = cid;
				cid = p1->ymin; p1->ymin = p2->ymin; p2->ymin = cid;
				cdd = p1->xmin; p1->xmin = p2->xmin; p2->xmin = cdd;
				cdd = p1->fm; p1->fm = p2->fm; p2->fm = cdd;
			}
			//下一条边
			p2 = p2->next;
		}
		//下一条边
		p1 = p1->next;
	}
}


void CDemo2View::FillPoly(std::vector<CPoint>* points,CClientDC* pDC)
{

	COLORREF fillColor = RGB(255, 0, 0);
//	pDC->setPen(color);

	//获得ET表
	EDGE* pET = GetET(points);
	//AET表
	EDGE* pAET = NULL;
	//最小y值
	int y = pET->ymin;
	//最大y值
	int ymax = pET->ymax;
	//有需要处理的扫描线
	while (y <= ymax)
	{
		//ET表不为空，并且当前的边的ymin和当前扫描线相等
		while (pET != NULL && pET->ymin == y)
		{
			//将当前边移动到AET表中
			EDGE* p = pET;//获得当前边
			pET = pET->next;//ET指针移动到ET表中的下一条边
			p->next = NULL;//不连接到下一条边，以免重新连会到ET表
			if (pAET == NULL)//当前AET为空，直接加入到AET中
				pAET = p;
			else
			{//当前AET不为空，连接到AET表后
				pAET->next = p;
				//连接当前的边的上一条边为AET表中原有的最后一条边
				p->last = pAET;
				//AET指针移动到表中最后一条边
				pAET = p;
			}
			//根据移动过来的边，修改ymax值
			if (ymax < pAET->ymax)
				ymax = pAET->ymax;
		}
		//找到AET表的第一条边
		while (pAET->last != NULL)
			pAET = pAET->last;
		//排序
		SortAET(pAET);
		EDGE* pFill = pAET;
		const int heightImg = imgData.size();
		const int widthImg = imgData[0].size();
		//填充AET表中的区间
		while (pFill != NULL)
		{
			//填充当前边和下一条边之间的区间
			for (int i = (int)pFill->xmin; i <= (int)pFill->next->xmin; i++)
			{
				if (imgData[y % heightImg][i % widthImg] == 1)
					pDC->SetPixel(i, y,fillColor);
			}
			//AET表中的边应成对出现
			pFill = pFill->next->next;
		}
		//将AET表中ymax为当前扫描线的边清除出表
		pFill = pAET;
		while (pFill != NULL)
		{
			//当前边ymax等于扫描线
			if (pFill->ymax == y)
			{
				//当前边在AET中有上一条边
				if (pFill->last != NULL)
					pFill->last->next = pFill->next;
				//当前边在AET中没有上一条边
				else
					pAET = pFill->next;
				//当前边在AET中有下一条边
				if (pFill->next != NULL)
					pFill->next->last = pFill->last;
				//销毁已经清除出链表的边对象
				EDGE* pdelete = pFill;
				//移动到下一条边
				pFill = pFill->next;
				delete pdelete;
			}
			else
				//移动到下一条边
				pFill = pFill->next;
		}
		//AET表中仍然有边
		if (pAET != NULL)
		{
			//重新计算AET中各边的xmin值
			pFill = pAET;
			while (pFill != NULL)
			{
				pFill->xmin += pFill->fm;
				pFill = pFill->next;
			}
			//重新排序
			SortAET(pAET);
			//找到AET表中的最后一条边，以便将ET中的边移动过来
			while (pAET->next != NULL)
				pAET = pAET->next;
		}
		//处理下一条扫描线
		y++;
	}
}

BOOL CDemo2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDemo2View 绘图

void CDemo2View::OnDraw(CDC* pDC)
{
	CDemo2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	

	// TODO: 在此处为本机数据添加绘制代码
}


// CDemo2View 打印


void CDemo2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDemo2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDemo2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDemo2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDemo2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDemo2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDemo2View 诊断

#ifdef _DEBUG
void CDemo2View::AssertValid() const
{
	CView::AssertValid();
}

void CDemo2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemo2Doc* CDemo2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemo2Doc)));
	return (CDemo2Doc*)m_pDocument;
}
#endif //_DEBUG


// CDemo2View 消息处理程序


void CDemo2View::OnButton32771()
{
	
}


void CDemo2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	poly.push_back(point);

	if (poly.size() == 1)
	{
		CView::OnLButtonDown(nFlags, point);
		return;
	}
	
	
	CClientDC dc(this);

	dc.MoveTo(poly[poly.size() - 2]);
	dc.LineTo(point);
	



	CView::OnLButtonDown(nFlags, point);
}


void CDemo2View::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	CClientDC dc(this);


	
	dc.MoveTo(point);
	dc.LineTo(poly[0]);
//	FillPoly(&this->poly, &dc);
	EdgeMarkFIll(&this->poly, &dc);
	CView::OnLButtonDblClk(nFlags, point);
}


