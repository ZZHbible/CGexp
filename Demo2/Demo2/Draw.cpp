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

//DDAֱ��ɨ��ת���㷨
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

//�е㻭�߷�
void CDraw::MidpointLine(QPainter *pDC, int x0, int y0, int x1, int y1, QColor color)
{
	pDC->setPen(color);

	int a,b,delta1,delta2,d,x,y;
	//����˵�����xֵ���
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
	//б���жϣ�б�ʾ���ֵ����1����mΪfalse������Ϊtrue
	BOOL m = (fabs(y1-y0)<=fabs(x1-x0));
	//��������x0����x1��������ֵ����
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
	//б�ʾ���ֵС�ڵ���1
	if (m)
	{
		//��һ�������yֵ����
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
		//�����������yֵ�ݼ�
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
	//б�ʾ���ֵ����1
	else
	{
		//�ڶ��������yֵ����
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
		//�����������yֵ�ݼ�
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

//Bresenham�����㷨
void CDraw::BresenhamLine(QPainter *pDC, int x1, int y1, int x2, int y2, QColor color)
{
	pDC->setPen(color);

	int x,y,dx,dy,p;
	//����˵�����xֵ���
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
	//б���жϣ�б�ʾ���ֵ����1����mΪfalse������Ϊtrue
	BOOL m = (fabs(y2-y1)<=fabs(x2-x1));
	//���x1����x2����������ֵ
	if (x1 > x2)
	{
		p=x1;x1=x2;x2=p;
		p=y1;y1=y2;y2=p;
	}
	x = x1;
	y = y1;
	dx = x2 - x1;
	dy = y2 - y1;
	//б�ʾ���ֵС�ڵ���1
	if (m)
	{
		//��һ�������y����
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
		//�����������y�ݼ�
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
	//б�ʾ���ֵ����1
	else
	{
		//�ڶ��������y����
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
		//�����������y�ݼ�
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
	//��õ�ǰ�豸��������ı���ɫ

	QColor bk = QColor("white");// pDC->GetBkColor();
	for (int i=1;i<=e;i++)
	{
		//���Ƶ����ų�10������С��5
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
	//��õ�ǰ�豸��������ı���ɫ
	QColor bk("white");// pDC->GetBkColor();
	for (int i=1;i<=e;i++)
	{
		//�����߿���Ҫ���Ƶ�������Χ���ƶ��������
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
		//����Բ���϶ԳƵİ˸����ص�
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
		//����Բ���϶ԳƵİ˸����ص�
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
	//������Ƶ�1����
	x=0;y=b;
	p=2*bb+aa*(1-2*b);
	while (bb*x<=aa*y)
	{
		//���ƶԳƵ��ĸ���
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
	//������Ƶ�2����
	x=a;y=0;
	p=2*aa+bb*(1-2*a);
	while (bb*x>aa*y)
	{
		//���ƶԳƵ��ĸ���
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
	QStack<QPoint> s;//��ջ
	QPoint p;
	s.push(QPoint(x,y));//��һ��������ջ
	while (!s.empty())//��ջ��Ϊ��
	{
		p = s.top();s.pop();//ȡջ��Ԫ�ز���ջ
		pDC->drawPoint(p.x(),p.y());//�������ص�Ϊָ����ɫ
		x = p.x();y = p.y();
		x0 =x + 1;

		QPixmap pixmap(1, 1);
		QPoint targetPos(0, 0);
		QRegion sourceArea(x0, y, 1, 1);
		widget->render(&pixmap, targetPos, sourceArea);


		while (pixmap.toImage().pixelColor(0, 0) != boundaryvalue)//����ҷ�Ԫ��
		{
			pDC->drawPoint(x0,y);
			x0++;
		}
		xr = x0 -1;//���ұ�����
		x0 = x -1;
		sourceArea = QRegion(x0, y, 1, 1);
		widget->render(&pixmap, targetPos, sourceArea);
		while (pixmap.toImage().pixelColor(0, 0) != boundaryvalue)//���������
		{
			pDC->drawPoint(x0,y);
			x0--;
		}
		xl = x0 + 1;//���������
		//�����һ������һ��ɨ���ߣ������ڱ߽���δ��������
		//��ѡȡ������������������������ջ
		y0 = y;
		for (int i=1;i>=-1;i-=2)
		{
			x0 = xr;
			y = y0 + i;//�����һ�к���һ��
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
					s.push(QPoint(xid,y));//��������ջ
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
	//Ҫ����ı������е�һ����
	EDGE* p1 = pEDGE;
	//��һ���ߵ���һ����
	EDGE* p2 = NULL;
	while (p1 != NULL)
	{
		p2 = p1->next;//�����һ����
		while (p2 != NULL)
		{
			//���ǰһ���ߵ�ymin���ں�һ����
			//����ymin���ʱxmin���ں�һ����
			if (p1->ymin > p2->ymin || 
				(p1->ymin == p2->ymin && p1->xmin > p2->xmin))
			{
				//��������
				int cid;double cdd;
				cid = p1->ymax;p1->ymax = p2->ymax;p2->ymax = cid;
				cid = p1->ymin;p1->ymin = p2->ymin;p2->ymin = cid;
				cdd = p1->xmin;p1->xmin = p2->xmin;p2->xmin = cdd;
				cdd = p1->fm;p1->fm = p2->fm;p2->fm = cdd;
			}
			//��һ����
			p2 = p2->next;
		}
		//��һ����
		p1 = p1->next;
	}
}

EDGE* CDraw::GetET(QPolygonF* points)
{
	EDGE* pHead= NULL;
	EDGE* p = NULL;
	//ѭ�����б�������ı�����
	for (int i=0;i<points->size();i++)
	{
		//��õ�ǰ�����һ�㣬����һ����
		QPointF point1 = (QPointF)points->at(i);
		QPointF point2;
		//�����ǰ���Ѿ������һ���㣬��͵�һ�㹹��һ����
		if (i == points->size() -1)
			point2 = (QPointF)points->at(0);
		else
			point2 = (QPointF)points->at(i+1);
		//��ƽ����x�ᣬ����
		if (point1.y() == point2.y())
			continue;
		//����߽ṹ
		EDGE* edge = new EDGE();
		//����б�ʵ���
		edge->fm = (double)(point2.x() - point1.x())/(point2.y() - point1.y());
		//����ymin��ymax��xmin
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
		//��������
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
	//�����жϾֲ�����С��������Ӧ�ı�
	p = pHead;
	EDGE* pNext = NULL;
	while (p!=NULL)
	{
		//���û����һ�����ߣ���͵�һ���߱Ƚ�
		if (p->next == NULL)
			pNext = pHead;
		else
			pNext = p->next;
		//��������������ߵ�yminֵ��ymaxֵ����ͬ
		//��ʾ�������ߵĽڵ㲻Ϊ�ֲ������С
		if (p->ymin != pNext->ymin && p->ymax != pNext->ymax)
		{
			//����ymaxֵ�ϴ�ı�
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
	//������������
	SortET(pHead);
	//��������������
	return pHead;
}

void CDraw::Polygonfill(QPainter *pDC, QPolygonF* points, QColor color)
{
	pDC->setPen(color);

	//���ET��
	EDGE* pET = GetET(points);
	//AET��
	EDGE* pAET = NULL;
	//��Сyֵ
	int y = pET->ymin;
	//���yֵ
	int ymax = pET->ymax;
	//����Ҫ�����ɨ����
	while (y <= ymax)
	{
		//ET��Ϊ�գ����ҵ�ǰ�ıߵ�ymin�͵�ǰɨ�������
		while (pET != NULL && pET->ymin == y)
		{
			//����ǰ���ƶ���AET����
			EDGE* p = pET;//��õ�ǰ��
			pET = pET->next;//ETָ���ƶ���ET���е���һ����
			p->next = NULL;//�����ӵ���һ���ߣ������������ᵽET��
			if (pAET == NULL)//��ǰAETΪ�գ�ֱ�Ӽ��뵽AET��
				pAET = p;
			else
			{//��ǰAET��Ϊ�գ����ӵ�AET���
				pAET->next = p;
				//���ӵ�ǰ�ıߵ���һ����ΪAET����ԭ�е����һ����
				p->last = pAET;
				//AETָ���ƶ����������һ����
				pAET = p;
			}
			//�����ƶ������ıߣ��޸�ymaxֵ
			if (ymax < pAET->ymax)
				ymax = pAET->ymax;
		}
		//�ҵ�AET��ĵ�һ����
		while (pAET->last != NULL)
			pAET = pAET->last;
		//����
		SortAET(pAET);
		EDGE* pFill = pAET;
		const int heightImg = imgData.size();
		const int widthImg = imgData[0].size();
		//���AET���е�����
		while (pFill != NULL)
		{
			//��䵱ǰ�ߺ���һ����֮�������
			for (int i = (int)pFill->xmin; i <= (int)pFill->next->xmin; i++)
			{
				if (imgData[y % heightImg][i % widthImg] == 1)
					pDC->drawPoint(i, y);
			}
			//AET���еı�Ӧ�ɶԳ���
			pFill = pFill->next->next;
		}
		//��AET����ymaxΪ��ǰɨ���ߵı��������
		pFill = pAET;
		while (pFill != NULL)
		{
			//��ǰ��ymax����ɨ����
			if (pFill->ymax == y)
			{
				//��ǰ����AET������һ����
				if (pFill->last != NULL)
					pFill->last->next = pFill->next;
				//��ǰ����AET��û����һ����
				else
					pAET = pFill->next;
				//��ǰ����AET������һ����
				if (pFill->next != NULL)
					pFill->next->last = pFill->last;
				//�����Ѿ����������ı߶���
				EDGE* pdelete = pFill;
				//�ƶ�����һ����
				pFill = pFill->next;
				delete pdelete;
			}
			else
				//�ƶ�����һ����
				pFill = pFill->next;
		}
		//AET������Ȼ�б�
		if (pAET != NULL)
		{
			//���¼���AET�и��ߵ�xminֵ
			pFill = pAET;
			while (pFill != NULL)
			{
				pFill->xmin+=pFill->fm;
				pFill = pFill->next;
			}
			//��������
			SortAET(pAET);
			//�ҵ�AET���е����һ���ߣ��Ա㽫ET�еı��ƶ�����
			while(pAET->next != NULL)
				pAET = pAET->next;
		}
		//������һ��ɨ����
		y++;
	}
}

void CDraw::SortAET(EDGE *pEDGE)
{
	//Ҫ����ı������е�һ����
	EDGE* p1 = pEDGE;
	//��һ���ߵ���һ����
	EDGE* p2 = NULL;
	while (p1 != NULL)
	{
		p2 = p1->next;//�����һ����
		while (p2 != NULL)
		{
			//���ǰһ���ߵ�xmin���ں�һ����
			if (p1->xmin > p2->xmin)
			{
				//��������
				int cid;double cdd;
				cid = p1->ymax;p1->ymax = p2->ymax;p2->ymax = cid;
				cid = p1->ymin;p1->ymin = p2->ymin;p2->ymin = cid;
				cdd = p1->xmin;p1->xmin = p2->xmin;p2->xmin = cdd;
				cdd = p1->fm;p1->fm = p2->fm;p2->fm = cdd;
			}
			//��һ����
			p2 = p2->next;
		}
		//��һ����
		p1 = p1->next;
	}
}

void CDraw::PolygonfillShadow(QPainter *pDC, QPolygonF* points, QColor color)
{
	pDC->setPen(color);

	//���ET��
	EDGE* pET = GetET(points);
	//AET��
	EDGE* pAET = NULL;
	//��Сyֵ
	int y = pET->ymin;
	//���yֵ
	int ymax = pET->ymax;
	//����Ҫ�����ɨ����
	while (y <= ymax)
	{
		//ET��Ϊ�գ����ҵ�ǰ�ıߵ�ymin�͵�ǰɨ�������
		while (pET != NULL && pET->ymin == y)
		{
			//����ǰ���ƶ���AET����
			EDGE* p = pET;//��õ�ǰ��
			pET = pET->next;//ETָ���ƶ���ET���е���һ����
			p->next = NULL;//�����ӵ���һ���ߣ������������ᵽET��
			if (pAET == NULL)//��ǰAETΪ�գ�ֱ�Ӽ��뵽AET��
				pAET = p;
			else
			{//��ǰAET��Ϊ�գ����ӵ�AET���
				pAET->next = p;
				//���ӵ�ǰ�ıߵ���һ����ΪAET����ԭ�е����һ����
				p->last = pAET;
				//AETָ���ƶ����������һ����
				pAET = p;
			}
			//�����ƶ������ıߣ��޸�ymaxֵ
			if (ymax < pAET->ymax)
				ymax = pAET->ymax;
		}
		//�ҵ�AET��ĵ�һ����
		while (pAET->last != NULL)
			pAET = pAET->last;
		//����
		SortAET(pAET);
		EDGE* pFill = pAET;
		//���AET���е�����
		while (pFill != NULL)
		{
			//��䵱ǰ�ߺ���һ����֮�������
			for (int i=(int)pFill->xmin;i<=(int)pFill->next->xmin;i++)
				if (i %10 == y%10)
					pDC->drawPoint(i,y);
			//AET���еı�Ӧ�ɶԳ���
			pFill = pFill->next->next;
		}
		//��AET����ymaxΪ��ǰɨ���ߵı��������
		pFill = pAET;
		while (pFill != NULL)
		{
			//��ǰ��ymax����ɨ����
			if (pFill->ymax == y)
			{
				//��ǰ����AET������һ����
				if (pFill->last != NULL)
					pFill->last->next = pFill->next;
				//��ǰ����AET��û����һ����
				else
					pAET = pFill->next;
				//��ǰ����AET������һ����
				if (pFill->next != NULL)
					pFill->next->last = pFill->last;
				//�����Ѿ����������ı߶���
				EDGE* pdelete = pFill;
				//�ƶ�����һ����
				pFill = pFill->next;
				delete pdelete;
			}
			else
				//�ƶ�����һ����
				pFill = pFill->next;
		}
		//AET������Ȼ�б�
		if (pAET != NULL)
		{
			//���¼���AET�и��ߵ�xminֵ
			pFill = pAET;
			while (pFill != NULL)
			{
				pFill->xmin+=pFill->fm;
				pFill = pFill->next;
			}
			//��������
			SortAET(pAET);
			//�ҵ�AET���е����һ���ߣ��Ա㽫ET�еı��ƶ�����
			while(pAET->next != NULL)
				pAET = pAET->next;
		}
		//������һ��ɨ����
		y++;
	}
}
