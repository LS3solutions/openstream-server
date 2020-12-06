#include "h264nvencconfigurationdialog.h"
#include "ui_h264nvencconfigurationdialog.h"


h264NVENCConfigurationDialog::h264NVENCConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::h264NVENCConfigurationDialog)
{
    ui->setupUi(this);

    /**
     * This Code deals with the Windows Size scaling
     */
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);

    /**
     * Ensures labels equal size
     */
    int maxWidth = 0;
    for (QLabel* label : findChildren<QLabel*>()) {
        if (maxWidth < label->width())
            maxWidth = label->width();
    }
    for (QLabel* label : findChildren<QLabel*>()) {
        if( label->objectName() != "h264_nvenc_title_label")
        label->setFixedWidth(maxWidth);
    }

    /**
      * Ensure buttons equal size
      */
    maxWidth = 0;
    for (QPushButton* btn : findChildren<QPushButton*>()) {
        if (maxWidth < btn->width())
            maxWidth = btn->width();
    }
    for (QPushButton* btn : findChildren<QPushButton*>()) {
        btn->setFixedWidth(maxWidth);
    }

    config = new ConfigurationManager("/assets/h264NVENC.conf");
    entries_snapshot = QHash<QString, QString>();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();

    /*Initialize UI Elements values*/
    ui->h264_nvenc_encoding_speed_combobox->addItem(ENCODER_SPEED_LLHP_LABEL);
    ui->h264_nvenc_encoding_speed_combobox->addItem(ENCODER_SPEED_LLHQ_LABEL);

    ui->h264_nvenc_process_priority_combobox->addItems(SYS_PRIORITY_OP_LIST);

    ui->h264_nvenc_frame_threads_combobox->addItems(FRAME_THREADS_OPT_LIST);
    ui->h264_nvenc_pool_threads_combobox->addItems(POOL_THREADS_OPT_LIST);

    ui->h264_nvenc_rate_control_combobox->addItems(RATE_CONTROL_OPT_LIST);

    ui->h264_nvenc_fec_percentage_combobox->addItems(FEC_OPT_LIST);

    setLoadedValues();
}

h264NVENCConfigurationDialog::~h264NVENCConfigurationDialog()
{
    delete ui;
}

