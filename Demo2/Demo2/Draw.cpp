// Draw.cpp: implementation of the Draw class.
//
//////////////////////////////////////////////////////////////////////


#include "Draw.h"

#include <fstream>
#include <string>
#include <sstream>

#include <math.h>

using namespace std;


CDraw::CDraw()
{
	imgData.clear();
	loadImgData("1234.txt");
}

CDraw::~CDraw()
{

}

void CDraw::loadImgData(const QString& filename)
{
	ifstream fin(filename.toLocal8Bit().data());
	if (!fin.is_open())
	{
		qDebug() << "image data not opened: " << filename;
		return;
	}

	std::string oneline;
	while (!fin.eof())
	{
		std::getline( fin,oneline);
		QVector<int> onelineData;

		stringstream ss(oneline);
		int data;
		while (!ss.eof())
		{
			ss >> data;
			onelineData.append(data);
		}

		imgData.append(onelineData);
	}

	fin.close();
	imgData.pop_back();// the last one is an empty line.

	qDebug() << "Finish loading image. size: " << imgData[0].size() << imgData.size();
}

//DDA直线扫描转换算法
void CDraw::DDALine(QPainter* pDC, int x1, int y1, int x2, int y2, QColor color)
{
	pDC->setPen(color);

	double dx,dy,e,x,y;
	dx = x2 -x1;
	dy = y2- y1;
	e = (fabs(dx) > fabs(dy)) ? fabs(dx):fabs(dy);
	dx/=e;
	dy/=e;
	x = x1;
	y = y1;
	for (int i=1;i<=e;i++)
	{
		pDC->drawPoint((int)(x+0.5),(int)(y+0.5));
		x+=dx;
		y+=dy;
	}
}

//中点画线法
void CDraw::MidpointLine(QPainter *pDC, int x0, int y0, int x1, int y1, QColor color)
{
	pDC->setPen(color);

	int a,b,delta1,delta2,d,x,y;
	//传入端点坐标x值相等
	if (x0 == x1)
	{
		if (y0 < y1)
		{
			for (int i=y0;i<=y1;i++)
				pDC->drawPoint(x0,i);
		}
		else
		{
			for (int i=y1;i<=y0;i++)
				pDC->drawPoint(x0,i);
		}
		return;
	}
	//斜率判断，斜率绝对值大于1，则m为false，否则为true
	BOOL m = (fabs(y1-y0)<=fabs(x1-x0));
	//如果传入的x0大于x1，将坐标值互换
	if (x0>x1)
	{
		d=x0;x0=x1;x1=d;
		d=y0;y0=y1;y1=d;
	}
	a = y0 - y1;
	b = x1 - x0;
	x = x0;
	y = y0;
	pDC->drawPoint(x,y);
	//斜率绝对值小于等于1
	if (m)
	{
		//第一种情况，y值递增
		if (y0 <= y1)
		{
			d = 2 * a +b;
			delta1 = 2 * a;
			delta2 = 2 * (a + b);
			while (x < x1)
			{
				if (d < 0)
				{x++;y++;d+=delta2;}
				else
				{x++;d+=delta1;}
				pDC->drawPoint(x,y);
			}
		}
		//第三种情况，y值递减
		else
		{
			d = 2 * a -b;
			delta1 = 2 * a;
			delta2 = 2 * (a - b);
			while (x < x1)
			{
				if (d < 0)
				{x++;d+=delta1;}
				else
				{x++;y--;d+=delta2;}
				pDC->drawPoint(x,y);
			}
		}
	}
	//斜率绝对值大于1
	else
	{
		//第二种情况，y值递增
		if (y0 <= y1)
		{
			d =  a + 2 * b;
			delta1 = 2 * b;
			delta2 = 2 * (a + b);
			while (y < y1)
			{
				if (d < 0)
				{y++;d+=delta1;}
				else
				{y++;x++;d+=delta2;}
				pDC->drawPoint(x,y);
			}
		}
		//第四种情况，y值递减
		else
		{
			d =  a - 2 * b;
			delta1 = -2 * b;
			delta2 = 2 * (a - b);
			while (y > y1)
			{
				if (d < 0)
				{y--;x++;d+=delta2;}
				else
				{y--;d+=delta1;}
				pDC->drawPoint(x,y);
			}
		}
	}
}

