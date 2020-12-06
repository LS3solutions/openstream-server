#include "h264amdamfconfigurationdialog.h"
#include "ui_h264amdamfconfigurationdialog.h"

h264AMDAMFConfigurationDialog::h264AMDAMFConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::h264AMDAMFConfigurationDialog)
{
    ui->setupUi(this);

    config = new ConfigurationManager("/assets/h264AMDAMF.conf");
    entries_snapshot = QHash<QString, QString>();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();
}

h264AMDAMFConfigurationDialog::~h264AMDAMFConfigurationDialog()
{
    delete ui;
}
