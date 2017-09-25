#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

namespace Ui {
class MainWindow;

}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void ConnectToExchange();
    void DisconnectToExchange();
    void ClearData();

    void NetworkError(QNetworkReply::NetworkError);
    void ReadyRead();

protected:
    void timerEvent(QTimerEvent *event);

private:
    void ExecuteExchange();
    Ui::MainWindow *ui;

    QNetworkRequest  _req;
    QNetworkAccessManager *_netAccessManager;
    QNetworkReply *_resp;

    int _timerId;

};

#endif // MAINWINDOW_H
