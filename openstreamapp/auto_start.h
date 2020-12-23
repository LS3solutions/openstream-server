#ifndef AUTO_START_H
#define AUTO_START_H

#include <QString>

std::wstring get_startup_folder();
void old_auto_start_remove();
void set_windows_auto_start(QString application_dir, QString application_file_name);

#endif // AUTO_START_H
