#include "jsontree.h"

#include <iostream>
#include <fstream>


QString JsonTree::getFilePathToLoad(const QString& filter) {
    return QFileDialog::getOpenFileName(this, tr("Open File"), QString::fromStdString(jsonPath), filter);
}

QString JsonTree::getFilePathToSave(const QString& filter) {
    return QFileDialog::getSaveFileName(this, tr("Save File"), QString::fromStdString(jsonPath), filter);
}

JsonTree::JsonTree(std::string jsonPath ,QWidget *parent) : QWidget(parent) {

    this->jsonPath = jsonPath;

    initGUI();

    connect(loadButtonJson, &QPushButton::clicked, this, static_cast<void (JsonTree::*)()>(&JsonTree::loadJson));
    connect(saveButtonJson, &QPushButton::clicked, this, static_cast<void (JsonTree::*)()>(&JsonTree::saveJson));

    connect(loadButtonYaml, &QPushButton::clicked, this, static_cast<void (JsonTree::*)()>(&JsonTree::loadYaml));
    connect(saveButtonYaml, &QPushButton::clicked, this, static_cast<void (JsonTree::*)()>(&JsonTree::saveYaml));


    connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &JsonTree::editItem);

    loadJson(QString::fromStdString(jsonPath)); // Load JSON data on startup

    update();
}

void JsonTree::initGUI(){

    QVBoxLayout *layoutV = new QVBoxLayout(this);
    treeWidget = new QTreeWidget(this);
    treeWidget->setHeaderLabels(QStringList() << "Key" << "Value");
    layoutV->addWidget(treeWidget);

    QHBoxLayout *layoutH = new QHBoxLayout(this);

    loadButtonJson = new QPushButton("Load JSON", this);
    loadButtonJson->setIcon(QIcon::fromTheme("document-open"));
    layoutH->addWidget(loadButtonJson);

    saveButtonJson = new QPushButton("Save JSON", this);
    saveButtonJson->setIcon(QIcon::fromTheme("document-save"));
    layoutH->addWidget(saveButtonJson);

    loadButtonYaml = new QPushButton("Load YAML", this);
    loadButtonYaml->setIcon(QIcon::fromTheme("document-open"));
    layoutH->addWidget(loadButtonYaml);

    saveButtonYaml = new QPushButton("Save YAML", this);
    saveButtonYaml->setIcon(QIcon::fromTheme("document-save"));
    layoutH->addWidget(saveButtonYaml);

    QFrame* frame = new QFrame(this);
    frame->setLayout(layoutH);
    layoutV->addWidget(frame);


    connect(loadButtonJson, &QPushButton::clicked, this, static_cast<void (JsonTree::*)()>(&JsonTree::loadJson));
    connect(saveButtonJson, &QPushButton::clicked, this, static_cast<void (JsonTree::*)()>(&JsonTree::saveJson));
    connect(loadButtonYaml, &QPushButton::clicked, this, static_cast<void (JsonTree::*)()>(&JsonTree::loadYaml));
    connect(saveButtonYaml, &QPushButton::clicked, this, static_cast<void (JsonTree::*)()>(&JsonTree::saveYaml));
    connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &JsonTree::editItem);

}


void JsonTree::loadJson( QString jsonFilePath) {

    QFile file(jsonFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open the JSON file.");
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(data));
    treeWidget->clear();
    parseJson(doc.object(), nullptr);
    treeWidget->update();
}

