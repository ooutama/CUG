#include "aretegraphics.h"
#include <QtMath>


using namespace CUG;


const int AreteGraphics::ARROW_SIZE = 15;
const QSizeF AreteGraphics::loopSize = QSizeF(50, 50);

AreteGraphics::AreteGraphics(SommetGraphics* begin, SommetGraphics* end, const Arete &arete)
    : grapheArete(arete)
{
    startSommetItem = begin;
    endSommetItem = end;

    startSommetItem->addAreteItem(this);
    endSommetItem->addAreteItem(this);

    setFlags(ItemIsSelectable);
    setPen(QPen(Qt::darkGray, 3));
    defaultPen = pen();
    setZValue(-1);

    label = new QGraphicsSimpleTextItem(QString::number(getGrapheArete().getPoids()), this);
    label->setFont(QFont("Helvetica", 12));

    if (isLoop())
    {
        QRectF loopRect = QRectF(QPointF(0, 0), loopSize);
        loop = new QGraphicsEllipseItem(loopRect, startSommetItem);
        loop->setFlags(ItemStacksBehindParent);
        loop->setPen(getDefaultPen());
    }

    trackSommets();
}

AreteGraphics::~AreteGraphics()
{
    getStartSommetItem()->removeAreteItem(this);
    getEndSommetItem()->removeAreteItem(this);
    delete label;
    if (isLoop())
        delete loop;
}

const Arete& AreteGraphics::getGrapheArete() const
{
    return grapheArete;
}

bool AreteGraphics::isOriented() const
{
    return grapheArete.isOriented();
}

bool AreteGraphics::isLoop() const
{
    return grapheArete.isLoop();
}

SommetGraphics* AreteGraphics::getStartSommetItem()
{
    return startSommetItem;
}

SommetGraphics* AreteGraphics::getEndSommetItem()
{
    return endSommetItem;
}

void AreteGraphics::trackSommets()
{
    setLine(QLineF(startSommetItem->getCenterPos(), endSommetItem->getCenterPos()));
    if (isLoop())
        loop->setPos(calcLoopPosition());
}

void AreteGraphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (getStartSommetItem()->collidesWithItem(getEndSommetItem()))
        QGraphicsLineItem::paint(painter, option, widget);

    if (!isLoop())
    {
        label->setText(QString::number(getGrapheArete().getPoids()));
        if (getGrapheArete().getPoids() == 0)
            label->hide();
        else
            label->show();
        label->setPos(calcLabelPosition());
    }
    else
        label->hide();

    if (isOriented())
    {
        painter->setPen(pen());
        painter->setBrush(Qt::darkGray);

        SommetGraphics* startSommet = getStartSommetItem();
        QLineF fullLine = QLineF(startSommetItem->getCenterPos(), endSommetItem->getCenterPos());

        int SommetRadius = SommetGraphics::Sommet_DIAMETER / 2;
        QPointF intersectionPoint = fullLine.pointAt(1 - ((SommetRadius + 5) / fullLine.length()));

        setLine(QLineF(intersectionPoint, startSommet->getCenterPos()));

        const qreal Pi = 3.14;

        double angle = ::acos(line().dx() / line().length());
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * ARROW_SIZE,
                                        cos(angle + Pi / 3) * ARROW_SIZE);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * ARROW_SIZE,
                                        cos(angle + Pi - Pi / 3) * ARROW_SIZE);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
    }

    QGraphicsLineItem::paint(painter, option, widget);
}

QRectF AreteGraphics::boundingRect() const
{
    if (isOriented())
    {
        qreal extra = (pen().width() + ARROW_SIZE) / 2.0;
        QSize areteSize = QSize(line().p2().x() - line().p1().x(),
                               line().p2().y() - line().p1().y());
        return QRectF(line().p1(), areteSize).normalized().adjusted(-extra, -extra, extra, extra);
    }
    else
    {
        if (isLoop())
        {
            QRectF rect = loop->boundingRect();
            return QRectF(mapFromItem(loop, rect.topLeft()), loopSize);
        }
    }
    return QGraphicsLineItem::boundingRect();
}

QPainterPath AreteGraphics::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    if (isOriented())
        path.addPolygon(arrowHead);
    else
        if (isLoop())
            path = mapFromItem(loop, loop->shape());
    return path;
}

QPen AreteGraphics::getDefaultPen()
{
    return defaultPen;
}

QPointF AreteGraphics::calcLabelPosition() const
{
    QString text = QString::number(getGrapheArete().getPoids());
    QFontMetrics metrics(label->font());
    QPointF center = boundingRect().center();
    int x = center.x() - metrics.width(text)/2;
    int y = center.y() - metrics.height();
    return QPointF(x, y);
}

QPointF AreteGraphics::calcLoopPosition() const
{
    QPointF SommetCenter = startSommetItem->mapFromScene(startSommetItem->getCenterPos());
    return QPointF(SommetCenter.x() - loopSize.width() / 2, SommetCenter.y() - loopSize.height());
}