//Bresenham画线算法
void CDraw::BresenhamLine(QPainter *pDC, int x1, int y1, int x2, int y2, QColor color)
{
	pDC->setPen(color);

	int x,y,dx,dy,p;
	//传入端点坐标x值相等
	if (x1 == x2)
	{
		if (y1 < y2)
		{
			for (int i=y1;i<=y2;i++)
				pDC->drawPoint(x1,i);
		}
		else
		{
			for (int i=y2;i<=y1;i++)
				pDC->drawPoint(x1,i);
		}
		return;
	}
	//斜率判断，斜率绝对值大于1，则m为false，否则为true
	BOOL m = (fabs(y2-y1)<=fabs(x2-x1));
	//如果x1大于x2，交换坐标值
	if (x1 > x2)
	{
		p=x1;x1=x2;x2=p;
		p=y1;y1=y2;y2=p;
	}
	x = x1;
	y = y1;
	dx = x2 - x1;
	dy = y2 - y1;
	//斜率绝对值小于等于1
	if (m)
	{
		//第一种情况，y递增
		if (y1 <= y2)
		{
			p = (dy<<1) - dx;
			while (x <= x2)
			{
				pDC->drawPoint(x,y);
				if (p < 0)
				{x++;p=p+(dy<<1);}
				else
				{x++;y++;p=p+((dy-dx)<<1);}
			}
		}
		//第三种情况，y递减
		else
		{
			p = dx - (dy<<1);
			while (x <= x2)
			{
				pDC->drawPoint(x,y);
				if (p < 0)
				{x++;p=p-(dy<<1);}
				else
				{x++;y--;p=p-((dy+dx)<<1);}
			}
		}
	}
	//斜率绝对值大于1
	else
	{
		//第二种情况，y递增
		if (y1 <= y2)
		{
			p = (dx<<1) - dy;
			while (y <= y2)
			{
				pDC->drawPoint(x,y);
				if (p < 0)
				{y++;p=p+(dx<<1);}
				else
				{x++;y++;p=p+((dx-dy)<<1);}
			}
		}
		//第四种情况，y递减
		else
		{
			p = (dx<<1) + dy;
			while (y >= y2)
			{
				pDC->drawPoint(x,y);
				if (p < 0)
				{y--;p=p+(dx<<1);}
				else
				{x++;y--;p=p+((dx+dy)<<1);}
			}
		}
	}
}

void CDraw::DDALineStyle(QPainter *pDC, int x1, int y1, int x2, int y2, QColor color)
{
	

	double dx,dy,e,x,y;
	dx = x2 -x1;
	dy = y2- y1;
	e = (fabs(dx) > fabs(dy)) ? fabs(dx):fabs(dy);
	dx/=e;
	dy/=e;
	x = x1;
	y = y1;
	//获得当前设备环境对象的背景色

	QColor bk = QColor("white");// pDC->GetBkColor();
	for (int i=1;i<=e;i++)
	{
		//绘制点的序号除10的余数小于5
		if ((i - 1) % 10 < 5)
		{
			pDC->setPen(color);
			pDC->drawPoint((int)(x + 0.5), (int)(y + 0.5));
		}
		else
		{
			pDC->setPen(bk);
			pDC->drawPoint((int)(x + 0.5), (int)(y + 0.5));
		}
		x+=dx;
		y+=dy;
	}
}

