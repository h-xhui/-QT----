#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    init();
    newConnect();
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(getRemoteScreenInfo()));
    MyLabel* label = (MyLabel*)ui->label;
    label->setSocket(this->tcpSocket);
    QClipboard *board = QGuiApplication::clipboard();
    connect(board, &QClipboard::dataChanged, this, [=](){
        QString msg = board->text();
        qDebug() << msg;
        if (msg.mid(0, 8) == "file:///") {
            QString path = msg.mid(8);
            QFileInfo info(path);
            QString fileName = info.fileName();
            qint64 fileSize = info.size();
            QFile file;
            file.setFileName(path);
            if(!file.open(QIODevice::ReadOnly)){
               qDebug()<<"文件打开失败";
               return;
            }
            QTimer timer;
            QString head = QString("%1&%2&").arg(fileName).arg(fileSize);
            qDebug() << head;
            QByteArray array;
            array.resize(1);
            array[0] = 0;
            array.append(head.toUtf8());
            tcpSocket->flush();
            qint64 len = tcpSocket->write(array);
            tcpSocket->flush();
            qint64 len2 = 0, sendSize = 0;
            QByteArray data;
            data.resize(1);
            data[0] = 1;
            data.append(file.readAll());
            len2 = tcpSocket->write(data);
            qDebug()<<len2;

            qDebug()<<"文件发送完毕";
            file.close();
        } else {
            QByteArray sendMsg;
            SocketData *data = new SocketData;
            data->type = 6;
            char* tmp;
            tmp = msg.toUtf8().data();
            //memcpy(data->data, tmp, msg.toUtf8().data())
            sendMsg.resize(sizeof(SocketData));
            memcpy(sendMsg.data(), data, sizeof(SocketData));
            tcpSocket->write(sendMsg);
        }

        //tcpSocket->write(msg.toUtf8());
    }, Qt::QueuedConnection);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::init() {
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    ui->label->resize(1440, 900);
    this->resize(1600, 1200);
}

void MainWindow::newConnect()
{
    tcpSocket->abort(); //取消已有的连接

    //连接到主机，这里从界面获取主机地址和端口号
    tcpSocket->connectToHost(QString("192.168.23.128"),8888);
}

void MainWindow::getRemoteScreenInfo()
{
    QByteArray array;
    while(tcpSocket->waitForReadyRead(20)){
        array.append((QByteArray)tcpSocket->readAll());
    }

    QBuffer buffer(&array);
    buffer.open(QIODevice::ReadOnly);

    QImageReader reader(&buffer,"JPG");
    QImage img = reader.read();
    //qDebug() << img.width() << " " << img.height();
    if(!img.isNull()){
        ui->label->setPixmap(QPixmap::fromImage(img));
    } else {
        qDebug()<<"error"<<endl;
    }
    array.clear();
    buffer.close();
}

//鼠标按下事件
void MainWindow::mousePressEvent(QMouseEvent *event) {
    MyLabel* label = (MyLabel*)ui->label;
    label->setPress(true);
    QString str="("+QString::number(event->x())+","+QString::number(event->y())+")";
    int x = ui->label->pos().rx();
    int y = ui->label->pos().ry();
    int mouse_x = event->x();
    int mouse_y = event->y();
    int abs_x = mouse_x - x;
    int abs_y = mouse_y - y;
//    QString msg;
    if(abs_x < 0 || abs_y < 0) return;
    QByteArray msg;
    SocketData *data = new SocketData;
    data->type = 1;
    data->x = abs_x;
    data->y = abs_y;
    if(event->button()==Qt::LeftButton){
        data->type = 2;
    } else if(event->button()==Qt::RightButton) {
        data->type = 3;
    }
    msg.resize(sizeof(SocketData));
    memcpy(msg.data(), data, sizeof(SocketData));
    tcpSocket->write(msg);
}

//void MainWindow::mouseMoveEvent(QMouseEvent *event) {
//    qDebug()<<event->pos();
//    QString msg = "3 " + QString::number(event->x()) + " " + QString::number(event->y());
//    tcpSocket->write(msg.toUtf8());
//}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QByteArray msg;
    SocketData *data = new SocketData;
    data->type = 4;
    data->x = event->key();
    msg.resize(sizeof(SocketData));
    memcpy(msg.data(), data, sizeof(SocketData));
    tcpSocket->write(msg);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    QByteArray msg;
    SocketData *data = new SocketData;
    data->type = 9;
    data->x = event->key();
    msg.resize(sizeof(SocketData));
    memcpy(msg.data(), data, sizeof(SocketData));
    tcpSocket->write(msg);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    MyLabel* label = (MyLabel*)ui->label;
    label->setPress(false);
    QString str="("+QString::number(event->x())+","+QString::number(event->y())+")";
    int x = ui->label->pos().rx();
    int y = ui->label->pos().ry();
    int mouse_x = event->x();
    int mouse_y = event->y();
    int abs_x = mouse_x - x;
    int abs_y = mouse_y - y;
    if(abs_x < 0 || abs_y < 0) return;
    QByteArray msg;
    SocketData *data = new SocketData;
    data->type = 1;
    data->x = abs_x;
    data->y = abs_y;
    if(event->button()==Qt::LeftButton){
        data->type = 7;
    } else if(event->button()==Qt::RightButton) {
        data->type = 8;
    }
    msg.resize(sizeof(SocketData));
    memcpy(msg.data(), data, sizeof(SocketData));
    tcpSocket->write(msg);

}

void MainWindow::wheelEvent(QWheelEvent *event) {
    int x = ui->label->pos().rx();
    int y = ui->label->pos().ry();
    int mouse_x = event->x();
    int mouse_y = event->y();
    int abs_x = mouse_x - x;
    int abs_y = mouse_y - y;
    if (abs_x < 0 || abs_y < 0) return;
    QByteArray msg;
    SocketData *data = new SocketData;
    data->type = 5;
    data->x = abs_x;
    data->y = abs_y;
    data->delta = event->delta();
    msg.resize(sizeof(SocketData));
    memcpy(msg.data(), data, sizeof(SocketData));
    tcpSocket->write(msg);
}

