#include "ConfigurationManager.h"

ConfigurationManager::ConfigurationManager(QString encoder_conf_filename)
    : QObject()
{
    QHash<QString, QString> entries = QHash<QString, QString>();
    CONFIG_FILE_PATH = QString(QCoreApplication::applicationDirPath() + encoder_conf_filename);
    loadRootConfiguration();
}

void ConfigurationManager::reloadInMemoryValues() {
    QFile inputFile(CONFIG_FILE_PATH);
    if(inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            if(line.startsWith("#") ||
                    line.startsWith("\n") ||
                    line.isEmpty())
                continue;

            auto key_val = line.split("=");
            entries.insert(key_val.first().trimmed(), key_val.last().trimmed());
            qDebug() << "Entrie added" << key_val.first().trimmed() << "="
                                           <<  key_val.last().trimmed() << endl;
        }
    }
    inputFile.close();
}

void ConfigurationManager::loadRootConfiguration()
{
    if(!folderAssetsExists()){
        QDir().mkdir(ASSETS_FOLDER_FILE_PATH);
        copyAssetsFiles();
    }

    else if(!configFileExists()) {
        copyAssetsFiles();
    }

    reloadInMemoryValues();
}

void ConfigurationManager::saveConfiguration()
{
    QFile outputConf(CONFIG_FILE_PATH);
    outputConf.open(QIODevice::WriteOnly);
    QHash<QString, QString>::iterator i;
    QTextStream outStream(&outputConf);
    for(i = entries.begin(); i != entries.end(); i++)
    {
        QString line = i.key() + "=" + i.value();
        outStream << line << endl;
    }
    outputConf.close();
    qDebug() << "Configuration saved" << endl;
}

void ConfigurationManager::setEntry(QString entry, QString value)
{
    this->entries.insert(entry, value);
    qDebug () << "[ConfigurationManager] configuration key updated: "
              << entry << "->" << value << endl;
}

void ConfigurationManager::removeEntry(QString entry) {
    if(this->entries.contains(entry))
        this->entries.remove(entry);
}

QString ConfigurationManager::getKey(QString key)
{
    return entries.value(key);
}

bool ConfigurationManager::configFileExists() {
    QFileInfo check_file(this->CONFIG_FILE_PATH);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

bool ConfigurationManager::folderAssetsExists() {
    QFileInfo check_file(this->ASSETS_FOLDER_FILE_PATH);
    if (check_file.exists() && check_file.isDir()) {
        return true;
    } else {
        return false;
    }
}

void ConfigurationManager::copyAssetsFiles() {
    QFile().copy(":assets/apps_linux.json", ASSETS_FOLDER_FILE_PATH + "/apps_linux.json");
    QFile().copy(":assets/apps_windows.json", ASSETS_FOLDER_FILE_PATH + "/apps_windows.json");
    QFile().copy(":assets/box.png", ASSETS_FOLDER_FILE_PATH + "/box.png");
    QFile().copy(":assets/sunshine.conf", ASSETS_FOLDER_FILE_PATH + "/sunshine.conf");
    QFile().copy(":assets/h265CPU.conf", ASSETS_FOLDER_FILE_PATH + "/h265CPU.conf");
    QFile().copy(":assets/h264CPU.conf", ASSETS_FOLDER_FILE_PATH + "/h264CPU.conf");
    QFile().copy(":assets/h264NVENC.conf", ASSETS_FOLDER_FILE_PATH + "/h264NVENC.conf");
    QFile().copy(":assets/h265NVENC.conf", ASSETS_FOLDER_FILE_PATH + "/h265NVENC.conf");
    QFile().copy(":assets/h264AMDAMF.conf", ASSETS_FOLDER_FILE_PATH + "/h264AMDAMF.conf");
    QFile().copy(":assets/h265AMDAMF.conf", ASSETS_FOLDER_FILE_PATH + "/h265AMDAMF.conf");
    QFile().copy(":assets/encoder.conf", ASSETS_FOLDER_FILE_PATH + "/encoder.conf");
    QFile::setPermissions(ASSETS_FOLDER_FILE_PATH + "/sunshine.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    QFile::setPermissions(ASSETS_FOLDER_FILE_PATH + "/h265CPU.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    QFile::setPermissions(ASSETS_FOLDER_FILE_PATH + "/h264CPU.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    QFile::setPermissions(ASSETS_FOLDER_FILE_PATH + "/h264NVENC.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    QFile::setPermissions(ASSETS_FOLDER_FILE_PATH + "/h265NVENC.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    QFile::setPermissions(ASSETS_FOLDER_FILE_PATH + "/h264AMDAMF.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    QFile::setPermissions(ASSETS_FOLDER_FILE_PATH + "/h265AMDAMF.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    QFile::setPermissions(ASSETS_FOLDER_FILE_PATH + "/encoder.conf", QFileDevice::ReadOwner|QFileDevice::WriteOwner);
}

void ConfigurationManager::restoreDefaultConfiguration(QString filename) {
    QFile().remove(ASSETS_FOLDER_FILE_PATH + filename);
    QFile().copy(QString(":assets") + filename, ASSETS_FOLDER_FILE_PATH + filename);
    QFile::setPermissions(ASSETS_FOLDER_FILE_PATH + filename, QFileDevice::ReadOwner|QFileDevice::WriteOwner);
    qDebug() << "Copied: " << QString(":assets") + filename << Qt::endl;
    qDebug() << "To: " << ASSETS_FOLDER_FILE_PATH + filename << Qt::endl;
}
