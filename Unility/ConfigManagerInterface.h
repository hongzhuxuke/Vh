#pragma once
#include <QVariant>
#include <QString>

class ConfigSettingInterface {
public:
    ConfigSettingInterface() {};
    virtual ~ConfigSettingInterface() {};
public:
    virtual bool WriteValue(const QString& path, const QString& group, const QString& user_key, const QVariant &value) = 0;
    virtual int ReadInt(const QString& path, const QString& group, const QString& user_key, const int& defaultValue) = 0;
    virtual QString ReadString(const QString& path, const QString& group, const QString& user_key, const QString& defaultValue) = 0;
};