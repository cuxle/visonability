#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QProcess>
#include "frmdevicenode.h"
#include "device.h"
#include "commoninfo.h"

#include <QDebug>
#include <QLibraryInfo>
#include <QDir>
#include <QMessageBox>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#include <QHostAddress>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_client(new QMqttClient)
{
    initUi();
    initIcon();
    initMqttClient();

    // 调不起来 原因未知
   // initMqttServer();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete m_client;
//    if (m_process->state() == QProcess::Running) {
//        m_process->terminate();
//        m_process->waitForFinished(3000);
//    }
}

void MainWindow::initUi()
{
    ui->setupUi(this);

    initIcon();

    ui->widget1->setDeviceName(QString("设备%1").arg(1));
    ui->widget2->setDeviceName(QString("设备%1").arg(2));
    ui->widget3->setDeviceName(QString("设备%1").arg(3));
    ui->widget4->setDeviceName(QString("设备%1").arg(4));
    ui->widget5->setDeviceName(QString("设备%1").arg(5));
    ui->widget6->setDeviceName(QString("设备%1").arg(6));
    ui->widget7->setDeviceName(QString("设备%1").arg(7));
    ui->widget8->setDeviceName(QString("设备%1").arg(8));
    ui->widget9->setDeviceName(QString("设备%1").arg(9));
    ui->widget10->setDeviceName(QString("设备%1").arg(10));
    ui->widget11->setDeviceName(QString("设备%1").arg(11));
    ui->widget12->setDeviceName(QString("设备%1").arg(12));
    ui->widget13->setDeviceName(QString("设备%1").arg(13));
    ui->widget14->setDeviceName(QString("设备%1").arg(14));
    ui->widget15->setDeviceName(QString("设备%1").arg(15));
    ui->widget16->setDeviceName(QString("设备%1").arg(16));

    ui->widget17->setDeviceName(QString("设备%1").arg(17));
    ui->widget18->setDeviceName(QString("设备%1").arg(18));
    ui->widget19->setDeviceName(QString("设备%1").arg(19));
    ui->widget20->setDeviceName(QString("设备%1").arg(20));
    ui->widget21->setDeviceName(QString("设备%1").arg(21));
    ui->widget22->setDeviceName(QString("设备%1").arg(22));
    ui->widget23->setDeviceName(QString("设备%1").arg(23));
    ui->widget24->setDeviceName(QString("设备%1").arg(24));
}

//void MainWindow2::initMqttServer()
//{
//    if (m_process->state() == QProcess::Running)
//        return;
//    // D:/Qt/Qt5.10.1/5.10.1/mingw53_32/bin\\assistant

//    QString app1 = "emqx";
//    QString app2 = "C:\\Windows/System32\\cmd";
//    QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
//    qDebug() << app;
//#if !defined(Q_OS_MAC)
//    app += QLatin1String("assistant");
//#else
//    app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
//#endif

//    qDebug() << app;
//    QStringList args;
//    args << QLatin1String("start");
//    m_process->start(app1, args);
//    if (!m_process->waitForStarted()) {
//        QMessageBox::critical(this, tr("Remote Control"),
//            tr("Could not start Qt Assistant from %1.").arg(app1));
//        return;
//    }

//}

void MainWindow::initIcon()
{
    //如果logo图片不存在则隐藏标签,否则显示logo图片
//    ui->labLogoBg->setAlignment(Qt::AlignCenter);
//    QPixmap pixLogo(QUIHelper::appPath() + "/" + App::LogoBg);
//    if (pixLogo.isNull()) {
//        //ui->labLogoBg->setVisible(false);
//        //设置图形字体作为logo
//        int size = 60;
//        ui->labLogoBg->setFixedWidth(size + 20);
//        //IconFont::Instance()->setIcon(ui->labLogoBg, 0xe6f2, size);
//    } else {
//        int size = 150;
//        ui->labLogoBg->setFixedWidth(size + 20);
//        pixLogo = pixLogo.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        ui->labLogoBg->setPixmap(pixLogo);
//    }

    //从图形字体库中设置图标
//    ui->btnViewDevice->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xea00, 20, 20, 20));
//    ui->btnViewMap->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xe695, 20, 20, 20));
//    ui->btnViewData->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xe60a, 20, 20, 20));
//    ui->btnViewPlot->setIcon(IconFont::Instance()->getPixmap(QUIConfig::TextColor, 0xe67b, 20, 20, 20));
//    ui->btnData->setIcon(IconHelper::Instance()->getPixmap(QUIConfig::TextColor, 0xf002, 20, 20, 20));
    //    ui->btnConfig->setIcon(IconHelper::Instance()->getPixmap(QUIConfig::TextColor, 0xf085, 20, 20, 20));
}

