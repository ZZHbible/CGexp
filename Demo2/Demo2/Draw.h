

#if !defined(AFX_DRAW_H__FC3BFB54_8D76_41E9_8464_A2028E89CB6B__INCLUDED_)
#define AFX_DRAW_H__FC3BFB54_8D76_41E9_8464_A2028E89CB6B__INCLUDED_

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>

struct EDGE{//�߽ṹ���������Ϣͬʱ��ΪET���к�AET���еĵ�Ͱ
	int ymin;//��Сyֵ����ͬ��ET���еǼ����yֵ
	int ymax;//���yֵ����Ͱ����
	double xmin;//Сyֵ�˵��xֵ����Ͱ����
	double fm;//б�ʵ�������Ͱ����
	EDGE* next;//������һ����
	EDGE* last;//���ӵ���һ���ߣ�Ϊ������ʱʹ��
};
class CDraw  
{
public:
	// load the data file containing image: like 1234
	void loadImgData(const QString& filename);
	//�����ɨ��ת���㷨
	void Polygonfill(QPainter *pDC, QPolygonF* points, QColor color);
	//��ö���ε������ET��
	EDGE* GetET(QPolygonF* points);
	//�������ET������
	void SortET(EDGE* pEDGE);
	//�������AET������
	void SortAET(EDGE* pEDGE);
	//ɨ������������㷨
	void ScanlineSeedfill(QPainter *pDC, int x, int y, QColor boundaryvalue, QColor newvalue);
	void FloodFill(QPainter* pDC, int x, int y, QColor oldvalue, QColor newvalue);
	//Bresenham����Բ�㷨
	void BresenhamEllipse(QPainter *pDC, int x0, int y0, int a, int b, QColor color);
	//Bresenham��Բ�㷨
	void BresenhamCircle(QPainter *pDC, int x0, int y0, int R, QColor color);
	//�е㻭Բ��
	void MidpointCircle(QPainter* pDC, int x0, int y0, int R, QColor color);
	//��DDAֱ��ɨ��ת���㷨����ָ���߿��ֱ�߶�
	void DDALineWidth(QPainter *pDC, int x1, int y1, int x2, int y2, QColor color, int width);
	//��DDAֱ��ɨ��ת���㷨��������
	void DDALineStyle(QPainter* pDC, int x1, int y1, int x2, int y2, QColor color);
	//Bresenham�����㷨
	void BresenhamLine(QPainter* pDC, int x1, int y1, int x2, int y2, QColor color);
	//�е㻭�߷�
	void MidpointLine(QPainter* pDC, int x0, int y0, int x1, int y1, QColor color);
	//DDAֱ��ɨ��ת���㷨
	void DDALine(QPainter* pDC, int x1, int y1, int x2,int y2,QColor color);
	CDraw();
	virtual ~CDraw();

private:
	QVector<QVector<int> > imgData;

};

#endif // !defined(AFX_DRAW_H__FC3BFB54_8D76_41E9_8464_A2028E89CB6B__INCLUDED_)
