#ifndef H265AMDAMFCONFIGURATIONDIALOG_H
#define H265AMDAMFCONFIGURATIONDIALOG_H

#include <QDialog>
#include "ConfigurationManager.h"

namespace Ui {
class h265AMDAMFConfigurationDialog;
}

class h265AMDAMFConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit h265AMDAMFConfigurationDialog(QWidget *parent = nullptr);
    ~h265AMDAMFConfigurationDialog();

private:
    Ui::h265AMDAMFConfigurationDialog *ui;

    /**
     * @brief entries_snapshot keeps an In-memory snapshot
     * of configuration values. This helps to detect
     * configuration changes and automatically res-start the host.
     */
    QHash<QString, QString> entries_snapshot;

    ConfigurationManager *config;
};

#endif // H265AMDAMFCONFIGURATIONDIALOG_H
