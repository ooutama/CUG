#pragma once

#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QPainter>

#include "sommet.h"

class AreteGraphics;


class SommetGraphics : public QGraphicsEllipseItem
{
public:
    SommetGraphics(const QPointF& position, const CUG::Sommet& sommet,QColor color=colorTable[0]);
    ~SommetGraphics();

    enum { Type = UserType + 2 };

    inline int type() const
    {
        return Type;
    }

    static const int Sommet_DIAMETER;

    const CUG::Sommet& getGraphSommet() const;
    QList<AreteGraphics*> getAreteItems();

    void addAreteItem(AreteGraphics* areteItem);
    void removeAreteItem(AreteGraphics* areteItem);

    QPointF getCenterPos() const;
    void setCenterPos(qreal x, qreal y);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPen getDefaultPen() const;
    QBrush getDefaultBrush() const;

    void setColor(QColor color);
    QColor color() const;
    QColor onSelectedColor() const;
    void setOnSelectedColor(QColor color);

    static const QColor colorTable[];
    static const QColor defaultColor;
    static const QColor defaultOnSelectedColor;
    QGraphicsSimpleTextItem* label;

private:
    QPointF calcLabelPosition() const;

    const CUG::Sommet& graphesommet;

    QList<AreteGraphics*> aretes;

    QString text;
    QColor myColor;
    QColor selectedColor;

    QPen defaultPen;
    QBrush defaultBrush;
};
