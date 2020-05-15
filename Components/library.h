#ifndef LIBRARY_H
#define LIBRARY_H

#include <QJsonObject>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStringList>
#include <QMap>
#include "Components/tools/component.h"


enum class SaveFormat {
    Json, Binary
};

class Library
{
public:
    Library();

    void init();
    //to deserialize lib.json
    void set_lib_path(const QString& path);
    //to interract with command system
    Component* lib_has(const QString& component_name);
    QStringList components(const QString& filter);
    QMultiMap<int, QPair<QString, QString>> headers();

    bool remove_component(const QString& component_name);
    bool add_component(const Component* component);

    bool add_header(const QString& name, const QString& parent);
    bool remove_header(const QString& name);
    bool rename_header(const QString& old_name, const QString& new_name);
    void setJsonFormat();
    void setBinaryFormat();
    QString getLibPath() const;
    void update();

private:
    QString libPath = "";
    QJsonDocument* lib_json_doc = nullptr;
    SaveFormat save_format = SaveFormat::Json;
private:
    QJsonObject serialize_component(const Component& component);
};

#endif // LIBRARY_H
