#include "auth_pin_handler.h"

AuthPinHandler::AuthPinHandler()
{
    network_manager = new QNetworkAccessManager();
}

AuthPinHandler::~AuthPinHandler() {
    delete network_manager;
}

int AuthPinHandler::send_host_pin(QString pin) {
    QString url = "http://localhost:47989/pin/" + pin;
    QNetworkRequest req = QNetworkRequest(url);
    network_manager->get(req);
    connect(network_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

void AuthPinHandler::replyFinished(QNetworkReply *response) {
    if (response->error() == QNetworkReply::NoError)
          {
              // Reading the data from the response
              QVariant statusCode = response->attribute(QNetworkRequest::HttpStatusCodeAttribute);
              qDebug() << "Status code from auth: " << statusCode << Qt::endl;
              int status = statusCode.toInt();
              switch (status) {
                case 200:
                  qDebug() << "Pin auth succesful." << Qt::endl;
                  break;
                default:
                  qDebug() << "Unknown response from streamming host" << Qt::endl;
              }
          }
    else {
        qDebug() << "Error sending auth pin to streamming host" << Qt::endl;
    }
}
