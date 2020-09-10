#pragma once

#include <QObject>
#include "ConfigManagerInterface.h"

class ConfigManager : public QObject, public ConfigSettingInterface
{
    Q_OBJECT

public:
    ConfigManager(QObject *parent = nullptr);
    virtual ~ConfigManager();

    virtual bool WriteValue(const QString& path, const QString& group, const QString& user_key, const QVariant &value);
    virtual int ReadInt(const QString& path, const QString& group, const QString& user_key, const int& defaultValue);
    virtual QString ReadString(const QString& path, const QString& group, const QString& user_key, const QString& defaultValue);

};
