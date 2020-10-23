#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QFormLayout>
#include <QWidget>
#include "ConfigurationManager.h"
#include <QDebug>
#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QRegExp>

class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    ConfigurationDialog(QWidget*parent = nullptr);

public slots:
    void editConfigurationClicked();
    void accept() override;
    void reject() override;

private:
    ConfigurationManager *config;
    QFormLayout *configInputForm;
    void updateNewConfiguration();

    QDialogButtonBox *buttonBox;
    void cleaning();

    QLineEdit *minThreadsFieldLineEdit;
    QString ENCODER_LABEL = QString("Encoder");
    QString MIN_THREADS_LABEL = QString("Threads");
};

#endif // CONFIGURATIONDIALOG_H
