cmake_minimum_required(VERSION 3.20)

project(pc-telegram-bot
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(
    external/tgbot-cpp
    ${CMAKE_BINARY_DIR}/tgbot-cpp
)

add_executable(pc-telegram-bot
    src/main.cpp
    src/bot.cpp
    src/commands.cpp
    src/system.cpp
    src/auth.cpp
)

target_include_directories(pc-telegram-bot
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(pc-telegram-bot
    PRIVATE
        TgBot::TgBot
        nlohmann_json::nlohmann_json
        Threads::Threads
)

target_compile_options(pc-telegram-bot
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

install(
    TARGETS pc-telegram-bot
    RUNTIME DESTINATION bin
)