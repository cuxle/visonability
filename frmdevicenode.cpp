#include "frmdevicenode.h"
#include "ui_frmdevicenode.h"
#include "commoninfo.h"
#include "device.h"
#include <QDebug>

QVector<frmDeviceNode*> frmDeviceNode::vUiTable;

frmDeviceNode::frmDeviceNode(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::frmDeviceNode)
{
    ui->setupUi(this);
    this->initStyle();
    this->initForm();

    m_settings = new AdvanceSetting(this);
    m_settings->setNode(this);

    vUiTable.append(this);
    this->setEnabled(false);
}

frmDeviceNode::~frmDeviceNode()
{
    delete ui;
    delete m_settings;
}

std::pair<frmDeviceNode *, int> frmDeviceNode::getNextAvailableNode()
{
    std::pair<frmDeviceNode*, int> pair;
    for (int i = 0; i < vUiTable.size(); i++) {
        frmDeviceNode *node = vUiTable[i];
        if (!node->getIsCheckedIn()) {
            pair = std::make_pair(node, i);
            return pair;
        }
    }
    return pair;
}

bool frmDeviceNode::eventFilter(QObject *watched, QEvent *event)
{
//    if (event->type() == QEvent::MouseButtonDblClick) {
//        if (!online) {
//            //双击重连当前端口下的所有设备
//            QString portName = DBHelper::getPortName(deviceName);
//            DeviceServer::Instance()->readValue(portName, 255, true);
//        } else {
//            if (AppData::CurrentUserType.contains("管理员")) {
//                frmDeviceControl::Instance()->setPositionID(positionID);
//                frmDeviceControl::Instance()->show();
//            }
//        }
//    }

    return QWidget::eventFilter(watched, event);
}

Device *frmDeviceNode::getDevice() const
{
    return m_device;
}

void frmDeviceNode::setDevice(Device *deice)
{
    m_device = deice;
}

bool frmDeviceNode::getIsCheckedIn() const
{
    return isCheckedIn;
}

void frmDeviceNode::setIsCheckedIn(bool value)
{
    isCheckedIn = value;
}

void frmDeviceNode::updateWidgetInfo()
{
    debug(__func__);
    if (m_device == nullptr) return;

    double visibleValue = m_device->curVisiability();
    debug(visibleValue);
    QString visText = QString::number(visibleValue, 'f', 2) + "km";
    ui->lbVisiabilityValue->setText(visText);


    double pt1 = m_device->PT1();
    debug(pt1);
    QString visMaxText = QString::number(pt1, 'f', 2) + "km";
    ui->lbMaxValue->setText(visMaxText);

    double PT2 = m_device->getCurMinValueMeasured();
    debug(PT2);
    QString minVisText = QString::number(PT2, 'f', 2) + "km";
    ui->lbMinValue->setText(minVisText);

    // current
    double current = m_device->lightCurrent(0);
    ui->leL1Current->setText(QString::number(current, 'f', 2));
    current = m_device->lightCurrent(1);
    ui->leL2Current->setText(QString::number(current, 'f', 2));
    current = m_device->lightCurrent(2);
    ui->leL3Current->setText(QString::number(current, 'f', 2));
    current = m_device->lightCurrent(3);
    ui->leL4Current->setText(QString::number(current, 'f', 2));

    // state
    Light::State state;
    state = m_device->lightState(0);
    int index = getLightStateIndex(state);
    ui->cmbL1State->setCurrentIndex(index);

    state = m_device->lightState(1);
    index = getLightStateIndex(state);
    ui->cmbL2State->setCurrentIndex(index);

    state = m_device->lightState(2);
    index = getLightStateIndex(state);
    ui->cmbL3State->setCurrentIndex(index);

    state = m_device->lightState(3);
    index = getLightStateIndex(state);
    ui->cmbL4State->setCurrentIndex(index);
}

int frmDeviceNode::getLightStateIndex(const Light::State &state)
{
    int index = 0;
    if (state == Light::On) {
        index = 0;
    } else if (state == Light::Off) {
        index = 1;
    } else if (state == Light::Flash) {
        index = 2;
    } else {
        index = -1;
    }
    return index;
}

QString frmDeviceNode::getPositionID()
{
    return this->positionID;
}

QString frmDeviceNode::getDeviceName()
{
    return this->deviceName;
}

QString frmDeviceNode::getNodeName()
{
    return this->nodeName;
}

void frmDeviceNode::initStyle()
{
    //取消掉重合的边框使得看起来就一条边框
    qss = QString("QFrame#gboxDeviceTitle{border-bottom-left-radius:0px;border-bottom-right-radius:0px;}"
                  "QFrame#gboxDevicePanel{border-top-width:0px;border-top-left-radius:0px;border-top-right-radius:0px;}"
                  "QLabel:disabled{background:none;color:%1;}").arg("");
    this->setStyleSheet(qss);

//    QUIHelper::setLabStyle(ui->labNodeName, 1);
//    QUIHelper::setLabStyle(ui->labValue, 2);
//    QUIHelper::setLabStyle(ui->labNodeSign, 3);
}

void frmDeviceNode::initForm()
{
    QFont font;
//    //font.setPixelSize(QUIConfig::FontSize + 5);
//    ui->labNodeName->setFont(font);
//    ui->labValue->setFont(font);

//    //font.setPixelSize(QUIConfig::FontSize + 1);
//    ui->labNodeSign->setFont(font);

//    online = false;
//    alarm = false;
//    this->installEventFilter(this);
}