void MainWindow::initMqttClient()
{
    m_client->setHostname(m_server);
    m_client->setPort(m_port);

    connect(m_client, &QMqttClient::stateChanged, this, &MainWindow::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &MainWindow::brokerDisconnected);
    connect(m_client, &QMqttClient::messageReceived, this, &MainWindow::messageReceived);
    m_client->connectToHost();
}

void MainWindow::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    debug("message:" + message);
    debug("topic name:" + topic.name());
    if (topic.name() == MQTT_SUBTOPIC) {
        // parse json info
        parseReceivedDataAndUpload(message);
    }
}

QVariant MainWindow::parseJesonKey(const QJsonObject &obj, QString key)
{
    if (obj.contains(key)) {
        qInfo() << key << obj[key];
        return obj[key];
    } else {
        return QVariant();
    }
}

void MainWindow::parseReceivedDataAndUpload(const QByteArray &message)
{
    QJsonParseError parseJsonErr;
    QJsonDocument doc = QJsonDocument::fromJson(message, &parseJsonErr);
    if (parseJsonErr.error != QJsonParseError::NoError) {
        debug("pasrse jeson error:" + parseJsonErr.errorString());
        return;
    }
    if (doc.isNull()) {
        debug("JsonDocument is " + doc.isNull());
        return;
    }

    // if mac is in the map, get the device of the map, update the device info, and update widget
    // if mac is not in the map, this is a new device, alloc a new device

    QJsonObject obj = doc.object();

    QVariant mac = parseJesonKey(obj, "MAC");
    QString macStr = mac.toString();
    Device *d = nullptr;

    if (m_deviceTable.contains(macStr)) {
        d = m_deviceTable[macStr];
    } else {

        // assign a widget struct
        std::pair<frmDeviceNode *, int> node = frmDeviceNode::getNextAvailableNode();
        d = new Device(node.second, QString("device%1").arg(node.second));
        if (node.first == nullptr) return;
        node.first->setIsCheckedIn(true);
        node.first->setDevice(d);
        node.first->setEnabled(true);
        d->setUiNode(node.first);
        m_deviceTable.insert(macStr, d);
    }

    // fill all device value
    if (d == nullptr) return;

    QVariant vIp = parseJesonKey(obj, "IP");

    QHostAddress vAddress = QHostAddress(vIp.toString());
    if (!vAddress.isEqual(d->getIpv4Address())) {
        debug("ip address is not equal");
    }

    QVariant v = parseJesonKey(obj, "MOR");
    if (!v.isNull()) {
        d->setCurVisiabilityValue(v.toDouble());
    }

    // PT1 报警上限值
    // PT2 报警下限值
    v = parseJesonKey(obj, "PT1");
    if (!v.isNull()) {
        d->setPT1(v.toDouble());
    }

    v = parseJesonKey(obj, "PT2");
    if (!v.isNull()) {
        d->setPT2(v.toDouble());
    }

    v = parseJesonKey(obj, "LGT1");
    if (!v.isNull()) {
        d->setLightCurrent(0, v.toDouble());
    }

    v = parseJesonKey(obj, "LGT2");
    if (!v.isNull()) {
        d->setLightCurrent(1, v.toDouble());
    }

    v = parseJesonKey(obj, "LGT3");
    if (!v.isNull()) {
        d->setLightCurrent(2, v.toDouble());
    }

    v = parseJesonKey(obj, "LGT4");
    if (!v.isNull()) {
        d->setLightCurrent(3, v.toDouble());
    }

    // update interface
    d->updateWidgetInfo();

}

void MainWindow::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(": State Change")
                    + QString::number(m_client->state())
                    + QLatin1Char('\n');
    if(m_client->state() == QMqttClient::Connected) {
        debug(content);
    }

    if (m_client->state() == QMqttClient::Connected) {
        mqttClientSubscribe();
    }
//    ui->editLog->insertPlainText(content);
}

void MainWindow::brokerDisconnected()
{
    // init ui ?
    qInfo() << __func__ << "client state:" << m_client->state();
}

void MainWindow::mqttClientSubscribe()
{
    auto subscription1 = m_client->subscribe(MQTT_SUBTOPIC);
    if (!subscription1) {
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not subscribe. Is there a valid connection?"));
        return;
    } else {
        debug("subcription 1 successfully");
    }

    auto subscription2 = m_client->subscribe(MQTT_PUBTOPIC);
    if (!subscription2) {
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not subscribe. Is there a valid connection?"));
        return;
    } else {
        debug("subcription 2 successfully");
    }
}
