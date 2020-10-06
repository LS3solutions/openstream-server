#ifndef AUTHLISTENERWORKER_H
#define AUTHLISTENERWORKER_H

#include <QObject>
#include <QDebug>
#include <string>
#include <windows.h>

class AuthListenerWorker : public QObject
{
    Q_OBJECT
public:
    AuthListenerWorker(QObject *parent = nullptr);

public slots:
    void start_listening();

signals:
    void finished();
    void error();
    void auth_attempt();

};

#endif // AUTHLISTENERWORKER_H
