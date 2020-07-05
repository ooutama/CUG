#include "sommetgraphics.h"
#include "aretegraphics.h"
#include <QPropertyAnimation>


using namespace CUG;


const int SommetGraphics::Sommet_DIAMETER = 50;

SommetGraphics::SommetGraphics(const QPointF& position, const Sommet &sommet,QColor color)
    : graphesommet(sommet)
{
    setRect(QRectF(0, 0, Sommet_DIAMETER, Sommet_DIAMETER));
    setCenterPos(position.x(), position.y());
    this->myColor=color;
    setPen(QPen(Qt::cyan, 2));
    setBrush(QBrush(myColor));
    selectedColor = defaultOnSelectedColor;
    defaultPen = pen();
    defaultBrush = brush();
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);

    label = new QGraphicsSimpleTextItem(getGraphSommet().getText(), this);
    label->setFont(QFont("Helvetica", 12));
}

SommetGraphics::~SommetGraphics()
{
    qDeleteAll(aretes);
    aretes.clear();
    delete label;
}


const Sommet& SommetGraphics::getGraphSommet() const
{
    return graphesommet;
}

QList<AreteGraphics *> SommetGraphics::getAreteItems()
{
    return aretes;
}

void SommetGraphics::addAreteItem(AreteGraphics *areteItem)
{
    if (areteItem && !aretes.contains(areteItem))
        aretes.append(areteItem);
}

void SommetGraphics::removeAreteItem(AreteGraphics *areteItem)
{
    if (areteItem)
        aretes.removeOne(areteItem);
}

QPointF SommetGraphics::getCenterPos() const
{
    return mapToScene(boundingRect().center());
}

void SommetGraphics::setCenterPos(qreal x, qreal y)
{
    qreal leftTopX = x - Sommet_DIAMETER/2;
    qreal leftTopY = y - Sommet_DIAMETER/2;

    leftTopX = leftTopX < 0 ? 0 : leftTopX;
    leftTopY = leftTopY < 0 ? 0 : leftTopY;

    setPos(leftTopX, leftTopY);
}

QVariant SommetGraphics::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange)
        foreach (AreteGraphics* arete, aretes)
            arete->trackSommets();
    return QGraphicsItem::itemChange(change, value);
}

void SommetGraphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if (isSelected())
        myColor = onSelectedColor();
    else
        myColor = defaultColor;
    label->setText(getGraphSommet().getText());
    label->setPos(calcLabelPosition());
    QGraphicsEllipseItem::paint(painter, option, widget);
}

QPen SommetGraphics::getDefaultPen() const
{
    return defaultPen;
}

QBrush SommetGraphics::getDefaultBrush() const
{
    return defaultBrush;
}

QPointF SommetGraphics::calcLabelPosition() const
{
    QString text = getGraphSommet().getText();
    if (text.length() != 0)
    {
        QFontMetrics metrics(label->font());
        if(text.length()<5)
            return QPointF(Sommet_DIAMETER/2 - metrics.horizontalAdvance(text)/2, 32.5 - metrics.height());//we can use width
        else {
            return QPointF(Sommet_DIAMETER/2 - metrics.horizontalAdvance(text)/2, -5 - metrics.height());// we can use width
        }
    }
    else
        return QPointF();
}

void SommetGraphics::setColor(QColor color)
{
    this->myColor = color;
}

QColor SommetGraphics::color() const {

    return this->myColor;
}

QColor SommetGraphics::onSelectedColor() const
{
    return this->selectedColor;
}

void SommetGraphics::setOnSelectedColor(QColor color)
{
   this->selectedColor = color;
}


const QColor SommetGraphics::colorTable[] = {
    QColor(255,255,255),
    QColor(50,205,50),

};

const QColor SommetGraphics::defaultColor = colorTable[0];
const QColor SommetGraphics::defaultOnSelectedColor = colorTable[1];
