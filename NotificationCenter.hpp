/***************************************************************************************
 * NotificationCenter.hpp
 * Feature: Simple per-user notification inbox (request/confirm messages).
 *
 * STANDARD LIBRARIES USED:
 *  <unordered_map> : per-email inbox
 *  <string>        : message text and email keys
 *  <vector>        : fetch results
 ****************************************************************************************/
#pragma once
#include <unordered_map>
#include <string>
#include <vector>

namespace sb {

class NotificationCenter {
public:
    // Push a notification to a user's inbox (keyed by email).
    void notify(const std::string& email, const std::string& message);

    // Retrieve all messages for 'email' and CLEAR the inbox.
    std::vector<std::string> fetchAndClear(const std::string& email);

    // Peek without clearing (useful for tests).
    std::vector<std::string> peek(const std::string& email) const;

private:
    std::unordered_map<std::string, std::vector<std::string>> inbox_;
};

} // namespace sb