#include "./yamlLoader.h"

static YAMLLoader globalConfig;

YAMLLoader& getGlobalConfig() {
    return globalConfig;
}