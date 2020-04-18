#ifndef ORTOGONALLINE_H
#define ORTOGONALLINE_H

#include "iline.h"
#include "ConnectionController/lineitems/ortogonallineitem.h"

//class-container for ortogonallineitems

class OrtogonalLine : public ILine
{
public:
    OrtogonalLine(Pin* a, Pin* b);
    void setLine(qreal x1, qreal y1, qreal x2, qreal y2) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    ~OrtogonalLine() override;
private:
    QVector<OrtogonalLineItem*> vec_of_sublines = {};
};

#endif // ORTOGONALLINE_H
