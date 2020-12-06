#include "h265configurationdialog.h"
#include "ui_h265configurationdialog.h"



h265ConfigurationDialog::h265ConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::h265ConfigurationDialog)
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
        if( label->objectName() != "h265_cpu_title_label"
                && label->objectName() != "h265_cpu_info_label" )
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

    config = new ConfigurationManager("/assets/h265CPU.conf");
    entries_snapshot = QHash<QString, QString>();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();

    /*Initialize UI Elements values*/
    /*Software Encoder Speed*/
    ui->h265_cpu_encoding_speed_combobox->addItems(this->ENCODER_SPEED_OPT_LIST);

    /*Process Priority*/
    ui->h265_cpu_process_priority_combobox->addItems(this->SYS_PRIORITY_OP_LIST);

    /*Frame threads*/
    ui->h265_cpu_frame_threads_combobox->addItems(this->FRAME_THREADS_OPT_LIST);

    /*Pool threads*/
    ui->h265_cpu_pool_threads_combobox->addItems(this->POOL_THREADS_OPT_LIST);

    /*VBV max rate*/
    ui->h265_cpu_vbv_max_rate_value_combobox->addItems(VBV_MAX_RATE_LABEL_OPT_LIST);

    /*VBV_BUFSIZE*/
    ui->h265_cpu_vbv_bufsize_combobox->addItems(VBV_BUFSIZE_LABEL_OPT_LIST);

    /*CRF*/
    ui->h265_cpu_crf_rate_combobox->addItems(CRF_OPT_LIST);

    /*FEC*/
    ui->h265_cpu_fec_percentage_combobox->addItems(FEC_OPT_LIST);

    setLoadedValues();
}

h265ConfigurationDialog::~h265ConfigurationDialog()
{
    delete ui;
}

void h265ConfigurationDialog::restoreDefaultsValues() {
    config->restoreDefaultConfiguration("/h265CPU.conf");
}

/**
 * @brief h265ConfigurationDialog::setLoadedValues
 * Set the values in the GUI form according to the last state found in
 * the encoder configuration file, and already saved in the In Memory config.
 */
