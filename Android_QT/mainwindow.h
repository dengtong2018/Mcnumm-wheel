#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bleinterface.h"
#include "joystickprototype.h"
#include <QTouchDevice>
#include <QTouchEvent>
#include "message.h"


extern BLEInterface *m_bleInterface;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_bleScanButton_clicked();

    void on_connectButton_clicked();

    void on_sendButton_clicked();

    void on_servicesComboBox_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    JoystickPrototype *Joystick;
    void dataReceived(QByteArray data);
};

#endif // MAINWINDOW_H
