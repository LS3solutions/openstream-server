#include "ConfigurationManager.h"

ConfigurationManager::ConfigurationManager()
    : QObject()
{
    QHash<QString, QString> entries = QHash<QString, QString>();
    loadRootConfiguration();
}

void ConfigurationManager::loadRootConfiguration()
{
    QFile inputFile(configFile);
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
    QFile outputConf(configFile);
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