void h265ConfigurationDialog::setLoadedValues() {
    /*Encoder preset*/
    if(config->getKey("sw_preset") == ENCODER_SPEED_FAST) {
        ui->h265_cpu_encoding_speed_combobox->setCurrentIndex(0);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_FAST);
    }
    else if(config->getKey("sw_preset") == ENCODER_SPEED_FASTER) {
        ui->h265_cpu_encoding_speed_combobox->setCurrentIndex(1);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_FASTER);
    }
    else if(config->getKey("sw_preset") == ENCODER_SPEED_SUPERFAST) {
        ui->h265_cpu_encoding_speed_combobox->setCurrentIndex(2);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_SUPERFAST);
    }
    else if(config->getKey("sw_preset") == ENCODER_SPEED_ULTRAFAST) {
        ui->h265_cpu_encoding_speed_combobox->setCurrentIndex(3);
        entries_snapshot.insert("sw_preset", ENCODER_SPEED_ULTRAFAST);
    }

    /*System Priority*/
    /*Compare against the configuration values (1,2,3)*/
    if(config->getKey("system_priority") == SYS_PRIORITY_ABOVE_NORMAL) {
       ui->h265_cpu_process_priority_combobox->setCurrentIndex(0);
       entries_snapshot.insert("system_priority", SYS_PRIORITY_ABOVE_NORMAL);
    }
    else if(config->getKey("system_priority") == SYS_PRIORITY_HIGH) {
        ui->h265_cpu_process_priority_combobox->setCurrentIndex(1);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_HIGH);
    }
    else if(config->getKey("system_priority") == SYS_PRIORITY_REAL_TIME) {
        ui->h265_cpu_process_priority_combobox->setCurrentIndex(2);
        entries_snapshot.insert("system_priority", SYS_PRIORITY_REAL_TIME);
    }

    /*Frame threads*/
    if(config->getKey("min_threads") == FRAME_THREADS_2) {
        ui->h265_cpu_frame_threads_combobox->setCurrentIndex(0);
        entries_snapshot.insert("min_threads", FRAME_THREADS_2);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_4) {
        ui->h265_cpu_frame_threads_combobox->setCurrentIndex(1);
        entries_snapshot.insert("min_threads", FRAME_THREADS_4);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_6) {
        ui->h265_cpu_frame_threads_combobox->setCurrentIndex(2);
        entries_snapshot.insert("min_threads", FRAME_THREADS_6);
    }
    else if(config->getKey("min_threads") == FRAME_THREADS_8) {
        ui->h265_cpu_frame_threads_combobox->setCurrentIndex(3);
        entries_snapshot.insert("min_threads", FRAME_THREADS_8);
    }

    /*pool threads*/
    if(config->getKey("pools") == POOL_THREADS_2) {
        ui->h265_cpu_pool_threads_combobox->setCurrentIndex(0);
        entries_snapshot.insert("pools", POOL_THREADS_2);
    }
    else if(config->getKey("pools") == POOL_THREADS_4) {
        ui->h265_cpu_pool_threads_combobox->setCurrentIndex(1);
        entries_snapshot.insert("pools", POOL_THREADS_4);
    }
    else if(config->getKey("pools") == POOL_THREADS_6) {
        ui->h265_cpu_pool_threads_combobox->setCurrentIndex(2);
        entries_snapshot.insert("pools", POOL_THREADS_6);
    }
    else if(config->getKey("pools") == POOL_THREADS_8) {
        ui->h265_cpu_pool_threads_combobox->setCurrentIndex(3);
        entries_snapshot.insert("pools", POOL_THREADS_8);
    }

    /*CRF*/
    if(config->getKey("crf") == CRF_3) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(0);
        entries_snapshot.insert("crf", CRF_3);
    } else if(config->getKey("crf") == CRF_6) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(1);
        entries_snapshot.insert("crf", CRF_6);
    } else if(config->getKey("crf") == CRF_9) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(2);
        entries_snapshot.insert("crf", CRF_9);
    }
    else if(config->getKey("crf") == CRF_11) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(3);
        entries_snapshot.insert("crf", CRF_11);
    }
    else if(config->getKey("crf") == CRF_14) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(4);
        entries_snapshot.insert("crf", CRF_14);
    }
    else if(config->getKey("crf") == CRF_17) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(5);
        entries_snapshot.insert("crf", CRF_17);
    }
    else if(config->getKey("crf") == CRF_20) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(6);
        entries_snapshot.insert("crf", CRF_20);
    }
    else if(config->getKey("crf") == CRF_23) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(7);
        entries_snapshot.insert("crf", CRF_23);
    }
    else if(config->getKey("crf") == CRF_25) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(8);
        entries_snapshot.insert("crf", CRF_25);
    }
    else if(config->getKey("crf") == CRF_28) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(9);
        entries_snapshot.insert("crf", CRF_28);
    }
    else if(config->getKey("crf") == CRF_31) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(10);
        entries_snapshot.insert("crf", CRF_31);
    }
    else if(config->getKey("crf") == CRF_34) {
        ui->h265_cpu_crf_rate_combobox->setCurrentIndex(11);
        entries_snapshot.insert("crf", CRF_34);
    }

    /*VBV Bufsize*/
    if(config->getKey("vbv_bufsize") == VBV_BUFSIZE_1) {
        ui->h265_cpu_vbv_bufsize_combobox->setCurrentIndex(0);
        entries_snapshot.insert("vbv_bufsize", VBV_BUFSIZE_1);
    }
    else if(config->getKey("vbv_bufsize") == VBV_BUFSIZE_5) {
        ui->h265_cpu_vbv_bufsize_combobox->setCurrentIndex(1);
        entries_snapshot.insert("vbv_bufsize", VBV_BUFSIZE_5);
    }
    else if(config->getKey("vbv_bufsize") == VBV_BUFSIZE_10) {
        ui->h265_cpu_vbv_bufsize_combobox->setCurrentIndex(2);
        entries_snapshot.insert("vbv_bufsize", VBV_BUFSIZE_10);

    }
    else if(config->getKey("vbv_bufsize") ==  VBV_BUFSIZE_15) {
        ui->h265_cpu_vbv_bufsize_combobox->setCurrentIndex(3);
        entries_snapshot.insert("vbv_bufsize",  VBV_BUFSIZE_15);
    }

    /*vbv-maxrate*/
    if(config->getKey("vbv_maxrate") == VBV_MAX_RATE_10) {
        ui->h265_cpu_vbv_max_rate_value_combobox->setCurrentIndex(0);
        entries_snapshot.insert("vbv_maxrate", VBV_MAX_RATE_10);
    }
    else if(config->getKey("vbv_maxrate") == VBV_MAX_RATE_20) {
        ui->h265_cpu_vbv_max_rate_value_combobox->setCurrentIndex(1);
        entries_snapshot.insert("vbv_maxrate", VBV_MAX_RATE_20);
    }
    else if(config->getKey("vbv_maxrate") == VBV_MAX_RATE_30) {
        ui->h265_cpu_vbv_max_rate_value_combobox->setCurrentIndex(2);
        entries_snapshot.insert("vbv_maxrate", VBV_MAX_RATE_30);

    }
    else if(config->getKey("vbv_maxrate") == VBV_MAX_RATE_40) {
        ui->h265_cpu_vbv_max_rate_value_combobox->setCurrentIndex(3);
        entries_snapshot.insert("vbv_maxrate", VBV_MAX_RATE_40);
    }

    /*FEC*/
    if(config->getKey("fec_percentage") == FEC_10) {
        ui->h265_cpu_fec_percentage_combobox->setCurrentIndex(0);
        entries_snapshot.insert("fec_percentage", FEC_10);
    }
    else if(config->getKey("fec_percentage") == FEC_20) {
        ui->h265_cpu_fec_percentage_combobox->setCurrentIndex(1);
        entries_snapshot.insert("fec_percentage", FEC_20);
    }
    else if(config->getKey("fec_percentage") == FEC_30) {
        ui->h265_cpu_fec_percentage_combobox->setCurrentIndex(2);
        entries_snapshot.insert("fec_percentage", FEC_30);
    }
    else if(config->getKey("fec_percentage") == FEC_40) {
        ui->h265_cpu_fec_percentage_combobox->setCurrentIndex(3);
        entries_snapshot.insert("fec_percentage", FEC_40);
    }
}

