#include "auto_start.h"
#include <shlobj.h>
#include <stdexcept>
#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <string>
#include <cstring>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QProcess>

char* AUTO_START_SHORTCUT_FILENAME = "\\OpenStreamApp - Shortcut.lnk";

void old_auto_start_remove() {
    // Valid file path name (file is there).
    const char *lpStr1;
    std::wstring startup_folder_ws(get_startup_folder());
    std::string startup_folder_str(startup_folder_ws.begin(), startup_folder_ws.end());
    startup_folder_str.append(AUTO_START_SHORTCUT_FILENAME);
    std::string filepath_and_name = startup_folder_str.c_str();
    lpStr1 = filepath_and_name.c_str();
    QFileInfo check_file(lpStr1);

    if(check_file.exists() && check_file.isFile())
        QDir().remove(lpStr1);
    else
        return ;
}

/**
 * @brief get_startup_folder
 * @return the Windows startup folder in the machine.
 */
std::wstring get_startup_folder() {
    PWSTR pszPath;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Startup,
                                      0,
                                      NULL,
                                      &pszPath);
    if (SUCCEEDED(hr))
    {
        // The function succeeded, so copy the returned path to a
        // C++ string, free the memory allocated by the function,
        // and return the path string.
        std::wstring path(pszPath);
        CoTaskMemFree(static_cast<LPVOID>(pszPath));
        return path;
    }
    else
    {
        // The function failed, so handle the error.
        // ...
        // You might want to throw an exception, or just return an
        // empty string here.
        throw std::runtime_error("The SHGetKnownFolderPath function failed");
    }
}

void set_windows_auto_start(QString application_dir, QString application_file_name) {
    QProcess autostart_logon_pro;
    QString openstream_exe_file_path = application_dir + "/" + application_file_name;
    autostart_logon_pro.open(QProcess::Unbuffered);
    autostart_logon_pro.start(application_dir + "/utils/AutoLoginTaskSchedulerProject.exe",
                              QStringList() << openstream_exe_file_path << application_dir);
    //Block until the process finish.
    autostart_logon_pro.waitForFinished(-1);
    qDebug() << autostart_logon_pro.readAll();
}
