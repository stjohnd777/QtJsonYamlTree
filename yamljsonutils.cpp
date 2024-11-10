#include "yamljsonutils.h"
#include <iostream>
#include <fstream>

// Convert YAML file to JSON file
void JsonYamlConversions::ConvertYamlFileToJsonFile(const std::string& yamlFilePath, const std::string& jsonFilePath) {
    try {
        YAML::Node yamlNode = YAML::LoadFile(yamlFilePath);
        nlohmann::json jsonObj = ConvertYamlCppToNlohmannJson(yamlNode);

        std::ofstream jsonFile(jsonFilePath);
        if (!jsonFile.is_open()) {
            throw std::ios_base::failure("Failed to open the JSON output file.");
        }

        jsonFile << jsonObj.dump(4);  // Pretty-print with 4-space indentation
        jsonFile.close();
        std::cout << "YAML to JSON conversion successful!\n";
    } catch (const std::exception& e) {
        std::cerr << "Error during YAML to JSON conversion: " << e.what() << '\n';
    }
}

// Convert YAML::Node to nlohmann::json
nlohmann::json JsonYamlConversions::ConvertYamlCppToNlohmannJson(const YAML::Node yamlNode) {
    nlohmann::json jsonObj = nlohmann::json::object();
    for (auto it = yamlNode.begin(); it != yamlNode.end(); ++it) {
        jsonObj[it->first.as<std::string>()] = it->second.as<std::string>();
    }
    return jsonObj;
}

// Convert JSON file to YAML file
void JsonYamlConversions::ConvertJsonFileToYamlFile(const std::string& jsonFilePath, const std::string& yamlFilePath) {
    try {
        std::ifstream jsonFile(jsonFilePath);
        if (!jsonFile.is_open()) {
            throw std::ios_base::failure("Failed to open the JSON input file.");
        }

        nlohmann::json jsonObj;
        jsonFile >> jsonObj;

        YAML::Node yamlNode = ConvertNlohmannJsonToYamlCpp(jsonObj);

        std::ofstream yamlFile(yamlFilePath);
        if (!yamlFile.is_open()) {
            throw std::ios_base::failure("Failed to open the YAML output file.");
        }

        yamlFile << yamlNode;
        yamlFile.close();
        std::cout << "JSON to YAML conversion successful!\n";
    } catch (const std::exception& e) {
        std::cerr << "Error during JSON to YAML conversion: " << e.what() << '\n';
    }
}

// Convert nlohmann::json to YAML::Node
YAML::Node JsonYamlConversions::ConvertNlohmannJsonToYamlCpp(const nlohmann::json jsonObj) {
    YAML::Node yamlNode;
    for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it) {
        yamlNode[it.key()] = it.value().get<std::string>();
    }
    return yamlNode;
}

// Convert nlohmann::json to QJsonObject
QJsonObject JsonYamlConversions::ConvertNlohmannJsonToQJson(const nlohmann::json& j) {
    QJsonObject jsonObject;
    for (auto it = j.begin(); it != j.end(); ++it) {
        QString key = QString::fromStdString(it.key());
        if (it->is_object()) {
            jsonObject.insert(key, ConvertNlohmannJsonToQJson(it.value()));
        } else if (it->is_array()) {
            QJsonArray jsonArray;
            for (const auto& element : *it) {
                if (element.is_object() || element.is_array()) {
                    jsonArray.append(ConvertNlohmannJsonToQJson(element));
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

// Convert YAML node to QJsonObject
QJsonObject JsonYamlConversions::ConvertYamlCppToQJson(const YAML::Node& yamlNode) {
    return ConvertNlohmannJsonToQJson(ConvertYamlCppToNlohmannJson(yamlNode));
}