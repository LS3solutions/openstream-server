#include "openstreammainwindow.h"
#include "ui_openstreammainwindow.h"

OpenstreamMainWindow::OpenstreamMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenstreamMainWindow)
{
    ui->setupUi(this);
    ui->main_area_console_output->setReadOnly(true);
    ui->main_area_widget->setCurrentIndex(0);

    h265CPUConfigDialog = new h265ConfigurationDialog(this);
    h264CPUConfigDialog = new h264CPUConfigurationDialog(this);
    h264NVENCConfigDialog = new h264NVENCConfigurationDialog(this);
    h265NVENCConfigDialog = new h265NVENCConfigurationDialog(this);
    h264AMDAMFConfigDialog = new h264AMDAMFConfigurationDialog(this);
    h265AMDAMFConfigDialog = new h265AMDAMFConfigurationDialog(this);


    allocateSharedMemoryFootprint();
    createMinimalActions();
    createTrayIcon();

    /*Auth pin handling related*/
    auth_pin_handler = new AuthPinHandler();
    allocate_auth_listener();

    /*Connections*/
    connect(ui->start_button,
            &QAbstractButton::clicked,
            this,
            &OpenstreamMainWindow::startSunshine);
    connect(ui->stop_button,
            &QAbstractButton::clicked,
            this,
            &OpenstreamMainWindow::stopSunshine);
    connect(trayIcon,
            &QSystemTrayIcon::activated,
            this,
            &OpenstreamMainWindow::trayIconActivated);
    connect(this,
            &OpenstreamMainWindow::auth_finished,
            this,
            &OpenstreamMainWindow::authListenerWorkerRegeneration);
    //This restores de QTrayApp when the notification is clicked
    connect(trayIcon,
            &QSystemTrayIcon::messageClicked,
            this,
            &QWidget::showNormal);
    connect(h265CPUConfigDialog,
            &h265ConfigurationDialog::configuration_changed,
            this,
            &OpenstreamMainWindow::configuration_changed_apply);
    connect(h264CPUConfigDialog,
            &h264CPUConfigurationDialog::configuration_changed,
            this,
            &OpenstreamMainWindow::configuration_changed_apply);
    connect(h264NVENCConfigDialog,
            &h264NVENCConfigurationDialog::configuration_changed,
            this,
            &OpenstreamMainWindow::configuration_changed_apply);
    connect(h265NVENCConfigDialog,
            &h265NVENCConfigurationDialog::configuration_changed,
            this,
            &OpenstreamMainWindow::configuration_changed_apply);

    setWindowTitle(tr("Open Stream"));
    readEncoderConfiguration();
    updateEncoderButtonsSelected();
    h265CPUConfigDialog->hide();
    h264CPUConfigDialog->hide();
    h264NVENCConfigDialog->hide();
    h265NVENCConfigDialog->hide();
    h264AMDAMFConfigDialog->hide();
    h265AMDAMFConfigDialog->hide();
    icon_off = new QIcon(":/images/joystick.png");
    icon_on = new QIcon(":/images/joystick_on.png");
    trayIcon->setIcon(*icon_off);
    trayIcon->show();
}

OpenstreamMainWindow::~OpenstreamMainWindow()
{
    delete ui;
}

/**
 * @brief OpenstreamMainWindow::createMinimalActions
 * creates the minimal actions for the Tray application popup menu.
 */
void OpenstreamMainWindow::createMinimalActions() {
   this->minimizeAction = new QAction(tr("Mi&nimize"), this);
   connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

   maximizeAction = new QAction(tr("Ma&ximize"), this);
   connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

   restoreAction = new QAction(tr("&Restore"), this);
   connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

   quitAction = new QAction(tr("&Quit"), this);
   connect(quitAction, &QAction::triggered, this, &OpenstreamMainWindow::stopHostBeforeClose);
   connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

/**
 * @brief OpenstreamMainWindow::createTrayIcon
 * creates the Tray icon object for tray functionality.
 * Should be called after createMinimalActions.
 */
void OpenstreamMainWindow::createTrayIcon() {
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIconMenu->setStyleSheet(
                "QMenu"
                "{"
                "background-color: #979dac;"
                "font: \"Open Sans Semibold\";"
                "}"
                );
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(STATE_STOPPED_MSG_TRAY);
}

/**
 * @brief OpenstreamMainWindow::trayIconActivated
 * deals with user mouse interactions
 * @param reason
 */
void OpenstreamMainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
        case QSystemTrayIcon::DoubleClick:
            this->showNormal();
            break;
        default:
            //Empty by now. Default behaviour for all clicks.
        ;
    }
}

