#include "drawwidget.h"
//for pow use
#include "math.h"

#include <QMessageBox>
#include <QString>
#include <QKeyEvent>
#include <QScrollBar>

//Constructor: create scene, add border, initial scale
DrawWidget::DrawWidget(QWidget *parent)
    :QGraphicsView (parent)
{
    myscene = new QGraphicsScene(this);
    setScene(myscene);
    scale(1.2, 1.2);
    setMinimumHeight(150);
    setStyleSheet("border: 2px solid black line");
}

//Zoom in or out
void DrawWidget::scaleWindow(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.5 or factor > 10)
        return;

    scale(scaleFactor, scaleFactor);
}

//Set mouse scrolling available
void DrawWidget::wheelEvent(QWheelEvent *event)
{
    scaleWindow(pow(2., -event->angleDelta().y() / 150.0));
}

//Set gray and white bgcolor
void DrawWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    QRectF sceneRect = this->sceneRect();

    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::lightGray);
    gradient.setColorAt(1, Qt::white);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::green);
}

//To be able to grab scene
void DrawWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _drag = true;
        _dragStartX = event->x();
        _dragStartY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

//Stop moving
void DrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _drag = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

//Move scene according to mouse move while leftclicked
void DrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (_drag)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - _dragStartX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - _dragStartY));
        _dragStartX = event->x();
        _dragStartY = event->y();
        event->accept();
        return;
    }
    event->ignore();

}

//Type only text on the scene
void DrawWidget::welcomeText(QString text)
{
    QFont font;
    font.setBold(true);
    font.setPointSize(14);

    welText = new QGraphicsTextItem();
    welText->setPos(5, 125);
    welText->setPlainText(text);
    welText->setFont(font);
    myscene->addItem(welText);
}

//Type caption under graph
//How to calculate likelihood-function
//And value of function
void DrawWidget::calculations(const std::vector<int> &num, const std::vector<int> &par)
{
    QString calc="";
    long long int maxprob=1;
    for(uint i=0; i<par.size(); ++i)
    {
        int calcnum=num[uint(par[i])]; //no negatives since its a full graph
        for(uint j=0; j<i; ++j)
        {
            if(par[j]==par[i])
            {
                calcnum=calcnum-num[j+1];
            }
        }
        calc+=QString::number(calcnum);
        maxprob=maxprob*calcnum;
        if(i!=par.size()-1)
        {
            calc+=" * ";
        }
    }
    calc+=" = ";
    calc+=QString::number(maxprob);
    solution = new QGraphicsTextItem();
    solution->setPos(5, 95);
    solution->setPlainText(calc);
    myscene->addItem(solution);
}

//Draw the special graph
//Use ellipses and Bezier-curve
void DrawWidget::drawGraph(const std::vector<int> &num, const std::vector<int> &par, QString errorMes)
{
    //Set scene size accoring to the number of nodes
    //Make it central of the view
    myscene->setSceneRect(0, 0, 40+50*num.size(), 120);
    scrollContentsBy(50, 50);
    if (num.size()!=par.size()+1)
    {
        QMessageBox::information(nullptr, "Error", "Hibás függvényparaméterezés!");
    }
    //Vectors containing ellipses and labels
    myCircles.resize(int(num.size()));
    nodeText.resize(myCircles.size());

    //Draw nodes
    //Set positions correct
    for(int i=0; i<myCircles.size(); ++i)
    {
        QPen myNodes = QPen(Qt::red);
        myNodes.setWidth(2);
        myCircles[i] = myscene->addEllipse(QRectF(10+50*i, 50, 40, 40), myNodes, QBrush("#FFFFB7"));
        nodeText[i] = new QGraphicsSimpleTextItem();
        nodeText[i]->setText(QString::number(num[uint(i)]));
        nodeText[i]->setPos(25+50*i, 60);
        myscene->addItem(nodeText[i]);
    }

    //Draw edges
    //If the nodes connected by the edges are further
    //Make the Bezier-curve higher
    for(uint i=0; i<par.size(); ++i)
    {
        if(par[i]!=-1)
        {
            int dist=abs(int(i)+1-par[i]);
            myTriangles.resize(myTriangles.size()+1);
            pathVec.resize(pathVec.size()+1);
            QPainterPath myPath;
            myPath.moveTo(10+50*par[i]+20, 50);
            //Draw Bezier curve according to the distance of the nodes
            myPath.cubicTo(10+50*par[i]+20, 50-30-7*dist, 10+50*(i+1)+20, 50-30-7*dist, 10+50*(i+1)+20, 50);
            QPen myEdges = QPen(Qt::black);
            myEdges.setWidth(2);
            pathVec[pathVec.size()-1] = myscene->addPath(myPath, myEdges);
        }
    }
    //Draw edge arrows
    //red if forward
    //green if backward (illustration only)
    //triangle correct angle
    for(uint i=0; i<par.size(); ++i)
    {
        if(par[i]!=-1)
        {
            int dist=int(i)+1-par[i];
            QPolygonF Triangle;
            if(dist>0){
                Triangle.append(QPointF(10+50*(i+1)+20, 50));
                Triangle.append(QPointF(10+50*(i+1)+24, 44));
                Triangle.append(QPointF(10+50*(i+1)+14, 46));
                Triangle.append(QPointF(10+50*(i+1)+20, 50));
                myTriangles[myTriangles.size()-1] = myscene->addPolygon(Triangle, QPen(Qt::black), QBrush(Qt::red));
            } else{
                Triangle.append(QPointF(10+50*(i+1)+20, 50));
                Triangle.append(QPointF(10+50*(i+1)+15, 44));
                Triangle.append(QPointF(10+50*(i+1)+25, 46));
                Triangle.append(QPointF(10+50*(i+1)+20, 50));
                myTriangles[myTriangles.size()-1] = myscene->addPolygon(Triangle, QPen(Qt::black), QBrush(Qt::green));
            }
        }
    }

    //If not an illustration and full tree (n-1 edges)
    //Create lable with likelihood-function calculations
    if(int(par.size())==pathVec.size() and errorMes=="")
    {
        calculations(num, par);
    }
    //If illustration set given text
    if(errorMes!="")
    {
        solution = new QGraphicsTextItem();
        solution->setPos(5, 95);
        solution->setPlainText(errorMes);
        myscene->addItem(solution);
    }
    horizontalScrollBar()->setSliderPosition(0);
}
