#include "library.h"
#include <QJsonObject>

Library::Library()
{

}

void Library::init()
{

//    QFile saveFile(QStringLiteral("lib.json"));
//    if(saveFile.open(QIODevice::ReadOnly)){
//        qDebug() << "file opened";
//    }
    QFile saveFile(save_format == SaveFormat::Json
        ? QStringLiteral("lib.json")
        : QStringLiteral("lib.dat"));


    if(saveFile.exists()){
        if (!saveFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open lib file.");
            return;
        }
        libPath = save_format == SaveFormat::Json
                ? QStringLiteral("lib.json")
                : QStringLiteral("lib.dat");

        lib_json_doc = new QJsonDocument( QJsonDocument::fromJson(saveFile.readAll()));
        saveFile.close();
    }else{
//        qDebug() << " lib init , libpath set" << libPath;
        QFile saveFile("lib.json");

        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open lib file.");
            return;
        }
        QJsonObject testObject;
        QJsonObject headers;
        QJsonObject digital;
        digital["head"] = "none";
        digital["level"] = 0;
        QJsonObject analog;
        analog["head"] = "none";
        analog["level"] = 0;
        QJsonObject active;
        active["head"] = "analog";
        active["level"] = 1;
        QJsonObject passive;
        passive["head"] = "analog";
        passive["level"] = 1;
        QJsonObject sources;
        sources["head"] = "analog";
        sources["level"] = 1;


        headers["Digital"] = digital;
        headers["Analog"] = analog;
        headers["active"] = active;
        headers["passive"] = passive;
        headers["sources"] = sources;
        testObject["Headers"] = headers;
        lib_json_doc = new QJsonDocument(testObject);

        saveFile.write(save_format == SaveFormat::Json
            ? lib_json_doc->toJson()
            : lib_json_doc->toBinaryData());

        libPath = (save_format == SaveFormat::Json
                   ? QStringLiteral("lib.json")
                   : QStringLiteral("lib.dat"));
        saveFile.close();
    }

}

void Library::set_lib_path(const QString &path)
{
    libPath = path;
    QFile saveFile(path);

    if (!saveFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open lib file.");
        return;
    }
    lib_json_doc = new QJsonDocument( QJsonDocument::fromJson(saveFile.readAll()));

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
    for (int i = 0;i < pin_array.size() - 1; i+=2){
        comp->add_pin(pin_array[i].toDouble(), pin_array[i+1].toDouble());
    }
    comp->set_pixmap(obj_to_deserialise["icon"].toString());

    QJsonObject prop_obj = obj_to_deserialise["properties"].toObject();
    for( auto i : prop_obj.keys()){
        comp->setProperty(i.toStdString().c_str(), prop_obj[i].toVariant());
    }

    QJsonObject tri_state_properties = obj_to_deserialise["tri_states_properties"].toObject();

    for (auto i : tri_state_properties.keys()){
        if(tri_state_properties.contains(i + "_active")){
            QJsonArray tristatearray = tri_state_properties[i].toArray();
            QString statesString = "";

            for(auto j : tristatearray.toVariantList()){
                statesString += j.toString() + ",";
            }
            comp->tri_states_map.insert(i, statesString);
            comp->tri_states_map.insert(i + "_active", tri_state_properties[i + "_active"].toString());
        }
    }

    return comp;
}

QStringList Library::components(const QString &filter)
{
    if(!lib_json_doc){
        qWarning("No library file detected");
        return QList<QString>();
    }
    if(filter == "all"){
        QStringList lst = lib_json_doc->object().keys();
        lst.removeOne("Headers");
        return lst;
    }
    return QList<QString>();
}

QMultiMap<int, QPair<QString, QString> > Library::headers()
{
    QMultiMap<int, QPair<QString, QString> > headers_map = {};
    if(!lib_json_doc){
        qWarning("No library file detected");
        return QMultiMap<int, QPair<QString, QString>>();
    }
    QJsonObject head_obj = lib_json_doc->object()["Headers"].toObject();

    for (auto i : head_obj.keys()){
        headers_map.insert(head_obj[i].toObject()["level"].toInt(), QPair<QString, QString>(i, head_obj[i].toObject()["head"].toString()));
    }

    return headers_map;
}

