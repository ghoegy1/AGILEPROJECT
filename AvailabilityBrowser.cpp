/***************************************************************************************
 * AvailabilityBrowser.cpp — implementation (updated to drop empty-day matches)
 ****************************************************************************************/
#include "AvailabilityBrowser.hpp"
#include "Utils.hpp"
#include <algorithm>  // remove_if

namespace sb {

bool AvailabilityBrowser::sameUser(const Profile& a, const Profile& b) {
    // Prefer email identity if present; fallback to name; final fallback: address equality
    if (!a.email().empty() && !b.email().empty()) return a.email() == b.email();
    if (!a.name().empty() && !b.name().empty())   return a.name() == b.name();
    return &a == &b;
}

bool AvailabilityBrowser::hasCourse(const Profile& p, const std::string& normalizedCourse) {
    for (const auto& c : p.courses()) {
        if (c == normalizedCourse) return true;
    }
    return false;
}

std::vector<std::pair<std::string, std::vector<AvailabilitySlot>>>
AvailabilityBrowser::browseByCourse(const std::vector<Profile>& all,
                                    const Profile& self,
                                    const std::string& courseCode) {
    std::vector<std::pair<std::string, std::vector<AvailabilitySlot>>> out;
    std::string norm = upperCopy(trim(courseCode));
    for (const auto& p : all) {
        if (sameUser(p, self)) continue;
        if (hasCourse(p, norm)) {
            out.push_back({p.name().empty() ? p.email() : p.name(), p.availability()});
        }
    }
    return out;
}

std::vector<std::pair<std::string, std::vector<AvailabilitySlot>>>
AvailabilityBrowser::browseByCourseAndDay(const std::vector<Profile>& all,
                                          const Profile& self,
                                          const std::string& courseCode,
                                          Day day) {
    // Start with all classmates in the course
    auto base = browseByCourse(all, self, courseCode);

    // Filter each classmate's slots to the chosen day; keep only if any remain
    std::vector<std::pair<std::string, std::vector<AvailabilitySlot>>> out;
    out.reserve(base.size());
    for (auto& entry : base) {
        auto& slots = entry.second;
        slots.erase(std::remove_if(slots.begin(), slots.end(),
                   [day](const AvailabilitySlot& s){ return s.day != day; }),
                   slots.end());
        if (!slots.empty()) {
            out.push_back(std::move(entry));
        }
    }
    return out;
}

} // namespace sb