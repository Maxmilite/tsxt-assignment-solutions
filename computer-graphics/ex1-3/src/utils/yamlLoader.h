#ifndef YAML_LOADER_H__
#define YAML_LOADER_H__

#include <string>
#include <yaml-cpp/yaml.h>

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

class YAMLLoader {
public:
    YAMLLoader(const std::string& filePath) {
        load(filePath);
    }
    YAMLLoader() = default;

    void load(const std::string& filePath) {
        try {
            config = YAML::LoadFile(filePath);
        } catch (const YAML::Exception& e) {
            throw std::runtime_error("Failed to load config file: " + std::string(e.what()));
        }
    }

    template<typename T>
    T get(const std::string& key) const {
        auto split = [](const std::string& s, char delim) -> std::vector<std::string> {
            std::vector<std::string> result;
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                result.push_back(item);
            }
            return result;
        };
        auto keys = split(key, '.');
        YAML::Node node = YAML::Clone(config);
        for (const auto& k : keys) {
            node = node[k];
        }
        try {
            return node.as<T>();
        } catch (const YAML::Exception& e) {
            throw std::runtime_error("Failed to get key: " + key + " - " + std::string(e.what()));
        }
    }

    template<typename T>
    T get(const std::string& key, const T& defaultValue) const {
        try {
            return get<T>(key);
        } catch (const std::exception& e) {
            spdlog::debug(e.what());
            return defaultValue;
        }
    }

    glm::vec3 getVec3(const std::string& key) const {
        auto vec = get<std::vector<float>>(key);
        return glm::vec3(vec[0], vec[1], vec[2]);
    }

    glm::vec3 getVec3(const std::string& key, const glm::vec3& defaultValue) const {
        try {
            return getVec3(key);
        } catch (const std::exception& e) {
            spdlog::debug(e.what());
            return defaultValue;
        }
    }

    template<typename T>
    T set(const std::string& key, const T& value) {
        auto split = [](const std::string& s, char delim) -> std::vector<std::string> {
            std::vector<std::string> result;
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim)) {
                result.push_back(item);
            }
            return result;
        };
        std::vector<std::string> keys = split(key, '.');

        if (keys.size() == 1) {
            auto oldValue = config[keys[0]].as<T>();
            config[keys[0]] = value;
            return oldValue;
        } else if (keys.size() == 2) {
            auto oldValue = config[keys[0]][keys[1]].as<T>();
            config[keys[0]][keys[1]] = value;
            return oldValue;
        } else if (keys.size() == 3) {
            auto oldValue = config[keys[0]][keys[1]][keys[2]].as<T>();
            config[keys[0]][keys[1]][keys[2]] = value;
            return oldValue;
        } else if (keys.size() == 4) {
            auto oldValue = config[keys[0]][keys[1]][keys[2]][keys[3]].as<T>();
            config[keys[0]][keys[1]][keys[2]][keys[3]] = value;
            return oldValue;
        }
        spdlog::error("Invalid key: {}", key);
        throw std::runtime_error("Invalid key: " + key);
    }

private:
    YAML::Node config;

};

YAMLLoader& getGlobalConfig();

#endif // YAML_LOADER_H