/**
 * @brief OpenstreamMainWindow::setVisible
 * overriden setVisible for tray application.
 * @param visible
 */
void OpenstreamMainWindow::setVisible(bool visible) {
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QWidget::setVisible(visible);

}

/**
 * @brief OpenstreamMainWindow::closeEvent
 * overriden closeEvent for tray application.
 * @param event
 */
void OpenstreamMainWindow::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

/**
 * @brief OpenstreamMainWindow::allocateSharedMemoryFootprint
 * Checks if another application is running with the same memory footprint.
 */
void OpenstreamMainWindow::allocateSharedMemoryFootprint() {
    sharedMemoryFootprint = new QSharedMemory(sharedMemoryFootprintKey, this);
    bool semaphore = sharedMemoryFootprint->create(4);
    if(semaphore) {
        qDebug() << "Starting new application. Not previous footprint found." << Qt::endl;
    }
    else {
        switch(sharedMemoryFootprint->error())
        {
            case QSharedMemory::InvalidSize:
                qDebug() << "Invalid footprint size" << Qt::endl;
                break;
           case QSharedMemory::QSharedMemory::KeyError:
                qDebug() << "Invalid footprint key" << Qt::endl;
                break;
           case QSharedMemory::AlreadyExists:
                qDebug() << "An instance of Sunshine Launcher is already running."
                         << "Closing current instance." << Qt::endl;
                sharedMemoryFootprintErrorMessage();
                break;
           default:
                qDebug() << "Unknown error found creating shared memory footprint" << Qt::endl;
        }

    }
}

void OpenstreamMainWindow::sharedMemoryFootprintErrorMessage() {
    QMessageBox m = QMessageBox(QMessageBox::Icon::Critical,
                                "Application error",
                                "Sunshine launcher is already executing.",
                                QMessageBox::StandardButton::Close);

    m.setStyleSheet("QMessageBox"
                    "{"
                    "color: rgb(255, 170, 0);"
                    "background-color: #272640;"
                    "}"
                    "QPushButton"
                    "{"
                    "background-color: #272640;"
                    "color: #FFFFFF;"
                    "text-align: center;"
                    "font: \"Open Sans Light\";"
                    "}"
                    "QPushButton:hover"
                    "{"
                    "background-color: #006466;"
                    "}"
                    "QLabel"
                    "{"
                    "color: #ffffff;"
                    "font: \"Open Sans Light\";"
                    "}");
    m.exec();
    exit(EXIT_FAILURE);
}

void OpenstreamMainWindow::allocateNewProcess() {
    proc = new QProcess;
}

void OpenstreamMainWindow::startSunshine() {
    allocateNewProcess();
    switch(proc->state()) {
        case QProcess::NotRunning:
            appStart();
            break;
        case QProcess::Starting:
            appStarting();
            break;
        case QProcess::Running:
            appRunning();
            break;
        default:
            qDebug() << "Unknown host state" << Qt::endl;
    }
}

void OpenstreamMainWindow::stopSunshine() {
    qDebug() << "Stoping sunshine process...";
    qint64 pid = proc->processId();
    if(pid > 0) {
        //TODO: research use of kill vs terminate
        proc->kill();
        proc->disconnect();
        this->set_off_host_state_indicator();
        proc->deleteLater();
        proc = nullptr;
        //Allocate new process
        qDebug() << "Process host stopped " << pid << Qt::endl;
    }
    else {
        qDebug() << "Process currently stopped.";
    }
}

