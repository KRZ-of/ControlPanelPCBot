#include "commands.hpp"
#include "system.hpp"

#include <iostream>

bool CommandHandler::execute(
    const std::string& action,
    const std::string& argument
) {
    if (action == "menu") {
        return true;
    }

    if (action == "volume_up") {
        return System::volumeUp();
    }

    if (action == "volume_down") {
        return System::volumeDown();
    }

    if (action == "mute") {
        return System::mute();
    }

    if (action == "play_pause") {
        return System::playPause();
    }

    if (action == "next") {
        return System::next();
    }

    if (action == "previous") {
        return System::previous();
    }

    if (action == "lock") {
        return System::lock();
    }

    if (action == "workspace") {
        try {
            const int number = std::stoi(argument);
            return System::workspace(number);
        } catch (...) {
            return false;
        }
    }

    std::cerr << "[COMMAND] Unknown action: "
              << action << std::endl;

    return false;
}