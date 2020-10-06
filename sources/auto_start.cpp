#include "headers/auto_start.h"
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

char* AUTO_START_SHORTCUT_FILENAME = "\\OpenStreamApp - Shortcut.lnk";

bool auto_start_exists() {
    // Valid file path name (file is there).
    const char *lpStr1;
    std::wstring startup_folder_ws(get_startup_folder());
    std::string startup_folder_str(startup_folder_ws.begin(), startup_folder_ws.end());
    startup_folder_str.append(AUTO_START_SHORTCUT_FILENAME);
    std::string filepath_and_name = startup_folder_str.c_str();
    lpStr1 = filepath_and_name.c_str();
    QFileInfo check_file(lpStr1);

    if(check_file.exists() && check_file.isFile())
        return true;
    else
        return false;
    delete lpStr1;
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

/**
 * @brief CreateLink creates a shortcut.
 * @param lpszPathObj Address of a buffer that contains the path of the object,
 *        including the file name.
 * @param lpszPathLink Address of a buffer that contains the path where the
 *        Shell link is to be stored, including the file name.
 * @param lpszDesc Address of a buffer that contains a description of the
 *        Shell link, stored in the Comment field of the link
 *        properties.
 * @return
 */
HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPath, LPCSTR lpszPathLink, LPCSTR lpszDesc)
{
    HRESULT hres = CoInitialize(NULL);
    IShellLink* psl;


    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);
        psl->SetWorkingDirectory(lpszPath);

        // Query IShellLink for the IPersistFile interface, used for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            WCHAR wsz[MAX_PATH];

            // Ensure that the string is Unicode.
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

            // Add code here to check return value from MultiByteWideChar
            // for success.

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(wsz, TRUE);
            ppf->Release();
        }
        psl->Release();
    }
    CoUninitialize();
    return hres;
}

void set_windows_auto_start(std::string application_dir, std::string application_file_name) {

    std::wstring startup_folder_ws(get_startup_folder());
    std::string startup_folder_str(startup_folder_ws.begin(), startup_folder_ws.end());
    startup_folder_str.append(AUTO_START_SHORTCUT_FILENAME); //.lnk extension for shortcut
    std::string application = application_dir + '\\' + application_file_name;
    std::string description = "Shortcut to " + application_file_name;
    qDebug() << "Creating shortcut: " << Qt::endl <<
                "App: " << application.c_str() << Qt::endl <<
                "Folder: " << startup_folder_str.c_str() << Qt::endl;
    CreateLink(application.c_str(), application_dir.c_str(), startup_folder_str.c_str(), description.c_str());
}
