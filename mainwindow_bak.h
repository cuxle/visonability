#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMqttClient>

class Device;
class DeviceModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_leDeviceName_editingFinished();

    void on_listView_clicked(const QModelIndex &index);

    void on_pbForceHighWarn_clicked();

    void on_pbForceLowWarn_clicked();

    void on_pbReboot_clicked();

private:
    Ui::MainWindow *ui;
    void initDevices();
    void fillDeviceInfo(Device *device);
    void updateLogStateChange();

    Device *m_curDevice;

    DeviceModel *m_model;
};

#endif // MAINWINDOW_H