void OpenstreamMainWindow::appStart() {
    qDebug() << "Start application" << Qt::endl;
    QString app_dir = QCoreApplication::applicationDirPath();
    connect(proc, &QProcess::readyReadStandardOutput, this, &OpenstreamMainWindow::updateAppConsole);
    connect(proc, &QProcess::readyRead, this, &OpenstreamMainWindow::updateAppConsole);
    connect(proc, &QProcess::readyReadStandardError, this, &OpenstreamMainWindow::updateAppConsoleError);
    connect(proc, &QProcess::started, this, &OpenstreamMainWindow::set_on_host_state_indicator);
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &OpenstreamMainWindow::appStoppedWatch);
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &OpenstreamMainWindow::set_off_host_state_indicator);
    proc->open(QProcess::Unbuffered);
    if (current_encoder == h264CPU) {
       proc->start(app_dir + "/openstreamhost/openstreamhost.exe", QStringList() << app_dir + H264_CONF);
    }
    else if(current_encoder == h265CPU) {
        proc->start(app_dir + "/openstreamhost/openstreamhost.exe", QStringList() << app_dir + H265_CONF);
    }
    else if(current_encoder == h264NVENC) {
        proc->start(app_dir + "/openstreamhost/openstreamhost.exe", QStringList() << app_dir + H264_NVENC_CONF);
    }
    else if(current_encoder == h265NVENC) {
        proc->start(app_dir + "/openstreamhost/openstreamhost.exe", QStringList() << app_dir + H265_NVENC_CONF);
    }
    else if(current_encoder == h264AMDAMF) {
        proc->start(app_dir + "/openstreamhost/openstreamhost.exe", QStringList() << app_dir + H264_AMD_AMF_CONF);
    }
    else if(current_encoder == h265AMDAMF) {
        proc->start(app_dir + "/openstreamhost/openstreamhost.exe", QStringList() << app_dir + H265_AMD_AMF_CONF);
    }
    else {
        qDebug() << "DEFAULT ENCODER FILE SETTINGS" << Qt::endl;
        proc->start(app_dir + "/openstreamhost/openstreamhost.exe", QStringList() << app_dir + "/assets/sunshine.conf");
    }
    qDebug() << "Application started" << Qt::endl;
}

void OpenstreamMainWindow::appStarting() {
    qDebug() << "A sunshine host is currently starting" << Qt::endl;
    QMessageBox::information(this, "Sunshine Launcher",
                             tr("A sunshine host is currently starting"));
}

void OpenstreamMainWindow::appRunning() {
    qDebug() << "A sunshine host was already started" << Qt::endl;
    QMessageBox m = QMessageBox(
                QMessageBox::Icon::Information,
                "Openstream start",
                "A openstream server is already up",
                QMessageBox::StandardButton::Ok);
    m.setStyleSheet("QMessageBox"
                    "{"
                    "color: rgb(255, 170, 0);"
                    "background-color: #272640;"
                    "}"
                    "QPushButton"
                    "{"
                    "background-color: #272640;"
                    "color: #FFFFFF;"
                    "text-align: center;"
                    "font: \"Open Sans Light\";"
                    "}"
                    "QPushButton:hover"
                    "{"
                    "background-color: #006466;"
                    "}"
                    "QLabel"
                    "{"
                    "color: #ffffff;"
                    "font: \"Open Sans Light\";"
                    "}");
    m.exec();
}

void OpenstreamMainWindow::set_on_host_state_indicator() {
    ui->main_area_status_label->setText(STATE_RUNNING_MSG);
    trayIcon->setIcon(*icon_on);
    trayIcon->setToolTip(STATE_RUNNING_MSG);
}

void OpenstreamMainWindow::set_off_host_state_indicator() {
    ui->main_area_status_label->setText(STATE_STOPPED_MSG);
    trayIcon->setIcon(*icon_off);
    trayIcon->setToolTip(STATE_STOPPED_MSG);
}

