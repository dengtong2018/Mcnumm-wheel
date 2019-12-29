#include "joystickprototype.h"
#include "ui_joystickprototype.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QtMath>
#include <QString>
#include <QTouchEvent>
#include "message.h"
#include <cstdlib>
extern Message *message;


JoystickPrototype::JoystickPrototype(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JoystickPrototype)
{
    ui->setupUi(this);

    joystickWidget = new JoystickWidget(JOYSTICK_LINEAR_SPEED, this);
    ui->horizontalLayout->addWidget(joystickWidget);
    joystickWidget->setAttribute(Qt::WA_AcceptTouchEvents);

    RightjoystickWidget = new JoystickWidget(JOYSTICK_ANGULAR_SPEED ,this);
    RightjoystickWidget->setAttribute(Qt::WA_AcceptTouchEvents);
    ui->horizontalLayout->addWidget(RightjoystickWidget);
}

JoystickPrototype::~JoystickPrototype()
{
    delete ui;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
JoystickWidget::JoystickWidget(JoystickType joystick_type ,QWidget *parent)
    : QOpenGLWidget(parent),
      joystick_type_(joystick_type)
{
    this->setAutoFillBackground(true);
}

//void JoystickWidget::mousePressEvent(QMouseEvent* event)
//{
//    Q_UNUSED(event)
//    m_MousePressed = true;
//}

//void JoystickWidget::mouseReleaseEvent(QMouseEvent *event)
//{
//    Q_UNUSED(event)
//    update();
//    m_MousePressed = false;
//}

//void JoystickWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    Q_UNUSED(event)
//    if (event->type() == QEvent::MouseMove)
//    {
//        xCenter = event->pos().x();
//        yCenter = event->pos().y();
//        update();
//    }
//}


bool JoystickWidget::event(QEvent * pEvent)
{
    //qDebug() << "===========" << pEvent;
    QTouchEvent *touchEvent;
    QList<QTouchEvent::TouchPoint> point;
    switch (pEvent->type())
    {
    case QEvent::TouchBegin:
        m_MousePressed = true;
        return true;
    case QEvent::TouchUpdate:
        pEvent->accept();
        touchEvent = static_cast<QTouchEvent *>(pEvent);
        point = touchEvent->touchPoints();
        for(uint8_t i = 0 ; i < point.size(); i++){
            //qDebug()<<"x:"<<point[i].pos().x()<<"y:"<<point[i].pos().y();
        }
        if(point.size() != 0){
            xCenter = point[0].pos().x();
            yCenter = point[0].pos().y();
            update();
        }
        return true;
    case QEvent::TouchEnd:
        m_MousePressed = false;
        update();
        return true;
    case QEvent::GraphicsSceneResize:

        break;
    default:
        break;
    }

    return QWidget::event(pEvent);
}

void JoystickWidget::setCenter()
{
    xCenter = getBigCircleCenter().x();
    yCenter = getBigCircleCenter().y();
}

QPoint JoystickWidget::getBigCircleCenter()
{
    QPoint circle_center;
    circle_center.setX(width()/2);
    circle_center.setY(height()/2);
    return circle_center;
}

void JoystickWidget::paintEvent(QPaintEvent *event)
{
    if(!m_MousePressed){
        setCenter();
    }

    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    //draw background circle white
    QBrush outerBrush(Qt::white);
    QPen outerPen(Qt::black, 2);
    int big_circle_radius;
    int radius;

    painter.setPen(outerPen);
    painter.setBrush(outerBrush);
    big_circle_radius = (width()>500?500:width())/2;
    radius = big_circle_radius/2;
    painter.drawEllipse(getBigCircleCenter() ,big_circle_radius, big_circle_radius);
    //painter.drawEllipse(width()/2 - x/2 ,height()/2 - y/2 , x, y);

//    //draw square lines for referential ease of use
//    painter.drawLine(width()/2, 0, width()/2, width()/2 - 60);
//    painter.drawLine(width()/2, width()/2 + 60, width()/2, width());
//    painter.drawLine(0, width()/2, width()/2 - 60, width()/2);
//    painter.drawLine(width()/2 + 60, width()/2, width(), width()/2);

    //draw frontground circle
    qreal small_circle_length = xCenter - getBigCircleCenter().x();
    qreal small_circle_heigh = yCenter - getBigCircleCenter().y();

    //check inner circle for collision with edges of larger circle
    if(qPow(qPow(small_circle_heigh, 2) + qPow(small_circle_length, 2), .5) > big_circle_radius)
    {
        qreal theta = qAtan(small_circle_heigh/small_circle_length);

        qreal newX = qCos(theta)*(big_circle_radius);
        qreal newY = qSin(theta)*(big_circle_radius);

        if(small_circle_length >= 0)
        {
            xCenter = getBigCircleCenter().x() + newX;
            yCenter = getBigCircleCenter().y() + newY;
        }
        else
        {
            xCenter = getBigCircleCenter().x() - newX;
            yCenter = getBigCircleCenter().y() - newY;
        }
    }

    MsgId msg_id = MSGID_MAX;
    QByteArray buffer;
    qreal x_volume =  -(getBigCircleCenter().x() - xCenter);
    qreal y_volume =  getBigCircleCenter().y() - yCenter;

    if(JOYSTICK_LINEAR_SPEED == joystick_type_){
        LinerSpeedJoystick liner_speed;
        liner_speed.move_left_right = static_cast<int8_t>(x_volume/(big_circle_radius)*100.0);
        liner_speed.move_forward_back = static_cast<int8_t>(y_volume/(big_circle_radius)*100.0);
        buffer = QByteArray((char *)&liner_speed ,sizeof(LinerSpeedJoystick));
        msg_id = MSGID_LEFT_JOYSTICK;
    }else if(JOYSTICK_ANGULAR_SPEED == joystick_type_){
        AngularSpeedJoystick angular_speed;
        angular_speed.rotate = static_cast<int8_t>(x_volume/(big_circle_radius)*100.0);
        buffer = QByteArray((char *)&angular_speed ,sizeof(AngularSpeedJoystick));
        msg_id = MSGID_RIGHT_JOYSTICK;
    }
    message->PackAndSend(msg_id , buffer);

    painter.drawEllipse(QPoint(xCenter ,yCenter), radius, radius);
}