void CDraw::DDALineWidth(QPainter *pDC, int x1, int y1, int x2, int y2, QColor color, int width)
{
	pDC->setPen(color);

	double dx,dy,e,x,y;
	dx = x2 -x1;
	dy = y2- y1;
	e = (fabs(dx) > fabs(dy)) ? fabs(dx):fabs(dy);
	dx/=e;
	dy/=e;
	x = x1;
	y = y1;
	//获得当前设备环境对象的背景色
	QColor bk("white");// pDC->GetBkColor();
	for (int i=1;i<=e;i++)
	{
		//根据线宽，在要绘制的像素周围绘制额外的像素
		for (int j=(1-width)/2;j<=width/2;j++)
			for (int k=(1-width)/2;k<=width/2;k++)
				pDC->drawPoint((int)(x+0.5) + j,(int)(y+0.5) + k);
		x+=dx;
		y+=dy;
	}
}

void CDraw::MidpointCircle(QPainter *pDC, int x0, int y0, int R, QColor color)
{
	pDC->setPen(color);

	int x,y,dx,dy,d;
	x = 0; y = R; d = 1 - R;
	dx = 3;dy = 5 - (R<<1);
	while (x <= y)
	{
		//绘制圆弧上对称的八个像素点
		pDC->drawPoint(x + x0, y + y0);
		pDC->drawPoint(-x + x0, y + y0);
		pDC->drawPoint(x + x0, -y + y0);
		pDC->drawPoint(-x + x0, -y + y0);
		pDC->drawPoint(y + x0, x + y0);
		pDC->drawPoint(-y + x0, x + y0);
		pDC->drawPoint(y + x0, -x + y0);
		pDC->drawPoint(-y + x0, -x + y0);
		if (d < 0)
		{
			d+=dx;
			dx+=2;
			x++;
		}
		else
		{
			d+=(dx+dy);
			dx+=2;
			dy+=2;
			x++;
			y--;
		}
	}
}

void CDraw::BresenhamCircle(QPainter *pDC, int x0, int y0, int R, QColor color)
{
	pDC->setPen(color);

	int x,y,p;
	x = 0;y = R;p = 3 - (R<<1);
	for (;x<=y;x++)
	{
		//绘制圆弧上对称的八个像素点
		pDC->drawPoint(x + x0, y + y0);
		pDC->drawPoint(-x + x0, y + y0);
		pDC->drawPoint(x + x0, -y + y0);
		pDC->drawPoint(-x + x0, -y + y0);
		pDC->drawPoint(y + x0, x + y0);
		pDC->drawPoint(-y + x0, x + y0);
		pDC->drawPoint(y + x0, -x + y0);
		pDC->drawPoint(-y + x0, -x + y0);
		if (p < 0)
			p+=((x<<2) + 6);
		else
		{
			p+=(((x-y)<<2) + 10);
			y--;
		}
	}
}

void CDraw::BresenhamEllipse(QPainter *pDC, int x0, int y0, int a, int b, QColor color)
{
	pDC->setPen(color);

	int x,y,p;
	int aa=a*a,bb=b*b;
	//下面绘制第1部分
	x=0;y=b;
	p=2*bb+aa*(1-2*b);
	while (bb*x<=aa*y)
	{
		//绘制对称的四个点
		pDC->drawPoint(x+x0,y+y0);
		pDC->drawPoint(-x+x0,y+y0);
		pDC->drawPoint(x+x0,-y+y0);
		pDC->drawPoint(-x+x0,-y+y0);
		if(p>=0)
		{
			p+=4*aa*(1-y)+bb*(4*x+6);
			y--;
		}
		else
		{
			p+= bb*(4*x+6);
		}
		x++;
	}
	//下面绘制第2部分
	x=a;y=0;
	p=2*aa+bb*(1-2*a);
	while (bb*x>aa*y)
	{
		//绘制对称的四个点
		pDC->drawPoint(x+x0,y+y0);
		pDC->drawPoint(-x+x0,y+y0);
		pDC->drawPoint(x+x0,-y+y0);
		pDC->drawPoint(-x+x0,-y+y0);
		if(p>=0)
		{
			p+=4*bb*(1-x)+aa*(4*y+6);
			x--;
		}
		else
		{
			p+= aa*(4*y+6);
		}
		y++;
	}
}

