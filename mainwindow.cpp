#include <QtNetwork>
#include <QUrl>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // connect signal/slot
    connect(ui->pbDisconnect, SIGNAL(clicked()),
            this, SLOT(DisconnectToExchange()));

    connect(ui->pbConnect, SIGNAL(clicked()),
            this, SLOT(ConnectToExchange()));

    connect(ui->pbClear, SIGNAL(clicked()),
            this, SLOT(ClearData()));


    _netAccessManager = new QNetworkAccessManager(this);
    QUrl url("https://www.bitstamp.net/api/v2/ticker/btceur/");
    _req.setUrl(url);

    ExecuteExchange();

    ui->pbConnect->setEnabled(false);
    _timerId = startTimer(10000);
    ui->teExchange->setPlainText("Started");
}


MainWindow::~MainWindow()
{
    killTimer(_timerId);
    delete _resp;
    delete _netAccessManager;
    delete ui;
}

void MainWindow::ConnectToExchange()
{
    ui->pbDisconnect->setEnabled(true);
    ui->pbConnect->setEnabled(false);

    ExecuteExchange();
}

void MainWindow::DisconnectToExchange()
{
    ui->pbDisconnect->setEnabled(false);
    ui->pbConnect->setEnabled(true);
    ui->teExchange->clear();

    delete _resp;
}

void MainWindow::ClearData()
{
    ui->teExchange->clear();
}

void MainWindow::ReadyRead()
{
    std::vector<char> buf;
    qint64 chunk;

    while(_resp->bytesAvailable() > 0) {
        chunk = _resp->bytesAvailable();

        buf.resize(chunk + 1);
        memset(& buf[0], 0, chunk + 1);

        if(chunk != _resp->read(& buf[0], chunk)) {
            ui->lblServiceStatus->setText("Service error");
            ui->teExchange->clear();
            return;
        } else
            ui->lblServiceStatus->setText("Service ready");

        std::string str(buf.begin(),buf.end());
        ui->teExchange->setPlainText(QString::fromStdString(str));

    }
}


void MainWindow::NetworkError(QNetworkReply::NetworkError) {
    ui->lblServiceStatus->setText("Service error");
    ui->teExchange->clear();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if( ui->pbConnect->isEnabled() == true)
        return;

    qDebug() << "Update...";
    ExecuteExchange();
}

void MainWindow::ExecuteExchange()
{
    _resp = _netAccessManager->get(_req);
    connect(_resp, SIGNAL(readyRead()),                          this, SLOT(ReadyRead()));
    connect(_resp, SIGNAL(error(QNetworkReply::NetworkError)),   this, SLOT(NetworkError(QNetworkReply::NetworkError)));
}


