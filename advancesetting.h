#ifndef ADVANCESETTING_H
#define ADVANCESETTING_H

#include <QDialog>


namespace Ui {
class AdvanceSetting;
}

class frmDeviceNode;

class AdvanceSetting : public QDialog
{
    Q_OBJECT

public:
    explicit AdvanceSetting(QWidget *parent = 0);
    ~AdvanceSetting();
    void setNode(frmDeviceNode *deviceNode);
private slots:
    void on_pbAliasName_clicked();

private:
    Ui::AdvanceSetting *ui;
    frmDeviceNode *m_deviceNode;
};

#endif // ADVANCESETTING_H
