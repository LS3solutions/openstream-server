#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QList>
#include <QPair>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSaveFile>
#include <QCoreApplication>

class ConfigurationManager : public QObject
{
    Q_OBJECT
public:
    ConfigurationManager();
    void setEntry(QString entry, QString value);
    void saveConfiguration();
    QString getKey(QString key);
private:

    const QString configFile = QString(QCoreApplication::applicationDirPath() + "/assets/sunshine.conf");

    /**
     * @brief loadRootConfiguration places the template configuration
     * for sunshine when the application start from resources into a
     * working file.
     */
    void loadRootConfiguration();

    /**
     * @brief saveConfiguration saves configuration in the working file.
     */

    QHash<QString, QString> entries;
};

#endif // CONFIGURATIONMANAGER_H
