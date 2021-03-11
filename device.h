#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QMqttClient>
#include <QHostAddress>
#include "mainwindow.h"


class frmDeviceNode;

class Light : public QObject {
    Q_OBJECT
public:
    enum State {
        Off,
        On,
        Flash,
        Invalid
    };
    Light(QObject *parent = nullptr);
    double current() const;
    void setCurrent(double current);

    void startSelfTest();

    State state() const;
    void setState(const State &state);

    bool isOk() const;

private:
    double m_current;
    State m_state = Off;
    bool m_isOk = true;
};

class Device : public QObject
{
    Q_OBJECT
public:
    enum DeviceState {
        HighAlarm,
        Normal,
        LowAarm,
        Invalid
    };
    Device(int id, QString name, QObject *parent = nullptr);
    ~Device();

    QString name() const;
    void setName(const QString &name);

    int id() const;
    void setId(int id);

    int getCurMaxValueMeasured() const;

    int getCurMinValueMeasured() const;

    void forceHighWarning();

    void forceLowWarning();

    double curVisiability() const;

    double maxVisiability() const;

    double minVisiability() const;

    void reboot();

    double mor() const;
    void setMor(double mor);

    double PT1() const;
    void setPT1(double PT1);

    double PT2() const;
    void setPT2(double PT2);

    void setLightCurrent(int index, double current);
    void setLightState(int index, Light::State state);
    void setLightSelfTest(int index);

    double lightCurrent(int index);
    Light::State lightState(int index);

    QString macAddress() const;
    void setMacAddress(const QString &macAddress);
    frmDeviceNode *getUiNode() const;
    void setUiNode(frmDeviceNode *uiNode);

    QHostAddress getIpv4Address() const;
    void setIpv4Address(const QHostAddress &ipv4Address);

    void updateWidgetInfo();

    double getCurVisiabilityValue() const;
    void setCurVisiabilityValue(double curVisiabilityValue);

private:
    void deviceStateUpdate();

    QString m_name;
    int m_id;
    DeviceState m_deviceState = Invalid;
    double m_curMaxValueMeasured = 5;
    double m_curMinValueMeasured = 0;

    double m_curVisiabilityValue = 0;

    double m_maxVisiabilityValue = 100;
    double m_minVisiabilityValue = 1;

    double m_mor;
    double m_PT1;
    double m_PT2;

    QHostAddress m_ipv4Address;
    QString m_macAddress;

    frmDeviceNode *m_uiNode;

    Light *m_l1;
    Light *m_l2;
    Light *m_l3;
    Light *m_l4;

    QMqttClient *m_client;
};

#endif // DEVICE_H
