#include "advancesetting.h"
#include "ui_advancesetting.h"
#include "frmdevicenode.h"

AdvanceSetting::AdvanceSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdvanceSetting)
{
    ui->setupUi(this);
}

AdvanceSetting::~AdvanceSetting()
{
    delete ui;
}

void AdvanceSetting::setNode(frmDeviceNode *deviceNode)
{
    m_deviceNode = deviceNode;
}

void AdvanceSetting::on_pbAliasName_clicked()
{
    m_deviceNode->setDeviceName(ui->leAliasName->text());
}
