#include "headers/ConfigurationDialog.h"
#include <QCoreApplication>

ConfigurationDialog::ConfigurationDialog(QWidget *parent)
    : QDialog(parent)
{
    config = new ConfigurationManager();

    //TODO: refine this writing to disc
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();

    configInputForm = new QFormLayout;

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Save
                                 | QDialogButtonBox::Close);

    /*encoder*/
    encoderFieldGroupBox = new QGroupBox(this);
    encoderFieldRadioBtnHardware = new QRadioButton("Hardware", this);
    encoderFieldRadioBtnSoftware = new QRadioButton("Software", this);

    if(config->getKey(QString("encoder")) == "nvenc")
        encoderFieldRadioBtnHardware->setChecked(true);
    else
        encoderFieldRadioBtnSoftware->setChecked(true);

    encoderHBoxLayout = new QHBoxLayout(this);
    encoderHBoxLayout->addWidget(encoderFieldRadioBtnHardware);
    encoderHBoxLayout->addWidget( encoderFieldRadioBtnSoftware);
    encoderFieldGroupBox->setLayout(encoderHBoxLayout);

    configInputForm->addRow(ENCODER_LABEL, encoderFieldGroupBox);

    /*nr of threads*/
    minThreadsFieldLineEdit = new QLineEdit(this);
    minThreadsFieldLineEdit->setText(config->getKey(QString("min_threads")));

    configInputForm->addRow(MIN_THREADS_LABEL, minThreadsFieldLineEdit);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConfigurationDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConfigurationDialog::reject);
    configInputForm->addWidget(buttonBox);
    this->setWindowModality(Qt::WindowModality::ApplicationModal);
    this->setLayout(configInputForm);
}

void ConfigurationDialog::accept()
{
    updateNewConfiguration();
    QDialog::accept();
}

void ConfigurationDialog::reject()
{
    this->hide();
    QDialog::reject();
}

void ConfigurationDialog::editConfigurationClicked()
{
    this->show();
}

void ConfigurationDialog::updateNewConfiguration()
{
    qDebug() << "Update new configuration" << endl;
    QHash<QString, QString> entries = QHash<QString, QString>();

    /*Checks for encoder opt*/
    if(encoderFieldRadioBtnHardware->isChecked())
        config->setEntry("encoder", "nvenc");
    else
        config->setEntry("encoder", "software");

    /*Checks for nr of threads*/
    QString nrOfThreads = minThreadsFieldLineEdit->text();
    config->setEntry("min_threads", nrOfThreads);

    config->saveConfiguration();
}
