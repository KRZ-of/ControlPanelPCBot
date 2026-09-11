#include "bot.hpp"
#include "commands.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {

json loadConfig() {
    std::ifstream file("config/config.json");

    if (!file.is_open()) {
        throw std::runtime_error(
            "Не удалось открыть config/config.json"
        );
    }

    json config;
    file >> config;

    return config;
}

void sendMessage(
    TgBot::Bot& bot,
    int64_t chatId,
    const std::string& text
) {
    TgBot::SendMessageArgs args;

    args.chatId = chatId;
    args.text = text;
    args.parseMode = "HTML";

    bot.getApi().sendMessage(args);
}

std::string buildMenu(const json& commands) {
    std::stringstream result;

    result << "🖥<b>PC Control</b>\n\n";

    for (const auto& command : commands) {
        const std::string name =
            command.value("name", "");

        const std::string description =
            command.value("description", "");

        if (name.empty()) {
            continue;
        }

        if (name == "start") {
            continue;
        }

        result << "<code>/" << name << "</code>"
               << " — "
               << description
               << "\n";
    }

    return result.str();
}

}

BotController::BotController(
    const std::string& token,
    const Auth& auth
)
    : bot_(token),
      auth_(auth) {
}

void BotController::registerCommands() {
    const json config = loadConfig();

    if (!config.contains("commands")) {
        throw std::runtime_error(
            "В config.json отсутствует секция commands"
        );
    }

    const auto& commands = config["commands"];

    for (const auto& command : commands) {

        const std::string name =
            command.value("name", "");

        const std::string description =
            command.value("description", "");

        const std::string action =
            command.value("action", "");

        if (name.empty()) {
            std::cerr
                << "[BOT] Пропущена команда без name"
                << std::endl;

            continue;
        }

        if (action.empty()) {
            std::cerr
                << "[BOT] Команда /"
                << name
                << " не имеет action"
                << std::endl;

            continue;
        }

        bot_.getEvents().onCommand(
            name,
            [this, name, action, commands](
                TgBot::Message::Ptr message
            ) {

                if (!message || !message->from) {
                    return;
                }

                const int64_t userId =
                    message->from->id;

                std::cout
                    << "[BOT] /"
                    << name
                    << " from user "
                    << userId
                    << std::endl;

                /*
                 * Проверяем разрешённого пользователя
                 */
                if (!auth_.isAllowed(userId)) {

                    std::cout
                        << "[AUTH] Access denied for "
                        << userId
                        << std::endl;

                    sendMessage(
                        bot_,
                        message->chat->id,
                        "⛔ Доступ запрещён."
                    );

                    return;
                }

                /*
                 * Главное меню
                 */
                if (action == "menu") {

                    sendMessage(
                        bot_,
                        message->chat->id,
                        buildMenu(commands)
                    );

                    return;
                }

                /*
                 * Получаем аргумент команды.
                 *
                 * Например:
                 *
                 * /workspace 3
                 *
                 * argument = "3"
                 */
                std::string argument;

                if (message->text) {

                    const std::string prefix =
                        "/" + name;

                    if (message->text->size() > prefix.size()) {

                        argument =
                            message->text->substr(
                                prefix.size()
                            );

                        /*
                         * Убираем пробелы в начале
                         */
                        while (
                            !argument.empty() &&
                            argument.front() == ' '
                        ) {
                            argument.erase(
                                argument.begin()
                            );
                        }
                    }
                }

                /*
                 * Выполняем действие
                 */
                const bool success =
                    CommandHandler::execute(
                        action,
                        argument
                    );

                if (success) {

                    sendMessage(
                        bot_,
                        message->chat->id,
                        "✅ Выполнено."
                    );

                } else {

                    sendMessage(
                        bot_,
                        message->chat->id,
                        "❌ Не удалось выполнить команду."
                    );
                }
            }
        );

        std::cout
            << "[BOT] Registered /"
            << name
            << " -> "
            << action
            << " | "
            << description
            << std::endl;
    }
}

void BotController::run() {

    try {

        const auto me =
            bot_.getApi().getMe();

        std::cout
            << "[BOT] Username: "
            << me->username.value_or("")
            << std::endl;

        TgBot::TgLongPoll longPoll(bot_);

        std::cout
            << "[BOT] Started."
            << std::endl;

        longPoll.startLoop();

    } catch (const TgBot::TgException& e) {

        std::cerr
            << "[BOT] Telegram error: "
            << e.what()
            << std::endl;

        throw;
    }
}