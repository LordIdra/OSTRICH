#pragma once

#include <util/Types.h>



auto FileExists(const string &path) -> bool;
auto ReadFile(const string &path) -> string;

auto PrintCompileLog(const unsigned int id) -> void;
auto PrintLinkLog(const unsigned int id) -> void;

auto CheckCompileSuccess(const string &path, const unsigned int id) -> void;
auto CheckLinkSuccess(const unsigned int id) -> void;
