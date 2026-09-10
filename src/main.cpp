#include "auth.hpp"
#include "bot.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    try {
        std::ifstream file("config/config.json");

        if (!file.is_open()) {
            std::cerr
                << "Ошибка: config/config.json не найден."
                << std::endl;

            return 1;
        }

        json config;
        file >> config;

        const std::string token =
            config["telegram"]["token"];

        if (token.empty() ||
            token == "PASTE_YOUR_BOT_TOKEN_HERE") {

            std::cerr
                << "Ошибка: укажи Telegram Bot Token "
                   "в config/config.json"
                << std::endl;

            return 1;
        }

        std::vector<int64_t> allowedUsers =
            config["telegram"]["allowed_user_ids"]
            .get<std::vector<int64_t>>();

        Auth auth(allowedUsers);

        BotController bot(token, auth);

        bot.registerCommands();
        bot.run();

    } catch (const std::exception& e) {
        std::cerr
            << "[FATAL] "
            << e.what()
            << std::endl;

        return 1;
    }

    return 0;
}