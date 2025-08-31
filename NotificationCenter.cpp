/***************************************************************************************
 * NotificationCenter.cpp — implementation
 ****************************************************************************************/
#include "NotificationCenter.hpp"

namespace sb {

void NotificationCenter::notify(const std::string& email, const std::string& message) {
    inbox_[email].push_back(message);
}

std::vector<std::string> NotificationCenter::fetchAndClear(const std::string& email) {
    std::vector<std::string> out;
    auto it = inbox_.find(email);
    if (it != inbox_.end()) {
        out = std::move(it->second);
        inbox_.erase(it);
    }
    return out;
}

std::vector<std::string> NotificationCenter::peek(const std::string& email) const {
    auto it = inbox_.find(email);
    if (it == inbox_.end()) return {};
    return it->second;
}

} // namespace sb