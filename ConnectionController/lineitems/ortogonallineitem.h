#ifndef ORTOGONALLINEITEM_H
#define ORTOGONALLINEITEM_H

#include <QGraphicsItem>
#include "Components/tools/component.h"

class OrtogonalLineItem : public QGraphicsLineItem
{
public:
    OrtogonalLineItem();
    virtual ~OrtogonalLineItem();

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

    void setOrientation(const Qt::Orientation& a);
private:
    Qt::Orientation orient = Qt::Orientation::Horizontal;
};

#endif // ORTOGONALLINEITEM_H
