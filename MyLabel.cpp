#include "MyLabel.h"

MyLabel::MyLabel(QWidget* parent) :QLabel(parent) {
    flag = false;
    isPress = false;
    setMouseTracking(true);
}
MyLabel::~MyLabel() {

}

void MyLabel::mouseMoveEvent(QMouseEvent *event) {
    if (!flag || !isPress) return;
    QByteArray msg;
    SocketData *data = new SocketData;
    data->type = 6;
    data->x = event->x();
    data->y = event->y();
    msg.resize(sizeof(SocketData));
    memcpy(msg.data(), data, sizeof(SocketData));
    tcpSocket->write(msg);
}
