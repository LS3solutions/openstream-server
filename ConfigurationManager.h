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
#include <QFileInfo>
#include <QDir>

class ConfigurationManager : public QObject
{
    Q_OBJECT
public:
    ConfigurationManager();
    void setEntry(QString entry, QString value);
    void saveConfiguration();
    QString getKey(QString key);
private:

    const QString CONFIG_FILE_PATH = QString(QCoreApplication::applicationDirPath() + "/assets/sunshine.conf");
    const QString ASSETS_FOLDER_FILE_PATH = QString(QCoreApplication::applicationDirPath() + "/assets");

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

    /**
     * @brief checks if the configuration file exists in assets folder.
     * Useful for prevent runtime errors if the assets folder hasn't being copied in
     * when the installer was built.
     */
    bool configFileExists();

    /**
     * @brief checks if the assets folder exists. Useful for prevent runtime errors
     * if the assets folder hasn't being copied in when the installer was built.
     */
    bool folderAssetsExists();

    void copyAssetsFiles();
};

#endif // CONFIGURATIONMANAGER_H
