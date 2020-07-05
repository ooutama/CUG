#pragma once

#include <QGraphicsLineItem>
#include <QApplication>

#include "sommetgraphics.h"
#include "arete.h"


class AreteGraphics : public QGraphicsLineItem
{
    Q_DECLARE_TR_FUNCTIONS(Arete)
public:
    AreteGraphics(SommetGraphics* begin, SommetGraphics* end, const CUG::Arete& arete);
    ~AreteGraphics();

    enum { Type = UserType + 1 };

    inline int type() const
    {
        return Type;
    }

    const CUG::Arete& getGrapheArete() const;
    bool isOriented() const;
    bool isLoop() const;

    SommetGraphics* getStartSommetItem();
    SommetGraphics* getEndSommetItem();

    void trackSommets();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QPen getDefaultPen();

private:
    QPointF calcLabelPosition() const;
    QPointF calcLoopPosition() const;

    QPen defaultPen;
    QGraphicsSimpleTextItem* label;

    SommetGraphics* startSommetItem;
    SommetGraphics* endSommetItem;

    QPolygonF arrowHead;
    QGraphicsEllipseItem* loop;

    static const int ARROW_SIZE;
    static const QSizeF loopSize;

    CUG::Arete::const_ref grapheArete;
};
