#include "devicemodel.h"
#include "device.h"

#include <QDateTime>
#include <QHostAddress>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

DeviceModel::DeviceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    //initDevices(); // this is for test

    // individual interface for mqtt setting ?
    m_client = new QMqttClient;
    m_client->setHostname(m_server);
    m_client->setPort(m_port);

    connect(m_client, &QMqttClient::stateChanged, this, &DeviceModel::updateLogStateChange);
    connect(m_client, &QMqttClient::disconnected, this, &DeviceModel::brokerDisconnected);
    m_client->connectToHost();    
}

void DeviceModel::mqttClientSubscribe()
{
    auto subscription1 = m_client->subscribe(MQTT_SUBTOPIC);
    if (!subscription1) {
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not subscribe. Is there a valid connection?"));
        return;
    } else {
        qInfo() << "subcription 1 successfully";
    }

    auto subscription2 = m_client->subscribe(MQTT_PUBTOPIC);
    if (!subscription2) {
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not subscribe. Is there a valid connection?"));
        return;
    } else {
        qInfo() << "subcription 2 successfully";
    }
}

void DeviceModel::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" Received Topic: ")
                + topic.name()
                + QLatin1String(" Message: ")
                + message
                + QLatin1Char('\n');
    // parse json file
    // when a new device come in, create a new device
    // when an existed device come in, update its settings
    if (topic == MQTT_SUBTOPIC) {
        parseReceivedDataAndUpload(message);
    }
}

void DeviceModel::parseReceivedDataAndUpload(const QByteArray &message)
{
    QJsonParseError parseJsonErr;
    QJsonDocument doc = QJsonDocument::fromJson(message, &parseJsonErr);
    if (parseJsonErr.error != QJsonParseError::NoError) {
        qInfo() << "pasrse jeson error:" << parseJsonErr.errorString();
        return;
    }
    if (doc.isNull()) {
        qInfo() << "JsonDocument is " << doc.isNull();
        return;
    }

    QJsonObject obj = doc.object();
    QVariant vIp = parseJesonKey(obj, "IP");

    QHostAddress vAddress = QHostAddress(vIp.toString());
    quint32 decAddr = vAddress.toIPv4Address();
    if (decAddr != 0) {
        Device *d = nullptr;
        // no existed in hash, create a new one
        if (!m_devicesTable.contains(decAddr)) {
            int index = getAvailableIndex();
            d = new Device(index, QString("dev%1").arg(index));
            //d->setIpv4Address(vAddress);

            m_devices.append(d);
            m_devicesTable.insert(decAddr, d);
        } else {
            d = m_devicesTable[decAddr];
        }

//        QVariant v = parseJesonKey(obj, "MAC");
//        if (!v.isNull()) {
//            d->setMacAddress(v.toString());
//        }

//        v = parseJesonKey(obj, "MOR");
//        if (!v.isNull()) {
//            d->setMor(v.toDouble());
//        }

//        v = parseJesonKey(obj, "PT1");
//        if (!v.isNull()) {
//            d->setPT1(v.toDouble());
//        }

//        v = parseJesonKey(obj, "PT2");
//        if (!v.isNull()) {
//            d->setPT2(v.toDouble());
//        }

//        v = parseJesonKey(obj, "LGT1");
//        if (!v.isNull()) {
//            Light *l1 = d->l1();
//            l1->setCurrent(v.toDouble());
//        }

//        v = parseJesonKey(obj, "LGT2");
//        if (!v.isNull()) {
//            Light *l2 = d->l2();
//            l2->setCurrent(v.toDouble());
//        }

//        v = parseJesonKey(obj, "LGT3");
//        if (!v.isNull()) {
//            Light *l3 = d->l3();
//            l3->setCurrent(v.toDouble());
//        }

//        v = parseJesonKey(obj, "LGT4");
//        if (!v.isNull()) {
//            Light *l4 = d->l4();
//            l4->setCurrent(v.toDouble());
//        }
    }
    // update interface
}

int DeviceModel::getAvailableIndex()
{
    return 1;
}

QVariant DeviceModel::parseJesonKey(const QJsonObject &obj, QString key)
{
    if (obj.contains(key)) {
        qInfo() << key << obj[key];
        return obj[key];
    } else {
        return QVariant();
    }
}

void DeviceModel::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(": State Change")
                    + QString::number(m_client->state())
                    + QLatin1Char('\n');
    qInfo() << content;
    if (m_client->state() == QMqttClient::Connected) {
        mqttClientSubscribe();
    }
//    ui->editLog->insertPlainText(content);
}

void DeviceModel::brokerDisconnected()
{
//    ui->lineEditHost->setEnabled(true);
//    ui->spinBoxPort->setEnabled(true);
//    ui->buttonConnect->setText(tr("Connect"));
    qInfo() << "client state:" << m_client->state();
}

void DeviceModel::initDevices()
{
    QString ipBase = "192.168.1.100";
    QHostAddress address(ipBase);
    quint32 decIP = address.toIPv4Address();
    for (int i = 0; i < 20; i++) {
        Device *d = new Device(i, QString("dev%1").arg(i));
        QHostAddress addr(decIP + i);
        //d->setIpv4Address(addr);
        m_devices.append(d);
    }
}

QModelIndex DeviceModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    //1. parent is inValid
    return createIndex(row, column, m_devices[row]);
}

int DeviceModel::rowCount(const QModelIndex &parent) const
{
    return m_devices.size();
}

QVariant DeviceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (index.column() == 0) {
        return m_devices.at(index.row())->name();
    }

    return QVariant();
}

Device *DeviceModel::device(int row)
{
    return m_devices[row];
}
