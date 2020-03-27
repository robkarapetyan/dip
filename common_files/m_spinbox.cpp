#include "common_files/m_spinbox.h"

M_spinbox::M_spinbox(QWidget *parent) : QSpinBox (parent)
{
    setSuffix("%");
    setRange(50, 150);
    setSingleStep(10);
    setValue(100);
}

void M_spinbox::keyPressEvent(QKeyEvent *event)
{
    QSpinBox::keyPressEvent(event);
}
