

#if !defined(AFX_DRAW_H__FC3BFB54_8D76_41E9_8464_A2028E89CB6B__INCLUDED_)
#define AFX_DRAW_H__FC3BFB54_8D76_41E9_8464_A2028E89CB6B__INCLUDED_

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

struct EDGE{//边结构，保存边信息同时作为ET表中和AET表中的吊桶
	int ymin;//最小y值，等同于ET表中登记项的y值
	int ymax;//最大y值，吊桶数据
	double xmin;//小y值端点的x值，吊桶数据
	double fm;//斜率倒数，吊桶数据
	EDGE* next;//连接下一个边
	EDGE* last;//连接到上一个边，为了排序时使用
};
class CDraw  
{
public:
	// load the data file containing image: like 1234
	void loadImgData(const QString& filename);
	//多边形扫描转换算法
	void Polygonfill(QPainter *pDC, QPolygonF* points, QColor color);
	//获得多边形的最初的ET表
	EDGE* GetET(QPolygonF* points);
	//将传入的ET表排序
	void SortET(EDGE* pEDGE);
	//将传入的AET表排序
	void SortAET(EDGE* pEDGE);
	//扫描线种子填充算法
	void ScanlineSeedfill(QPainter *pDC, int x, int y, QColor boundaryvalue, QColor newvalue);
	void FloodFill(QPainter* pDC, int x, int y, QColor oldvalue, QColor newvalue);
	//Bresenham画椭圆算法
	void BresenhamEllipse(QPainter *pDC, int x0, int y0, int a, int b, QColor color);
	//Bresenham画圆算法
	void BresenhamCircle(QPainter *pDC, int x0, int y0, int R, QColor color);
	//中点画圆法
	void MidpointCircle(QPainter* pDC, int x0, int y0, int R, QColor color);
	//用DDA直线扫描转换算法绘制指定线宽的直线段
	void DDALineWidth(QPainter *pDC, int x1, int y1, int x2, int y2, QColor color, int width);
	//用DDA直线扫描转换算法绘制虚线
	void DDALineStyle(QPainter* pDC, int x1, int y1, int x2, int y2, QColor color);
	//Bresenham画线算法
	void BresenhamLine(QPainter* pDC, int x1, int y1, int x2, int y2, QColor color);
	//中点画线法
	void MidpointLine(QPainter* pDC, int x0, int y0, int x1, int y1, QColor color);
	//DDA直线扫描转换算法
	void DDALine(QPainter* pDC, int x1, int y1, int x2,int y2,QColor color);
	CDraw();
	virtual ~CDraw();

private:
	QVector<QVector<int> > imgData;

};

#endif // !defined(AFX_DRAW_H__FC3BFB54_8D76_41E9_8464_A2028E89CB6B__INCLUDED_)