void frmDeviceNode::setPositionID(const QString &positionID)
{
//    this->positionID = positionID;
//    ui->labPositionID->setText("位号: " + positionID);
}

void frmDeviceNode::setDeviceName(const QString &deviceName)
{
    this->deviceName = deviceName;
    ui->lbNodeName->setText(deviceName);
}

void frmDeviceNode::setNodeName(const QString &nodeName)
{
    this->nodeName = nodeName;
    //ui->labNodeName->setText(nodeName);
}

void frmDeviceNode::setNodeType(const QString &nodeType)
{
//    this->nodeType = nodeType;
//    ui->labNodeType->setText("型号: " + nodeType);
}

void frmDeviceNode::setNodeSign(const QString &nodeSign)
{
//    this->nodeSign = nodeSign;
//    ui->labNodeSign->setText(nodeSign);
}

void frmDeviceNode::setValue(float value)
{
    this->value = value;

//    //有两个传感器 是开关量 数值是 1、2  显示时 用正常 和异常代替
//    if (nodeType == "SJ-0001" || nodeType == "JG-0001") {
//        ui->labValue->setText(value == 1 ? "正常" : "异常");
//    } else {
//        ui->labValue->setText(QString::number(value));
//    }
}

void frmDeviceNode::setOnline(bool online)
{
    this->online = online;
    this->setEnabled(online);
    //ui->labNodeName->setEnabled(online);
}

void frmDeviceNode::setAlarm(bool alarm)
{
    this->alarm = alarm;
    //QUIHelper::setLabStyle(ui->labValue, alarm ? 0 : 2);
}

void frmDeviceNode::setSelect(bool select)
{
    //选中特殊显示,比如边框加粗
    QString qss;
    if (select) {
        qss = QString("#gboxDeviceTitle,#gboxDevicePanel{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}"
                      "#labNodeType,#labPositionID{color:%3;}").arg("#1DB7BF").arg("#1DB7BF").arg("#FFFFFF");
    } else {
//        qss = QString("#gboxDeviceTitle,#gboxDevicePanel{background:qlineargradient(spread:pad,x1:0,y1:0,x2:0,y2:1,stop:0 %1,stop:1 %2);}"
//                      "#labNodeType,#labPositionID{color:%3;}").arg(QUIConfig::NormalColorStart).arg(QUIConfig::NormalColorEnd).arg(QUIConfig::TextColor);
    }

    this->setStyleSheet(this->qss + qss);
}

void frmDeviceNode::on_pbAdvanceSetting_clicked()
{
    m_settings->exec();
}

void frmDeviceNode::on_pbSetL1Current_clicked()
{
    bool ok = false;
    double current = ui->leL1Current->text().toDouble(&ok);
    if (!ok) return;
    if (m_device != nullptr) {
        m_device->setLightCurrent(0, current);
    }
}

Light::State frmDeviceNode::getSateFromCmbIndex(int index)
{
    Light::State state = Light::Invalid;
    if (index == 0) {
        state = Light::Off;
    } else if (index == 1) {
        state = Light::On;
    } else if (index == 2){
        state = Light::Flash;
    }
    return state;
}

void frmDeviceNode::on_pbSetL1State_clicked()
{
    // 0 灭
    // 1 亮
    // 2 闪
    int index = ui->cmbL1State->currentIndex();
    m_device->setLightState(0, getSateFromCmbIndex(index));
}

void frmDeviceNode::on_pbL1SelfTest_clicked()
{
    m_device->setLightSelfTest(0);
}

void frmDeviceNode::on_pbSetL2Current_clicked()
{
    bool ok = false;
    double current = ui->leL2Current->text().toDouble(&ok);
    if (!ok) return;
    if (m_device != nullptr) {
        m_device->setLightCurrent(1, current);
    }
}

void frmDeviceNode::on_pbSetL2State_clicked()
{
    int index = ui->cmbL2State->currentIndex();
    m_device->setLightState(1, getSateFromCmbIndex(index));
}

void frmDeviceNode::on_pbL2SelfTest_clicked()
{
    m_device->setLightSelfTest(1);
}

void frmDeviceNode::on_pbSetL3Current_clicked()
{
    bool ok = false;
    double current = ui->leL3Current->text().toDouble(&ok);
    if (!ok) return;
    if (m_device != nullptr) {
        m_device->setLightCurrent(2, current);
    }
}

void frmDeviceNode::on_pbSetL3State_clicked()
{
    int index = ui->cmbL3State->currentIndex();
    m_device->setLightState(2, getSateFromCmbIndex(index));
}

void frmDeviceNode::on_pbL3SelfTest_clicked()
{
    m_device->setLightSelfTest(2);
}

void frmDeviceNode::on_pbSetL4Current_clicked()
{
    bool ok = false;
    double current = ui->leL4Current->text().toDouble(&ok);
    if (!ok) return;
    if (m_device != nullptr) {
        m_device->setLightCurrent(3, current);
    }
}

void frmDeviceNode::on_pbSetL4State_clicked()
{
    int index = ui->cmbL4State->currentIndex();
    m_device->setLightState(3, getSateFromCmbIndex(index));
}

void frmDeviceNode::on_pbL4SelfTest_clicked()
{
    m_device->setLightSelfTest(3);
}