void h264NVENCConfigurationDialog::setLoadedValues() {
    if(config->getKey("nv_preset") == ENCODER_SPEED_LLHP) {
        ui->h264_nvenc_encoding_speed_combobox->setCurrentIndex(0);
        entries_snapshot.insert("nv_preset", ENCODER_SPEED_LLHP);
    }
    else if(config->getKey("nv_preset") == ENCODER_SPEED_LLHQ) {
        ui->h264_nvenc_encoding_speed_combobox->setCurrentIndex(1);
        entries_snapshot.insert("nv_preset", ENCODER_SPEED_LLHQ);
    }

    /*System Priority*/
    /*Compare against the configuration values (1,2,3)*/
    if(config->getKey("system_priority") == SYS_PRIORITY_ABOVE_NORMAL) {
       ui->h264_nvenc_process_priority_combobox->setCurrentIndex(0);
       entries_snapshot.insert("system_priority", SYS_PRIORITY_ABOVE_NORMAL);
    }
    else if(config->getKey("system_priority") == SYS_PRIORITY_HIGH) {
        ui->h264_nvenc_process_priority_combobox->setCurrentIndex(1);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_HIGH);
    }
    else if(config->getKey("system_priority") == SYS_PRIORITY_REAL_TIME) {
        ui->h264_nvenc_process_priority_combobox->setCurrentIndex(2);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_REAL_TIME);
    }

    /*Frame threads*/
    if(config->getKey("min_threads") == FRAME_THREADS_2) {
        ui->h264_nvenc_frame_threads_combobox->setCurrentIndex(0);
        entries_snapshot.insert("min_threads", FRAME_THREADS_2);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_4) {
        ui->h264_nvenc_frame_threads_combobox->setCurrentIndex(1);
        entries_snapshot.insert("min_threads", FRAME_THREADS_4);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_6) {
        ui->h264_nvenc_frame_threads_combobox->setCurrentIndex(2);
        entries_snapshot.insert("min_threads", FRAME_THREADS_6);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_8) {
        ui->h264_nvenc_frame_threads_combobox->setCurrentIndex(3);
        entries_snapshot.insert("min_threads", FRAME_THREADS_8);
    }

    /*Pool threads*/
    if(config->getKey("pools") == POOL_THREADS_2) {
        ui->h264_nvenc_pool_threads_combobox->setCurrentIndex(0);
        entries_snapshot.insert("pools", POOL_THREADS_2);
    }
    else if(config->getKey("pools") == POOL_THREADS_4) {
        ui->h264_nvenc_pool_threads_combobox->setCurrentIndex(1);
        entries_snapshot.insert("pools", POOL_THREADS_4);
    }
    else if(config->getKey("pools") == POOL_THREADS_6) {
        ui->h264_nvenc_pool_threads_combobox->setCurrentIndex(2);
        entries_snapshot.insert("pools", POOL_THREADS_6);
    }
    else if(config->getKey("pools") == POOL_THREADS_8) {
        ui->h264_nvenc_pool_threads_combobox->setCurrentIndex(3);
        entries_snapshot.insert("pools", POOL_THREADS_8);
    }

    /*Rate control*/
    if(config->getKey("nv_rc") == RATE_CONTROL_AUTO) {
        ui->h264_nvenc_rate_control_combobox->setCurrentIndex(0);
        entries_snapshot.insert("nv_rc", RATE_CONTROL_AUTO);
    }
    else if(config->getKey("nv_rc") == RATE_CONTROL_CBR) {
        ui->h264_nvenc_rate_control_combobox->setCurrentIndex(1);
        entries_snapshot.insert("nv_rc", RATE_CONTROL_CBR);
    }
    else if(config->getKey("nv_rc") == RATE_CONTROL_CONSTANT_QP) {
        ui->h264_nvenc_rate_control_combobox->setCurrentIndex(2);
        entries_snapshot.insert("nv_rc", RATE_CONTROL_CONSTANT_QP);
    }
    else if(config->getKey("nv_rc") == RATE_CONTROL_VBR_HIGH_QUALITY) {
        ui->h264_nvenc_rate_control_combobox->setCurrentIndex(3);
        entries_snapshot.insert("nv_rc", RATE_CONTROL_VBR_HIGH_QUALITY);
    }

    /*FEC*/
    if(config->getKey("fec_percentage") == FEC_10) {
        ui->h264_nvenc_fec_percentage_combobox->setCurrentIndex(0);
        entries_snapshot.insert("fec_percentage", FEC_10);
    }
    else if(config->getKey("fec_percentage") == FEC_20) {
        ui->h264_nvenc_fec_percentage_combobox->setCurrentIndex(1);
        entries_snapshot.insert("fec_percentage", FEC_20);
    }
    else if(config->getKey("fec_percentage") == FEC_30) {
        ui->h264_nvenc_fec_percentage_combobox->setCurrentIndex(2);
        entries_snapshot.insert("fec_percentage", FEC_30);
    }
    else if(config->getKey("fec_percentage") == FEC_40) {
        ui->h264_nvenc_fec_percentage_combobox->setCurrentIndex(3);
        entries_snapshot.insert("fec_percentage", FEC_40);
    }
}

void h264NVENCConfigurationDialog::restoreDefaultsValues(){
    config->restoreDefaultConfiguration("/h264NVENC.conf");
}



void h264NVENCConfigurationDialog::on_h264_nvenc_restore_button_clicked()
{
    restoreDefaultsValues();
    config->reloadInMemoryValues();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();
    setLoadedValues();
    emit configuration_changed(h264NVENC);
}