void OpenstreamMainWindow::updateAppConsole() {
    QByteArray strData = proc->readAllStandardOutput();
    QString lineString(strData);
    ui->main_area_console_output->moveCursor(QTextCursor::End);
    ui->main_area_console_output->textCursor().insertText(lineString);
    ui->main_area_console_output->moveCursor(QTextCursor::End);
}

void OpenstreamMainWindow::updateAppConsoleError()
{
    QByteArray strData = proc->readAllStandardError();
    QString lineString(strData);
    ui->main_area_console_output->moveCursor (QTextCursor::End);
    ui->main_area_console_output->textCursor().insertText(lineString);
    ui->main_area_console_output->moveCursor(QTextCursor::End);
}

void OpenstreamMainWindow::appStoppedWatch() {
    qDebug() << "Streamming host was stopped" << Qt::endl;
}

void OpenstreamMainWindow::stopHostBeforeClose() {
    if(this->proc != nullptr)
        this->stopSunshine();
}

/**
 * @brief on_event_loop_started
 * Starts the openstreamhost when the GUI has been started.
 */
void OpenstreamMainWindow::on_event_loop_started() {
    startSunshine();
}


void OpenstreamMainWindow::allocate_auth_listener() {
    auth_listener_thread = new QThread();
    auth_listener_worker = new AuthListenerWorker();
    auth_listener_worker->moveToThread(auth_listener_thread);
    connect(auth_listener_worker,
            &AuthListenerWorker::auth_attempt,
            this,
            &OpenstreamMainWindow::showAuthMessagePopUp);
    connect(auth_listener_worker,
            &AuthListenerWorker::auth_attempt,
            this,
            &OpenstreamMainWindow::inputAuthPinCapture);
    connect(auth_listener_thread,
            &QThread::started,
            auth_listener_worker,
            &AuthListenerWorker::start_listening);
    connect(auth_listener_worker,
            &AuthListenerWorker::finished,
            auth_listener_thread,
            &QThread::quit);
    connect(auth_listener_worker,
            &AuthListenerWorker::finished,
            auth_listener_worker,
            &QObject::deleteLater);
    connect(auth_listener_thread,
            &QThread::finished,
            auth_listener_thread,
            &QThread::deleteLater);
    auth_listener_thread->start();
}

void OpenstreamMainWindow::authListenerWorkerRegeneration() {
    allocate_auth_listener();
}

void OpenstreamMainWindow::inputAuthPinCapture() {
    bool ok;
    QString pin = QInputDialog::getText(this, tr("Introduce PIN"),
                                        tr("PIN:"), QLineEdit::Normal,
                                        "", &ok);
    if(ok) {
        qDebug() << "PIN captured!: " << pin << Qt::endl;
        auth_pin_handler->send_host_pin(pin);
    }
    else {
        qDebug() << "NO PIN captured! :( " << pin << Qt::endl;
    }

    emit auth_finished(); /*Restart named pipe listening*/
}

void OpenstreamMainWindow::showAuthMessagePopUp() {
    QString title = QString(tr("Client is trying to connect"));
    QString body = QString(tr("A client wants to use openstream.\n"
                              "Click this message for introduce your pin"));

    trayIcon->showMessage(title, body, trayIcon->icon(), 60 * 100000);
}

void OpenstreamMainWindow::on_go_back_config_button_clicked()
{
    ui->main_area_widget->setCurrentIndex(0);
    ui->start_button->setEnabled(true);
    ui->stop_button->setEnabled(true);
}

void OpenstreamMainWindow::on_configure_button_clicked()
{
    ui->main_area_widget->setCurrentIndex(1);
    ui->start_button->setEnabled(false);
    ui->stop_button->setEnabled(false);
}

void OpenstreamMainWindow::on_h265_CPU_configure_button_clicked()
{
    h265CPUConfigDialog->exec();
}

/**
 * @brief configuration_changed_apply
 * This slot is connected to configuration signals,
 * to auto restart the host if configuration applies.
 */
