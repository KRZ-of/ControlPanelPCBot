#pragma once

#include <cstdint>
#include <vector>

class Auth {
public:
    explicit Auth(const std::vector<int64_t>& allowedUsers);

    bool isAllowed(int64_t userId) const;

private:
    std::vector<int64_t> allowedUsers_;
};
