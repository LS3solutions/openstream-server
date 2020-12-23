#ifndef H264NVENCCONFIGURATIONDIALOG_H
#define H264NVENCCONFIGURATIONDIALOG_H

#include <QDialog>
#include "ConfigurationManager.h"
#include <QDesktopWidget>
#include <QCoreApplication>
#include "VideoEncoderEnum.h"

namespace Ui {
class h264NVENCConfigurationDialog;
}

class h264NVENCConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit h264NVENCConfigurationDialog(QWidget *parent = nullptr);
    ~h264NVENCConfigurationDialog();

signals:
    void configuration_changed(VideoEncoder);

private slots:
    void on_h264_nvenc_ok_button_clicked();
    void on_h264_nvenc_cancel_button_clicked();
    void on_h264_nvenc_restore_button_clicked();

private:
    Ui::h264NVENCConfigurationDialog *ui;

    /**
     * @brief entries_snapshot keeps an In-memory snapshot
     * of configuration values. This helps to detect
     * configuration changes and automatically res-start the host.
     */
    QHash<QString, QString> entries_snapshot;

    ConfigurationManager *config;

    /*Default values*/
    QString ENCODER_SPEED_LLHP_LABEL = QString("Low Latency High Performance (llhp)");
    QString ENCODER_SPEED_LLHQ_LABEL = QString("Low Latency High Quality (llhq)");
    QString ENCODER_SPEED_LLHP = QString("llhp");
    QString ENCODER_SPEED_LLHQ = QString("llhq");

    //Process Priority
    QString SYS_PRIORITY_ABOVE_NORMAL_LABEL = QString("Above Normal");
    QString SYS_PRIORITY_HIGH_LABEL = QString("High");
    QString SYS_PRIORITY_REAL_TIME_LABEL = QString("Real Time");
    QStringList SYS_PRIORITY_OP_LIST = QStringList() << SYS_PRIORITY_ABOVE_NORMAL_LABEL
                                                     << SYS_PRIORITY_HIGH_LABEL
                                                     << SYS_PRIORITY_REAL_TIME_LABEL;
    QString SYS_PRIORITY_ABOVE_NORMAL = QString("1");
    QString SYS_PRIORITY_HIGH = QString("2");
    QString SYS_PRIORITY_REAL_TIME = QString("3");

    //Frame Threads
    QString FRAME_THREADS_2 = QString("2");
    QString FRAME_THREADS_4 = QString("4");
    QString FRAME_THREADS_6 = QString("6");
    QString FRAME_THREADS_8 = QString("8");
    QStringList FRAME_THREADS_OPT_LIST = QStringList() << FRAME_THREADS_2
                                                       << FRAME_THREADS_4
                                                       << FRAME_THREADS_6
                                                       << FRAME_THREADS_8;

    //Pool Threads
    QString POOL_THREADS_2 = QString("2");
    QString POOL_THREADS_4 = QString("4");
    QString POOL_THREADS_6 = QString("6");
    QString POOL_THREADS_8 = QString("8");
    QStringList POOL_THREADS_OPT_LIST = QStringList() << POOL_THREADS_2
                                                      << POOL_THREADS_4
                                                      << POOL_THREADS_6
                                                      << POOL_THREADS_8;

    //Rate Control
    QString RATE_CONTROL_AUTO = QString("auto");
    QString RATE_CONTROL_CBR_LABEL =  QString("CBR low delay high quality");
    QString RATE_CONTROL_CONSTANT_QP_LABEL = QString("Constant QP mode");
    QString RATE_CONTROL_VBR_HIGH_QUALITY_LABEL = QString("VBR high quality");
    QStringList RATE_CONTROL_OPT_LIST = QStringList() << RATE_CONTROL_AUTO
                                                      << RATE_CONTROL_CBR_LABEL
                                                      << RATE_CONTROL_CONSTANT_QP_LABEL
                                                      << RATE_CONTROL_VBR_HIGH_QUALITY_LABEL;
    QString RATE_CONTROL_CBR = QString("cbr_ld_hq");
    QString RATE_CONTROL_CONSTANT_QP = QString("constqp");
    QString RATE_CONTROL_VBR_HIGH_QUALITY = QString("vbr_hq");

    //FEC Percentage
    QString FEC_10 = QString("10");
    QString FEC_20 = QString("20");
    QString FEC_30 = QString("30");
    QString FEC_40 = QString("40");
    QStringList FEC_OPT_LIST = QStringList() << FEC_10 << FEC_20 << FEC_30 << FEC_40;


    void setLoadedValues();
    void restoreDefaultsValues();

};

#endif // H264NVENCCONFIGURATIONDIALOG_H
