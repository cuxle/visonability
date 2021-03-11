#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>
#include "device.h"
#include "devicemodel.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSplitter *spliter = ui->splitter;
//    QList<int> size;
//    size.append(1);
//    size.append(3);
    int firstWidth = this->width() / 4;
    int secondWidth = this->width() * 3 / 4;
    spliter->setSizes(QList<int>()<< firstWidth << secondWidth);

    initDevices();

//    connect(ui->lineEditHost, &QLineEdit::textChanged, m_client, &QMqttClient::setHostname);
//    connect(ui->spinBoxPort, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::setClientPort);
    //updateLogStateChange();

    m_model = new DeviceModel();
    ui->listView->setModel(m_model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_buttonPublish_clicked()
//{
//    if (m_client->publish(ui->lineEditTopic->text(), ui->lineEditMessage->text().toUtf8()) == -1)
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
//}

//void MainWindow::on_buttonSubscribe_clicked()
//{
//    auto subscription = m_client->subscribe(ui->lineEditTopic->text());
//    if (!subscription) {
//        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
//        return;
//    }
//}

//void MainWindow::on_buttonPing_clicked()
//{
//    ui->buttonPing->setEnabled(false);
//    m_client->requestPing();
//}


void MainWindow::initDevices()
{
    for (int i = 0; i < 20; i++) {
        Device *d = new Device(i, QString("dev%1").arg(i));
        QVariant variant = QVariant::fromValue(static_cast<void *>(d));
        QListWidgetItem  *item = new QListWidgetItem;
        item->setText(d->name());
        item->setData(Qt::UserRole, variant);
        //ui->listWidget->addItem(item);
    }
}

void MainWindow::fillDeviceInfo(Device *device)
{
    if (device == nullptr) return;
    QString devName = device->name();
    ui->leDeviceName->setText(devName);
    // 高报警值
    // 低报警值
    // Mor值 能见度值

}


void MainWindow::on_leDeviceName_editingFinished()
{
    if (m_curDevice != nullptr) {
        m_curDevice->setName(ui->leDeviceName->text());
    }
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    qDebug() << __func__ << index.row();
    Device *device = static_cast<Device *>(index.internalPointer());
    if (device == nullptr) {
        qDebug() << "cast failed";
    } else {
        qDebug() << "cast sucessfully";
        qDebug() << "name:" << device->name();
        ui->leDeviceName->setText(device->name());
    }
    m_curDevice = device;
}

void MainWindow::on_pbForceHighWarn_clicked()
{
    if (m_curDevice != nullptr) {
        m_curDevice->forceHighWarning();
    }
}

void MainWindow::on_pbForceLowWarn_clicked()
{
    if (m_curDevice != nullptr) {
        m_curDevice->forceLowWarning();
    }
}

void MainWindow::on_pbReboot_clicked()
{
    if (m_curDevice != nullptr) {
        m_curDevice->forceLowWarning();
    }
}
