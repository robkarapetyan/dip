#ifndef M_SPINBOX_H
#define M_SPINBOX_H
#include <QSpinBox>

class M_spinbox : public QSpinBox
{
public:
    M_spinbox(QWidget *parent);
    void keyPressEvent(QKeyEvent *event);
};

#endif // M_SPINBOX_H
