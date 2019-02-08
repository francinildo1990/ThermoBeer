#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&serial, SIGNAL(readyRead()),this,SLOT(dados()));
    for(auto& item : QSerialPortInfo::availablePorts()){
     ui->cbox_serial->addItem(item.portName());
    }

    for(auto& item : QSerialPortInfo::standardBaudRates()){
     ui->cbox_veloc->addItem(QString::number(item) );
}
}

MainWindow::~MainWindow()
{
    serial.close();
    delete ui;
}


void MainWindow::dados()
{
    auto info=serial.readAll();
    auto inform = QJsonDocument::fromJson(info).object().toVariantMap();

    if(inform.contains("SENSOR"))
        ui->lcd_temp->display(inform["SENSOR"].toString());
    if(inform.contains("STATUS"))
        ui->lb_status->setText(inform["STATUS"].toString());

}

void MainWindow::on_pb_conectar_clicked()
{
    QString cor="yellow";
    QString status="Conectado";
     serial.setPortName(ui->cbox_serial->currentText());
     serial.setBaudRate(ui->cbox_veloc->currentText().toInt());
    if(serial.open(QIODevice::ReadWrite)){
        ui->lb_status->setText(status);
        ui->lb_status->setStyleSheet("color: "+cor);
    }
    else {
     qDebug()<<"Sem sinal";
    }
}



void MainWindow::on_pb_reiniciar_clicked()
{
    serial.clear();
    ui->cbox_serial->clearEditText();
    ui->cbox_veloc->clearEditText();
    QString cor_2="red";
    QString status_2="Aguardando Conexão...";
    ui->lb_status->setText(status_2);
    ui->lb_status->setStyleSheet("color: "+cor_2);
}