void JsonTree::saveJson(QString jsonPath) {

    QJsonObject rootObj;
    buildJson(rootObj, treeWidget->invisibleRootItem());

    QJsonDocument doc(rootObj);
    QFile file(jsonPath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't save to the JSON file.");
        return;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

void JsonTree::loadYaml( QString path) {
    YAML::Node yamlNode = YAML::LoadFile(path.toStdString());
    QJsonObject qJsonObject = YamlToQjson(yamlNode);
    treeWidget->clear();
    parseJson(qJsonObject, nullptr);
    treeWidget->update();
}

void JsonTree::saveYaml(QString path) {
    YAML::Node yamlNode;
    buildYaml(yamlNode, treeWidget->invisibleRootItem());

    std::ofstream yamlFile(path.toStdString());
    if (yamlFile.is_open()) {
        yamlFile << yamlNode;
        yamlFile.close();
        std::cout << "JSON to YAML conversion successful!\n";
    } else {
        throw std::ios_base::failure("Failed to open the YAML output file.");
    }
}


QJsonObject JsonTree::NlohmannToQJson( const nlohmann::json j ){

    QJsonObject jsonObject;

    // Iterate through each key-value pair in the nlohmann::json object
    for (auto it = j.begin(); it != j.end(); ++it) {
        QString key = QString::fromStdString(it.key());
        if (it->is_object()) {
            jsonObject.insert(key, NlohmannToQJson(it.value()));
        } else if (it->is_array()) {
            QJsonArray jsonArray;
            for (const auto& element : *it) {
                if (element.is_object()) {
                    jsonArray.append(NlohmannToQJson(element));
                } else if (element.is_array()) {
                    jsonArray.append(NlohmannToQJson(element));
                } else if (element.is_string()) {
                    jsonArray.append(QString::fromStdString(element.get<std::string>()));
                } else if (element.is_number_integer()) {
                    jsonArray.append(element.get<int>());
                } else if (element.is_number_unsigned()) {
                    jsonArray.append(static_cast<qint64>(element.get<unsigned int>()));
                } else if (element.is_number_float()) {
                    jsonArray.append(element.get<double>());
                } else if (element.is_boolean()) {
                    jsonArray.append(element.get<bool>());
                } else {
                    jsonArray.append(QJsonValue());
                }
            }
            jsonObject.insert(key, jsonArray);
        } else if (it->is_string()) {
            jsonObject.insert(key, QString::fromStdString(it->get<std::string>()));
        } else if (it->is_number_integer()) {
            jsonObject.insert(key, it->get<int>());
        } else if (it->is_number_unsigned()) {
            jsonObject.insert(key, static_cast<qint64>(it->get<unsigned int>()));
        } else if (it->is_number_float()) {
            jsonObject.insert(key, it->get<double>());
        } else if (it->is_boolean()) {
            jsonObject.insert(key, it->get<bool>());
        } else {
            jsonObject.insert(key, QJsonValue());
        }
    }

    return jsonObject;
}


QJsonObject JsonTree::YamlToQjson( const YAML::Node yamlNode ){
    // Convert the YAML node to JSON object
    nlohmann::json jsonObj = nlohmann::json::object();
    for (auto it = yamlNode.begin(); it != yamlNode.end(); ++it) {
        jsonObj[it->first.as<std::string>()] = it->second.as<std::string>();
    }
    return NlohmannToQJson(jsonObj);

}

QString elementToString(const QJsonValue &element) {
    if (element.isString()) return element.toString();
    if (element.isDouble()) return QString::number(element.toDouble());
    if (element.isBool()) return element.toBool() ? "true" : "false";
    if (element.isObject()) return QJsonDocument(element.toObject()).toJson(QJsonDocument::Compact);
    if (element.isArray()) return QJsonDocument(element.toArray()).toJson(QJsonDocument::Compact);
    return "null";
}

void JsonTree::parseJson(const QJsonObject &qJsonObject, QTreeWidgetItem *parent) {
    for (auto it = qJsonObject.begin(); it != qJsonObject.end(); ++it) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, it.key());
        item->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::GoDown));

        if (it->isObject()) {
            item->setText(1, "");
            if (parent) {
                parent->addChild(item);
            } else {
                treeWidget->addTopLevelItem(item);
            }
            parseJson(it->toObject(), item);
        } else if (it->isArray()) {
            QStringList arrayElements;
            QJsonArray array = it->toArray();
            for (const auto& element : array) {
                arrayElements.append(elementToString(element));
            }
            item->setText(1, "[" + arrayElements.join(", ") + "]");
            if (parent) {
                parent->addChild(item);
            } else {
                treeWidget->addTopLevelItem(item);
            }
        } else if (it->isString()) {
            item->setText(1, it->toString());
            if (parent) {
                parent->addChild(item);
            } else {
                treeWidget->addTopLevelItem(item);
            }
        } else if (it->isDouble()) {
            item->setText(1, QString::number(it->toDouble()));
            if (parent) {
                parent->addChild(item);
            } else {
                treeWidget->addTopLevelItem(item);
            }
        } else if (it->isBool()) {
            item->setText(1, it->toBool() ? "true" : "false");
            if (parent) {
                parent->addChild(item);
            } else {
                treeWidget->addTopLevelItem(item);
            }
        } else {
            item->setText(1, "null");
            if (parent) {
                parent->addChild(item);
            } else {
                treeWidget->addTopLevelItem(item);
            }
        }

        if (item->childCount() == 0) {
            item->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::GoNext));
            item->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::SyncError));
        }
    }
}

void JsonTree::buildJson(QJsonObject &json, QTreeWidgetItem *item) {
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *child = item->child(i);

        if (child->childCount() > 0) {
            QJsonObject childObj;
            buildJson(childObj, child);
            json.insert(child->text(0), childObj);
        } else {
            json.insert(child->text(0), child->text(1));
        }
    }
}

void JsonTree::buildYaml(YAML::Node& yaml, QTreeWidgetItem *item) {

    if (!item) return;

    // Iterate over the child items of the current item
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem *child = item->child(i);
        QString key = child->text(0);  // Assuming column 0 is the key
        QVariant value = child->data(1, Qt::DisplayRole);  // Assuming column 1 is the value

        // Check if the child itself has further children
        if (child->childCount() > 0) {
            YAML::Node subNode;
            buildYaml(subNode, child);
            yaml[key.toStdString()] = subNode;
        } else {
            // Convert value to a string and insert it into the YAML node
            yaml[key.toStdString()] = value.toString().toStdString();
        }
    }
}

// slots

void JsonTree::loadJson( ) {
    loadJson(getFilePathToLoad(tr("JSON Files (*.json)")));
}


void JsonTree::loadYaml(){
    loadYaml(getFilePathToLoad(tr("JSON Files (*.yaml)")));
}


void JsonTree::saveJson() {
    saveJson(getFilePathToSave(tr("JSON Files (*.json)")));
}


void JsonTree::saveYaml() {
    saveYaml(getFilePathToSave(tr("JSON Files (*.yaml)")));

}

void JsonTree::editItem(QTreeWidgetItem *item, int column) {
    if (item->childCount() == 0 && column == 1) { // Editable only if it's a leaf node
        bool ok;
        QString newValue = QInputDialog::getText(this, "Edit Value", "Enter new value:", QLineEdit::Normal, item->text(1), &ok);
        if (ok && !newValue.isEmpty()) {
            item->setText(1, newValue);
        }
    }
}

