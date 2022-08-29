#include "Log.h"



namespace Log {

    auto Info(const std::string &value) -> void {
        std::cout << WHITE << "[INFO] " << NO_COLOR << value << "\n";
    }

    auto Warn(const std::string &value) -> void {
        std::cout << YELLOW << "[WARN] " << NO_COLOR << value << "\n";
    }

    auto Error(const std::string &value) -> void {
        std::cout << RED << "[ERROR] " << NO_COLOR << value << "\n";
    }
}

