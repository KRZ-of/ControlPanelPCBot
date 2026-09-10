#pragma once

#include <memory>
#include <string>

#include <tgbot/tgbot.h>

#include "auth.hpp"

class BotController {
public:
    BotController(
        const std::string& token,
        const Auth& auth
    );

    void registerCommands();
    void run();

private:
    TgBot::Bot bot_;
    const Auth& auth_;
};