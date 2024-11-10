// JsonYamlConversions.h

#pragma once

#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class JsonYamlConversions {
public:
    // Convert YAML file to JSON file
    static void ConvertYamlFileToJsonFile(const std::string& yamlFilePath, const std::string& jsonFilePath);
    
    // Convert YAML::Node to nlohmann::json
    static nlohmann::json ConvertYamlCppToNlohmannJson(const YAML::Node yamlNode);

    // Convert JSON file to YAML file
    static void ConvertJsonFileToYamlFile(const std::string& jsonFilePath, const std::string& yamlFilePath);
    
    // Convert nlohmann::json to YAML::Node
    static YAML::Node ConvertNlohmannJsonToYamlCpp(const nlohmann::json jsonObj);

    // Convert nlohmann::json to QJsonObject
    static QJsonObject ConvertNlohmannJsonToQJson(const nlohmann::json& j);

    // Convert YAML node to QJsonObject
    static QJsonObject ConvertYamlCppToQJson(const YAML::Node& yamlNode);
};