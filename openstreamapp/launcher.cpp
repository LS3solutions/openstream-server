#include "launcher.h"

Launcher::Launcher(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << DEBUGFLAG << "Running at: " << qApp->applicationDirPath() << endl;
    allocateSharedMemoryFootprint();
    createMainGroupBox();
    configDialog->setGUIPriority();
    createMinmalActions();
    createTrayIcon();
    allocateNewProcess();


    auth_pin_handler = new AuthPinHandler();
    allocate_auth_listener();

    connect(startSunshineButton, &QAbstractButton::clicked, this, &Launcher::startSunshine);
    connect(stopSunshineButton, &QAbstractButton::clicked, this, &Launcher::stopSunshine);
    connect(configureSunshineButton, &QAbstractButton::clicked,
            configDialog, &ConfigurationDialog::editConfigurationClicked);
    connect(configDialog, &ConfigurationDialog::configuration_changed, this, &Launcher::configuration_changed_apply);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Launcher::trayIconActivated);

    //This restores de QTrayApp when the notification is clicked
    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &QWidget::showNormal);
    connect(this, &Launcher::auth_finished, this, &Launcher::authListenerWorkerRegeneration);
    this->setLayout(controlVBoxLayout);
    icon_off = new QIcon(":/images/joystick.png");
    icon_on = new QIcon(":/images/joystick_on.png");
    trayIcon->setIcon(*icon_off);
    trayIcon->show();


    setWindowTitle(tr("Open Stream"));
    resize(WIDTH, HEIGHT);
}

void Launcher::showAuthMessagePopUp() {
    QString title = QString(tr("Client is trying to connect"));
    QString body = QString(tr("A client wants to use openstream.\n"
                              "Click this message for introduce your pin"));

    trayIcon->showMessage(title, body, trayIcon->icon(), 60 * 100000);
}

void Launcher::inputAuthPinCapture() {
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

void Launcher::setVisible(bool visible) {
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QWidget::setVisible(visible);

}

void Launcher::allocate_auth_listener() {
    auth_listener_thread = new QThread();
    auth_listener_worker = new AuthListenerWorker();
    auth_listener_worker->moveToThread(auth_listener_thread);
    connect(auth_listener_worker, &AuthListenerWorker::auth_attempt, this, &Launcher::showAuthMessagePopUp);
    connect(auth_listener_worker, &AuthListenerWorker::auth_attempt, this, &Launcher::inputAuthPinCapture);
    connect(auth_listener_thread, &QThread::started, auth_listener_worker, &AuthListenerWorker::start_listening);
    connect(auth_listener_worker, &AuthListenerWorker::finished, auth_listener_thread, &QThread::quit);
    connect(auth_listener_worker,  &AuthListenerWorker::finished, auth_listener_worker, &QObject::deleteLater);
    connect(auth_listener_thread, &QThread::finished, auth_listener_thread, &QThread::deleteLater);
    auth_listener_thread->start();
}

void Launcher::authListenerWorkerRegeneration() {
    allocate_auth_listener();
}

void Launcher::closeEvent(QCloseEvent *event)
{
    if(trayIcon->isVisible()) {
        QMessageBox::information(this, "Sunshine Launcher",
                                 tr("The program will keep running in the"
                                    "system tray. To terminate the program,"
                                    "choose <b>Quit</b> in the context"
                                    "menu of the system tray entry."));
        hide();
        event->ignore();
    }
}

void Launcher::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
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

void Launcher::allocateNewProcess(){
    proc = new QProcess();
}

void Launcher::startSunshine()
{
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
            qDebug() << "Unknown host state" << endl;
    }
}

