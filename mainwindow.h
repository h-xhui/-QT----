#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QWidget>
#include <qfile.h>
#include <qfiledialog.h>
#include <QLabel>
#include <QImageReader>
#include <QMouseEvent>
#include <bits/stdc++.h>
#include <windows.h>
#include <QClipboard>
#include <CodeUtil.h>
#include <SocketData.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpSocket* getSocket() {
        return this->tcpSocket;
    }

private slots:

    void newConnect();

    void getRemoteScreenInfo();

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    //void mouseMoveEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void wheelEvent(QWheelEvent *event);

    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;

    QTcpSocket *tcpSocket;

    QString message;

    CodeUtil codeUtil;

    QLabel* label;
private:
    void init();
};

#endif // MAINWINDOW_H

