#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket(new QTcpSocket(this))
    // , timer(new QTimer(this))
{
    ui->setupUi(this);
    connectToServer();
    // timer -> setInterval(100);

    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(ui -> forwardButton, &QPushButton::pressed, this, &MainWindow::onforwardButton_pressed);
    connect(ui -> forwardButton, &QPushButton::released, this, &MainWindow::onforwardButton_released);
    connect(ui -> backwardButton, &QPushButton::pressed, this, &MainWindow::onbackwardButton_pressed);
    connect(ui -> backwardButton, &QPushButton::released, this, &MainWindow::onbackwardButton_released);
    connect(ui -> leftButton, &QPushButton::pressed, this, &MainWindow::onleftButton_pressed);
    connect(ui -> leftButton, &QPushButton::released, this, &MainWindow::onleftButton_released);
    connect(ui -> rightButton, &QPushButton::pressed, this, &MainWindow::onrightButton_pressed);
    connect(ui -> rightButton, &QPushButton::released, this, &MainWindow::onrightButton_released);
    // connect(timer, &QTimer::timeout, this, &MainWindow::on_forwardButton_pressing);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToServer()
{
    qDebug() << "尝试连接到服务器...";
    socket->connectToHost("192.168.5.17", 25500);
}

void MainWindow::onConnected()
{
    qDebug() << "成功连接到服务器";
}

void MainWindow::onDisconnected()
{
    qDebug() << "与服务器断开连接";
}

bool isSocketFree(char* message, QTcpSocket* socket){
    if(message && message[0] != '\0' && socket && socket -> state() == QAbstractSocket::ConnectedState) return true;
    else return false;
}

void MainWindow::socketSendMessage(char* message){
    if(isSocketFree(message, socket)){
        if(socket -> write(message))qDebug() << "发送命令成功";
        else qDebug() << "发送消息失败";
    }
}

void MainWindow::onReadyRead()
{
    while(socket->bytesAvailable() > 0)
    {
        QString string;
        QDataStream reader(socket);
        reader >> string;

        qDebug() << string;

        if(string == "forward")
        {
            //功能处理
        }

    }
}

void MainWindow::onforwardButton_pressed(){
    qDebug() << "前进按键触发";
    // timer -> start();

    char* cmd = "forward";
    socketSendMessage(cmd);
}

void MainWindow::onforwardButton_released(){
    qDebug() << "前进按键释放";
    // if(timer -> isActive())timer -> stop();

    char* cmd = "stop";
    socketSendMessage(cmd);
}

void MainWindow::onbackwardButton_pressed(){
    qDebug() << "后退按键触发";
    char* cmd = "backward";
    socketSendMessage(cmd);
}

void MainWindow::onbackwardButton_released(){
    qDebug() << "后退按键释放";
    char* cmd = "stop";
    socketSendMessage(cmd);
}

void MainWindow::onleftButton_pressed(){
    qDebug() << "左转按键触发";
    char* cmd = "spinleft";
    socketSendMessage(cmd);
}

void MainWindow::onleftButton_released(){
    qDebug() << "左转按键释放";
    char* cmd = "stop";
    socketSendMessage(cmd);
}

void MainWindow::onrightButton_pressed(){
    qDebug() << "右转按键触发";
    char* cmd = "spinright";
    socketSendMessage(cmd);
}

void MainWindow::onrightButton_released(){
    qDebug() << "右转按键释放";
    char* cmd = "stop";
    socketSendMessage(cmd);
}



// void MainWindow::on_forwardButton_pressing(){
//     qDebug() << "按键触发";

// }


