#ifndef H265AMDAMFCONFIGURATIONDIALOG_H
#define H265AMDAMFCONFIGURATIONDIALOG_H

#include <QDialog>
#include "ConfigurationManager.h"
#include <QDesktopWidget>
#include <QCoreApplication>
#include "VideoEncoderEnum.h"

namespace Ui {
class h265AMDAMFConfigurationDialog;
}

class h265AMDAMFConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit h265AMDAMFConfigurationDialog(QWidget *parent = nullptr);
    ~h265AMDAMFConfigurationDialog();

signals:
    void configuration_changed(VideoEncoder);

private slots:
    void on_h265_amd_amf_ok_button_clicked();

private slots:
    void on_h265_amd_amf_cancel_button_clicked();

private slots:
    void on_h265_amd_amf_restore_button_clicked();

private:
    Ui::h265AMDAMFConfigurationDialog *ui;

    /**
     * @brief entries_snapshot keeps an In-memory snapshot
     * of configuration values. This helps to detect
     * configuration changes and automatically res-start the host.
     */
    QHash<QString, QString> entries_snapshot;

    ConfigurationManager *config;

    /*Quality*/
    QString ENCODER_QUALITY_SPEED = QString("speed");
    QString ENCODER_QUALITY_BALANCED = QString("balanced");
    QString ENCODER_QUALITY_QUALITY = QString("quality");
    QStringList ENCODER_QUALITY_OPT_LIST = QStringList() << ENCODER_QUALITY_SPEED
                                                         << ENCODER_QUALITY_BALANCED
                                                         << ENCODER_QUALITY_QUALITY;

    /*Rate control*/
    QString ENCODER_RATE_CONTROL_CQP = QString("cqp");
    QString ENCODER_RATE_CONTROL_CBR = QString("cbr");
    QString ENCODER_RATE_CONTROL_VBR_PEAK = QString("vbr_peak");
    QString ENCODER_RATE_CONTROL_VBV_LATENCY = QString("vbr_latency");
    QStringList ENCODER_RATE_CONTROL_OPT_LIST = QStringList() << ENCODER_RATE_CONTROL_CQP
                                                              << ENCODER_RATE_CONTROL_CBR
                                                              << ENCODER_RATE_CONTROL_VBR_PEAK
                                                              << ENCODER_RATE_CONTROL_VBV_LATENCY;


    QString ENCODER_MAXRATE_5_LABEL = QString("5 Mbps");
    QString ENCODER_MAXRATE_5 = QString("5000000");
    QString ENCODER_MAXRATE_10_LABEL = QString("10 Mbps");
    QString ENCODER_MAXRATE_10 = QString("10000000");
    QString ENCODER_MAXRATE_15_LABEL = QString("15 Mbps");
    QString ENCODER_MAXRATE_15 = QString("15000000");
    QString ENCODER_MAXRATE_20_LABEL = QString("20 Mbps");
    QString ENCODER_MAXRATE_20 = QString("20000000");
    QString ENCODER_MAXRATE_25_LABEL = QString("25 Mbps");
    QString ENCODER_MAXRATE_25 = QString("25000000");
    QString ENCODER_MAXRATE_30_LABEL = QString("30 Mbps");
    QString ENCODER_MAXRATE_30 = QString("30000000");
    QString ENCODER_MAXRATE_35_LABEL = QString("35 Mbps");
    QString ENCODER_MAXRATE_35 = QString("35000000");
    QString ENCODER_MAXRATE_40_LABEL = QString("40 Mbps");
    QString ENCODER_MAXRATE_40 = QString("40000000");
    QString ENCODER_MAXRATE_45_LABEL = QString("45 Mbps");
    QString ENCODER_MAXRATE_45 = QString("45000000");
    QString ENCODER_MAXRATE_50_LABEL = QString("50 Mbps");
    QString ENCODER_MAXRATE_50 = QString("50000000");
    QString ENCODER_MAXRATE_55_LABEL = QString("55 Mbps");
    QString ENCODER_MAXRATE_55 = QString("55000000");
    QString ENCODER_MAXRATE_60_LABEL = QString("60 Mbps");
    QString ENCODER_MAXRATE_60 = QString("60000000");
    QString ENCODER_MAXRATE_65_LABEL = QString("65 Mbps");
    QString ENCODER_MAXRATE_65 = QString("65000000");
    QString ENCODER_MAXRATE_70_LABEL = QString("70 Mbps");
    QString ENCODER_MAXRATE_70 = QString("70000000");
    QString ENCODER_MAXRATE_75_LABEL = QString("75 Mbps");
    QString ENCODER_MAXRATE_75 = QString("75000000");
    QString ENCODER_MAXRATE_80_LABEL = QString("80 Mbps");
    QString ENCODER_MAXRATE_80 = QString("80000000");
    QStringList ENCODER_MAXRATE_OPT_LIST = QStringList() << ENCODER_MAXRATE_5_LABEL
                                                         << ENCODER_MAXRATE_10_LABEL
                                                         << ENCODER_MAXRATE_15_LABEL
                                                         << ENCODER_MAXRATE_20_LABEL
                                                         << ENCODER_MAXRATE_25_LABEL
                                                         << ENCODER_MAXRATE_30_LABEL
                                                         << ENCODER_MAXRATE_35_LABEL
                                                         << ENCODER_MAXRATE_40_LABEL
                                                         << ENCODER_MAXRATE_45_LABEL
                                                         << ENCODER_MAXRATE_50_LABEL
                                                         << ENCODER_MAXRATE_55_LABEL
                                                         << ENCODER_MAXRATE_60_LABEL
                                                         << ENCODER_MAXRATE_65_LABEL
                                                         << ENCODER_MAXRATE_70_LABEL
                                                         << ENCODER_MAXRATE_75_LABEL
                                                         << ENCODER_MAXRATE_80_LABEL;

    void setLoadedValues();
    void restoreDefaultsValues();
};

#endif // H265AMDAMFCONFIGURATIONDIALOG_H