void h264NVENCConfigurationDialog::on_h264_nvenc_cancel_button_clicked()
{
    this->hide();
    setLoadedValues();
}

void h264NVENCConfigurationDialog::on_h264_nvenc_ok_button_clicked()
{
    //encoder preset
    QString selected_encoder_preset_label = ui->h264_nvenc_encoding_speed_combobox->currentText();
    if(selected_encoder_preset_label == ENCODER_SPEED_LLHP_LABEL) {
        config->setEntry("nv_preset", ENCODER_SPEED_LLHP);
    }
    else if(selected_encoder_preset_label == ENCODER_SPEED_LLHQ_LABEL) {
        config->setEntry("nv_preset", ENCODER_SPEED_LLHQ);
    }

    /*System priority*/
    QString selected_system_priority_label = ui->h264_nvenc_process_priority_combobox->currentText();
    if(selected_system_priority_label == SYS_PRIORITY_ABOVE_NORMAL_LABEL) {
        config->setEntry("system_priority", SYS_PRIORITY_ABOVE_NORMAL);
    }
    else if (selected_system_priority_label == SYS_PRIORITY_HIGH_LABEL) {
        config->setEntry("system_priority", SYS_PRIORITY_HIGH);
    }
    else if(selected_system_priority_label == SYS_PRIORITY_REAL_TIME_LABEL) {
        config->setEntry("system_priority", SYS_PRIORITY_REAL_TIME);
    }
    /*Frame threads*/
    QString selected_frame_threads = ui->h264_nvenc_frame_threads_combobox->currentText();
    config->setEntry("min_threads", selected_frame_threads);
    /*Pool threads*/
    QString selected_pool_threads = ui->h264_nvenc_pool_threads_combobox->currentText();
    config->setEntry("pools", selected_pool_threads);

    /*rate control*/
    QString selected_rate_control_label = ui->h264_nvenc_rate_control_combobox->currentText();
    if(selected_rate_control_label == RATE_CONTROL_AUTO) {
        config->setEntry("nv_rc", RATE_CONTROL_AUTO);
    }
    else if(selected_rate_control_label == RATE_CONTROL_CBR_LABEL) {
        config->setEntry("nv_rc", RATE_CONTROL_CBR);
    }
    else if(selected_rate_control_label == RATE_CONTROL_CONSTANT_QP_LABEL) {
        config->setEntry("nv_rc", RATE_CONTROL_CONSTANT_QP);
    }
    else if(selected_rate_control_label == RATE_CONTROL_VBR_HIGH_QUALITY_LABEL) {
        config->setEntry("nv_rc", RATE_CONTROL_VBR_HIGH_QUALITY);
    }

    /*fec percentage*/
    QString selected_fec = ui->h264_nvenc_fec_percentage_combobox->currentText();
    config->setEntry("fec_percentage", selected_fec);

    if( entries_snapshot.value("nv_preset") != config->getKey("nv_preset") ||
        entries_snapshot.value("system_priority") != config->getKey("system_priority") ||
        entries_snapshot.value("min_threads") != config->getKey("min_threads") ||
        entries_snapshot.value("pools") != config->getKey("pools") ||
        entries_snapshot.value("nv_rc") != config->getKey("nv_rc") ||
        entries_snapshot.value("fec_percentage") != config->getKey("fec_percentage")
            ) {
        entries_snapshot.insert("nv_preset",  config->getKey("nv_preset"));
        entries_snapshot.insert("system_priority", config->getKey("system_priority"));
        entries_snapshot.insert("min_threads", config->getKey("min_threads"));
        entries_snapshot.insert("pools", config->getKey("pools"));
        entries_snapshot.insert("nv_rc", config->getKey("nv_rc"));
        entries_snapshot.insert("fec_percentage", config->getKey("fec_percentage"));
        config->saveConfiguration();
        emit configuration_changed(h264NVENC);
    }
    this->hide();
}
