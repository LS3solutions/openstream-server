#include "h264amdamfconfigurationdialog.h"
#include "ui_h264amdamfconfigurationdialog.h"

h264AMDAMFConfigurationDialog::h264AMDAMFConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::h264AMDAMFConfigurationDialog)
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
        if( label->objectName() != "h264_amd_amf_title_label"
                && label->objectName() != "h264_amd_amf_info_label" )
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

    /*Initialize ui element values*/
    ui->h264_amd_amf_quality_combobox->addItems(ENCODER_QUALITY_OPT_LIST);

    ui->h264_amd_amf_rate_control_combobox->addItem(ENCODER_RATE_CONTROL_VBR_PEAK);

    ui->h264_amd_amf_max_bitrate_combobox->addItems(ENCODER_MAXRATE_OPT_LIST);

    config = new ConfigurationManager("/assets/h264AMDAMF.conf");
    entries_snapshot = QHash<QString, QString>();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();

    setLoadedValues();
}

h264AMDAMFConfigurationDialog::~h264AMDAMFConfigurationDialog()
{
    delete ui;
}

void h264AMDAMFConfigurationDialog::setLoadedValues() {
    if(config->getKey("amf_quality") == ENCODER_QUALITY_SPEED) {
        ui->h264_amd_amf_quality_combobox->setCurrentIndex(0);
        entries_snapshot.insert("amf_quality", ENCODER_QUALITY_SPEED);
    }
    else if(config->getKey("amf_quality") == ENCODER_QUALITY_BALANCED) {
        ui->h264_amd_amf_quality_combobox->setCurrentIndex(1);
        entries_snapshot.insert("amf_quality", ENCODER_QUALITY_BALANCED);
    }
    else if(config->getKey("amf_quality") == ENCODER_QUALITY_QUALITY) {
        ui->h264_amd_amf_quality_combobox->setCurrentIndex(2);
        entries_snapshot.insert("amf_quality", ENCODER_QUALITY_QUALITY);
    }

    ui->h264_amd_amf_rate_control_combobox->setCurrentIndex(0);
    entries_snapshot.insert("amf_rc", ENCODER_RATE_CONTROL_VBR_PEAK);


    if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_5) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(0);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_5);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_10) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(1);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_10);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_15) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(2);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_15);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_20) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(3);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_20);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_25) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(4);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_25);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_30) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(5);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_30);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_35) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(6);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_35);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_40) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(7);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_40);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_45) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(8);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_45);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_50) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(9);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_50);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_55) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(10);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_55);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_60) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(11);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_60);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_65) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(12);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_65);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_70) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(13);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_70);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_75) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(14);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_75);
    }
    else if(config->getKey("amf_maxrate") == ENCODER_MAXRATE_80) {
        ui->h264_amd_amf_max_bitrate_combobox->setCurrentIndex(15);
        entries_snapshot.insert("amf_maxrate", ENCODER_MAXRATE_80);
    }
}

void h264AMDAMFConfigurationDialog::restoreDefaultsValues() {
    config->restoreDefaultConfiguration("/h264AMDAMF.conf");
}


void h264AMDAMFConfigurationDialog::on_h264_amd_amf_restore_button_clicked()
{
    restoreDefaultsValues();
    config->reloadInMemoryValues();
    config->setEntry("file_apps", QCoreApplication::applicationDirPath() + "/assets/apps_windows.json");
    config->setEntry("file_state", QCoreApplication::applicationDirPath() + "/sunshine_state.json");
    config->saveConfiguration();
    setLoadedValues();
    emit configuration_changed(h264AMDAMF);
}

void h264AMDAMFConfigurationDialog::on_h264_amd_amf_cancel_button_clicked()
{
   this->hide();
   //For cancel, restore loaded values
   this->setLoadedValues();
}

void h264AMDAMFConfigurationDialog::on_h264_amd_amf_ok_button_clicked()
{
    QString selected_quality = ui->h264_amd_amf_quality_combobox->currentText();
    config->setEntry("amf_quality", selected_quality);

    QString selected_rate_control = ui->h264_amd_amf_rate_control_combobox->currentText();
    config->setEntry("amf_rc", selected_rate_control);

    QString selected_max_rate = ui->h264_amd_amf_max_bitrate_combobox->currentText();
    if(selected_max_rate == ENCODER_MAXRATE_5_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_5);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_10_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_10);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_15_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_15);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_20_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_20);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_25_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_25);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_30_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_30);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_35_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_35);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_40_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_40);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_45_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_45);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_50_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_50);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_55_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_55);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_60_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_60);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_65_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_65);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_70_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_70);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_75_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_75);
    }
    else if(selected_max_rate == ENCODER_MAXRATE_80_LABEL) {
        config->setEntry("amf_maxrate", ENCODER_MAXRATE_80);
    }

    if(entries_snapshot.value("amf_quality") != config->getKey("amf_quality") ||
       entries_snapshot.value("amf_rc") != config->getKey("amf_rc") ||
       entries_snapshot.value("amf_maxrate") != config->getKey("amf_maxrate")
      ) {
         config->saveConfiguration();
         entries_snapshot.insert("amf_quality", config->getKey("amf_quality"));
         entries_snapshot.insert("amf_rc", config->getKey("amf_rc"));
         entries_snapshot.insert("amf_maxrate", config->getKey("amf_maxrate"));
         emit configuration_changed(h264AMDAMF);
    }
    this->hide();
}
