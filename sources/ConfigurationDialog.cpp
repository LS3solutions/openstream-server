#include "headers/ConfigurationDialog.h"
#include <QCoreApplication>
#include "headers/set_priority_class.h"

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

    /********Software encoder speed****/
    encswSpeedFastRadioBtn = new QRadioButton(ENCODER_SPEED_FAST.left(1).toUpper()+ENCODER_SPEED_FAST.mid(1),this);
    encswSpeedFasterRadioBtn = new QRadioButton(ENCODER_SPEED_FASTER.left(1).toUpper()+ENCODER_SPEED_FASTER.mid(1), this);
    encswSpeedUltraFastRadioBtn = new QRadioButton(ENCODER_SPEED_ULTRAFAST.left(1).toUpper()+ENCODER_SPEED_ULTRAFAST.mid(1), this);
    encswHBoxLayout = new QHBoxLayout(this);
    encswHBoxLayout->addWidget(encswSpeedFastRadioBtn);
    encswHBoxLayout->addWidget(encswSpeedFasterRadioBtn);
    encswHBoxLayout->addWidget(encswSpeedUltraFastRadioBtn);

    if(config->getKey(QString("sw_preset")) == ENCODER_SPEED_FAST)
        encswSpeedFastRadioBtn->setChecked(true);
    else if(config->getKey(QString("sw_preset")) == ENCODER_SPEED_FASTER)
        encswSpeedFasterRadioBtn->setChecked(true);
    else if (config->getKey(QString("sw_preset")) == ENCODER_SPEED_ULTRAFAST)
        encswSpeedUltraFastRadioBtn->setChecked(true);

    encswFieldGroupBox = new QGroupBox(this);
    encswFieldGroupBox->setLayout(encswHBoxLayout);
    configInputForm->addRow(ENCODER_SPEED_LABEL, encswFieldGroupBox);

    /********Software codec****/
    swcodech264RadioBtn = new QRadioButton(SWCODEC_H264_LABEL, this);
    swcodech265RadioBtn = new QRadioButton(SWCODEC_H265_LABEL, this);
    swcodecHBoxLayout = new QHBoxLayout(this);
    swcodecHBoxLayout->addWidget(swcodech264RadioBtn);
    swcodecHBoxLayout->addWidget(swcodech265RadioBtn);

    if(config->getKey(QString("hevc_mode")) == SWCODEC_MODE_H264_VALUE)
        swcodech264RadioBtn->setChecked(true);
    else if (config->getKey(QString("hevc_mode")) == SWCODEC_MODE_HEVC_VALUE)
        swcodech265RadioBtn->setChecked(true);

    swcodecFieldGroupBox = new QGroupBox(this);
    swcodecFieldGroupBox->setLayout(swcodecHBoxLayout);
    configInputForm->addRow(SWCODEC_LABEL, swcodecFieldGroupBox);

    /********System process priority****/
    syspriorityAboveNormalBtn = new QRadioButton(SYS_PRIORITY_ABOVE_NORMAL_LABEL, this);
    syspriorityHighBtn = new QRadioButton(SYS_PRIORITY_HIGH_LABEL, this);
    syspriorityRealTimeBtn = new QRadioButton(SYS_PRIORITY_REAL_TIME_LABEL, this);
    sysPriorityHBoxLayout = new QHBoxLayout(this);
    sysPriorityHBoxLayout->addWidget(syspriorityAboveNormalBtn);
    sysPriorityHBoxLayout->addWidget(syspriorityHighBtn);
    sysPriorityHBoxLayout->addWidget(syspriorityRealTimeBtn);
    sysPriorityGroupBox = new QGroupBox(this);

    if(config->getKey(QString("system_priority")) == SYS_PRIORITY_ABOVE_NORMAL)
        syspriorityAboveNormalBtn->setChecked(true);
    else if(config->getKey(QString("system_priority")) == SYS_PRIORITY_HIGH)
        syspriorityHighBtn->setChecked(true);
    else if(config->getKey(QString("system_priority")) == SYS_PRIORITY_REAL_TIME)
        syspriorityRealTimeBtn->setChecked(true);

    sysPriorityGroupBox->setLayout(sysPriorityHBoxLayout);
    configInputForm->addRow(SYS_PRIORITY_OPT_LABEL, sysPriorityGroupBox);

    /********Number of threads*********/
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

    /********Software encoder speed****/
    if(encswSpeedFastRadioBtn->isChecked())
        config->setEntry("sw_preset", ENCODER_SPEED_FAST);
    else if(encswSpeedFasterRadioBtn->isChecked())
        config->setEntry("sw_preset", ENCODER_SPEED_FASTER);
    else if(encswSpeedUltraFastRadioBtn->isChecked())
        config->setEntry("sw_preset", ENCODER_SPEED_ULTRAFAST);

    /********Software codec****/
    if(swcodech264RadioBtn->isChecked())
        config->setEntry("hevc_mode", SWCODEC_MODE_H264_VALUE);
    else if(swcodech265RadioBtn->isChecked())
        config->setEntry("hevc_mode", SWCODEC_MODE_HEVC_VALUE);

    /*Checks for nr of threads*/
    QString nrOfThreads = minThreadsFieldLineEdit->text();
    config->setEntry("min_threads", nrOfThreads);

    /********System process priority****/
    if(syspriorityAboveNormalBtn->isChecked())
        config->setEntry("system_priority", SYS_PRIORITY_ABOVE_NORMAL);
    else if(syspriorityHighBtn->isChecked())
        config->setEntry("system_priority", SYS_PRIORITY_HIGH);
    else if(syspriorityRealTimeBtn->isChecked())
        config->setEntry("system_priority", SYS_PRIORITY_REAL_TIME);

    config->saveConfiguration();

    /*Pseudo-Restart host after config changes*/
    emit configuration_changed();
}

void ConfigurationDialog::setGUIPriority() {
    int priority = config->getKey("system_priority").toInt();
    set_host_process_priority_gui(priority);
}
