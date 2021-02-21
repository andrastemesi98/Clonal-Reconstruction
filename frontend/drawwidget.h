#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QGraphicsView>
#include <QGraphicsSimpleTextItem>
#include <QPainter>

class unittest; //to be able to test private functions

class DrawWidget : public QGraphicsView
{
    Q_OBJECT
    friend class unittest;

public:
    DrawWidget(QWidget *parent = nullptr);

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;
    void scaleWindow(qreal scaleFactor);

    void drawGraph(const std::vector<int> &num, const std::vector<int> &par, QString errosMes="");
    void welcomeText(QString text="Példagráfok");
    void calculations(const std::vector<int> &num, const std::vector<int> &par);


signals:

public slots:

private:
    QGraphicsScene *myscene;
    QVector<QGraphicsEllipseItem*> myCircles;
    QVector<QGraphicsSimpleTextItem*> nodeText;
    QVector<QGraphicsPathItem*> pathVec;
    QVector<QGraphicsPolygonItem*> myTriangles;

    QGraphicsTextItem *solution;
    QGraphicsTextItem *welText;


    bool _drag;
    int _dragStartX, _dragStartY;

};

#endif // DRAWWIDGET_H