bool Library::remove_component(const QString &component_name)
{
    if(!lib_json_doc){
        qWarning("No library file detected");
        return false;
    }

    //component removal
    qDebug() << "Libeditor::" << "removing' " << component_name << "' from " << libPath;
    QFile saveFile(libPath);
    if (!lib_json_doc->object().contains(component_name))
        qWarning() << "No such component named '" + component_name + "'.. in library";
    //rename instead of remove for undo/redo stuff
//    lib_json_doc->object().remove(component_name); //not working properly -_-
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

    qDebug() << "Libeditor::removed";
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

bool Library::add_header(const QString &name, const QString &parent = "")
{
    if(!lib_json_doc){
        qWarning("No library file detected");
        return false;
    }

    //header addition
    QJsonObject rootobj = lib_json_doc->object();
    QJsonObject headerobj = rootobj["Headers"].toObject();

    if(headerobj.keys().contains(name)){
        return false;
    }
    if(parent == ""){
        QJsonObject subRoot;
        subRoot["head"] = "none";
        subRoot["level"] = 0;

        headerobj[name] = subRoot;
    }else{

        if(rootobj["Headers"].toObject().keys().contains(parent)){

            QJsonObject subRoot;
            subRoot["head"] = parent;
            subRoot["level"] = rootobj["Headers"].toObject()
                    [parent].toObject()["level"].toInt() + 1;
            qDebug() << subRoot;
            qDebug() << headerobj << "before";

            headerobj[name] = subRoot;
            qDebug() << headerobj << "after";

        }
    }

    rootobj["Headers"] = headerobj;
    lib_json_doc->object() = rootobj;

    QFile saveFile(libPath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    lib_json_doc->setObject(rootobj);
    saveFile.write(save_format == SaveFormat::Json
        ? lib_json_doc->toJson()
        : lib_json_doc->toBinaryData());
    saveFile.close();
    return true;
}

bool Library::remove_header(const QString &name)
{
    //also delete all component of header's children
    if(!lib_json_doc){
        qWarning("No library file detected");
        return false;
    }

    //header removal
    QJsonObject rootobj = lib_json_doc->object();
    QJsonObject headersobj = rootobj["Headers"].toObject();
    if(!headersobj.keys().contains(name)){
        return false;
    }else{
        headersobj.remove(name);
    }


    QFile saveFile(libPath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    rootobj["Headers"] = headersobj;
    lib_json_doc->setObject(rootobj);

    saveFile.write(save_format == SaveFormat::Json
        ? lib_json_doc->toJson()
        : lib_json_doc->toBinaryData());
    saveFile.close();
    return true;
}

bool Library::rename_header(const QString &old_name, const QString &new_name)
{
    if(!lib_json_doc){
        qWarning("No library file detected");
        return false;
    }

    //header rename
    QJsonObject rootobj = lib_json_doc->object();
    QJsonObject headersobj = rootobj["Headers"].toObject();

    if(!headersobj.keys().contains(old_name)
            || new_name == ""){
        return false;
    }

    QJsonObject subRoot = headersobj[old_name].toObject();
    headersobj.remove(old_name);

    headersobj[new_name] = subRoot;

    rootobj["Headers"] = headersobj;

    //rename all old head element's parent with new head
    for (auto i : rootobj.keys()){
        if(i == "Headers"){
            continue;
        }
        QJsonObject a = rootobj[i].toObject();
        QJsonObject current = a["properties"].toObject();

        qDebug() << i;

        if(current["head"].toString() == old_name){
            qDebug() << "old- " << old_name << ", new- " << new_name;
            current["head"] = new_name;
        }

        a["properties"] = current;
        rootobj[i] = a;
    }
    //---------------------------------------------------

    lib_json_doc->setObject(rootobj);

    QFile saveFile(libPath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    lib_json_doc->setObject(rootobj);
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

QString Library::getLibPath() const
{
    return libPath;
}

void Library::update()
{
    QFile saveFile(libPath);

    if (!saveFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open lib file.");
        return;
    }
    lib_json_doc = new QJsonDocument( QJsonDocument::fromJson(saveFile.readAll()));

    saveFile.close();
}

QJsonObject Library::serialize_component(const Component& component)
{
    QJsonObject object;
    QJsonObject objectProperties;
    QJsonObject tristateProperties;
    QJsonArray pinArray;
    for(auto  i : component.vec_of_pins){
//        QJsonValue pin = i->pos().rx();
        pinArray.append(i->pos().rx());
//        pin = i->pos().ry();
        pinArray.append(i->pos().ry());
    }

    for(auto i : component.dynamicPropertyNames()){
//        if(component.property(i).toString().isEmpty()){

//        }else{
            objectProperties[i] = QJsonValue::fromVariant(component.property(i));
//        }
    }
    for (auto i : component.tri_states_map.keys()){
        if(component.tri_states_map.keys().contains(i + "_active")){
            QJsonArray tristateArray;
            QStringList tristatesList = component.tri_states_map[i].split(",");
            tristateArray = QJsonArray::fromStringList(tristatesList);
            tristateProperties[i] = tristateArray;
            tristateProperties[i + "_active"] = component.tri_states_map[i + "_active"];
        }
    }
    object["icon"] = component.icon_path();
    object["pins"] = pinArray;
    object["properties"] = objectProperties;
    object["tri_states_properties"] = tristateProperties;
//    component.property("aaa").to
    return object;
}

