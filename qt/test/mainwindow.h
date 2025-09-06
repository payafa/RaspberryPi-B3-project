#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void connectToServer();
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onforwardButton_pressed();
    void onforwardButton_released();
    void onbackButton_pressed();
    void onbackButton_released();
    void onleftButton_pressed();
    void onleftButton_released();
    void onrightButton_pressed();
    void onrightButton_released();
    void socketSendMessage(char*);
    // void on_forwardButton_pressing();

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    QTcpSocket* socket = nullptr;
};
#endif // MAINWINDOW_H
