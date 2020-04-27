#include "library.h"

Library::Library()
{

}

void Library::init()
{
    QFile saveFile(save_format == SaveFormat::Json
        ? QStringLiteral("lib.json")
        : QStringLiteral("lib.dat"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open lib file.");
        return;
    }
    QJsonObject testObject;

    lib_json_doc = new QJsonDocument(testObject);
    saveFile.write(save_format == SaveFormat::Json
        ? lib_json_doc->toJson()
        : lib_json_doc->toBinaryData());

    libPath = (save_format == SaveFormat::Json
               ? QStringLiteral("lib.json")
               : QStringLiteral("lib.dat"));
    saveFile.close();
}

void Library::set_lib_path(const QString &path)
{
    libPath = path;
    QFile saveFile(path);

    if (!saveFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open lib file.");
        return;
    }
    saveFile.close();
}

//returns pointer of component object if library has object named "component_name"
//nullptr otherwise
Component *Library::lib_has(const QString &component_name)
{
    if(!lib_json_doc){
        return nullptr;
    }
    if (!lib_json_doc->object().contains(component_name)){
        return nullptr;
    }

    QJsonObject obj_to_deserialise = lib_json_doc->object()[component_name].toObject();
    Component* comp = new Component;
    comp->setObjectName(component_name);

    QJsonArray pin_array = obj_to_deserialise["pins"].toArray();
    for (int i = 0;i < pin_array.size() - 1; ++i){
        comp->add_pin(pin_array[i].toDouble(), pin_array[i+1].toDouble());
    }
    //dynamic properties deserialization???
    //{

    //}

    return comp;
}

bool Library::remove_component(const QString &component_name)
{
    if(!lib_json_doc){
        qWarning("No library file detected");
        return false;
    }

    //component removal
    {
        QFile saveFile(libPath);
        if (!lib_json_doc->object().contains(component_name))
            qWarning() << "No such component named '" + component_name + "' in library";
        //rename instead of remove for undo/redo stuff
//        lib_json_doc->object().remove(component_name); //not working properly -_-
        QJsonObject testobj = lib_json_doc->object();
        testobj.remove(component_name);

        lib_json_doc->setObject(testobj);
        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
            return false;
        }
        saveFile.write(save_format == SaveFormat::Json
            ? lib_json_doc->toJson()
            : lib_json_doc->toBinaryData());
        saveFile.close();
    }
    return true;
}

bool Library::add_component(const Component *com)
{
    if(!lib_json_doc){
        qWarning("No library file detected");
        return false;
    }

    //component addition
    QJsonObject testobj = lib_json_doc->object();

    testobj[com->objectName()] = serialize_component(*com);

    QFile saveFile(libPath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    lib_json_doc->setObject(testobj);
    saveFile.write(save_format == SaveFormat::Json
        ? lib_json_doc->toJson()
        : lib_json_doc->toBinaryData());
    saveFile.close();
    return true;
}

void Library::setJsonFormat()
{
    save_format = SaveFormat::Json;
}

void Library::setBinaryFormat()
{
    save_format = SaveFormat::Binary;
}

QJsonObject Library::serialize_component(const Component& component)
{
    QJsonObject object;
    QJsonObject objectProperties;
    QJsonArray pinArray;
    for(auto  i : component.vec_of_pins){
//        QJsonValue pin = i->pos().rx();
        pinArray.append(i->pos().rx());
//        pin = i->pos().ry();
        pinArray.append(i->pos().ry());
    }

    for(auto i : component.dynamicPropertyNames()){
        objectProperties[i] = QJsonValue::fromVariant(component.property(i));
    }
    object["icon"] = component.icon_path();
    object["pins"] = pinArray;
    object["properties"] = objectProperties;

    return object;
}
