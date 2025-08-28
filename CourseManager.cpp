/***************************************************************************************
 * CourseManager.cpp — implementation
 ****************************************************************************************/
#include "CourseManager.hpp"
#include "Profile.hpp"
#include "Utils.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

namespace sb {

std::vector<std::string> CourseManager::normalizeDedup(const std::vector<std::string>& raw) {
    std::vector<std::string> tmp;
    tmp.reserve(raw.size());
    for (auto t : raw) {
        t = sb::trim(t);
        if (!t.empty()) {
            tmp.push_back(sb::upperCopy(t));
        }
    }
    std::vector<std::string> out;
    for (const auto& c : tmp) {
        if (std::find(out.begin(), out.end(), c) == out.end()) {
            out.push_back(c);
        }
    }
    return out;
}

void CourseManager::addCourses(Profile& prof, const std::string& commaSeparated) {
    if (!prof.exists()) { std::cout << "Create a profile first.\n"; return; }
    auto tokens = sb::split(commaSeparated, ',');
    int added = 0;
    auto& list = prof.coursesMutable();
    for (auto t : tokens) {
        if (t.empty()) continue;
        t = sb::upperCopy(t);
        if (std::find(list.begin(), list.end(), t) == list.end()) {
            list.push_back(t);
            ++added;
        }
    }
    if (added == 0) std::cout << "No new courses were added (duplicates or empty input).\n";
    else            std::cout << "Added " << added << " course(s).\n";
}

void CourseManager::removeCourses(Profile& prof, const std::string& commaSeparated) {
    if (!prof.exists()) { std::cout << "Create a profile first.\n"; return; }
    auto& list = prof.coursesMutable();
    if (list.empty()) { std::cout << "No courses to remove.\n"; return; }

    auto tokens = sb::split(commaSeparated, ',');
    std::vector<size_t> idx;
    for (auto& t : tokens) {
        if (t.empty()) continue;
        try {
            int oneBased = std::stoi(t);
            if (oneBased >= 1 && static_cast<size_t>(oneBased) <= list.size()) {
                idx.push_back(static_cast<size_t>(oneBased - 1));
            }
        } catch (...) { /* ignore bad tokens */ }
    }
    if (idx.empty()) { std::cout << "No valid indices provided.\n"; return; }

    std::sort(idx.begin(), idx.end());
    idx.erase(std::unique(idx.begin(), idx.end()), idx.end());
    for (auto it = idx.rbegin(); it != idx.rend(); ++it) {
        list.erase(list.begin() + *it);
    }
    std::cout << "Removed " << idx.size() << " course(s).\n";
}

} // namespace sb