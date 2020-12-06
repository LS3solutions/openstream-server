#ifndef H264AMDAMFCONFIGURATIONDIALOG_H
#define H264AMDAMFCONFIGURATIONDIALOG_H

#include <QDialog>
#include "ConfigurationManager.h"

namespace Ui {
class h264AMDAMFConfigurationDialog;
}

class h264AMDAMFConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit h264AMDAMFConfigurationDialog(QWidget *parent = nullptr);
    ~h264AMDAMFConfigurationDialog();

private:
    Ui::h264AMDAMFConfigurationDialog *ui;

    /**
     * @brief entries_snapshot keeps an In-memory snapshot
     * of configuration values. This helps to detect
     * configuration changes and automatically res-start the host.
     */
    QHash<QString, QString> entries_snapshot;

    ConfigurationManager *config;
};

#endif // H264AMDAMFCONFIGURATIONDIALOG_H
