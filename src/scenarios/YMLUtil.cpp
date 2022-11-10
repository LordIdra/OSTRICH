#include "YMLUtil.h"

#include <yaml-cpp/yaml.h>



namespace YMLUtil {
    namespace {
        ErrorCode currentError;
    }

    auto SetCurrentError(const ErrorCode error) -> void {
        currentError = error;
    }

    auto GetCurrentError() -> ErrorCode {
        return currentError;
    }

    auto CheckKeyExists(const YAML::Node &node, const string &path) -> bool {
        if (!node[path]) {
            currentError = MISSING_KEY;
            return false;
        }
        return true;
    }

    auto GetString(const YAML::Node &node, const string &path) -> string {
        if (!node[path]) {
            currentError = MISSING_KEY;
            return "";
        }
        return node[path].as<string>();
    }

    auto GetInt(const YAML::Node &node, const string &path) -> int {
        if (!node[path]) {
            currentError = MISSING_KEY;
            return 0;
        }
        return node[path].as<int>();
    }

    auto GetDouble(const YAML::Node &node, const string &path) -> double {
        if (!node[path]) {
            currentError = MISSING_KEY;
            return 0.0;
        }

        return node[path].as<double>();
    }

    auto GetVec3(const YAML::Node &node, const string &path) -> vec3 {
        if (!node[path]) {
            currentError = MISSING_KEY;
            return vec3();
        }

        if (!node[path].IsSequence()) {
            currentError = INCORRECT_TYPE;
            return vec3();
        }

        float x = node[path][0].as<double>();
        float y = node[path][1].as<double>();
        float z = node[path][2].as<double>();

        return vec3(x, y, z);
    }

    auto SetString(YAML::Emitter &emitter, const string &key, const string &value) -> void {
        emitter << key << value;
    }

    auto SetDouble(YAML::Emitter &emitter, const string &key, const double value) -> void {
        emitter << key << value;
    }

    auto SetVec3(YAML::Emitter &emitter, const string &key, const vec3 &value) -> void {
        emitter << key << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
    }
}