void h265ConfigurationDialog::on_h265_cpu_cancel_button_clicked()
{
    this->hide();
    //For cancel, restore loaded values
    this->setLoadedValues();
}

/**
 * @brief h265ConfigurationDialog::on_h265_cpu_restore_button_clicked
 * restore default values from resources file. It copies the resources
 * file to the assets folder.
 */
void h265ConfigurationDialog::on_h265_cpu_restore_button_clicked()
{
    restoreDefaultsValues();
    config->reloadInMemoryValues();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();
    setLoadedValues();
    emit configuration_changed(h265CPU);
}

/**
 * @brief h265ConfigurationDialog::on_h265_cpu_ok_button_clicked
 *
 * - Go through all the values collected values from the GUI
 * - Invokes the event to auto-restart the host if app is running.
 */
void h265ConfigurationDialog::on_h265_cpu_ok_button_clicked()
{
    /*Encoder Speed*/
    QString selected_encoder_speed_value = ui->h265_cpu_encoding_speed_combobox->currentText();
    config->setEntry("sw_preset", selected_encoder_speed_value);
    /*System priority*/
    QString selected_system_priority_label = ui->h265_cpu_process_priority_combobox->currentText();
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
    QString selected_frame_threads = ui->h265_cpu_frame_threads_combobox->currentText();
    config->setEntry("min_threads", selected_frame_threads);
    /*Pool threads*/
    QString selected_pool_threads = ui->h265_cpu_pool_threads_combobox->currentText();
    config->setEntry("pools", selected_pool_threads);

    /*VBV maxrate value*/
    QString selected_vbv_maxrate_label = ui->h265_cpu_vbv_max_rate_value_combobox->currentText();
    if(selected_vbv_maxrate_label == VBV_MAX_RATE_10_LABEL) {
        config->setEntry("vbv_maxrate", VBV_MAX_RATE_10);
    }
    else if(selected_vbv_maxrate_label == VBV_MAX_RATE_20_LABEL) {
        config->setEntry("vbv_maxrate", VBV_MAX_RATE_20);
    }
    else if(selected_vbv_maxrate_label == VBV_MAX_RATE_30_LABEL) {
        config->setEntry("vbv_maxrate", VBV_MAX_RATE_30);
    }
    else if(selected_vbv_maxrate_label == VBV_MAX_RATE_40_LABEL) {
        config->setEntry("vbv_maxrate", VBV_MAX_RATE_40);
    }
    /*VBV bufsize*/
    QString selected_vbv_bufsize_label = ui->h265_cpu_vbv_bufsize_combobox->currentText();
    if(selected_vbv_bufsize_label == VBV_BUFSIZE_1_LABEL) {
        config->setEntry("vbv_bufsize", VBV_BUFSIZE_1);
    }
    else if(selected_vbv_bufsize_label == VBV_BUFSIZE_5_LABEL) {
        config->setEntry("vbv_bufsize", VBV_BUFSIZE_5);
    }
    else if(selected_vbv_bufsize_label == VBV_BUFSIZE_10_LABEL) {
        config->setEntry("vbv_bufsize", VBV_BUFSIZE_10);
    }
    else if(selected_vbv_bufsize_label == VBV_BUFSIZE_15_LABEL) {
        config->setEntry("vbv_bufsize", VBV_BUFSIZE_15);
    }
    /*crf rate*/
    QString selected_crf = ui->h265_cpu_crf_rate_combobox->currentText();
    config->setEntry("crf", selected_crf);
    /*fec percentage*/
    QString selected_fec = ui->h265_cpu_fec_percentage_combobox->currentText();
    config->setEntry("fec_percentage", selected_fec);

    if(
        entries_snapshot.value("sw_preset") != config->getKey("sw_preset") ||
        entries_snapshot.value("system_priority") != config->getKey("system_priority") ||
        entries_snapshot.value("min_threads") != config->getKey("min_threads") ||
        entries_snapshot.value("pools") != config->getKey("pools") ||
        entries_snapshot.value("vbv_maxrate") != config->getKey("vbv_maxrate") ||
        entries_snapshot.value("vbv_bufsize") != config->getKey("vbv_bufsize") ||
        entries_snapshot.value("crf") != config->getKey("crf") ||
        entries_snapshot.value("fec_percentage") != config->getKey("fec_percentage")
            ) {
            entries_snapshot.insert("sw_preset", config->getKey("sw_preset"));
            entries_snapshot.insert("system_priority", config->getKey("system_priority"));
            entries_snapshot.insert("min_threads", config->getKey("min_threads"));
            entries_snapshot.insert("pools", config->getKey("pools"));
            entries_snapshot.insert("vbv_maxrate", config->getKey("vbv_maxrate"));
            entries_snapshot.insert("vbv_bufsize", config->getKey("vbv_bufsize"));
            entries_snapshot.insert("fec_percentage", config->getKey("fec_percentage"));
            entries_snapshot.insert("crf", config->getKey("crf"));
            config->saveConfiguration();
            emit configuration_changed(h265CPU);
    }
    this->hide();
}
