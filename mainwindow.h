#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMqttClient>
#include <QMap>

class Device;
class QProcess;
class QLabel;
class QPushButton;
class QLineEdit;
class QCombox;

namespace Ui {
class MainWindow;
}

struct UiLightNode {
    QPushButton *currentSetBtn;
    QPushButton *stateSetBtn;
    QPushButton *selfTestBtn;
    QLineEdit *current;
    QCombox *cmbState; // 0亮，1灭，2闪
};

struct UiNode {
    bool isCheckedIn = false;
    QLabel *curValue;
    QLabel *maxValue;
    QLabel *minValue;
    QPushButton *advanceSettingBtn;
    UiLightNode light1;
    UiLightNode light2;
    UiLightNode light3;
    UiLightNode light4;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateLogStateChange();
    void brokerDisconnected();
    void messageReceived(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());

private:
    void initUi();
    void initIcon();
    void initMqttClient();
    void initMqttServer();
    void mqttClientSubscribe();
    void parseReceivedDataAndUpload(const QByteArray &message);

    QHash<QString, Device*> m_deviceTable; // string:mac, device:device
    QVariant parseJesonKey(const QJsonObject &obj, QString key);
    Ui::MainWindow *ui;

    const QString MQTT_SUBTOPIC = "UpLoadTopic"; //指令下载的
    const QString MQTT_PUBTOPIC = "DownLoadTopic";   //订阅上传的

//    QProcess *m_process;

    QMqttClient *m_client;
    const QString m_server = "192.168.1.129";
    const quint16 m_port = 1883;
};

#endif // MAINWINDOW_H
