#pragma once

#include <string>

namespace System {

    bool execute(const std::string& command);

    bool volumeUp();
    bool volumeDown();
    bool mute();

    bool playPause();
    bool next();
    bool previous();

    bool lock();
    bool workspace(int number);

}
