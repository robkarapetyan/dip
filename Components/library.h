#ifndef LIBRARY_H
#define LIBRARY_H

#include <QJsonObject>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

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

    bool remove_component(const QString& component_name);
    bool add_component(const Component* component);

    void setJsonFormat();
    void setBinaryFormat();
private:
    QString libPath = "";
    QJsonDocument* lib_json_doc = nullptr;
    SaveFormat save_format = SaveFormat::Json;
private:
    QJsonObject serialize_component(const Component& component);
};

#endif // LIBRARY_H
