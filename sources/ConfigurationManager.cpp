#include "headers/ConfigurationManager.h"

ConfigurationManager::ConfigurationManager()
    : QObject()
{
    QHash<QString, QString> entries = QHash<QString, QString>();
    loadRootConfiguration();
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
}
