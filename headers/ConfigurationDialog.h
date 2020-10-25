#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QFormLayout>
#include <QWidget>
#include "ConfigurationManager.h"
#include <QDebug>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QRegExp>

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigurationDialog(QWidget*parent = nullptr);

public slots:
    void editConfigurationClicked();
    void accept() override;
    void reject() override;

private:
    ConfigurationManager *config;
    QFormLayout *configInputForm;
    void updateNewConfiguration();

    QDialogButtonBox *buttonBox;
    void cleaning();

    /********Software encoder speed****/
    QRadioButton *encswSpeedFastRadioBtn;
    QRadioButton *encswSpeedFasterRadioBtn;
    QRadioButton *encswSpeedUltraFastRadioBtn;
    QGroupBox *encswFieldGroupBox;
    QHBoxLayout *encswHBoxLayout;
    QString ENCODER_SPEED_LABEL = QString("Encoding speed");
    QString ENCODER_SPEED_FAST = QString("fast");
    QString ENCODER_SPEED_FASTER = QString("faster");
    QString ENCODER_SPEED_ULTRAFAST = QString("ultrafast");

    /********Software codec****/
    QRadioButton *swcodech264RadioBtn;
    QRadioButton *swcodech265RadioBtn;
    QGroupBox *swcodecFieldGroupBox;
    QHBoxLayout *swcodecHBoxLayout;
    QString SWCODEC_H264_LABEL = QString("h264");
    QString SWCODEC_H265_LABEL = QString("h265");
    QString SWCODEC_MODE_H264_VALUE = QString("1");
    QString SWCODEC_MODE_HEVC_VALUE = QString("2");
    QString SWCODEC_LABEL = QString("Video codec");

    /********Number of threads*********/
    QLineEdit *minThreadsFieldLineEdit;
    QString MIN_THREADS_LABEL = QString("Threads");
};

#endif // CONFIGURATIONDIALOG_H
