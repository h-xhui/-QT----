#ifndef MYLABEL_H
#define MYLABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <mainwindow.h>
#include <QTcpSocket>
#include <CodeUtil.h>
class MyLabel : public QLabel {
    Q_OBJECT;
public:
    MyLabel(QWidget *parent = 0);
    ~MyLabel();
public slots:
    void mouseMoveEvent(QMouseEvent *ev);
private:
    int mouse_x;
    int mouse_y;
    bool flag;
    QTcpSocket* tcpSocket;
    bool isPress;
public:
    QPair<int, int> getMousePos() {
        return {mouse_x, mouse_y};
    }
    void setSocket(QTcpSocket* tcpSocket) {
        this->tcpSocket = tcpSocket;
        flag = true;
    }
    void setPress(bool flag) {
        isPress = flag;
    }
};

#endif // MYLABEL_H
