#ifndef FRMDEVICENODE_H
#define FRMDEVICENODE_H

#include <QWidget>
#include <QVector>

#include "panelitem.h"
#include "advancesetting.h"
#include "device.h"

namespace Ui
{
    class frmDeviceNode;
}

class frmDeviceNode : public QWidget
{
    Q_OBJECT

public:
    explicit frmDeviceNode(QWidget *parent = 0);
    ~frmDeviceNode();

    static std::pair<frmDeviceNode *, int> getNextAvailableNode();
    static QVector<frmDeviceNode*> vUiTable;

    void setDeviceName();
    QString getPositionID();
    QString getDeviceName();
    QString getNodeName();

    bool getIsCheckedIn() const;
    void setIsCheckedIn(bool value);

    void updateWidgetInfo();

    Device *getDevice() const;
    void setDevice(Device *deice);

public slots:
    void initStyle();
    void initForm();
    void setPositionID(const QString &positionID);
    void setDeviceName(const QString &deviceName);
    void setNodeName(const QString &nodeName);
    void setNodeType(const QString &nodeType);
    void setNodeSign(const QString &nodeSign);
    void setValue(float value);

    void setOnline(bool online);
    void setAlarm(bool alarm);
    void setSelect(bool select);

private slots:
    void on_pbAdvanceSetting_clicked();

    void on_pbSetL1Current_clicked();

    void on_pbSetL1State_clicked();

    void on_pbL1SelfTest_clicked();

    void on_pbSetL2Current_clicked();

    void on_pbSetL2State_clicked();

    void on_pbL2SelfTest_clicked();

    void on_pbSetL3Current_clicked();

    void on_pbSetL3State_clicked();

    void on_pbL3SelfTest_clicked();

    void on_pbSetL4Current_clicked();

    void on_pbSetL4State_clicked();

    void on_pbL4SelfTest_clicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    int getLightStateIndex(const Light::State &state);
    Light::State getSateFromCmbIndex(int index);
    Ui::frmDeviceNode *ui;
    AdvanceSetting *m_settings;
    QString positionID;
    QString deviceName;
    QString nodeName;
    QString nodeType;
    QString nodeSign;
    float value;

    bool online;
    bool alarm;
    QString qss;
    bool isCheckedIn;
    Device *m_device;
};

#endif // FRMDEVICENODE_H
