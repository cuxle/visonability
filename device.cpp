#include "device.h"
#include "frmdevicenode.h"
#include <QDebug>

Device::Device(int id, QString name, QObject *parent)
    : m_id(id),
      m_name(name),
      QObject(parent)
{
    m_l1 = new Light;
    m_l2 = new Light;
    m_l3 = new Light;
    m_l4 = new Light;
}

Device::~Device()
{
    delete m_l1;
    delete m_l2;
    delete m_l3;
    delete m_l4;
}

QString Device::name() const
{
    return m_name;
}

void Device::setName(const QString &name)
{
    m_name = name;
}

int Device::id() const
{
    return m_id;
}

void Device::setId(int id)
{
    m_id = id;
}

int Device::getCurMaxValueMeasured() const
{
    return m_curMaxValueMeasured;
}

int Device::getCurMinValueMeasured() const
{
    return m_curMinValueMeasured;
}

void Device::forceHighWarning()
{
    qDebug() << __func__;
}

void Device::forceLowWarning()
{
    qDebug() << __func__;
}

double Device::curVisiability() const
{
    return m_curVisiabilityValue;
}

double Device::maxVisiability() const
{
    return m_maxVisiabilityValue;
}

double Device::minVisiability() const
{
    return m_maxVisiabilityValue;
}

void Device::reboot()
{
    qDebug() << __func__;
}

double Device::mor() const
{
    return m_mor;
}

void Device::setMor(double mor)
{
    m_mor = mor;
    if (mor < m_curMinValueMeasured) {
        m_curMinValueMeasured = mor;
    } else if (mor > m_curMaxValueMeasured) {
        m_curMaxValueMeasured = mor;
    }

}

double Device::PT1() const
{
    return m_PT1;
}

void Device::setPT1(double PT1)
{
    m_PT1 = PT1;
}

double Device::PT2() const
{
    return m_PT2;
}

void Device::setPT2(double PT2)
{
    m_PT2 = PT2;
}

void Device::setLightCurrent(int index, double current)
{
    switch (index) {
    case 0:
        m_l1->setCurrent(current);
        break;
    case 1:
        m_l2->setCurrent(current);
        break;
    case 2:
        m_l3->setCurrent(current);
        break;
    case 3:
        m_l4->setCurrent(current);
        break;
    default:
        break;
    }
}

void Device::setLightState(int index, Light::State state)
{
    switch (index) {
    case 0:
        m_l1->setState(state);
        break;
    case 1:
        m_l2->setState(state);
        break;
    case 2:
        m_l3->setState(state);
        break;
    case 3:
        m_l4->setState(state);
        break;
    default:
        break;
    }
}

void Device::setLightSelfTest(int index)
{
    switch (index) {
    case 0:
        m_l1->startSelfTest();
        break;
    case 1:
        m_l2->startSelfTest();
        break;
    case 2:
        m_l3->startSelfTest();
        break;
    case 3:
        m_l4->startSelfTest();
        break;
    default:
        break;
    }
}

QString Device::macAddress() const
{
    return m_macAddress;
}

void Device::setMacAddress(const QString &macAddress)
{
    m_macAddress = macAddress;
}

frmDeviceNode *Device::getUiNode() const
{
    return m_uiNode;
}

void Device::setUiNode(frmDeviceNode *uiNode)
{
    m_uiNode = uiNode;
}

QHostAddress Device::getIpv4Address() const
{
    return m_ipv4Address;
}

void Device::setIpv4Address(const QHostAddress &ipv4Address)
{
    m_ipv4Address = ipv4Address;
}

void Device::updateWidgetInfo()
{
    m_uiNode->updateWidgetInfo();
}

double Device::getCurVisiabilityValue() const
{
    return m_curVisiabilityValue;
}

void Device::setCurVisiabilityValue(double curVisiabilityValue)
{
    m_curVisiabilityValue = curVisiabilityValue;
    if (m_curVisiabilityValue < m_curMinValueMeasured) {
        m_curMinValueMeasured = m_curVisiabilityValue;
    } else if (m_curVisiabilityValue > m_curMaxValueMeasured) {
        m_curMaxValueMeasured = m_curVisiabilityValue;
    }
}

void Device::deviceStateUpdate()
{
    if (m_curVisiabilityValue > m_PT1) {
        // 高报警
        m_deviceState = HighAlarm;
    } else if (m_curVisiabilityValue < m_PT2){
        // 低报警
        m_deviceState = LowAarm;
    } else {
        // 正常工作模式
        m_deviceState = Normal;
    }
}

Light::State Device::lightState(int index)
{
    switch (index) {
    case 0:
        return m_l1->state();
    case 1:
        return m_l2->state();
    case 2:
        return m_l3->state();
    case 3:
        return m_l4->state();
    default:
        break;
    }
    return Light::Invalid;
}

double Device::lightCurrent(int index)
{
    switch (index) {
    case 0:
        return m_l1->current();
    case 1:
        return m_l2->current();
    case 2:
        return m_l3->current();
    case 3:
        return m_l4->current();
    default:
        break;
    }
    return 0;
}

Light::Light(QObject *parent)
    :QObject(parent)
{
    
}

double Light::current() const
{
    return m_current;
}

void Light::setCurrent(double current)
{
    m_current = current;
}

void Light::startSelfTest()
{
    qDebug() << __func__;
}

Light::State Light::state() const
{
    return m_state;
}

void Light::setState(const State &state)
{
    m_state = state;
}

bool Light::isOk() const
{
    return m_isOk;
}
