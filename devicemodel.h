#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QMqttClient>
#include <QHash>

class Device;

class DeviceModel : public QAbstractListModel
{
public:
    explicit DeviceModel(QObject *parent = nullptr);

signals:

public slots:

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Device *device(int row);
    QList<Device*> m_devices;
private:
    void initDevices();
    void mqttClientSubscribe();
    int getAvailableIndex();
    void updateLogStateChange();
    void brokerDisconnected();
    void messageReceived(const QByteArray &message, const QMqttTopicName &topic);
    QVariant parseJesonKey(const QJsonObject &obj, QString key);
    void parseReceivedDataAndUpload(const QByteArray &message);
    QMqttClient *m_client;

    // QAbstractItemModel interface

    // QAbstractItemModel interface

    const QString m_server = "127.0.0.1";
    const quint16 m_port = 1883;
    QHash<quint32, Device *> m_devicesTable;

    //这里应该跟下位机方向相反？
    //const QString MQTT_SUBTOPIC = "DownLoadTopic"; //指令下载的
    //const QString MQTT_PUBTOPIC = "UpLoadTopic";   //订阅上传的

    const QString MQTT_SUBTOPIC = "UpLoadTopic"; //指令下载的
    const QString MQTT_PUBTOPIC = "DownLoadTopic";   //订阅上传的

public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
};

#endif // DEVICEMODEL_H
