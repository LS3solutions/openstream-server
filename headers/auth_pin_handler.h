#ifndef AUTHPINHANDLER_H
#define AUTHPINHANDLER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

class AuthPinHandler : public QObject
{
    Q_OBJECT
public:
    AuthPinHandler();
    ~AuthPinHandler();

    int send_host_pin(QString);

public slots:
    void replyFinished(QNetworkReply* response);

private:
    QNetworkAccessManager *network_manager;


};

#endif // AUTHPINHANDLER_H