void Launcher::appStart() {
    qDebug() << "Start application" << endl;
    QString app_dir = QCoreApplication::applicationDirPath();
    connect(proc, &QProcess::readyReadStandardOutput, this, &Launcher::updateAppConsole);
    connect(proc, &QProcess::readyRead, this, &Launcher::updateAppConsole);
    connect(proc, &QProcess::readyReadStandardError, this, &Launcher::updateAppConsoleError);
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, &Launcher::appStoppedWatch);
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Launcher::set_off_host_state_indicator);
    proc->open(QProcess::Unbuffered);
    proc->start(app_dir + "/openstreamhost/openstreamhost.exe", QStringList() << *SUNSHINE_CONF);
    this->set_on_host_state_indicator();
    qDebug() << "Application started" << endl;
}

void Launcher::appStarting() {
    qDebug() << "A sunshine host is currently starting" << endl;
    QMessageBox::information(this, "Sunshine Launcher",
                             tr("A sunshine host is currently starting"));
}

void Launcher::appRunning() {
    qDebug() << "A sunshine host was already started" << endl;
    QMessageBox::information(this, "Sunshine Launcher",
                             tr("A sunshine host was already started"));
}

void Launcher::updateAppConsole()
{
    QByteArray strData = proc->readAllStandardOutput();
    consoleOutput->setTextColor(Qt::black);
    consoleOutput->moveCursor (QTextCursor::End);
    consoleOutput->insertPlainText(strData);

}

void Launcher::updateAppConsoleError()
{
    QByteArray strData = proc->readAllStandardError();
    consoleOutput->setTextColor(Qt::black);
    consoleOutput->append(strData);
}

void Launcher::appStoppedWatch() {
    qDebug() << "Streamming host was stopped" << endl;
}

void Launcher::stopHostBeforeClose() {
    this->stopSunshine();
}

void Launcher::stopSunshine()
{
    qDebug() << "Stoping sunshine process...";
    qint64 pid = proc->processId();
    if(pid > 0) {
        //TODO: research use of kill vs terminate
        proc->kill();
        //Allocate new process
        delete proc;
        allocateNewProcess();
        qDebug() << "Process host stopped " << pid << endl;
    }
    else {
        qDebug() << "Process currently stopped.";
    }
}

void Launcher::createMainGroupBox()
{
    controlVBoxLayout = new QVBoxLayout;
    startSunshineButton = new QPushButton(tr("Start"));
    startSunshineButton->setDefault(true);
    startSunshineButton->setMaximumWidth(this->width() / 3);
    startSunshineButton->setMinimumWidth(this->width() / 3);


    stopSunshineButton = new QPushButton(tr("Stop"));
    stopSunshineButton->setMaximumWidth(this->width() / 3);
    stopSunshineButton->setMinimumWidth(this->width() / 3);

    configureSunshineButton = new QPushButton(tr("Configure"));
    configureSunshineButton->setMaximumWidth(this->width() / 3);
    configureSunshineButton->setMinimumWidth(this->width() / 3);

    host_state_indicator = new QLabel(STATE_STOPPED_MSG);

    consoleLoggerVBoxLayout = new QVBoxLayout;
    consoleOutput = new QTextEdit();
    consoleOutput->setReadOnly(true);

    vSpacer = new QSpacerItem(0, 10, QSizePolicy::Fixed);

    controlVBoxLayout->addWidget(startSunshineButton);
    controlVBoxLayout->addWidget(stopSunshineButton);
    controlVBoxLayout->addWidget(configureSunshineButton);
    controlVBoxLayout->addSpacerItem(vSpacer);
    controlVBoxLayout->addWidget(host_state_indicator);

    consoleLoggerVBoxLayout->addWidget(consoleOutput);
    controlVBoxLayout->setAlignment(Qt::AlignCenter);

    controlWidget = new QWidget;
    controlWidget->setLayout(controlVBoxLayout);

    loggerWidget = new QWidget;
    loggerWidget->setLayout(consoleLoggerVBoxLayout);

    mainWidget = new QTabWidget;
    mainWidget->addTab(controlWidget, tr("Control Panel"));
    mainWidget->addTab(loggerWidget, tr("Logs"));

    configDialog = new ConfigurationDialog(this);
    configDialog->setVisible(false);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mainWidget);

    setLayout(mainLayout);
}