void OpenstreamMainWindow::configuration_changed_apply(VideoEncoder sender_encoder) {
    if(proc != nullptr && proc->state() == QProcess::Running && current_encoder == sender_encoder) {
        /*Pseudo-Restart host after config changes*/
       QMessageBox m = QMessageBox(
                    QMessageBox::Icon::Information,
                    "Info",
                    "Configuration changed. Stream host will restart.",
                    QMessageBox::StandardButton::Ok);
        m.setStyleSheet("QMessageBox"
                        "{"
                        "color: rgb(255, 170, 0);"
                        "background-color: #272640;"
                        "}"
                        "QPushButton"
                        "{"
                        "background-color: #272640;"
                        "color: #FFFFFF;"
                        "text-align: center;"
                        "font: \"Open Sans Light\";"
                        "}"
                        "QPushButton:hover"
                        "{"
                        "background-color: #006466;"
                        "}"
                        "QLabel"
                        "{"
                        "color: #ffffff;"
                        "font: \"Open Sans Light\";"
                        "}");
        m.exec();
        stopSunshine();
        startSunshine();
    }
}

void OpenstreamMainWindow::readEncoderConfiguration() {
    QFile inputFile(QCoreApplication::applicationDirPath() + "/assets/encoder.conf");
    if(inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            if(line.startsWith("#") ||
                    line.startsWith("\n") ||
                    line.isEmpty())
                continue;

            auto key_val = line.split("=");
            QString encoder_str =  key_val.last().trimmed();
            qDebug() << encoder_str << Qt::endl;
            if (encoder_str == "h264CPU") {
                current_encoder = h264CPU;
            }
            else if(encoder_str == "h265CPU") {
                current_encoder = h265CPU;
            }
            else if(encoder_str == "h264NVENC") {
                current_encoder = h264NVENC;
            }
            else if (encoder_str == "h265NVENC") {
                current_encoder = h265NVENC;
            }
            else {
                current_encoder = h264CPU;
            }
        }
    }
    inputFile.close();
}

void OpenstreamMainWindow::writeEncoderConfiguration() {
    QFile outputFile(QCoreApplication::applicationDirPath() + "/assets/encoder.conf");
    outputFile.open(QIODevice::WriteOnly);
    QTextStream outStream(&outputFile);
    if (current_encoder == h264CPU) {
        outStream << "encoder" << "=" << "h264CPU" << Qt::endl;
    }
    else if(current_encoder == h265CPU) {
        outStream << "encoder" << "=" << "h265CPU" << Qt::endl;
    }
    else if(current_encoder == h264NVENC) {
        outStream << "encoder" << "=" << "h264NVENC" << Qt::endl;
    }
    else if (current_encoder == h265NVENC) {
        outStream << "encoder" << "=" << "h265NVENC" << Qt::endl;
    } else if(current_encoder == h264AMDAMF) {
        outStream << "encoder" << "=" << "h264AMDAMF" << Qt::endl;
    } else if(current_encoder == h265AMDAMF) {
        outStream << "encoder" << "=" << "h265AMDAMF" << Qt::endl;
    }
    else {
       outStream << "encoder" << "=" << "h264CPU" << Qt::endl;
    }
}

/**
 * @brief OpenstreamMainWindow::removeIconsFromSelectionButtons
 * Empty all the icons from encoders selection buttons.
 */
void OpenstreamMainWindow::removeIconsFromSelectionButtons() {
    ui->h264_CPU_select_button->setIcon(QIcon());
    ui->h264_CPU_select_button->setText("Select");
    ui->h265_CPU_select_button->setIcon(QIcon());
    ui->h265_CPU_select_button->setText("Select");
    ui->h264_NVENC_select_button->setIcon(QIcon());
    ui->h264_NVENC_select_button->setText("Select");
    ui->h265_NVENC_select_button->setIcon(QIcon());
    ui->h265_NVENC_select_button->setText("Select");
    ui->h264_AMD_select_button->setIcon(QIcon());
    ui->h264_AMD_select_button->setText("Select");
    ui->h265_AMD_select_button->setIcon(QIcon());
    ui->h265_AMD_select_button->setText("Select");
}

/**
 * @brief OpenstreamMainWindow::updateEncoderButtonsSelected
 * Updates the icon for the approriate select button based on which
 * encoder is selected.
 */
