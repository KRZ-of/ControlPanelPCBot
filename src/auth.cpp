#include "auth.hpp"

Auth::Auth(const std::vector<int64_t>& allowedUsers)
    : allowedUsers_(allowedUsers) {
}

bool Auth::isAllowed(int64_t userId) const {
    for (const auto id : allowedUsers_) {
        if (id == userId) {
            return true;
        }
    }

    return false;
}