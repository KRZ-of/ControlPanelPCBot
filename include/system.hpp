#pragma once

#include <string>

namespace System {

    bool execute(const std::string& command);

    bool command(const std::string& command);

    bool volumeUp(int count);
    bool volumeDown(int count);
    bool mute();

    bool playPause();
    bool next();
    bool previous();

    bool lock();
    bool workspace(int number);

}