void OpenstreamMainWindow::updateEncoderButtonsSelected() {
    removeIconsFromSelectionButtons();

    if(this->current_encoder == h264CPU) {
        ui->h264_CPU_select_button->setIcon(QIcon(":images/check-ic.png"));
        ui->h264_CPU_select_button->setText("");
    }
    else if(this->current_encoder == h265CPU) {
        ui->h265_CPU_select_button->setIcon(QIcon(":images/check-ic.png"));
        ui->h265_CPU_select_button->setText("");
    }
    else if(this->current_encoder == h264NVENC) {
        ui->h264_NVENC_select_button->setIcon(QIcon(":images/check-ic.png"));
        ui->h264_NVENC_select_button->setText("");
    }
    else if(this->current_encoder == h265NVENC) {
        ui->h265_NVENC_select_button->setIcon(QIcon(":images/check-ic.png"));
        ui->h265_NVENC_select_button->setText("");
    }
    else if(this->current_encoder == h264AMDAMF) {
        ui->h264_AMD_select_button->setIcon(QIcon(":images/check-ic.png"));
        ui->h264_AMD_select_button->setText("");
    }
    else if(this->current_encoder == h265AMDAMF) {
        ui->h265_AMD_select_button->setIcon(QIcon(":images/check-ic.png"));
        ui->h265_AMD_select_button->setText("");
    }
}

void OpenstreamMainWindow::on_h265_CPU_select_button_clicked()
{
    if(current_encoder == h265CPU)
        return;

    current_encoder = h265CPU;
    writeEncoderConfiguration();
    removeIconsFromSelectionButtons();
    updateEncoderButtonsSelected();
    configuration_changed_apply(h265CPU);
}

void OpenstreamMainWindow::on_h264_CPU_configuration_button_clicked()
{
    h264CPUConfigDialog->exec();
}

void OpenstreamMainWindow::on_h264_CPU_select_button_clicked()
{
    if(current_encoder == h264CPU)
        return;
    current_encoder = h264CPU;
    writeEncoderConfiguration();
    removeIconsFromSelectionButtons();
    updateEncoderButtonsSelected();
    configuration_changed_apply(h264CPU);
}

void OpenstreamMainWindow::on_h264_NVENC_configure_button_clicked()
{
    h264NVENCConfigDialog->exec();
}

void OpenstreamMainWindow::on_h264_NVENC_select_button_clicked()
{
    if(current_encoder == h264NVENC)
        return;
    current_encoder = h264NVENC;
    writeEncoderConfiguration();
    removeIconsFromSelectionButtons();
    updateEncoderButtonsSelected();
    configuration_changed_apply(h264NVENC);
}

void OpenstreamMainWindow::on_h265_NVENC_configure_button_clicked()
{
    h265NVENCConfigDialog->exec();
}

void OpenstreamMainWindow::on_h265_NVENC_select_button_clicked()
{
    if(current_encoder == h265NVENC)
        return;
    current_encoder = h265NVENC;
    writeEncoderConfiguration();
    removeIconsFromSelectionButtons();
    updateEncoderButtonsSelected();
    configuration_changed_apply(h265NVENC);
}

void OpenstreamMainWindow::resizeEvent(QResizeEvent *event)  {
    qDebug() << "Windows resize" << Qt::endl;
}

void OpenstreamMainWindow::on_h264_AMD_select_button_clicked()
{
    if(current_encoder == h264AMDAMF)
        return;
    current_encoder = h264AMDAMF;
    writeEncoderConfiguration();
    removeIconsFromSelectionButtons();
    updateEncoderButtonsSelected();
    configuration_changed_apply(h264AMDAMF);
}

void OpenstreamMainWindow::on_h265_AMD_select_button_clicked()
{
    if(current_encoder == h265AMDAMF)
        return;
    current_encoder = h265AMDAMF;
    writeEncoderConfiguration();
    removeIconsFromSelectionButtons();
    updateEncoderButtonsSelected();
    configuration_changed_apply(h265AMDAMF);
}
