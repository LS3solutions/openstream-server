#ifndef OPENSTREAMMAINWINDOW_H
#define OPENSTREAMMAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QSharedMemory>
#include <QProcess>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QInputDialog>

#include "auth_listener_worker.h"
#include "auth_pin_handler.h"
#include "h265configurationdialog.h"
#include "h264cpuconfigurationdialog.h"
#include "h264nvencconfigurationdialog.h"
#include "h265nvencconfigurationdialog.h"
#include "h264amdamfconfigurationdialog.h"
#include "h265amdamfconfigurationdialog.h"
#include "VideoEncoderEnum.h"

namespace Ui {
class OpenstreamMainWindow;
}

class OpenstreamMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenstreamMainWindow(QWidget *parent = nullptr);
    void setVisible(bool visible) override;
    ~OpenstreamMainWindow();

public slots:
    /**
     * @brief on_event_loop_started
     * Starts the openstreamhost when the GUI has been started.
     */
    void on_event_loop_started();

protected:
    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    /**
     * @brief auth_finished is emitted when the authentication pin dialog
     * has been closed.
     * It doesn't matter if the pairing was succesful or not. After the dialog
     * has been closed, the named pipe will be waiting again
     * for new pairing processes.
     */
    void auth_finished();

public slots:
    /**
     * @brief configuration_changed_apply
     * This slot is connected to configuration signals,
     * to auto restart the host if configuration applies.
     */
    void configuration_changed_apply(VideoEncoder);

private slots:
    void on_h265_AMD_select_button_clicked();

private slots:
    void on_h264_AMD_select_button_clicked();

private slots:
    void on_h265_NVENC_configure_button_clicked();

private slots:
    void on_h265_NVENC_select_button_clicked();

private slots:
    void on_h264_NVENC_select_button_clicked();

private slots:
    void on_h264_NVENC_configure_button_clicked();

private slots:
    void on_h264_CPU_select_button_clicked();

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

    void on_configure_button_clicked();
    void on_h265_CPU_configure_button_clicked();
    void on_h264_CPU_configuration_button_clicked();
    void on_go_back_config_button_clicked();
    void on_h265_CPU_select_button_clicked();

private:
    Ui::OpenstreamMainWindow *ui;
    QString *SUNSHINE_CONF = new QString("/assets/sunshine.conf");
    QString H265_CONF = QString("/assets/h265CPU.conf");
    QString H264_CONF = QString("/assets/h264CPU.conf");
    QString H264_NVENC_CONF = QString("/assets/h264NVENC.conf");
    QString H265_NVENC_CONF = QString("/assets/h265NVENC.conf");
    QString H264_AMD_AMF_CONF = QString("/assets/h264AMDAMF.conf");
    QString H265_AMD_AMF_CONF = QString("/assets/h265AMDAMF.conf");

    /*Holds the state of which encoder is selected*/
    VideoEncoder current_encoder;


    //QProces to handle the Sunshine underneath binary executable.
    QProcess *proc;
    void allocateNewProcess();

    /*Private API for start/stop*/
    void appStart();
    void appStarting();
    void appRunning();

    /*Shared Memory Footprint functionality*/
    /*/**
     * is used to keep only
     * one instance of the program in the computer.
     */
    QString sharedMemoryFootprintKey = "OPEN STREAM";
    QSharedMemory *sharedMemoryFootprint;
    void allocateSharedMemoryFootprint();
    void sharedMemoryFootprintErrorMessage();

    /*TrayIcon functionality*/
    void createMainGroupBox();
    void createMinimalActions();
    void createTrayIcon();

    /*State indicator*/
    QString STATE_RUNNING_MSG = "Server Up";
    QString STATE_STOPPED_MSG = "Server Down";
    QString STATE_RUNNING_MSG_TRAY = "Openstream - Running";
    QString STATE_STOPPED_MSG_TRAY = "Openstream - Stopped";
    QIcon *icon_off;
    QIcon *icon_on;
    /*State indicator*/

    /*Tray app related*/
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    /*Auth pin related*/
    AuthListenerWorker *auth_listener_worker;
    QThread *auth_listener_thread;
    void allocate_auth_listener();
    AuthPinHandler *auth_pin_handler;

    /*Configuration Dialogs*/
    h265ConfigurationDialog *h265CPUConfigDialog;
    h264CPUConfigurationDialog *h264CPUConfigDialog;
    h264NVENCConfigurationDialog *h264NVENCConfigDialog;
    h265NVENCConfigurationDialog *h265NVENCConfigDialog;
    h264AMDAMFConfigurationDialog *h264AMDAMFConfigDialog;
    h265AMDAMFConfigurationDialog *h265AMDAMFConfigDialog;

    /*Read/write encoder configuration*/
    void readEncoderConfiguration();
    void writeEncoderConfiguration();

    void updateEncoderButtonsSelected();
    void removeIconsFromSelectionButtons();
};

#endif // OPENSTREAMMAINWINDOW_H
