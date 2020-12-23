#include <QApplication>
#include <QCoreApplication>
#ifndef QT_NO_SYSTEMTRAYICON
#include "launcher.h"
#include <QMessageBox>
#include <QGuiApplication>
#include <QStyleHints>
#include <QDesktopWidget>
#include "auto_start.h"
#include "openstreammainwindow.h"
#include <QStatusBar>
#include <QFontDatabase>
#include <QTimer>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(openstreamapp);

    /**
      * This code deals with the HIGH SCALE DPI
      * https://stackoverflow.com/questions/43904594/pyqt-adjusting-for-different-screen-resolution
      */
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    if(!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, "Sunshine Launcher",
                              QObject::tr("I couldn't detect any system tray on the system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);

    /**
      * This code deals with the Auto Star mechanism
      */
    old_auto_start_remove();
    QString app_dir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
    QString app_name = QCoreApplication::applicationName() + ".exe";
    set_windows_auto_start(app_dir, app_name);

    OpenstreamMainWindow w;

    /**
     * This Code deals with the Windows Size scaling
     */
    QDesktopWidget dw;
    int width=dw.width()*0.7;
    int height=dw.height()*0.8;
    w.resize(width, height);

    /**
      * This code disables the main widget borders and the setup bar in bottom border
      */
    w.centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    w.statusBar()->hide();

    /**
      * This code add the Fonts to the program.
      */
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-ExtraBold.ttf");
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-ExtraBoldItalic.ttf");
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-Italic.ttf");
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-Light.ttf");
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-SemiBold.ttf");
    QFontDatabase::addApplicationFont(":resources/fonts/OpenSans-SemiBoldItalic.ttf");
    w.show();

    QTimer::singleShot(1000 / 2, &w, &OpenstreamMainWindow::on_event_loop_started);
    return a.exec();

    /*Legacy code
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);

    Auto startup logic
    old_auto_start_remove();
    QString app_dir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
    QString app_name = QCoreApplication::applicationName() + ".exe";
    set_windows_auto_start(app_dir, app_name);
    Auto startup logic

    if(!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, "Sunshine Launcher",
                              QObject::tr("I couldn't detect any system tray on the system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    Launcher w;

    if (!QGuiApplication::styleHints()->showIsFullScreen() && !QGuiApplication::styleHints()->showIsMaximized()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(&w);
        w.resize(availableGeometry.width() / 3, availableGeometry.height() * 2 / 3);
        w.setMaximumSize(availableGeometry.width() / 3, availableGeometry.height() * 1 / 3);
        w.setMinimumSize(availableGeometry.width() / 3, availableGeometry.height() * 1 / 3);
        w.move((availableGeometry.width() - w.width()) / 2,
                    (availableGeometry.height() - w.height()) / 2);
    }
    w.show();
    QTimer::singleShot(1000 / 2, &w, &Launcher::on_event_loop_started);
    return a.exec();*/
}

#else

#include <QLabel>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString text("QSystemTrayIcon is not supportd on this platform");

    QLabel *label = new QLabel(text);
    label->setWordWrap(true);
    label->show();
    qDebug() << text;
    app.exec();
}

#endif
