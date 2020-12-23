#include "auth_listener_worker.h"

AuthListenerWorker::AuthListenerWorker(QObject *parent) : QObject(parent) {}

void AuthListenerWorker::start_listening()
{
    //All is created in the running thread.
    //Nothing is created in the GUI thread.
    //TODO: Handle pipe name as a constant. Watch out with types
    HANDLE pipe = CreateNamedPipe( TEXT("\\\\.\\pipe\\openstream_auth_pipe"),
                                   PIPE_ACCESS_INBOUND,
                                   PIPE_TYPE_MESSAGE,
                                   1,
                                   0,
                                   0,
                                   0,
                                   NULL
                             );
    if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
        qDebug() << "Failed to create outbound pipe instance." << Qt::endl;
        qDebug() << GetLastError() << Qt::endl;
        return;
    }
    qDebug() << "Listening for pipe client to connect." << Qt::endl;
    // This call blocks until a client process connects to the pipe
    BOOL result = ConnectNamedPipe(pipe, NULL);
    if (!result) {
        qDebug() << "Failed connecting pipe client process." << Qt::endl;
        CloseHandle(pipe);
        return;
    }
    qDebug() << "Reading pipe data from client" << Qt::endl;
    wchar_t buffer[128];
    DWORD numBytesRead = 0;
    result = ReadFile(
            pipe,
            buffer, // the data from the pipe will be put here
            127 * sizeof(wchar_t), // number of bytes allocated
            &numBytesRead, // this will store number of bytes actually read
            NULL // not using overlapped IO
    );

    if(result) {
        buffer[numBytesRead / sizeof(wchar_t)] = '\0';
        qDebug() << "Number of bytes read: " << numBytesRead << Qt::endl;
        qDebug() << "Message: " << buffer << Qt::endl;
    }
    else{
        qDebug() << "Failed reading client pipe data." << Qt::endl;
        CloseHandle(pipe);
        return;
    }

    CloseHandle(pipe);
    qDebug() << "Finished named pipe worker listening." << Qt::endl;
    emit auth_attempt();
    emit finished();
    return;
}
