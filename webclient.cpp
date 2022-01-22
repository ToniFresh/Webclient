#include <QDebug>
#include <QCoreApplication>

#include "webclient.h"

Webclient::Webclient(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
}

void Webclient::connected()
{
    // normgerechter HTTP Request zum Server
    m_socket->write(QString("GET / HTTP/1.1\r\nHost:" + m_hostname + "\r\n\r\n").toLocal8Bit());
}

void Webclient::readyRead()
{
    // HTTP-Antwort vom Server
    output->setText(m_socket->readAll());
    m_socket->disconnectFromHost();
}

void Webclient::on_goButton_clicked()
{
    goButton->setAutoFillBackground("color: green");
    m_hostname = input->text();
    unsigned short port = 80;
    m_socket = new QTcpSocket(this);

    // Signal ->  Slot-Verbindung
    connect(m_socket, &QTcpSocket::connected, this, &Webclient::connected);
    connect(m_socket, &QTcpSocket::readyRead, this, &Webclient::readyRead);

    m_socket->connectToHost(m_hostname, port); // TCP Connect
    if (!m_socket->waitForConnected(5000)) {
        output->setText("connect failed!");
        output->setStyleSheet("color:red;");
    }
    else
        output->setStyleSheet("color:black;");

}
