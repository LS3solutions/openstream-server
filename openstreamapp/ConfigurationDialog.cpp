#include "ConfigurationDialog.h"
#include <QCoreApplication>
#include "set_priority_class.h"
#include <QMessageBox>

ConfigurationDialog::ConfigurationDialog(QWidget *parent)
    : QDialog(parent)
{
    config = new ConfigurationManager("/assets/sunshine.conf");
    entries_snapshot = QHash<QString, QString>();

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
    encswSpeedSuperFastRadioBtn = new QRadioButton(ENCODER_SPEED_SUPERFAST.left(1).toUpper()+ENCODER_SPEED_SUPERFAST.mid(1), this);
    encswHBoxLayout = new QHBoxLayout(this);
    encswHBoxLayout->addWidget(encswSpeedFastRadioBtn);
    encswHBoxLayout->addWidget(encswSpeedFasterRadioBtn);
    encswHBoxLayout->addWidget(encswSpeedSuperFastRadioBtn);
    encswHBoxLayout->addWidget(encswSpeedUltraFastRadioBtn);

    if(config->getKey(QString("sw_preset")) == ENCODER_SPEED_FAST) {
        encswSpeedFastRadioBtn->setChecked(true);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_FAST);
    }
    else if(config->getKey(QString("sw_preset")) == ENCODER_SPEED_FASTER) {
        encswSpeedFasterRadioBtn->setChecked(true);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_FASTER);
    }
    else if (config->getKey(QString("sw_preset")) == ENCODER_SPEED_ULTRAFAST) {
        encswSpeedUltraFastRadioBtn->setChecked(true);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_ULTRAFAST);
    }
    else if(config->getKey(QString("sw_preset")) == ENCODER_SPEED_SUPERFAST) {
        encswSpeedSuperFastRadioBtn->setChecked(true);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_SUPERFAST);
    }
    encswFieldGroupBox = new QGroupBox(this);
    encswFieldGroupBox->setLayout(encswHBoxLayout);
    configInputForm->addRow(ENCODER_SPEED_LABEL, encswFieldGroupBox);

    /********codec****/
    swcodech264RadioBtn = new QRadioButton(SWCODEC_H264_LABEL, this);
    swcodech265RadioBtn = new QRadioButton(SWCODEC_H265_LABEL, this);
    swcodecHBoxLayout = new QHBoxLayout(this);
    swcodecHBoxLayout->addWidget(swcodech264RadioBtn);
    swcodecHBoxLayout->addWidget(swcodech265RadioBtn);

    if(config->getKey(QString("hevc_mode")) == SWCODEC_MODE_H264_VALUE) {
        swcodech264RadioBtn->setChecked(true);
        entries_snapshot.insert("hevc_mode", SWCODEC_MODE_H264_VALUE);
    }
    else if (config->getKey(QString("hevc_mode")) == SWCODEC_MODE_HEVC_VALUE) {
        swcodech265RadioBtn->setChecked(true);
        entries_snapshot.insert("hevc_mode", SWCODEC_MODE_HEVC_VALUE);
    }

    swcodecFieldGroupBox = new QGroupBox(this);
    swcodecFieldGroupBox->setLayout(swcodecHBoxLayout);
    configInputForm->addRow(SWCODEC_LABEL, swcodecFieldGroupBox);

    /********Software - NVEC encoders************/
    encoderFieldGroupBox = new QGroupBox(this);
    encoderFieldRadioBtnNVENC = new QRadioButton("nvenc", this);
    encoderFieldRadioBtnAMF = new QRadioButton("amf", this);
    encoderFieldRadioBtnSoftware = new QRadioButton("software", this);

    if(config->getKey(QString("encoder")) == "nvenc") {
         encoderFieldRadioBtnNVENC->setChecked(true);
         entries_snapshot.insert("encoder", ENCODER_NVENC);
    }
    else if (config->getKey(QString("encoder")) == "amf") {
        encoderFieldRadioBtnAMF->setChecked(true);
        entries_snapshot.insert("encoder", ENCODER_AMF);
    }
    else {
        encoderFieldRadioBtnSoftware->setChecked(true);
        entries_snapshot.insert("encoder", ENCODER_SW);
    }

    encoderHBoxLayout = new QHBoxLayout(this);
    encoderHBoxLayout->addWidget(encoderFieldRadioBtnNVENC);
    encoderHBoxLayout->addWidget(encoderFieldRadioBtnAMF);
    encoderHBoxLayout->addWidget( encoderFieldRadioBtnSoftware);
    encoderFieldGroupBox->setLayout(encoderHBoxLayout);

    configInputForm->addRow(ENCODER_LABEL, encoderFieldGroupBox);

    /********System process priority****/
    syspriorityAboveNormalBtn = new QRadioButton(SYS_PRIORITY_ABOVE_NORMAL_LABEL, this);
    syspriorityHighBtn = new QRadioButton(SYS_PRIORITY_HIGH_LABEL, this);
    syspriorityRealTimeBtn = new QRadioButton(SYS_PRIORITY_REAL_TIME_LABEL, this);
    sysPriorityHBoxLayout = new QHBoxLayout(this);
    sysPriorityHBoxLayout->addWidget(syspriorityAboveNormalBtn);
    sysPriorityHBoxLayout->addWidget(syspriorityHighBtn);
    sysPriorityHBoxLayout->addWidget(syspriorityRealTimeBtn);
    sysPriorityGroupBox = new QGroupBox(this);

    if(config->getKey(QString("system_priority")) == SYS_PRIORITY_ABOVE_NORMAL) {
        syspriorityAboveNormalBtn->setChecked(true);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_ABOVE_NORMAL);
    }
    else if(config->getKey(QString("system_priority")) == SYS_PRIORITY_HIGH) {
        syspriorityHighBtn->setChecked(true);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_HIGH);
    }
    else if(config->getKey(QString("system_priority")) == SYS_PRIORITY_REAL_TIME) {
        syspriorityRealTimeBtn->setChecked(true);
        entries_snapshot.insert("system_priority",  SYS_PRIORITY_REAL_TIME);
    }

    sysPriorityGroupBox->setLayout(sysPriorityHBoxLayout);
    configInputForm->addRow(SYS_PRIORITY_OPT_LABEL, sysPriorityGroupBox);

    /********Number of threads*********/
    minThreadsFieldLineEdit = new QLineEdit(this);
    minThreadsFieldLineEdit->setText(config->getKey(QString("min_threads")));
    entries_snapshot.insert("min_threads", config->getKey(QString("min_threads")));

    configInputForm->addRow(MIN_THREADS_LABEL, minThreadsFieldLineEdit);

    poolsFieldLineEdit = new QLineEdit(this);
    poolsFieldLineEdit->setText(config->getKey(QString("pools")));
    entries_snapshot.insert("pools", config->getKey(QString("pools")));

    configInputForm->addRow(POOLS_LABEL, poolsFieldLineEdit);

    /*******Encoder params****************/
    x265vbvMaxRateFieldLineEdit = new QLineEdit(this);
    x265vbvMaxRateFieldLineEdit->setText(config->getKey(QString("vbv_maxrate")));
    entries_snapshot.insert("vbv_maxrate", config->getKey(QString("vbv_maxrate")));
    configInputForm->addRow(x265vbvMaxRate_LABEL, x265vbvMaxRateFieldLineEdit);

    x265vbvBufsizeFieldLineEdit = new QLineEdit(this);
    x265vbvBufsizeFieldLineEdit->setText(config->getKey(QString("vbv_bufsize")));
    entries_snapshot.insert("vbv_bufsize", config->getKey(QString("vbv_bufsize")));
    configInputForm->addRow(x265vbvBufsize_LABEL, x265vbvBufsizeFieldLineEdit);

    crfLineEdit = new QLineEdit(this);
    crfLineEdit->setText(config->getKey(QString("crf")));
    entries_snapshot.insert("crf", config->getKey(QString("crf")));
    configInputForm->addRow(crf_LABEL, crfLineEdit);

    QPLineEdit = new QLineEdit(this);
    QPLineEdit->setText(config->getKey(QString("qp")));
    entries_snapshot.insert("qp", config->getKey(QString("qp")));
    configInputForm->addRow(QP_LABEL, QPLineEdit);

    /*******FEC_PERCENTAGE*************/
    fecPercentageFieldLineEdit = new QLineEdit(this);
    fecPercentageFieldLineEdit->setText(config->getKey(QString("fec_percentage")));
    entries_snapshot.insert("fec_percentage", config->getKey(QString("fec_percentage")));
    configInputForm->addRow(fecPercentage_LABEL, fecPercentageFieldLineEdit);

    /*******Connections********************/
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
    else if(encswSpeedSuperFastRadioBtn->isChecked())
        config->setEntry("sw_preset", ENCODER_SPEED_SUPERFAST);

    /********Encoder***********/
    /*Checks for encoder opt*/
    if(encoderFieldRadioBtnNVENC->isChecked())
        config->setEntry("encoder", ENCODER_NVENC);
    else if (encoderFieldRadioBtnAMF->isChecked()) {
        config->setEntry("encoder", ENCODER_AMF);
    }
    else
        config->setEntry("encoder", ENCODER_SW);

    /********Software codec****/
    if(swcodech264RadioBtn->isChecked())
        config->setEntry("hevc_mode", SWCODEC_MODE_H264_VALUE);
    else if(swcodech265RadioBtn->isChecked())
        config->setEntry("hevc_mode", SWCODEC_MODE_HEVC_VALUE);

    /*Checks for nr of threads*/
    QString nrOfThreads = minThreadsFieldLineEdit->text();
    config->setEntry("min_threads", nrOfThreads);
    QString nrPools = poolsFieldLineEdit->text();
    config->setEntry("pools", nrPools);

    /********System process priority****/
    if(syspriorityAboveNormalBtn->isChecked())
        config->setEntry("system_priority", SYS_PRIORITY_ABOVE_NORMAL);
    else if(syspriorityHighBtn->isChecked())
        config->setEntry("system_priority", SYS_PRIORITY_HIGH);
    else if(syspriorityRealTimeBtn->isChecked())
        config->setEntry("system_priority", SYS_PRIORITY_REAL_TIME);


    /***************ENCODEr params***************/
    QString x265vbvMaxRate = x265vbvMaxRateFieldLineEdit->text();
    config->setEntry("vbv_maxrate", x265vbvMaxRate);
    QString x265vbvBufsize = x265vbvBufsizeFieldLineEdit->text();
    config->setEntry("vbv_bufsize", x265vbvBufsize);
    QString crf = crfLineEdit->text();
    config->setEntry("crf", crf);
    QString qp = QPLineEdit->text();
    config->setEntry("qp", qp);

    /*****FEC_PERCENTAGE****************/
    QString fecPercentage = fecPercentageFieldLineEdit->text();
    config->setEntry("fec_percentage", fecPercentage);

    config->saveConfiguration();

    /*
    *   TODO: This short term working memory for detect config changes
    *   should be improved to something less manual, when the config
    *   options became unmanageable.
    */
    if( entries_snapshot.value("sw_preset") != config->getKey("sw_preset")
        || entries_snapshot.value("hevc_mode") != config->getKey("hevc_mode")
        || entries_snapshot.value("min_threads") != config->getKey("min_threads")
        || entries_snapshot.value("system_priority") != config->getKey("system_priority")
        || entries_snapshot.value("vbv_maxrate") != config->getKey("vbv_maxrate")
        || entries_snapshot.value("vbv_bufsize") != config->getKey("vbv_bufsize")
        || entries_snapshot.value("crf") != config->getKey("crf")
        || entries_snapshot.value("pools") != config->getKey("pools")
        || entries_snapshot.value("encoder") != config->getKey("encoder")
        || entries_snapshot.value("fec_percentage") != config->getKey("fec_percentage")
        || entries_snapshot.value("qp") != config->getKey("qp") )
    {
        entries_snapshot.insert("sw_preset",config->getKey("sw_preset"));
        entries_snapshot.insert("hevc_mode", config->getKey("hevc_mode"));
        entries_snapshot.insert("min_threads",config->getKey("min_threads"));
        entries_snapshot.insert("system_priority", config->getKey("system_priority"));
        entries_snapshot.insert("vbv_maxrate", config->getKey("vbv_maxrate"));
        entries_snapshot.insert("vbv_bufsize", config->getKey("vbv_bufsize"));
        entries_snapshot.insert("crf", config->getKey("crf"));
        entries_snapshot.insert("pools", config->getKey("pools"));
        entries_snapshot.insert("encoder", config->getKey("encoder"));
        entries_snapshot.insert("fec_percentage", config->getKey("fec_percentage"));
        entries_snapshot.insert("qp", config->getKey("qp"));
        emit configuration_changed();
    }

}

void ConfigurationDialog::setGUIPriority() {
    int priority = config->getKey("system_priority").toInt();
    set_host_process_priority_gui(priority);
}
