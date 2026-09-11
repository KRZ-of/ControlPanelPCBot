#include "system.hpp"

#include <cstdlib>
#include <iostream>

namespace System {

bool execute(const std::string& command) {
    std::cout << "[SYSTEM] " << command << std::endl;

    const int result = std::system(command.c_str());

    return result == 0;
}

bool command(const std::string& command) {
    return execute(command);
}

bool volumeUp(int count) {
    if (count < 1 || count > 100) {
        return false;
    }

    return execute(
        "wpctl set-volume @DEFAULT_AUDIO_SINK@ " + std::to_string(count) + "%+"
    );
}

bool volumeDown(int count) {
    return execute(
        "wpctl set-volume @DEFAULT_AUDIO_SINK@ " + std::to_string(count) + "%-"
    );
}

bool mute() {
    return execute("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle");
}

bool playPause() {
    return execute("playerctl play-pause");
}

bool next() {
    return execute("playerctl next");
}

bool previous() {
    return execute("playerctl previous");
}

bool lock() {
    return execute("loginctl lock-session");
}

bool workspace(int number) {
    if (number < 1 || number > 20) {
        return false;
    }

    return execute(
        "hyprctl dispatch workspace " + std::to_string(number)
    );
}

}