#ifndef JOYSTICKWIDGET_H
#define JOYSTICKWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QScrollBar>
#include "bleinterface.h"

extern BLEInterface *m_bleInterface;


namespace Ui {
class JoystickWidget;
class JoystickPrototype;
}

typedef enum{
    JOYSTICK_LINEAR_SPEED,
    JOYSTICK_ANGULAR_SPEED
}JoystickType;



class JoystickWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    JoystickWidget(JoystickType joystick_type ,QWidget *parent);
    bool m_MousePressed, m_MouseUp, m_MouseDown, m_MouseLeft, m_MouseRight;
    void setCenter();
    qreal xCenter, yCenter;

protected:
//    void mousePressEvent(QMouseEvent * event);
//    void mouseReleaseEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
    bool event(QEvent * pEvent);
    QPoint GetCircleCorner(QPoint circle_center ,int circle_radius);
    QPoint getBigCircleCenter();

private:
    int xCoordinate, yCoordinate;
    int xVector, yVector;
    int xCorner, yCorner;
    JoystickType joystick_type_;
};


class JoystickPrototype : public QWidget
{
    Q_OBJECT

public:
    explicit JoystickPrototype(QWidget *parent = 0);
    ~JoystickPrototype();
    JoystickWidget *joystickWidget;
    JoystickWidget *RightjoystickWidget;

private:
    Ui::JoystickPrototype *ui;
};

#endif // JOYSTICKWIDGET_H
