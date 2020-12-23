#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QGroupBox>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QMenu>
#include <QCoreApplication>
#include <QMessageBox>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QProcess>
#include <QTextEdit>
#include <QTabWidget>
#include <QSharedMemory>
#include <QDir>
#include <QSaveFile>
#include "ConfigurationDialog.h"
#include <QTimer>
#include <QtConcurrent/QtConcurrentRun>
#include <QFuture>
#include <QInputDialog>


#include "auth_listener_worker.h"
#include "auth_pin_handler.h"
#include "set_priority_class.h"

class Launcher : public QWidget
{
    Q_OBJECT

public:
    Launcher(QWidget *parent = nullptr);
    void setVisible(bool visible) override;

protected:
    void closeEvent(QCloseEvent *) override;

signals:
    /**
     * @brief auth_finished is emitted when the authentication pin dialog
     * has been closed.
     * It doesn't matter if the pairing was succesful or not. After the dialog
     * has been closed, the named pipe will be waiting again.
     */
    void auth_finished();

public slots:
    /**
     * @brief on_event_loop_started
     * Automatically starts the host for usability with
     * Auto start functionality when systems boot.
     */
    void on_event_loop_started();

    void configuration_changed_apply();
private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void startSunshine();
    void stopSunshine();
    void updateAppConsole();
    void updateAppConsoleError();
    void appStoppedWatch();
    void stopHostBeforeClose();
    void set_off_host_state_indicator();
    void set_on_host_state_indicator();

    /**
     * @brief authListenerWorkerRegeneration is intended to be
     * executed after destruction of auth_listener_thread.
     * It will regenerate the IPC thread after a new client
     * has tried to be paired.
     */
    void authListenerWorkerRegeneration();

    /**
     * @brief showAuthMessagePopUp activates a pop-up message
     * when IPC communication is received from the streamming host
     */
    void showAuthMessagePopUp();

    /**
     * @brief inputAuthPinCapture opens a dialog for capture of
     * the authentication pin challenge
     */
    void inputAuthPinCapture();

private:
    uint RUN = 0;
    int HEIGHT = 200;
    int WIDTH = 300;
    QString DEBUGFLAG = QString("[launcher] ");

    QString *SUNSHINE_CONF = new QString(QCoreApplication::applicationDirPath()+"/assets/sunshine.conf");

    QProcess *proc;
    QString sharedMemoryFootprintKey = "OPEN STREAM";

    /*Auth pin related*/
    AuthListenerWorker *auth_listener_worker;
    QThread *auth_listener_thread;
    void allocate_auth_listener();
    AuthPinHandler *auth_pin_handler;
    /*Auth pin related*/

    void resourcesStartup();
    void allocateNewProcess();

    void createMainGroupBox();
    void createMinmalActions();
    void createTrayIcon();

    void startSunshineApp();
    void appStart();
    void appStarting();
    void appRunning();

    QSharedMemory *sharedMemoryFootprint;
    void allocateSharedMemoryFootprint();
    void sharedMemoryFootprintErrorMessage();

    void copyStaticFile(QString resourceName,
                        QString fileName);

    QTabWidget *mainWidget;
    QWidget *controlWidget;
    QWidget *loggerWidget;

    QVBoxLayout *mainLayout;
    QVBoxLayout *controlVBoxLayout;
    QVBoxLayout *consoleLoggerVBoxLayout;
    QPushButton *startSunshineButton;
    QPushButton *stopSunshineButton;
    QPushButton *configureSunshineButton;

    QTextEdit *consoleOutput;

    /*State indicator*/
    QLabel *host_state_indicator;
    QSpacerItem *vSpacer;
    QString STATE_RUNNING_MSG = "Openstream host is Running";
    QString STATE_STOPPED_MSG = "Openstream host is Stopped";
    QString STATE_RUNNING_MSG_TRAY = "Openstream - Running";
    QString STATE_STOPPED_MSG_TRAY = "Openstream - Stopped";
    QIcon *icon_off;
    QIcon *icon_on;
    /*State indicator*/

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    ConfigurationDialog *configDialog;

};
#endif // LAUNCHER_H