void CDraw::FloodFill(QPainter *pDC, int x, int y, QColor oldvalue, QColor newvalue)
{
	QWidget* widget = dynamic_cast<QWidget*>(pDC->device());
	if (nullptr == widget)
		return;

	// Just 1 pixel.
	QPixmap pixmap(1, 1);
	// Target coordinates inside the pixmap where drawing should start.
	QPoint targetPos(0, 0);
	// Source area inside the widget that should be rendered.
	QRegion sourceArea(x, y, 1, 1/* use appropriate coordinates from the mouse event */);
	// Render it.
	widget->render(&pixmap, targetPos, sourceArea /* look into what flags you need */);
	// Do whatever else you need to extract the color from the 1 pixel pixmap.

	if (pixmap.toImage().pixelColor(0,0) == oldvalue)
	{
		pDC->setPen(newvalue);
		pDC->drawPoint(x,y);
		FloodFill(pDC,x,y-1,oldvalue,newvalue);
		FloodFill(pDC,x,y+1,oldvalue,newvalue);
		FloodFill(pDC,x-1,y,oldvalue,newvalue);
		FloodFill(pDC,x+1,y,oldvalue,newvalue);
	}
}

void CDraw::ScanlineSeedfill(QPainter *pDC, int x, int y, QColor boundaryvalue, QColor newvalue)
{
	QWidget* widget = dynamic_cast<QWidget*>(pDC->device());
	if (nullptr == widget)
		return;

	pDC->setPen(newvalue);

	int x0,xl,xr,y0,xid;
	int flag,xnextspan;
	QStack<QPoint> s;//堆栈
	QPoint p;
	s.push(QPoint(x,y));//第一个种子入栈
	while (!s.empty())//堆栈不为空
	{
		p = s.top();s.pop();//取栈顶元素并弹栈
		pDC->drawPoint(p.x(),p.y());//绘制像素点为指定颜色
		x = p.x();y = p.y();
		x0 =x + 1;

		QPixmap pixmap(1, 1);
		QPoint targetPos(0, 0);
		QRegion sourceArea(x0, y, 1, 1);
		widget->render(&pixmap, targetPos, sourceArea);


		while (pixmap.toImage().pixelColor(0, 0) != boundaryvalue)//填充右方元素
		{
			pDC->drawPoint(x0,y);
			x0++;
		}
		xr = x0 -1;//最右边像素
		x0 = x -1;
		sourceArea = QRegion(x0, y, 1, 1);
		widget->render(&pixmap, targetPos, sourceArea);
		while (pixmap.toImage().pixelColor(0, 0) != boundaryvalue)//填充左方像素
		{
			pDC->drawPoint(x0,y);
			x0--;
		}
		xl = x0 + 1;//最左边像素
		//检查上一条和下一条扫描线，若存在边界且未填充的像素
		//则选取代表各连续区间的种子像素入栈
		y0 = y;
		for (int i=1;i>=-1;i-=2)
		{
			x0 = xr;
			y = y0 + i;//获得上一行和下一行
			while (x0 >= xl)
			{
				flag = 0;
				sourceArea = QRegion(x0, y, 1, 1);
				widget->render(&pixmap, targetPos, sourceArea);
				while ((pixmap.toImage().pixelColor(0, 0) != boundaryvalue)
					&& (pixmap.toImage().pixelColor(0, 0) != newvalue)
					&& (x0 > xl))
				{
					if (flag == 0)
					{
						flag = 1;
						xid = x0;
					}
					x0--;
				}
				if (flag == 1)
				{
					s.push(QPoint(xid,y));//新种子入栈
					flag = 0;
				}
				xnextspan = x0;
				sourceArea = QRegion(x0, y, 1, 1);
				widget->render(&pixmap, targetPos, sourceArea);
				while ((pixmap.toImage().pixelColor(0, 0) == boundaryvalue)
					|| (pixmap.toImage().pixelColor(0, 0) == newvalue)
					&& (x0 >= xl))
					x0--;
				if (xnextspan == x0) x0--;
			}
		}
	}
}

