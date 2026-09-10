#pragma once

#include <string>

class CommandHandler {
public:
    static bool execute(const std::string& action,
                        const std::string& argument = "");
};
