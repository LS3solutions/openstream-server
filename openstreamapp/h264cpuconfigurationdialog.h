#ifndef H264CPUCONFIGURATIONDIALOG_H
#define H264CPUCONFIGURATIONDIALOG_H

#include <QDialog>
#include "ConfigurationManager.h"
#include "VideoEncoderEnum.h"
#include <QDesktopWidget>
#include <QCoreApplication>

namespace Ui {
class h264CPUConfigurationDialog;
}

class h264CPUConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit h264CPUConfigurationDialog(QWidget *parent = nullptr);
    ~h264CPUConfigurationDialog();

signals:
    void configuration_changed(VideoEncoder);

private slots:
    void on_h264_cpu_cancel_button_clicked();
    void on_h264_cpu_restore_button_clicked();
    void on_h264_cpu_ok_button_clicked();

private:
    Ui::h264CPUConfigurationDialog *ui;

    /**
     * @brief entries_snapshot keeps an In-memory snapshot
     * of configuration values. This helps to detect
     * configuration changes and automatically res-start the host.
     */
    QHash<QString, QString> entries_snapshot;

    ConfigurationManager *config;

    /*Default values*/
    QString ENCODER_SPEED_FAST = QString("fast");
    QString ENCODER_SPEED_FASTER = QString("faster");
    QString ENCODER_SPEED_SUPERFAST = QString("superfast");
    QString ENCODER_SPEED_ULTRAFAST = QString("ultrafast");
    QStringList ENCODER_SPEED_OPT_LIST = QStringList() << ENCODER_SPEED_FAST
                                                       << ENCODER_SPEED_FASTER
                                                       << ENCODER_SPEED_SUPERFAST
                                                       << ENCODER_SPEED_ULTRAFAST;

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

    //VBV-MAX-Rate
    QString VBV_MAX_RATE_10_LABEL = QString("10 Mbps");
    QString VBV_MAX_RATE_10 = QString("10000");
    QString VBV_MAX_RATE_20_LABEL = QString("20 Mbps");
    QString VBV_MAX_RATE_20 = QString("20000");
    QString VBV_MAX_RATE_30_LABEL = QString("30 Mbps");
    QString VBV_MAX_RATE_30 = QString("30000");
    QString VBV_MAX_RATE_40_LABEL = QString("40 Mbps");
    QString VBV_MAX_RATE_40 = QString("40000");
    QStringList VBV_MAX_RATE_LABEL_OPT_LIST = QStringList() << VBV_MAX_RATE_10_LABEL
                                                            << VBV_MAX_RATE_20_LABEL
                                                            << VBV_MAX_RATE_30_LABEL
                                                            << VBV_MAX_RATE_40_LABEL;

    //VBV_BUFSIZE
    QString VBV_BUFSIZE_1_LABEL = QString("1 Mbps");
    QString VBV_BUFSIZE_1 = QString("1000");
    QString VBV_BUFSIZE_5_LABEL = QString("5 Mbps");
    QString VBV_BUFSIZE_5 = QString("5000");
    QString VBV_BUFSIZE_10_LABEL = QString("10 Mbps");
    QString VBV_BUFSIZE_10 = QString("10000");
    QString VBV_BUFSIZE_15_LABEL = QString("15 Mbps");
    QString VBV_BUFSIZE_15 = QString("15000");
    QStringList VBV_BUFSIZE_LABEL_OPT_LIST = QStringList() << VBV_BUFSIZE_1_LABEL
                                                           << VBV_BUFSIZE_5_LABEL
                                                           << VBV_BUFSIZE_10_LABEL
                                                           << VBV_BUFSIZE_15_LABEL;
    //CRF
    QString CRF_3 = QString("3");
    QString CRF_6 = QString("6");
    QString CRF_9 = QString("9");
    QString CRF_11 = QString("11");
    QString CRF_14 = QString("14");
    QString CRF_17 = QString("17");
    QString CRF_20 = QString("20");
    QString CRF_23 = QString("23");
    QString CRF_25 = QString("25");
    QString CRF_28 = QString("28");
    QString CRF_31 = QString("31");
    QString CRF_34 = QString("34");
    QStringList CRF_OPT_LIST = QStringList() << CRF_3 << CRF_6 << CRF_9
                                             << CRF_11 << CRF_14 << CRF_17 << CRF_20
                                             << CRF_23 << CRF_25 << CRF_28 << CRF_31
                                             << CRF_34;

    //QP
    //Currently unused
    QString QP_15 = QString("15");
    QString QP_20 = QString("20");
    QString QP_25 = QString("25");
    QString QP_30 = QString("30");
    QStringList QP_OPT_LIST = QStringList() << QP_15 << QP_20 << QP_25 << QP_30;

    //FEC Percentage
    QString FEC_10 = QString("10");
    QString FEC_20 = QString("20");
    QString FEC_30 = QString("30");
    QString FEC_40 = QString("40");
    QStringList FEC_OPT_LIST = QStringList() << FEC_10 << FEC_20 << FEC_30 << FEC_40;

    void setLoadedValues();
    void restoreDefaultsValues();
};

#endif // H264CPUCONFIGURATIONDIALOG_H