void CDraw::SortET(EDGE *pEDGE)
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
				int cid;double cdd;
				cid = p1->ymax;p1->ymax = p2->ymax;p2->ymax = cid;
				cid = p1->ymin;p1->ymin = p2->ymin;p2->ymin = cid;
				cdd = p1->xmin;p1->xmin = p2->xmin;p2->xmin = cdd;
				cdd = p1->fm;p1->fm = p2->fm;p2->fm = cdd;
			}
			//下一条边
			p2 = p2->next;
		}
		//下一条边
		p1 = p1->next;
	}
}

EDGE* CDraw::GetET(QPolygonF* points)
{
	EDGE* pHead= NULL;
	EDGE* p = NULL;
	//循环点列表构造最初的边链表
	for (int i=0;i<points->size();i++)
	{
		//获得当前点和下一点，构成一条边
		QPointF point1 = (QPointF)points->at(i);
		QPointF point2;
		//如果当前点已经是最后一个点，则和第一点构成一条边
		if (i == points->size() -1)
			point2 = (QPointF)points->at(0);
		else
			point2 = (QPointF)points->at(i+1);
		//边平行于x轴，舍弃
		if (point1.y() == point2.y())
			continue;
		//构造边结构
		EDGE* edge = new EDGE();
		//计算斜率倒数
		edge->fm = (double)(point2.x() - point1.x())/(point2.y() - point1.y());
		//设置ymin，ymax和xmin
		if (point1.y() > point2.y())
		{
			edge->ymax = point1.y();edge->ymin = point2.y();
			edge->xmin = point2.x();
		}
		else
		{
			edge->ymax = point2.y();edge->ymin = point1.y();
			edge->xmin = point1.x();
		}
		edge->next = NULL;edge->last = NULL;
		//构造链表
		if (p == NULL)
		{
			pHead = edge;p = edge;
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
	while (p!=NULL)
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
				p->xmin+=p->fm;
			}
			else
			{
				pNext->ymin++;
				pNext->xmin+=pNext->fm;
			}
		}
		p = p->next;
	}
	//将边链表排序
	SortET(pHead);
	//返回排序后的链表
	return pHead;
}

void CDraw::Polygonfill(QPainter *pDC, QPolygonF* points, QColor color)
{
	pDC->setPen(color);

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
					pDC->drawPoint(i, y);
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
				pFill->xmin+=pFill->fm;
				pFill = pFill->next;
			}
			//重新排序
			SortAET(pAET);
			//找到AET表中的最后一条边，以便将ET中的边移动过来
			while(pAET->next != NULL)
				pAET = pAET->next;
		}
		//处理下一条扫描线
		y++;
	}
}

void CDraw::SortAET(EDGE *pEDGE)
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
				int cid;double cdd;
				cid = p1->ymax;p1->ymax = p2->ymax;p2->ymax = cid;
				cid = p1->ymin;p1->ymin = p2->ymin;p2->ymin = cid;
				cdd = p1->xmin;p1->xmin = p2->xmin;p2->xmin = cdd;
				cdd = p1->fm;p1->fm = p2->fm;p2->fm = cdd;
			}
			//下一条边
			p2 = p2->next;
		}
		//下一条边
		p1 = p1->next;
	}
}

void CDraw::PolygonfillShadow(QPainter *pDC, QPolygonF* points, QColor color)
{
	pDC->setPen(color);

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
		//填充AET表中的区间
		while (pFill != NULL)
		{
			//填充当前边和下一条边之间的区间
			for (int i=(int)pFill->xmin;i<=(int)pFill->next->xmin;i++)
				if (i %10 == y%10)
					pDC->drawPoint(i,y);
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
				pFill->xmin+=pFill->fm;
				pFill = pFill->next;
			}
			//重新排序
			SortAET(pAET);
			//找到AET表中的最后一条边，以便将ET中的边移动过来
			while(pAET->next != NULL)
				pAET = pAET->next;
		}
		//处理下一条扫描线
		y++;
	}
}
