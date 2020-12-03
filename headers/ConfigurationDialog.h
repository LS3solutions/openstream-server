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
    void setGUIPriority();

signals:
    void configuration_changed();

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

    /********System process priority****/
    QRadioButton *syspriorityAboveNormalBtn;
    QRadioButton *syspriorityHighBtn;
    QRadioButton *syspriorityRealTimeBtn;
    QGroupBox *sysPriorityGroupBox;
    QHBoxLayout *sysPriorityHBoxLayout;
    QString SYS_PRIORITY_ABOVE_NORMAL_LABEL = QString("Above Normal");
    QString SYS_PRIORITY_HIGH_LABEL = QString("High");
    QString SYS_PRIORITY_REAL_TIME_LABEL = QString("Real Time");
    QString SYS_PRIORITY_OPT_LABEL = QString("Process Priority");
    QString SYS_PRIORITY_ABOVE_NORMAL = QString("1");
    QString SYS_PRIORITY_HIGH = QString("2");
    QString SYS_PRIORITY_REAL_TIME = QString("3");

    /********Software encoder speed****/
    QRadioButton *encswSpeedFastRadioBtn;
    QRadioButton *encswSpeedFasterRadioBtn;
    QRadioButton *encswSpeedUltraFastRadioBtn;
    QRadioButton *encswSpeedSuperFastRadioBtn;
    QGroupBox *encswFieldGroupBox;
    QHBoxLayout *encswHBoxLayout;
    QString ENCODER_SPEED_LABEL = QString("Encoding speed");
    QString ENCODER_SPEED_FAST = QString("fast");
    QString ENCODER_SPEED_FASTER = QString("faster");
    QString ENCODER_SPEED_ULTRAFAST = QString("ultrafast");
    QString ENCODER_SPEED_SUPERFAST = QString("superfast");

    /**********SW/HW encoder ************/
    QGroupBox *encoderFieldGroupBox;
    QRadioButton *encoderFieldRadioBtnNVENC;
    QRadioButton *encoderFieldRadioBtnAMF;
    QRadioButton *encoderFieldRadioBtnSoftware;
    QHBoxLayout *encoderHBoxLayout;
    QString ENCODER_NVENC = QString("nvenc");
    QString ENCODER_AMF = QString("amf");
    QString ENCODER_SW = QString("software");
    QString ENCODER_LABEL = QString("Encoder");

    /********codec****/
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
    QLineEdit *poolsFieldLineEdit;
    QString POOLS_LABEL = QString("Pools");

    /*******Encoder params****************/
    QLineEdit *x265vbvMaxRateFieldLineEdit;
    QString x265vbvMaxRate_LABEL = QString("vbv_maxrate");
    QLineEdit *x265vbvBufsizeFieldLineEdit;
    QString x265vbvBufsize_LABEL = QString("vbv_bufsize");
    QLineEdit *crfLineEdit;
    QString crf_LABEL = QString("CRF");
    QLineEdit *QPLineEdit;
    QString QP_LABEL = QString("QP");

    /*****FEC_PERCENTAGE****************/
    QLineEdit *fecPercentageFieldLineEdit;
    QString fecPercentage_LABEL = QString("fec percentage");


    QHash<QString, QString> entries_snapshot;
};

#endif // CONFIGURATIONDIALOG_H
