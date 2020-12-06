#include "h265amdamfconfigurationdialog.h"
#include "ui_h265amdamfconfigurationdialog.h"

h265AMDAMFConfigurationDialog::h265AMDAMFConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::h265AMDAMFConfigurationDialog)
{
    ui->setupUi(this);
    config = new ConfigurationManager("/assets/h265AMDAMF.conf");
    entries_snapshot = QHash<QString, QString>();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();
}

h265AMDAMFConfigurationDialog::~h265AMDAMFConfigurationDialog()
{
    delete ui;
}
