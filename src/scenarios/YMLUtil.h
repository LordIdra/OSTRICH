#pragma once

#include <util/Types.h>

#include <yaml-cpp/emitter.h>
#include <yaml-cpp/node/node.h>



namespace YMLUtil {

    enum ErrorCode {
        NONE,
        MISSING_KEY,
        MISSING_BASE_KEY,
        INCORRECT_TYPE,
        FILE_NOT_FOUND
    };

    auto SetCurrentError(const ErrorCode error) -> void;

    auto GetCurrentError() -> ErrorCode;

    auto CheckKeyExists(const YAML::Node &node, const string &path) -> bool;

    auto GetString(const YAML::Node &node, const string &path) -> string;
    auto GetInt   (const YAML::Node &node, const string &path) -> int;
    auto GetDouble(const YAML::Node &node, const string &path) -> double;
    auto GetVec3  (const YAML::Node &node, const string &path) -> vec3;

    auto SetVec3  (YAML::Emitter &emitter, const string &key, const vec3   &value) -> void;
    auto SetDouble(YAML::Emitter &emitter, const string &key, const double  value) -> void;
    auto SetString(YAML::Emitter &emitter, const string &key, const string &value) -> void;

}