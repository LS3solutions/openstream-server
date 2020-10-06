#ifndef AUTO_START_H
#define AUTO_START_H

#include <string>

std::wstring get_startup_folder();
bool auto_start_exists();
void set_windows_auto_start(std::string application_dir, std::string application_file_name);

#endif // AUTO_START_H