void Launcher::createMinmalActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, this, &Launcher::stopHostBeforeClose);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void Launcher::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(STATE_STOPPED_MSG_TRAY);
}

void Launcher::allocateSharedMemoryFootprint() {
    sharedMemoryFootprint = new QSharedMemory(sharedMemoryFootprintKey, this);
    bool semaphore = sharedMemoryFootprint->create(4);
    if(semaphore) {
        qDebug() << "Starting new application. Not previous footprint found." << endl;
    }
    else {
        switch(sharedMemoryFootprint->error())
        {
            case QSharedMemory::InvalidSize:
                qDebug() << "Invalid footprint size" << endl;
                break;
           case QSharedMemory::QSharedMemory::KeyError:
                qDebug() << "Invalid footprint key" << endl;
                break;
           case QSharedMemory::AlreadyExists:
                qDebug() << "An instance of Sunshine Launcher is already running."
                         << "Closing current instance." << endl;
                sharedMemoryFootprintErrorMessage();
                break;
           default:
                qDebug() << "Unknown error found creating shared memory footprint" << endl;
        }

    }
}

void Launcher::sharedMemoryFootprintErrorMessage() {
    if(QMessageBox::Ok == QMessageBox::critical(this,
                                                tr("Application Error"),
                                                tr("Sunshine launcher is already executing.")))
    {
        exit(EXIT_FAILURE);
    }
}

void Launcher::resourcesStartup()
{
    /*Copy static sunshine.exe*/
    QFile exeInputFile(":/sunshine-windows/sunshine.exe" );
    if(!exeInputFile.open(QFile::ReadOnly))
    {
         qDebug() << "Could not open sunshine.exe in resources for read" << endl;
         return;
    }
    QFile exeOutFile(QString("sunshine.exe"));
    if(exeOutFile.exists())
    {
        qDebug() << "Sunshine.exe already found on folder" << endl;
    }
    else {
        QDataStream in(&exeInputFile);
        in.device()->reset();
        QByteArray mExe = in.device()->readAll();
        exeInputFile.close();

        QSaveFile newExeFile("sunshine.exe");
        newExeFile.open(QIODevice::WriteOnly);
        newExeFile.write(mExe);
        newExeFile.commit();
    }

    /*Copy default sunshine.conf*/
    if(!QDir("assets").exists())
    {
        QDir().mkdir("assets");
        copyStaticFile(QString(":sunshine-windows/assets/sunshine.conf"), QString("assets/sunshine.conf"));
        copyStaticFile(QString(":sunshine-windows/assets/apps_windows.json"), QString("assets/apps_windows.json"));
    }
    qDebug() << DEBUGFLAG << "resouces copied successfuly" << endl;
}


void Launcher::copyStaticFile(QString resourceName, QString filename)
{
    QFile outputFile(filename);
    outputFile.open(QIODevice::WriteOnly);
    QFile inputFile(resourceName);
    if(inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        QTextStream out(&outputFile);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            out << line << endl;
        }
    }
    inputFile.close();
    outputFile.close();
}

void Launcher::set_on_host_state_indicator() {
    host_state_indicator->setText(STATE_RUNNING_MSG);
    trayIcon->setIcon(*icon_on);
    trayIcon->setToolTip(STATE_RUNNING_MSG_TRAY);
}

void Launcher::set_off_host_state_indicator() {
    host_state_indicator->setText(STATE_STOPPED_MSG);
    trayIcon->setIcon(*icon_off);
    trayIcon->setToolTip(STATE_STOPPED_MSG_TRAY);
}

void Launcher::on_event_loop_started() {
    appStart();
}

void Launcher::configuration_changed_apply() {
    configDialog->setGUIPriority();
    if(proc->state() == QProcess::Running) {
        /*Pseudo-Restart host after config changes*/
        QMessageBox::information(this, "Info",
                                         tr("Configuration changed. Stream host will restart."));
        stopSunshine();
        startSunshine();
    }
}
