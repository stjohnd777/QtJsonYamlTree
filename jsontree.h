#pragma once


#include <QTreeWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QTreeWidgetItem>
#include <QInputDialog>
#include <QIcon>

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

/*
|central -------------------------|
|                                 |
|VBoxLayout ----------------------|
|QTreeView                        |
|---------------------------------|
|VBoxLayout | btn | btn | btn | bt|
|---------------------------------|
|                                 |
|---------------------------------|
 */
class JsonTree : public QWidget {
    Q_OBJECT
public:
    JsonTree(std::string jsonPath ,QWidget *parent = nullptr);
protected:
    void initGUI();
    void loadJson(QString jsonFilePath);
    void saveJson(QString jsonFilePath) ;
    void loadYaml(QString yamlFilePath);
    void saveYaml(QString yamlFilePath);
private slots:
    // display file chooser then pass to loadJson(file)
    void loadJson() ;
    // display file chooser then pass to loadYaml(file)
    void loadYaml();
    // display file chooser then pass to saveJson(file)
    void saveJson() ;
    // display file chooser then pass to saveYaml(file)
    void saveYaml() ;
    // edit leaves in column 2
    void editItem(QTreeWidgetItem *item, int column);
private:
    std::string jsonPath ;
    QTreeWidget *treeWidget;
    QPushButton *loadButtonJson ;
    QPushButton *saveButtonJson ;
    QPushButton *loadButtonYaml;
    QPushButton *saveButtonYaml ;

    // can remove and use utils
    QJsonObject NlohmannToQJson( const nlohmann::json j );
    // can remove and use utils
    QJsonObject YamlToQjson( const YAML::Node yamlNode );

    // parse into UI tree
    // yaml converted to json
    void parseJson(const QJsonObject &qJsonObject, QTreeWidgetItem *parent) ;

    // pull QJsonObject from UI tree
    void buildJson(QJsonObject &json, QTreeWidgetItem *item) ;
    // pull YAML from UI tree
    void buildYaml(YAML::Node &yaml, QTreeWidgetItem *item) ;


    QString getFilePathToLoad(const QString& filter) ;
    QString getFilePathToSave(const QString& filter);
};
