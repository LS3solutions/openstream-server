#include "set_priority_class.h"
#include <QDebug>

void set_host_process_priority_gui(int sys_priority) {
    DWORD dwError, dwPriClass;

    bool priority_set = false;

    switch (sys_priority) {
        case 1:
            priority_set = SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
            break;
        case 2:
            priority_set = SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
            break;
        case 3:
            priority_set = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
            break;
        default:
            priority_set = SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
            break;
    }

    if(!priority_set)
    {
      dwError = GetLastError();
      qDebug() << "Failed to set process priority (%d)\n" << Qt::endl;
      return;
    }
    // This two lines logs the streaming host priority.
    dwPriClass = GetPriorityClass(GetCurrentProcess());
    qDebug() << "Priority class successfuly set" << Qt::endl;
}
